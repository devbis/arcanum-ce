#include "tig/sound.h"

#include <stdio.h>

#include <mss_compat.h>

#include "tig/cache.h"
#include "tig/debug.h"
#include "tig/file.h"
#include "tig/tig.h"
#include "tig/timer.h"

#define FIRST_VOICE_HANDLE 0
#define FIRST_MUSIC_HANDLE 2
#define FIRST_EFFECT_HANDLE 6
#define SOUND_HANDLE_MAX 60

typedef struct TigSound {
    /* 0000 */ unsigned char active; // boolean
    /* 0004 */ unsigned int flags;
    /* 0008 */ int fade_duration;
    /* 000C */ int fade_step;
    /* 0010 */ int loops;
    /* 0014 */ HSTREAM audio_stream;
    /* 0018 */ HAUDIO audio_handle;
    /* 001C */ int next_sound_handle;
    /* 0020 */ char path[MAX_PATH];
    /* 0124 */ int id;
    /* 0128 */ int volume;
    /* 012C */ int extra_volume;
    /* 0130 */ TigCacheEntry* cache_entry;
    /* 0134 */ bool field_134;
    /* 0138 */ int64_t field_138;
    /* 0140 */ int64_t field_140;
    /* 0148 */ int field_148;
} TigSound;

static_assert(sizeof(TigSound) == 0x150, "wrong size");

static void tig_sound_update();
static int tig_sound_acquire_handle(int type);
static void tig_sound_reset_sound(TigSound* sound);

// 0x5C246C
static int tig_sound_type_flags[TIG_SOUND_TYPE_COUNT] = {
    TIG_SOUND_EFFECT,
    TIG_SOUND_MUSIC,
    TIG_SOUND_VOICE,
};

// 0x62B2C0
static TigSoundFileNameFunc* tig_sound_path_func;

// 0x62B328
static TigSound tig_sounds[SOUND_HANDLE_MAX];

// 0x6301E8
static int tig_sound_next_effect_handle;

// 0x6301EC
static bool tig_sound_initialized;

// 0x6301F0
static TigCache* tig_sound_cache;

// 0x6301F4
static int tig_sound_effects_volume;

// 0x532D40
int tig_sound_init(TigContext* ctx)
{
    // COMPAT: Load `mss32.dll`.
    mss_compat_init();

    if ((ctx->flags & TIG_CONTEXT_HAVE_MSS_REDIST_PATH) != 0
        && ctx->mss_redist_path != NULL) {
        AIL_set_redist_directory(ctx->mss_redist_path);
    }

    tig_sound_initialized = false;
    tig_sound_next_effect_handle = FIRST_EFFECT_HANDLE;

    if (AIL_quick_startup(1, 0, 22050, 16, 2)) {
        tig_sound_initialized = true;
    }

    tig_sound_set_path_func(ctx->sound_file_path_resolver);

    // Create a file cache for 20 files, approx. 1 MB total.
    tig_sound_cache = tig_cache_create(20, 1000000);

    return TIG_OK;
}

// 0x532DB0
void tig_sound_exit()
{
    if (tig_sound_initialized) {
        tig_sound_initialized = false;
        tig_sound_fade_out_all(0);
        tig_cache_destroy(tig_sound_cache);
        AIL_quick_shutdown();
    }

    // COMPAT: Unload `mss32.dll`.
    mss_compat_exit();
}

// 0x532DE0
void tig_sound_ping()
{
    // 0x739E84
    static timer_t tig_sound_ping_time;

    if (!tig_sound_initialized) {
        return;
    }

    if (tig_ping_time < tig_sound_ping_time - 1000) {
        tig_sound_ping_time = tig_ping_time;
    }

    // TODO: Looks odd, check.
    if (tig_ping_time > tig_sound_ping_time + 1000) {
        tig_sound_ping_time = tig_ping_time + 100;
        tig_sound_update();
    } else if (tig_ping_time >= tig_sound_ping_time) {
        tig_sound_ping_time += 100;
        tig_sound_update();
    }
}

