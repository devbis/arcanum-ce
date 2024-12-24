#include "game/tb.h"

#include "game/gamelib.h"
#include "game/object.h"

#define TEXT_DURATION_KEY "text duration"
#define MAX_TEXT_BLOCKS 8

#define TEXT_BLOCK_IN_USE 0x0001
#define TEXT_BLOCK_PERMANENT 0x0002

typedef struct TextBlock {
    /* 0000 */ unsigned int flags;
    /* 0008 */ int64_t obj;
    /* 0010 */ unsigned int time;
    /* 0014 */ int duration;
    /* 0018 */ TigVideoBuffer* video_buffer;
    /* 001C */ TigRect rect;
    /* 002C */ int field_2C;
} TextBlock;

static_assert(sizeof(TextBlock) == 0x30, "wrong size");

static void sub_4D6350(TextBlock* tb);
static void sub_4D63B0(TextBlock* tb, TigRect* rect);
static void sub_4D6410(TextBlock* tb, long long location, int offset_x, int offset_y, TigRect* rect);
static void tb_text_duration_changed();
static TextBlock* find_text_block(int64_t obj);

// 0x5B8EA0
static uint8_t tb_colors[TB_TYPE_COUNT][3] = {
    { 255, 255, 255 },
    { 255, 0, 0 },
    { 0, 255, 0 },
    { 0, 0, 255 },
};

// 0x5B8EB0
static TigRect stru_5B8EB0 = { 0, 0, 200, 200 };

// 0x5B8EC0
static int dword_5B8EC0[9][8] = {
    { 0, 7, 6, 5, 4, 1, 2, 3 },
    { 0, 1, 7, 2, 6, 3, 5, 4 },
    { 0, 1, 2, 3, 4, 7, 6, 5 },
    { 7, 6, 5, 0, 4, 1, 3, 2 },
    { 0, 1, 2, 3, 4, 5, 6, 7 },
    { 1, 2, 3, 0, 4, 7, 5, 6 },
    { 7, 6, 5, 4, 3, 2, 1, 0 },
    { 4, 3, 5, 2, 6, 1, 7, 0 },
    { 1, 2, 3, 4, 5, 6, 7, 0 },
};

// 0x602920
static TigRect tb_iso_window_bounds;

// 0x602930
static TextBlock tb_text_blocks[MAX_TEXT_BLOCKS];

// 0x602AB0
static GameContextF8* tb_iso_window_invalidate_rect;

// 0x602AB4
static bool tb_enabled;

// 0x602AB8
static int tb_text_duration;

// 0x602ABC
static tig_window_handle_t tb_iso_window_handle;

// 0x602AC0
static int dword_602AC0;

// 0x602AC4
static int dword_602AC4;

// 0x602AC8
static ViewOptions tb_view_options;

// 0x602AD0
static tig_font_handle_t tb_fonts[TB_TYPE_COUNT];

