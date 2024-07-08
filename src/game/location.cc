#include "game/lib/location.h"

#include "tig/debug.h"
#include "tig/rect.h"
#include "tig/window.h"

// 0x5FC278
static TigRect stru_5FC278;

// 0x5FC288
static int64_t qword_5FC288;

// 0x5FC290
static int64_t qword_5FC290;

// 0x5FC298
static GameContextF8* dword_5FC298;

// 0x5FC2A0
static int64_t qword_5FC2A0;

// 0x5FC2C8
static bool location_is_in_editor;

// 0x5FC2CC
static int location_iso_window_handle;

// 0x5FC2D0
static int dword_5FC2D0;

// 0x5FC2D4
static int dword_5FC2D4;

// 0x5FC2D8
static int64_t qword_5FC2D8;

// 0x5FC2E0
static int64_t qword_5FC2E0;

// 0x5FC2E8
static int64_t qword_5FC2E8;

// 0x5FC2F0
static int64_t qword_5FC2F0;

// 0x5FC2F8
static LocationFunc5FC2F8* dword_5FC2F8;

// 0x4B8440
bool location_init(GameContext* ctx)
{
    TigWindowData window_data;
    if (tig_window_data(ctx->iso_window_handle, &window_data)) {
        tig_debug_printf("location_init: ERROR: couldn't grab window data!\n");
        exit(EXIT_SUCCESS);
    }

    dword_5FC298 = ctx->field_8;

    stru_5FC278.width = window_data.rect.width;
    stru_5FC278.height = window_data.rect.height;
    stru_5FC278.y = 0;
    stru_5FC278.x = 0;

    qword_5FC2E0 = 0;
    qword_5FC2E8 = 0;

    location_set_limits(0x100000000, 0x100000000);
    sub_4B8CE0(sub_4B9810());

    dword_5FC2D0 = 0;
    location_is_in_editor = ctx->editor;

    return true;
}

// 0x4B8530
void location_exit()
{
}

// 0x4B8540
void location_resize(ResizeContext* ctx)
{
    stru_5FC278 = ctx->field_14;
    location_iso_window_handle = ctx->iso_window_handle;
    qword_5FC2A0 = stru_5FC278.width / 2;
    qword_5FC290 = stru_5FC278.height / 2;
}

// TODO: Review type.
// 0x4B85A0
bool sub_4B85A0(void* a1)
{
    // TODO: Incomplete.
}

// 0x4B8D40
void location_set_func_5FC2F8(LocationFunc5FC2F8* func)
{
    dword_5FC2F8 = func;
}

// 0x4B93F0
void sub_4B93F0(int a1, int a2, int* a3, int* a4)
{
    // TODO: Strange math, check.
    int v1 = (a1 - 40) / 2;
    int v2 = 2 * (a2 / 2);
    *a3 = v2 - v1;
    *a4 = v1 + v2;
}

// 0x4B9760
bool location_set_limits(int64_t x, int64_t y)
{
    if (x > 0x100000000 || y > 0x100000000) {
        return false;
    }

    qword_5FC2E0 = 0;
    qword_5FC2E8 = 0;
    qword_5FC2F0 = x;
    qword_5FC288 = y;
    qword_5FC2D8 = 20 * y;

    return true;
}

// 0x4B97E0
void location_get_limits(int64_t* x, int64_t* y)
{
    *x = qword_5FC2F0;
    *y = qword_5FC288;
}

// 0x4B9810
int sub_4B9810()
{
    return qword_5FC2F0 >> 1;
}
