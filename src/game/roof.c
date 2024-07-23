#include "game/roof.h"

// 0x5A53A0
static int dword_5A53A0 = 256;

// 0x5E2E34
static GameContextF8* dword_5E2E34;

// 0x5E2E40
static bool dword_5E2E40;

// 0x5E2E44
static tig_window_handle_t roof_window_handle;

// 0x5E2E50
static ViewOptions roof_view_options;

// 0x4390D0
void roof_exit()
{
    roof_window_handle = TIG_WINDOW_HANDLE_INVALID;
    dword_5E2E34 = NULL;
}

// 0x4390F0
void roof_resize(ResizeInfo* resize_info)
{
    roof_window_handle = resize_info->iso_window_handle;
}

// 0x439100
bool sub_439100(ViewOptions* view_options)
{
    roof_view_options = *view_options;
    return true;
}

// 0x4395C0
int sub_4395C0(int a1)
{
    return ((a1 >> 2) & 0xF) + ((a1 >> 8) << 4);
}

// 0x4395E0
int64_t sub_4395E0(int64_t a1)
{
    return ((a1 & 0xFFFFFFFF) - (a1 & 0xFFFFFFFF) % 4 + 2)
        | (((a1 >> 32) - (a1 >> 32) % 4 + 2) << 32);
}

// 0x439EA0
void sub_439EA0(int64_t a1)
{
    sub_43A1A0(sub_4395E0(a1), 0, -1);
}

// 0x439EE0
void sub_439EE0(int64_t a1)
{
    tig_art_id_t aid;

    aid = sub_4396A0(a1);
    if (sub_504940(aid) == 0) {
        sub_439700(a1, sub_504970(aid, 1));
    }
}

// 0x439F20
void sub_439F20(int64_t a1)
{
    tig_art_id_t aid;

    aid = sub_4396A0(a1);
    if (sub_504940(aid) != 0) {
        sub_439700(a1, sub_504970(aid, 0));
    }
}

// 0x439FA0
bool sub_439FA0(int64_t a1)
{
    tig_art_id_t aid;

    if (dword_5E2E40) {
        aid = sub_4396A0(a1);
        if (aid != TIG_ART_ID_INVALID
            && sub_5048D0(aid) == 0
            && sub_504940(aid) != 0) {
            return true;
        }
    }

    return false;
}

// 0x439FF0
bool sub_439FF0(int64_t a1, int64_t a2, int a3)
{
    int64_t v1;

    sub_4B8730(a1, a2, &v1);
    return sub_43A030(v1, a3);
}

// 0x43A110
void sub_43A110(int a1)
{
    dword_5A53A0 = a1;
    if (dword_5E2E34 != NULL) {
        dword_5E2E34(0);
    }
}

// 0x43A130
int sub_43A130()
{
    return dword_5A53A0;
}
