#include "game/lib/gsound.h"

#include <stdio.h>

#include "game/lib/message.h"
#include "game/lib/random.h"
#include "game/lib/settings.h"
#include "tig/file.h"
#include "tig/sound.h"

#define TWO 2
#define TWENTY_FIVE 25

typedef struct SoundScheme {
    uint8_t field_0;
    uint8_t field_1;
    uint8_t field_2;
    int frequency;
    int time_start;
    int time_end;
    int balance_left;
    int balance_right;
    int volume_min;
    int volume_max;
    int field_20;
    char path[MAX_PATH];
};

// See 0x41BF70.
static_assert(sizeof(SoundScheme) == 0x128, "wrong size");

typedef struct SoundSchemeList {
    int field_0;
    int field_4;
    int field_8;
    SoundScheme field_C[TWENTY_FIVE];
};

static_assert(sizeof(SoundSchemeList) == 0x1CF4, "wrong size");

static const char* gsound_build_sound_path(const char* name);

// 0x5A0F38
static const char* gsound_base_sound_path = "sound\\";

// 0x5D1A18
static int gsound_voice_volume;

// 0x5D1A20
static int64_t qword_5D1A20;

// 0x5D1A28
static int64_t qword_5D1A28;

// 0x5D1A30
static int dword_5D1A30[TWO];

// 0x5D1A38
static int dword_5D1A38[TWO];

// 0x5D1A40
static int dword_5D1A40;

// 0x5D1A44
static int gsound_music_volume;

// 0x5D1A48
static int dword_5D1A48;

// 0x5D1A4C
static int dword_5D1A4C;

// 0x5D1A68
static int* dword_5D1A68;

// 0x5D1A60
static int64_t qword_5D1A60;

// 0x5D1A6C
static bool dword_5D1A6C;

// 0x5D1A70
static int dword_5D1A70;

// 0x5D1A88
static int64_t qword_5D1A88;

// 0x5D1A98
static SoundSchemeList stru_5D1A98[TWO];

// 0x5D5480
static int dword_5D5480;

// 0x5D5484
static int dword_5D5484[TWO];

// NOTE: It's `bool`, but needs to be 4 byte integer because of saving/reading
// compatibility.
//
// 0x5D5594
static int dword_5D5594;

// 0x5D5598
static int dword_5D5598;

// 0x5D559C
static int gsound_effects_volume;

// 0x5D55C8
static int64_t qword_5D55C8;

// 0x5D55B0
static int dword_5D55B0;

// NOTE: It's `bool`, but needs to be 4 byte integer because of saving/reading
// compatibility.
//
// 0x5D55D8
static int dword_5D55D8;

// 0x5D55E0
static int64_t qword_5D55E0;

// 0x5D55E8
static int64_t qword_5D55E8;

// 0x41A940
int sub_41A940(int sound_id, char* path)
{
    if (dword_5D1A6C) {
        MessageListItem msg;
        msg.num = sound_id;

        for (int index = 0; index < dword_5D55B0; index++) {
            if (message_find(dword_5D1A68[index], &msg)) {
                sprintf(path, "%s%s", gsound_base_sound_path, msg.text);
                return TIG_OK;
            }
        }

        if (dword_5D5598 != -1) {
            if (message_find(dword_5D5598, &msg)) {
                sprintf(path, "%s%s", gsound_base_sound_path, msg.text);
                return TIG_OK;
            }
        }
    }

    path[0] = '\0';
    return TIG_ERR_16;
}

// 0x41AA30
bool gsound_init(GameContext* ctx)
{
    // TODO: Incomplete.
}

// 0x41AF80
const char* gsound_build_sound_path(const char* name)
{
    // 0x5D5490
    static char path[MAX_PATH];

    sprintf(path, "%s%s", gsound_base_sound_path, name);

    return path;
}

