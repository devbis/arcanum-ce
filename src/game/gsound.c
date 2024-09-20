#include "game/gsound.h"

#include <stdio.h>

#include "game/gamelib.h"
#include "game/mes.h"
#include "game/player.h"
#include "game/random.h"

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
    tig_sound_handle_t sound_handle;
    char path[MAX_PATH];
} SoundScheme;

// See 0x41BF70.
static_assert(sizeof(SoundScheme) == 0x128, "wrong size");

typedef struct SoundSchemeList {
    int field_0;
    int field_4;
    int field_8;
    SoundScheme field_C[TWENTY_FIVE];
} SoundSchemeList;

static_assert(sizeof(SoundSchemeList) == 0x1CF4, "wrong size");

static const char* gsound_build_sound_path(const char* name);
static void sub_41AFB0(SoundSchemeList* scheme);
static void sub_41B3A0();
static void sub_41B3B0(tig_sound_handle_t sound_handle);
static void sub_41B420(int64_t x, int64_t y, int* volume_ptr, int* extra_volume_ptr, TigSoundPositionalSize size);
static void sub_41BA20(int fade_duration, int index);
static void sub_41BAC0(int fade_duration);
static void sub_41BCD0(const char* name, char* buffer);
static bool sub_41BD10(int a1, int* a2);
static void sub_41BE20(int num);
static SoundScheme* sub_41BF70(SoundSchemeList* a1, const char* path);
static void sub_41C260(char* str);
static void sub_41C290(const char* str, const char* key, int* value1, int* value2);
static void sub_41C690(int64_t a1, int64_t a2);
static void gsound_set_defaults(int min_radius, int max_radius, int a3, int a4);
static int gsound_effects_volume_get();
static void gsound_effects_volume_changed();
static int gsound_voice_volume_get();
static void gsound_voice_volume_changed();
static int gsound_music_volume_get();
static void gsound_music_volume_changed();
static int adjust_volume(tig_sound_handle_t sound_handle, int volume);

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
static mes_file_handle_t dword_5D1A40;

// 0x5D1A44
static int gsound_music_volume;

// 0x5D1A48
static int dword_5D1A48;

// 0x5D1A4C
static int dword_5D1A4C;

// 0x5D1A50
static int64_t qword_5D1A50;

// 0x5D1A58
static int64_t qword_5D1A58;

// 0x5D1A68
static mes_file_handle_t* dword_5D1A68;

// 0x5D1A60
static int64_t qword_5D1A60;

// 0x5D1A6C
static bool gsound_initialized;

// 0x5D1A70
static int dword_5D1A70;

// 0x5D1A78
static int64_t sound_minimum_radius[TIG_SOUND_SIZE_COUNT];

// 0x5D1A98
static SoundSchemeList stru_5D1A98[TWO];

// 0x5D5480
static mes_file_handle_t dword_5D5480;

// 0x5D5484
static int dword_5D5484[TWO];

// 0x5D548C
static int dword_5D548C;

// NOTE: It's `bool`, but needs to be 4 byte integer because of saving/reading
// compatibility.
//
// 0x5D5594
static int dword_5D5594;

// 0x5D5598
static mes_file_handle_t dword_5D5598;

// 0x5D559C
static int gsound_effects_volume;

// 0x5D55A0
static int sound_maximum_volume[TIG_SOUND_SIZE_COUNT];

// 0x5D55B0
static int dword_5D55B0;

// 0x5D55B8
static int64_t sound_maximum_radius[TIG_SOUND_SIZE_COUNT];

// 0x5D55D8
static bool gsound_combat_music_active;

// 0x5D55E0
static int64_t qword_5D55E0;

// 0x5D55E8
static int64_t qword_5D55E8;

// 0x41A940
int gsound_resolve_path(int sound_id, char* path)
{
    MesFileEntry mes_file_entry;
    int index;

    if (gsound_initialized) {
        mes_file_entry.num = sound_id;

        for (index = 0; index < dword_5D55B0; index++) {
            if (mes_search(dword_5D1A68[index], &mes_file_entry)) {
                sprintf(path, "%s%s", gsound_base_sound_path, mes_file_entry.str);
                return TIG_OK;
            }
        }

        if (dword_5D5598 != MES_FILE_HANDLE_INVALID) {
            if (mes_search(dword_5D5598, &mes_file_entry)) {
                sprintf(path, "%s%s", gsound_base_sound_path, mes_file_entry.str);
                return TIG_OK;
            }
        }
    }

    path[0] = '\0';
    return TIG_ERR_16;
}

