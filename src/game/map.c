#include "game/map.h"

#include "game/description.h"
#include "game/gsound.h"
#include "game/light_scheme.h"
#include "game/location.h"
#include "game/mes.h"
#include "game/object.h"
#include "game/player.h"
#include "game/sector.h"
#include "game/stat.h"
#include "game/timeevent.h"

#define MAP_LIST_CAPACITY 200
#define MAP_NAME_LENGTH 256
#define MAP_MODULE_COUNT 17
#define SENTINEL 0xBADDBEEF

typedef bool(MapInitFunc)(GameContext* ctx);
typedef void(MapResetFunc)();
typedef bool(MapModuleLoadFunc)();
typedef void(MapModuleUnloadFunc)();
typedef void(MapExitFunc)();
typedef void(MapPingFunc)(unsigned int time);
typedef void(MapUpdateViewFunc)(ViewOptions* view_options);
typedef bool(MapSaveFunc)(TigFile* stream);
typedef bool(MapLoadFunc)(LoadContext* ctx);
typedef void(MapResizeFunc)(ResizeContext* ctx);

typedef struct MapModule {
    const char* name;
    MapInitFunc* init_func;
    MapResetFunc* reset_func;
    MapModuleLoadFunc* mod_load_func;
    MapModuleUnloadFunc* mod_unload_func;
    MapExitFunc* exit_func;
    MapPingFunc* ping_func;
    MapUpdateViewFunc* update_view_func;
    MapSaveFunc* save_func;
    MapLoadFunc* load_func;
    MapResizeFunc* resize_func;
} MapModule;

typedef struct MapListInfo {
    char name[MAP_NAME_LENGTH];
    long long x;
    long long y;
    int worldmap;
    int area;
} MapListInfo;

// See 0x40EA90.
static_assert(sizeof(MapListInfo) == 0x118, "wrong size");

static void map_load_postprocess();
static bool sub_411450(const char* name);
static void map_disable_objects();

// 0x59F058
static const char* off_59F058[MAP_TYPE_COUNT] = {
    "NONE",
    "START_MAP",
    "SHOPPING_MAP",
};

// 0x59F068
static MapModule map_modules[] = {
    { "Scroll" },
    { "Location" },
    { "Light" },
    { "Tile" },
    { "Roof" },
    { "Effect" },
    { "O_Name" },
    { "Object_Node" },
    { "Obj" },
    { "Proto" },
    { "Object" },
    { "Terrain" },
    { "Sector" },
    { "TB" },
    { "TF" },
    { "Wall" },
    { "JumpPoint" },
};

static_assert(sizeof(map_modules) / sizeof(map_modules[0]) == MAP_MODULE_COUNT, "wrong size");

// 0x59F3DC
static char off_59F3DC[] = "*** Deleted Map ***";

// 0x5D11E0
static long long qword_5D11E0;

// 0x5D11EC
static int dword_5D11EC;

// 0x5D11D0
static bool map_gender_check_enabled;

// 0x5D11D8
static int dword_5D11D8;

// 0x5D11F0
static bool dword_5D11F0;

// 0x5D11F4
static char* map_folder;

// 0x5D11F8
static char* map_name;

// 0x5D11FC
static int dword_5D11FC;

// 0x5D1200
static bool dword_5D1200;

// 0x5D1204
static MapListInfo* map_list_info;

// 0x5D1208
static bool map_in_map_clear_objects;

// 0x5D120C
static int map_list_info_count;

// 0x5D1210
static int dword_5D1210;

// 0x5D1214
static int dword_5D1214[MAP_TYPE_COUNT];

// 0x40EA90
bool map_init(GameContext* ctx)
{
    if (dword_5D11F0) {
        return true;
    }

    map_name = (char*)calloc(MAX_PATH, sizeof(*map_name));
    map_folder = (char*)calloc(MAX_PATH, sizeof(*map_folder));
    map_list_info = (MapListInfo*)calloc(MAP_LIST_CAPACITY, sizeof(*map_list_info));

    for (int index = 0; index < MAP_MODULE_COUNT; index++) {
        if (map_modules[index].init_func != NULL) {
            if (!map_modules[index].init_func(ctx)) {
                tig_debug_printf("map_init(): init function %d (%s) failed\n", map_modules[index].name);

                while (--index >= 0) {
                    if (map_modules[index].exit_func != NULL) {
                        map_modules[index].exit_func();
                    }
                }

                free(map_name);
                free(map_folder);
                free(map_list_info);

                return false;
            }
        }
    }

    map_gender_check_enabled = 0;
    dword_5D11F0 = true;
    dword_5D1200 = ctx->editor;
    dword_5D11EC = 0;

    return true;
}

