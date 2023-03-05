#include "tig/debug.h"

#include <stdarg.h>
#include <stdio.h>
#include <time.h>

#define DEBUG_FUNC_MAX 8

typedef struct BuiltInDebugFuncData {
    const char* name;
    TigDebugFunc* func;
};

static void tig_debug_read_settings();
static void tig_debug_read_registry_settings();
static void tig_debug_console_init();
static void tig_debug_console_impl(const char* string);
static void tig_debug_debugger_impl(const char* string);
static void tig_debug_file_init();
static void tig_debug_file_impl(const char* string);
static void tig_debug_mono_init();
static void tig_debug_mono_impl(const char* string);

// 0x5BE7A0
static BuiltInDebugFuncData built_in_debug_funcs[] = {
    { "console", tig_debug_console_impl },
    { "debugger", tig_debug_debugger_impl },
    { "file", tig_debug_file_impl },
    { "mono", tig_debug_mono_impl },
};

#define BUILT_IN_DEBUG_FUNCS_MAX (sizeof(built_in_debug_funcs) / sizeof(built_in_debug_funcs[0]))

// 0x5BE7C0
static void (*debug_console_init_func)() = tig_debug_console_init;

// 0x5BE7C4
static void (*debug_file_init_func)() = tig_debug_file_init;

// 0x5BE7C8
static void (*debug_mono_init_func)() = tig_debug_mono_init;

// 0x60420C
static FILE* debug_file_handle;

// 0x604210
static bool debug_mono_initialized;

// 0x604614
static bool have_debug_funcs;

// 0x604618
static TigDebugFunc* debug_funcs[DEBUG_FUNC_MAX];

// 0x604638
static char* debug_mono_dest;

// 0x4FEB10
int tig_debug_init(TigContext* ctx)
{
    tig_debug_read_settings();
    sub_4FE790(tig_debug_println);
    tig_debug_println("\n");
    tig_debug_println(sub_51F260(0));

    time_t now;
    time(&now);
    tig_debug_println(asctime(localtime(&now)));

    return 0;
}

// 0x4FEB60
void tig_debug_exit()
{
}

// 0x4FEB70
void tig_debug_printf(const char* format, ...)
{
    // 0x604214
    static char buffer[1024];

    va_list args;
    va_start(args, format);

    if (have_debug_funcs) {
        vsnprintf(buffer, sizeof(buffer), format, args);

        for (int index = 0; index < DEBUG_FUNC_MAX; index++) {
            TigDebugFunc* func = debug_funcs[index];
            if (func != NULL) {
                func(buffer);
            }
        }
    }

    va_end(args);
}

// 0x4FEBC0
void tig_debug_println(const char* string)
{
    if (have_debug_funcs) {
        for (int index = 0; index < DEBUG_FUNC_MAX; index++) {
            TigDebugFunc* func = debug_funcs[index];
            if (func != NULL) {
                func(string);
                func("\n");
            }
        }
    }
}

// 0x4FEC00
void tig_debug_read_settings()
{
    tig_debug_read_registry_settings();

    const char* command_line = GetCommandLineA();
    const char* pch = strstr(command_line, "tigdebug");
    while (pch != NULL) {
        bool enabled = true;
        if (pch > command_line && pch[-1] == '-') {
            enabled = false;
        }

        // Consume "tigdebug".
        pch += 8;

        if (*pch == '=') {
            // Consume "="
            pch++;

            for (int index = 0; index < BUILT_IN_DEBUG_FUNCS_MAX; index++) {
                if (strnicmp(pch, built_in_debug_funcs[index].name, strlen(built_in_debug_funcs[index].name)) == 0) {
                    if (enabled) {
                        tig_debug_add_func(built_in_debug_funcs[index].func);
                    } else {
                        tig_debug_remove_func(built_in_debug_funcs[index].func);
                    }
                }
            }
        }

        pch = strstr(pch, "tigdebug");
    }
}

// 0x4FEC10
void tig_debug_read_registry_settings()
{
    HKEY key;
    if (RegOpenKeyExA(HKEY_LOCAL_MACHINE, "Software\Troika\TIG\Debug Output", 0, KEY_QUERY_VALUE, &key) == ERROR_SUCCESS) {
        for (int index = 0; index < BUILT_IN_DEBUG_FUNCS_MAX; index++) {
            DWORD type;
            DWORD data;
            DWORD cbData;
            if (RegQueryValueExA(key, built_in_debug_funcs[index].name, 0, &type, (LPBYTE)data, &cbData) == ERROR_SUCCESS) {
                if (type == REG_DWORD) {
                    if (data != 0) {
                        tig_debug_add_func(built_in_debug_funcs[index].func);
                    } else {
                        tig_debug_remove_func(built_in_debug_funcs[index].func);
                    }
                }
            }
        }
        RegCloseKey(key);
    }
}

// 0x4FED60
bool tig_debug_add_func(TigDebugFunc* func)
{
    for (int index = 0; index < DEBUG_FUNC_MAX; index++) {
        if (debug_funcs[index] == func) {
            return false;
        }

        if (debug_funcs[index] == NULL) {
            debug_funcs[index] = func;
            have_debug_funcs = true;
            return true;
        }
    }

    return false;
}

// 0x4FEDA0
bool tig_debug_remove_func(TigDebugFunc* func)
{
    int funcs_present = 0;

    for (int index = 0; index < DEBUG_FUNC_MAX; index++) {
        if (debug_funcs[index] != NULL) {
            funcs_present++;
            if (debug_funcs[index] == func) {
                debug_funcs[index] = NULL;

                // FIXME: Looks like a bug, if the function to remove is the
                // first, it thinks there are no other functions.
                if (funcs_present == 1) {
                    have_debug_funcs = false;
                }
                return true;
            }
        }
    }

    return false;
}

// 0x4FEDF0
void tig_debug_console_init()
{
    if (AllocConsole()) {
        SetConsoleTitleA("Debug Output");
    }

    debug_console_init_func = NULL;
}

// 0x4FEE10
void tig_debug_console_impl(const char* string)
{
    if (debug_console_init_func != NULL) {
        debug_console_init_func();
    }

    DWORD chars_written;
    WriteConsoleA(GetStdHandle(STD_OUTPUT_HANDLE), string, strlen(string) + 1, &chars_written, NULL);
}

// 0x4FEE50
void tig_debug_debugger_impl(const char* string)
{
    OutputDebugStringA(string);
}

// 0x4FEE60
void tig_debug_file_init()
{
    if (debug_file_handle == NULL) {
        debug_file_handle = fopen("debug.txt", "w");
    }

    debug_file_init_func = NULL;
}

// 0x4FEE90
void tig_debug_file_impl(const char* string)
{
    if (debug_file_init_func != NULL) {
        debug_file_init_func();
    }

    if (debug_file_handle != NULL) {
        fputs(string, debug_file_handle);
        fflush(debug_file_handle);
    }
}

// 0x4FEEC0
void tig_debug_mono_init()
{
    if (!debug_mono_initialized) {
        memset((void*)0xB0000, 0, 4000);
        debug_mono_dest = (char*)0xB0000;
        debug_mono_initialized = true;
    }

    debug_mono_init_func = NULL;
}

// 0x4FEF00
void tig_debug_mono_impl(const char* string)
{
    if (debug_mono_init_func != NULL) {
        debug_mono_init_func();
    }

    // #todo
}