// 0x41AFF0
void gsound_exit()
{
    dword_5D1A6C = false;

    for (int index = 0; index < dword_5D55B0; index++) {
        message_unload(dword_5D1A68[index]);
    }

    free(dword_5D1A68);
    dword_5D1A68 = NULL;

    dword_5D55B0 = 0;

    message_unload(dword_5D5480);
    message_unload(dword_5D1A40);
}

// 0x41B060
void gsound_reset()
{
    qword_5D1A28 = 0;
    qword_5D55E8 = 0;
    qword_5D55E0 = 0;
    dword_5D55D8 = false;
    dword_5D1A70 = 0;
    sub_41C340(0);
}

// 0x41B0A0
bool gsound_mod_load()
{
    message_load(gsound_build_sound_path("snd_user.mes"), &dword_5D5598);
    return true;
}

// 0x41B0D0
void gsound_mod_unload()
{
    message_unload(dword_5D5598);
    dword_5D5598 = -1;
}

// 0x41B0F0
bool gsound_load(LoadContext* ctx)
{
    int temp[TWO];

    for (int index = 0; index < TWO; index++) {
        if (tig_file_fread(&(temp[index]), sizeof(*temp), 1, ctx->stream) != 1) {
            return false;
        }
    }

    if (tig_file_fread(&dword_5D5594, sizeof(dword_5D5594), 1, ctx->stream) != 1) {
        return false;
    }

    for (int index = 0; index < TWO; index++) {
        if (tig_file_fread(&(dword_5D1A38[index]), sizeof(*dword_5D1A38), 1, ctx->stream) != 1) {
            return false;
        }
    }

    if (tig_file_fread(&dword_5D55D8, sizeof(dword_5D55D8), 1, ctx->stream) != 1) {
        return false;
    }

    for (int index = 0; index < TWO; index++) {
        if (tig_file_fread(&(dword_5D1A30[index]), sizeof(*dword_5D1A30), 1, ctx->stream) != 1) {
            return false;
        }
    }

    sub_41BD50(temp[0], temp[1]);

    return true;
}

// 0x41B1D0
bool gsound_save(TigFile* stream)
{
    for (int index = 0; index < TWO; index++) {
        int v1 = stru_5D1A98[index].field_0 ? stru_5D1A98[index].field_4 : 0;
        if (tig_file_fwrite(&v1, sizeof(v1), 1, stream) != 1) {
            return false;
        }
    }

    if (tig_file_fwrite(&dword_5D5594, sizeof(dword_5D5594), 1, stream) != 1) {
        return false;
    }

    for (int index = 0; index < TWO; index++) {
        if (tig_file_fwrite(&(dword_5D1A38[index]), sizeof(*dword_5D1A38), 1, stream) != 1) {
            return false;
        }
    }

    if (tig_file_fwrite(&dword_5D55D8, sizeof(dword_5D55D8), 1, stream) != 1) {
        return false;
    }

    for (int index = 0; index < TWO; index++) {
        if (tig_file_fwrite(&(dword_5D1A30[index]), sizeof(*dword_5D1A30), 1, stream) != 1) {
            return false;
        }
    }
}

// 0x41B2E0
int gsound_play_sfx(const char* path, int loops, int volume, int extra_volume, int id)
{
    if (!dword_5D1A6C) {
        return -1;
    }

    if (dword_5D1A70) {
        return -1;
    }

    int sound_handle;
    tig_sound_allocate(&sound_handle, TIG_SOUND_TYPE_EFFECT);
    tig_sound_set_loops(sound_handle, loops);
    tig_sound_set_volume(sound_handle, volume * gsound_effects_volume / 100);
    tig_sound_set_extra_volume(sound_handle, extra_volume);
    tig_sound_play_by_path(sound_handle, path, id);
    if (!tig_sound_is_active(sound_handle)) {
        return -1;
    }

    return sound_handle;
}

