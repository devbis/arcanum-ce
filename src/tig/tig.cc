#include "tig/tig.h"

#include "tig/debug.h"
#include "tig/memory.h"
#include "tig/timer.h"

typedef int(TigInitFunc)(TigContext* ctx);
typedef void(TigExitFunc)();

static void tig_init_executable_file_path();

// 0x5BF2D4
static TigInitFunc* init_funcs[] = {
    tig_memory_init,
    tig_debug_init,
    tig_timer_init,
};

#define TIG_INIT_FUNC_MAX (sizeof(init_funcs) / sizeof(init_funcs[0]))

// 0x5BF330
static TigExitFunc* exit_funcs[] = {
    tig_memory_exit,
    tig_debug_exit,
    tig_timer_exit,
};

#define TIG_INIT_EXIT_FUNC_MAX (sizeof(exit_funcs) / sizeof(exit_funcs[0]))

static_assert(TIG_INIT_FUNC_MAX == TIG_INIT_EXIT_FUNC_MAX, "number of init and exit funcs does not match");

// 0x60F134
static char tig_executable_path[MAX_PATH];

// 0x60F238
static char* tig_executable_file_name;

// 0x60F23C
static bool tig_initialized;

// 0x60F240
static bool in_tig_exit;

// 0x60F244
static bool tig_active;

// 0x739F34
unsigned int tig_ping_time;

// 0x51F130
int tig_init(TigContext* ctx)
{
    if (tig_initialized) {
        return 2;
    }

    for (int index = 0; index < TIG_INIT_FUNC_MAX; index++) {
        if ((ctx->flags & 0x2000) != 0 && init_funcs[index] == tig_sound_init) {
            continue;
        }

        int rc = init_funcs[index](ctx);
        if (rc != TIG_OK) {
            tig_debug_printf("Error initializing TIG init_func %d", index);
            while (index-- >= 0) {
                exit_funcs[index]();
            }
            return rc;
        }
    }

    atexit(tig_exit);
    tig_init_executable_file_path();

    tig_initialized = true;

    return TIG_OK;
}

// 0x51F1D0
void tig_exit()
{
    if (tig_initialized) {
        if (!in_tig_exit) {
            in_tig_exit = true;

            for (int index = TIG_INIT_EXIT_FUNC_MAX; index > 0; index--) {
                exit_funcs[index - 1]();
            }

            tig_initialized = false;

            in_tig_exit = false;
        }
    }
}

// 0x51F220
void tig_ping()
{
    tig_timer_start(&tig_ping_time);
    tig_mouse_ping();
    tig_kb_ping();
    tig_message_ping();
    tig_net_ping();
    tig_sound_ping();
    tig_art_ping();
}

// NOTE: Purpose is unclear, both this function and `tig_ping` are public.
//
// 0x51F250
void sub_51F250()
{
    tig_ping();
}

// 0x51F260
const char* tig_get_executable(bool file_name_only)
{
    if (!tig_initialized) {
        tig_init_executable_file_path();
    }

    if (file_name_only) {
        return tig_executable_file_name;
    }

    return tig_executable_path;
}

// 0x51F290
void tig_init_executable_file_path()
{
    if (GetModuleFileNameA(NULL, tig_executable_path, sizeof(tig_executable_path)) == 0) {
        tig_debug_println("GetModuleFileName failed.");
        strcpy(tig_executable_path, "<unknown>");
    }

    char* pch = strrchr(tig_executable_path, '\\');
    if (pch != NULL) {
        tig_executable_file_name = pch + 1;
    } else {
        tig_executable_file_name = tig_executable_path;
    }
}

// 0x51F300
void tig_set_active(bool is_active)
{
    tig_active = is_active;
    tig_mouse_set_active(is_active);
    tig_sound_set_active(is_active);
}

// 0x51F320
int tig_get_active_()
{
    return tig_active;
}
