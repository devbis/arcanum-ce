#include "game/gamelib.h"

#include <stdio.h>

#include "game/ci.h"
#include "game/facade.h"
#include "game/gmovie.h"
#include "game/li.h"
#include "game/mes.h"
#include "game/sector.h"
#include "game/tb.h"
#include "game/tc.h"
#include "game/tile_block.h"
#include "game/wall.h"

#define GAMELIB_LONG_VERSION_LENGTH 40
#define GAMELIB_SHORT_VERSION_LENGTH 36
#define GAMELIB_LOCALE_LENGTH 4
#define VERSION_MAJOR 1
#define VERSION_MINOR 0
#define VERSION_PATCH 7
#define VERSION_BUILD 4
#define SPLASH_KEY "splash"

#define TEN 10
#define MODULE_COUNT 62

typedef bool(GameInitFunc)(GameContext* ctx);
typedef void(GameResetFunc)();
typedef bool(GameModuleLoadFunc)();
typedef void(GameModuleUnloadFunc)();
typedef void(GameExitFunc)();
typedef void(GamePingFunc)(unsigned int time);
typedef void(GameUpdateViewFunc)(ViewOptions* view_options);
typedef bool(GameSaveFunc)(TigFile* stream);
typedef bool(GameLoadFunc)(LoadContext* ctx);
typedef void(GameResizeFunc)(ResizeContext* ctx);

typedef struct GameLibModule {
    const char* name;
    GameInitFunc* init_func;
    GameResetFunc* reset_func;
    GameModuleLoadFunc* mod_load_func;
    GameModuleUnloadFunc* mod_unload_func;
    GameExitFunc* exit_func;
    GamePingFunc* ping_func;
    GameUpdateViewFunc* update_view_func;
    GameSaveFunc* save_func;
    GameLoadFunc* load_func;
    GameResizeFunc* resize_func;
} GameLibModule;

static_assert(sizeof(GameLibModule) == 0x2C, "wrong size");

typedef struct GameSaveEntry {
    time_t modify_time;
    char* path;
} GameSaveEntry;

static int sub_403D40(const GameSaveEntry* a, const GameSaveEntry* b);
static int sub_403DB0(const GameSaveEntry* a, const GameSaveEntry* b);
static void difficulty_changed();
static void sub_4046F0(void* info);
static void sub_404740(UnknownContext* info);
static void gamelib_logo();
static void gamelib_splash(tig_window_handle_t window_handle);
static void sub_404930();
static void sub_404A20();
static bool sub_404C10(const char* module_name);
static void sub_405070();

// 0x59A330
static GameLibModule gamelib_modules[MODULE_COUNT] = {
    { "Description" },
    { "Item-Effect" },
    { "Teleport" },
    { "Sector" },
    { "Random" },
    { "Critter" },
    { "Name" },
    { "ScriptName" },
    { "Portait" },
    { "AnimFX" },
    { "Script" },
    { "MagicTech" },
    { "MT-AT" },
    { "MT-Item" },
    { "Spell" },
    { "Stat" },
    { "Level" },
    { "Map" },
    { "LightScheme" },
    { "MagicTech-Post" },
    { "Player" },
    { "Area" },
    { "Facade" },
    { "TC" },
    { "Dialog" },
    { "Skill" },
    { "SoundGame" },
    { "Item" },
    { "Combat" },
    { "TimeEvent" },
    { "Rumor" },
    { "Quest" },
    { "Bless" },
    { "Curse" },
    { "AI" },
    { "Broadcast" },
    { "Anim" },
    { "Anim-Private" },
    { "Multiplayer" },
    { "Tech" },
    { "Background" },
    { "Reputation" },
    { "Reaction" },
    { "Tile-Script" },
    { "Sector-Script" },
    { "WP" },
    { "Inven-Source" },
    { "Newspaper" },
    { "TownMap" },
    { "GMovie" },
    { "Brightness" },
    { "GFade" },
    { "Anti-Teleport" },
    { "Trap" },
    { "WallCheck" },
    { "LI" },
    { "CI" },
    { "TileBlock" },
    { "MT-Obj-Node" },
    { "MonsterGen" },
    { "Party" },
    { "gameinit" },
};

// 0x59ADD8
static int dword_59ADD8 = 1;

// 0x59ADDC
static char arcanum1[260] = "Arcanum";

// 0x59AEDC
static char arcanum2[260] = "Arcanum";

// 0x5CFF08
static char byte_5CFF08[MAX_PATH];

// 0x5D000C
static TigRectListNode* dword_5D000C;

// 0x5D0010
static bool dword_5D0010;

// 0x5D0018
static TigRect stru_5D0018;

// 0x5D0028
static char byte_5D0028[TEN][MAX_PATH];

// 0x5D0B50
static ViewOptions gamelib_view_options;

// 0x5D0B58
static char byte_5D0B58[MAX_PATH];

// 0x5D0C5C
static char byte_5D0C5C[MAX_PATH];

// 0x5D0D60
static TigRect stru_5D0D60;

// 0x5D0D70
static bool dword_5D0D70;

// 0x5D0D74
static bool dword_5D0D74;

// 0x5D0D78
static int dword_5D0D78;

// 0x5D0D7C
static int dword_5D0D7C;

// 0x5D0D80
static int dword_5D0D80;

// 0x5D0E88
static GameInitInfo stru_5D0E88;

// 0x5D0E98
static TigRectListNode* dword_5D0E98;

// 0x5D0E9C
static int gamelib_game_difficulty;

// 0x5D0EA0
static bool gamelib_mod_loaded;