// 0x41AA30
bool gsound_init(GameInitInfo* init_info)
{
    mes_file_handle_t tmp_mes_file;
    MesFileEntry mes_file_entry;
    int index;
    int cnt;

    (void)init_info;

    mes_load(gsound_build_sound_path("snd_00index.mes"), &tmp_mes_file);
    dword_5D55B0 = mes_num_entries(tmp_mes_file);
    if (dword_5D55B0 == 0) {
        return false;
    }

    dword_5D1A68 = CALLOC(dword_5D55B0, sizeof(*dword_5D1A68));

    mes_file_entry.num = 0;
    for (index = 0; index < dword_5D55B0; index++) {
        dword_5D1A68[index] = MES_FILE_HANDLE_INVALID;
        mes_find_next(tmp_mes_file, &mes_file_entry);
        mes_load(gsound_build_sound_path(mes_file_entry.str), &(dword_5D1A68[index]));
    }

    mes_unload(tmp_mes_file);

    mes_load(gsound_build_sound_path("SchemeIndex.mes"), &dword_5D5480);
    mes_load(gsound_build_sound_path("SchemeList.mes"), &dword_5D1A40);

    for (index = 0; index < 2; index++) {
        sub_41AFB0(&(stru_5D1A98[index]));
    }

    gsound_initialized = true;

    sub_41C690(400, 300);
    gsound_set_defaults(150, 800, 150, 400);

    sound_maximum_volume[TIG_SOUND_SIZE_LARGE] = 100;

    mes_load(gsound_build_sound_path("soundparams.mes"), &tmp_mes_file);

    if (tmp_mes_file != MES_FILE_HANDLE_INVALID) {
        cnt = 0;

        mes_file_entry.num = 1;
        if (mes_search(tmp_mes_file, &mes_file_entry)) {
            sound_minimum_radius[TIG_SOUND_SIZE_LARGE] = atoi(mes_file_entry.str);
            cnt++;
        }

        mes_file_entry.num = 2;
        if (mes_search(tmp_mes_file, &mes_file_entry)) {
            sound_maximum_radius[TIG_SOUND_SIZE_LARGE] = atoi(mes_file_entry.str);
            cnt++;
        }

        mes_file_entry.num = 3;
        if (mes_search(tmp_mes_file, &mes_file_entry)) {
            qword_5D1A60 = atoi(mes_file_entry.str);
            cnt++;
        }

        mes_file_entry.num = 4;
        if (mes_search(tmp_mes_file, &mes_file_entry)) {
            qword_5D1A20 = atoi(mes_file_entry.str);
            cnt++;
        }

        mes_file_entry.num = 10;
        if (mes_search(tmp_mes_file, &mes_file_entry)) {
            sound_minimum_radius[TIG_SOUND_SIZE_SMALL] = atoi(mes_file_entry.str);
        } else {
            sound_minimum_radius[TIG_SOUND_SIZE_SMALL] = 50;
        }

        mes_file_entry.num = 11;
        if (mes_search(tmp_mes_file, &mes_file_entry)) {
            sound_maximum_radius[TIG_SOUND_SIZE_SMALL] = atoi(mes_file_entry.str);
        } else {
            sound_maximum_radius[TIG_SOUND_SIZE_SMALL] = 150;
        }

        mes_file_entry.num = 12;
        if (mes_search(tmp_mes_file, &mes_file_entry)) {
            sound_maximum_volume[TIG_SOUND_SIZE_MEDIUM] = atoi(mes_file_entry.str);
        } else {
            sound_maximum_volume[TIG_SOUND_SIZE_MEDIUM] = 40;
        }

        mes_file_entry.num = 20;
        if (mes_search(tmp_mes_file, &mes_file_entry)) {
            sound_minimum_radius[TIG_SOUND_SIZE_MEDIUM] = atoi(mes_file_entry.str);
        } else {
            sound_minimum_radius[TIG_SOUND_SIZE_MEDIUM] = 50;
        }

        mes_file_entry.num = 21;
        if (mes_search(tmp_mes_file, &mes_file_entry)) {
            sound_maximum_radius[TIG_SOUND_SIZE_MEDIUM] = atoi(mes_file_entry.str);
        } else {
            sound_maximum_radius[TIG_SOUND_SIZE_MEDIUM] = 400;
        }

        mes_file_entry.num = 22;
        if (mes_search(tmp_mes_file, &mes_file_entry)) {
            sound_maximum_volume[TIG_SOUND_SIZE_MEDIUM] = atoi(mes_file_entry.str);
        } else {
            sound_maximum_volume[TIG_SOUND_SIZE_MEDIUM] = 70;
        }

        mes_file_entry.num = 30;
        if (mes_search(tmp_mes_file, &mes_file_entry)) {
            sound_minimum_radius[TIG_SOUND_SIZE_EXTRA_LARGE] = atoi(mes_file_entry.str);
        } else {
            sound_minimum_radius[TIG_SOUND_SIZE_EXTRA_LARGE] = 50;
        }

        mes_file_entry.num = 31;
        if (mes_search(tmp_mes_file, &mes_file_entry)) {
            sound_maximum_radius[TIG_SOUND_SIZE_EXTRA_LARGE] = atoi(mes_file_entry.str);
        } else {
            sound_maximum_radius[TIG_SOUND_SIZE_EXTRA_LARGE] = 1500;
        }

        mes_file_entry.num = 32;
        if (mes_search(tmp_mes_file, &mes_file_entry)) {
            sound_maximum_volume[TIG_SOUND_SIZE_EXTRA_LARGE] = atoi(mes_file_entry.str);
        } else {
            sound_maximum_volume[TIG_SOUND_SIZE_EXTRA_LARGE] = 100;
        }

        if (cnt != 0) {
            sub_41B3A0();
        }

        mes_unload(tmp_mes_file);
    }

    settings_add(&settings, "effects volume", "5", gsound_effects_volume_changed);
    settings_add(&settings, "voice volume", "5", gsound_voice_volume_changed);
    settings_add(&settings, "music volume", "5", gsound_music_volume_changed);
    gsound_effects_volume_changed();
    gsound_voice_volume_changed();
    gsound_music_volume_changed();
    gsound_combat_music_active = false;
    dword_5D1A70 = 0;

    return true;
}

