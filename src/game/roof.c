#include "game/roof.h"

#include "game/gamelib.h"
#include "game/light.h"
#include "game/location.h"
#include "game/mes.h"
#include "game/sector.h"
#include "game/tile.h"

static int roof_id_from_loc(int64_t loc);
static int sub_4395C0(int a1);
static void roof_xy(int64_t loc, int64_t* sx, int64_t* sy);
static tig_art_id_t roof_art_id_get(int64_t loc);
static bool roof_art_id_set(int64_t loc, tig_art_id_t aid);
static void sub_43A140(int x, int y, tig_art_id_t aid, TigRect* rect);
static void roof_fill(int64_t loc, int a2, int a3);

// 0x5A53A0
static unsigned int roof_blit_flags = TIG_ART_BLT_BLEND_ALPHA_CONST;

// 0x5A53A4
static char byte_5A53A4[13][4][4] = {
    {
        { 0, 0, 0, 0 },
        { 0, 0, 0, 0 },
        { 1, 1, 1, 0 },
        { 1, 1, 1, 0 },
    },
    {
        { 1, 1, 1, 0 },
        { 1, 1, 1, 0 },
        { 1, 1, 1, 0 },
        { 1, 1, 1, 0 },
    },
    {
        { 0, 0, 0, 0 },
        { 0, 0, 0, 0 },
        { 1, 1, 1, 1 },
        { 1, 1, 1, 1 },
    },
    {
        { 1, 1, 1, 0 },
        { 1, 1, 1, 0 },
        { 1, 1, 1, 1 },
        { 1, 1, 1, 1 },
    },
    {
        { 1, 1, 1, 0 },
        { 1, 1, 1, 0 },
        { 1, 1, 1, 0 },
        { 0, 0, 0, 0 },
    },
    {
        { 1, 1, 1, 1 },
        { 1, 1, 1, 1 },
        { 1, 1, 1, 1 },
        { 1, 1, 1, 0 },
    },
    {
        { 0, 0, 1, 1 },
        { 0, 0, 1, 1 },
        { 1, 1, 1, 1 },
        { 1, 1, 1, 1 },
    },
    {
        { 0, 0, 0, 0 },
        { 0, 0, 0, 0 },
        { 0, 0, 1, 1 },
        { 0, 0, 1, 1 },
    },
    {
        { 1, 1, 1, 1 },
        { 1, 1, 1, 1 },
        { 1, 1, 1, 1 },
        { 1, 1, 1, 1 },
    },
    {
        { 0, 0, 1, 1 },
        { 0, 0, 1, 1 },
        { 0, 0, 1, 1 },
        { 0, 0, 0, 0 },
    },
    {
        { 1, 1, 1, 1 },
        { 1, 1, 1, 1 },
        { 1, 1, 1, 1 },
        { 0, 0, 0, 0 },
    },
    {
        { 0, 0, 1, 1 },
        { 0, 0, 1, 1 },
        { 0, 0, 1, 1 },
        { 0, 0, 1, 1 },
    },
    {
        { 1, 1, 1, 1 },
        { 1, 1, 1, 1 },
        { 1, 1, 1, 1 },
        { 0, 0, 1, 1 },
    },
};

// 0x5E2E30
static uint8_t roof_partial_opacity;

// 0x5E2E31
static uint8_t roof_full_transparency;

// 0x5E2E34
static IsoInvalidateRectFunc* roof_iso_window_invalidate_rect;

// 0x5E2E38
static bool dword_5E2E38;

// 0x5E2E3C
static bool roof_editor;

// 0x5E2E40
static bool roof_enabled;

// 0x5E2E44
static tig_window_handle_t roof_iso_window_handle;

// 0x5E2E48
static uint8_t roof_full_opacity;

// 0x5E2E50
static ViewOptions roof_view_options;

// 0x438F90
bool roof_init(GameInitInfo* init_info)
{
    mes_file_handle_t mes_file;
    MesFileEntry mes_file_entry;

    roof_iso_window_handle = init_info->iso_window_handle;
    roof_iso_window_invalidate_rect = init_info->invalidate_rect_func;
    roof_view_options.type = VIEW_TYPE_ISOMETRIC;
    roof_editor = init_info->editor;
    roof_enabled = true;

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
        roof_blit_flags = TIG_ART_BLT_BLEND_ALPHA_STIPPLE_D;
    }

    return true;
}

