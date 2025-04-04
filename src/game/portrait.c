#include "game/portrait.h"

#include <stdio.h>

#include "game/mes.h"
#include "game/multiplayer.h"
#include "game/player.h"
#include "game/stat.h"

static void sub_4CE4A0(int64_t obj, int num, tig_window_handle_t window_handle, int x, int y, int width, int height);
static bool sub_4CEAC0(int64_t obj, const char* str);

// 0x5B7C0C
static const char* portrait_race_specifiers[RACE_COUNT] = {
    /*     HUMAN */ "HU",
    /*     DWARF */ "DW",
    /*       ELF */ "EL",
    /*  HALF_ELF */ "HE",
    /*     GNOME */ "GN",
    /*  HALFLING */ "HA",
    /*  HALF_ORC */ "HO",
    /* HALF_OGRE */ "HG",
    /*  DARK_ELF */ "EL",
    /*      OGRE */ "HG",
    /*       ORC */ "HO",
};

// 0x5B7C38
static char portrait_gender_specifiers[GENDER_COUNT] = {
    /* FEMALE */ 'F',
    /*   MALE */ 'M',
};

// 0x601744
static mes_file_handle_t portrait_user_mes_file;

// 0x601748
static bool portrait_have_user_mes_file;

// 0x60174C
static mes_file_handle_t portrait_mes_file;

// 0x4CE350
bool portrait_init(GameInitInfo* init_info)
{
    (void)init_info;

    if (!mes_load("portrait\\gameport.mes", &portrait_mes_file)) {
        return false;
    }

    portrait_have_user_mes_file = mes_load("portrait\\userport.mes", &portrait_user_mes_file);

    return true;
}

// 0x4CE390
void portrait_exit()
{
    mes_unload(portrait_mes_file);

    if (portrait_have_user_mes_file) {
        mes_unload(portrait_user_mes_file);
    }
}

// 0x4CE3C0
void portrait_path(int num, char* path, int size)
{
    mes_file_handle_t mes_file;
    MesFileEntry mes_file_entry;

    mes_file_entry.num = num;
    if (num >= 1000) {
        mes_file = portrait_mes_file;
    } else if (num >= 1) {
        if (!portrait_have_user_mes_file) {
            path[0] = '\0';
            return;
        }

        mes_file = portrait_user_mes_file;
    } else {
        mes_file = portrait_mes_file;
        mes_file_entry.num = 1000;
    }

    if (!mes_search(mes_file, &mes_file_entry)) {
        path[0] = '\0';
        return;
    }

    if (size >= 128) {
        sprintf(path, "portrait\\%s_b.bmp", mes_file_entry.str);
    } else {
        sprintf(path, "portrait\\%s.bmp", mes_file_entry.str);
    }
}

// 0x4CE470
void portrait_draw_native(int64_t obj, int num, tig_window_handle_t window_handle, int x, int y)
{
    sub_4CE4A0(obj, num, window_handle, x, y, 0, 0);
}

// 0x4CE4A0
void sub_4CE4A0(int64_t obj, int num, tig_window_handle_t window_handle, int x, int y, int width, int height)
{
    TigBmp bmp;
    int rc;
    char* pch;
    TigRect src_rect;
    TigRect dst_rect;

    if (obj != OBJ_HANDLE_NULL
        && obj_field_int32_get(obj, OBJ_F_TYPE) == OBJ_TYPE_PC
        && (obj_field_int32_get(obj, OBJ_F_PC_FLAGS) & OPCF_USE_ALT_DATA) != 0) {
        if (!sub_4A4C40(obj, width, bmp.name)) {
            return;
        }
    } else {
        portrait_path(num, bmp.name, width);
    }

    rc = tig_bmp_create(&bmp);
    if (rc != TIG_OK) {
        if (strlen(bmp.name) >= 6) {
            pch = &(bmp.name[strlen(bmp.name) - 6]);
            if (strcmpi(pch, "_b.bmp") == 0) {
                *pch = '\0';
                strcat(bmp.name, ".bmp");
                rc = tig_bmp_create(&bmp);
            }
        }
    }

    if (rc == TIG_OK) {
        src_rect.x = 0;
        src_rect.y = 0;
        src_rect.width = bmp.width;
        src_rect.height = bmp.height;

        dst_rect.x = x;
        dst_rect.y = y;
        dst_rect.width = bmp.width;
        dst_rect.height = bmp.height;

        if (width > 0) {
            dst_rect.width = width;
        }

        if (height > 0) {
            dst_rect.height = height;
        }

        tig_window_copy_from_bmp(window_handle, &dst_rect, &bmp, &src_rect);
        tig_bmp_destroy(&bmp);
    }
}

// 0x4CE640
void portrait_draw_128x128(int64_t obj, int num, tig_window_handle_t window_handle, int x, int y)
{
    sub_4CE4A0(obj, num, window_handle, x, y, 128, 128);
}

// 0x4CE680
void portrait_draw_32x32(int64_t obj, int num, tig_window_handle_t window_handle, int x, int y)
{
    sub_4CE4A0(obj, num, window_handle, x, y, 32, 32);
}

// 0x4CE6B0
bool portrait_find_first(int64_t obj, int* portrait_ptr)
{
    MesFileEntry mes_file_entry;

    *portrait_ptr = 0;

    if (obj_field_int32_get(obj, OBJ_F_TYPE) == OBJ_TYPE_PC
        && (obj_field_int32_get(obj, OBJ_F_PC_FLAGS) & OPCF_USE_ALT_DATA) != 0) {
        return false;
    }

    mes_file_entry.num = 1000;
    while (mes_search(portrait_mes_file, &mes_file_entry)) {
        if (sub_4CEAC0(obj, mes_file_entry.str)) {
            *portrait_ptr = mes_file_entry.num;
            return true;
        }
        mes_file_entry.num++;
    }

    if (portrait_have_user_mes_file) {
        mes_file_entry.num = 1;
        while (mes_file_entry.num < 1000 && mes_search(portrait_user_mes_file, &mes_file_entry)) {
            if (sub_4CEAC0(obj, mes_file_entry.str)) {
                *portrait_ptr = mes_file_entry.num;
                return true;
            }
            mes_file_entry.num++;
        }
    }

    return false;
}