// 0x41AF80
const char* gsound_build_sound_path(const char* name)
{
    // 0x5D5490
    static char path[MAX_PATH];

    sprintf(path, "%s%s", gsound_base_sound_path, name);

    return path;
}

// 0x41AFB0
void sub_41AFB0(SoundSchemeList* scheme)
{
    int index;

    scheme->field_0 = 0;
    scheme->field_8 = 0;

    for (index = 0; index < TWENTY_FIVE; index++) {
        memset(&(scheme->field_C[index]), 0, sizeof(*scheme->field_C));
        scheme->field_C[index].sound_handle = TIG_SOUND_HANDLE_INVALID;
    }
}

// 0x41AFF0
void gsound_exit()
{
    int index;

    gsound_initialized = false;

    for (index = 0; index < dword_5D55B0; index++) {
        mes_unload(dword_5D1A68[index]);
    }

    FREE(dword_5D1A68);
    dword_5D1A68 = NULL;

    dword_5D55B0 = 0;

    mes_unload(dword_5D5480);
    mes_unload(dword_5D1A40);
}

// 0x41B060
void gsound_reset()
{
    qword_5D1A28 = 0;
    qword_5D55E8 = 0;
    qword_5D55E0 = 0;
    gsound_combat_music_active = false;
    dword_5D1A70 = 0;
    gsound_stop_all(0);
}

// 0x41B0A0
bool gsound_mod_load()
{
    mes_load(gsound_build_sound_path("snd_user.mes"), &dword_5D5598);
    return true;
}

