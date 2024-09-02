#include "game/location.h"

#include "tig/debug.h"
#include "tig/rect.h"
#include "tig/window.h"

// 0x5FC278
static TigRect stru_5FC278;

// 0x5FC288
static int64_t location_limit_y;

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
static ViewOptions location_view_options;

// 0x5FC2D8
static int64_t qword_5FC2D8;

// 0x5FC2E0
static int64_t qword_5FC2E0;

// 0x5FC2E8
static int64_t qword_5FC2E8;

// 0x5FC2F0
static int64_t location_limit_x;

// 0x5FC2F8
static LocationFunc5FC2F8* dword_5FC2F8;

// 0x4B8440
bool location_init(GameInitInfo* init_info)
{
    TigWindowData window_data;

    if (tig_window_data(init_info->iso_window_handle, &window_data)) {
        tig_debug_printf("location_init: ERROR: couldn't grab window data!\n");
        exit(EXIT_SUCCESS);
    }

    location_iso_window_handle = init_info->iso_window_handle;
    dword_5FC298 = init_info->field_8;

    stru_5FC278.x = 0;
    stru_5FC278.y = 0;
    stru_5FC278.width = window_data.rect.width;
    stru_5FC278.height = window_data.rect.height;

    qword_5FC2E0 = 0;
    qword_5FC2E8 = 0;

    location_set_limits(0x100000000, 0x100000000);
    sub_4B8CE0(sub_4B9810());

    location_view_options.type = VIEW_TYPE_ISOMETRIC;
    location_is_in_editor = init_info->editor;

    return true;
}

// 0x4B8530
void location_exit()
{
}

// 0x4B8540
void location_resize(ResizeContext* resize_info)
{
    stru_5FC278 = resize_info->field_14;
    location_iso_window_handle = resize_info->iso_window_handle;
    qword_5FC2A0 = stru_5FC278.width / 2;
    qword_5FC290 = stru_5FC278.height / 2;
}

// 0x4B85A0
bool location_update_view(ViewOptions* view_options)
{
    int64_t v1;

    if (!sub_4B8730(qword_5FC2A0, qword_5FC290, &v1)) {
        return false;
    }

    if (view_options->type == location_view_options.type) {
        if (view_options->type == VIEW_TYPE_ISOMETRIC) {
            return true;
        }
        if (view_options->zoom == location_view_options.zoom) {
            return true;
        }
    }

    if (view_options->type == VIEW_TYPE_ISOMETRIC) {
        qword_5FC2E0 = 0;
        qword_5FC2E8 = 0;
        location_view_options = *view_options;
        sub_4B8CE0(v1);
        return true;
    }

    if (view_options->zoom >= 12 && view_options->zoom <= 64) {
        qword_5FC2E0 = stru_5FC278.width;
        qword_5FC2E8 = 0;
        location_view_options = *view_options;
        sub_4B8CE0(v1);
        return true;
    }

    return false;
}

// 0x4B8680
void sub_4B8680(int64_t location, int64_t* x, int64_t* y)
{
    if (location_view_options.type == VIEW_TYPE_ISOMETRIC) {
        *x = qword_5FC2E0 + 40 * (LOCATION_GET_Y(location) - LOCATION_GET_X(location) - 1);
        *y = qword_5FC2E8 + 20 * (LOCATION_GET_X(location) + LOCATION_GET_Y(location));
    } else {
        *x = qword_5FC2E0 - location_view_options.zoom * LOCATION_GET_X(location);
        *y = qword_5FC2E8 + location_view_options.zoom * LOCATION_GET_Y(location);
    }
}

// 0x4B8AD0
void sub_4B8AD0(int64_t* a1, int64_t* a2)
{
    *a1 = qword_5FC2E0;
    *a2 = qword_5FC2E8;
}

// 0x4B8CE0
void sub_4B8CE0(int64_t a1)
{
    int64_t v1;
    int64_t v2;

    sub_4B8940(a1, &v1, &v2);
    sub_4B8B30(v1, v2);
    dword_5FC298(&stru_5FC278);
    if (dword_5FC2F8 != NULL) {
        dword_5FC2F8(a1);
    }
}

// 0x4B8D40
void location_set_func_5FC2F8(LocationFunc5FC2F8* func)
{
    dword_5FC2F8 = func;
}

// 0x4B93F0
void sub_4B93F0(int a1, int a2, int* a3, int* a4)
{
    int v1;
    int v2;

    v1 = (a1 - 40) / 2;
    v2 = 2 * (a2 / 2);
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
    location_limit_x = x;
    location_limit_y = y;
    qword_5FC2D8 = 20 * y;

    return true;
}

// 0x4B97E0
void location_get_limits(int64_t* x, int64_t* y)
{
    *x = location_limit_x;
    *y = location_limit_y;
}

// 0x4B9810
int64_t sub_4B9810()
{
    return (location_limit_x >> 1) | ((location_limit_y >> 1) << 32);
}

// 0x4B98B0
void sub_4B98B0(int64_t a1, int64_t a2, int64_t* a3, int64_t* a4)
{
    int v1;
    int v2;

    if (location_view_options.type == VIEW_TYPE_ISOMETRIC) {
        v1 = (a1 - qword_5FC2E0) >> 1;
        v2 = a2 - qword_5FC2E8;
        *a3 = (v2 - v1) / 40;
        *a4 = (v1 + v2) / 40;
    } else {
        *a3 = (qword_5FC2E0 + location_view_options.zoom - a1 - 1) / location_view_options.zoom;
        *a4 = (a2 - qword_5FC2E8) / location_view_options.zoom;
    }
}
