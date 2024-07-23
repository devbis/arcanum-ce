#include "game/roof.h"

#include "game/mes.h"
#include "game/tile.h"

static void sub_43A140(int x, int y, tig_art_id_t aid, TigRect* rect);

// 0x5A53A0
static int dword_5A53A0 = 256;

// 0x5E2E30
static uint8_t roof_partial_opacity;

// 0x5E2E31
static uint8_t roof_full_transparency;

// 0x5E2E34
static GameContextF8* dword_5E2E34;

// 0x5E2E38
static bool dword_5E2E38;

// 0x5E2E3C
static bool roof_editor;

// 0x5E2E40
static bool dword_5E2E40;

// 0x5E2E44
static tig_window_handle_t roof_window_handle;

// 0x5E2E48
static uint8_t roof_full_opacity;

// 0x5E2E50
static ViewOptions roof_view_options;

// 0x438F90
bool roof_init(GameInitInfo* init_info)
{
    mes_file_handle_t mes_file;
    MesFileEntry mes_file_entry;

    roof_window_handle = init_info->iso_window_handle;
    dword_5E2E34 = init_info->field_8;
    roof_view_options.type = VIEW_TYPE_ISOMETRIC;
    roof_editor = init_info->editor;
    dword_5E2E40 = true;

    if (!mes_load("art\\roof\\roofshade.mes", &mes_file)) {
        return false;
    }

    mes_file_entry.num = 0;
    if (!mes_search(mes_file, &mes_file_entry)) {
        mes_unload(mes_file);
        return false;
    }
    roof_full_opacity = (uint8_t)atoi(mes_file_entry.str);

    mes_file_entry.num = 1;
    if (!mes_search(mes_file, &mes_file_entry)) {
        mes_unload(mes_file);
        return false;
    }
    roof_partial_opacity = (uint8_t)atoi(mes_file_entry.str);

    mes_file_entry.num = 2;
    if (!mes_search(mes_file, &mes_file_entry)) {
        mes_unload(mes_file);
        return false;
    }
    roof_full_transparency = (uint8_t)atoi(mes_file_entry.str);

    mes_unload(mes_file);

    dword_5E2E38 = tig_video_3d_check_initialized() == TIG_OK;
    if (!dword_5E2E38) {
        dword_5A53A0 = TIG_ART_BLT_BLEND_ALPHA_STIPPLE_D;
    }

    return true;
}

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

// 0x4395A0
int sub_4395A0(int a1, int a2)
{
    return sub_4395C0(sub_4D7090(a1, a2));
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

// 0x43A140
void sub_43A140(int x, int y, tig_art_id_t aid, TigRect* rect)
{
    TigArtFrameData art_frame_data;

    rect->x = 0;
    rect->y = 0;
    rect->width = 0;
    rect->height = 0;

    if (tig_art_frame_data(aid, &art_frame_data) == TIG_OK) {
        rect->x = x - art_frame_data.hot_x;
        rect->y = y - art_frame_data.hot_y;
        rect->width = art_frame_data.width;
        rect->height = art_frame_data.height;
    }
}