// 0x41B0D0
void gsound_mod_unload()
{
    mes_unload(dword_5D5598);
    dword_5D5598 = MES_FILE_HANDLE_INVALID;
}

// 0x41B0F0
bool gsound_load(GameLoadInfo* load_info)
{
    int temp[TWO];
    int index;

    for (index = 0; index < TWO; index++) {
        if (tig_file_fread(&(temp[index]), sizeof(*temp), 1, load_info->stream) != 1) {
            return false;
        }
    }

    if (tig_file_fread(&dword_5D5594, sizeof(dword_5D5594), 1, load_info->stream) != 1) {
        return false;
    }

    for (index = 0; index < TWO; index++) {
        if (tig_file_fread(&(dword_5D1A38[index]), sizeof(*dword_5D1A38), 1, load_info->stream) != 1) {
            return false;
        }
    }

    if (tig_file_fread(&gsound_combat_music_active, sizeof(gsound_combat_music_active), 1, load_info->stream) != 1) {
        return false;
    }

    for (index = 0; index < TWO; index++) {
        if (tig_file_fread(&(dword_5D1A30[index]), sizeof(*dword_5D1A30), 1, load_info->stream) != 1) {
            return false;
        }
    }

    sub_41BD50(temp[0], temp[1]);

    return true;
}

// 0x41B1D0
bool gsound_save(TigFile* stream)
{
    int index;
    int v1;

    for (index = 0; index < TWO; index++) {
        v1 = stru_5D1A98[index].field_0 ? stru_5D1A98[index].field_4 : 0;
        if (tig_file_fwrite(&v1, sizeof(v1), 1, stream) != 1) {
            return false;
        }
    }

    if (tig_file_fwrite(&dword_5D5594, sizeof(dword_5D5594), 1, stream) != 1) {
        return false;
    }

    for (index = 0; index < TWO; index++) {
        if (tig_file_fwrite(&(dword_5D1A38[index]), sizeof(*dword_5D1A38), 1, stream) != 1) {
            return false;
        }
    }

    if (tig_file_fwrite(&gsound_combat_music_active, sizeof(gsound_combat_music_active), 1, stream) != 1) {
        return false;
    }

    for (index = 0; index < TWO; index++) {
        if (tig_file_fwrite(&(dword_5D1A30[index]), sizeof(*dword_5D1A30), 1, stream) != 1) {
            return false;
        }
    }
}

// 0x41B2A0
void gsound_ping(int a1)
{
    int delta;

    if (gsound_initialized) {
        delta = abs(a1 - dword_5D548C);
        if (delta > 250) {
            dword_5D548C = a1;
            sub_41BAF0();
        }
    }
}

// 0x41B2D0
void gsound_flush()
{
    gsound_reset();
}

// 0x41B2E0
tig_sound_handle_t gsound_play_sfx(const char* path, int loops, int volume, int extra_volume, int id)
{
    tig_sound_handle_t sound_handle;

    if (!gsound_initialized) {
        return TIG_SOUND_HANDLE_INVALID;
    }

    if (dword_5D1A70) {
        return TIG_SOUND_HANDLE_INVALID;
    }

    tig_sound_create(&sound_handle, TIG_SOUND_TYPE_EFFECT);
    tig_sound_set_loops(sound_handle, loops);
    tig_sound_set_volume(sound_handle, volume * gsound_effects_volume / 100);
    tig_sound_set_extra_volume(sound_handle, extra_volume);
    tig_sound_play(sound_handle, path, id);
    if (!tig_sound_is_active(sound_handle)) {
        return TIG_SOUND_HANDLE_INVALID;
    }

    return sound_handle;
}

// 0x41B3A0
void sub_41B3A0()
{
    tig_sound_enumerate_positional(sub_41B3B0);
}

// 0x41B3B0
void sub_41B3B0(tig_sound_handle_t sound_handle)
{
    int64_t x;
    int64_t y;
    TigSoundPositionalSize size;
    int volume;
    int extra_volume;

    tig_sound_get_position(sound_handle, &x, &y);
    size = tig_sound_get_positional_size(sound_handle);
    sub_41B420(x, y, &volume, &extra_volume, size);
    tig_sound_set_volume(sound_handle, adjust_volume(sound_handle, volume));
    tig_sound_set_extra_volume(sound_handle, extra_volume);
}