// 0x40EBA0
void map_reset()
{
    unsigned int reset_start;
    unsigned int start;

    if (dword_5D11F0) {
        tig_debug_printf("\nmap_reset: Resetting...\n");
        tig_timer_now(&reset_start);

        tig_debug_printf("map_reset: clearing magictech...");
        tig_timer_now(&start);
        sub_457100();
        tig_debug_printf("done. Time (ms): %d\n", tig_timer_elapsed(start));

        tig_debug_printf("map_reset: interrupting anims...");
        tig_timer_now(&start);
        sub_424250();
        tig_debug_printf("done. Time (ms): %d\n", tig_timer_elapsed(start));

        tig_debug_printf("map_reset: closing map...");
        tig_timer_now(&start);
        map_close();
        dword_5D1210 = 0;
        tig_debug_printf("done. Time (ms): %d\n", tig_timer_elapsed(start));

        for (int index = 0; index < MAP_MODULE_COUNT; index++) {
            if (map_modules[index].reset_func != NULL) {
                tig_debug_printf("map_reset: Processing Reset Function: %d", index);
                tig_timer_now(&start);
                map_modules[index].reset_func();
                tig_debug_printf(" done. Time (ms): %d.\n", tig_timer_elapsed(start));
            }
        }

        dword_5D11EC = 0;

        tig_debug_printf("map_reset: Done.  Total time: %f seconds.\n", (double)tig_timer_elapsed(reset_start) / 1000.0);
    }
}

// 0x40ECE0
void map_resize(ResizeContext* ctx)
{
    for (int index = 0; index < MAP_MODULE_COUNT; index++) {
        if (map_modules[index].resize_func != NULL) {
            map_modules[index].resize_func(ctx);
        }
    }
}

// 0x40ED10
bool map_mod_load()
{
    if (!map_list_info_load()) {
        return false;
    }

    for (int index = 0; index < MAP_MODULE_COUNT; index++) {
        if (map_modules[index].mod_load_func != NULL) {
            if (!map_modules[index].mod_load_func()) {
                tig_debug_printf("map_mod_load(): mod load function %d failed\n", index);

                while (--index >= 0) {
                    if (map_modules[index].mod_unload_func != NULL) {
                        map_modules[index].mod_unload_func();
                    }
                }

                return false;
            }
        }
    }

    return true;
}

// 0x40ED80
void map_mod_unload()
{
    for (int index = MAP_MODULE_COUNT - 1; index >= 0; index--) {
        if (map_modules[index].mod_unload_func != NULL) {
            map_modules[index].mod_unload_func();
        }
    }

    memset(map_list_info, 0, 280);
    map_list_info_count = 0;
}

// 0x40EDC0
void map_exit()
{
    if (dword_5D11F0) {
        map_close();
    }

    for (int index = MAP_MODULE_COUNT - 1; index >= 0; index--) {
        if (map_modules[index].exit_func != NULL) {
            map_modules[index].exit_func();
        }
    }

    free(map_name);
    free(map_folder);
    free(map_list_info);

    dword_5D11F0 = false;
}

// 0x40EE20
void map_ping(unsigned int time)
{
    for (int index = 0; index < MAP_MODULE_COUNT; index++) {
        if (map_modules[index].ping_func != NULL) {
            map_modules[index].ping_func(time);
        }
    }
}

