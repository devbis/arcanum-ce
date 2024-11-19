#include "ui/gameuilib.h"

#include "game/gamelib.h"
#include "game/target.h"
#include "ui/anim_ui.h"
#include "ui/broadcast_ui.h"
#include "ui/charedit_ui.h"
#include "ui/combat_ui.h"
#include "ui/compact_ui.h"
#include "ui/cyclic_ui.h"
#include "ui/dialog_ui.h"
#include "ui/fate_ui.h"
#include "ui/follower_ui.h"
#include "ui/hotkey_ui.h"
#include "ui/intgame.h"
#include "ui/inven_ui.h"
#include "ui/iso.h"
#include "ui/item_ui.h"
#include "ui/logbook_ui.h"
#include "ui/mainmenu_ui.h"
#include "ui/matchmaker_ui.h"
#include "ui/mp_ctrl_ui.h"
#include "ui/multiplayer_ui.h"
#include "ui/roller_ui.h"
#include "ui/schematic_ui.h"
#include "ui/scrollbar_ui.h"
#include "ui/skill_ui.h"
#include "ui/sleep_ui.h"
#include "ui/slide_ui.h"
#include "ui/spell_ui.h"
#include "ui/tb_ui.h"
#include "ui/textedit_ui.h"
#include "ui/wmap_rnd.h"
#include "ui/wmap_ui.h"
#include "ui/written_ui.h"

#define MODULE_COUNT 32
#define SAVE_CURRENT "Save\\Current"
#define SAVE_CURRENT_DATA2_SAV "Save\\Current\\data2.sav"
#define VERSION 25
#define SENTINEL 0xBEEFCAFE

typedef bool(GameUiInitFunc)(GameInitInfo* init_info);
typedef void(GameUiResetFunc)();
typedef bool(GameUiModuleLoadFunc)();
typedef void(GameUiModuleUnloadFunc)();
typedef void(GameUiExitFunc)();
typedef bool(GameUiSaveFunc)(TigFile* stream);
typedef bool(GameUiLoadFunc)(GameLoadInfo* load_info);
typedef void(GameUiResizeFunc)(ResizeInfo* resize_info);

typedef struct GameUiLibModule {
    const char* name;
    GameUiInitFunc* init_func;
    GameUiResetFunc* reset_func;
    GameUiModuleLoadFunc* mod_load_func;
    GameUiModuleUnloadFunc* mod_unload_func;
    GameUiExitFunc* exit_func;
    GameUiSaveFunc* save_func;
    GameUiLoadFunc* load_func;
    GameUiResizeFunc* resize_func;
} GameUiLibModule;

static_assert(sizeof(GameUiLibModule) == 0x24, "wrong size");

