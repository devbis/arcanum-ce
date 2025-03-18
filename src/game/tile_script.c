#include "game/tile_script.h"

#include "game/sector.h"
#include "game/tile_script_list.h"
#include "game/tile.h"

typedef struct TileScript {
    int64_t loc;
    Script scr;
} TileScript;

static_assert(sizeof(TileScript) == 0x18, "wrong size");

static bool tile_script_get(int64_t loc, TileScript* tile_script);
static bool tile_script_set(TileScript* tile_script);
static bool tile_script_remove(int64_t loc);
static void tile_script_get_rect(int64_t sector_id, int tile, TigRect* rect);

// 0x5FC868
static tig_art_id_t tile_script_iso_art_id;

// 0x5FC86C
static bool tile_script_editor;

// 0x5FC870
static IsoInvalidateRectFunc* tile_script_iso_invalidate_rect;

// 0x5FC874
static bool tile_script_initialized;

// 0x5FC878
static bool tile_script_enabled;

// 0x5FC87C
static tig_window_handle_t tile_script_iso_window_handle;

// 0x5FC880
static ViewOptions tile_script_view_options;

// 0x5FC888
static tig_art_id_t tile_script_td_art_id;

// 0x4C0530
bool tile_script_init(GameInitInfo* init_info)
{
    tile_script_iso_window_handle = init_info->iso_window_handle;
    tile_script_iso_invalidate_rect = init_info->invalidate_rect_func;
    tile_script_editor = init_info->editor;
    tile_script_view_options.type = VIEW_TYPE_ISOMETRIC;

    tig_art_interface_id_create(475, 0, 0, 0, &tile_script_iso_art_id);
    tig_art_interface_id_create(476, 0, 0, 0, &tile_script_td_art_id);

    tile_script_initialized = true;
    tile_script_enabled = true;

    sub_4F6310();

    return true;
}

// 0x4C05B0
void tile_script_reset()
{
}

// 0x4C05C0
void tile_script_exit()
{
    TileScriptListNode* next;

    tile_script_initialized = false;

    while (tile_script_node_head != NULL) {
        next = tile_script_node_head->next;
        FREE(tile_script_node_head);
        tile_script_node_head = next;
    }
}

// 0x4C05D0
void tile_script_resize(GameResizeInfo* resize_info)
{
    tile_script_iso_window_handle = resize_info->window_handle;
}

// NOTE: Original code returns `1` which breaks required signature.
//
// 0x4C05E0
void tile_script_update_view(ViewOptions* view_options)
{
    tile_script_view_options = *view_options;
}

// 0x4C0600
bool tile_script_is_enabled()
{
    return tile_script_enabled;
}

// 0x4C0610
void tile_script_toggle()
{
    tile_script_enabled = !tile_script_enabled;
}

// 0x4C0630
void tile_script_render(UnknownContext* render_info)
{
    TigArtBlitInfo art_blit_info;
    TigArtFrameData art_frame_data;
    TigRect src_rect;
    TigRect dst_rect;
    Sector601808* sector_node;
    Sector* sector;
    TileScriptListNode* tile_script_node;
    TigRect tile_rect;
    TigRectListNode* rect_node;

    if (!tile_script_enabled) {
        return;
    }

    switch (tile_script_view_options.type) {
    case VIEW_TYPE_ISOMETRIC:
        art_blit_info.art_id = tile_script_iso_art_id;
        break;
    case VIEW_TYPE_TOP_DOWN:
        art_blit_info.art_id = tile_script_td_art_id;
        tig_art_frame_data(art_blit_info.art_id, &art_frame_data);
        src_rect.x = 0;
        src_rect.y = 0;
        src_rect.width = art_frame_data.width;
        src_rect.height = art_frame_data.height;
        break;
    default:
        // Unknown view type.
        return;
    }

    art_blit_info.flags = 0;
    art_blit_info.src_rect = &src_rect;
    art_blit_info.dst_rect = &dst_rect;

    sector_node = render_info->field_C;
    while (sector_node != NULL) {
        if (sector_lock(sector_node->id, &sector)) {
            tile_script_node = sector->tile_scripts.head;
            while (tile_script_node != NULL) {
                tile_script_get_rect(sector_node->id, tile_script_node->id, &tile_rect);
                rect_node = *render_info->rects;
                while (rect_node != NULL) {
                    if (tig_rect_intersection(&tile_rect, &(rect_node->rect), &dst_rect) == TIG_OK) {
                        src_rect.x = dst_rect.x - tile_rect.x;
                        src_rect.y = dst_rect.y - tile_rect.y;
                        src_rect.width = dst_rect.width;
                        src_rect.height = dst_rect.height;
                    }
                    tig_window_blit_art(tile_script_iso_window_handle, &art_blit_info);
                    rect_node = rect_node->next;
                }
                tile_script_node = tile_script_node->next;
            }
            sector_unlock(sector_node->id);
        }
        sector_node = sector_node->next;
    }
}