// 0x5D0EA4
static char byte_5D0EA4[MAX_PATH];

// 0x5D0FA8
static char byte_5D0FA8[MAX_PATH];

// 0x5D10AC
static void(*dword_5D10AC)(UnknownContext* info);

// 0x5D10B0
static GUID stru_5D10B0;

// 0x5D10C0
static int dword_5D10C0;

// 0x5D10C4
static bool dword_5D10C4;

// 0x5D10D4
static GameExtraSaveFunc* gamelib_extra_save_func;

// 0x5D10D8
static GameExtraLoadFunc* gamelib_extra_load_func;

// 0x5D10DC
static bool dword_5D10DC;

// 0x5D10E0
static bool in_save;

// 0x5D10E4
static bool in_load;

// 0x5D10E8
static bool in_reset;

// 0x5D10EC
static int dword_5D10EC;

// 0x739E60
unsigned int gamelib_ping_time;

// 0x739E70
Settings settings;

// 0x739E7C
TigVideoBuffer* dword_739E7C;

// 0x4020F0
bool gamelib_init(GameInitInfo* init_info)
{
    char version[40];
    TigWindowData window_data;
    TigVideoBufferCreateInfo vb_create_info;
    int index;

    gamelib_copy_version(version, NULL, NULL);
    tig_debug_printf("\n%s\n", version);

    if (init_info->editor) {
        settings_init(&settings, "worlded.cfg");
    } else {
        settings_init(&settings, "arcanum.cfg");
    }

    settings_load(&settings);

    settings_add(&settings, "difficulty", "1", difficulty_changed);
    difficulty_changed();

    gamelib_mod_loaded = false;
    sub_404930();
    sub_404A20();

    if (!init_info->editor) {
        gamelib_logo();
        gamelib_splash(init_info->iso_window_handle);
    }

    init_info->field_8 = sub_402D30;
    init_info->field_C = sub_402E50;

    stru_5D0E88 = *init_info;

    tig_window_data(init_info->iso_window_handle, &window_data);

    dword_5D0D78 = window_data.rect.x;
    dword_5D0D7C = window_data.rect.y;
    dword_5D10C0 = window_data.rect.width / 4;
    dword_5D0D80 = window_data.rect.height / 4;

    stru_5D0018.x = 0;
    stru_5D0018.y = 0;
    stru_5D0018.width = window_data.rect.width;
    stru_5D0018.height = window_data.rect.height;

    stru_5D0D60.x = -256;
    stru_5D0D60.y = -256;
    stru_5D0D60.width = window_data.rect.width + 512;
    stru_5D0D60.height = window_data.rect.height + 512;

    vb_create_info.flags = TIG_VIDEO_BUFFER_CREATE_COLOR_KEY | TIG_VIDEO_BUFFER_CREATE_SYSTEM_MEMORY;
    vb_create_info.width = window_data.rect.width;
    vb_create_info.height = window_data.rect.height;
    vb_create_info.color_key = tig_color_make(0, 255, 0);
    vb_create_info.background_color = vb_create_info.color_key;
    if (tig_video_buffer_create(&vb_create_info, &dword_739E7C) != TIG_OK) {
        return false;
    }

    if (init_info->editor) {
        dword_5D10AC = sub_404740;
    } else {
        dword_5D10AC = sub_4046F0;
    }

    gamelib_view_options.type = VIEW_TYPE_ISOMETRIC;

    for (index = 0; index < MODULE_COUNT; index++) {
        if (gamelib_modules[index].init_func != NULL) {
            if (!gamelib_modules[index].init_func(init_info)) {
                tig_debug_printf("gamelib_init(): init function %d (%s) failed\n",
                    index,
                    gamelib_modules[index].name);

                while (--index >= 0) {
                    if (gamelib_modules[index].exit_func != NULL) {
                        gamelib_modules[index].exit_func();
                    }
                }

                return false;
            }
        }
    }

    return true;
}

// 0x402380
void gamelib_reset()
{
    tig_timestamp_t reset_started_at;
    tig_timestamp_t module_started_at;
    tig_duration_t duration;
    TigRectListNode* node;
    TigRectListNode* next;
    int index;

    tig_debug_printf("gamelib_reset: Resetting.\n");
    tig_timer_now(&reset_started_at);

    in_reset = true;
    strcpy(arcanum1, "Arcanum");
    sub_4D1050();

    if (tig_file_is_directory("Save\\Current")) {
        tig_debug_printf("gamelib_reset: Begin Removing Files...");
        tig_timer_now(&module_started_at);

        if (!sub_52E040("Save\\Current")) {
            tig_debug_printf("gamelib_init(): error emptying folder %s\n", "Save\\Current");
        }

        duration = tig_timer_elapsed(module_started_at);
        tig_debug_printf("done. Time (ms): %d\n", duration);
    }

    node = dword_5D0E98;
    while (node != NULL) {
        next = node->next;
        tig_rect_node_destroy(node);
        node = next;
    }
    dword_5D0E98 = NULL;

    for (index = 0; index < MODULE_COUNT; index++) {
        if (gamelib_modules[index].reset_func != NULL) {
            tig_debug_printf("gamelib_reset: Processing Reset Function: %d", index);
            tig_timer_now(&module_started_at);

            gamelib_modules[index].reset_func();

            duration = tig_timer_elapsed(module_started_at);
            tig_debug_printf(" done. Time (ms): %d.\n", duration);
        }
    }

    sub_4D1040();
    in_reset = false;

    duration = tig_timer_elapsed(reset_started_at);
    tig_debug_printf("gamelib_reset(): Done.  Total time: %f seconds.\n", duration);
}