// 0x532E30
void tig_sound_update()
{
    int index;
    TigSound* snd;
    int new_volume;

    if (!tig_sound_initialized) {
        return;
    }

    for (index = 0; index < SOUND_HANDLE_MAX; index++) {
        snd = &(tig_sounds[index]);
        if (snd->active != 0 && (snd->flags & TIG_SOUND_WAIT) == 0) {
            if ((snd->flags & TIG_SOUND_FADE_OUT) != 0) {
                snd->fade_step++;
                if (snd->fade_step <= snd->fade_duration) {
                    new_volume = snd->volume * (snd->fade_duration - snd->fade_step) / snd->fade_duration;
                } else {
                    snd->flags &= ~TIG_SOUND_FADE_OUT;
                    if (snd->next_sound_handle >= 0) {
                        TigSound* next_snd = &(tig_sounds[snd->next_sound_handle]);
                        next_snd->flags &= ~TIG_SOUND_WAIT;
                        next_snd->flags |= TIG_SOUND_FADE_IN;
                    }
                    new_volume = 0;
                }

                if ((snd->flags & TIG_SOUND_STREAMED) != 0) {
                    AIL_set_stream_volume(snd->audio_stream, (float)new_volume);
                    snd->flags |= TIG_SOUND_STOP;
                } else if ((snd->flags & TIG_SOUND_MEMORY) != 0) {
                    AIL_quick_set_volume(snd->audio_handle, (float)(new_volume * snd->volume / 128), (float)snd->extra_volume);
                    snd->flags |= TIG_SOUND_STOP;
                } else {
                    snd->flags |= TIG_SOUND_STOP;
                }
            } else if ((snd->flags & TIG_SOUND_FADE_IN) != 0) {
                if (snd->fade_step == 0) {
                    AIL_start_stream(snd->audio_stream);
                }

                snd->fade_step++;

                new_volume = snd->volume;
                if (snd->fade_step <= snd->fade_duration) {
                    new_volume = snd->fade_step * snd->volume / snd->fade_duration;
                } else {
                    snd->flags &= ~TIG_SOUND_FADE_IN;
                }

                if ((snd->flags & TIG_SOUND_STREAMED) != 0) {
                    AIL_set_stream_volume(snd->audio_stream, (float)new_volume);
                } else if ((snd->flags & TIG_SOUND_MEMORY) != 0) {
                    AIL_quick_set_volume(snd->audio_handle, (float)new_volume, 64);
                }
            } else {
                if ((snd->flags & TIG_SOUND_MEMORY) != 0) {
                    if (AIL_quick_status(snd->audio_handle) == QSTAT_DONE) {
                        snd->flags |= TIG_SOUND_STOP;
                    }
                }

                if ((snd->flags & TIG_SOUND_STOP) != 0) {
                    tig_sound_stop(index);
                }
            }
        }
    }
}

// 0x533000
void tig_sound_set_path_func(TigSoundFileNameFunc* func)
{
    tig_sound_path_func = func;
}

// 0x533010
bool tig_sound_is_initialized()
{
    return tig_sound_initialized;
}

// 0x533020
void tig_sound_fade_out(int sound_handle, int fade_duration)
{
    TigSound* snd;

    if (!tig_sound_initialized) {
        return;
    }

    if (!(sound_handle >= 0 && sound_handle < SOUND_HANDLE_MAX)) {
        return;
    }

    snd = &(tig_sounds[sound_handle]);
    snd->flags &= ~(TIG_SOUND_WAIT | TIG_SOUND_FADE_IN);
    if ((snd->flags & TIG_SOUND_FADE_OUT) == 0) {
        snd->flags |= TIG_SOUND_FADE_OUT;
        snd->fade_duration = abs(fade_duration);
        snd->fade_step = 0;
    }
}

// NOTE: Purpose is unclear, used only from `tig_sound_stop`.
//
// 0x533080
void tig_sound_fade_out_from_stop(int sound_handle, int fade_duration)
{
    tig_sound_fade_out(sound_handle, fade_duration);
}

// 0x5330A0
void tig_sound_fade_out_all(int fade_duration)
{
    int index;

    if (!tig_sound_initialized) {
        return;
    }

    for (index = 0; index < SOUND_HANDLE_MAX; index++) {
        if (tig_sounds[index].active != 0) {
            tig_sound_fade_out(index, fade_duration);
        }
    }

    tig_sound_next_effect_handle = FIRST_EFFECT_HANDLE;

    tig_sound_update();
}