// 0x4D5B80
bool tb_init(GameInitInfo* init_info)
{
    TigWindowData window_data;
    TigVideoBufferCreateInfo vb_create_info;
    int index;
    TigFont font;

    if (tig_window_data(init_info->iso_window_handle, &window_data) != TIG_OK) {
        return false;
    }

    tb_iso_window_handle = init_info->iso_window_handle;
    tb_iso_window_bounds.x = 0;
    tb_iso_window_bounds.y = 0;
    tb_iso_window_bounds.width = window_data.rect.width;
    tb_iso_window_bounds.height = window_data.rect.height;
    tb_iso_window_invalidate_rect = init_info->field_8;
    tb_view_options.type = VIEW_TYPE_ISOMETRIC;
    tb_enabled = true;
    dword_602AC4 = tig_color_make(0, 0, 255);

    vb_create_info.flags = TIG_VIDEO_BUFFER_CREATE_SYSTEM_MEMORY | TIG_VIDEO_BUFFER_CREATE_COLOR_KEY;
    vb_create_info.width = 200;
    vb_create_info.height = 200;
    vb_create_info.background_color = dword_602AC4;
    vb_create_info.color_key = dword_602AC4;

    dword_602AC0 = tig_color_make(0, 0, 0);

    for (index = 0; index < MAX_TEXT_BLOCKS; index++) {
        if (tig_video_buffer_create(&vb_create_info, &(tb_text_blocks[index].video_buffer)) != TIG_OK) {
            while (--index >= 0) {
                tig_video_buffer_destroy(tb_text_blocks[index].video_buffer);
            }
            return false;
        }
    }

    font.flags = TIG_FONT_NO_ALPHA_BLEND | TIG_FONT_CENTERED | TIG_FONT_SHADOW;
    tig_art_interface_id_create(229, 0, 0, 0, &(font.art_id));

    for (index = 0; index < TB_TYPE_COUNT; index++) {
        font.color = tig_color_make(tb_colors[index][0], tb_colors[index][1], tb_colors[index][2]);
        tig_font_create(&font, &(tb_fonts[index]));
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
    int index;

    tb_clear();

    for (index = 0; index < TB_TYPE_COUNT; index++) {
        tig_font_destroy(tb_fonts[index]);
    }

    for (index = 0; index < MAX_TEXT_BLOCKS; index++) {
        tig_video_buffer_destroy(tb_text_blocks[index].video_buffer);
    }

    tb_iso_window_handle = TIG_WINDOW_HANDLE_INVALID;
    tb_iso_window_invalidate_rect = NULL;
}

// 0x4D5E20
void tb_resize(ResizeInfo* resize_info)
{
    tb_iso_window_bounds = resize_info->field_14;
    tb_iso_window_handle = resize_info->iso_window_handle;
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
void tb_ping(unsigned int time)
{
    int index;
    TigRect rect;

    for (index = 0; index < MAX_TEXT_BLOCKS; index++) {
        if ((tb_text_blocks[index].flags & TEXT_BLOCK_IN_USE) != 0
            && (tb_text_blocks[index].flags & TEXT_BLOCK_PERMANENT) == 0) {
            if (tig_timer_between(tb_text_blocks[index].time, time) > tb_text_blocks[index].duration) {
                sub_4D63B0(&(tb_text_blocks[index]), &rect);
                tb_iso_window_invalidate_rect(&rect);
                sub_4D6350(&(tb_text_blocks[index]));
            }
        }
    }
}

// 0x4D5F10
void tb_render(UnknownContext* render_info)
{
    int index;
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

    for (index = 0; index < MAX_TEXT_BLOCKS; index++) {
        if ((tb_text_blocks[index].flags & TEXT_BLOCK_IN_USE) != 0) {
            sub_4D63B0(&(tb_text_blocks[index]), &rect);

            node = *render_info->rects;
            while (node != NULL) {
                if (tig_rect_intersection(&rect, &(node->rect), &intersection) == TIG_OK) {
                    src_rect.x = intersection.x + tb_text_blocks[index].rect.x - rect.x;
                    src_rect.y = intersection.y + tb_text_blocks[index].rect.y - rect.y;
                    src_rect.width = intersection.width;
                    src_rect.height = intersection.height;
                    tig_window_copy_from_vbuffer(tb_iso_window_handle, &intersection, tb_text_blocks[index].video_buffer, &src_rect);
                }
                node = node->next;
            }
        }
    }
}

// 0x4D5FE0
void tb_add(int64_t obj, int type, const char* str)
{
    int index;
    int v1 = -1;
    int v2 = -1;
    TigRect dirty_rect;

    for (index = 0; index < MAX_TEXT_BLOCKS; index++) {
        if ((tb_text_blocks[index].flags & TEXT_BLOCK_IN_USE) != 0) {
            if (tb_text_blocks[index].obj != OBJ_HANDLE_NULL
                && tb_text_blocks[index].obj == obj) {
                v1 = index;
                break;
            }

            if (v1 == -1
                || ((tb_text_blocks[index].flags & TEXT_BLOCK_PERMANENT) == 0
                    && tb_text_blocks[index].time < tb_text_blocks[index - 1].time)) {
                v1 = index;
            }
        } else {
            v2 = index;
        }
    }

    if (v2 == -1) {
        sub_4D6350(&(tb_text_blocks[v1]));
        v2 = v1;
    }

    tig_video_buffer_fill(tb_text_blocks[v2].video_buffer, &stru_5B8EB0, dword_602AC4);
    tig_font_push(tb_fonts[type]);
    tig_font_write(tb_text_blocks[v2].video_buffer, str, &stru_5B8EB0, &dirty_rect);
    tig_font_pop();

    tb_text_blocks[v2].time = gamelib_ping_time;
    tb_text_blocks[v2].duration = tb_text_duration;
    tb_text_blocks[v2].flags = TEXT_BLOCK_IN_USE;
    tb_text_blocks[v2].obj = obj;
    tb_text_blocks[v2].rect = dirty_rect;
    tb_text_blocks[v2].field_2C = -1;
    sub_43D0E0(obj, OF_TEXT);
    sub_4D63B0(&(tb_text_blocks[v2]), &dirty_rect);
    tb_iso_window_invalidate_rect(&dirty_rect);
}

// 0x4D6160
void tb_expire_in(int64_t obj, int seconds)
{
    TextBlock* tb;


    tb = find_text_block(obj);
    if (tb == NULL) {
        return;
    }

    if (seconds == TB_EXPIRE_NEVER) {
        tb->flags |= TEXT_BLOCK_PERMANENT;
    } else {
        tb->flags &= ~TEXT_BLOCK_PERMANENT;
        tb->time = gamelib_ping_time;
        tb->duration = (seconds >= 0) ? 1000 * seconds : tb_text_duration;
    }
}

// 0x4D6210
void tb_move(int64_t obj, int64_t loc, int offset_x, int offset_y)
{
    TextBlock* tb;
    TigRect rect;
    TigRect temp_rect;

    tb = find_text_block(obj);
    if (tb == NULL) {
        return;
    }

    sub_4D63B0(tb, &rect);

    sub_4D6410(tb, loc, offset_x, offset_y, &temp_rect);

    tig_rect_union(&rect, &temp_rect, &rect);
    tb_iso_window_invalidate_rect(&rect);
}

// 0x4D62B0
void tb_remove(int64_t obj)
{
    TextBlock* tb;
    unsigned int flags;

    tb = find_text_block(obj);
    if (tb != NULL) {
        sub_4D6350(tb);
    } else {
        flags = obj_field_int32_get(obj, OBJ_F_FLAGS);
        flags &= ~OF_TEXT;
        obj_field_int32_set(obj, OBJ_F_FLAGS, flags);
    }
}

// 0x4D6320
void tb_clear()
{
    int index;

    for (index = 0; index < MAX_TEXT_BLOCKS; index++) {
        if ((tb_text_blocks[index].flags & TEXT_BLOCK_IN_USE) != 0) {
            sub_4D6350(&(tb_text_blocks[index]));
        }
    }
}

// 0x4D6350
static void sub_4D6350(TextBlock* tb)
{
    TigRect rect;
    unsigned int flags;

    sub_4D63B0(tb, &rect);
    tb_iso_window_invalidate_rect(&rect);

    flags = obj_field_int32_get(tb->obj, OBJ_F_FLAGS);
    flags &= ~OF_TEXT;
    obj_field_int32_set(tb->obj, OBJ_F_FLAGS, flags);

    tb->flags = 0;
    tb->obj = OBJ_HANDLE_NULL;
}

// 0x4D63B0
static void sub_4D63B0(TextBlock* tb, TigRect* rect)
{
    int64_t location;
    int offset_x;
    int offset_y;

    location = obj_field_int64_get(tb->obj, OBJ_F_LOCATION);
    offset_x = obj_field_int32_get(tb->obj, OBJ_F_OFFSET_X);
    offset_y = obj_field_int32_get(tb->obj, OBJ_F_OFFSET_Y);
    sub_4D6410(tb, location, offset_x, offset_y, rect);
}

// 0x4D6410
static void sub_4D6410(TextBlock* tb, long long location, int offset_x, int offset_y, TigRect* rect)
{
    int64_t x;
    int64_t y;
    int v1;
    int index;

    sub_4B8680(location, &x, &y);

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

    if (tb->field_2C == -1) {
        tb->field_2C = 0;

        v1 = 0;
        if (x >= 380) {
            if (x >= 420) {
                v1 += 2;
            } else {
                v1 += 1;
            }
        }

        if (y >= 190) {
            if (y >= 290) {
                v1 += 6;
            } else {
                v1 += 3;
            }
        }

        for (index = 0; index < 8; index++) {
            rect->x = (int)x;
            rect->y = (int)y;

            switch (dword_5B8EC0[v1][index]) {
            case 0:
                rect->x -= rect->width / 2;
                rect->y -= rect->height + 100;
                break;
            case 1:
                rect->x += 40;
                rect->y -= rect->height / 2 + 55;
                break;
            case 2:
                rect->x += 80;
                rect->y -= rect->height / 2;
                break;
            case 3:
                rect->x += 40;
                rect->y += rect->height / 2 + 20;
                break;
            case 4:
                rect->x -= rect->width / 2;
                rect->y += 10;
                break;
            case 5:
                rect->x -= rect->width + 40;
                rect->y += rect->height / 2 + 20;
                break;
            case 6:
                rect->x -= rect->width + 80;
                rect->y -= rect->height / 2;
                break;
            case 7:
                rect->x -= rect->width + 40;
                rect->y -= rect->height / 2 + 55;
                break;
            }

            if (rect->x >= tb_iso_window_bounds.x
                && rect->y >= tb_iso_window_bounds.y
                && rect->x + rect->width <= tb_iso_window_bounds.x + tb_iso_window_bounds.width
                && rect->y + rect->height <= tb_iso_window_bounds.y + tb_iso_window_bounds.height) {
                break;
            }
        }

        tb->field_2C = dword_5B8EC0[v1][index];
    }

    rect->x = (int)x;
    rect->y = (int)y;

    switch (tb->field_2C) {
    case 0:
        rect->x -= rect->width / 2;
        rect->y -= rect->height + 100;
        break;
    case 1:
        rect->x += 40;
        rect->y -= rect->height / 2 + 55;
        break;
    case 2:
        rect->x += 80;
        rect->y -= rect->height / 2;
        break;
    case 3:
        rect->x += 40;
        rect->y += rect->height / 2 + 20;
        break;
    case 4:
        rect->x -= rect->width / 2;
        rect->y += 10;
        break;
    case 5:
        rect->x -= rect->width + 40;
        rect->y += rect->height / 2 + 20;
        break;
    case 6:
        rect->x -= rect->width + 80;
        rect->y -= rect->height / 2;
        break;
    case 7:
        rect->x -= rect->width + 40;
        rect->y -= rect->height / 2 + 55;
        break;
    }
}

// 0x4D67F0
static void tb_text_duration_changed()
{
    int index;

    tb_text_duration = settings_get_value(&settings, TEXT_DURATION_KEY) * 125 * 8;

    for (index = 0; index < MAX_TEXT_BLOCKS; index++) {
        if ((tb_text_blocks[index].flags & TEXT_BLOCK_IN_USE) != 0
            && (tb_text_blocks[index].flags & TEXT_BLOCK_PERMANENT) == 0) {
            tb_text_blocks[index].duration = 1000 * tb_text_duration;
        }
    }
}

TextBlock* find_text_block(int64_t obj)
{
    int idx;

    for (idx = 0; idx < MAX_TEXT_BLOCKS; idx++) {
        if ((tb_text_blocks[idx].flags & TEXT_BLOCK_IN_USE) != 0
            && tb_text_blocks[idx].obj == obj) {
            return &(tb_text_blocks[idx]);
        }
    }

    return NULL;
}