// 0x41B720
int gsound_play_sfx_id_ex(int id, int loops, int volume, int extra_volume)
{
    if (!dword_5D1A6C) {
        return -1;
    }

    char path[MAX_PATH];
    sub_41A940(id, path);
    return gsound_play_sfx(path, loops, volume, extra_volume, id);
}

// 0x41B780
int gsound_play_sfx_id(int id, int loops)
{
    return gsound_play_sfx_id_ex(id, loops, 127, 64);
}

// 0x41B980
int sub_41B980(object_id_t object_id)
{
    if (object_field_get(object_id, OBJ_F_TYPE) != OBJ_TYPE_SCENERY) {
        return 2;
    }

    unsigned int flags = object_field_get(object_id, OBJ_F_SCENERY_FLAGS);

    if ((flags & SCENERY_FLAG_0x40) != 0) {
        return 0;
    }

    if ((flags & SCENERY_FLAG_0x80) != 0) {
        return 1;
    }

    if ((flags & SCENERY_FLAG_0x100) != 0) {
        return 2;
    }

    return 3;
}

// 0x41BAC0
void sub_41BAC0(int a1)
{
    if (dword_5D1A6C) {
        for (int index = 0; index < TWO; index++) {
            sub_41BA20(a1, index);
        }
    }
}

// 0x41BD10
bool sub_41BD10(int a1, int* a2)
{
    for (int index = 0; index < TWO; index++) {
        if (stru_5D1A98[index].field_0 != 0) {
            if (stru_5D1A98[index].field_4 == a1) {
                *a2 = index;
                return true;
            }
        }
    }

    return false;
}

// 0x41BF70
SoundScheme* sub_41BF70(SoundSchemeList* a1, const char* path)
{
    SoundScheme* scheme = NULL;

    char* copy = _strdup(path);
    if (copy != NULL) {
        if (a1->field_8 < TWENTY_FIVE) {
            scheme = &(a1->field_C[a1->field_8++]);
            memset(scheme, 0, sizeof(*scheme));
            scheme->field_0 = 0;
            scheme->field_1 = 0;
            scheme->field_2 = 0;
            scheme->balance_left = 50;
            scheme->balance_right = 50;
            scheme->frequency = 5;
            scheme->time_start = 0;
            scheme->time_end = 23;
            scheme->volume_min = 100;
            scheme->volume_max = 100;
            scheme->field_20 = -1;
            scheme->path[0] = '\0';
            sub_41C260(copy);

            char* pch = strchr(copy, ' ');
            if (pch != NULL) {
                *pch++ = '\0';
            }

            strcpy(scheme->path, copy);

            if (pch != NULL) {
                sub_41C290(pch, "/vol:", &(scheme->volume_min), &(scheme->volume_max));

                if (strstr(pch, "/loop") != NULL || strstr(pch, "/anchor") != NULL || strstr(pch, "/over") != NULL) {
                    scheme->field_0 = 1;

                    if (strstr(pch, "/over") != NULL) {
                        scheme->field_2 = 1;
                    }

                    if (strstr(pch, "/loop") != NULL) {
                        scheme->field_1 = 1;
                        sub_41C290(pch, "/time:", &(scheme->time_start), &(scheme->time_end));
                    }
                } else {
                    sub_41C290(pch, "/freq:", &(scheme->frequency), NULL);
                    sub_41C290(pch, "/time:", &(scheme->time_start), &(scheme->time_end));
                    sub_41C290(pch, "/bal:", &(scheme->balance_left), &(scheme->balance_right));

                    int v1 = -1;
                    int v2 = -1;
                    sub_41C290(pch, "/scatter:", &v1, &v2);

                    if (v1 >= 0) {
                        scheme->volume_max = 100;
                        scheme->volume_min = 100 - v1;
                        scheme->balance_left = 50 - v1 / 2;
                        scheme->balance_right = 50 + v1 / 2;
                    }
                }
            }
        }

        free(copy);

        if (scheme->volume_max < scheme->volume_min) {
            scheme->volume_max = scheme->volume_min;
        }

        if (scheme->balance_right < scheme->balance_left) {
            scheme->balance_right = scheme->balance_left;
        }

        // TODO: Strange 64-bit math, check.
        scheme->volume_min = 127 * scheme->volume_min / 100;
        scheme->volume_max = 127 * scheme->volume_max / 100;
        scheme->balance_left = 127 * scheme->balance_left / 100;
        scheme->balance_right = 127 * scheme->balance_right / 100;

        if (scheme->volume_min < 0) {
            scheme->volume_min = 0;
        }

        if (scheme->volume_min > 127) {
            scheme->volume_min = 127;
        }

        if (scheme->volume_max < 0) {
            scheme->volume_max = 0;
        }

        if (scheme->volume_max > 127) {
            scheme->volume_max = 127;
        }

        if (scheme->balance_left < 0) {
            scheme->balance_left = 0;
        }

        // FIXME: Looks wrong, max balance should be 100.
        if (scheme->balance_left > 127) {
            scheme->balance_left = 0;
        }

        if (scheme->balance_right < 0) {
            scheme->balance_right = 0;
        }

        // FIXME: Looks wrong, max balance should be 100.
        if (scheme->balance_right > 127) {
            scheme->balance_right = 127;
        }
    }

    return scheme;
}

