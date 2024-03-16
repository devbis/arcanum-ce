#include "tig/sound.h"

#include <stdio.h>

#include "tig/cache.h"
#include "tig/debug.h"
#include "tig/tig.h"

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
};

static_assert(sizeof(TigSound) == 0x150, "wrong size");

static void tig_sound_update();
static int tig_sound_acquire_handle(int type);
static void tig_sound_reset_sound(TigSound* sound);

// 0x5C246C
static int tig_sound_type_flags[TIG_SOUND_TYPE_COUNT] = {
    0x80,
    0x100,
    0x200,
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
    if ((ctx->flags & 0x1000) != 0) {
        if (ctx->field_30) {
            AIL_set_redist_directory(a1->field_30);
        }
    }

    tig_sound_initialized = false;
    tig_sound_next_effect_handle = FIRST_EFFECT_HANDLE;

    if (AIL_quick_startup(1, 0, 22050, 16, 2)) {
        tig_sound_initialized = true;
    }

    tig_sound_set_path_func(ctx->sound_file_path_resolver);

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
}

// 0x532DE0
void tig_sound_ping()
{
    // 0x739E84
    static unsigned int tig_sound_ping_time;

    if (tig_sound_initialized) {
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
}

// 0x532E30
void tig_sound_update()
{
    if (tig_sound_initialized) {
        for (int index = 0; index < SOUND_HANDLE_MAX; index++) {
            TigSound* snd = &(tig_sounds[index]);
            if (snd->active) {
                if ((snd->flags & TIG_SOUND_WAIT) == 0) {
                    if ((snd->flags & TIG_SOUND_FADE_OUT) != 0) {
                        int new_volume;
                        if (++snd->fade_step <= snd->fade_duration) {
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
                            AIL_set_stream_volume(snd->audio_stream, new_volume);
                            snd->flags |= TIG_SOUND_STOP;
                        } else if ((snd->flags & TIG_SOUND_MEMORY) != 0) {
                            AIL_quick_set_volume(snd->audio_handle, new_volume * snd->volume / 128, snd->extra_volume);
                            snd->flags |= TIG_SOUND_STOP;
                        } else {
                            snd->flags |= TIG_SOUND_STOP;
                        }
                    } else if ((snd->flags & TIG_SOUND_FADE_IN) != 0) {

                        if (snd->fade_step == 0) {
                            AIL_start_stream(snd->audio_stream);
                        }

                        int new_volume;
                        if (++snd->fade_step <= snd->fade_duration) {
                            new_volume = snd->fade_step * snd->volume / snd->fade_duration;
                        } else {
                            snd->flags &= ~TIG_SOUND_FADE_IN;
                        }

                        if ((snd->flags & TIG_SOUND_STREAMED) != 0) {
                            AIL_set_stream_volume(snd->audio_stream, new_volume);
                        } else if ((snd->flags & TIG_SOUND_MEMORY) != 0) {
                            AIL_quick_set_volume(snd->audio_handle, new_volume, 64);
                        }
                    } else {
                        if ((snd->flags & TIG_SOUND_MEMORY) != 0) {
                            if (AIL_quick_status(snd->audio_handle) == 1) {
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
    }
}

// 0x533000
void tig_sound_set_path_func(TigSoundFileNameFunc* func)
{
    tig_sound_path_func = func;
}

// 0x533020
void tig_sound_fade_out(int sound_handle, int fade_duration)
{
    if (tig_sound_initialized) {
        if (sound_handle >= 0 && sound_handle < SOUND_HANDLE_MAX) {
            TigSound* snd = &(tig_sounds[sound_handle]);
            snd->flags &= ~(TIG_SOUND_WAIT | TIG_SOUND_FADE_IN);
            if ((snd->flags & TIG_SOUND_FADE_OUT) == 0) {
                snd->flags |= TIG_SOUND_FADE_OUT;
                snd->fade_duration = abs(fade_duration);
                snd->fade_step = 0;
            }
        }
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
    if (tig_sound_initialized) {
        for (int index = 0; index < SOUND_HANDLE_MAX; index++) {
            if (tig_sounds[index].active) {
                tig_sound_fade_out(index, fade_duration);
            }
        }

        tig_sound_next_effect_handle = FIRST_EFFECT_HANDLE;

        tig_sound_update();
    }
}

// 0x5330F0
int tig_sound_allocate(int* sound_handle, int type)
{
    if (tig_sound_initialized) {
        *sound_handle = tig_sound_acquire_handle(type);
        if (*sound_handle != -1) {
            tig_sounds[*sound_handle].flags |= tig_sound_type_flags[type];
            tig_sounds[*sound_handle].loops = 1;
            tig_sounds[*sound_handle].volume = 127;
            tig_sounds[*sound_handle].extra_volume = 64;
        }
    } else {
        *sound_handle = -1;
    }

    return TIG_OK;
}

// 0x5331A0
int tig_sound_acquire_handle(int type)
{
    switch (type) {
    case TIG_SOUND_TYPE_EFFECT:
        for (int index = 0; index < SOUND_HANDLE_MAX; index++) {
            TigSound* snd = &(tig_sounds[tig_sound_next_effect_handle]);
            if (!snd->active) {
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
        for (int index = FIRST_MUSIC_HANDLE; index < FIRST_EFFECT_HANDLE; index++) {
            TigSound* snd = &(tig_sounds[index]);
            if (!snd->active) {
                tig_sound_reset_sound(snd);
                snd->active = 1;
                return index;
            }
        }
        break;
    case TIG_SOUND_TYPE_VOICE:
        for (int index = FIRST_VOICE_HANDLE; index < FIRST_MUSIC_HANDLE; index++) {
            TigSound* snd = &(tig_sounds[index]);
            if (!snd->active) {
                tig_sound_reset_sound(snd);
                snd->active = 1;
                return index;
            }
        }
        break;
    }

    tig_debug_printf("No sound handle available for sound type %d! Current sounds are:\n");
    for (int index = 0; index < SOUND_HANDLE_MAX; index++) {
        // FIXME: This approach does not respect lower and upper bounds for the
        // requested sound type. Let's say we're out of music handles, it will
        // dump voice and sound handles, which (1) cannot store music handles,
        // and (2) can be empty or obsolete value (no check for `active`).
        tig_debug_printf("%s\n", tig_sounds[index].path);
    }
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
    if (tig_sound_initialized) {
        if (sound_handle >= 0 && sound_handle < SOUND_HANDLE_MAX) {
            if (tig_sounds[sound_handle].active) {
                tig_sound_fade_out_from_stop(sound_handle, 0);

                if ((tig_sounds[sound_handle].flags & TIG_SOUND_STREAMED) != 0) {
                    AIL_close_stream(tig_sounds[sound_handle].audio_stream);
                    tig_sounds[sound_handle].active = 0;
                } else if ((tig_sounds[sound_handle].flags & TIG_SOUND_MEMORY) != 0) {
                    AIL_quick_unload(tig_sounds[sound_handle].audio_handle);
                    tig_cache_release(tig_sound_cache, tig_sounds[sound_handle].cache_entry);
                    tig_sounds[sound_handle].active = 0;
                } else {
                    tig_sounds[sound_handle].active = 0;
                }
            }
        }
    }
}

// 0x5333A0
int tig_sound_play_by_path(int sound_handle, const char* path, int id)
{
    if (tig_sound_initialized) {
        // FIXME: No `SOUND_HANDLE_MAX` guard.
        if (sound_handle >= 0) {
            TigSound* snd = &(tig_sounds[sound_handle]);
            strcpy(snd->path, path);
            snd->cache_entry = tig_cache_acquire(tig_sound_cache, path);

            if (snd->cache_entry->data != NULL) {
                snd->audio_handle = AIL_quick_load_mem(snd->cache_entry->data, snd->cache_entry->size);
                AIL_quick_set_volume(snd->audio_handle, snd->volume, snd->extra_volume);
                AIL_quick_play(snd->audio_handle, snd->loops);
                snd->flags |= TIG_SOUND_MEMORY;
                snd->id = id;
            } else {
                snd->active = 0;
            }
        }
    }

    return TIG_OK;
}

// 0x533480
int tig_sound_play_by_id(int sound_handle, int id)
{
    if (tig_sound_initialized) {
        char path[MAX_PATH];
        tig_sound_path_func(id, path);
        return tig_sound_play_by_path(sound_handle, path, id);
    } else {
        return TIG_OK;
    }
}

// 0x5334D0
int tig_sound_play_streamed(int sound_handle, const char* name, int loops, int fade_duration, int prev_sound_handle)
{
    if (tig_sound_initialized) {
        char path[MAX_PATH];
        if (sub_52F410(name, path)) {
            if (sound_handle >= 0 && sound_handle < SOUND_HANDLE_MAX) {
                TigSound* snd = &(tig_sounds[sound_handle]);
                snd->fade_duration = abs(fade_duration);
                snd->fade_step = 0;

                HDIGDRIVER dig;
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
                    TigSound* prev_snd = &(tig_sounds[prev_sound_handle]);
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
            }
        }
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

// 0x533700
void tig_sound_set_loops(int sound_handle, int loops)
{
    if (tig_sound_initialized) {
        if (sound_handle >= 0 && sound_handle < SOUND_HANDLE_MAX) {
            tig_sounds[sound_handle].loops = loops;
        }
    }
}

// 0x533760
void tig_sound_set_volume(int sound_handle, int volume)
{
    if (tig_sound_initialized) {
        if (sound_handle >= 0 && sound_handle < SOUND_HANDLE_MAX) {
            TigSound* snd = &(tig_sounds[sound_handle]);
            if (snd->volume != volume) {
                snd->volume = volume;

                if ((snd->flags & TIG_SOUND_STREAMED) != 0) {
                    AIL_set_stream_volume(snd->audio_stream, volume);
                } else if ((snd->flags & TIG_SOUND_MEMORY) != 0) {
                    AIL_quick_set_volume(snd->audio_stream, volume, snd->extra_volume);
                }
            }
        }
    }
}

// 0x5337D0
void tig_sound_set_volume_by_type(int type, int volume)
{
    if (tig_sound_initialized) {
        for (int index = 0; index < SOUND_HANDLE_MAX; index++) {
            TigSound* snd = &(tig_sounds[index]);
            if (snd->active) {
                if ((tig_sound_type_flags[type] & snd->flags) != 0) {
                    tig_sound_set_volume(index, volume);
                }
            }
        }

        if (type == TIG_SOUND_TYPE_EFFECT) {
            tig_sound_set_effects_volume(volume);
        }
    }
}

// 0x533830
int tig_sound_get_type(int sound_handle)
{
    if (tig_sound_initialized) {
        if (sound_handle >= 0 && sound_handle < SOUND_HANDLE_MAX) {
            for (int index = 0; index < TIG_SOUND_TYPE_COUNT; index++) {
                if ((tig_sounds[sound_handle].flags & tig_sound_type_flags[index]) != 0) {
                    return index;
                }
            }
        }
    }

    // NOTE: Not sure if it's `TIG_SOUND_TYPE_EFFECT`, `TIG_OK`, or simply 0.
    // Probably should be -1 to denote error.
    return TIG_SOUND_TYPE_EFFECT;
}

// 0x5338D0
int sub_5338D0(int sound_handle)
{
    if (tig_sound_initialized) {
        if (sound_handle >= 0 && sound_handle < SOUND_HANDLE_MAX) {
            return tig_sounds[sound_handle].field_148;
        }
    }

    return 2;
}

// 0x533910
void sub_533910(int sound_handle, int value)
{
    if (tig_sound_initialized) {
        if (sound_handle >= 0 && sound_handle < SOUND_HANDLE_MAX) {
            tig_sounds[sound_handle].field_148 = value;
        }
    }
}

// 0x533980
void tig_sound_set_extra_volume(int sound_handle, int extra_volume)
{
    if (tig_sound_initialized) {
        if (sound_handle >= 0 && sound_handle < SOUND_HANDLE_MAX) {
            TigSound* snd = &(tig_sounds[sound_handle]);
            if (snd->extra_volume != extra_volume) {
                snd->extra_volume = extra_volume;

                if ((snd->flags & TIG_SOUND_STREAMED) == 0 && (snd->flags & TIG_SOUND_MEMORY) != 0) {
                    AIL_quick_set_volume(snd->audio_handle, snd->volume, extra_volume);
                }
            }
        }
    }
}

// 0x5339E0
bool tig_sound_is_active_id(int id)
{
    if (tig_sound_initialized) {
        for (int index = 0; index < SOUND_HANDLE_MAX; index++) {
            if (tig_sounds[index].active) {
                if (tig_sounds[index].id == id) {
                    return true;
                }
            }
        }
    }

    return false;
}

// 0x533A20
bool tig_sound_is_playing(int sound_handle)
{
    // FIXME: No `tig_sound_initialized` guard.

    if (sound_handle >= 0 && sound_handle < SOUND_HANDLE_MAX) {
        TigSound* snd = &(tig_sounds[sound_handle]);
        if (snd->active) {
            if ((snd->flags & TIG_SOUND_MEMORY) != 0) {
                // TODO: Figure out status.
                if (AIL_quick_status(snd->audio_handle) == 3) {
                    return true;
                }
            }

            if ((snd->flags & TIG_SOUND_STREAMED) != 0) {
                // TODO: Figure out status.
                if (AIL_stream_status(snd->audio_stream) == 4) {
                    return true;
                }
            }
        }
    }

    return 0;
}

// 0x533A90
bool tig_sound_is_active(int sound_handle)
{
    // FIXME: No `tig_sound_initialized` guard.

    if (sound_handle >= 0 && sound_handle < SOUND_HANDLE_MAX) {
        return tig_sounds[sound_handle].active != 0;
    }

    return false;
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
        tig_sound_cache->length,
        tig_sound_cache->bytes);
    return buffer;
}

// 0x533B10
void sub_533B10(int sound_handle, int64_t a2, int64_t a3)
{
    // FIXME: No `tig_sound_initialized` guard.

    if (sound_handle >= 0 && sound_handle < SOUND_HANDLE_MAX) {
        tig_sounds[sound_handle].field_138 = a2;
        tig_sounds[sound_handle].field_140 = a3;
        tig_sounds[sound_handle].field_134 = true;
    }
}

// 0x533B60
void sub_533B60(int sound_handle, int64_t* a2, int64_t* a3)
{
    // FIXME: No `tig_sound_initialized` guard.

    if (sound_handle >= 0 && sound_handle < SOUND_HANDLE_MAX) {
        TigSound* snd = &(tig_sounds[sound_handle]);
        if (snd->active) {
            if (snd->field_134) {
                *a2 = snd->field_138;
                *a3 = snd->field_140;
            }
        }
    }
}

// 0x533BF0
void sub_533BF0(TigSoundEnumerateFunc* func)
{
    for (int index = 0; index < SOUND_HANDLE_MAX; index++) {
        TigSound* snd = &(tig_sounds[index]);
        if (snd->active) {
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
    if (tig_sound_initialized) {
        int sound_handle;
        if (tig_sound_allocate(&sound_handle, TIG_SOUND_TYPE_EFFECT) == TIG_OK) {
            tig_sound_play_by_id(sound_handle, id);
            tig_sound_set_volume(sound_handle, tig_sound_effects_volume);
        }
    }
}

// 0x533C90
void tig_sound_set_active(bool is_active)
{
    if (tig_sound_initialized) {
        HDIGDRIVER dig;
        AIL_quick_handles(&dig, NULL, NULL);

        if (is_active) {
            AIL_digital_handle_reacquire(dig);
        } else {
            AIL_digital_handle_release(dig);
        }
    }
}
