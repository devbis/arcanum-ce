#include "game/tb.h"

#include "game/gamelib.h"
#include "game/object.h"

#define TEXT_DURATION_KEY "text duration"
#define MAX_TEXT_BUBBLES 8

#define TEXT_BUBBLE_IN_USE 0x0001
#define TEXT_BUBBLE_PERMANENT 0x0002

typedef enum TbPosition {
    TB_POS_INVALID = -1,
    TB_POS_TOP,
    TB_POS_TOP_RIGHT,
    TB_POS_RIGHT,
    TB_POS_BOTTOM_RIGHT,
    TB_POS_BOTTOM,
    TB_POS_BOTTOM_LEFT,
    TB_POS_LEFT,
    TB_POS_TOP_LEFT,
} TbPosition;

typedef struct TextBubble {
    /* 0000 */ unsigned int flags;
    /* 0008 */ int64_t obj;
    /* 0010 */ tig_timestamp_t timestamp;
    /* 0014 */ int duration;
    /* 0018 */ TigVideoBuffer* video_buffer;
    /* 001C */ TigRect rect;
    /* 002C */ TbPosition pos;
} TextBubble;

static_assert(sizeof(TextBubble) == 0x30, "wrong size");

static void tb_remove_internal(TextBubble* tb);
static void tb_get_rect(TextBubble* tb, TigRect* rect);
static void tb_calc_rect(TextBubble* tb, int64_t loc, int offset_x, int offset_y, TigRect* rect);
static void tb_text_duration_changed();
static TextBubble* find_text_bubble(int64_t obj);
static TextBubble* find_free_text_bubble(int64_t obj);
static void adjust_text_bubble_rect(TigRect* rect, TbPosition pos);

// 0x5B8EA0
static uint8_t tb_colors[TB_TYPE_COUNT][3] = {
    { 255, 255, 255 },
    { 255, 0, 0 },
    { 0, 255, 0 },
    { 0, 0, 255 },
};

// 0x5B8EB0
static TigRect tb_content_rect = { 0, 0, 200, 200 };

#define TB_PLACEMENT_ATTEMPTS 8

// 0x5B8EC0
static int tb_placement_tbl[9][TB_PLACEMENT_ATTEMPTS] = {
    { TB_POS_TOP, TB_POS_TOP_LEFT, TB_POS_LEFT, TB_POS_BOTTOM_LEFT, TB_POS_BOTTOM, TB_POS_TOP_RIGHT, TB_POS_RIGHT, TB_POS_BOTTOM_RIGHT },
    { TB_POS_TOP, TB_POS_TOP_RIGHT, TB_POS_TOP_LEFT, TB_POS_RIGHT, TB_POS_LEFT, TB_POS_BOTTOM_RIGHT, TB_POS_BOTTOM_LEFT, TB_POS_BOTTOM },
    { TB_POS_TOP, TB_POS_TOP_RIGHT, TB_POS_RIGHT, TB_POS_BOTTOM_RIGHT, TB_POS_BOTTOM, TB_POS_TOP_LEFT, TB_POS_LEFT, TB_POS_BOTTOM_LEFT },
    { TB_POS_TOP_LEFT, TB_POS_LEFT, TB_POS_BOTTOM_LEFT, TB_POS_TOP, TB_POS_BOTTOM, TB_POS_TOP_RIGHT, TB_POS_BOTTOM_RIGHT, TB_POS_RIGHT },
    { TB_POS_TOP, TB_POS_TOP_RIGHT, TB_POS_RIGHT, TB_POS_BOTTOM_RIGHT, TB_POS_BOTTOM, TB_POS_BOTTOM_LEFT, TB_POS_LEFT, TB_POS_TOP_LEFT },
    { TB_POS_TOP_RIGHT, TB_POS_RIGHT, TB_POS_BOTTOM_RIGHT, TB_POS_TOP, TB_POS_BOTTOM, TB_POS_TOP_LEFT, TB_POS_BOTTOM_LEFT, TB_POS_LEFT },
    { TB_POS_TOP_LEFT, TB_POS_LEFT, TB_POS_BOTTOM_LEFT, TB_POS_BOTTOM, TB_POS_BOTTOM_RIGHT, TB_POS_RIGHT, TB_POS_TOP_RIGHT, TB_POS_TOP },
    { TB_POS_BOTTOM, TB_POS_BOTTOM_RIGHT, TB_POS_BOTTOM_LEFT, TB_POS_RIGHT, TB_POS_LEFT, TB_POS_TOP_RIGHT, TB_POS_TOP_LEFT, TB_POS_TOP },
    { TB_POS_TOP_RIGHT, TB_POS_RIGHT, TB_POS_BOTTOM_RIGHT, TB_POS_BOTTOM, TB_POS_BOTTOM_LEFT, TB_POS_LEFT, TB_POS_TOP_LEFT, TB_POS_TOP },
};