// 0x4024D0
void gamelib_exit()
{
    settings_save(&settings);

    for (int index = MODULE_COUNT - 1; index >= 0; index--) {
        if (gamelib_modules[index].exit_func != NULL) {
            gamelib_modules[index].exit_func();
        }
    }

    mes_stats();

    TigRectListNode* node = dword_5D0E98;
    while (node != NULL) {
        TigRectListNode* next = node->next;
        tig_rect_node_destroy(node);
        node = next;
    }

    if (dword_739E7C != NULL) {
        tig_video_buffer_destroy(dword_739E7C);
        dword_739E7C = NULL;
    }

    if (tig_file_is_directory("Save\\Current")) {
        if (!sub_52E040("Save\\Current")) {
            // FIXME: Typo in function name, this is definitely not
            // `gamelib_init`.
            tig_debug_printf("gamelib_init(): error emptying folder %s\n", "Save\\Current");
        }
    }

    settings_exit(&settings);
}

// 0x402580
void gamelib_ping()
{
    int index;

    tig_timer_now(&gamelib_ping_time);

    for (index = 0; index < MODULE_COUNT; index++) {
        if (gamelib_modules[index].ping_func != NULL) {
            gamelib_modules[index].ping_func(gamelib_ping_time);
        }
    }
}

// 0x4025C0
void gamelib_resize(ResizeInfo* resize_info)
{
    TigVideoBufferCreateInfo vb_create_info;
    int index;
    TigRect bounds;
    TigRect frame;

    stru_5D0E88.iso_window_handle = resize_info->iso_window_handle;
    stru_5D0018 = resize_info->field_14;

    dword_5D0D78 = resize_info->field_4.x;
    dword_5D0D7C = resize_info->field_4.y;

    stru_5D0D60.x = stru_5D0018.x - 256;
    stru_5D0D60.y = stru_5D0018.y - 256;
    stru_5D0D60.width = stru_5D0018.width + 512;
    stru_5D0D60.height = stru_5D0018.height + 512;

    if (dword_739E7C != NULL) {
        tig_video_buffer_destroy(dword_739E7C);
        dword_739E7C = NULL;
    }

    vb_create_info.flags = TIG_VIDEO_BUFFER_CREATE_COLOR_KEY | TIG_VIDEO_BUFFER_CREATE_SYSTEM_MEMORY;
    vb_create_info.width = stru_5D0018.width;
    vb_create_info.height = stru_5D0018.height;
    vb_create_info.color_key = tig_color_make(0, 255, 0);
    vb_create_info.background_color = vb_create_info.color_key;
    if (tig_video_buffer_create(&vb_create_info, &dword_739E7C) != TIG_OK) {
        tig_debug_printf("gamelib_resize: ERROR: Failed to rebuild scratch buffer!\n");
        return;
    }

    for (index = 0; index < MODULE_COUNT; index++) {
        if (gamelib_modules[index].resize_func != NULL) {
            gamelib_modules[index].resize_func(resize_info);
        }
    }

    if (dword_5D0E98 != NULL) {
        bounds = resize_info->field_4;
        bounds.x = 0;
        bounds.y = 0;
        if (tig_rect_intersection(&(dword_5D0E98->rect), &bounds, &frame) == TIG_OK) {
            dword_5D0E98->rect = frame;
        }
    }
}

// 0x402780
void gamelib_default_module_name_set(const char* name)
{
    strcpy(arcanum2, name);
}

// 0x4027B0
const char* gamelib_default_module_name_get()
{
    return arcanum2;
}

// 0x4027C0
void gamelib_modlist_create(GameModuleList* module_list, int type)
{
    TigFileList file_list;
    unsigned int file_index;
    char* dot;
    bool is_file;
    bool cont;
    bool found;
    unsigned int module_index;
    bool have_mp;
    char path[TIG_MAX_PATH];

    module_list->count = 0;
    module_list->selected = 0;
    module_list->paths = NULL;

    if (type != 0 && type != 1) {
        tig_debug_printf("Invalid type passed into gamelib_modlist_create()\n");
        return;
    }

    tig_file_list_create(&file_list, "Modules\\*.*");

    for (file_index = 0; file_index < file_list.count; file_index++) {
        cont = false;
        is_file = false;
        dot = strrchr(file_list.entries[file_index].path, '.');
        if (dot != NULL) {
            if (strcmpi(dot, ".dat") == 0
                && (file_list.entries[file_index].attributes & TIG_FILE_ATTRIBUTE_SUBDIR) == 0) {
                *dot = '\0';
                is_file = true;
                cont = true;
            }
        } else if ((file_list.entries[file_index].attributes & TIG_FILE_ATTRIBUTE_SUBDIR) != 0) {
            cont = true;
        }

        if (cont) {
            found = false;
            module_index = module_list->count;
            while (module_index > 0) {
                if (strcmpi(file_list.entries[file_index].path, module_list->paths[module_index - 1]) == 0) {
                    found = true;
                    break;
                }
                module_index--;
            }

            if (!found) {
                strcpy(path, "Modules\\");
                strcat(path, file_list.entries[file_index].path);
                if (is_file) {
                    strcat(path, ".dat");
                }

                tig_file_repository_add(path);
                have_mp = tig_file_exists_in_path(path, "mp.txt", NULL);
                tig_file_repository_remove(path);

                if (have_mp && type == 1 || type == 0) {
                    module_list->paths = (char**)REALLOC(module_list->paths, sizeof(module_list->paths) * (module_list->count + 1));
                    module_list->paths[module_list->count] = STRDUP(file_list.entries[file_index].path);

                    if (strcmpi(byte_5D0EA4, file_list.entries[file_index].path) == 0) {
                        module_list->selected = module_list->count;
                    }

                    module_list->count++;
                }
            }
        }
    }

    tig_file_list_destroy(&file_list);
}