// 0x5C2AE0
static GameUiLibModule gameuilib_modules[MODULE_COUNT] = {
    { "Scrollbar", scrollbar_ui_init, scrollbar_ui_reset, NULL, NULL, scrollbar_ui_exit, NULL, NULL, NULL },
    { "Cyclic-UI", cyclic_ui_init, NULL, NULL, NULL, cyclic_ui_exit, NULL, NULL, NULL },
    { "MainMenu-UI", mainmenu_ui_init, NULL, NULL, NULL, mainmenu_ui_exit, NULL, NULL, NULL },
    { "Intgame", intgame_init, intgame_reset, NULL, NULL, intgame_exit, intgame_save, intgame_load, intgame_resize },
    { "Hotkey-UI", hotkey_ui_init, NULL, NULL, NULL, hotkey_ui_exit, NULL, NULL, hotkey_ui_resize },
    { "Target", target_init, target_reset, NULL, NULL, target_exit, NULL, NULL, target_resize },
    { "Anim-UI", anim_ui_init, anim_ui_reset, NULL, NULL, anim_ui_exit, anim_ui_save, anim_ui_load, NULL },
    { "Iso", iso_init, iso_reset, NULL, NULL, iso_exit, NULL, NULL, iso_resize },
    { "TB-UI", tb_ui_init, tb_ui_reset, NULL, NULL, tb_ui_exit, NULL, NULL, NULL },
    { "TextEdit-UI", textedit_ui_init, textedit_ui_reset, NULL, NULL, textedit_ui_exit, NULL, NULL, NULL },
    { "WMap-UI", wmap_ui_init, wmap_ui_reset, NULL, NULL, wmap_ui_exit, wmap_ui_save, wmap_ui_load, NULL },
    { "WMap-Rnd", wmap_rnd_init, wmap_rnd_reset, wmap_rnd_mod_load, wmap_rnd_mod_unload, wmap_rnd_exit, wmap_rnd_save, wmap_rnd_load, NULL },
    { "LogBook-UI", logbook_ui_init, logbook_ui_reset, NULL, NULL, logbook_ui_exit, NULL, NULL, NULL },
    { "Spell-UI", spell_ui_init, spell_ui_reset, NULL, NULL, spell_ui_exit, spell_ui_save, spell_ui_load, NULL },
    { "Sleep-UI", sleep_ui_init, sleep_ui_reset, NULL, NULL, sleep_ui_exit, NULL, NULL, NULL },
    { "Skill_UI", skill_ui_init, skill_ui_reset, NULL, NULL, skill_ui_exit, NULL, NULL, NULL },
    { "Char-Edit", charedit_init, charedit_reset, NULL, NULL, charedit_exit, NULL, NULL, NULL },
    { "Inven-UI", inven_ui_init, inven_ui_reset, NULL, NULL, inven_ui_exit, NULL, NULL, NULL },
    { "Item-UI", item_ui_init, NULL, NULL, NULL, item_ui_exit, NULL, NULL, NULL },
    { "Broadcast-UI", broadcast_ui_init, broadcast_ui_reset, NULL, NULL, broadcast_ui_exit, NULL, NULL, NULL },
    { "Dialog-UI", dialog_ui_init, dialog_ui_reset, NULL, NULL, dialog_ui_exit, NULL, NULL, NULL },
    { "Multiplayer-UI", multiplayer_ui_init, mutliplayer_ui_reset, NULL, NULL, multiplayer_ui_exit, NULL, NULL, NULL },
    { "Fate-UI", fate_ui_init, fate_ui_reset, NULL, NULL, fate_ui_exit, NULL, NULL, NULL },
    { "Combat-UI", combat_ui_init, combat_ui_reset, NULL, NULL, combat_ui_exit, NULL, NULL, combat_ui_resize },
    { "Schematic-UI", schematic_ui_init, schematic_ui_reset, NULL, NULL, schematic_ui_exit, NULL, NULL, NULL },
    { "Written-UI", NULL, NULL, written_ui_mod_load, written_ui_mod_unload, NULL, NULL, NULL, NULL },
    { "Follower-UI", follower_ui_init, follower_ui_reset, NULL, NULL, follower_ui_exit, follower_ui_save, follower_ui_load, follower_ui_resize },
    { "Roller-UI", roller_ui_init, NULL, NULL, NULL, roller_ui_exit, NULL, NULL, NULL },
    { "MP-Ctrl-UI", mp_ctrl_ui_init, NULL, NULL, NULL, mp_ctrl_ui_exit, NULL, NULL, NULL },
    { "Matchmaker-UI", matchmaker_ui_init, matchmaker_ui_reset, NULL, NULL, matchmaker_ui_exit, NULL, NULL, NULL },
    { "Slide-UI", NULL, NULL, slide_ui_mod_load, slide_ui_mod_unload, NULL, NULL, NULL, NULL },
    { "Compact-UI", compact_ui_init, compact_ui_reset, NULL, NULL, compact_ui_exit, NULL, NULL, NULL },
};

// 0x63CBE0
static bool dword_63CBE0;

// 0x63CBE4
static bool gameuilib_initialized;

// 0x63CBE8
static bool gameuilib_mod_loaded;

// 0x53E600
bool gameuilib_init(GameInitInfo* init_info)
{
    int index;

    for (index = 0; index < MODULE_COUNT; index++) {
        if (gameuilib_modules[index].init_func != NULL) {
            if (!gameuilib_modules[index].init_func(init_info)) {
                tig_debug_printf("gameuilib_init(): init function %d (%s) failed\n", index, gameuilib_modules[index].name);

                while (--index >= 0) {
                    if (gameuilib_modules[index].exit_func != NULL) {
                        gameuilib_modules[index].exit_func();
                    }
                }

                return false;
            }
        }
    }

    gameuilib_initialized = true;

    gamelib_set_extra_save_func(gameuilib_save);
    gamelib_set_extra_load_func(gameuilib_load);

    return true;
}

// 0x53E6A0
void gameuilib_exit()
{
    int index;

    tig_debug_printf("[Exiting Game]\n");

    for (index = MODULE_COUNT - 1; index >= 0; index--) {
        if (gameuilib_modules[index].exit_func != NULL) {
            gameuilib_modules[index].exit_func();
        }
    }

    gameuilib_initialized = false;
}

// 0x53E6E0
void gameuilib_reset()
{
    int index;

    for (index = 0; index < MODULE_COUNT; index--) {
        if (gameuilib_modules[index].reset_func != NULL) {
            gameuilib_modules[index].reset_func();
        }
    }
}

// 0x53E700
void gameuilib_resize(ResizeContext* resize_info)
{
    int index;

    for (index = 0; index < MODULE_COUNT; index++) {
        if (gameuilib_modules[index].resize_func != NULL) {
            gameuilib_modules[index].resize_func(resize_info);
        }
    }
}

// 0x53E730
bool gameuilib_mod_load()
{
    int index;

    gameuilib_mod_unload();

    for (index = 0; index < MODULE_COUNT; index++) {
        if (gameuilib_modules[index].mod_load_func != NULL) {
            if (!gameuilib_modules[index].mod_load_func()) {
                while (--index >= 0) {
                    if (gameuilib_modules[index].mod_unload_func != NULL) {
                        gameuilib_modules[index].mod_unload_func();
                    }
                }

                return false;
            }
        }
    }

    gameuilib_mod_loaded = true;

    return true;
}

