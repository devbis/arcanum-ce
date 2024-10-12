#include "game/area.h"

#include "game/location.h"
#include "game/map.h"
#include "game/mes.h"
#include "game/mp_utils.h"
#include "game/multiplayer.h"
#include "game/obj.h"
#include "game/player.h"
#include "game/sector.h"
#include "game/townmap.h"
#include "game/ui.h"

static int sub_4CB220(int64_t obj);
static int sub_4CB630(int64_t obj);

// 0x5FF5A8
static int dword_5FF5A8;

// 0x5FF5AC
static int64_t* dword_5FF5AC;

// 0x5FF5B0
static char** dword_5FF5B0;

// 0x5FF5B4
static mes_file_handle_t dword_5FF5B4;

// 0x5FF5B8
static unsigned char* dword_5FF5B8[8];

// 0x5FF5D8
static char** dword_5FF5D8;

// 0x5FF5DC
static int* dword_5FF5DC;

// 0x5FF5E0
static int* dword_5FF5E0;

// 0x5FF5E4
static int* dword_5FF5E4;

// 0x5FF5E8
static uint8_t* dword_5FF5E8;

// 0x5FF5EC
static int dword_5FF5EC;

// 0x5FF5F0
static int dword_5FF5F0[8];

// 0x4CA940
bool area_init(GameInitInfo* init_info)
{
    (void)init_info;

    dword_5FF5A8 = 0;

    return true;
}

// 0x4CA950
void area_reset()
{
    int index;
    int player;

    for (index = 0; index < dword_5FF5A8; index++) {
        dword_5FF5E8[index] = 0;
    }

    for (player = 0; player < 8; player++) {
        for (index = 0; index < dword_5FF5A8; index++) {
            dword_5FF5B8[player][index] = 0;
        }
    }
}

// 0x4CA9A0
void area_exit()
{
}

// 0x4CA9B0
bool area_mod_load()
{
    MesFileEntry mes_file_entry;
    int index;
    char* str;
    int64_t x;
    int64_t y;
    char* pch;
    int radius;

    dword_5FF5A8 = 0;

    if (!mes_load("mes\\gamearea.mes", &dword_5FF5B4)) {
        return true;
    }

    mes_file_entry.num = 0;
    mes_get_msg(dword_5FF5B4, &mes_file_entry);
    do {
        dword_5FF5A8++;
    } while (mes_find_next(dword_5FF5B4, &mes_file_entry));

    dword_5FF5B0 = (char**)MALLOC(sizeof(*dword_5FF5B0) * dword_5FF5A8);
    dword_5FF5D8 = (char**)MALLOC(sizeof(*dword_5FF5D8) * dword_5FF5A8);
    dword_5FF5AC = (int64_t*)MALLOC(sizeof(*dword_5FF5AC) * dword_5FF5A8);
    dword_5FF5E8 = (uint8_t*)MALLOC(sizeof(*dword_5FF5E8) * dword_5FF5A8);

    for (index = 0; index < 8; index++) {
        dword_5FF5B8[index] = (uint8_t*)MALLOC(sizeof(*dword_5FF5B8[index]) * dword_5FF5A8);
    }

    dword_5FF5E4 = (int*)MALLOC(sizeof(*dword_5FF5E4) * dword_5FF5A8);
    dword_5FF5DC = (int*)MALLOC(sizeof(*dword_5FF5DC) * dword_5FF5A8);
    dword_5FF5E0 = (int*)MALLOC(sizeof(*dword_5FF5E0) * dword_5FF5A8);

    index = 0;
    mes_file_entry.num = 0;
    mes_get_msg(dword_5FF5B4, &mes_file_entry);
    tig_str_parse_set_separator(',');

    do {
        str = mes_file_entry.str;
        tig_str_parse_value_64(&str, &x);
        tig_str_parse_value_64(&str, &y);
        dword_5FF5AC[index] = x | y;
        dword_5FF5DC[index] = 0;
        dword_5FF5E0[index] = 0;
        tig_str_parse_value(&str, &(dword_5FF5DC[index]));
        tig_str_parse_value(&str, &(dword_5FF5E0[index]));

        pch = strchr(mes_file_entry.str, '/');
        if (pch != NULL) {
            dword_5FF5B0[index] = pch + 1;

            pch = strchr(pch + 1, '/');
            if (pch != NULL) {
                *pch = '\0';
                dword_5FF5D8[index] = pch + 1;
                dword_5FF5E8[index] = 0;
                dword_5FF5E4[index] = 320;

                pch = strchr(pch + 1, '/');
                if (pch != NULL) {
                    *pch = '\0';

                    pch++;
                    if (tig_str_parse_named_value(&pch, "Radius:", &radius)) {
                        if (index > 0) {
                            dword_5FF5E4[index] = radius << 6;
                        } else if (index == 0) {
                            dword_5FF5E4[0] = 0;
                        } else {
                            // FIXME: Unreachable (and wrong).
                            dword_5FF5E4[index] = -1;
                        }
                    }
                }
            } else {
                tig_debug_printf("Area: area_init: ERROR: Line %d has invalid description data!\n", index);
            }
        } else {
            tig_debug_printf("Area: area_init: ERROR: Line %d has invalid location data!\n", index);
        }

        index++;
    } while (mes_find_next(dword_5FF5B4, &mes_file_entry));

    return true;
}