// 0x4390D0
void roof_exit()
{
    roof_iso_window_handle = TIG_WINDOW_HANDLE_INVALID;
    roof_iso_window_invalidate_rect = NULL;
}

// 0x4390F0
void roof_resize(GameResizeInfo* resize_info)
{
    roof_iso_window_handle = resize_info->window_handle;
}

// 0x439100
bool render_update_view(ViewOptions* view_options)
{
    roof_view_options = *view_options;
    return true;
}

// 0x439120
void roof_toggle()
{
    roof_enabled = !roof_enabled;
}

// 0x439140
void roof_render(UnknownContext* render_info)
{
    TigArtBlitInfo art_blit_info;
    TigRect dst_rect;
    TigRect src_rect;
    LocRect loc_rect;
    int64_t y;
    int64_t x;
    unsigned int flags;
    tig_art_id_t aid;
    int64_t loc_x;
    int64_t loc_y;
    TigRect roof_rect;
    TigRectListNode* node;

    if (!roof_enabled) {
        return;
    }

    if (roof_view_options.type != VIEW_TYPE_ISOMETRIC) {
        return;
    }

    if (dword_5E2E38) {
        art_blit_info.color = light_get_outdoor_color();
        flags = TIG_ART_BLT_BLEND_COLOR_CONST;
    } else {
        flags = 0;
    }

    loc_rect = *render_info->field_4;

    for (y = loc_rect.y1; y <= loc_rect.y2; y += 4) {
        for (x = loc_rect.x1; x <= loc_rect.x2; x += 4) {
            aid = roof_art_id_get(LOCATION_MAKE(x, y));
            if (aid != TIG_ART_ID_INVALID
                && !sub_5048D0(aid)) {
                roof_xy(LOCATION_MAKE(x, y), &loc_x, &loc_y);
                if (loc_x > INT_MIN
                    && loc_x < INT_MAX
                    && loc_y > INT_MIN
                    && loc_y < INT_MAX) {
                    sub_43A140((int)loc_x, (int)loc_y, aid, &roof_rect);
                    node = *render_info->rects;
                    while (node != NULL) {
                        if (tig_rect_intersection(&roof_rect, &node->rect, &dst_rect) == TIG_OK) {
                            src_rect.x = dst_rect.x - roof_rect.x;
                            src_rect.y = dst_rect.y - roof_rect.y;
                            src_rect.width = dst_rect.width;
                            src_rect.height = dst_rect.height;

                            art_blit_info.art_id = aid;
                            art_blit_info.src_rect = &src_rect;
                            art_blit_info.dst_rect = &dst_rect;
                            art_blit_info.flags = flags;

                            if (sub_504940(aid)) {
                                if (roof_blit_flags == TIG_ART_BLT_BLEND_ALPHA_CONST) {
                                    art_blit_info.flags |= TIG_ART_BLT_BLEND_ALPHA_LERP_BOTH;

                                    switch (sub_504840(aid)) {
                                    case 0:
                                        art_blit_info.alpha[0] = roof_full_transparency;
                                        art_blit_info.alpha[1] = roof_partial_opacity;
                                        art_blit_info.alpha[2] = roof_partial_opacity;
                                        art_blit_info.alpha[3] = roof_full_transparency;
                                        break;
                                    case 1:
                                        art_blit_info.alpha[0] = roof_partial_opacity;
                                        art_blit_info.alpha[2] = roof_partial_opacity;
                                        art_blit_info.alpha[1] = roof_full_opacity;
                                        art_blit_info.alpha[3] = roof_full_transparency;
                                        break;
                                    case 2:
                                        art_blit_info.alpha[0] = roof_full_transparency;
                                        art_blit_info.alpha[1] = roof_partial_opacity;
                                        art_blit_info.alpha[2] = roof_full_opacity;
                                        art_blit_info.alpha[3] = roof_partial_opacity;
                                        break;
                                    case 3:
                                        art_blit_info.alpha[0] = roof_partial_opacity;
                                        art_blit_info.alpha[1] = roof_full_opacity;
                                        art_blit_info.alpha[2] = roof_full_opacity;
                                        art_blit_info.alpha[3] = roof_partial_opacity;
                                        break;
                                    case 4:
                                        art_blit_info.alpha[0] = roof_partial_opacity;
                                        art_blit_info.alpha[1] = roof_partial_opacity;
                                        art_blit_info.alpha[2] = roof_full_transparency;
                                        art_blit_info.alpha[3] = roof_full_transparency;
                                        break;
                                    case 5:
                                        art_blit_info.alpha[0] = roof_full_opacity;
                                        art_blit_info.alpha[1] = roof_full_opacity;
                                        art_blit_info.alpha[2] = roof_partial_opacity;
                                        art_blit_info.alpha[3] = roof_partial_opacity;
                                        break;
                                    case 6:
                                        art_blit_info.alpha[0] = roof_partial_opacity;
                                        art_blit_info.alpha[1] = roof_partial_opacity;
                                        art_blit_info.alpha[2] = roof_full_opacity;
                                        art_blit_info.alpha[3] = roof_full_opacity;
                                        break;
                                    case 7:
                                        art_blit_info.alpha[0] = roof_full_transparency;
                                        art_blit_info.alpha[1] = roof_full_transparency;
                                        art_blit_info.alpha[2] = roof_partial_opacity;
                                        art_blit_info.alpha[3] = roof_partial_opacity;
                                        break;
                                    case 8:
                                        art_blit_info.alpha[0] = roof_full_opacity;
                                        art_blit_info.alpha[1] = roof_full_opacity;
                                        art_blit_info.alpha[2] = roof_full_opacity;
                                        art_blit_info.alpha[3] = roof_full_opacity;
                                        break;
                                    case 9:
                                        art_blit_info.alpha[0] = roof_partial_opacity;
                                        art_blit_info.alpha[1] = roof_full_transparency;
                                        art_blit_info.alpha[2] = roof_full_transparency;
                                        art_blit_info.alpha[3] = roof_partial_opacity;
                                        break;
                                    case 10:
                                        art_blit_info.alpha[0] = roof_full_opacity;
                                        art_blit_info.alpha[1] = roof_partial_opacity;
                                        art_blit_info.alpha[2] = roof_full_transparency;
                                        art_blit_info.alpha[3] = roof_partial_opacity;
                                        break;
                                    case 11:
                                        art_blit_info.alpha[0] = roof_partial_opacity;
                                        art_blit_info.alpha[2] = roof_partial_opacity;
                                        art_blit_info.alpha[1] = roof_full_transparency;
                                        art_blit_info.alpha[3] = roof_full_opacity;
                                        break;
                                    case 12:
                                        art_blit_info.alpha[0] = roof_full_opacity;
                                        art_blit_info.alpha[1] = roof_partial_opacity;
                                        art_blit_info.alpha[2] = roof_partial_opacity;
                                        art_blit_info.alpha[3] = roof_full_opacity;
                                        break;
                                    default:
                                        // Should be unreachable.
                                        __assume(0);
                                    }
                                } else {
                                    art_blit_info.flags |= roof_blit_flags;
                                }
                            }

                            art_blit_info.flags |= TIG_ART_BLT_SCRATCH_VALID;
                            art_blit_info.scratch_video_buffer = gamelib_scratch_video_buffer;
                            tig_window_blit_art(roof_iso_window_handle, &art_blit_info);
                        }
                        node = node->next;
                    }
                }
            }
        }
    }
}

