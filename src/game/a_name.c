#include "game/a_name.h"

#include <stdio.h>

#include "game/mes.h"

static bool build_tile_file_name(const char* name1, const char* name2, int a3, int a4, char* fname);
static bool sub_4EB0C0(int num, int type, int flippable, char** name_ptr);
static bool count_tile_names();
static bool build_roof_file_name(int index, char* buffer);
static bool load_roof_data();

// 0x603AE0
static char** outdoor_non_flippable_tile_names;

// 0x603AE4
static bool dword_603AE4;

// 0x603AE8
static uint8_t* outdoor_flippable_tile_flags;

// 0x603AEC
static int* indoor_non_flippable_tile_sounds;

// 0x603AF0
static int* outdoor_non_flippable_tile_sounds;

// 0x603AF4
static uint8_t* indoor_non_flippable_tile_flags;

// 0x603AF8
static int* dword_603AF8;

// 0x603AFC
static uint8_t* indoor_flippable_tile_flags;

// 0x603B00
static mes_file_handle_t tilename_mes_file;

// 0x603B04
static char** indoor_flippable_tile_names;

// 0x603B08
static int num_indoor_flippable_names;

// 0x603B0C
static int* outdoor_flippable_tile_sounds;

// 0x603B10
static char** indoor_non_flippable_tile_names;

// 0x603B14
static char** outdoor_flippable_tile_names;

// 0x603B18
static int num_outdoor_non_flippable_names;

// 0x603B1C
static int num_outdoor_flippable_names;

// 0x603B20
static int* indoor_flippable_tile_sounds;

// 0x603B24
static uint8_t* outdoor_non_flippable_tile_flags;

// 0x603B28
static int num_indoor_non_flippable_names;

// 0x603B6C
static bool light_initialized;

// 0x603B70
static mes_file_handle_t light_mes_file;

// 0x603B74
static char **roof_file_names;

// 0x603B78
static mes_file_handle_t roofname_mes_file;

// 0x603B7C
static int num_roof_file_names;

// 0x603B80
static int roof_initialized;

// 0x687660
static int* dword_687660[7];

// 0x687680
static int dword_687680[7];

// 0x4EAC80
bool a_name_tile_init()
{
    if (!mes_load("art\\tile\\tilename.mes", &tilename_mes_file)) {
        return false;
    }

    if (!count_tile_names()
        || !load_tile_names()
        || !load_tile_edges()) {
        mes_unload(tilename_mes_file);
        return false;
    }

    dword_687680[0] = 16 * (num_outdoor_flippable_names + 4);
    dword_687680[1] = 16 * (num_outdoor_non_flippable_names + 4);
    dword_687680[2] = 16 * (num_indoor_flippable_names + 4);
    dword_687680[3] = 16 * (num_indoor_non_flippable_names + 4);
    dword_687680[4] = 16 * (num_outdoor_flippable_names * num_outdoor_flippable_names + 4);
    dword_687680[5] = 16 * (num_outdoor_flippable_names * num_outdoor_non_flippable_names + 4);
    dword_687680[6] = 16 * (num_outdoor_non_flippable_names * num_outdoor_non_flippable_names + 4);

    return true;
}

// 0x4EAD50
void a_name_tile_exit()
{
    mes_unload(tilename_mes_file);

    FREE(dword_603AF8);
    FREE(outdoor_flippable_tile_names);
    FREE(outdoor_non_flippable_tile_names);
    FREE(indoor_flippable_tile_names);
    FREE(indoor_non_flippable_tile_names);
    FREE(outdoor_flippable_tile_flags);
    FREE(outdoor_non_flippable_tile_flags);
    FREE(indoor_flippable_tile_flags);
    FREE(indoor_non_flippable_tile_flags);
    FREE(outdoor_flippable_tile_sounds);
    FREE(outdoor_non_flippable_tile_sounds);
    FREE(indoor_flippable_tile_sounds);
    FREE(indoor_non_flippable_tile_sounds);

    if (dword_603AE4) {
        int index;

        for (index = 0; index < 7; index++) {
            FREE(dword_687660[index]);
        }

        dword_603AE4 = false;
    }
}

// 0x4EAE90
bool a_name_tile_aid_to_fname(tig_art_id_t aid, char* fname)
{
    int num1;
    int num2;
    int v1;
    int v2;
    int type;
    int flippable1;
    int flippable2;
    char* name1;
    char* name2;

    if (tig_art_type(aid) != TIG_ART_TYPE_TILE) {
        tig_debug_println("Asking for tile name with non tile art ID.");
        fname[0] = '\0';
        return false;
    }

    num1 = tig_art_tile_id_num1_get(aid);
    num2 = tig_art_tile_id_num2_get(aid);
    v1 = sub_503700(aid);
    v2 = sub_5037B0(aid);
    type = tig_art_tile_id_type_get(aid);
    flippable1 = tig_art_tile_id_flippable1_get(aid);
    flippable2 = tig_art_tile_id_flippable2_get(aid);

    if (!sub_4EB0C0(num1, type, flippable1, &name1)
        || !sub_4EB0C0(num2, type, flippable2, &name2)) {
        return false;
    }

    return build_tile_file_name(name1, name2, v1, v2, fname);
}