// 0x4C07B0
bool tile_script_get(int64_t loc, TileScript* tile_script)
{
    int64_t sector_id;
    Sector* sector;
    int tile;
    bool rc;

    sector_id = sector_id_from_loc(loc);
    if (!sector_lock(sector_id, &sector)) {
        return false;
    }

    tile = tile_id_from_loc(loc);
    rc = tile_script_list_get(&(sector->tile_scripts), tile, &(tile_script->scr));
    tile_script->loc = loc;
    sector_unlock(sector_id);

    return rc;
}

// 0x4C0830
bool tile_script_set(TileScript* tile_script)
{
    int tile;
    int64_t sector_id;
    Sector* sector;
    TigRect dirty_rect;

    tile = tile_id_from_loc(tile_script->loc);
    sector_id = sector_id_from_loc(tile_script->loc);
    if (!sector_lock(sector_id, &sector)) {
        return false;
    }

    if (!tile_script_editor || tile_script->scr.num != 0) {
        tile_script_list_set(&(sector->tile_scripts), tile, &(tile_script->scr));
    } else {
        tile_script_list_remove(&(sector->tile_scripts), tile);
    }

    sector_unlock(sector_id);

    tile_script_get_rect(sector_id, tile, &dirty_rect);
    tile_script_iso_invalidate_rect(&dirty_rect);

    return true;
}

// 0x4C08E0
void tile_script_move(int64_t old_loc, int64_t new_loc)
{
    TileScript tile_script;

    if (!tile_script_get(old_loc, &tile_script)) {
        return;
    }

    tile_script_remove(old_loc);

    tile_script.loc = new_loc;
    tile_script_set(&tile_script);
}

// 0x4C0930
bool tile_script_remove(int64_t loc)
{
    TileScript tile_script;

    if (!tile_script_get(loc, &tile_script)) {
        return false;
    }

    tile_script.scr.num = 0;
    tile_script_set(&tile_script);

    return true;
}

// 0x4C0980
void tile_script_exec(int64_t loc, int64_t triggerer_obj)
{
    TileScript tile_script;
    Script scr;
    ScriptInvocation invocation;

    if (!tile_script_get(loc, &tile_script)) {
        return;
    }

    scr = tile_script.scr;

    invocation.script = &(tile_script.scr);
    invocation.attachee_obj = OBJ_HANDLE_NULL;
    invocation.extra_obj = OBJ_HANDLE_NULL;
    invocation.line = 0;
    invocation.attachment_point = SAP_USE;
    invocation.triggerer_obj = triggerer_obj;
    sub_4449B0(&invocation);

    if (invocation.script->num != scr.num
        || invocation.script->hdr.flags != scr.hdr.flags
        || invocation.script->hdr.counters != scr.hdr.counters) {
        tile_script_set(&tile_script);
    }
}

// 0x4C0A40
void tile_script_get_rect(int64_t sector_id, int tile, TigRect* rect)
{
    int64_t loc;
    int64_t x;
    int64_t y;
    TigArtFrameData art_frame_data;

    loc = location_make((SECTOR_X(sector_id) << 6) | TILE_X(tile),
        (SECTOR_Y(sector_id) << 6) | TILE_Y(tile));
    location_xy(loc, &x, &y);

    if (x < INT_MIN
        || x > INT_MAX
        || y < INT_MIN
        || y > INT_MAX) {
        rect->x = 0;
        rect->y = 0;
        rect->width = 0;
        rect->height = 0;
        return;
    }

    if (tile_script_view_options.type == VIEW_TYPE_TOP_DOWN) {
        rect->x = (int)x;
        rect->y = (int)y;
        rect->width = tile_script_view_options.zoom;
        rect->height = tile_script_view_options.zoom;
        return;
    }

    if (tig_art_frame_data(tile_script_iso_art_id, &art_frame_data) != TIG_OK) {
        rect->x = 0;
        rect->y = 0;
        rect->width = 0;
        rect->height = 0;
        return;
    }

    rect->x = (int)x - art_frame_data.hot_x + 40;
    rect->y = (int)y - (40 - art_frame_data.height) / 2 - art_frame_data.hot_y + 40;
    rect->width = art_frame_data.width;
    rect->height = art_frame_data.height;
}