// 0x40EE50
bool map_save(TigFile* stream)
{
    int sentinel = SENTINEL;
    unsigned int save_start;
    unsigned int start;
    long start_pos = 0;

    tig_debug_printf("\nmap_save()\n");
    tig_timer_now(&save_start);

    if (!dword_5D11F0) {
        return false;
    }

    tig_debug_printf("map_save: flushing map...");
    tig_timer_now(&start);
    map_flush(1);
    tig_debug_printf("done.  Time (ms): %d\n", tig_timer_elapsed(start));

    tig_debug_printf("map_save: saving map names...");
    tig_timer_now(&start);

    if (tig_file_fputs(map_name, stream) == -1) {
        tig_debug_printf("map_save(): error writing map name.\n");
        return false;
    }

    if (tig_file_fputc('\n', stream) == -1) {
        tig_debug_printf("map_save(): error writing map name delimiter.\n");
        return false;
    }

    if (tig_file_fputs(map_folder, stream) == -1) {
        tig_debug_printf("map_save(): error writing map save folder.\n");
        return false;
    }

    if (tig_file_fputc('\n', stream) == -1) {
        tig_debug_printf("map_save(): error writing map save folder delimiter.\n");
        return false;
    }

    tig_debug_printf("done.  Time (ms): %d\n", tig_timer_elapsed(start));

    // NOTE: This loop is a little bit odd - on any error it stops but does not
    // return false to indicate something bad happened while saving. Could lead
    // to unreadable save games. Check.
    for (int index = 0; index < MAP_MODULE_COUNT; index++) {
        if (map_modules[index].save_func != NULL) {
            tig_debug_printf("map_save: Function %d (%s)", index, map_modules[index].name);
            tig_timer_now(&start);

            if (!map_modules[index].save_func(stream)) {
                tig_debug_printf("map_save: save function %d (%s) failed\n", index, map_modules[index].name);
                break;
            }

            long pos;
            tig_file_fgetpos(stream, &pos);
            tig_debug_printf(" wrote to: %lu, Total: (%lu), Time (ms): %d\n",
                pos,
                pos - start_pos,
                tig_timer_elapsed(start));
            start_pos = pos;

            if (tig_file_fwrite(&sentinel, sizeof(sentinel), 1, stream) != 1) {
                tig_debug_printf("map_save(): ERROR: Sentinel Failed to Save!");
                break;
            }
        }
    }

    tig_debug_printf("map_save: Save Complete.  Total time: %f seconds.\n", (double)tig_timer_elapsed(save_start) / 1000.0);

    return true;
}

// 0x40F080
bool map_load(LoadContext* ctx)
{
    int sentinel;
    unsigned int load_start;
    unsigned int start;
    long start_pos = 0;

    tig_debug_printf("\nmap_load\n");
    tig_timer_now(&load_start);

    if (!dword_5D11F0) {
        return false;
    }

    tig_debug_printf("map_load: retrieving map names...");
    tig_timer_now(&start);

    char name[MAX_PATH];
    if (tig_file_fgets(name, sizeof(name), ctx->stream) == NULL) {
        map_close();
        return false;
    }

    const size_t name_length = strlen(name);
    if (name[name_length - 1] == '\n') {
        name[name_length - 1] = '\0';
    }

    char folder[MAX_PATH];
    if (tig_file_fgets(folder, sizeof(folder), ctx->stream) == NULL) {
        map_close();
        return false;
    }

    const size_t folder_length = strlen(folder);
    if (folder[folder_length - 1] == '\n') {
        folder[folder_length - 1] = '\0';
    }

    tig_debug_printf("done.  Time (ms): %d\n", tig_timer_elapsed(start));

    for (int index = 0; index < MAP_MODULE_COUNT; index++) {
        if (map_modules[index].load_func != NULL) {
            tig_debug_printf("map_load: Function %d (%s)", index, map_modules[index].name);
            tig_timer_now(&start);

            if (!map_modules[index].load_func(ctx)) {
                tig_debug_printf("map_load(): error calling map load func %d (%s)\n", index, map_modules[index].name);
                map_close();
                return false;
            }

            if (tig_file_fread(&sentinel, sizeof(sentinel), 1, ctx->stream) != 1) {
                tig_debug_printf("map_load: ERROR: Load Sentinel Failed to Load!\n");
                // FIXME: Missing map_close?
                return false;
            }

            if (sentinel != SENTINEL) {
                tig_debug_printf("map_load: ERROR: Load Sentinel Failed to Match!\n");
                // FIXME: Missing map_close?
                return false;
            }

            long pos;
            tig_file_fgetpos(ctx->stream, &pos);
            tig_debug_printf(" read to: %lu, Total: (%lu), Time (ms): %d\n",
                pos,
                start_pos - pos,
                tig_timer_elapsed(start));
            start_pos = pos;
        }
    }

    tig_debug_printf("map_load: opening map...");
    tig_timer_now(&start);
    bool success = map_open(name, folder, 1);
    tig_debug_printf("done.  Time (ms): %d\n", tig_timer_elapsed(start));

    tig_debug_printf("map_load: Load Complete.  Total time: %f seconds.\n", (double)tig_timer_elapsed(load_start) / 1000.0);

    return success;
}

