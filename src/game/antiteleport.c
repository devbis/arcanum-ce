#include "game/antiteleport.h"

#include <tig/tig.h>

#include "game/area.h"
#include "game/location.h"
#include "game/map.h"
#include "game/mes.h"
#include "game/obj.h"

typedef struct MapEntry {
    int map;
} MapEntry;

static_assert(sizeof(MapEntry) == 0x4, "wrong size");

typedef struct AntiTeleportMapList {
    MapEntry* entries;
    int cnt;
} AntiTeleportMapList;

static_assert(sizeof(AntiTeleportMapList) == 0x8, "wrong size");

typedef struct RegionEntry {
    int64_t location;
    int64_t radius;
} RegionEntry;

static_assert(sizeof(RegionEntry) == 0x10, "wrong size");

typedef struct AntiteleportRegionList {
    RegionEntry* entries;
    int cnt;
} AntiteleportRegionList;

static_assert(sizeof(AntiteleportRegionList) == 0x8, "wrong size");

static void sub_4BDAC0();
static void antiteleport_region_list_init(AntiteleportRegionList* region_list);
static bool antiteleport_region_list_load(mes_file_handle_t mes_file, AntiteleportRegionList* region_list, int num);
static void antiteleport_map_list_init(AntiTeleportMapList* map_list);
static bool antiteleport_map_list_load(mes_file_handle_t mes_file, AntiTeleportMapList* map_list, int num);

// 0x5FC490
static AntiTeleportMapList antiteleport_map_list;

// 0x5FC498
static AntiteleportRegionList antiteleport_region_list;

// 0x5FC4A0
static bool antiteleport_initialized;

// 0x5FC4A4
static bool antiteleport_disabled;

// 0x5FC4A8
static bool antiteleport_mod_loaded;

// 0x4BD980
bool antiteleport_init(GameInitInfo* init_info)
{
    (void)init_info;

    if (antiteleport_disabled) {
        return true;
    }

    antiteleport_region_list_init(&antiteleport_region_list);
    antiteleport_map_list_init(&antiteleport_map_list);

    antiteleport_initialized = true;

    return true;
}

// 0x4BD9C0
void antiteleport_exit()
{
    if (!antiteleport_disabled) {
        sub_4BDAC0();
        antiteleport_initialized = false;
    }
}

// 0x4BD9E0
bool antiteleport_mod_load()
{
    mes_file_handle_t antiteleport_mes_file;

    if (antiteleport_disabled) {
        return true;
    }

    if (!mes_load("Rules\\AntiTeleport.mes", &antiteleport_mes_file)) {
        // FIXME: Typo in function name.
        tig_debug_printf("AntiTeleport: antiteleport_init: Note: No Message File Detected.\n");
        return true;
    }

    tig_str_parse_set_separator(',');

    if (!antiteleport_region_list_load(antiteleport_mes_file, &antiteleport_region_list, 100)) {
        tig_debug_println("Disabling Anti-Teleport Regions because of bad message file (Region List).");
        mes_unload(antiteleport_mes_file);
        antiteleport_disabled = true;
        return true;
    }

    if (!antiteleport_map_list_load(antiteleport_mes_file, &antiteleport_map_list, 1000)) {
        tig_debug_println("Disabling Anti-Teleport Regions because of bad message file (Map List).");
        mes_unload(antiteleport_mes_file);
        antiteleport_disabled = true;
        return true;
    }

    mes_unload(antiteleport_mes_file);
    antiteleport_mod_loaded = true;

    return true;
}

// 0x4BDAB0
void antiteleport_mod_unload()
{
    sub_4BDAC0();
    antiteleport_mod_loaded = false;
}

// 0x4BDAC0
void sub_4BDAC0()
{
    if (antiteleport_region_list.entries != NULL) {
        FREE(antiteleport_region_list.entries);
        antiteleport_region_list.entries = NULL;
        antiteleport_region_list.cnt = 0;
    }

    if (antiteleport_map_list.entries != NULL) {
        FREE(antiteleport_map_list.entries);
        antiteleport_map_list.entries = NULL;
        antiteleport_map_list.cnt = 0;
    }
}