// 0x4395A0
int roof_id_from_loc(int64_t loc)
{
    return sub_4395C0(tile_id_from_loc(loc));
}

// 0x4395C0
int sub_4395C0(int a1)
{
    return ((a1 >> 2) & 0xF) + ((a1 >> 8) << 4);
}

// 0x4395E0
int64_t roof_normalize_loc(int64_t loc)
{
    int64_t x = LOCATION_GET_X(loc);
    int64_t y = LOCATION_GET_Y(loc);

    return LOCATION_MAKE(x - x % 4 + 2, y - y % 4 + 2);
}

// 0x439640
void roof_xy(int64_t loc, int64_t* sx, int64_t* sy)
{
    loc = roof_normalize_loc(loc);
    location_xy(loc, sx, sy);

    *sx -= 120;
    *sy -= 200;
}

// 0x4396A0
tig_art_id_t roof_art_id_get(int64_t loc)
{
    int64_t sector_id;
    Sector* sector;
    tig_art_id_t aid;

    sector_id = sector_id_from_loc(loc);
    if (!sector_lock(sector_id, &sector)) {
        return TIG_ART_ID_INVALID;
    }

    aid = sector->roofs.art_ids[roof_id_from_loc(loc)];

    sector_unlock(sector_id);

    return aid;
}