// 0x5330F0
int tig_sound_allocate(int* sound_handle, int type)
{
    if (!tig_sound_initialized) {
        *sound_handle = -1;
        return TIG_OK;
    }

    *sound_handle = tig_sound_acquire_handle(type);
    if (*sound_handle != -1) {
        tig_sounds[*sound_handle].flags |= tig_sound_type_flags[type];
        tig_sounds[*sound_handle].loops = 1;
        tig_sounds[*sound_handle].volume = 127;
        tig_sounds[*sound_handle].extra_volume = 64;
    }

    return TIG_OK;
}

// 0x5331A0
int tig_sound_acquire_handle(int type)
{
    int index;
    TigSound* snd;

    switch (type) {
    case TIG_SOUND_TYPE_EFFECT:
        for (index = 0; index < SOUND_HANDLE_MAX; index++) {
            snd = &(tig_sounds[tig_sound_next_effect_handle]);
            if (snd->active == 0) {
                tig_sound_reset_sound(snd);
                snd->active = 1;
                return tig_sound_next_effect_handle;
            }

            if (++tig_sound_next_effect_handle >= SOUND_HANDLE_MAX) {
                tig_sound_next_effect_handle = FIRST_EFFECT_HANDLE;
            }
        }
        break;
    case TIG_SOUND_TYPE_MUSIC:
        for (index = FIRST_MUSIC_HANDLE; index < FIRST_EFFECT_HANDLE; index++) {
            snd = &(tig_sounds[index]);
            if (snd->active == 0) {
                tig_sound_reset_sound(snd);
                snd->active = 1;
                return index;
            }
        }
        break;
    case TIG_SOUND_TYPE_VOICE:
        for (index = FIRST_VOICE_HANDLE; index < FIRST_MUSIC_HANDLE; index++) {
            snd = &(tig_sounds[index]);
            if (snd->active == 0) {
                tig_sound_reset_sound(snd);
                snd->active = 1;
                return index;
            }
        }
        break;
    }

    tig_debug_printf("No sound handle available for sound type %d! Current sounds are:\n");
    for (index = 0; index < SOUND_HANDLE_MAX; index++) {
        // FIXME: This approach does not respect lower and upper bounds for the
        // requested sound type. Let's say we're out of music handles, it will
        // dump voice and sound handles, which (1) cannot store music handles,
        // and (2) can be empty or obsolete value (no check for `active`).
        tig_debug_printf("%s\n", tig_sounds[index].path);
    }

    return -1;
}

// 0x5332F0
void tig_sound_reset_sound(TigSound* sound)
{
    memset(sound, 0, sizeof(*sound));
    sound->next_sound_handle = -1;
}

// 0x533310
void tig_sound_stop(int sound_handle)
{
    TigSound* snd;

    if (!tig_sound_initialized) {
        return;
    }

    if (!(sound_handle >= 0 && sound_handle < SOUND_HANDLE_MAX)) {
        return;
    }

    snd = &(tig_sounds[sound_handle]);
    if (snd->active != 0) {
        tig_sound_fade_out_from_stop(sound_handle, 0);

        if ((snd->flags & TIG_SOUND_STREAMED) != 0) {
            AIL_close_stream(snd->audio_stream);
            snd->active = 0;
        } else if ((snd->flags & TIG_SOUND_MEMORY) != 0) {
            AIL_quick_unload(snd->audio_handle);
            tig_cache_release(tig_sound_cache, snd->cache_entry);
            snd->active = 0;
        } else {
            snd->active = 0;
        }
    }
}

// 0x5333A0
int tig_sound_play_by_path(int sound_handle, const char* path, int id)
{
    TigSound* snd;

    if (!tig_sound_initialized) {
        return TIG_OK;
    }

    // FIXME: No `SOUND_HANDLE_MAX` guard.
    if (!(sound_handle >= 0)) {
        return TIG_OK;
    }

    snd = &(tig_sounds[sound_handle]);
    strcpy(snd->path, path);
    snd->cache_entry = tig_cache_acquire(tig_sound_cache, path);

    if (snd->cache_entry->data != NULL) {
        snd->audio_handle = AIL_quick_load_mem(snd->cache_entry->data, snd->cache_entry->size);
        AIL_quick_set_volume(snd->audio_handle, (float)snd->volume, (float)snd->extra_volume);
        AIL_quick_play(snd->audio_handle, snd->loops);
        snd->flags |= TIG_SOUND_MEMORY;
        snd->id = id;
    } else {
        snd->active = 0;
    }

    return TIG_OK;
}

