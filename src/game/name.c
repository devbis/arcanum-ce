#include "game/name.h"

#include <stdio.h>

#include "game/a_name.h"
#include "game/mes.h"

static void sub_41CE60();
static bool sub_41D1F0();
static bool sub_41D2C0();
static int sub_41D390();
static void sub_41DA40(int a1, int a2, unsigned int* a3, int* a4, int* a5);
static tig_art_id_t sub_41DFC0(int type, int* extra);
static tig_art_id_t sub_41E200(tig_art_id_t art_id);
static int sub_41E960(int a1);
static bool sub_41F190(TigMessage* msg);
static void sub_41F240();

// 0x5A1064
static const char* off_5A1064 = "art";

// 0x5A1068
static const char* off_5A1068 = "art\\missing.dat";

// 0x5A106C
static int dword_5A106C[] = {
    1,
    2,
    4,
    8,
    0x10,
    0x20,
    0x40,
    0x80,
    0x100,
    0x200,
    0x400,
    0x800,
    0x1000,
    0x2000,
    0x4000,
    0x8000,
    0x10000,
    0x20000,
    0x40000,
};

// 0x5A10B8
static int dword_5A10B8[] = {
    1,
    6,
    23,
    0,
    21,
    21,
    21,
    21,
    21,
    21,
    21,
    21,
    21,
    21,
    17,
    18,
    19,
    24,
    3,
};

// 0x5A1104
static int dword_5A1104[] = {
    1,
    1,
    1,
    0,
    1,
    2,
    3,
    4,
    5,
    6,
    7,
    8,
    10,
    13,
    1,
    1,
    1,
    1,
    0,
};

// 0x5A1150
static int dword_5A1150[] = {
    1,
    1,
    0,
    0,
    20,
    20,
    20,
    20,
    20,
    20,
    20,
    20,
    20,
    20,
    7,
    7,
    7,
    7,
    1,
};

// 0x5A119C
static char byte_5A119C[2] = {
    'X',
    'S',
};

// 0x5A11A0
static char byte_5A11A0[3] = {
    'F',
    'M',
    'X',
};

// 0x5A11A4
static const char *off_5A11A4[] = {
    "HM",
    "DF",
    "GH",
    "HG",
    "EF",
};

// 0x5A11B8
static const char *off_5A11B8[] = {
    "UW",
    "V1",
    "LA",
    "CM",
    "PM",
    "RB",
    "PC",
    "BN",
    "CD",
};

// 0x5A11DC
static char byte_5A11DC[15] = {
    'A',
    'B',
    'C',
    'D',
    'E',
    'F',
    'G',
    'H',
    'I',
    'J',
    'K',
    'X',
    'Y',
    'N',
    'Z',
};

// 0x5A11EC
static char byte_5A11EC[3] = {
    'F',
    'B',
    'U',
};

// 0x5D55F0
static int dword_5D55F0;

// 0x5D55F4
static mes_file_handle_t name_monster_mes_file;

// 0x5D55F8
static mes_file_handle_t name_eye_candy_mes_file;

// 0x5D55FC
static mes_file_handle_t name_unique_npc_mes_file;

// 0x5D5600
static int dword_5D5600;

// 0x5D5604
static mes_file_handle_t name_container_mes_file;

// 0x5D5608
static unsigned int* dword_5D5608;

// 0x5D560C
static unsigned int* dword_5D560C;

// 0x5D5610
static unsigned int* dword_5D5610;

// 0x5D5614
static mes_file_handle_t name_interface_mes_file;

// 0x5D5618
static int dword_5D5618;

// 0x5D561C
static mes_file_handle_t name_scenery_mes_file;

// 0x5D5620
static bool name_initialized;

// 0x5D5624
static bool dword_5D5624;

// 0x739E48
static tig_window_handle_t dword_739E48;

// 0x739E4C
static int dword_739E4C;

// 0x739E50
static int* dword_739E50;

// 0x739E54
static int dword_739E54;

// 0x739E58
static int dword_739E58;