// 0x439700
bool roof_art_id_set(int64_t loc, tig_art_id_t aid)
{
    int64_t sec;
    Sector* sector;
    tig_art_id_t old_aid;
    int64_t sx;
    int64_t sy;
    TigRect rect;

    loc = roof_normalize_loc(loc);
    sec = sector_id_from_loc(loc);
    if (!sector_lock(sec, &sector)) {
        return false;
    }

    old_aid = sector->roofs.art_ids[roof_id_from_loc(loc)];
    sector->roofs.art_ids[roof_id_from_loc(loc)] = aid;
    sector->roofs.field_0 = 0;

    sector_unlock(sec);

    if (aid == TIG_ART_ID_INVALID) {
        aid = old_aid;
    }

    roof_xy(loc, &sx, &sy);
    if (sx > INT_MIN
        && sx < INT_MAX
        && sy > INT_MIN
        && sy < INT_MAX) {
        sub_43A140((int)sx, (int)sy, aid, &rect);
        roof_iso_window_invalidate_rect(&rect);
    }

    return true;
}

// 0x439890
bool sub_439890(int x, int y)
{
    int64_t loc;
    int64_t loc_x;
    int64_t loc_y;
    tig_art_id_t aid;
    TigRect rect;

    if (!roof_enabled) {
        return false;
    }

    if (!location_at(x, y + 120, &loc)) {
        return false;
    }

    aid = roof_art_id_get(loc);
    if (aid == TIG_ART_ID_INVALID) {
        return false;
    }

    if (sub_5048D0(aid)) {
        return false;
    }

    roof_xy(loc, &loc_x, &loc_y);
    sub_43A140((int)loc_x, (int)loc_y, aid, &rect);
    if (x >= rect.x
        && y >= rect.y
        && x < rect.x + rect.width
        && y < rect.y + rect.height
        && !sub_502FD0(aid, x - rect.x, y - rect.y)) {
        return true;
    }

    return false;
}

// 0x439D30
void roof_recalc(int64_t loc)
{
    tig_art_id_t aid;
    int v1;
    int tile;
    int v5;
    int v6;
    bool v8;
    bool v9;

    aid = roof_art_id_get(loc);
    if (aid == TIG_ART_ID_INVALID) {
        return;
    }

    v1 = sub_504840(aid);
    tile = tile_id_from_loc(loc);
    v5 = tile & 3;
    v6 = (tile >> 6) % 4;

    switch (v1) {
    case 0:
        v8 = v6 > 1 && v5 < 3;
        break;
    case 1:
        v8 = v5 < 3;
        break;
    case 2:
        v8 = v6 > 1;
        break;
    case 3:
        if (v5 >= 3 && v6 <= 1) {
            v8 = false;
        } else {
            v8 = true;
        }
        break;
    case 4:
        if (v5 >= 3) {
            v8 = false;
        } else {
            if (v6 < 3) {
                v8 = true;
            } else {
                v8 = false;
            }
        }
        break;
    case 5:
        if (v5 >= 3) {
            if (v6 < 3) {
                v8 = true;
            } else {
                v8 = false;
            }
        } else {
            v8 = true;
        }
        break;
    case 6:
        v8 = v5 > 1 || v6 > 1;
        break;
    case 7:
        if (v5 <= 1 || v6 <= 1) {
            v8 = false;
        } else {
            v8 = true;
        }
        break;
    case 8:
        v8 = true;
        break;
    case 9:
        if (v5 <= 1) {
            v8 = false;
        } else {
            if (v6 < 3) {
                v8 = true;
            } else {
                v8 = false;
            }
        }
        break;
    case 10:
        v8 = v6 < 3;
        break;
    case 11:
        v8 = v5 > 1;
        break;
    case 12:
        v8 = v5 > 1 || v6 < 3;
        break;
    default:
        // Should be unreachable.
        __assume(0);
    }

    v9 = sub_5048D0(aid);
    if (v8 != v9) {
        roof_fill(roof_normalize_loc(loc), 1 - v9, -1);
    }
}