// 0x533480
int tig_sound_play_by_id(int sound_handle, int id)
{
    char path[MAX_PATH];

    if (!tig_sound_initialized) {
        return TIG_OK;
    }

    tig_sound_path_func(id, path);

    return tig_sound_play_by_path(sound_handle, path, id);
}

// 0x5334D0
int tig_sound_play_streamed(int sound_handle, const char* name, int loops, int fade_duration, int prev_sound_handle)
{
    char path[MAX_PATH];
    TigSound* snd;
    HDIGDRIVER dig;
    TigSound* prev_snd;

    if (!tig_sound_initialized) {
        return TIG_OK;
    }

    if (!tig_file_extract(name, path)) {
        return TIG_OK;
    }

    if (!(sound_handle >= 0 && sound_handle < SOUND_HANDLE_MAX)) {
        return TIG_OK;
    }

    snd = &(tig_sounds[sound_handle]);
    snd->fade_duration = abs(fade_duration);
    snd->fade_step = 0;

    AIL_quick_handles(&dig, NULL, NULL);

    snd->flags |= TIG_SOUND_STREAMED;
    snd->audio_stream = AIL_open_stream(dig, path, 0);
    if (snd->audio_stream == NULL) {
        snd->active = 0;
        return TIG_OK;
    }

    strcpy(snd->path, name);
    snd->loops = loops;

    AIL_set_stream_loop_count(snd->audio_stream, loops);

    if (fade_duration != 0 && prev_sound_handle >= 0 && fade_duration > 0) {
        snd->flags |= TIG_SOUND_WAIT;
    } else {
        snd->flags |= TIG_SOUND_FADE_IN;
    }

    if (prev_sound_handle >= 0) {
        prev_snd = &(tig_sounds[prev_sound_handle]);
        if ((prev_snd->flags & TIG_SOUND_FADE_IN) != 0) {
            prev_snd->flags &= ~TIG_SOUND_FADE_IN;
        }

        if ((prev_snd->flags & TIG_SOUND_WAIT) != 0) {
            prev_snd->flags &= ~TIG_SOUND_WAIT;
        }

        prev_snd->flags |= TIG_SOUND_FADE_OUT;
        prev_snd->fade_duration = abs(fade_duration);
        prev_snd->fade_step = 0;
        prev_snd->next_sound_handle = sound_handle;
    }

    return TIG_OK;
}

// 0x533680
int tig_sound_play_streamed_indefinitely(int sound_handle, const char* name, int fade_duration, int prev_sound_handle)
{
    return tig_sound_play_streamed(sound_handle, name, 0, fade_duration, prev_sound_handle);
}

// 0x5336A0
int tig_sound_play_streamed_once(int sound_handle, const char* name, int fade_duration, int prev_sound_handle)
{
    return tig_sound_play_streamed(sound_handle, name, 1, fade_duration, prev_sound_handle);
}

// 0x5336C0
int tig_sound_get_loops(int sound_handle)
{
    if (!tig_sound_initialized) {
        // NOTE: Probably `TIG_NOT_INITIALIZED`.
        return 1;
    }

    if (!(sound_handle >= 0 && sound_handle < SOUND_HANDLE_MAX)) {
        return 0;
    }

    return tig_sounds[sound_handle].loops;
}

// 0x533700
void tig_sound_set_loops(int sound_handle, int loops)
{
    if (!tig_sound_initialized) {
        return;
    }

    if (!(sound_handle >= 0 && sound_handle < SOUND_HANDLE_MAX)) {
        return;
    }

    tig_sounds[sound_handle].loops = loops;
}

// 0x533730
int tig_sound_get_volume(int sound_handle)
{
    if (!tig_sound_initialized) {
        return 0;
    }

    if (!(sound_handle >= 0 && sound_handle < SOUND_HANDLE_MAX)) {
        return 0;
    }

    return tig_sounds[sound_handle].volume;
}