// 0x41B420
void sub_41B420(int64_t x, int64_t y, int* volume_ptr, int* extra_volume_ptr, TigSoundPositionalSize size)
{
    // TODO: Incomplete.
}

// 0x41B720
tig_sound_handle_t gsound_play_sfx_id_ex(int id, int loops, int volume, int extra_volume)
{
    char path[MAX_PATH];

    if (!gsound_initialized) {
        return TIG_SOUND_HANDLE_INVALID;
    }

    gsound_resolve_path(id, path);
    return gsound_play_sfx(path, loops, volume, extra_volume, id);
}

// 0x41B780
tig_sound_handle_t gsound_play_sfx_id(int id, int loops)
{
    return gsound_play_sfx_id_ex(id, loops, 127, 64);
}

// 0x41B7A0
tig_sound_handle_t sub_41B7A0(int id, int loops, int64_t x, int64_t y)
{
    return sub_41B7D0(id, loops, x, y, TIG_SOUND_SIZE_LARGE);
}

// 0x41B7D0
tig_sound_handle_t sub_41B7D0(int id, int loops, int64_t x, int64_t y, int size)
{
    int volume;
    int extra_volume;
    tig_sound_handle_t sound_handle;

    if (!gsound_initialized) {
        return TIG_SOUND_HANDLE_INVALID;
    }

    sub_41B420(x, y, &volume, &extra_volume, size);

    sound_handle = gsound_play_sfx_id_ex(id, loops, volume, extra_volume);
    tig_sound_set_position(sound_handle, x, y);
    tig_sound_set_positional_size(sound_handle, size);

    return sound_handle;
}

// 0x41B850
tig_sound_handle_t sub_41B850(int id, int loops, int64_t location)
{
    return sub_41B870(id, loops, location, TIG_SOUND_SIZE_LARGE);
}

// 0x41B870
tig_sound_handle_t sub_41B870(int id, int loops, int64_t location, int size)
{
    int64_t x;
    int64_t y;

    if (!gsound_initialized) {
        return TIG_SOUND_HANDLE_INVALID;
    }

    if (qword_5D1A28 == OBJ_HANDLE_NULL) {
        qword_5D1A28 = location;
        sub_4B8680(location, &qword_5D55E8, &qword_5D55E0);
    }

    sub_4B8680(location, &x, &y);

    return sub_41B7D0(id, loops, x - qword_5D55E8, y - qword_5D55E0, size);
}

// 0x41B930
tig_sound_handle_t sub_41B930(int id, int loops, int64_t obj)
{
    int64_t location;
    int size;

    if (!gsound_initialized) {
        return TIG_SOUND_HANDLE_INVALID;
    }

    location = obj_field_int64_get(obj, OBJ_F_LOCATION);
    size = gsound_get_positional_size(obj);

    return sub_41B870(id, loops, location, size);
}

// 0x41B980
TigSoundPositionalSize gsound_get_positional_size(object_id_t object_id)
{
    unsigned int flags;

    if (obj_field_int32_get(object_id, OBJ_F_TYPE) == OBJ_TYPE_SCENERY) {
        flags = obj_field_int32_get(object_id, OBJ_F_SCENERY_FLAGS);
        if ((flags & OSCF_SOUND_SMALL) != 0) {
            return TIG_SOUND_SIZE_SMALL;
        }
        if ((flags & OSCF_SOUND_MEDIUM) != 0) {
            return TIG_SOUND_SIZE_MEDIUM;
        }
        if ((flags & OSCF_SOUND_EXTRA_LARGE) != 0) {
            return TIG_SOUND_SIZE_EXTRA_LARGE;
        }
    }

    return TIG_SOUND_SIZE_LARGE;
}