// 0x53E790
void gameuilib_mod_unload()
{
    int index;

    if (gameuilib_mod_loaded) {
        for (index = MODULE_COUNT - 1; index >= 0; index--) {
            if (gameuilib_modules[index].mod_unload_func != NULL) {
                gameuilib_modules[index].mod_unload_func();
            }
        }

        gameuilib_mod_loaded = false;
    }
}

// 0x53E7C0
bool gameuilib_save()
{
    int sentinel = SENTINEL;
    int start_pos = 0;
    TigFile* stream;
    int version;
    int index;
    int pos;

    if (!tig_file_is_directory(SAVE_CURRENT)) {
        tig_debug_printf("gameuilib_save(): Error finding folder %s\n", SAVE_CURRENT);
        return false;
    }

    stream = tig_file_fopen(SAVE_CURRENT_DATA2_SAV, "wb");
    if (stream == NULL) {
        tig_debug_printf("gameuilib_save(): Error creating data2.sav\n");
        return false;
    }

    version = VERSION;
    if (tig_file_fwrite(&version, sizeof(version), 1, stream) != 1) {
        tig_debug_printf("gameuilib_save(): Error writing version\n");
        tig_file_fclose(stream);
        tig_file_remove(SAVE_CURRENT_DATA2_SAV);
        return false;
    }

    tig_file_fgetpos(stream, &start_pos);
    tig_debug_printf("\ngameuilib_save: Start Pos: %lu\n", start_pos);

    for (index = 0; index < MODULE_COUNT; index++) {
        if (gameuilib_modules[index].save_func != NULL) {
            if (!gameuilib_modules[index].save_func(stream)) {
                tig_debug_printf("gameuilib_save(): save function %d (%s) failed\n",
                    index,
                    gameuilib_modules[index].name);
                tig_file_fclose(stream);
                tig_file_remove(SAVE_CURRENT_DATA2_SAV);
                return false;
            }

            tig_file_fgetpos(stream, &pos);
            tig_debug_printf("gameuilib_save: Function %d (%s) wrote to: %lu, Total: (%lu)\n",
                index,
                gameuilib_modules[index].name,
                pos,
                pos - start_pos);
            start_pos = pos;

            if (tig_file_fwrite(&sentinel, sizeof(sentinel), 1, stream) != 1) {
                tig_debug_printf("gameuilib_save(): ERROR: Sentinel Failed to Save!\n");
                tig_file_fclose(stream);
                tig_file_remove(SAVE_CURRENT_DATA2_SAV);
                return false;
            }
        }
    }

    return true;
}

// 0x53E950
bool gameuilib_load()
{
    int start_pos = 0;
    TigFile* stream;
    GameLoadInfo load_info;
    int index;
    int pos;
    int sentinel;

    if (!tig_file_is_directory(SAVE_CURRENT)) {
        tig_debug_printf("gameuilib_load(): Error finding folder %s\n", SAVE_CURRENT);
        return false;
    }

    stream = tig_file_fopen(SAVE_CURRENT_DATA2_SAV, "rb");
    if (stream == NULL) {
        tig_debug_printf("gameuilib_load(): Error creating data2.sav\n");
        return false;
    }

    if (tig_file_fread(&(load_info.version), sizeof(load_info.version), 1, stream) != 1) {
        tig_debug_printf("gameuilib_load(): Error reading version\n");
        tig_file_fclose(stream);
        return false;
    }

    load_info.stream = stream;

    tig_file_fgetpos(stream, &start_pos);
    tig_debug_printf("gameuilib_load: Start Pos: %lu\n", start_pos);

    for (index = 0; index < MODULE_COUNT; index++) {
        if (gameuilib_modules[index].load_func != NULL) {
            if (!gameuilib_modules[index].load_func(&load_info)) {
                tig_debug_printf("gameuilib_load(): load function %d (%s) failed\n",
                    index,
                    gameuilib_modules[index].name);
                tig_file_fclose(stream);
                return false;
            }

            tig_file_fgetpos(stream, &pos);
            tig_debug_printf("gameuilib_load: Function %d (%s) read to: %lu, Total: (%lu)\n",
                index,
                gameuilib_modules[index].name,
                pos,
                pos - start_pos);
            start_pos = pos;

            if (tig_file_fread(&sentinel, sizeof(sentinel), 1, stream) != 1) {
                tig_debug_printf("gameuilib_load(): ERROR: Load Sentinel Failed to Load!");
                tig_file_fclose(stream);
                return false;
            }

            if (sentinel != SENTINEL) {
                tig_debug_printf("gameuilib_load(): ERROR: Load Sentinel Failed to Match!");
                tig_file_fclose(stream);
                return false;
            }
        }
    }

    return true;
}

// 0x53EAD0
bool sub_53EAD0()
{
    if (!dword_63CBE0) {
        return false;
    }

    dword_63CBE0 = false;
    return true;
}

// 0x53EAF0
void sub_53EAF0()
{
    dword_63CBE0 = true;
}

// 0x53EB00
void sub_53EB00()
{
    dword_63CBE0 = false;
}