// 0x40F320
void map_update_view(ViewOptions* view_options)
{
    if (dword_5D11F0) {
        for (int index = MAP_MODULE_COUNT - 1; index >= 0; index--) {
            if (map_modules[index].update_view_func != NULL) {
                map_modules[index].update_view_func(view_options);
            }
        }
    }
}

// 0x40FEE0
bool map_get_name(int map, char** name)
{
    if (map >= 0 && map < map_list_info_count) {
        *name = map_list_info[map].name;
        return true;
    } else {
        *name = NULL;
        return false;
    }
}

// 0x40FF40
int sub_40FF40()
{
    return dword_5D1210;
}

// 0x40FF50
int sub_40FF50(int map_type)
{
    if (map_type >= 0 && map_type < MAP_TYPE_COUNT) {
        return dword_5D1214[map_type];
    } else {
        return 0;
    }
}

// 0x40FF70
bool map_get_starting_location(int map, long long* x, long long* y)
{
    if (map >= 0 && map < map_list_info_count) {
        *x = map_list_info[map].x;
        *y = map_list_info[map].y;
        return true;
    } else {
        *x = 0;
        *y = 0;
        return false;
    }
}

// 0x40FFE0
bool map_get_area(int map, int* area)
{
    if (map >= 0 && map < map_list_info_count) {
        *area = map_list_info[map].area;
        return true;
    } else {
        *area = 0;
        return false;
    }
}

// 0x410020
bool map_get_worldmap(int map, int* worldmap)
{
    if (map >= 0 && map < map_list_info_count) {
        *worldmap = map_list_info[map].worldmap;
        return true;
    } else {
        *worldmap = -1;
        return false;
    }
}

// 0x410060
bool map_is_clearing_objects()
{
    return map_in_map_clear_objects;
}

// 0x410270
int sub_410270()
{
    return dword_5D11FC;
}

// 0x410280
void sub_410280(long long* location)
{
    if (location != NULL) {
        *location = qword_5D11E0;
    }
}

// 0x4102C0
void sub_4102C0(char** name, char** folder)
{
    if (name != NULL) {
        *name = map_name;
    }

    if (folder != NULL) {
        *folder = map_folder;
    }
}

// 0x410C50
void map_load_postprocess()
{
    int64_t obj;
    int v1;
    unsigned int flags;
    TimeEvent timeevent;

    if (sub_4082C0(&obj, &v1)) {
        do {
            if (!sub_43D990(obj)) {
                sub_406520(obj);
            }

            flags = obj_field_int32_get(obj, OBJ_F_FLAGS);
            if ((flags & OF_TELEPORTED) != 0) {
                timeevent.type = TIMEEVENT_TYPE_TELEPORTED;
                timeevent.params[0].object_value = obj;
                sub_45B820(&timeevent);

                flags &= ~OF_TELEPORTED;
                obj_field_int32_set(obj, OBJ_F_FLAGS, flags);
            }
        } while (sub_408390(&obj, &v1));
    }
}

// 0x411450
bool sub_411450(const char* name)
{
    char path[TIG_MAX_PATH];
    TigFile* stream;
    int64_t obj;

    sprintf(path, "%s\\mobile.mdy", name);

    if (tig_file_exists(path, NULL)) {
        stream = tig_file_fopen(path, "rb");
        if (stream == NULL) {
            tig_debug_printf("Error opening mobile dynamic objects file %s for reading.\n", path);
            return false;
        }

        while (obj_read(stream, &obj)) {
            // Noop.
        }

        if (tig_file_feof(stream) == 0) {
            tig_file_fclose(stream);
            tig_debug_printf("Error reading mobile dynamic objects from file %s\n", path);
        }

        tig_file_fclose(stream);
    }

    return true;
}

// 0x411520
void map_clear_objects()
{
    if (map_in_map_clear_objects) {
        tig_debug_println("Waring: map_clear_objects attempting to recurse.");
    } else {
        map_in_map_clear_objects = true;

        timeevent_clear_for_map_close();

        object_id_t obj;
        int v1;
        if (sub_4082C0(&obj, &v1)) {
            do {
                if (!sub_43D990(obj)) {
                    sub_43CF70(obj);
                    sub_43CFF0(obj);
                }
            } while (sub_408390(&obj, &v1));
        }

        sub_4D0B40();
        sub_4E5300();

        map_in_map_clear_objects = false;
    }
}