// 0x533760
void tig_sound_set_volume(int sound_handle, int volume)
{
    TigSound* snd;

    if (!tig_sound_initialized) {
        return;
    }

    if (!(sound_handle >= 0 && sound_handle < SOUND_HANDLE_MAX)) {
        return;
    }

    snd = &(tig_sounds[sound_handle]);
    if (snd->volume != volume) {
        snd->volume = volume;

        if ((snd->flags & TIG_SOUND_STREAMED) != 0) {
            AIL_set_stream_volume(snd->audio_stream, (float)volume);
        } else if ((snd->flags & TIG_SOUND_MEMORY) != 0) {
            AIL_quick_set_volume(snd->audio_stream, (float)volume, (float)snd->extra_volume);
        }
    }
}

// 0x5337D0
void tig_sound_set_volume_by_type(int type, int volume)
{
    int index;
    TigSound* snd;

    if (!tig_sound_initialized) {
        return;
    }

    for (index = 0; index < SOUND_HANDLE_MAX; index++) {
        snd = &(tig_sounds[index]);
        if (snd->active != 0) {
            if ((tig_sound_type_flags[type] & snd->flags) != 0) {
                tig_sound_set_volume(index, volume);
            }
        }
    }

    if (type == TIG_SOUND_TYPE_EFFECT) {
        tig_sound_set_effects_volume(volume);
    }
}

// 0x533830
int tig_sound_get_type(int sound_handle)
{
    int type;

    if (!tig_sound_initialized) {
        return 0;
    }

    if (!(sound_handle >= 0 && sound_handle < SOUND_HANDLE_MAX)) {
        return 0;
    }

    for (type = 0; type < TIG_SOUND_TYPE_COUNT; type++) {
        if ((tig_sounds[sound_handle].flags & tig_sound_type_flags[type]) != 0) {
            return type;
        }
    }

    // NOTE: Probably should be -1 to denote error.
    return 0;
}

// 0x533880
void tig_sound_set_type(int sound_handle, int type)
{
    TigSound* snd;

    if (!tig_sound_initialized) {
        return;
    }

    if (!(sound_handle >= 0 && sound_handle < SOUND_HANDLE_MAX)) {
        return;
    }

    snd = &(tig_sounds[sound_handle]);
    snd->flags &= ~(TIG_SOUND_EFFECT | TIG_SOUND_MUSIC | TIG_SOUND_VOICE);
    snd->flags |= tig_sound_type_flags[type];
}

// 0x5338D0
int sub_5338D0(int sound_handle)
{
    if (!tig_sound_initialized) {
        return 2;
    }

    if (!(sound_handle >= 0 && sound_handle < SOUND_HANDLE_MAX)) {
        return 2;
    }

    return tig_sounds[sound_handle].field_148;
}

// 0x533910
void sub_533910(int sound_handle, int value)
{
    if (!tig_sound_initialized) {
        return;
    }

    if (!(sound_handle >= 0 && sound_handle < SOUND_HANDLE_MAX)) {
        return;
    }

    tig_sounds[sound_handle].field_148 = value;
}

// 0x533940
int tig_sound_get_extra_volume(int sound_handle)
{
    if (!tig_sound_initialized) {
        return 64;
    }

    if (!(sound_handle >= 0 && sound_handle < SOUND_HANDLE_MAX)) {
        return 64;
    }

    return tig_sounds[sound_handle].extra_volume;
}

// 0x533980
void tig_sound_set_extra_volume(int sound_handle, int extra_volume)
{
    TigSound* snd;

    if (!tig_sound_initialized) {
        return;
    }

    if (!(sound_handle >= 0 && sound_handle < SOUND_HANDLE_MAX)) {
        return;
    }

    snd = &(tig_sounds[sound_handle]);
    if (snd->extra_volume != extra_volume) {
        snd->extra_volume = extra_volume;

        if ((snd->flags & TIG_SOUND_STREAMED) == 0
            && (snd->flags & TIG_SOUND_MEMORY) != 0) {
            AIL_quick_set_volume(snd->audio_handle, (float)snd->volume, (float)extra_volume);
        }
    }
}

// 0x5339E0
bool tig_sound_is_active_id(int id)
{
    int index;
    TigSound* snd;

    if (!tig_sound_initialized) {
        return false;
    }

    for (index = 0; index < SOUND_HANDLE_MAX; index++) {
        snd = &(tig_sounds[index]);
        if (snd->active != 0 && snd->id == id) {
            return true;
        }
    }

    return false;
}