// 0x402A10
void gamelib_modlist_destroy(GameModuleList* module_list)
{
    unsigned int index;

    for (index = 0; index < module_list->count; index++) {
        FREE(module_list->paths[index]);
    }
    FREE(module_list->paths);

    module_list->count = 0;
    module_list->selected = 0;
    module_list->paths = NULL;
}

// 0x402A50
bool gamelib_mod_load(const char* path)
{
    TigFileList file_list;
    unsigned int file_index;
    int module_index;

    gamelib_mod_unload();

    if (!sub_404C10(path)) {
        return false;
    }

    if (byte_5D0FA8[0] != '\0') {
        tig_file_repository_guid(byte_5D0FA8, &stru_5D10B0);
    }

    dword_5D10C4 = true;

    if (tig_file_is_directory("Save\\Current")) {
        if (!tig_file_is_empty_directory("Save\\Current")) {
            if (!sub_52E040("Save\\Current")) {
                tig_debug_printf("gamelib_mod_load(): error emptying folder %s\n", "Save\\Current");
                sub_405070();
                return false;
            }
        }
    }

    if (!stru_5D0E88.editor) {
        tig_file_list_create(&file_list, "maps\\*.*");

        for (file_index = 0; file_index < file_list.count; file_index++) {
            if ((file_list.entries[file_index].attributes & TIG_FILE_ATTRIBUTE_SUBDIR) != 0
                && file_list.entries[file_index].path[0] != '.') {
                if (sub_4102F0(file_list.entries[file_index].path)) {
                    tig_debug_printf("gamelib_mod_load(): error preprocessing mobile object data for map %s\n",
                        file_list.entries[file_index].path);
                    sub_405070();
                    return false;
                }
            }
        }

        tig_file_list_destroy(&file_list);
    }

    for (module_index = 0; module_index < MODULE_COUNT; module_index++) {
        if (gamelib_modules[module_index].mod_load_func != NULL) {
            if (!gamelib_modules[module_index].mod_load_func()) {
                tig_debug_printf("gamelib_load(): mod load function %d (%s) failed\n",
                    module_index,
                    gamelib_modules[module_index].name);

                while (--module_index >= 0) {
                    if (gamelib_modules[module_index].mod_unload_func != NULL) {
                        gamelib_modules[module_index].mod_unload_func();
                    }
                }

                sub_405070();

                return false;
            }
        }
    }

    strcpy(byte_5D0EA4, path);
    gamelib_mod_loaded = true;

    return true;
}

// 0x402C20
bool gamelib_mod_guid(GUID* guid_ptr)
{
    if (!dword_5D10C4) {
        return false;
    }

    *guid_ptr = stru_5D10B0;

    return true;
}

// 0x402C60
void gamelib_mod_unload()
{
    int index;

    if (gamelib_mod_loaded) {
        for (index = MODULE_COUNT - 1; index >= 0; index--) {
            if (gamelib_modules[index].mod_unload_func != NULL) {
                gamelib_modules[index].mod_unload_func();
            }
        }
        gamelib_mod_loaded = false;
    }
}

// 0x402CA0
bool gamelib_in_reset()
{
    return in_reset;
}

// 0x402CB0
int sub_402CB0()
{
    return 0;
}

// 0x402CC0
void sub_402CC0()
{
    dword_5D10EC = 0;
}

// 0x402CD0
void gamelib_update_view(ViewOptions* view_options)
{
    for (int index = 0; index < MODULE_COUNT; index++) {
        if (gamelib_modules[index].update_view_func != NULL) {
            gamelib_modules[index].update_view_func(view_options);
        }
    }

    gamelib_view_options = *view_options;

    sub_402D30(NULL);
}

// 0x402D10
void gamelib_get_view_options(ViewOptions* view_options)
{
    *view_options = gamelib_view_options;
}

// 0x402D30
void sub_402D30(TigRect* rect)
{
    TigRect dirty_rect;

    if (rect != NULL) {
        dirty_rect = *rect;

        if (tig_rect_intersection(&dirty_rect, &stru_5D0018, &dirty_rect) != TIG_OK) {
            return;
        }
    } else {
        dirty_rect = stru_5D0018;
    }

    if (dword_5D0D74) {
        if (dword_5D000C != NULL) {
            sub_52D480(dword_5D000C, &dirty_rect);
        } else {
            dword_5D000C = tig_rect_node_create();
            dword_5D000C->rect = dirty_rect;
        }
    } else {
        if (dword_5D0E98 != NULL) {
            sub_52D480(dword_5D0E98, &dirty_rect);
        } else {
            dword_5D0E98 = tig_rect_node_create();
            dword_5D0E98->rect = dirty_rect;
        }

        dword_5D0010 = true;
    }
}