// 0x4115D0
void sub_4115D0(const char* name)
{
    char path[MAX_PATH];
    strcpy(path, name);
    strcat(path, "\\mapinfo.txt");

    TigFile* stream = tig_file_fopen(path, "rt");
    if (stream != NULL) {
        char key[64];
        while (tig_file_fgets(path, sizeof(path), stream) != NULL) {
            char* str = path;

            tig_str_parse_set_separator(':');
            tig_str_parse_str_value(&str, key);

            if (_strcmpi(key, "LightScheme") == 0) {
                int light_scheme;

                tig_str_parse_value(&str, &light_scheme);
                light_scheme_set_map_default(light_scheme);
                light_scheme_set(LIGHT_SCHEME_MAP_DEFAULT, light_scheme_get_hour());
            } else if (_strcmpi(key, "SoundScheme") == 0) {
                int v1;
                int v2;

                tig_str_parse_set_separator(',');
                tig_str_parse_value(&str, &v1);
                tig_str_parse_value(&str, &v2);

                if (!dword_5D1200) {
                    sub_41BD50(v1, v2);
                }
            }
        }

        tig_file_fclose(stream);
    }
}

// 0x412380
void map_enable_gender_check()
{
    map_gender_check_enabled = true;
}

// 0x411750
void map_disable_objects()
{
    int64_t obj;
    int v1;
    int64_t location;
    int sector;
    unsigned int flags;

    if (sub_4082C0(&obj, &v1)) {
        do {
            if (!sub_43D990(obj)) {
                location = obj_field_int64_get(obj, OBJ_F_LOCATION);
                sector = sub_4CFC50(location);
                if (!sub_4D0DE0(sector) && !player_is_pc_obj(obj)) {
                    if (sub_45DDA0() != player_get_pc_obj()) {
                        flags = obj_field_int32_get(obj, OBJ_F_FLAGS);
                        flags |= OF_OFF;
                        obj_field_int32_set(obj, OBJ_F_FLAGS, flags);
                    }
                }
            }
        } while (sub_408390(&obj, &v1));
    }
}

// 0x412390
void map_gender_check()
{
    char name[1000];
    location_t location;

    if (map_gender_check_enabled) {
        object_id_t obj;
        int v1;
        if (sub_4082C0(&obj, &v1)) {
            do {
                if (obj_field_int32_get(obj, OBJ_F_TYPE) == OBJ_TYPE_NPC) {
                    const int description = obj_field_int32_get(obj, OBJ_F_DESCRIPTION);
                    if (description >= DESCRIPTION_FIRST_CRITTER_NAME && description <= DESCRIPTION_LAST_CRITTER_NAME) {
                        const int actual_gender = stat_get_base(obj, STAT_GENDER);
                        const int expected_gender = description % GENDER_COUNT;
                        if (actual_gender == GENDER_MALE && expected_gender == GENDER_FEMALE) {
                            sub_441B60(obj, obj, name);

                            location = obj_field_int64_get(obj, OBJ_F_LOCATION);
                            tig_debug_printf("Male character with female description: %s at location %I64d %I64d\n",
                                name,
                                location);
                        } else if (actual_gender == GENDER_FEMALE && expected_gender == GENDER_FEMALE) {
                            // FIXME: Wrong condition above.
                            sub_441B60(obj, obj, name);

                            location = obj_field_int64_get(obj, OBJ_F_LOCATION);
                            tig_debug_printf("Female character with male description: %s at location %I64d %I64d\n",
                                name,
                                location);
                        }
                    }

                    const int unknown_description = obj_field_int32_get(obj, OBJ_F_CRITTER_DESCRIPTION_UNKNOWN);
                    if (unknown_description >= DESCRIPTION_FIRST_CRITTER_NAME && unknown_description <= DESCRIPTION_LAST_CRITTER_NAME) {
                        const int actual_gender = stat_get_base(obj, STAT_GENDER);
                        const int expected_gender = unknown_description % GENDER_COUNT;
                        if (actual_gender == GENDER_MALE && expected_gender == GENDER_FEMALE) {
                            sub_441B60(obj, obj, name);

                            location = obj_field_int64_get(obj, OBJ_F_LOCATION);
                            tig_debug_printf("Male character with female unknown description: %s at location %I64d %I64d\n",
                                name,
                                location);
                        } else if (actual_gender == GENDER_FEMALE && expected_gender == GENDER_FEMALE) {
                            // FIXME: Wrong condition above.
                            sub_441B60(obj, obj, name);

                            location = obj_field_int64_get(obj, OBJ_F_LOCATION);
                            tig_debug_printf("Female character with male unknown description: %s at location %I64d %I64d\n",
                                name,
                                location);
                        }
                    }
                }
            } while (sub_408390(&obj, &v1));
        }
    }
}

