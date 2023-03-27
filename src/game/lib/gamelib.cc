#include "game/lib/gamelib.h"

#include <stdio.h>

#include "game/lib/ci.h"
#include "game/lib/gmovie.h"
#include "game/lib/li.h"
#include "game/lib/message.h"
#include "tig/debug.h"
#include "tig/timer.h"
#include "tig/window.h"

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
};

static_assert(sizeof(GameLibModule) == 0x2C, "wrong size");

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

// 0x5CFF08
static char byte_5CFF08[MAX_PATH];

// 0x5D0028
static char byte_5D0028[TEN][MAX_PATH];

// 0x5D0B50
static ViewOptions gamelib_view_options;

// 0x5D0B58
static char byte_5D0B58[MAX_PATH];

// 0x5D0C5C
static char byte_5D0C5C[MAX_PATH];

// 0x5D0E98
static TigRectListNode* dword_5D0E98;

// 0x5D0E9C
static int gamelib_game_difficulty;

// 0x5D0EA0
static bool dword_5D0EA0;

// 0x5D0EA4
static char byte_5D0EA4[MAX_PATH];

// 0x5D0FA8
static char byte_5D0FA8[MAX_PATH];

// 0x5D10B0
static GUID stru_5D10B0;

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

// 0x4024D0
void gamelib_exit()
{
    settings_save(&settings);

    for (int index = MODULE_COUNT - 1; index >= 0; index--) {
        if (gamelib_modules[index].exit_func != NULL) {
            gamelib_modules[index].exit_func();
        }
    }

    message_stats();

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

    if (sub_52E220("Save\\Current")) {
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
    tig_timer_start(&gamelib_ping_time);

    for (int index = 0; index < MODULE_COUNT; index++) {
        if (gamelib_modules[index].ping_func != NULL) {
            gamelib_modules[index].ping_func(gamelib_ping_time);
        }
    }
}

// 0x402C60
void gamelib_mod_unload()
{
    if (dword_5D0EA0) {
        for (int index = MODULE_COUNT - 1; index >= 0; index--) {
            if (gamelib_modules[index].mod_unload_func != NULL) {
                gamelib_modules[index].mod_unload_func();
            }
        }
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

// 0x404570
void gamelib_game_difficulty_changed()
{
    gamelib_game_difficulty = settings_get_value(&settings, "difficulty");
}

// 0x404590
int gamelib_get_game_difficulty()
{
    return gamelib_game_difficulty;
}

void sub_4045A0()
{
    sub_4BBC80();
    sub_4BB920();
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

// 0x404650
const char* gamelib_get_locale()
{
    // 0x5D0D84
    static char locale[GAMELIB_LOCALE_LENGTH];

    int msg_file;
    MessageListItem msg;

    if (message_load("mes\\language.mes", &msg_file)) {
        msg.num = 1;
        if (message_find(msg_file, &msg)) {
            sub_4D43A0(msg_file, &msg);
            strcpy(locale, msg.text);
            message_unload(msg_file);
            return locale;
        }

        message_unload(msg_file);
    }

    return "en";
}

// 0x404810
void gamelib_logo()
{
    gmovie_play_path("movies\\SierraLogo.bik", 0, 0);
    gmovie_play_path("movies\\TroikaLogo.bik", 0, 0);
}

// 0x404830
void gamelib_splash(int window_handle)
{
    settings_add(&settings, SPLASH_KEY, "0", NULL);
    int splash = settings_get_value(&settings, SPLASH_KEY);

    TigVideoBuffer* video_buffer;
    if (tig_window_vbid_get(window_handle, &video_buffer) == TIG_OK) {
        TigFileList file_list;
        tig_file_list_create(&file_list, "art\\splash\\*.bmp");

        if (file_list.count != 0) {
            int value;
            char path[MAX_PATH];

            for (int index = 0; index < 3; index++) {
                value = (index + splash) % file_list.count;
                sprintf(path, "art\\splash\\%s", file_list.entries[value].path);
                if (tig_video_buffer_load_from_bmp(path, video_buffer, 0) == TIG_OK) {
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

// 0x404C10
bool sub_404C10(const char* module_name)
{
    char path1[MAX_PATH];
    char path2[MAX_PATH];

    if (module_name[0] == '\\' || module_name[0] == '.' || module_name == ':') {
        path1[0] = '\0';
    } else {
        strcpy(path1, ".\\Modules\\");
    }

    strcat(path1, module_name);
    size_t end = strlen(path1);

    strcat(path1, ".dat");
    if (tig_file_info(path1, NULL)) {
        if (!tig_file_repository_add(path1)) {
            return false;
        }

        strcpy(byte_5D0FA8, path1);

        path1[end] = '\0';

        for (int index = 0; index < TEN; index++) {
            _snprintf(path2, MAX_PATH, "%s.patch%d", path1, index);
            if (tig_file_info(path2, NULL)) {
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
            if (tig_file_info(path1, NULL)) {
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
        if (sub_52E220(path2)) {
            tig_file_repository_add(path2);
            strcpy(byte_5D0B58, path2);
        }
    }

    if (sub_52E220(path1)) {
        tig_file_repository_add(path1);
        strcpy(byte_5CFF08, path1);
        return true;
    }

    if (sub_52DFE0(path1)) {
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
    tig_file_repository_remove(byte_5CFF08);

    if (byte_5D0B58[0] != '\0') {
        tig_file_repository_remove(byte_5D0B58);
    }

    for (int index = TEN - 1; index >= 0; index--) {
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