// 0x4CE7A0
bool portrait_find_last(int64_t obj, int* portrait_ptr)
{
    MesFileEntry mes_file_entry;
    int num = 0;

    if (obj_field_int32_get(obj, OBJ_F_TYPE) == OBJ_TYPE_PC
        && (obj_field_int32_get(obj, OBJ_F_PC_FLAGS) & OPCF_USE_ALT_DATA) != 0) {
        return false;
    }

    mes_file_entry.num = *portrait_ptr;
    while (mes_search(portrait_mes_file, &mes_file_entry)) {
        if (sub_4CEAC0(obj, mes_file_entry.str)) {
            num = mes_file_entry.num;
        }
        mes_file_entry.num++;
    }

    if (num != 0) {
        *portrait_ptr = num;
        return true;
    }

    if (portrait_have_user_mes_file) {
        mes_file_entry.num = 1;
        while (mes_file_entry.num < 1000 && mes_search(portrait_user_mes_file, &mes_file_entry)) {
            if (sub_4CEAC0(obj, mes_file_entry.str)) {
                num = mes_file_entry.num;
            }
            mes_file_entry.num++;
        }
    }

    if (num != 0) {
        *portrait_ptr = num;
        return true;
    }

    return false;
}

// 0x4CE8B0
bool portrait_find_next(int64_t obj, int* portrait_ptr)
{
    MesFileEntry mes_file_entry;

    if (obj_field_int32_get(obj, OBJ_F_TYPE) == OBJ_TYPE_PC
        && (obj_field_int32_get(obj, OBJ_F_PC_FLAGS) & OPCF_USE_ALT_DATA) != 0) {
        return false;
    }

    mes_file_entry.num = *portrait_ptr + 1;
    if (mes_file_entry.num >= 1000) {
        while (mes_search(portrait_mes_file, &mes_file_entry)) {
            if (sub_4CEAC0(obj, mes_file_entry.str)) {
                *portrait_ptr = mes_file_entry.num;
                return true;
            }
            mes_file_entry.num++;
        }

        mes_file_entry.num = 1;
    }

    if (portrait_have_user_mes_file) {
        while (mes_file_entry.num < 1000 && mes_search(portrait_user_mes_file, &mes_file_entry)) {
            if (sub_4CEAC0(obj, mes_file_entry.str)) {
                *portrait_ptr = mes_file_entry.num;
                return true;
            }
            mes_file_entry.num++;
        }
    }

    return false;
}

// 0x4CE9B0
bool portrait_find_prev(int64_t obj, int* portrait_ptr)
{
    MesFileEntry mes_file_entry;

    if (obj_field_int32_get(obj, OBJ_F_TYPE) == OBJ_TYPE_PC
        && (obj_field_int32_get(obj, OBJ_F_PC_FLAGS) & OPCF_USE_ALT_DATA) != 0) {
        return false;
    }

    if (*portrait_ptr == 1000) {
        return false;
    }

    mes_file_entry.num = *portrait_ptr - 1;
    if (*portrait_ptr - 1 < 1000) {
        if (portrait_have_user_mes_file) {
            while (mes_file_entry.num >= 1 && mes_search(portrait_user_mes_file, &mes_file_entry)) {
                if (sub_4CEAC0(obj, mes_file_entry.str)) {
                    *portrait_ptr = mes_file_entry.num;
                    return true;
                }
                mes_file_entry.num--;
            }
        }
        mes_file_entry.num = mes_entries_count(portrait_mes_file) + 999;
    }

    while (mes_file_entry.num >= 1000 && mes_search(portrait_mes_file, &mes_file_entry)) {
        if (sub_4CEAC0(obj, mes_file_entry.str)) {
            *portrait_ptr = mes_file_entry.num;
            return true;
        }
        mes_file_entry.num--;
    }

    return false;
}

// 0x4CEAC0
bool sub_4CEAC0(int64_t obj, const char* str)
{
    int race;
    int gender;
    char buffer[4];

    if (str != NULL) {
        race = stat_base_get(obj, STAT_RACE);
        gender = stat_base_get(obj, STAT_GENDER);
        sprintf(buffer, "%s%c",
            portrait_race_specifiers[race],
            portrait_gender_specifiers[gender]);

        if (strnicmp(buffer, str, 3) == 0) {
            return true;
        }

        if (strnicmp("ANY", str, 3) == 0) {
            return true;
        }

        if (obj_field_int32_get(obj, OBJ_F_TYPE) == OBJ_TYPE_NPC
            && strnicmp("NPC", str, 3) == 0) {
            return true;
        }
    }

    return false;
}

// 0x4CEB80
int portrait_get(int64_t obj)
{
    int type;
    int portrait;
    int next;
    int prev;

    type = obj_field_int32_get(obj, OBJ_F_TYPE);
    portrait = obj_field_int32_get(obj, OBJ_F_CRITTER_PORTRAIT);

    if (portrait != 0 && type == OBJ_TYPE_NPC) {
        if (portrait == portrait_get(player_get_local_pc_obj())) {
            if (portrait_find_next(obj, &next)) {
                portrait = next;
            } else if (portrait_find_prev(obj, &prev)) {
                portrait = prev;
            }
        }
    }

    return portrait;
}