// 0x402E50
bool sub_402E50()
{
    bool ret = false;
    TigRectListNode* node;
    TigRectListNode* next;
    TigRect rect;
    int64_t v1[4];
    int v2[68];
    Sector601808* v3;
    UnknownContext v4;

    if (dword_59ADD8 > 0 && dword_5D0010) {
        dword_5D0D74 = true;

        if (sub_4B9130(&stru_5D0D60, v1)) {
            if (gamelib_view_options.type == VIEW_TYPE_ISOMETRIC) {
                sub_4D0090(v1, v2);
            }

            v3 = sub_4D02E0(v1);
            v4.field_0 = &stru_5D0D60;
            v4.field_4 = v1;
            v4.field_8 = v2;
            v4.field_C = v3;
            v4.rects = &dword_5D0E98;
            dword_5D10AC(&v4);
            sub_4D0400(v3);

            node = dword_5D0E98;
            while (node != NULL) {
                next = node->next;
                rect = node->rect;
                rect.x += dword_5D0D78;
                rect.y += dword_5D0D7C;
                tig_window_set_needs_display_in_rect(&rect);
                tig_rect_node_destroy(node);
                node = next;
            }
            ret = true;
        }

        node = dword_5D000C;
        dword_5D000C = NULL;
        dword_5D0E98 = node;

        if (node == NULL) {
            dword_5D0010 = false;
        }

        dword_5D0D74 = false;
    }

    return ret;
}

// 0x402F90
void sub_402F90()
{
    dword_59ADD8--;
}

// 0x402FA0
void sub_402FA0()
{
    dword_59ADD8++;
}

// 0x402FC0
void sub_402FC0()
{
    tig_window_fill(stru_5D0E88.iso_window_handle, NULL, 0);
    tig_window_display();
    sub_402D30(NULL);
}

// 0x402FE0
char* sub_402FE0()
{
    return arcanum1;
}

// 0x402FF0
void sub_402FF0(const char* name)
{
    if (name != NULL && *name != '\0') {
        strcpy(arcanum1, name);
    }
}

// 0x403030
bool gamelib_save(const char* name, const char* description)
{
    // TODO: Incomplete.
}

// 0x403410
bool gamelib_load(const char* name)
{
    int start_pos = 0;
    int pos;
    tig_timestamp_t start_time;
    tig_timestamp_t time;
    tig_duration_t duration;
    char path[TIG_MAX_PATH];
    TigFile* stream;
    GameLoadInfo load_info;
    int index;
    unsigned int sentinel;

    tig_debug_printf("\ngamelib_load: Loading from File: %s.\n", name);
    tig_timer_now(&start_time);

    in_load = true;

    sub_460870(64);

    if (!tig_file_is_directory("Save\\Current")) {
        tig_debug_printf("gamelib_load(): Error finding folder %s\n", "Save\\Current");
        in_load = false;
        return false;
    }

    sprintf(path, "save\\%s", name);

    tig_debug_printf("gamelib_load: begin removing files...");
    tig_timer_now(&time);
    if (!sub_52E040("Save\\Current")) {
        tig_debug_printf("gamelib_load(): Error clearing folder %s\n", "Save\\Current");
        in_load = false;
        return false;
    }
    duration = tig_timer_elapsed(time);
    tig_debug_printf("done.  Time (ms): %d\n", duration);

    tig_debug_printf("gamelib_load: begin restoring folder archive...");
    tig_timer_now(&time);
    if (!sub_52E550(path, "Save\\Current")) {
        tig_debug_printf("gamelib_load(): error restoring archive %s to save\\test\n", path);
        in_load = false;
        return false;
    }
    duration = tig_timer_elapsed(time);
    tig_debug_printf("done.  Time (ms): %d\n", duration);

    stream = tig_file_fopen("Save\\Current\\data.sav", "rb");
    if (stream == NULL) {
        tig_debug_printf("gamelib_load(): Error reading data.sav\n");
        in_load = false;
        return false;
    }

    if (tig_file_fread(&(load_info.version), sizeof(load_info.version), 1, stream) != 1) {
        tig_debug_printf("gamelib_load(): Error reading version\n");
        in_load = false;
        return false;
    }

    load_info.stream = stream;

    tig_file_fgetpos(stream, &start_pos);
    tig_debug_printf("gamelib_load: Start Pos: %lu\n", start_pos);

    sub_460890(1);

    for (index = 0; index < MODULE_COUNT; index++) {
        if (gamelib_modules[index].load_func != NULL) {
            tig_debug_printf("gamelib_load: Function %d (%s)", index, gamelib_modules[index].name);
            tig_timer_now(&time);

            if (!gamelib_modules[index].load_func(&load_info)) {
                tig_debug_printf("gamelib_load(): load function %d (%s) failed\n", index, gamelib_modules[index].name);
                break;
            }

            duration = tig_timer_elapsed(time);
            tig_file_fgetpos(stream, &pos);
            tig_debug_printf(" read to: %lu, Total: (%lu), Time (ms): %d\n",
                pos,
                pos - start_pos,
                duration);
            start_pos = pos;

            if (tig_file_fread(&sentinel, sizeof(sentinel), 1, load_info.stream) != 1) {
                tig_debug_printf("gamelib_load(): ERROR: Load Sentinel Failed to Load!\n");
                break;
            }

            if (sentinel != 0xBEEFCAFE) {
                tig_debug_printf("gamelib_load(): ERROR: Load Sentinel Failed to Match!\n");
                break;
            }

            sub_460890(index + 1);
        }
    }

    tig_file_fclose(stream);

    if (index < MODULE_COUNT) {
        in_load = false;
        return false;
    }

    if (gamelib_extra_load_func != NULL) {
        tig_debug_printf("gamelib_load: Begin gamelib_extra_load_func()...");
        tig_timer_now(&time);
        if (!gamelib_extra_load_func()) {
            in_load = false;
            return false;
        }
        duration = tig_timer_elapsed(time);
        tig_debug_printf("done. Time (ms): %d\n", duration);
    }

    sub_460890(64);

    in_load = false;

    duration = tig_timer_elapsed(&start_time);
    tig_debug_printf("gamelib_load: Load Complete.  Total time: %f seconds.\n", (float)duration / 1000.0f);

    return true;
}

