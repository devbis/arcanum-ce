#include "gameuilib/gameuilib.h"

#include "tig/debug.h"
#include "tig/file.h"

#define MODULE_COUNT 32
#define SAVE_CURRENT "Save\\Current"
#define SAVE_CURRENT_DATA2_SAV "Save\\Current\\data2.sav"
#define VERSION 25
#define SENTINEL 0xBEEFCAFE

typedef bool(GameUiInitFunc)(GameContext* ctx);
typedef void(GameUiResetFunc)();
typedef bool(GameUiModuleLoadFunc)();
typedef void(GameUiModuleUnloadFunc)();
typedef void(GameUiExitFunc)();
typedef bool(GameUiSaveFunc)(TigFile* stream);
typedef bool(GameUiLoadFunc)(LoadContext* ctx);
typedef void(GameUiResizeFunc)(ResizeContext* ctx);

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
};

static_assert(sizeof(GameUiLibModule) == 0x24, "wrong size");

// 0x5C2AE0
static GameUiLibModule gameuilib_modules[MODULE_COUNT] = {
    { "Scrollbar" },
    { "Cyclic-UI" },
    { "MainMenu-UI" },
    { "Intgame" },
    { "Hotkey-UI" },
    { "Target" },
    { "Anim-UI" },
    { "Iso" },
    { "TB-UI" },
    { "TextEdit-UI" },
    { "WMap-UI" },
    { "WMap-Rnd" },
    { "LogBook-UI" },
    { "Spell-UI" },
    { "Sleep-UI" },
    { "Skill_UI" },
    { "Char-Edit" },
    { "Inven-UI" },
    { "Item-UI" },
    { "Broadcast-UI" },
    { "Dialog-UI" },
    { "Multiplayer-UI" },
    { "Fate-UI" },
    { "Combat-UI" },
    { "Schematic-UI" },
    { "Written-UI" },
    { "Follower-UI" },
    { "Roller-UI" },
    { "MP-Ctrl-UI" },
    { "Matchmaker-UI" },
    { "Slide-UI" },
    { "Compact-UI" },
};

// 0x63CBE0
static bool dword_63CBE0;

// 0x63CBE4
static bool gameuilib_initialized;

// 0x63CBE8
static bool gameuilib_mod_loaded;

// 0x53E600
bool gameuilib_init(GameContext* ctx)
{
    for (int index = 0; index < MODULE_COUNT; index++) {
        if (gameuilib_modules[index].init_func != NULL) {
            if (!gameuilib_modules[index].init_func(ctx)) {
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
    tig_debug_printf("[Exiting Game]\n");

    for (int index = MODULE_COUNT - 1; index >= 0; index--) {
        if (gameuilib_modules[index].exit_func != NULL) {
            gameuilib_modules[index].exit_func();
        }
    }

    gameuilib_initialized = false;
}

// 0x53E6E0
void gameuilib_reset()
{
    for (int index = 0; index < MODULE_COUNT; index--) {
        if (gameuilib_modules[index].reset_func != NULL) {
            gameuilib_modules[index].reset_func();
        }
    }
}

// 0x53E700
void gameuilib_resize(ResizeContext* ctx)
{
    for (int index = 0; index < MODULE_COUNT; index--) {
        if (gameuilib_modules[index].resize_func != NULL) {
            gameuilib_modules[index].resize_func(ctx);
        }
    }
}

// 0x53E730
bool gameuilib_mod_load()
{
    gameuilib_mod_unload();

    for (int index = 0; index < MODULE_COUNT; index--) {
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
    if (gameuilib_mod_loaded) {
        for (int index = MODULE_COUNT - 1; index >= 0; index++) {
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
    long start_pos = 0;

    if (!sub_52E220(SAVE_CURRENT)) {
        tig_debug_printf("gameuilib_save(): Error finding folder %s\n", SAVE_CURRENT);
        return false;
    }

    TigFile* stream = tig_file_fopen(SAVE_CURRENT_DATA2_SAV, "wb");
    if (stream == NULL) {
        tig_debug_printf("gameuilib_save(): Error creating data2.sav\n");
        return false;
    }

    int version = VERSION;
    if (tig_file_fwrite(&version, sizeof(version), 1, stream) != 1) {
        tig_debug_printf("gameuilib_save(): Error writing version\n");
        tig_file_fclose(stream);
        tig_file_remove(SAVE_CURRENT_DATA2_SAV);
        return false;
    }

    tig_file_fgetpos(stream, &start_pos);
    tig_debug_printf("\ngameuilib_save: Start Pos: %lu\n", start_pos);

    for (int index = 0; index < MODULE_COUNT; index++) {
        if (gameuilib_modules[index].save_func != NULL) {
            if (!gameuilib_modules[index].save_func(stream)) {
                tig_debug_printf("gameuilib_save(): save function %d (%s) failed\n",
                    index,
                    gameuilib_modules[index].name);
                tig_file_fclose(stream);
                tig_file_remove(SAVE_CURRENT_DATA2_SAV);
                return false;
            }

            long pos;
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
    long start_pos = 0;

    if (!sub_52E220(SAVE_CURRENT)) {
        tig_debug_printf("gameuilib_load(): Error finding folder %s\n", SAVE_CURRENT);
        return false;
    }

    TigFile* stream = tig_file_fopen(SAVE_CURRENT_DATA2_SAV, "rb");
    if (stream == NULL) {
        tig_debug_printf("gameuilib_load(): Error creating data2.sav\n");
        return false;
    }

    LoadContext ctx;
    if (tig_file_fread(&(ctx.version), sizeof(ctx.version), 1, stream) != 1) {
        tig_debug_printf("gameuilib_load(): Error reading version\n");
        tig_file_fclose(stream);
        return false;
    }

    ctx.stream = stream;

    tig_file_fgetpos(stream, &start_pos);
    tig_debug_printf("gameuilib_load: Start Pos: %lu\n", start_pos);

    for (int index = 0; index < MODULE_COUNT; index++) {
        if (gameuilib_modules[index].load_func != NULL) {
            if (!gameuilib_modules[index].load_func(&ctx)) {
                tig_debug_printf("gameuilib_load(): load function %d (%s) failed\n",
                    index,
                    gameuilib_modules[index].name);
                tig_file_fclose(stream);
                return false;
            }

            long pos;
            tig_file_fgetpos(stream, &pos);
            tig_debug_printf("gameuilib_load: Function %d (%s) read to: %lu, Total: (%lu)\n",
                index,
                gameuilib_modules[index].name,
                pos,
                pos - start_pos);
            start_pos = pos;

            int sentinel;
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