// 0x41CA40
bool name_init(GameInitInfo* init_info)
{
    (void)init_info;

    if (name_initialized) {
        return true;
    }

    if (!mes_load("art\\scenery\\scenery.mes", &name_scenery_mes_file)) {
        return false;
    }

    if (!mes_load("art\\interface\\interface.mes", &name_interface_mes_file)) {
        mes_unload(name_scenery_mes_file);
        return false;
    }

    if (!mes_load("art\\unique_npc\\unique_npc.mes", &name_unique_npc_mes_file)) {
        mes_unload(name_interface_mes_file);
        mes_unload(name_scenery_mes_file);
        return false;
    }

    if (!mes_load("art\\monster\\monster.mes", &name_monster_mes_file)) {
        mes_unload(name_unique_npc_mes_file);
        mes_unload(name_interface_mes_file);
        mes_unload(name_scenery_mes_file);
        return false;
    }

    if (!mes_load("art\\eye_candy\\eye_candy.mes", &name_eye_candy_mes_file)) {
        mes_unload(name_monster_mes_file);
        mes_unload(name_unique_npc_mes_file);
        mes_unload(name_interface_mes_file);
        mes_unload(name_scenery_mes_file);
        return false;
    }

    if (!mes_load("art\\container\\container.mes", &name_container_mes_file)) {
        mes_unload(name_eye_candy_mes_file);
        mes_unload(name_monster_mes_file);
        mes_unload(name_unique_npc_mes_file);
        mes_unload(name_interface_mes_file);
        mes_unload(name_scenery_mes_file);
        return false;
    }

    if (!a_name_light_init()) {
        mes_unload(name_container_mes_file);
        mes_unload(name_eye_candy_mes_file);
        mes_unload(name_monster_mes_file);
        mes_unload(name_unique_npc_mes_file);
        mes_unload(name_interface_mes_file);
        mes_unload(name_scenery_mes_file);
        return false;
    }

    if (!a_name_tile_init()) {
        a_name_light_exit();
        mes_unload(name_container_mes_file);
        mes_unload(name_eye_candy_mes_file);
        mes_unload(name_monster_mes_file);
        mes_unload(name_unique_npc_mes_file);
        mes_unload(name_interface_mes_file);
        mes_unload(name_scenery_mes_file);
        return false;
    }

    if (!a_name_roof_init()) {
        a_name_tile_exit();
        a_name_light_exit();
        mes_unload(name_container_mes_file);
        mes_unload(name_eye_candy_mes_file);
        mes_unload(name_monster_mes_file);
        mes_unload(name_unique_npc_mes_file);
        mes_unload(name_interface_mes_file);
        mes_unload(name_scenery_mes_file);
        return false;
    }

    if (!a_name_wall_init()) {
        a_name_roof_exit();
        a_name_tile_exit();
        a_name_light_exit();
        mes_unload(name_container_mes_file);
        mes_unload(name_eye_candy_mes_file);
        mes_unload(name_monster_mes_file);
        mes_unload(name_unique_npc_mes_file);
        mes_unload(name_interface_mes_file);
        mes_unload(name_scenery_mes_file);
        return false;
    }

    if (!a_name_portal_init()) {
        a_name_wall_exit();
        a_name_roof_exit();
        a_name_tile_exit();
        a_name_light_exit();
        mes_unload(name_container_mes_file);
        mes_unload(name_eye_candy_mes_file);
        mes_unload(name_monster_mes_file);
        mes_unload(name_unique_npc_mes_file);
        mes_unload(name_interface_mes_file);
        mes_unload(name_scenery_mes_file);
        return false;
    }

    if (!a_name_item_init()) {
        a_name_portal_exit();
        a_name_wall_exit();
        a_name_roof_exit();
        a_name_tile_exit();
        a_name_light_exit();
        mes_unload(name_container_mes_file);
        mes_unload(name_eye_candy_mes_file);
        mes_unload(name_monster_mes_file);
        mes_unload(name_unique_npc_mes_file);
        mes_unload(name_interface_mes_file);
        mes_unload(name_scenery_mes_file);
        return false;
    }

    if (!a_name_facade_init()) {
        a_name_item_exit();
        a_name_portal_exit();
        a_name_wall_exit();
        a_name_roof_exit();
        a_name_tile_exit();
        a_name_light_exit();
        mes_unload(name_container_mes_file);
        mes_unload(name_eye_candy_mes_file);
        mes_unload(name_monster_mes_file);
        mes_unload(name_unique_npc_mes_file);
        mes_unload(name_interface_mes_file);
        mes_unload(name_scenery_mes_file);
        return false;
    }

    name_initialized = true;

    sub_4EBC40();
    sub_41CE60();

    return true;
}

// 0x41CDA0
void name_exit()
{
    if (name_initialized) {
        FREE(dword_5D560C);
        FREE(dword_5D5608);
        FREE(dword_5D5610);

        a_name_facade_exit();
        a_name_item_exit();
        a_name_portal_exit();
        a_name_wall_exit();
        a_name_roof_exit();
        a_name_tile_exit();
        a_name_light_exit();

        mes_unload(name_container_mes_file);
        mes_unload(name_eye_candy_mes_file);
        mes_unload(name_monster_mes_file);
        mes_unload(name_unique_npc_mes_file);
        mes_unload(name_interface_mes_file);
        mes_unload(name_scenery_mes_file);

        name_initialized = false;
        dword_5D5624 = false;
    }
}

// 0x41CE60
void sub_41CE60()
{
    int idx;
    int num;
    tig_art_id_t aid;
    MesFileEntry mes_file_entry;

    dword_5D55F0 = 10;
    dword_5D5618 = mes_num_entries(name_monster_mes_file);
    dword_5D5600 = mes_num_entries(name_unique_npc_mes_file);
    dword_5D560C = (unsigned int*)CALLOC(dword_5D55F0, sizeof(*dword_5D560C));
    dword_5D5608 = (unsigned int*)CALLOC(dword_5D5618, sizeof(*dword_5D5608));
    dword_5D5610 = (unsigned int*)CALLOC(dword_5D5600, sizeof(*dword_5D5610));

    if (!sub_41D1F0()) {
        for (idx = 0; idx < 19; idx++) {
            for (num = 0; num < dword_5D55F0; num++) {
                if (tig_art_critter_id_create(num & 1, num / 2, 0, 0, 0, 4, dword_5A10B8[idx], dword_5A1104[idx], 0, &aid) != TIG_OK) {
                    dword_5D560C[num] |= dword_5A106C[idx];
                    tig_debug_printf("Unable to create valid art id for critter %d (%s%c) using animation %d (%c%c)\n",
                        num,
                        off_5A11A4[num / 2],
                        byte_5A11A0[num & 1],
                        dword_5A10B8[idx],
                        byte_5A11DC[dword_5A1104[idx]],
                        'a' + dword_5A10B8[idx]);
                } else if (tig_art_exists(aid) != TIG_OK) {
                    dword_5D560C[num] |= dword_5A106C[idx];
                    tig_debug_printf("Missing art for critter %d (%s%c) using animation %d (%c%c)\n",
                        num,
                        off_5A11A4[num / 2],
                        byte_5A11A0[num & 1],
                        dword_5A10B8[idx],
                        byte_5A11DC[dword_5A1104[idx]],
                        dword_5A10B8[idx]);
                }
            }

            for (num = 0; num < dword_5D5618; num++) {
                if (tig_art_monster_id_create(num, 0, 0, 0, 4, dword_5A10B8[idx], dword_5A1104[idx], 0, &aid) != TIG_OK) {
                    dword_5D5608[num] |= dword_5A106C[idx];

                    mes_file_entry.num = num;
                    mes_get_msg(name_monster_mes_file, &mes_file_entry);

                    tig_debug_printf("Unable to create valid art id for monster %d (%s) using animation %d (%c%c)\n",
                        num,
                        mes_file_entry.str,
                        dword_5A10B8[idx],
                        byte_5A11DC[dword_5A1104[idx]],
                        'a' + dword_5A10B8[idx]);
                } else if (tig_art_exists(aid) != TIG_OK) {
                    dword_5D5608[num] |= dword_5A106C[idx];

                    mes_file_entry.num = num;
                    mes_get_msg(name_monster_mes_file, &mes_file_entry);

                    tig_debug_printf("Missing art for monster %d (%s) using animation %d (%c%c)\n",
                        num,
                        mes_file_entry.str,
                        dword_5A10B8[idx],
                        byte_5A11DC[dword_5A1104[idx]],
                        'a' + dword_5A10B8[idx]);
                }
            }

            for (num = 0; num < dword_5D5600; num++) {
                if (tig_art_unique_npc_id_create(num, 0, 0, 4, dword_5A10B8[idx], dword_5A1104[idx], 0, &aid) != TIG_OK) {
                    dword_5D5610[num] |= dword_5A106C[idx];

                    mes_file_entry.num = num;
                    mes_get_msg(name_unique_npc_mes_file, &mes_file_entry);

                    tig_debug_printf("Unable to create valid art id for unique npc %d (%s) using animation %d (%c%c)\n",
                        num,
                        mes_file_entry.str,
                        dword_5A10B8[idx],
                        byte_5A11DC[dword_5A1104[idx]],
                        'a' + dword_5A10B8[idx]);
                } else if (tig_art_exists(aid) != TIG_OK) {
                    dword_5D5610[num] |= dword_5A106C[idx];

                    mes_file_entry.num = num;
                    mes_get_msg(name_monster_mes_file, &mes_file_entry);

                    tig_debug_printf("Missing art for unique npc %d (%s) using animation %d (%c%c)\n",
                        num,
                        mes_file_entry.str,
                        dword_5A10B8[idx],
                        byte_5A11DC[dword_5A1104[idx]],
                        'a' + dword_5A10B8[idx]);
                }
            }
        }

        sub_41D2C0();
    }

    dword_5D5624 = true;
}