// 0x403790
bool sub_403790(const char* name)
{
    char path[MAX_PATH];

    if (_strcmpi(name, "SlotAutoAuto-Save") == 0) {
        return false;
    }

    sprintf(path, "save\\%s.gsi", name);
    tig_file_remove(path);

    sprintf(path + 13, ".bmp");
    tig_file_remove(path);

    sprintf(path + 13, ".tfaf");
    tig_file_remove(path);

    sprintf(path + 13, ".tfai");
    tig_file_remove(path);

    return true;
}

// 0x403850
const char* sub_403850()
{
    // 0x5D0D88
    static char byte_5D0D88[256];

    // 0x5D10C8
    static GameSaveList save_list;

    byte_5D0D88[0] = '\0';
    gamelib_savlist_create(&save_list);
    sub_403C10(&save_list, 0, 1);

    if (save_list.count > 0) {
        return strcpy(byte_5D0D88, save_list.paths[0]);
    }

    return byte_5D0D88;
}

// 0x4038C0
bool gamelib_in_save()
{
    return in_save;
}

// 0x4038D0
bool gamelib_in_load()
{
    return in_load;
}

// 0x4038E0
void gamelib_set_extra_save_func(GameExtraSaveFunc* func)
{
    gamelib_extra_save_func = func;
}

// 0x4038F0
void gamelib_set_extra_load_func(GameExtraLoadFunc* func)
{
    gamelib_extra_load_func = func;
}

// 0x403900
void gamelib_savlist_create(GameSaveList* save_list)
{
    TigFileList file_list;
    unsigned int index;
    char fname[_MAX_FNAME];
    char ext[_MAX_EXT];

    save_list->count = 0;
    save_list->paths = NULL;
    save_list->module = NULL;

    tig_file_list_create(&file_list, "save\\slot*.*");

    for (index = 0; index < file_list.count; index++) {
        _splitpath(file_list.entries[index].path, NULL, NULL, fname, ext);
        if (strcmpi(ext, ".gsi") == 0) {
            save_list->paths = (char**)REALLOC(save_list->paths, sizeof(save_list->paths) * (save_list->count + 1));
            save_list->paths[save_list->count++] = STRDUP(fname);
        }
    }

    tig_file_list_destroy(&file_list);
}

// 0x4039E0
void gamelib_modsavlist_create(const char* module, GameSaveList* save_list)
{
    TigFileList file_list;
    unsigned int index;
    char fname[_MAX_FNAME];
    char ext[_MAX_EXT];
    char path[TIG_MAX_PATH];

    save_list->count = 0;
    save_list->paths = NULL;
    save_list->module = STRDUP(module);

    snprintf(path, sizeof(path), ".\\Modules\\%s\\save\\slot*.*", module);
    tig_file_list_create(&file_list, path);

    for (index = 0; index < file_list.count; index++) {
        _splitpath(file_list.entries[index].path, NULL, NULL, fname, ext);
        if (strcmpi(ext, ".gsi") == 0) {
            save_list->paths = (char**)REALLOC(save_list->paths, sizeof(save_list->paths) * (save_list->count + 1));
            save_list->paths[save_list->count++] = STRDUP(fname);
        }
    }

    tig_file_list_destroy(&file_list);
}

// 0x403BB0
void gamelib_savlist_destroy(GameSaveList* save_list)
{
    unsigned int index;

    for (index = 0; index < save_list->count; index++) {
        FREE(save_list->paths[index]);
    }

    if (save_list->paths != NULL) {
        FREE(save_list->paths);
    }

    save_list->count = 0;
    save_list->paths = NULL;

    if (save_list->module != NULL) {
        FREE(save_list->module);
    }

    save_list->module = NULL;
}

// 0x403C10
void sub_403C10(GameSaveList* save_list, int a2, int a3)
{
    GameSaveEntry* entries;
    unsigned int index;
    char path[TIG_MAX_PATH];
    TigFileInfo file_info;

    dword_5D10DC = a3 == 0;

    entries = (GameSaveEntry*)MALLOC(sizeof(*entries) * save_list->count);

    for (index = 0; index < save_list->count; index++) {
        entries[index].path = save_list->paths[index];
        if (save_list->module != NULL) {
            snprintf(path, sizeof(path),
                ".\\Modules\\%s\\save\\%s.gsi",
                save_list->module,
                save_list->paths[index]);
        } else {
            snprintf(path, sizeof(path),
                "save\\%s.gsi",
                save_list->paths[index]);
        }

        if (!tig_file_exists(path, &file_info)) {
            tig_debug_printf("GameLib: : ERROR: Couldn't find file!\n");
            // FIXME: Leaks `entries`.
            return;
        }

        entries[index].modify_time = file_info.modify_time;
    }

    if (a2 == 1) {
        qsort(entries, save_list->count, sizeof(*entries), sub_403DB0);
    } else {
        qsort(entries, save_list->count, sizeof(*entries), sub_403D40);
    }

    for (index = 0; index < save_list->count; index++) {
        save_list->paths[index] = entries[index].path;
    }

    FREE(entries);
}