// 0x41C260
void sub_41C260(char* str)
{
    while (*str != '\0') {
        *str = tolower(*str);
        str++;
    }
}

// 0x41C290
void sub_41C290(const char* str, const char* key, int* value1, int* value2)
{
    const char* pch = strstr(str, key);
    if (pch != NULL) {
        char* copy = _strdup(pch + strlen(key));
        if (copy != NULL) {
            char* space = strchr(copy, ' ');
            if (space != NULL) {
                *space = '\0';
            }

            if (value1 != NULL) {
                *value1 = atoi(copy);

                if (value2 != NULL) {
                    char* sep = strchr(copy, ',');
                    if (sep == NULL) {
                        sep = strchr(copy, '-');
                    }

                    if (sep != NULL) {
                        *value2 = atoi(sep + 1);
                    } else {
                        *value2 = *value1;
                    }
                }
            }

            free(copy);
        }
    }
}

// 0x41C340
void sub_41C340(int duration)
{
    if (dword_5D1A6C) {
        sub_41BAC0(duration);
        dword_5D5594 = false;
        tig_sound_fade_out_all(duration);
    }
}

// 0x41C370
int gsound_play_voice(const char* path, int fade_duration)
{
    if (dword_5D1A70) {
        return -1;
    }

    int sound_handle;
    tig_sound_allocate(&sound_handle, TIG_SOUND_TYPE_VOICE);
    tig_sound_set_volume(sound_handle, gsound_voice_volume);
    tig_sound_play_streamed_once(sound_handle, path, fade_duration, -1);
    if (!tig_sound_is_active(sound_handle)) {
        return -1;
    }

    return sound_handle;
}

// 0x41C3D0
int gsound_play_music_once(const char* path, int fade_duration)
{
    if (dword_5D1A70) {
        return -1;
    }

    int sound_handle;
    tig_sound_allocate(&sound_handle, TIG_SOUND_TYPE_MUSIC);
    tig_sound_set_volume(sound_handle, 100 * gsound_music_volume / 100);
    tig_sound_play_streamed_once(sound_handle, path, fade_duration, -1);
    if (!tig_sound_is_active(sound_handle)) {
        return -1;
    }

    return sound_handle;
}

// 0x41C450
int gsound_play_music_indefinitely(const char* path, int fade_duration)
{
    if (dword_5D1A70) {
        return -1;
    }

    int sound_handle;
    tig_sound_allocate(&sound_handle, TIG_SOUND_TYPE_MUSIC);
    tig_sound_set_volume(sound_handle, 100 * gsound_music_volume / 100);
    tig_sound_play_streamed_indefinitely(sound_handle, path, fade_duration, -1);
    if (!tig_sound_is_active(sound_handle)) {
        return -1;
    }

    return sound_handle;
}