// 0x602920
static TigRect tb_iso_content_rect;

// 0x602930
static TextBubble tb_text_bubbles[MAX_TEXT_BUBBLES];

// 0x602AB0
static IsoInvalidateRectFunc* tb_iso_window_invalidate_rect;

// 0x602AB4
static bool tb_enabled;

// 0x602AB8
static int tb_text_duration;

// 0x602ABC
static tig_window_handle_t tb_iso_window_handle;

// 0x602AC0
static int dword_602AC0;

// 0x602AC4
static tig_color_t tb_background_color;

// 0x602AC8
static ViewOptions tb_view_options;

// 0x602AD0
static tig_font_handle_t tb_fonts[TB_TYPE_COUNT];

// 0x4D5B80
bool tb_init(GameInitInfo* init_info)
{
    TigWindowData window_data;
    TigVideoBufferCreateInfo vb_create_info;
    int idx;
    TigFont font;

    if (tig_window_data(init_info->iso_window_handle, &window_data) != TIG_OK) {
        return false;
    }

    tb_iso_window_handle = init_info->iso_window_handle;
    tb_iso_content_rect.x = 0;
    tb_iso_content_rect.y = 0;
    tb_iso_content_rect.width = window_data.rect.width;
    tb_iso_content_rect.height = window_data.rect.height;
    tb_iso_window_invalidate_rect = init_info->invalidate_rect_func;
    tb_view_options.type = VIEW_TYPE_ISOMETRIC;
    tb_enabled = true;
    tb_background_color = tig_color_make(0, 0, 255);

    vb_create_info.flags = TIG_VIDEO_BUFFER_CREATE_SYSTEM_MEMORY | TIG_VIDEO_BUFFER_CREATE_COLOR_KEY;
    vb_create_info.width = 200;
    vb_create_info.height = 200;
    vb_create_info.background_color = tb_background_color;
    vb_create_info.color_key = tb_background_color;

    dword_602AC0 = tig_color_make(0, 0, 0);

    for (idx = 0; idx < MAX_TEXT_BUBBLES; idx++) {
        if (tig_video_buffer_create(&vb_create_info, &(tb_text_bubbles[idx].video_buffer)) != TIG_OK) {
            while (--idx >= 0) {
                tig_video_buffer_destroy(tb_text_bubbles[idx].video_buffer);
            }
            return false;
        }
    }

    font.flags = TIG_FONT_NO_ALPHA_BLEND | TIG_FONT_CENTERED | TIG_FONT_SHADOW;
    tig_art_interface_id_create(229, 0, 0, 0, &(font.art_id));

    for (idx = 0; idx < TB_TYPE_COUNT; idx++) {
        font.color = tig_color_make(tb_colors[idx][0], tb_colors[idx][1], tb_colors[idx][2]);
        tig_font_create(&font, &(tb_fonts[idx]));
    }

    settings_add(&settings, TEXT_DURATION_KEY, "6", tb_text_duration_changed);
    tb_text_duration_changed();

    return true;
}

// 0x4D5DB0
void tb_reset()
{
    tb_clear();
}