// 0x41D1F0
bool sub_41D1F0()
{
    TigFile* stream;
    int expected_cnt;
    int actual_cnt;

    stream = tig_file_fopen(off_5A1068, "rb");
    if (stream == NULL) {
        return false;
    }

    expected_cnt = sub_41D390();

    if (tig_file_fread(&actual_cnt, sizeof(actual_cnt), 1, stream) != 1) {
        tig_file_fclose(stream);
        return false;
    }

    if (expected_cnt != actual_cnt) {
        tig_file_fclose(stream);
        return false;
    }

    if (tig_file_fread(dword_5D560C, sizeof(*dword_5D560C), dword_5D55F0, stream) < dword_5D55F0) {
        tig_file_fclose(stream);
        return false;
    }

    if (tig_file_fread(dword_5D5608, sizeof(*dword_5D5608), dword_5D5618, stream) < dword_5D5618) {
        tig_file_fclose(stream);
        return false;
    }

    if (tig_file_fread(dword_5D5610, sizeof(*dword_5D5610), dword_5D5600, stream) < dword_5D5618) {
        tig_file_fclose(stream);
        return false;
    }

    tig_file_fclose(stream);

    return true;
}

// 0x41D2C0
bool sub_41D2C0()
{
    int cnt;
    TigFile* stream;

    cnt = sub_41D390();
    tig_file_mkdir(off_5A1064);

    stream = tig_file_fopen(off_5A1068, "wb");
    if (stream == NULL) {
        return false;
    }

    if (tig_file_fwrite(&cnt, sizeof(cnt), 1, stream) != 1) {
        tig_file_fclose(stream);
        return false;
    }

    if (tig_file_fwrite(dword_5D560C, sizeof(*dword_5D560C), dword_5D55F0, stream) != dword_5D55F0) {
        tig_file_fclose(stream);
        return false;
    }

    if (tig_file_fwrite(dword_5D5608, sizeof(*dword_5D5608), dword_5D5618, stream) != dword_5D5618) {
        tig_file_fclose(stream);
        return false;
    }

    if (tig_file_fwrite(dword_5D5610, sizeof(*dword_5D5610), dword_5D5600, stream) != dword_5D5600) {
        tig_file_fclose(stream);
        return false;
    }

    tig_file_fclose(stream);

    return true;
}

// 0x41D390
int sub_41D390()
{
    int cnt = 0;
    TigFileList dir_list;
    TigFileList file_list;
    char path[TIG_MAX_PATH];
    unsigned int index;

    tig_file_list_create(&dir_list, "art\\critter\\*.*");
    for (index = 0; index < dir_list.count; index++) {
        if (dir_list.entries[index].path[0] != '.') {
            sprintf(path, "art\\critter\\%s\\*.art", dir_list.entries[index].path);
            tig_file_list_create(&file_list, path);
            cnt += file_list.count;
            tig_file_list_destroy(&file_list);
        }
    }
    tig_file_list_destroy(&dir_list);

    tig_file_list_create(&dir_list, "art\\monster\\*.*");
    for (index = 0; index < dir_list.count; index++) {
        if (dir_list.entries[index].path[0] != '.') {
            sprintf(path, "art\\monster\\%s\\*.art", dir_list.entries[index].path);
            tig_file_list_create(&file_list, path);
            cnt += file_list.count;
            tig_file_list_destroy(&file_list);
        }
    }
    tig_file_list_destroy(&dir_list);

    tig_file_list_create(&dir_list, "art\\unique_npc\\*.*");
    for (index = 0; index < dir_list.count; index++) {
        if (dir_list.entries[index].path[0] != '.') {
            sprintf(path, "art\\unique_npc\\%s\\*.art", dir_list.entries[index].path);
            tig_file_list_create(&file_list, path);
            cnt += file_list.count;
            tig_file_list_destroy(&file_list);
        }
    }
    tig_file_list_destroy(&dir_list);

    return cnt;
}