// 0x4EAF70
bool build_tile_file_name(const char* name1, const char* name2, int a3, int a4, char* fname)
{
    // 0x5BB4E4
    static const char off_5BB4E4[] = "06b489237ea5dc10";

    int v1;
    int v2;

    if (a4 >= 8) {
        a4 -= 8;
    }

    if (a3 == 15 || strcmpi(name1, name2) == 0) {
        sprintf(fname,
            "art\\tile\\%sbse%c%c.art",
            name1,
            off_5BB4E4[a3],
            a4 + 'a');
        return true;
    }

    if (a3 == 0) {
        sprintf(fname,
            "art\\tile\\%sbse%c%c.art",
            name2,
            off_5BB4E4[0],
            a4 + 'a');
        return true;
    }

    if (!sub_4EB7D0(name1, &v1)) {
        sprintf(fname,
            "art\\tile\\%sbse%c%c.art",
            name1,
            off_5BB4E4[a3],
            a4 + 'a');
        return true;
    }

    if (!sub_4EB7D0(name2, &v2)) {
        sprintf(fname,
            "art\\tile\\%sbse%c%c.art",
            name2,
            off_5BB4E4[15 - a3],
            a4 + 'a');
        return true;
    }

    if (v1 < v2) {
        sprintf(fname,
            "art\\tile\\%s%s%c%c.art",
            name1,
            name2,
            off_5BB4E4[a3],
            a4 + 'a');
    } else {
        sprintf(fname,
            "art\\tile\\%s%s%c%c.art",
            name2,
            name1,
            off_5BB4E4[15 - a3],
            a4 + 'a');
    }
    return true;
}

// 0x4EB0C0
bool sub_4EB0C0(int num, int type, int flippable, char** name_ptr)
{
    if (flippable) {
        if (type) {
            if (num < num_outdoor_flippable_names) {
                *name_ptr = outdoor_flippable_tile_names[num];
                return true;
            }
        } else {
            if (num < num_indoor_flippable_names) {
                *name_ptr = indoor_flippable_tile_names[num];
                return true;
            }
        }
    } else {
        if (type) {
            if (num < num_outdoor_non_flippable_names) {
                *name_ptr = outdoor_non_flippable_tile_names[num];
                return true;
            }
        } else {
            if (num < num_indoor_non_flippable_names) {
                *name_ptr = indoor_non_flippable_tile_names[num];
                return true;
            }
        }
    }
    return false;
}

// 0x4EB270
bool count_tile_names()
{
    num_outdoor_flippable_names = mes_entries_count_in_range(tilename_mes_file, 0, 99);
    num_outdoor_non_flippable_names = mes_entries_count_in_range(tilename_mes_file, 100, 199);
    num_indoor_flippable_names = mes_entries_count_in_range(tilename_mes_file, 200, 299);
    num_indoor_non_flippable_names = mes_entries_count_in_range(tilename_mes_file, 300, 399);
    return true;
}

// 0x4ED1E0
bool a_name_light_init()
{
    if (light_initialized) {
        return true;
    }

    if (!mes_load("art\\light\\light.mes", &light_mes_file)) {
        return false;
    }

    return light_initialized;
}

// 0x4ED220
void a_name_light_exit()
{
    if (light_initialized) {
        mes_unload(light_mes_file);
        light_initialized = false;
    }
}

// 0x4ED250
bool a_name_light_aid_to_fname(tig_art_id_t aid, char* fname)
{
    MesFileEntry mes_file_entry;

    if (!light_initialized) {
        return false;
    }

    mes_file_entry.num = tig_art_num_get(aid);
    if (!mes_search(light_mes_file, &mes_file_entry)) {
        return false;
    }

    if (sub_504790(aid)) {
        sprintf(fname, "art\\light\\%s_s%d.art", mes_file_entry.str, sub_504700(aid) / 8);
    } else {
        sprintf(fname, "art\\light\\%s.art", mes_file_entry.str);
    }

    return true;
}

// 0x4ED2F0
bool a_name_roof_init()
{
    if (!load_roof_data()) {
        return false;
    }

    roof_initialized = true;

    return true;
}

// 0x4ED310
void a_name_roof_exit()
{
    if (roof_initialized) {
        mes_unload(roofname_mes_file);
        FREE(roof_file_names);
        roof_initialized = false;
    }
}

// 0x4ED370
bool a_name_roof_aid_to_fname(tig_art_id_t aid, char* fname)
{
    if (tig_art_type(aid) != TIG_ART_TYPE_ROOF) {
        return false;
    }

    return build_roof_file_name(tig_art_num_get(aid), fname);
}

// 0x4ED3A0
bool build_roof_file_name(int index, char* fname)
{
    if (index >= num_roof_file_names) {
        return false;
    }

    sprintf(fname, "art\\roof\\%s.art", roof_file_names[index]);

    return true;
}

// 0x4ED3E0
bool a_name_roof_fname_to_aid(const char* fname, tig_art_id_t* aid_ptr)
{
    int index;

    for (index = 0; index < num_roof_file_names; index++) {
        if (strcmpi(fname, roof_file_names[index]) == 0) {
            tig_art_roof_id_create(index, 0, 0, 0, aid_ptr);
            return true;
        }
    }

    return false;
}

// 0x4ED440
bool load_roof_data()
{
    MesFileEntry mes_file_entry;
    int index;

    num_roof_file_names = 0;
    roof_file_names = NULL;

    if (!mes_load("art\\roof\\roofname.mes", &roofname_mes_file)) {
        return false;
    }

    num_roof_file_names = mes_entries_count(roofname_mes_file);
    if (num_roof_file_names == 0) {
        mes_unload(roofname_mes_file);
        return false;
    }

    mes_file_entry.num = 0;
    if (!mes_search(roofname_mes_file, &mes_file_entry)) {
        mes_unload(roofname_mes_file);
        return false;
    }

    roof_file_names = MALLOC(sizeof(char*) * num_roof_file_names);
    index = 0;
    do {
        roof_file_names[index++] = mes_file_entry.str;
    } while (mes_find_next(roofname_mes_file, &mes_file_entry));

    return true;
}