// 0x403D40
int sub_403D40(const GameSaveEntry* a, const GameSaveEntry* b)
{
    if (dword_5D10DC) {
        if (toupper(a->path[4]) == 'A') {
            return -1;
        }

        if (toupper(b->path[4]) == 'A') {
            return 1;
        }
    }

    if (a->modify_time < b->modify_time) {
        return 1;
    } else if (a->modify_time > b->modify_time) {
        return -1;
    } else {
        return 0;
    }
}

// 0x403DB0
int sub_403DB0(const GameSaveEntry* a, const GameSaveEntry* b)
{
    return -strnicmp(a->path, b->path, 8);
}

// 0x404570
void difficulty_changed()
{
    gamelib_game_difficulty = settings_get_value(&settings, "difficulty");
}

// 0x404590
int gamelib_get_game_difficulty()
{
    return gamelib_game_difficulty;
}

// 0x4045A0
void sub_4045A0()
{
    li_redraw();
    ci_redraw();
    sub_402D30(NULL);
    sub_402E50();
    tig_window_set_needs_display_in_rect(NULL);
}

// 0x4045D0
bool gamelib_copy_version(char* long_version, char* short_version, char* locale)
{
    if (long_version != NULL) {
        _snprintf(long_version,
            GAMELIB_LONG_VERSION_LENGTH - 1,
            "Arcanum %d.%d.%d.%d %s",
            VERSION_MAJOR,
            VERSION_MINOR,
            VERSION_PATCH,
            VERSION_BUILD,
            gamelib_get_locale());
    }

    if (short_version != NULL) {
        _snprintf(short_version,
            GAMELIB_SHORT_VERSION_LENGTH - 1,
            "%d.%d.%d.%d",
            VERSION_MAJOR,
            VERSION_MINOR,
            VERSION_PATCH,
            VERSION_BUILD);
    }

    if (locale != NULL) {
        strncpy(locale, gamelib_get_locale(), GAMELIB_LOCALE_LENGTH - 1);
    }
}

// 0x404640
void sub_404640()
{
}

// 0x404650
const char* gamelib_get_locale()
{
    // 0x5D0D84
    static char locale[GAMELIB_LOCALE_LENGTH];

    mes_file_handle_t mes_file;
    MesFileEntry mes_file_entry;

    if (mes_load("mes\\language.mes", &mes_file)) {
        mes_file_entry.num = 1;
        if (mes_search(mes_file, &mes_file_entry)) {
            mes_get_msg(mes_file, &mes_file_entry);
            strcpy(locale, mes_file_entry.str);
            mes_unload(mes_file);
            return locale;
        }

        mes_unload(mes_file);
    }

    return "en";
}

// 0x4046F0
void sub_4046F0(void* info)
{
    if (tig_video_3d_begin_scene() == TIG_OK) {
        sub_4D8350(info);
        sub_4D6950(info);
        sub_43C690(info);
        sub_43B390(info);
        sub_439140(info);
        sub_4D5F10(info);
        sub_4D5310(info);
        sub_4C95F0(info);
        tig_video_3d_end_scene();
    }
}

// 0x404740
void sub_404740(UnknownContext* info)
{
    TigRectListNode* node;
    tig_color_t color;

    color = tig_color_make(0, 0, 255);
    node = *info->rects;
    while (node != NULL) {
        tig_window_fill(stru_5D0E88.iso_window_handle, &(node->rect), color);
        node = node->next;
    }

    if (tig_video_3d_begin_scene() == TIG_OK) {
        sub_4D8350(info);
        sub_4D6950(info);
        sub_4C9E70(info);
        sub_4E3320(info);
        sub_4C0630(info);
        sub_4BB0C0(info);
        sub_43B390(info);
        sub_4CF390(info);
        sub_4DF500(info);
        sub_4BFE00(info);
        sub_439140(info);
        sub_4D5F10(info);
        tig_video_3d_end_scene();
    }
}

// 0x404810
void gamelib_logo()
{
    gmovie_play_path("movies\\SierraLogo.bik", 0, 0);
    gmovie_play_path("movies\\TroikaLogo.bik", 0, 0);
}

// 0x404830
void gamelib_splash(tig_window_handle_t window_handle)
{
    int splash;
    TigVideoBuffer* video_buffer;
    TigFileList file_list;
    int value;
    char path[TIG_MAX_PATH];
    int index;

    settings_add(&settings, SPLASH_KEY, "0", NULL);
    splash = settings_get_value(&settings, SPLASH_KEY);

    if (tig_window_vbid_get(window_handle, &video_buffer) == TIG_OK) {
        tig_file_list_create(&file_list, "art\\splash\\*.bmp");

        if (file_list.count != 0) {
            for (index = 0; index < 3; index++) {
                value = (index + splash) % file_list.count;
                sprintf(path, "art\\splash\\%s", file_list.entries[value].path);
                if (tig_video_buffer_load_from_bmp(path, &video_buffer, 0) == TIG_OK) {
                    break;
                }
            }

            tig_file_list_destroy(&file_list);
            tig_window_set_needs_display_in_rect(NULL);
            tig_window_display();
            settings_set_value(&settings, SPLASH_KEY, value + 1);
        } else {
            tig_file_list_destroy(&file_list);
        }
    }
}