// 0x41D510
tig_art_id_t sub_41D510(tig_art_id_t aid)
{
    switch (tig_art_type(aid)) {
    case TIG_ART_TYPE_TILE:
    case TIG_ART_TYPE_WALL:
        aid = tig_art_id_palette_set(aid, 0);
        aid = sub_502D30(aid, 0);
        break;
    case TIG_ART_TYPE_CRITTER: {
        int armor;
        int race;
        int gender;
        int shield;
        int anim;
        int weapon;

        armor = tig_art_critter_id_armor_get(aid);
        race = tig_art_critter_id_race_get(aid);
        gender = tig_art_critter_id_gender_get(aid);
        shield = tig_art_critter_id_shield_get(aid);
        anim = tig_art_id_anim_get(aid);
        weapon = tig_art_critter_id_weapon_get(aid);

        if (race == TIG_ART_CRITTER_RACE_ELF
            && gender == TIG_ART_CRITTER_GENDER_FEMALE) {
            race = TIG_ART_CRITTER_RACE_HUMAN;
        }

        if (armor == TIG_ART_ARMOR_TYPE_PLATE
            || armor == TIG_ART_ARMOR_TYPE_PLATE_CLASSIC) {
            gender = TIG_ART_CRITTER_GENDER_MALE;
            if (race == TIG_ART_CRITTER_RACE_ELF) {
                race = TIG_ART_CRITTER_RACE_HUMAN;
            } else if (race == TIG_ART_CRITTER_RACE_HALFLING) {
                race = TIG_ART_CRITTER_RACE_DWARF;
            }
        }

        sub_41DA40(gender + 2 * race, dword_5D55F0, dword_5D560C, &anim, &weapon);

        if (anim == 24) {
            armor = 0;
            shield = 0;
            weapon = 0;
            gender = 0;
        } else if (anim == 23) {
            weapon = 1;
            shield = 0;
        } else {
            if (weapon == 1 && (anim == 3 || anim == 5)) {
                weapon = 0;
            } else if (anim >= 6 && anim <= 19) {
                weapon = 0;
                shield = 0;
            } else if (anim == 1) {
                shield = 0;
            }
        }

        tig_art_critter_id_create(gender, race, armor, shield, 0, 0, anim, weapon, 0, &aid);
        break;
    }
    case TIG_ART_TYPE_PORTAL:
        aid = tig_art_id_frame_set(aid, 0);
        aid = tig_art_id_rotation_set(aid, 0);
        aid = tig_art_id_palette_set(aid, 0);
        aid = sub_502D30(aid, 0);
        break;
    case TIG_ART_TYPE_SCENERY:
    case TIG_ART_TYPE_CONTAINER:
        aid = tig_art_id_frame_set(aid, 0);
        aid = tig_art_id_rotation_set(aid, 0);
        aid = tig_art_id_palette_set(aid, 0);
        break;
    case TIG_ART_TYPE_INTERFACE:
    case TIG_ART_TYPE_MISC:
        aid = tig_art_id_frame_set(aid, 0);
        aid = tig_art_id_palette_set(aid, 0);
        break;
    case TIG_ART_TYPE_ITEM:
        aid = tig_art_id_damaged_set(aid, 0);
        aid = tig_art_id_palette_set(aid, 0);
        break;
    case TIG_ART_TYPE_LIGHT:
        aid = tig_art_id_frame_set(aid, 0);
        if (sub_504790(aid)) {
            int v1 = sub_504700(aid);
            aid = sub_504730(aid, v1 - v1 % 8);
        } else {
            aid = tig_art_id_rotation_set(aid, 0);
        }
        break;
    case TIG_ART_TYPE_ROOF:
        aid = sub_504880(aid, 0);
        aid = tig_art_id_palette_set(aid, 0);
        aid = tig_art_roof_id_fill_set(aid, 0);
        aid = tig_art_roof_id_fade_set(aid, 0);
        break;
    case TIG_ART_TYPE_FACADE:
        tig_art_facade_id_create(tig_art_facade_id_num_get(aid), 0, tig_art_tile_id_type_get(aid), 0, 0, 0, &aid);
        break;
    case TIG_ART_TYPE_MONSTER: {
        int armor;
        int specie;
        int shield;
        int anim;
        int weapon;

        armor = tig_art_critter_id_armor_get(aid);
        specie = tig_art_monster_id_specie_get(aid);
        shield = tig_art_critter_id_shield_get(aid);
        anim = tig_art_id_anim_get(aid);
        weapon = tig_art_critter_id_weapon_get(aid);
        sub_41DA40(specie, dword_5D5618, dword_5D5608, &anim, &weapon);

        if (anim == 24) {
            armor = 0;
            shield = 0;
            weapon = 0;
        } else if (anim == 23) {
            weapon = 1;
            shield = 0;
        } else {
            if (weapon == 1 && (anim == 3 || anim == 5)) {
                weapon = 0;
            } else if (anim >= 6 && anim <= 19) {
                weapon = 0;
                shield = 0;
            } else if (anim == 1) {
                shield = 0;
            }
        }

        tig_art_monster_id_create(specie, armor, shield, 0, 0, anim, weapon, 0, &aid);
        break;
    }
    case TIG_ART_TYPE_UNIQUE_NPC: {
        int num;
        int shield;
        int anim;
        int weapon;

        num = tig_art_num_get(aid);
        shield = tig_art_critter_id_shield_get(aid);
        anim = tig_art_id_anim_get(aid);
        weapon = tig_art_critter_id_weapon_get(aid);
        sub_41DA40(num, dword_5D5600, dword_5D5610, &anim, &weapon);
        if (anim == 24) {
            shield = 0;
            weapon = 0;
        } else if (anim == 23) {
            shield = 0;
            weapon = 1;
        } else {
            if (weapon == 1 && (anim == 3 || anim == 5)) {
                weapon = 0;
            } else if (anim >= 6 && anim <= 19) {
                weapon = 0;
                shield = 0;
            } else if (anim == 1) {
                shield = 0;
            }
        }
        tig_art_unique_npc_id_create(num, shield, 0, 0, anim, weapon, 0, &aid);
        break;
    }
    case TIG_ART_TYPE_EYE_CANDY:
        tig_art_eye_candy_id_create(tig_art_num_get(aid), 0, 0, 0, tig_art_eye_candy_id_type_get(aid), 0, 4, &aid);
        break;
    }

    return aid;
}