// 0x4D5DC0
void tb_exit()
{
    int idx;

    tb_clear();

    for (idx = 0; idx < TB_TYPE_COUNT; idx++) {
        tig_font_destroy(tb_fonts[idx]);
    }

    for (idx = 0; idx < MAX_TEXT_BUBBLES; idx++) {
        tig_video_buffer_destroy(tb_text_bubbles[idx].video_buffer);
    }

    tb_iso_window_handle = TIG_WINDOW_HANDLE_INVALID;
    tb_iso_window_invalidate_rect = NULL;
}

// 0x4D5E20
void tb_resize(GameResizeInfo* resize_info)
{
    tb_iso_content_rect = resize_info->content_rect;
    tb_iso_window_handle = resize_info->window_handle;
}

// 0x4D5E60
bool tb_update_view(ViewOptions* view_options)
{
    tb_view_options = *view_options;
    return true;
}

// 0x4D5E80
void tb_close()
{
    tb_clear();
}

// 0x4D5E90
void tb_toggle()
{
    tb_enabled = !tb_enabled;
}

// 0x4D5EB0
void tb_ping(tig_timestamp_t timestamp)
{
    int idx;
    TigRect rect;

    for (idx = 0; idx < MAX_TEXT_BUBBLES; idx++) {
        if ((tb_text_bubbles[idx].flags & TEXT_BUBBLE_IN_USE) != 0
            && (tb_text_bubbles[idx].flags & TEXT_BUBBLE_PERMANENT) == 0
            && tig_timer_between(tb_text_bubbles[idx].timestamp, timestamp) > tb_text_bubbles[idx].duration) {
            tb_get_rect(&(tb_text_bubbles[idx]), &rect);
            tb_iso_window_invalidate_rect(&rect);
            tb_remove_internal(&(tb_text_bubbles[idx]));
        }
    }
}

// 0x4D5F10
void tb_draw(UnknownContext* render_info)
{
    int idx;
    TigRect rect;
    TigRectListNode* node;
    TigRect intersection;
    TigRect src_rect;

    if (!tb_enabled) {
        return;
    }

    if (tb_view_options.type != VIEW_TYPE_ISOMETRIC) {
        return;
    }

    for (idx = 0; idx < MAX_TEXT_BUBBLES; idx++) {
        if ((tb_text_bubbles[idx].flags & TEXT_BUBBLE_IN_USE) != 0) {
            tb_get_rect(&(tb_text_bubbles[idx]), &rect);

            node = *render_info->rects;
            while (node != NULL) {
                if (tig_rect_intersection(&rect, &(node->rect), &intersection) == TIG_OK) {
                    src_rect.x = intersection.x + tb_text_bubbles[idx].rect.x - rect.x;
                    src_rect.y = intersection.y + tb_text_bubbles[idx].rect.y - rect.y;
                    src_rect.width = intersection.width;
                    src_rect.height = intersection.height;
                    tig_window_copy_from_vbuffer(tb_iso_window_handle, &intersection, tb_text_bubbles[idx].video_buffer, &src_rect);
                }
                node = node->next;
            }
        }
    }
}

// 0x4D5FE0
void tb_add(int64_t obj, int type, const char* str)
{
    TextBubble* tb;
    TigRect dirty_rect;

    tb = find_free_text_bubble(obj);
    if (tb == NULL) {
        return;
    }

    tig_video_buffer_fill(tb->video_buffer, &tb_content_rect, tb_background_color);
    tig_font_push(tb_fonts[type]);
    tig_font_write(tb->video_buffer, str, &tb_content_rect, &dirty_rect);
    tig_font_pop();

    tb->timestamp = gamelib_ping_time;
    tb->duration = tb_text_duration;
    tb->flags = TEXT_BUBBLE_IN_USE;
    tb->obj = obj;
    tb->rect = dirty_rect;
    tb->pos = TB_POS_INVALID;

    object_flags_set(obj, OF_TEXT);

    tb_get_rect(tb, &dirty_rect);
    tb_iso_window_invalidate_rect(&dirty_rect);
}