// 0x4125C0
bool map_list_info_load()
{
    MesFileEntry mes_file_entry;

    if (!mes_load("Rules\\MapList.mes", &dword_5D11D8)) {
        return false;
    }

    tig_str_parse_set_separator(',');

    mes_file_entry.num = 5000;
    while (mes_search(dword_5D11D8, &mes_file_entry)) {
        char* str = mes_file_entry.str;
        tig_str_parse_str_value(&str, map_list_info[map_list_info_count].name);

        // NOTE: This check is silly. When string it long enough it will easly
        // overrun `MAP_NAME_LENGTH` and crash before this check
        if (strlen(map_list_info[map_list_info_count].name) >= MAP_NAME_LENGTH) {
            tig_debug_println("ERROR: Map name is too long");
            mes_unload(dword_5D11D8);
            return false;
        }

        tig_str_parse_value_64(&str, &(map_list_info[map_list_info_count].x));
        tig_str_parse_value_64(&str, &(map_list_info[map_list_info_count].y));

        if (str != NULL) {
            int type;
            if (tig_str_match_named_str_to_list(&str, "Type:", off_59F058, MAP_TYPE_COUNT, &type)) {
                dword_5D1214[type] = map_list_info_count + 1;
            }
        }

        map_list_info[map_list_info_count].worldmap = -1;
        if (str != NULL) {
            int worldmap;
            if (tig_str_parse_named_value(&str, "WorldMap:", &worldmap)) {
                map_list_info[map_list_info_count].worldmap = worldmap;
            }
        }

        map_list_info[map_list_info_count].area = 0;
        if (str != NULL) {
            int area;
            if (tig_str_parse_named_value(&str, "Area:", &area)) {
                map_list_info[map_list_info_count].area = area;
            }
        }

        if (++map_list_info_count >= MAP_LIST_CAPACITY) {
            break;
        }

        mes_file_entry.num++;
    }

    mes_unload(dword_5D11D8);
    return true;
}

// 0x4127A0
int map_list_info_find(const char* name)
{
    for (int index = 0; index < map_list_info_count; index++) {
        if (_strcmpi(map_list_info[index].name, name) == 0) {
            return index;
        }
    }

    return -1;
}

// 0x412830
bool map_list_info_set(int index, const char* name, long long x, long long y, bool is_start_map, int worldmap, int area)
{
    strcpy(map_list_info[index].name, name);
    map_list_info[index].x = x;
    map_list_info[index].y = y;
    map_list_info[index].worldmap = worldmap;
    map_list_info[index].area = area;

    if (is_start_map) {
        dword_5D1214[MAP_TYPE_START_MAP] = index + 1;
    } else {
        if (dword_5D1214[MAP_TYPE_START_MAP] == index + 1) {
            dword_5D1214[MAP_TYPE_START_MAP] = 0;
        }
    }

    return true;
}

// 0x412BD0
bool map_list_info_add(const char* name, long long x, long long y, bool is_start_map)
{
    int index;

    index = map_list_info_find(name);
    if (index != -1) {
        return map_list_info_set(index, name, x, y, is_start_map, map_list_info[index].worldmap, map_list_info[index].area);
    }

    index = map_list_info_find(off_59F3DC);
    if (index != -1) {
        return map_list_info_set(index, name, x, y, is_start_map, -1, 0);
    }

    return map_list_info_add_internal(name, x, y, is_start_map);
}

// 0x412C90
bool map_list_info_add_internal(const char* name, long long x, long long y, bool is_start_map)
{
    if (map_list_info_count < MAP_LIST_CAPACITY) {
        return map_list_info_set(map_list_info_count++, name, x, y, is_start_map, -1, 0);
    }

    return false;
}