// 0x4BDB10
void antiteleport_region_list_init(AntiteleportRegionList* region_list)
{
    region_list->entries = NULL;
    region_list->cnt = 0;
}

// 0x4BDB30
bool antiteleport_region_list_load(mes_file_handle_t mes_file, AntiteleportRegionList* region_list, int num)
{
    MesFileEntry mes_file_entry;
    int cnt;
    RegionEntry* region_entry;
    char* str;
    int64_t x;
    int64_t y;
    int radius;

    cnt = 0;
    mes_file_entry.num = num;
    if (mes_search(mes_file, &mes_file_entry)) {
        do {
            mes_file_entry.num++;
            cnt++;
        } while (mes_search(mes_file, &mes_file_entry));

        if (cnt != 0) {
            region_list->entries = (RegionEntry*)MALLOC(sizeof(RegionEntry) * cnt);

            mes_file_entry.num = num;
            while (cnt > 0) {
                region_entry = &(region_list->entries[region_list->cnt]);
                mes_get_msg(mes_file, &mes_file_entry);

                str = mes_file_entry.str;
                tig_str_parse_value_64(&str, &x);
                tig_str_parse_value_64(&str, &y);
                region_entry->location = (x << 32) | y;

                tig_str_parse_value(&str, &radius);
                region_entry->radius = (int64_t)radius << 6;

                mes_file_entry.num++;
                region_list->cnt++;
                cnt--;
            }
        }
    }

    return true;
}

// 0x4BDC40
void antiteleport_map_list_init(AntiTeleportMapList* map_list)
{
    map_list->entries = NULL;
    map_list->cnt = 0;
}

// 0x4BDC60
bool antiteleport_map_list_load(mes_file_handle_t mes_file, AntiTeleportMapList* map_list, int num)
{
    MesFileEntry mes_file_entry;
    int cnt;
    MapEntry* map_entry;
    char* str;
    int map;

    cnt = 0;
    mes_file_entry.num = num;
    if (mes_search(mes_file, &mes_file_entry)) {
        do {
            mes_file_entry.num++;
            cnt++;
        } while (mes_search(mes_file, &mes_file_entry));

        if (cnt != 0) {
            map_list->entries = (MapEntry*)MALLOC(sizeof(MapEntry) * cnt);

            mes_file_entry.num = num;
            while (cnt > 0) {
                map_entry = &(map_list->entries[map_list->cnt]);
                mes_get_msg(mes_file, &mes_file_entry);

                str = mes_file_entry.str;
                tig_str_parse_value(&str, &map);
                if (map > 5000) {
                    map -= 4999;
                } else {
                    map += 1;
                }
                map_entry->map = map;

                mes_file_entry.num++;
                map_list->cnt++;
                cnt--;
            }
        }
    }

    return true;
}

// 0x4BDD30
bool sub_4BDD30(int64_t obj, int64_t a2)
{
    int map;
    int index;
    int area;
    int64_t loc;
    int64_t distance;
    int num_regions;

    if (obj == OBJ_HANDLE_NULL) {
        return false;
    }

    if (!antiteleport_mod_loaded) {
        return true;
    }

    map = sub_40FF40();

    for (index = 0; index < antiteleport_map_list.cnt; index++) {
        if (map == antiteleport_map_list.entries[index].map) {
            break;
        }
    }

    if (index >= antiteleport_map_list.cnt) {
        return false;
    }

    if (map == sub_40FF50(MAP_TYPE_START_MAP)) {
        loc = obj_field_int64_get(obj, OBJ_F_LOCATION);
    } else if (map_get_area(map, &area)) {
        loc = area_get_location(area);
    } else {
        return false;
    }

    num_regions = 0;
    for (index = 0; index < antiteleport_region_list.cnt; index++) {
        distance = sub_4B96F0(loc, antiteleport_region_list.entries[index].location);
        if (distance < antiteleport_region_list.entries[index].radius) {
            return false;
        }
        num_regions++;
    }

    if (a2 != 0) {
        for (index = 0; index < antiteleport_region_list.cnt; index++) {
            distance = sub_4B96F0(a2, antiteleport_region_list.entries[index].location);
            if (distance < antiteleport_region_list.entries[index].radius) {
                return false;
            }
        }
    }

    return true;
}
