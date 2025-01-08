#include "game/tile_block.h"

#include "game/a_name.h"
#include "game/sector.h"
#include "game/tile.h"

static void tileblock_get_rect_internal(int64_t sector_id, int tile, TigRect* rect);

// 0x5FC370
static tig_art_id_t tileblock_td_aid;

// 0x5FC374
static bool tileblock_editor;

// 0x5FC378
static tig_art_id_t tileblock_iso_aid;

// 0x5FC37C
static IsoInvalidateRectFunc* tileblock_invalidate_rect;

// 0x5FC380
static bool tileblock_visible;

// 0x5FC388
static ViewOptions tileblock_view_options;

// 0x5FC390
static tig_window_handle_t tileblock_iso_window;

// 0x4BAFF0
bool tileblock_init(GameInitInfo* init_info)
{
    tileblock_iso_window = init_info->iso_window_handle;
    tileblock_invalidate_rect = init_info->invalidate_rect_func;
    tileblock_editor = init_info->editor;
    tileblock_view_options.type = VIEW_TYPE_ISOMETRIC;

    tig_art_interface_id_create(614, 0, 0, 0, &tileblock_iso_aid);
    tig_art_interface_id_create(615, 0, 0, 0, &tileblock_td_aid);

    tileblock_visible = true;

    return true;
}

// 0x4BB050
void tileblock_exit()
{
}

// 0x4BB060
bool tileblock_update_view(ViewOptions* view_info)
{
    tileblock_view_options = *view_info;
    return true;
}

// 0x4BB080
void tileblock_resize(GameResizeInfo* resize_info)
{
    tileblock_iso_window = resize_info->window_handle;
}

// 0x4BB090
bool tileblock_is_visible()
{
    return tileblock_visible;
}

// 0x4BB0A0
void tileblock_toggle()
{
    tileblock_visible = !tileblock_visible;
}

// 0x4BB0C0
void tileblock_render(UnknownContext* render_info)
{
    TigArtBlitInfo art_blit_info;
    TigRect src_rect;
    TigRect dst_rect;
    TigArtFrameData art_frame_data;
    Sector601808* sector_node;
    Sector* sector;
    int tile;
    tig_art_id_t aid;
    TigRect tb_rect;
    bool tb_rect_calculated;
    TigRectListNode* rect_node;
    ObjectNode* obj_node;
    int obj_type;
    bool occupied;

    if (!tileblock_visible) {
        return;
    }

    switch (tileblock_view_options.type) {
    case VIEW_TYPE_ISOMETRIC:
        art_blit_info.art_id = tileblock_iso_aid;
        break;
    case VIEW_TYPE_TOP_DOWN:
        art_blit_info.art_id = tileblock_td_aid;
        tig_art_frame_data(tileblock_td_aid, &art_frame_data);
        src_rect.y = 0;
        src_rect.x = 0;
        src_rect.width = art_frame_data.width;
        src_rect.height = art_frame_data.height;
        break;
    default:
        // Unknown view type.
        return;
    }

    art_blit_info.flags = TIG_ART_BLT_BLEND_COLOR_CONST;
    art_blit_info.src_rect = &src_rect;
    art_blit_info.dst_rect = &dst_rect;

    sector_node = render_info->field_C;
    while (sector_node != NULL) {
        if (sector_lock(sector_node->id, &sector)) {
            for (tile = 0; tile < 4096; tile++) {
                occupied = false;
                tb_rect_calculated = false;

                aid = sector->tiles.art_ids[tile];
                if ((tig_art_type(aid) == TIG_ART_TYPE_FACADE
                        && !sub_504AC0(aid))
                    || sub_4EBAB0(aid)) {
                    occupied = true;
                } else {
                    obj_node = sector->objects.heads[tile];
                    while (obj_node != NULL) {
                        obj_type = obj_field_int32_get(obj_node->obj, OBJ_F_TYPE);
                        if (obj_type != OBJ_TYPE_WALL
                            && obj_type != OBJ_TYPE_PORTAL
                            && (obj_field_int32_get(obj_node->obj, OBJ_F_FLAGS) & OF_NO_BLOCK) == 0) {
                            occupied = true;
                            break;
                        }
                        obj_node = obj_node->next;
                    }
                }

                if (occupied) {
                    art_blit_info.color = tig_color_make(128, 0, 0);

                    tileblock_get_rect_internal(sector_node->id, tile, &tb_rect);
                    tb_rect_calculated = true;

                    rect_node = *render_info->rects;
                    while (rect_node != NULL) {
                        if (tig_rect_intersection(&tb_rect, &(rect_node->rect), &dst_rect) == TIG_OK) {
                            if (tileblock_view_options.type != VIEW_TYPE_TOP_DOWN) {
                                src_rect.x = dst_rect.x - tb_rect.x;
                                src_rect.width = dst_rect.width;
                                src_rect.y = dst_rect.y - tb_rect.y;
                                src_rect.height = dst_rect.height;
                            }
                            tig_window_blit_art(tileblock_iso_window, &art_blit_info);
                        }
                        rect_node = rect_node->next;
                    }
                }

                if ((sector->blocks.mask[tile / 32] & (1 << (tile % 32))) != 0) {
                    art_blit_info.color = tig_color_make(255, 0, 0);

                    if (!tb_rect_calculated) {
                        tileblock_get_rect_internal(sector_node->id, tile, &tb_rect);
                    }

                    rect_node = *render_info->rects;
                    while (rect_node != NULL) {
                        if (tig_rect_intersection(&tb_rect, &(rect_node->rect), &dst_rect) == TIG_OK) {
                            if (tileblock_view_options.type != VIEW_TYPE_TOP_DOWN) {
                                src_rect.x = dst_rect.x - tb_rect.x;
                                src_rect.width = dst_rect.width;
                                src_rect.y = dst_rect.y - tb_rect.y;
                                src_rect.height = dst_rect.height;
                            }
                            tig_window_blit_art(tileblock_iso_window, &art_blit_info);
                        }
                        rect_node = rect_node->next;
                    }
                }
            }

            sector_unlock(sector_node->id);
        }
        sector_node = sector_node->next;
    }
}