// 0x4D6160
void tb_expire_in(int64_t obj, int seconds)
{
    TextBubble* tb;

    tb = find_text_bubble(obj);
    if (tb == NULL) {
        return;
    }

    if (seconds == TB_EXPIRE_NEVER) {
        tb->flags |= TEXT_BUBBLE_PERMANENT;
    } else {
        tb->flags &= ~TEXT_BUBBLE_PERMANENT;
        tb->timestamp = gamelib_ping_time;
        tb->duration = (seconds >= 0) ? 1000 * seconds : tb_text_duration;
    }
}

// 0x4D6210
void tb_notify_moved(int64_t obj, int64_t loc, int offset_x, int offset_y)
{
    TextBubble* tb;
    TigRect rect;
    TigRect new_rect;

    tb = find_text_bubble(obj);
    if (tb == NULL) {
        return;
    }

    tb_get_rect(tb, &rect);
    tb_calc_rect(tb, loc, offset_x, offset_y, &new_rect);

    tig_rect_union(&rect, &new_rect, &rect);
    tb_iso_window_invalidate_rect(&rect);
}

// 0x4D62B0
void tb_remove(int64_t obj)
{
    TextBubble* tb;
    unsigned int flags;

    tb = find_text_bubble(obj);
    if (tb != NULL) {
        tb_remove_internal(tb);
    } else {
        flags = obj_field_int32_get(obj, OBJ_F_FLAGS);
        flags &= ~OF_TEXT;
        obj_field_int32_set(obj, OBJ_F_FLAGS, flags);
    }
}

// 0x4D6320
void tb_clear()
{
    int idx;

    for (idx = 0; idx < MAX_TEXT_BUBBLES; idx++) {
        if ((tb_text_bubbles[idx].flags & TEXT_BUBBLE_IN_USE) != 0) {
            tb_remove_internal(&(tb_text_bubbles[idx]));
        }
    }
}

// 0x4D6350
static void tb_remove_internal(TextBubble* tb)
{
    TigRect rect;
    unsigned int flags;

    tb_get_rect(tb, &rect);
    tb_iso_window_invalidate_rect(&rect);

    flags = obj_field_int32_get(tb->obj, OBJ_F_FLAGS);
    flags &= ~OF_TEXT;
    obj_field_int32_set(tb->obj, OBJ_F_FLAGS, flags);

    tb->flags = 0;
    tb->obj = OBJ_HANDLE_NULL;
}

// 0x4D63B0
static void tb_get_rect(TextBubble* tb, TigRect* rect)
{
    int64_t loc;
    int offset_x;
    int offset_y;

    loc = obj_field_int64_get(tb->obj, OBJ_F_LOCATION);
    offset_x = obj_field_int32_get(tb->obj, OBJ_F_OFFSET_X);
    offset_y = obj_field_int32_get(tb->obj, OBJ_F_OFFSET_Y);
    tb_calc_rect(tb, loc, offset_x, offset_y, rect);
}

// 0x4D6410
static void tb_calc_rect(TextBubble* tb, int64_t loc, int offset_x, int offset_y, TigRect* rect)
{
    int64_t x;
    int64_t y;
    int quad;
    int attempt;

    location_xy(loc, &x, &y);

    x += offset_x + 40;
    y += offset_y + 20;

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

    rect->width = tb->rect.width;
    rect->height = tb->rect.height;

    if (tb->pos == TB_POS_INVALID) {
        tb->pos = TB_POS_TOP;

        quad = 0;
        if (x >= 380) {
            if (x >= 420) {
                quad += 2;
            } else {
                quad += 1;
            }
        }

        if (y >= 190) {
            if (y >= 290) {
                quad += 6;
            } else {
                quad += 3;
            }
        }

        for (attempt = 0; attempt < TB_PLACEMENT_ATTEMPTS; attempt++) {
            rect->x = (int)x;
            rect->y = (int)y;

            adjust_text_bubble_rect(rect, tb_placement_tbl[quad][attempt]);

            if (rect->x >= tb_iso_content_rect.x
                && rect->y >= tb_iso_content_rect.y
                && rect->x + rect->width <= tb_iso_content_rect.x + tb_iso_content_rect.width
                && rect->y + rect->height <= tb_iso_content_rect.y + tb_iso_content_rect.height) {
                tb->pos = tb_placement_tbl[quad][attempt];
                break;
            }
        }
    }

    rect->x = (int)x;
    rect->y = (int)y;

    adjust_text_bubble_rect(rect, tb->pos);
}