// 0x41DA40
void sub_41DA40(int a1, int a2, unsigned int* a3, int* a4, int* a5)
{
    int idx;

    if (!dword_5D5624) {
        return;
    }

    if (a1 < 0 || a1 >= a2) {
        return;
    }

    for (idx = 0; idx < 19; idx++) {
        if ((dword_5A106C[idx] & a3[a1]) != 0
            && *a4 == dword_5A10B8[idx]
            && (*a4 != 21
                || *a5 == dword_5A1104[idx])) {
            *a4 = dword_5A1150[idx];
            if (*a4 == 1) {
                if ((a3[a1] & 0x1) != 0) {
                    *a5 = 0;
                }
            } else if (*a4 == 0) {
                if (*a5 == 0 && (a3[a1] & 0x8) != 0) {
                    *a5 = 1;
                }
            }
        }
    }
}

// 0x41DAE0
int sub_41DAE0(tig_art_id_t aid, char* path)
{
    MesFileEntry mes_file_entry;

    if (!name_initialized) {
        return TIG_ERR_IO;
    }

    aid = sub_41D510(aid);

    switch (tig_art_type(aid)) {
    case TIG_ART_TYPE_TILE:
        if (!a_name_tile_aid_to_fname(aid, path)) {
            return TIG_ERR_16;
        }
        return TIG_OK;
    case TIG_ART_TYPE_WALL:
        if (!sub_4ECEB0(aid, path)) {
            return TIG_ERR_16;
        }
        return TIG_OK;
    case TIG_ART_TYPE_CRITTER: {
        int armor;
        int race;
        char v3;
        int anim;
        const char* v5;
        const char* v6;
        int shield;
        char v8;
        int weapon;
        char v10;

        armor = tig_art_critter_id_armor_get(aid);
        if (armor >= TIG_ART_ARMOR_TYPE_COUNT) {
            return TIG_ERR_16;
        }

        race = tig_art_critter_id_race_get(aid);
        if (armor == TIG_ART_ARMOR_TYPE_PLATE
            || armor == TIG_ART_ARMOR_TYPE_PLATE_CLASSIC) {
            v3 = byte_5A11A0[2];
        } else {
            v3 = byte_5A11A0[tig_art_critter_id_gender_get(aid)];
        }
        anim = tig_art_id_anim_get(aid);
        v5 = off_5A11A4[race];
        if (anim == 24) {
            v6 = "XX";
            v3 = byte_5A11A0[2];
        } else {
            v6 = off_5A11B8[armor];
        }
        shield = tig_art_critter_id_shield_get(aid);
        v8 = byte_5A119C[shield];
        weapon = tig_art_critter_id_weapon_get(aid);
        if (weapon == TIG_ART_WEAPON_TYPE_TWO_HANDED_SWORD
            && shield == 1) {
            v10 = byte_5A11DC[3];
        } else {
            v10 = byte_5A11DC[weapon];
        }

        sprintf(path,
            "art\\critter\\%s%c\\%s%c%s%c%c%c.art",
            v5,
            v3,
            v5,
            v3,
            v6,
            v8,
            v10,
            'a' + anim);
        return TIG_OK;
    }
    case TIG_ART_TYPE_PORTAL:
        if (!a_name_portal_aid_to_fname(aid, path)) {
            return TIG_ERR_16;
        }
        return TIG_OK;
    case TIG_ART_TYPE_SCENERY:
        mes_file_entry.num = 1000 * tig_art_scenery_id_type_get(aid) + tig_art_num_get(aid);
        if (!mes_search(name_scenery_mes_file, &mes_file_entry)) {
            return TIG_ERR_16;
        }
        sprintf(path, "art\\scenery\\%s", mes_file_entry.str);
        return TIG_OK;
    case TIG_ART_TYPE_INTERFACE:
        mes_file_entry.num = tig_art_num_get(aid);
        if (!mes_search(name_interface_mes_file, &mes_file_entry)) {
            return TIG_ERR_16;
        }
        sprintf(path, "art\\interface\\%s", mes_file_entry.str);
        return TIG_OK;
    case TIG_ART_TYPE_ITEM:
        if (!a_name_item_aid_to_fname(aid, path)) {
            return TIG_ERR_16;
        }
        return TIG_OK;
    case TIG_ART_TYPE_CONTAINER:
        mes_file_entry.num = 1000 * tig_art_container_id_type_get(aid) + tig_art_num_get(aid);
        if (!mes_search(name_container_mes_file, &mes_file_entry)) {
            return TIG_ERR_16;
        }
        sprintf(path, "art\\container\\%s", mes_file_entry.str);
        return TIG_OK;
    case TIG_ART_TYPE_LIGHT:
        if (!a_name_light_aid_to_fname(aid, path)) {
            return TIG_ERR_16;
        }
        return TIG_OK;
    case TIG_ART_TYPE_ROOF:
        if (!a_name_roof_aid_to_fname(aid, path)) {
            return TIG_ERR_16;
        }
        return TIG_OK;
    case TIG_ART_TYPE_FACADE:
        if (!a_name_facade_aid_to_fname(aid, path)) {
            return TIG_ERR_16;
        }
        return TIG_OK;
    case TIG_ART_TYPE_MONSTER: {
        int anim;
        int armor;
        const char* v3;
        int shield;
        char v5;
        int weapon;
        char v7;

        mes_file_entry.num = tig_art_monster_id_specie_get(aid);
        if (!mes_search(name_monster_mes_file, &mes_file_entry)) {
            return TIG_ERR_16;
        }

        anim = tig_art_id_anim_get(aid);
        armor = tig_art_critter_id_armor_get(aid);
        v3 = anim != 24 ? off_5A11B8[armor] : "XX";
        shield = tig_art_critter_id_shield_get(aid);
        v5 = byte_5A119C[shield];
        weapon = tig_art_critter_id_weapon_get(aid);
        if (weapon == TIG_ART_WEAPON_TYPE_TWO_HANDED_SWORD
            && shield == 1) {
            v7 = byte_5A11DC[3];
        } else {
            v7 = byte_5A11DC[weapon];
        }

        sprintf(path,
            "art\\monster\\%s\\%s%s%c%c%c.art",
            mes_file_entry.str,
            mes_file_entry.str,
            v3,
            v5,
            v7,
            'a' + anim);
        return TIG_OK;
    }
    case TIG_ART_TYPE_UNIQUE_NPC: {
        int anim;
        int shield;
        char v3;
        int weapon;
        char v5;

        mes_file_entry.num = tig_art_num_get(aid);
        if (!mes_search(name_unique_npc_mes_file, &mes_file_entry)) {
            return TIG_ERR_16;
        }

        anim = tig_art_id_anim_get(aid);
        shield = tig_art_critter_id_shield_get(aid);
        v3 = byte_5A119C[shield];
        weapon = tig_art_critter_id_weapon_get(aid);
        if (weapon == TIG_ART_WEAPON_TYPE_TWO_HANDED_SWORD
            && shield == 1) {
            v5 = byte_5A11DC[3];
        } else {
            v5 = byte_5A11DC[weapon];
        }

        sprintf(path,
            "art\\unique_npc\\%s\\%s%c%c%c.art",
            mes_file_entry.str,
            mes_file_entry.str,
            v3,
            v5,
            'a' + anim);
        return TIG_OK;
    }
    case TIG_ART_TYPE_EYE_CANDY:
        mes_file_entry.num = tig_art_num_get(aid);
        if (!mes_search(name_eye_candy_mes_file, &mes_file_entry)) {
            return TIG_ERR_16;
        }
        sprintf(path,
            "art\\eye_candy\\%s_%c.art",
            mes_file_entry.str,
            byte_5A11EC[tig_art_eye_candy_id_type_get(aid)]);
        return TIG_OK;
    default:
        return TIG_ERR_16;
    }
}