// 0x4BB410
bool tileblock_is_tile_blocked(int64_t loc)
{
    int64_t sector_id;
    Sector* sector;
    int tile;
    bool blocked;

    sector_id = sector_id_from_loc(loc);
    if (sector_lock(sector_id, &sector)) {
        tile = sub_4D7090(loc);
        blocked = (sector->blocks.mask[tile / 32] & (1 << (tile % 32))) != 0;
        sector_unlock(sector_id);
        return blocked;
    }

    return false;
}

// 0x4BB490
void tileblock_set_tile_blocked(int64_t loc, bool blocked)
{
    int64_t sector_id;
    Sector* sector;
    int tile;
    TigRect rect;

    sector_id = sector_id_from_loc(loc);
    tile = sub_4D7090(loc);
    if (sector_lock(sector_id, &sector)) {
        if (blocked) {
            sector->blocks.mask[tile / 32] |= 1 << (tile % 32);
        } else {
            sector->blocks.mask[tile / 32] &= ~(1 << (tile % 32));
        }
        sector->blocks.modified = true;
        sector_unlock(sector_id);
    }

    tileblock_get_rect_internal(sector_id, tile, &rect);
    tileblock_invalidate_rect(&rect);
}

// 0x4BB550
void tileblock_get_rect(int64_t loc, TigRect* rect)
{
    tileblock_get_rect_internal(sector_id_from_loc(loc),
        sub_4D7090(loc),
        rect);
}

// 0x4BB590
void tileblock_get_rect_internal(int64_t sector_id, int tile, TigRect* rect)
{
    int64_t loc;
    int64_t x;
    int64_t y;
    TigArtFrameData art_frame_data;

    loc = location_make(SECTOR_X(sector_id) + TILE_X(tile), SECTOR_Y(sector_id) + TILE_Y(tile));
    location_xy(loc, &x, &y);

    if (x > INT_MIN && x < INT_MAX
        && y > INT_MIN && y < INT_MAX) {
        if (tileblock_view_options.type == VIEW_TYPE_TOP_DOWN) {
            rect->x = (int)x;
            rect->y = (int)y;
            rect->width = tileblock_view_options.zoom;
            rect->height = tileblock_view_options.zoom;
            return;
        }

        if (tig_art_frame_data(tileblock_iso_aid, &art_frame_data) == TIG_OK) {
            rect->x = (int)x - art_frame_data.hot_x + 40;
            rect->y = (int)y - ((40 - art_frame_data.height) / 2) - art_frame_data.hot_y + 40;
            rect->width = art_frame_data.width;
            rect->height = art_frame_data.height;
            return;
        }
    }

    // Something's wrong, nullify rect.
    rect->x = 0;
    rect->y = 0;
    rect->width = 0;
    rect->height = 0;
}