// 0x533A20
bool tig_sound_is_playing(int sound_handle)
{
    TigSound* snd;

    // FIXME: No `tig_sound_initialized` guard.

    if (!(sound_handle >= 0 && sound_handle < SOUND_HANDLE_MAX)) {
        return false;
    }

    snd = &(tig_sounds[sound_handle]);
    if (snd->active != 0) {
        if ((snd->flags & TIG_SOUND_MEMORY) != 0) {
            if (AIL_quick_status(snd->audio_handle) == QSTAT_PLAYING) {
                return true;
            }
        }

        if ((snd->flags & TIG_SOUND_STREAMED) != 0) {
            if (AIL_stream_status(snd->audio_stream) == SMP_PLAYING) {
                return true;
            }
        }
    }

    return false;
}

// 0x533A90
bool tig_sound_is_active(int sound_handle)
{
    // FIXME: No `tig_sound_initialized` guard.

    if (!(sound_handle >= 0 && sound_handle < SOUND_HANDLE_MAX)) {
        return false;
    }

    return tig_sounds[sound_handle].active != 0;
}

// 0x533AC0
void tig_sound_cache_flush()
{
    if (tig_sound_initialized) {
        tig_cache_flush(tig_sound_cache);
    }
}

// 0x533AE0
const char* tig_sound_cache_stats()
{
    // 0x62B2C4
    static char buffer[100];

    sprintf(buffer,
        "Sound Cache: %u items, %u bytes",
        tig_sound_cache->items_count,
        tig_sound_cache->bytes);
    return buffer;
}

// 0x533B10
void sub_533B10(int sound_handle, int64_t a2, int64_t a3)
{
    TigSound* snd;

    // FIXME: No `tig_sound_initialized` guard.

    if (!(sound_handle >= 0 && sound_handle < SOUND_HANDLE_MAX)) {
        return;
    }

    snd = &(tig_sounds[sound_handle]);
    snd->field_138 = a2;
    snd->field_140 = a3;
    snd->field_134 = true;
}

// 0x533B60
void sub_533B60(int sound_handle, int64_t* a2, int64_t* a3)
{
    TigSound* snd;

    // FIXME: No `tig_sound_initialized` guard.

    if (!(sound_handle >= 0 && sound_handle < SOUND_HANDLE_MAX)) {
        return;
    }

    snd = &(tig_sounds[sound_handle]);
    if (snd->active != 0 && snd->field_134) {
        *a2 = snd->field_138;
        *a3 = snd->field_140;
    }
}

// 0x533BC0
bool sub_533BC0(int sound_handle)
{
    TigSound* snd;

    // FIXME: No `tig_sound_initialized` guard.

    if (!(sound_handle >= 0 && sound_handle < SOUND_HANDLE_MAX)) {
        return false;
    }

    snd = &(tig_sounds[sound_handle]);
    if (snd->active == 0) {
        return false;
    }

    return snd->field_134;
}

// 0x533BF0
void sub_533BF0(TigSoundEnumerateFunc* func)
{
    int index;
    TigSound* snd;

    for (index = 0; index < SOUND_HANDLE_MAX; index++) {
        snd = &(tig_sounds[index]);
        if (snd->active != 0) {
            if (snd->field_134) {
                func(index);
            }
        }
    }
}

// 0x533C30
void tig_sound_set_effects_volume(int volume)
{
    tig_sound_effects_volume = volume;
}

// 0x533C40
void tig_sound_quick_play(int id)
{
    int sound_handle;

    if (!tig_sound_initialized) {
        return;
    }

    if (tig_sound_allocate(&sound_handle, TIG_SOUND_TYPE_EFFECT) == TIG_OK) {
        tig_sound_play_by_id(sound_handle, id);
        tig_sound_set_volume(sound_handle, tig_sound_effects_volume);
    }
}

// 0x533C90
void tig_sound_set_active(bool is_active)
{
    HDIGDRIVER dig;

    if (!tig_sound_initialized) {
        return;
    }

    AIL_quick_handles(&dig, NULL, NULL);

    if (is_active) {
        AIL_digital_handle_reacquire(dig);
    } else {
        AIL_digital_handle_release(dig);
    }
}