// 0x41BA20
void sub_41BA20(int fade_duration, int index)
{
    SoundSchemeList* scheme;
    SoundScheme* sound;
    int snd;

    scheme = &(stru_5D1A98[index]);
    if (scheme->field_0) {
        if (!dword_5D5594) {
            dword_5D1A38[index] = scheme->field_4;
        }

        for (snd = 0; snd < TWENTY_FIVE; snd++) {
            sound = &(scheme->field_C[snd]);
            if (sound->field_0) {
                if (sound->sound_handle != TIG_SOUND_HANDLE_INVALID) {
                    tig_sound_stop(sound->sound_handle, fade_duration);
                    sound->sound_handle = TIG_SOUND_HANDLE_INVALID;
                }
            }
        }
    } else {
        if (!dword_5D5594) {
            dword_5D1A38[index] = 0;
        }
    }
}

// 0x41B9E0
int sub_41B9E0(object_id_t obj)
{
    TigSoundPositionalSize size;

    size = gsound_get_positional_size(obj);
    return (int)sound_maximum_radius[size] / 40;
}

// 0x41BAC0
void sub_41BAC0(int fade_duration)
{
    int index;

    if (gsound_initialized) {
        for (index = 0; index < TWO; index++) {
            sub_41BA20(fade_duration, index);
        }
    }
}

// 0x41BCD0
void sub_41BCD0(const char* name, char* buffer)
{
    if (name[0] == '#') {
        gsound_resolve_path(atoi(name + 1), buffer);
    } else {
        sprintf(buffer, "%s%s", gsound_base_sound_path, name);
    }
}

// 0x41BD10
bool sub_41BD10(int a1, int* a2)
{
    int index;

    for (index = 0; index < TWO; index++) {
        if (stru_5D1A98[index].field_0 != 0) {
            if (stru_5D1A98[index].field_4 == a1) {
                *a2 = index;
                return true;
            }
        }
    }

    return false;
}

// 0x41BD50
void sub_41BD50(int a1, int a2)
{
    bool v1;
    int v2;
    bool v3;
    int v4;

    if (gsound_initialized && !dword_5D1A70) {
        if (a1 == a2) {
            a2 = 0;
        }

        if (gsound_combat_music_active) {
            dword_5D1A30[1] = a2;
            dword_5D1A30[0] = a1;
        } else {
            v1 = sub_41BD10(a1, &v2);
            v3 =  sub_41BD10(a2, &v4);
            if (v1) {
                if (!v3) {
                    sub_41BA20(25, 1 - v2);
                    sub_41BE20(a2);
                }
            } else {
                if (v3) {
                    sub_41BA20(25, 1 - v4);
                    sub_41BE20(a1);
                } else {
                    sub_41BAC0(25);
                    sub_41BE20(a1);
                    sub_41BE20(a2);
                }
            }
        }
    }
}

// 0x41BE20
void sub_41BE20(int num)
{
    SoundSchemeList* scheme;
    int index;
    MesFileEntry mes_file_entry;
    char* hash;
    SoundScheme* sound;
    char path[TIG_MAX_PATH];

    if (num == 0) {
        return;
    }

    for (index = 0; index < TWO; index++) {
        if (stru_5D1A98[index].field_0 == 0) {
            break;
        }
    }

    if (index == TWO) {
        return;
    }

    scheme = &(stru_5D1A98[index]);
    sub_41AFB0(scheme);

    scheme->field_4 = num;

    mes_file_entry.num = num;
    if (!mes_search(dword_5D5480, &mes_file_entry)) {
        return;
    }

    hash = strchr(mes_file_entry.str, '#');
    if (hash == NULL) {
        return;
    }

    scheme->field_0 = atoi(hash + 1);
    scheme->field_8 = 0;

    for (index = 0; index < 100; index++) {
        mes_file_entry.num = scheme->field_0 + index;
        if (mes_search(dword_5D1A40, &mes_file_entry)) {
            sound = sub_41BF70(scheme, mes_file_entry.str);
            if (sound != NULL) {
                if (sound->field_0
                    && !sound->field_1) {
                    sub_41BCD0(sound->path, path);
                    tig_sound_create(&(sound->sound_handle), TIG_SOUND_TYPE_MUSIC);
                    tig_sound_set_volume(sound->sound_handle, gsound_music_volume * sound->volume_min / 100);
                    tig_sound_play_streamed_once(sound->sound_handle, path, 25, TIG_SOUND_HANDLE_INVALID);
                }

                if (sound->field_2) {
                    dword_5D5594 = true;
                }
            }
        }
    }
}