// 0x41DFC0
tig_art_id_t sub_41DFC0(int type, int* extra)
{
    tig_art_id_t aid = TIG_ART_ID_INVALID;

    switch (type) {
    case TIG_ART_TYPE_TILE:
        tig_art_tile_id_create(0, 0, 0, 0, 1, 1, 1, 0, &aid);
        break;
    case TIG_ART_TYPE_WALL:
        tig_art_wall_id_create(0, 0, 0, 0, 0, 0, &aid);
        break;
    case TIG_ART_TYPE_CRITTER:
        tig_art_critter_id_create(0, 0, 0, 0, 0, 3, 0, 0, 0, &aid);
        break;
    case TIG_ART_TYPE_PORTAL:
        tig_art_portal_id_create(0, 0, 0, 0, 0, 0, &aid);
        break;
    case TIG_ART_TYPE_SCENERY: {
        int v1;

        if (extra != NULL) {
            v1 = extra[0];
        } else {
            v1 = 0;
        }

        tig_art_scenery_id_create(0, v1, 0, 0, 0, &aid);
        break;
    }
    case TIG_ART_TYPE_INTERFACE:
        tig_art_interface_id_create(0, 0, 0, 0, &aid);
        break;
    case TIG_ART_TYPE_ITEM: {
        int v1;
        int armor_coverage;
        int destroyed;
        int disposition;
        int subtype;

        if (extra != NULL) {
            v1 = extra[0];
            armor_coverage = extra[1];
            destroyed = extra[2];
            disposition = extra[3];
            subtype = extra[4];
        } else {
            v1 = 0;
            armor_coverage = 0;
            destroyed = 0;
            disposition = 1;
            subtype = 0;
        }

        tig_art_item_id_create(0,
            disposition,
            0,
            destroyed,
            subtype,
            v1,
            armor_coverage,
            0,
            &aid);
        break;
    }
    case TIG_ART_TYPE_CONTAINER: {
        int v1;

        if (extra != NULL) {
            v1 = extra[0];
        } else {
            v1 = 0;
        }

        tig_art_container_id_create(0, v1, 0, 0, 0, &aid);
        break;
    }
    case TIG_ART_TYPE_LIGHT:
        tig_art_light_id_create(0, 0, 0, 0, &aid);
        break;
    case TIG_ART_TYPE_MONSTER:
        tig_art_monster_id_create(0, 0, 0, 0, 3, 0, 0, 0, &aid);
        break;
    case TIG_ART_TYPE_UNIQUE_NPC:
        tig_art_unique_npc_id_create(0, 0, 0, 3, 0, 0, 0, &aid);
        break;
    case TIG_ART_TYPE_EYE_CANDY:
        tig_art_eye_candy_id_create(0, 0, 0, 0, 0, 0, 4, &aid);
        break;
    }

    if (aid != TIG_ART_ID_INVALID) {
        if (tig_art_exists(aid) != TIG_OK || sub_502E00(aid) != TIG_OK) {
            aid = sub_41E200(aid);
        }
    }

    return aid;
}