// 0x4CACB0
void area_mod_unload()
{
    int index;

    if (dword_5FF5B4 != MES_FILE_HANDLE_INVALID) {
        FREE(dword_5FF5B0);
        FREE(dword_5FF5D8);
        FREE(dword_5FF5AC);
        FREE(dword_5FF5E8);

        for (index = 0; index < 8; index++) {
            FREE(dword_5FF5B8[index]);
        }

        FREE(dword_5FF5E4);
        FREE(dword_5FF5DC);
        FREE(dword_5FF5E0);

        mes_unload(dword_5FF5B4);
        dword_5FF5B4 = MES_FILE_HANDLE_INVALID;
    }
}

// 0x4CAD50
bool area_load(GameLoadInfo* load_info)
{
    int player;

    if (tig_file_fread(dword_5FF5E8, dword_5FF5A8, 1, load_info->stream) != 1) return false;
    if (tig_file_fread(&dword_5FF5EC, sizeof(dword_5FF5EC), 1, load_info->stream) != 1) return false;

    for (player = 0; player < 8; player++) {
        if (tig_file_fread(dword_5FF5B8[player], dword_5FF5A8, 1, load_info->stream) != 1) return false;
        if (tig_file_fread(&(dword_5FF5F0[player]), sizeof(dword_5FF5F0[0]), 1, load_info->stream) != 1) return false;
    }

    return true;
}

// 0x4CADF0
bool area_save(TigFile* stream)
{
    int player;

    if (tig_file_fwrite(dword_5FF5E8, dword_5FF5A8, 1, stream) != 1) return false;
    if (tig_file_fwrite(&dword_5FF5EC, sizeof(dword_5FF5EC), 1, stream) != 1) return false;

    for (player = 0; player < 8; player++) {
        if (tig_file_fwrite(dword_5FF5B8[player], dword_5FF5A8, 1, stream) != 1) return false;
        if (tig_file_fwrite(&(dword_5FF5F0[player]), sizeof(dword_5FF5F0[0]), 1, stream) != 1) return false;
    }

    return true;
}

// 0x4CAE80
int sub_4CAE80()
{
  return dword_5FF5A8;
}

// 0x4CAE90
char* sub_4CAE90(int index)
{
    return index >= 0 && index < dword_5FF5A8
        ? dword_5FF5B0[index]
        : NULL;
}

// 0x4CAEB0
char* sub_4CAEB0(int index)
{
    return index >= 0 && index < dword_5FF5A8
        ? dword_5FF5D8[index]
        : NULL;
}

// 0x4CAED0
int64_t sub_4CAED0(int index)
{
    return index >= 0 && index < dword_5FF5A8
        ? dword_5FF5AC[index]
        : 0;
}

// 0x4CAF00
void sub_4CAF00(int index, int* a2, int* a3)
{
    // FIXME: No overflow precondition.
    *a2 = dword_5FF5DC[index];
    *a3 = dword_5FF5E0[index];
}

// 0x4CAF50
bool sub_4CAF50(int64_t obj, int a2)
{
    int v1;

    if (obj != OBJ_HANDLE_NULL
        && obj_field_int32_get(obj, OBJ_F_TYPE) == OBJ_TYPE_PC) {
        if ((tig_net_flags & TIG_NET_CONNECTED) != 0) {
            v1 = sub_4A2B10(obj);
            if (v1 != -1) {
                return (dword_5FF5B8[v1][a2] & 1) != 0;
            }
        } else if (obj == player_get_pc_obj()) {
            return (dword_5FF5E8[a2] & 1) != 0;
        }
    }
    return false;
}