// 0x4D67F0
static void tb_text_duration_changed()
{
    int index;

    tb_text_duration = settings_get_value(&settings, TEXT_DURATION_KEY) * 125 * 8;

    for (index = 0; index < MAX_TEXT_BUBBLES; index++) {
        if ((tb_text_bubbles[index].flags & TEXT_BUBBLE_IN_USE) != 0
            && (tb_text_bubbles[index].flags & TEXT_BUBBLE_PERMANENT) == 0) {
            tb_text_bubbles[index].duration = 1000 * tb_text_duration;
        }
    }
}

TextBubble* find_text_bubble(int64_t obj)
{
    int idx;

    for (idx = 0; idx < MAX_TEXT_BUBBLES; idx++) {
        if ((tb_text_bubbles[idx].flags & TEXT_BUBBLE_IN_USE) != 0
            && tb_text_bubbles[idx].obj == obj) {
            return &(tb_text_bubbles[idx]);
        }
    }

    return NULL;
}

TextBubble* find_free_text_bubble(int64_t obj)
{
    int idx;
    int idx_to_remove = -1;
    int idx_to_use = -1;

    for (idx = 0; idx < MAX_TEXT_BUBBLES; idx++) {
        if ((tb_text_bubbles[idx].flags & TEXT_BUBBLE_IN_USE) != 0) {
            if (tb_text_bubbles[idx].obj != OBJ_HANDLE_NULL
                && tb_text_bubbles[idx].obj == obj) {
                idx_to_remove = idx;
                idx_to_use = -1;
                break;
            }

            if (idx_to_remove == -1
                || ((tb_text_bubbles[idx].flags & TEXT_BUBBLE_PERMANENT) == 0
                    && tb_text_bubbles[idx].timestamp < tb_text_bubbles[idx_to_remove].timestamp)) {
                idx_to_remove = idx;
            }
        } else {
            idx_to_use = idx;
        }
    }

    if (idx_to_use == -1) {
        if (idx_to_remove == -1) {
            return NULL;
        }

        tb_remove_internal(&(tb_text_bubbles[idx_to_remove]));
        idx_to_use = idx_to_remove;
    }

    return &(tb_text_bubbles[idx_to_use]);
}

void adjust_text_bubble_rect(TigRect* rect, TbPosition pos)
{
    switch (pos) {
    case TB_POS_TOP:
        rect->x -= rect->width / 2;
        rect->y -= rect->height + 100;
        break;
    case TB_POS_TOP_RIGHT:
        rect->x += 40;
        rect->y -= rect->height / 2 + 55;
        break;
    case TB_POS_RIGHT:
        rect->x += 80;
        rect->y -= rect->height / 2;
        break;
    case TB_POS_BOTTOM_RIGHT:
        rect->x += 40;
        rect->y += rect->height / 2 + 20;
        break;
    case TB_POS_BOTTOM:
        rect->x -= rect->width / 2;
        rect->y += 10;
        break;
    case TB_POS_BOTTOM_LEFT:
        rect->x -= rect->width + 40;
        rect->y += rect->height / 2 + 20;
        break;
    case TB_POS_LEFT:
        rect->x -= rect->width + 80;
        rect->y -= rect->height / 2;
        break;
    case TB_POS_TOP_LEFT:
        rect->x -= rect->width + 40;
        rect->y -= rect->height / 2 + 55;
        break;
    }
}