// 0x41E200
tig_art_id_t sub_41E200(tig_art_id_t art_id)
{
    if (art_id == TIG_ART_ID_INVALID) {
        return TIG_ART_ID_INVALID;
    }

    for (;;) {
        switch (tig_art_type(art_id)) {
        case TIG_ART_TYPE_TILE: {
            int num = tig_art_tile_id_num1_get(art_id) + 1;
            int type = tig_art_tile_id_type_get(art_id);
            int flippable = tig_art_tile_id_flippable_get(art_id);

            if (tig_art_tile_id_create(num, num, 0, 0, type, flippable, flippable, 0, &art_id) == TIG_OK) {
                break;
            }

            // TODO: Check.
            if (type == 0) {
                if (flippable) {
                    flippable = 0;
                    num = 0;
                }
            } else {
                if (!flippable) {
                    flippable = 1;
                    type = 0;
                    num = 0;
                } else {
                    flippable = 0;
                    num = 0;
                }
            }

            if (tig_art_tile_id_create(num, num, 0, 0, type, flippable, flippable, 0, &art_id) == TIG_OK) {
                break;
            }

            return TIG_ART_ID_INVALID;
        }
        case TIG_ART_TYPE_WALL: {
            int num = tig_art_wall_id_num_get(art_id);
            int p_piece = tig_art_wall_id_p_piece_get(art_id);
            int variation = tig_art_wall_id_variation_get(art_id);
            int palette = tig_art_id_palette_get(art_id);
            int damaged = tig_art_id_damaged_get(art_id);

            // TODO: Probably wrong, check.
            if (damaged == 0) {
                if (tig_art_wall_id_create(num, p_piece, variation, 0, palette, 1024, &art_id) == TIG_OK) {
                    break;
                }
            } else if (damaged == 1024) {
                if (tig_art_wall_id_create(num, p_piece, variation, 0, palette, 128, &art_id) == TIG_OK) {
                    break;
                }
            }

            if (tig_art_wall_id_create(num, p_piece, variation, 0, palette + 1, 0, &art_id) == TIG_OK) {
                break;
            }

            if (tig_art_wall_id_create(num, p_piece, variation + 1, 0, 0, 0, &art_id) == TIG_OK) {
                break;
            }

            if (tig_art_wall_id_create(num, p_piece + 1, 0, 0, 0, 0, &art_id) == TIG_OK) {
                break;
            }

            if (tig_art_wall_id_create(num + 1, 0, 0, 0, 0, 0, &art_id) == TIG_OK) {
                break;
            }

            return TIG_ART_ID_INVALID;
        }
        case TIG_ART_TYPE_CRITTER: {
            int palette = tig_art_id_palette_get(art_id);
            int gender = tig_art_critter_id_gender_get(art_id);
            int race = tig_art_critter_id_race_get(art_id);
            int armor = tig_art_critter_id_armor_get(art_id);

            if (tig_art_critter_id_create(gender, race, armor, 0, 0, 3, 0, 0, palette + 1, &art_id) == TIG_OK) {
                break;
            }

            if (tig_art_critter_id_create(gender + 1, race, armor, 0, 0, 3, 0, 0, 0, &art_id) == TIG_OK) {
                break;
            }

            if (tig_art_critter_id_create(0, race + 1, armor, 0, 0, 3, 0, 0, 0, &art_id) == TIG_OK) {
                break;
            }

            if (tig_art_critter_id_create(0, 0, armor + 1, 0, 0, 3, 0, 0, 0, &art_id) == TIG_OK) {
                break;
            }

            return TIG_ART_ID_INVALID;
        }
        case TIG_ART_TYPE_PORTAL: {
            int palette = tig_art_id_palette_get(art_id);
            int type = tig_art_portal_id_type_get(art_id);
            int num = tig_art_num_get(art_id);
            int damaged = tig_art_id_damaged_get(art_id);

            if (damaged == 0) {
                if (tig_art_portal_id_create(num, type, 512, 0, 0, palette, &art_id) == TIG_OK) {
                    break;
                }
            }

            if (tig_art_portal_id_create(num, type, 0, 0, 0, palette + 1, &art_id) == TIG_OK) {
                break;
            }

            if (tig_art_portal_id_create(num, type, 0, 0, 0, 0, &art_id) == TIG_OK) {
                break;
            }

            if (tig_art_portal_id_create(num, type + 1, 0, 0, 0, 0, &art_id) == TIG_OK) {
                break;
            }

            if (tig_art_portal_id_create(num + 1, 0, 0, 0, 0, 0, &art_id) == TIG_OK) {
                break;
            }

            return TIG_ART_ID_INVALID;
        }
        case TIG_ART_TYPE_SCENERY: {
            int type = tig_art_scenery_id_type_get(art_id);
            int palette = tig_art_id_palette_get(art_id);
            int num = tig_art_num_get(art_id);

            if (tig_art_scenery_id_create(num, type, 0, 0, palette + 1, &art_id) == TIG_OK) {
                break;
            }

            if (tig_art_scenery_id_create(num + 1, type, 0, 0, 0, &art_id) == TIG_OK) {
                break;
            }

            return TIG_ART_ID_INVALID;
        }
        case TIG_ART_TYPE_INTERFACE: {
            int palette = tig_art_id_palette_get(art_id);
            int num = tig_art_num_get(art_id);

            if (tig_art_interface_id_create(num, 0, 0, palette + 1, &art_id) == TIG_OK) {
                break;
            }

            if (tig_art_interface_id_create(num + 1, 0, 0, 0, &art_id) == TIG_OK) {
                break;
            }

            return TIG_ART_ID_INVALID;
        }
        case TIG_ART_TYPE_ITEM: {
            int palette = tig_art_id_palette_get(art_id);
            int num = tig_art_num_get(art_id);
            int subtype = tig_art_item_id_subtype_get(art_id);
            int type = tig_art_item_id_type_get(art_id);
            int destroyed = tig_art_item_id_destroyed_get(art_id);
            int disposition = tig_art_item_id_disposition_get(art_id);
            int armor_coverage = tig_art_item_id_armor_coverage_get(art_id);

            if (tig_art_item_id_create(num, disposition, 0, destroyed, subtype, type, armor_coverage, palette + 1, &art_id) == TIG_OK) {
                break;
            }

            if (tig_art_item_id_create(num + 1, disposition, 0, destroyed, subtype, type, armor_coverage, 0, &art_id) == TIG_OK) {
                break;
            }

            if (type == TIG_ART_ITEM_TYPE_ARMOR
                && armor_coverage == TIG_ART_ARMOR_COVERAGE_TORSO) {
                if (tig_art_item_id_create(0, disposition, 0, destroyed, subtype + 1, type, armor_coverage, 0, &art_id) == TIG_OK) {
                    break;
                }
            }

            return TIG_ART_ID_INVALID;
        }
        case TIG_ART_TYPE_CONTAINER: {
            int type = tig_art_container_id_type_get(art_id);
            int palette = tig_art_id_palette_get(art_id);
            int num = tig_art_num_get(art_id);

            if (tig_art_container_id_create(num, type, 0, 0, palette + 1, &art_id) == TIG_OK) {
                break;
            }

            if (tig_art_container_id_create(num + 1, type, 0, 0, 0, &art_id) == TIG_OK) {
                break;
            }

            return TIG_ART_ID_INVALID;
        }
        case TIG_ART_TYPE_LIGHT: {
            int num = tig_art_num_get(art_id);

            if (tig_art_light_id_create(num + 1, 0, 0, 0, &art_id) == TIG_OK) {
                break;
            }

            return TIG_ART_ID_INVALID;
        }
        case TIG_ART_TYPE_MONSTER: {
            int palette = tig_art_id_palette_get(art_id);
            int num = tig_art_monster_id_specie_get(art_id);

            if (tig_art_monster_id_create(num, 0, 0, 0, 3, 0, 0, palette + 1, &art_id) == TIG_OK) {
                break;
            }

            if (tig_art_monster_id_create(num + 1, 0, 0, 0, 3, 0, 0, 0, &art_id) == TIG_OK) {
                break;
            }

            return TIG_ART_ID_INVALID;
        }
        case TIG_ART_TYPE_UNIQUE_NPC: {
            int palette = tig_art_id_palette_get(art_id);
            int num = tig_art_num_get(art_id);

            if (tig_art_unique_npc_id_create(num, 0, 0, 3, 0, 0, palette + 1, &art_id) == TIG_OK) {
                break;
            }

            if (tig_art_unique_npc_id_create(num + 1, 0, 0, 3, 0, 0, 0, &art_id) == TIG_OK) {
                break;
            }

            return TIG_ART_ID_INVALID;
        }
        case TIG_ART_TYPE_EYE_CANDY: {
            int num = tig_art_num_get(art_id);
            int type = tig_art_eye_candy_id_type_get(art_id);
            int palette = tig_art_id_palette_get(art_id);

            if (tig_art_eye_candy_id_create(num, 0, 0, 0, type, palette + 1, 4, &art_id) == TIG_OK) {
                break;
            }

            if (tig_art_eye_candy_id_create(num, 0, 0, 0, type + 1, 0, 4, &art_id) == TIG_OK) {
                break;
            }

            if (tig_art_eye_candy_id_create(num + 1, 0, 0, 0, 0, 0, 4, &art_id) == TIG_OK) {
                break;
            }

            return TIG_ART_ID_INVALID;
        }
        default:
            break;
        }

        if (art_id == TIG_ART_ID_INVALID) {
            return TIG_ART_ID_INVALID;
        }

        if (tig_art_exists(art_id) == TIG_OK
            && sub_502E00(art_id) == TIG_OK) {
            return art_id;
        }

        if (art_id == TIG_ART_ID_INVALID) {
            return TIG_ART_ID_INVALID;
        }
    }
}

