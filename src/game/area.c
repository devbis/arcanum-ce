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

#define DEFAULT_RADIUS 5

#define AREA_KNOWN 0x1

static int get_nearest_area(int64_t obj);
static int sub_4CB630(int64_t loc);

// 0x5FF5A8
static int area_count;

// 0x5FF5AC
static int64_t* area_locations;

// 0x5FF5B0
static char** area_names;

// 0x5FF5B4
static mes_file_handle_t area_gamearea_mes_file;

// 0x5FF5B8
static uint8_t* area_flags_per_player[8];

// 0x5FF5D8
static char** area_descriptions;

// 0x5FF5DC
static int* area_wm_x_offsets;

// 0x5FF5E0
static int* area_wm_y_offsets;

// 0x5FF5E4
static int* area_radiuses;

// 0x5FF5E8
static uint8_t* area_flags;

// 0x5FF5EC
static int area_last_known_area;

// 0x5FF5F0
static int area_last_known_area_per_player[8];

// 0x4CA940
bool area_init(GameInitInfo* init_info)
{
    (void)init_info;

    area_count = 0;

    return true;
}

// 0x4CA950
void area_reset()
{
    int area;
    int player;

    for (area = 0; area < area_count; area++) {
        area_flags[area] = 0;
    }

    for (player = 0; player < 8; player++) {
        for (area = 0; area < area_count; area++) {
            area_flags_per_player[player][area] = 0;
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

    area_count = 0;

    if (!mes_load("mes\\gamearea.mes", &area_gamearea_mes_file)) {
        return true;
    }

    mes_file_entry.num = 0;
    mes_get_msg(area_gamearea_mes_file, &mes_file_entry);
    do {
        area_count++;
    } while (mes_find_next(area_gamearea_mes_file, &mes_file_entry));

    area_names = (char**)MALLOC(sizeof(*area_names) * area_count);
    area_descriptions = (char**)MALLOC(sizeof(*area_descriptions) * area_count);
    area_locations = (int64_t*)MALLOC(sizeof(*area_locations) * area_count);
    area_flags = (uint8_t*)MALLOC(sizeof(*area_flags) * area_count);

    for (index = 0; index < 8; index++) {
        area_flags_per_player[index] = (uint8_t*)MALLOC(sizeof(*area_flags_per_player[index]) * area_count);
    }

    area_radiuses = (int*)MALLOC(sizeof(*area_radiuses) * area_count);
    area_wm_x_offsets = (int*)MALLOC(sizeof(*area_wm_x_offsets) * area_count);
    area_wm_y_offsets = (int*)MALLOC(sizeof(*area_wm_y_offsets) * area_count);

    index = 0;
    mes_file_entry.num = 0;
    mes_get_msg(area_gamearea_mes_file, &mes_file_entry);
    tig_str_parse_set_separator(',');

    do {
        str = mes_file_entry.str;
        tig_str_parse_value_64(&str, &x);
        tig_str_parse_value_64(&str, &y);
        area_locations[index] = location_make(x, y);
        area_wm_x_offsets[index] = 0;
        area_wm_y_offsets[index] = 0;
        tig_str_parse_value(&str, &(area_wm_x_offsets[index]));
        tig_str_parse_value(&str, &(area_wm_y_offsets[index]));

        pch = strchr(mes_file_entry.str, '/');
        if (pch != NULL) {
            area_names[index] = pch + 1;

            pch = strchr(pch + 1, '/');
            if (pch != NULL) {
                *pch++ = '\0';
                area_descriptions[index] = pch;
                area_flags[index] = 0;
                area_radiuses[index] = DEFAULT_RADIUS * 64;

                pch = strchr(pch, '/');
                if (pch != NULL) {
                    *pch++ = '\0';
                    if (tig_str_parse_named_value(&pch, "Radius:", &radius)) {
                        if (index > 0) {
                            area_radiuses[index] = radius * 64;
                        } else if (index == 0) {
                            area_radiuses[0] = 0;
                        } else {
                            // FIXME: Unreachable (and wrong).
                            area_radiuses[index] = -1;
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
    } while (mes_find_next(area_gamearea_mes_file, &mes_file_entry));

    return true;
}

// 0x4CACB0
void area_mod_unload()
{
    int player;

    if (area_gamearea_mes_file != MES_FILE_HANDLE_INVALID) {
        FREE(area_names);
        FREE(area_descriptions);
        FREE(area_locations);
        FREE(area_flags);

        for (player = 0; player < 8; player++) {
            FREE(area_flags_per_player[player]);
        }

        FREE(area_radiuses);
        FREE(area_wm_x_offsets);
        FREE(area_wm_y_offsets);

        mes_unload(area_gamearea_mes_file);
        area_gamearea_mes_file = MES_FILE_HANDLE_INVALID;
    }
}

// 0x4CAD50
bool area_load(GameLoadInfo* load_info)
{
    int player;

    if (tig_file_fread(area_flags, area_count, 1, load_info->stream) != 1) return false;
    if (tig_file_fread(&area_last_known_area, sizeof(area_last_known_area), 1, load_info->stream) != 1) return false;

    for (player = 0; player < 8; player++) {
        if (tig_file_fread(area_flags_per_player[player], area_count, 1, load_info->stream) != 1) return false;
        if (tig_file_fread(&(area_last_known_area_per_player[player]), sizeof(area_last_known_area_per_player[0]), 1, load_info->stream) != 1) return false;
    }

    return true;
}

// 0x4CADF0
bool area_save(TigFile* stream)
{
    int player;

    if (tig_file_fwrite(area_flags, area_count, 1, stream) != 1) return false;
    if (tig_file_fwrite(&area_last_known_area, sizeof(area_last_known_area), 1, stream) != 1) return false;

    for (player = 0; player < 8; player++) {
        if (tig_file_fwrite(area_flags_per_player[player], area_count, 1, stream) != 1) return false;
        if (tig_file_fwrite(&(area_last_known_area_per_player[player]), sizeof(area_last_known_area_per_player[0]), 1, stream) != 1) return false;
    }

    return true;
}

// 0x4CAE80
int area_get_count()
{
  return area_count;
}

// 0x4CAE90
char* area_get_name(int area)
{
    return area >= 0 && area < area_count
        ? area_names[area]
        : NULL;
}

// 0x4CAEB0
char* area_get_description(int area)
{
    return area >= 0 && area < area_count
        ? area_descriptions[area]
        : NULL;
}

// 0x4CAED0
int64_t area_get_location(int area)
{
    return area >= 0 && area < area_count
        ? area_locations[area]
        : 0;
}

// 0x4CAF00
void area_get_wm_offset(int area, int* x, int* y)
{
    // FIXME: No bound checking guard seen in previous functions.
    *x = area_wm_x_offsets[area];
    *y = area_wm_y_offsets[area];
}

// 0x4CAF50
bool area_is_known(int64_t pc_obj, int area)
{
    int player;

    if (pc_obj != OBJ_HANDLE_NULL
        && obj_field_int32_get(pc_obj, OBJ_F_TYPE) == OBJ_TYPE_PC) {
        if (tig_net_is_active()) {
            player = sub_4A2B10(pc_obj);
            if (player != -1) {
                return (area_flags_per_player[player][area] & 1) != 0;
            }
        } else if (pc_obj == player_get_local_pc_obj()) {
            return (area_flags[area] & AREA_KNOWN) != 0;
        }
    }

    return false;
}

// 0x4CAFD0
bool area_set_known(int64_t pc_obj, int area)
{
    PacketAreaKnownSet pkt;
    int player;

    if (area_is_known(pc_obj, area)) {
        return true;
    }

    if (pc_obj != OBJ_HANDLE_NULL
        && obj_field_int32_get(pc_obj, OBJ_F_TYPE) == OBJ_TYPE_PC) {
        if (tig_net_is_active()) {
            if (!multiplayer_is_locked()) {
                if (tig_net_is_host()) {
                    pkt.type = 101;
                    pkt.oid = sub_407EF0(pc_obj);
                    pkt.area = area;
                    tig_net_send_app_all(&pkt, sizeof(pkt));

                    player = sub_4A2B10(pc_obj);
                    if (player == -1) {
                        return false;
                    }

                    area_flags_per_player[player][area] |= 1;
                    area_last_known_area_per_player[player] = area;
                    mp_ui_toggle_primary_button(UI_PRIMARY_BUTTON_WORLDMAP, true, player);
                }
                return true;
            }
        } else if (pc_obj == player_get_local_pc_obj()) {
            area_flags[area] |= AREA_KNOWN;
            ui_toggle_primary_button(UI_PRIMARY_BUTTON_WORLDMAP, true);
            area_last_known_area = area;
            return true;
        }
    }

    return false;
}

// 0x4CB100
int area_get_last_known_area(int64_t pc_obj)
{
    int player;

    if (pc_obj != OBJ_HANDLE_NULL
        && obj_field_int32_get(pc_obj, OBJ_F_TYPE) == OBJ_TYPE_PC) {
        if (tig_net_is_active()) {
            player = sub_4A2B10(pc_obj);
            if (player != -1) {
                return area_last_known_area_per_player[player];
            }
        } else {
            if (pc_obj == player_get_local_pc_obj()) {
                return area_last_known_area;
            }
        }
    }

    return AREA_UNKNOWN;
}

// 0x4CB160
void area_reset_last_known_area(int64_t pc_obj)
{
    PacketAreaResetLastKnown pkt;
    int player;

    if (pc_obj != OBJ_HANDLE_NULL
        && obj_field_int32_get(pc_obj, OBJ_F_TYPE) == OBJ_TYPE_PC) {
        if (tig_net_is_active()) {
            if (!multiplayer_is_locked()) {
                if (!tig_net_is_host()) {
                    return;
                }

                pkt.type = 102;
                pkt.oid = sub_407EF0(pc_obj);
                tig_net_send_app_all(&pkt, sizeof(pkt));

                player = sub_4A2B10(pc_obj);
                if (player != -1) {
                    area_last_known_area_per_player[player] = AREA_UNKNOWN;
                }
            }
        } else if (pc_obj == player_get_local_pc_obj()) {
            area_last_known_area = AREA_UNKNOWN;
        }
    }
}

// 0x4CB220
int get_nearest_area(int64_t loc)
{
    int area;
    int nearest_area;
    int64_t nearest_dist;
    int64_t distance;

    nearest_area = AREA_UNKNOWN;
    nearest_dist = location_dist(area_get_location(0), loc);

    for (area = 1; area < area_count; area++) {
        distance = location_dist(area_get_location(area), loc);
        if (distance < nearest_dist) {
            nearest_area = area;
            nearest_dist = distance;
        }
    }

    return nearest_area;
}

// 0x4CB2A0
int area_get_nearest_known_area(int64_t loc, int64_t pc_obj, int64_t range)
{
    int area;
    int nearest_area;
    int64_t nearest_dist;
    int64_t dist;

    if (pc_obj == OBJ_HANDLE_NULL
        || obj_field_int32_get(pc_obj, OBJ_F_TYPE) != OBJ_TYPE_PC) {
        return AREA_UNKNOWN;
    }

    // NOTE: Original code is different.
    nearest_area = AREA_UNKNOWN;
    nearest_dist = range + 1;

    for (area = 1; area < area_count; area++) {
        dist = location_dist(area_get_location(area), loc);
        if (dist < nearest_dist) {
            if (area_is_known(pc_obj, area)) {
                nearest_area = area;
                nearest_dist = dist;
            }
        }
    }

    return nearest_area;
}

// 0x4CB4D0
int sub_4CB4D0(int64_t loc, bool a2)
{
    int area;
    int nearest_area;
    int64_t nearest_dist;
    int64_t dist;
    int64_t radius;

    // NOTE: Original code is different.
    nearest_area = AREA_UNKNOWN;
    nearest_dist = 99999;

    for (area = 1; area < area_count; area++) {
        radius = area_radiuses[area];
        if (a2 && radius <= 0) {
            radius = 64;
        }

        dist = location_dist(area_get_location(area), loc);
        if (dist < radius
            && dist < nearest_dist) {
            nearest_area = area;
            nearest_dist = dist;
        }
    }

    return nearest_area;
}

// 0x4CB630
int sub_4CB630(int64_t loc)
{
    int v1;
    int v2;
    int v3;

    v1 = townmap_get(sector_id_from_loc(loc));
    if (v1 == 0) {
        return AREA_UNKNOWN;
    }

    v2 = get_nearest_area(loc);
    if (v2 == 0) {
        return AREA_UNKNOWN;
    }

    v3 = townmap_get(sector_id_from_loc(area_get_location(v2)));
    if (v3 == 0) {
        return AREA_UNKNOWN;
    }

    if (v1 != v3) {
        return AREA_UNKNOWN;
    }

    return v2;
}

// 0x4CB6A0
int sub_4CB6A0(int64_t obj)
{
    int map;
    int area;

    if (obj == OBJ_HANDLE_NULL) {
        return AREA_UNKNOWN;
    }

    map = map_current_map();
    if (!map_get_area(map, &area)) {
        return AREA_UNKNOWN;
    }

    if (area == AREA_UNKNOWN
        && map == map_by_type(MAP_TYPE_START_MAP)) {
        area = sub_4CB630(obj_field_int64_get(obj, OBJ_F_LOCATION));
    }

    return area;
}