// 0x41BF70
SoundScheme* sub_41BF70(SoundSchemeList* a1, const char* path)
{
    SoundScheme* scheme = NULL;
    char* copy;
    char* pch;
    int v1;
    int v2;

    copy = STRDUP(path);
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
            scheme->sound_handle = TIG_SOUND_HANDLE_INVALID;
            scheme->path[0] = '\0';
            sub_41C260(copy);

            pch = strchr(copy, ' ');
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

                    v1 = -1;
                    v2 = -1;
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

        FREE(copy);

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
        *str = (unsigned char)tolower(*(unsigned char*)str);
        str++;
    }
}

// 0x41C290
void sub_41C290(const char* str, const char* key, int* value1, int* value2)
{
    const char* pch;
    char* copy;
    char* space;
    char* sep;

    pch = strstr(str, key);
    if (pch != NULL) {
        copy = STRDUP(pch + strlen(key));
        if (copy != NULL) {
            space = strchr(copy, ' ');
            if (space != NULL) {
                *space = '\0';
            }

            if (value1 != NULL) {
                *value1 = atoi(copy);

                if (value2 != NULL) {
                    sep = strchr(copy, ',');
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

            FREE(copy);
        }
    }
}

// 0x41C340
void gsound_stop_all(int fade_duration)
{
    if (gsound_initialized) {
        sub_41BAC0(fade_duration);
        dword_5D5594 = false;
        tig_sound_stop_all(fade_duration);
    }
}

// 0x41C370
tig_sound_handle_t gsound_play_voice(const char* path, int fade_duration)
{
    tig_sound_handle_t sound_handle;

    if (dword_5D1A70) {
        return TIG_SOUND_HANDLE_INVALID;
    }

    tig_sound_create(&sound_handle, TIG_SOUND_TYPE_VOICE);
    tig_sound_set_volume(sound_handle, gsound_voice_volume);
    tig_sound_play_streamed_once(sound_handle, path, fade_duration, -1);
    if (!tig_sound_is_active(sound_handle)) {
        return TIG_SOUND_HANDLE_INVALID;
    }

    return sound_handle;
}

// 0x41C3D0
tig_sound_handle_t gsound_play_music_once(const char* path, int fade_duration)
{
    tig_sound_handle_t sound_handle;

    if (dword_5D1A70) {
        return TIG_SOUND_HANDLE_INVALID;
    }

    tig_sound_create(&sound_handle, TIG_SOUND_TYPE_MUSIC);
    tig_sound_set_volume(sound_handle, 100 * gsound_music_volume / 100);
    tig_sound_play_streamed_once(sound_handle, path, fade_duration, -1);
    if (!tig_sound_is_active(sound_handle)) {
        return TIG_SOUND_HANDLE_INVALID;
    }

    return sound_handle;
}

// 0x41C450
tig_sound_handle_t gsound_play_music_indefinitely(const char* path, int fade_duration)
{
    tig_sound_handle_t sound_handle;

    if (dword_5D1A70) {
        return TIG_SOUND_HANDLE_INVALID;
    }

    tig_sound_create(&sound_handle, TIG_SOUND_TYPE_MUSIC);
    tig_sound_set_volume(sound_handle, 100 * gsound_music_volume / 100);
    tig_sound_play_streamed_indefinitely(sound_handle, path, fade_duration, -1);
    if (!tig_sound_is_active(sound_handle)) {
        return TIG_SOUND_HANDLE_INVALID;
    }

    return sound_handle;
}

// 0x41C4D0
void gsound_start_combat_music(int64_t obj)
{
    char path[MAX_PATH];
    int index;

    if (!gsound_combat_music_active) {
        if (obj != OBJ_HANDLE_NULL) {
            if (obj_field_int32_get(obj, OBJ_F_TYPE) == OBJ_TYPE_NPC) {
                for (index = 0; index < TWO; index++) {
                    if (stru_5D1A98[index].field_0) {
                        dword_5D1A30[index] = stru_5D1A98[index].field_4;
                    } else {
                        dword_5D1A30[index] = 0;
                    }
                }

                sub_41BAC0(25);

                gsound_combat_music_active = true;

                sprintf(path, "sound\\music\\combat %d.mp3", random_between(1, 6));
                dword_5D1A4C = gsound_play_music_once(path, 0);
                dword_5D1A48 = gsound_play_music_indefinitely("sound\\music\\combatmusic.mp3", 0);
            }
        }
    }
}

// 0x41C5A0
void gsound_stop_combat_music(int64_t obj)
{
    if (gsound_combat_music_active) {
        if (obj != OBJ_HANDLE_NULL) {
            if (obj_field_int32_get(obj, OBJ_F_TYPE) == OBJ_TYPE_PC) {
                tig_sound_destroy(dword_5D1A4C);
                tig_sound_destroy(dword_5D1A48);

                gsound_combat_music_active = false;

                sub_41BD50(dword_5D1A30[0], dword_5D1A30[1]);
            }
        }
    }
}

// 0x41C610
void sub_41C610()
{
    int index;

    if (dword_5D1A70 == 0) {
        for (index = 0; index < TWO; index++) {
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

// 0x41C690
void sub_41C690(int64_t a1, int64_t a2)
{
    if (gsound_initialized) {
        qword_5D1A50 = a1;
        qword_5D1A58 = a2;
        sub_41B3A0();
    }
}

// 0x41C6D0
void sub_41C6D0(int64_t location)
{
    int64_t x;
    int64_t y;
    if (gsound_initialized) {
        sub_4B8680(location, &x, &y);

        if (qword_5D1A28 != 0) {
            sub_4B8680(qword_5D1A28, &qword_5D55E8, &qword_5D55E0);
        }

        sub_41C690(x - qword_5D55E8, y - qword_5D55E0);
    }
}

// 0x41C780
void sub_41C780(tig_sound_handle_t sound_handle, int64_t location)
{
    int64_t x;
    int64_t y;
    TigSoundPositionalSize size;
    int volume;
    int extra_volume;

    sub_4B8680(location, &x, &y);
    x -= qword_5D55E8;
    y -= qword_5D55E0;
    tig_sound_set_position(sound_handle, x, y);
    size = tig_sound_get_positional_size(sound_handle);
    sub_41B420(x, y, &volume, &extra_volume, size);
    tig_sound_set_volume(sound_handle, adjust_volume(sound_handle, volume));
    tig_sound_set_extra_volume(sound_handle, extra_volume);
}

// 0x41C850
void gsound_set_defaults(int min_radius, int max_radius, int a3, int a4)
{
    if (!gsound_initialized) {
        return;
    }

    sound_minimum_radius[TIG_SOUND_SIZE_LARGE] = min_radius;
    sound_maximum_radius[TIG_SOUND_SIZE_LARGE] = max_radius;
    qword_5D1A60 = a3;
    qword_5D1A20 = a4;

    sub_41B3A0();
}

// 0x41C8A0
int gsound_effects_volume_get()
{
    return gsound_effects_volume;
}

// 0x41C8B0
void gsound_effects_volume_changed()
{
    object_id_t obj;
    int64_t location;

    gsound_effects_volume = 80 * (127 * settings_get_value(&settings, "effects volume") / 10) / 100;
    tig_sound_set_volume_by_type(TIG_SOUND_TYPE_EFFECT, gsound_effects_volume);

    obj = player_get_pc_obj();
    if (obj != OBJ_HANDLE_NULL) {
        location = obj_field_int64_get(obj, OBJ_F_LOCATION);
        sub_41C6D0(location);
    }
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
int adjust_volume(tig_sound_handle_t sound_handle, int volume)
{
    switch (tig_sound_get_type(sound_handle)) {
    case TIG_SOUND_TYPE_EFFECT:
        return volume * gsound_effects_volume_get() / 127;
    case TIG_SOUND_TYPE_MUSIC:
        return volume * gsound_music_volume_get() / 127;
    case TIG_SOUND_TYPE_VOICE:
        return volume * gsound_voice_volume_get() / 127;
    }

    return volume;
}
