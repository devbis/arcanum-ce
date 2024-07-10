#include "game/antiteleport.h"

#include <tig/tig.h>

#include "game/mes.h"

typedef struct S5FC490 {
    void* field_0;
    int field_4;
} S5FC490;

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
static void sub_4BDC40(S5FC490* a1);

// 0x5FC490
static S5FC490 stru_5FC490;

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
    sub_4BDC40(&stru_5FC490);

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

    if (!sub_4BDC60(antiteleport_mes_file, &stru_5FC490, 1000)) {
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

    if (stru_5FC490.field_0 != NULL) {
        free(stru_5FC490.field_0);
        stru_5FC490.field_0 = NULL;
        stru_5FC490.field_4 = 0;
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
void sub_4BDC40(S5FC490* a1)
{
    a1->field_0 = NULL;
    a1->field_4 = 0;
}