// 0x439EA0
void sub_439EA0(int64_t loc)
{
    roof_fill(roof_normalize_loc(loc), 0, -1);
}

// 0x439EE0
void sub_439EE0(int64_t loc)
{
    tig_art_id_t aid;

    aid = roof_art_id_get(loc);
    if (sub_504940(aid) == 0) {
        aid = sub_504970(aid, 1);
        roof_art_id_set(loc, aid);
    }
}

// 0x439F20
void sub_439F20(int64_t loc)
{
    tig_art_id_t aid;

    aid = roof_art_id_get(loc);
    if (sub_504940(aid) != 0) {
        aid = sub_504970(aid, 0);
        roof_art_id_set(loc, aid);
    }
}

// 0x439FA0
bool sub_439FA0(int64_t loc)
{
    tig_art_id_t aid;

    if (roof_enabled) {
        aid = roof_art_id_get(loc);
        if (aid != TIG_ART_ID_INVALID
            && sub_5048D0(aid) == 0
            && sub_504940(aid) != 0) {
            return true;
        }
    }

    return false;
}

// 0x439FF0
bool sub_439FF0(int64_t x, int64_t y, int a3)
{
    int64_t loc;

    location_at(x, y, &loc);
    return sub_43A030(loc, a3);
}

// 0x43A030
bool sub_43A030(int64_t loc, int a2)
{
    tig_art_id_t aid;

    if (!roof_enabled) {
        return false;
    }

    aid = tile_art_id_at(loc);
    if (tig_art_tile_id_type_get(aid) != 0) {
        return false;
    }

    aid = roof_art_id_get(location_make(location_get_x(loc) + 3, location_get_y(loc) + 3));
    if (aid == TIG_ART_ID_INVALID) {
        return false;
    }

    if (sub_5048D0(aid)) {
        return false;
    }

    if (!byte_5A53A4[sub_504840(aid)][(location_get_y(loc) + 3) % 4][(location_get_x(loc) + 3) % 4]) {
        return false;
    }

    if (a2) {
        return true;
    }

    if (sub_504940(aid)) {
        return false;
    }

    return true;
}

// 0x43A110
void roof_blit_flags_set(unsigned int flags)
{
    roof_blit_flags = flags;
    if (roof_iso_window_invalidate_rect != NULL) {
        roof_iso_window_invalidate_rect(NULL);
    }
}

// 0x43A130
unsigned int roof_blit_flags_get()
{
    return roof_blit_flags;
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

// 0x43A1A0
void roof_fill(int64_t loc, int a2, int a3)
{
    tig_art_id_t aid;
    int v1;

    aid = roof_art_id_get(loc);
    if (aid == TIG_ART_ID_INVALID) {
        return;
    }

    if (sub_5048D0(aid) == a2) {
        return;
    }

    if (a3 != -1) {
        v1 = sub_504840(aid);
        switch (a3) {
        case 1:
            switch (v1) {
            case 0:
            case 1:
            case 4:
                return;
            };
            break;
        case 3:
            switch (v1) {
            case 0:
            case 2:
            case 7:
                return;
            }
            break;
        case 5:
            switch (v1) {
            case 9:
            case 7:
            case 11:
                return;
            }
            break;
        case 7:
            switch (v1) {
            case 4:
            case 9:
            case 10:
                return;
            }
            break;
        }
    }

    aid = sub_504900(aid, a2);
    roof_art_id_set(loc, aid);

    roof_fill(location_make(location_get_x(loc) + 4, location_get_y(loc)), a2, 5);
    roof_fill(location_make(location_get_x(loc) - 4, location_get_y(loc)), a2, 1);
    roof_fill(location_make(location_get_x(loc), location_get_y(loc) + 4), a2, 3);
    roof_fill(location_make(location_get_x(loc), location_get_y(loc) - 4), a2, 7);
}