// 0x404930
void sub_404930()
{
    char data[260];
    DWORD dwSize = sizeof(data);
    DWORD dwType;
    HKEY hKey;

    dword_5D0D70 = true;

    if (RegOpenKeyExA(HKEY_LOCAL_MACHINE, "SOFTWARE\\Troika\\Arcanum", 0, KEY_READ, &hKey) == ERROR_SUCCESS) {
        dwType = REG_SZ;
        if (RegQueryValueExA(hKey, "installed_mode", NULL, &dwType, (LPBYTE)data, &dwSize) == ERROR_SUCCESS
            && strcmpi(data, "partial") == 0) {
            dword_5D0D70 = false;
        } else {
            // NOTE: Purpose? (We already know it's true)
            if (dword_5D0D70 == true) {
                return;
            }
        }

        dwType = REG_SZ;
        if (RegQueryValueExA(hKey, "installed_from", NULL, &dwType, (LPBYTE)data, &dwSize) == ERROR_SUCCESS) {
            strcpy(byte_5D0C5C, data);
        } else {
            dword_5D0D70 = true;
        }

        // FIXME: Missing RegCloseKey.
    }
}

// 0x404A20
void sub_404A20()
{
    TigFileList file_list;
    TigFileList remote_file_list;
    char path[TIG_MAX_PATH];
    unsigned int index;
    unsigned int remote_index;
    bool found;

    tig_file_list_create(&file_list, "arcanum*.dat");

    if (!dword_5D0D70) {
        strcpy(path, byte_5D0C5C);
        strcat(path, "\\Sierra\\Arcanum\\arcanum*.dat");
        tig_file_list_create(&remote_file_list, path);

        for (remote_index = 0; remote_index < remote_file_list.count; remote_index++) {
            found = false;
            for (index = 0; index < file_list.count; index++) {
                if (strcmpi(remote_file_list.entries[remote_index].path, file_list.entries[index].path) == 0) {
                    found = true;
                    break;
                }
            }
            if (!found) {
                strcpy(path, byte_5D0C5C);
                strcat(path, "\\Sierra\\Arcanum\\");
                strcat(path, remote_file_list.entries[remote_index].path);
                tig_file_repository_add(path);
            }
        }

        tig_file_list_destroy(&remote_file_list);
    }

    for (index = 0; index < file_list.count; index++) {
        tig_file_repository_add(file_list.entries[index].path);
    }

    tig_file_list_destroy(&file_list);

    tig_file_mkdir("data");
    tig_file_repository_add("data");
}

// 0x404C10
bool sub_404C10(const char* module_name)
{
    char path1[MAX_PATH];
    char path2[MAX_PATH];
    size_t end;
    int index;

    if (module_name[0] == '\\' || module_name[0] == '.' || module_name[1] == ':') {
        path1[0] = '\0';
    } else {
        strcpy(path1, ".\\Modules\\");
    }

    strcat(path1, module_name);
    end = strlen(path1);

    strcat(path1, ".dat");
    if (tig_file_exists(path1, NULL)) {
        if (!tig_file_repository_add(path1)) {
            return false;
        }

        strcpy(byte_5D0FA8, path1);

        path1[end] = '\0';

        for (index = 0; index < TEN; index++) {
            _snprintf(path2, MAX_PATH, "%s.patch%d", path1, index);
            if (tig_file_exists(path2, NULL)) {
                if (!tig_file_repository_add(path2)) {
                    return false;
                }

                strcpy(byte_5D0028[index], path2);
            }
        }
    } else {
        if (!dword_5D0D70) {
            strcpy(path2, byte_5D0C5C);
            strcat(path2, "\\Sierra\\Arcanum\\Modules\\");
            strcat(path2, module_name);
            strcat(path2, ".dat");

            // FIXME: Probably broken: previous code builds `path2`, but
            // attempts to load `path1`.
            if (tig_file_exists(path1, NULL)) {
                if (!tig_file_repository_add(path1)) {
                    return false;
                }

                strcpy(byte_5D0FA8, path2);
            }
        }
    }

    path1[end] = '\0';

    if (!dword_5D0D70) {
        strcpy(path2, byte_5D0C5C);
        strcat(path2, "\\Sierra\\Arcanum\\Modules\\");
        strcat(path2, module_name);
        if (tig_file_is_directory(path2)) {
            tig_file_repository_add(path2);
            strcpy(byte_5D0B58, path2);
        }
    }

    if (tig_file_is_directory(path1)) {
        tig_file_repository_add(path1);
        strcpy(byte_5CFF08, path1);
        return true;
    }

    if (tig_file_mkdir(path1)) {
        if (stru_5D0E88.editor == 1) {
            if (byte_5D0FA8[0] == '\0') {
                sub_52E260(path1, "Module template");
            }
        }

        tig_file_repository_add(path1);
        strcpy(byte_5CFF08, path1);
        return true;
    }

    tig_debug_printf("gamelib_mod_load(): error creating folder %s\n", path1);

    if (byte_5D0FA8[0] != '\0') {
        tig_file_repository_remove(byte_5D0FA8);
    }

    return false;
}

// 0x405070
void sub_405070()
{
    int index;

    tig_file_repository_remove(byte_5CFF08);

    if (byte_5D0B58[0] != '\0') {
        tig_file_repository_remove(byte_5D0B58);
    }

    for (index = TEN - 1; index >= 0; index--) {
        if (byte_5D0028[index][0] != '\0') {
            tig_file_repository_remove(byte_5D0028[index]);
            byte_5D0028[index][0] = '\0';
        }
    }

    if (byte_5D0FA8[0] != '\0') {
        tig_file_repository_remove(byte_5D0FA8);
    }

    byte_5CFF08[0] = '\0';
    byte_5D0B58[0] = '\0';
    byte_5D0FA8[0] = '\0';
    byte_5D0EA4[0] = '\0';

    memset(&stru_5D10B0, 0, sizeof(stru_5D10B0));

    dword_5D10C4 = 0;
}