// 0x41C4D0
void sub_41C4D0(object_id_t object_id)
{
    if (!dword_5D55D8) {
        if (object_id != 0) {
            if (object_field_get(object_id, OBJ_F_TYPE) == OBJ_TYPE_NPC) {
                for (int index = 0; index < TWO; index++) {
                    if (stru_5D1A98[index].field_0) {
                        dword_5D1A30[index] = stru_5D1A98[index].field_4;
                    } else {
                        dword_5D1A30[index] = 0;
                    }
                }

                sub_41BAC0(25);

                dword_5D55D8 = true;

                char path[MAX_PATH];
                sprintf(path, "sound\\music\\combat %d.mp3", random_between(1, 6));
                dword_5D1A4C = gsound_play_music_once(path, 0);
                dword_5D1A48 = gsound_play_music_indefinitely("sound\\music\\combatmusic.mp3", 0);
            }
        }
    }
}

// 0x41C5A0
void sub_41C5A0(object_id_t object_id)
{
    if (dword_5D55D8) {
        if (object_id != 0) {
            if (object_field_get(object_id, OBJ_F_TYPE) == OBJ_TYPE_PC) {
                tig_sound_stop(dword_5D1A4C);
                tig_sound_stop(dword_5D1A48);

                dword_5D55D8 = false;

                sub_41BD50(dword_5D1A30[0], dword_5D1A30[1]);
            }
        }
    }
}

// 0x41C610
void sub_41C610()
{
    if (dword_5D1A70 == 0) {
        for (int index = 0; index < TWO; index++) {
            if (stru_5D1A98[index].field_0) {
                dword_5D5484[index] = stru_5D1A98[index].field_4;
            } else {
                dword_5D5484[index] = 0;
            }
        }

        sub_41BAC0(0);
    }

    dword_5D1A70++;
}

// 0x41C660
void sub_41C660()
{
    if (dword_5D1A70 > 0) {
        dword_5D1A70--;
        if (dword_5D1A70 == 0) {
            sub_41BD50(dword_5D5484[0], dword_5D5484[1]);
        }
    }
}

// 0x41C850
void sub_41C850(int a1, int a2, int a3, int a4)
{
    if (dword_5D1A6C) {
        qword_5D1A88 = a1;
        qword_5D55C8 = a2;
        qword_5D1A60 = a3;
        qword_5D1A20 = a4;
        sub_41B3A0();
    }
}

// 0x41C8A0
int gsound_effects_volume_get()
{
    return gsound_effects_volume;
}

// 0x41C920
int gsound_voice_volume_get()
{
    return gsound_voice_volume;
}

// 0x41C930
void gsound_voice_volume_changed()
{
    gsound_voice_volume = 127 * settings_get_value(&settings, "voice volume") / 10;
    tig_sound_set_volume_by_type(TIG_SOUND_TYPE_VOICE, gsound_voice_volume);
}

// 0x41C970
int gsound_music_volume_get()
{
    return gsound_music_volume;
}

// 0x41C980
void gsound_music_volume_changed()
{
    gsound_music_volume = 80 * (127 * settings_get_value(&settings, "music volume") / 10) / 100;
    tig_sound_set_volume_by_type(TIG_SOUND_TYPE_VOICE, gsound_music_volume);
}

// 0x41C9D0
int sub_41C9D0(int sound_handle, int a2)
{
    switch (tig_sound_get_type(sound_handle)) {
    case TIG_SOUND_TYPE_EFFECT:
        return a2 * gsound_effects_volume_get() / 127;
    case TIG_SOUND_TYPE_MUSIC:
        return a2 * gsound_music_volume_get() / 127;
    case TIG_SOUND_TYPE_VOICE:
        return a2 * gsound_voice_volume_get() / 127;
    }

    return a2;
}