// 0x41E960
int sub_41E960(int a1)
{
    // TODO: Incomplete.
}

// 0x41F090
bool sub_41F090(int a1, int* a2, int* a3)
{
    TigWindowData window_data;

    dword_739E58 = a1;
    dword_739E50 = a2;
    dword_739E54 = -1;

    window_data.rect.x = 40;
    window_data.rect.y = 40;
    window_data.rect.width = 100;
    window_data.rect.height = 100;
    window_data.flags = TIG_WINDOW_ALWAYS_ON_TOP | TIG_WINDOW_MODAL | TIG_WINDOW_MESSAGE_FILTER;
    window_data.message_filter = sub_41F190;
    window_data.background_color = tig_color_make(30, 30, 30);

    if (tig_window_create(&window_data, &dword_739E48) != TIG_OK) {
        return false;
    }

    sub_41F240();

    dword_739E4C = 0;
    while (dword_739E4C == 0) {
        tig_ping();
        tig_window_display();
    }

    tig_window_destroy(dword_739E48);

    if (dword_739E4C != 1) {
        return false;
    }

    *a3 = dword_739E54;

    return true;
}

// 0x41F190
bool sub_41F190(TigMessage* msg)
{
    int v1;

    if (msg->type == TIG_MESSAGE_CHAR) {
        switch (msg->data.character.ch) {
        case 'a':
            if (dword_739E54 != -1) {
                dword_739E54 = sub_41E960(dword_739E54);
                sub_41F240();
            }
            break;
        case 'z':
            if (dword_739E54 == -1) {
                dword_739E54 = sub_41DFC0(dword_739E58, dword_739E50);
                sub_41F240();
            } else {
                v1 = sub_41E200(dword_739E54);
                if (v1 != -1) {
                    dword_739E54 = v1;
                }
                sub_41F240();
            }
            break;
        case '\x1B':
            dword_739E4C = -1;
            break;
        case '\n':
            dword_739E4C = 1;
            break;
        }
    }

    return true;
}

// 0x41F240
void sub_41F240()
{
    TigRect dst_rect;
    TigRect src_rect;
    TigArtFrameData art_frame_data;
    TigArtBlitInfo art_blit_spec;

    dst_rect.x = 10;
    dst_rect.y = 10;
    dst_rect.width = 80;
    dst_rect.height = 80;

    tig_window_fill(dword_739E48, &dst_rect, tig_color_make(0, 0, 60));

    if (dword_739E54 != TIG_ART_ID_INVALID) {
        if (tig_art_frame_data(dword_739E54, &art_frame_data) == TIG_OK) {
            src_rect.x = 0;
            src_rect.y = 0;
            src_rect.width = art_frame_data.width;
            src_rect.height = art_frame_data.height;

            if (art_frame_data.width > 80 || art_frame_data.height > 80) {
                if (80.0 / (double)art_frame_data.height < 80.0 / (double)art_frame_data.width) {
                    art_frame_data.width = (int)((double)art_frame_data.width * (80.0 / (double)art_frame_data.height));
                } else {
                    art_frame_data.height = (int)((double)art_frame_data.height * (80.0 / (double)art_frame_data.width));
                }
            } else {
                dst_rect.width = art_frame_data.width;
                dst_rect.height = art_frame_data.height;
            }

            dst_rect.x += (80 - dst_rect.width) / 2;
            dst_rect.y += (80 - dst_rect.height) / 2;

            art_blit_spec.flags = TIG_ART_BLT_PALETTE_ORIGINAL;
            art_blit_spec.art_id = dword_739E54;
            art_blit_spec.src_rect = &src_rect;
            art_blit_spec.dst_rect = &dst_rect;
            tig_window_blit_art(dword_739E48, &art_blit_spec);
        }
    }
}