// 0x4CAFD0
bool sub_4CAFD0(int64_t obj, int a2)
{
    Packet101 pkt;
    int player;

    if (sub_4CAF50(obj, a2)) {
        return true;
    }

    if (obj != OBJ_HANDLE_NULL
        && obj_field_int32_get(obj, OBJ_F_TYPE) == OBJ_TYPE_PC) {
        if ((tig_net_flags & TIG_NET_CONNECTED) != 0) {
            if (!sub_4A2BA0()) {
                if ((tig_net_flags & TIG_NET_HOST) != 0) {
                    pkt.type = 101;
                    pkt.field_8 = sub_407EF0(obj);
                    pkt.field_20 = a2;
                    tig_net_send_app_all(&pkt, sizeof(pkt));

                    player = sub_4A2B10(obj);
                    if (player == -1) {
                        return false;
                    }

                    dword_5FF5B8[player][a2] |= 1;
                    dword_5FF5F0[player] = a2;
                    sub_4EE230(3, 1, player);
                }
                return true;
            }
        } else if (obj == player_get_pc_obj()) {
            dword_5FF5E8[a2] |= 1;
            sub_460790(3, 1);
            dword_5FF5EC = a2;
            return true;
        }
    }

    return false;
}

// 0x4CB100
int sub_4CB100(int64_t obj_handle)
{
    int v1;

    if (obj_handle != OBJ_HANDLE_NULL
        && obj_field_int32_get(obj_handle, OBJ_F_TYPE) == OBJ_TYPE_PC) {
        if ((tig_net_flags & 0x1) != 0) {
            v1 = sub_4A2B10(obj_handle);
            if (v1 != -1) {
                return dword_5FF5F0[v1];
            }
        } else {
            if (obj_handle == player_get_pc_obj()) {
                return dword_5FF5EC;
            }
        }
    }

    return 0;
}

// 0x4CB160
void sub_4CB160(int64_t obj)
{
    Packet102 pkt;
    int player;

    if (obj != OBJ_HANDLE_NULL
        && obj_field_int32_get(obj, OBJ_F_TYPE) == OBJ_TYPE_PC) {
        if ((tig_net_flags & TIG_NET_CONNECTED) != 0) {
            if (!sub_4A2BA0()) {
                if ((tig_net_flags & TIG_NET_HOST) == 0) {
                    return;
                }

                pkt.type = 102;
                pkt.field_8 = sub_407EF0(obj);
                tig_net_send_app_all(&pkt, sizeof(pkt));

                player = sub_4A2B10(obj);
                if (player != -1) {
                    dword_5FF5F0[player] = 0;
                }
            }
        } else if (obj == player_get_pc_obj()) {
            dword_5FF5EC = 0;
        }
    }
}

// 0x4CB220
int sub_4CB220(int64_t obj)
{
    int index;
    int v1;
    int64_t v2;
    int64_t v3;

    v1 = 0;
    v2 = sub_4B96F0(sub_4CAED0(0), obj);
    for (index = 1; index < dword_5FF5A8; index++) {
        v3 = sub_4B96F0(sub_4CAED0(index), obj);
        if (v3 < v2) {
            v1 = index;
            v2 = v3;
        }
    }

    return v1;
}

// 0x4CB4D0
int sub_4CB4D0(int64_t location, int a2)
{
    // TODO: Incomplete.
}

// 0x4CB630
int sub_4CB630(int64_t obj)
{
    int v1;
    int v2;
    int v3;

    v1 = sub_4BE380(sub_4CFC50(obj));
    if (v1 == 0) {
        return 0;
    }

    v2 = sub_4CB220(obj);
    if (v2 == 0) {
        return 0;
    }

    v3 = sub_4BE380(sub_4CFC50(sub_4CAED0(v2)));
    if (v3 == 0) {
        return 0;
    }

    if (v1 != v3) {
        return 0;
    }

    return v2;
}

// 0x4CB6A0
int sub_4CB6A0(int64_t obj)
{
    int map;
    int area;

    if (obj == OBJ_HANDLE_NULL) {
        return 0;
    }

    map = sub_40FF40();
    if (!map_get_area(map, &area)) {
        return 0;
    }

    if (area == 0
        && map == sub_40FF50(MAP_TYPE_START_MAP)) {
        area = sub_4CB630(obj_field_int64_get(obj, OBJ_F_LOCATION));
    }

    return area;
}
