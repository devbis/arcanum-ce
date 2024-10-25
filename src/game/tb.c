#include "game/tb.h"

#include "game/gamelib.h"
#include "game/object.h"

#define TEXT_DURATION_KEY "text duration"
#define EIGHT 8

#define S602930_FLAG_0x1 0x1
#define S602930_FLAG_0x2 0x2

typedef struct S602930 {
    unsigned int flags;
    int field_4;
    object_id_t object_id;
    unsigned int time;
    int duration;
    TigVideoBuffer* video_buffer;
    TigRect rect;
    int field_2C;
} S602930;

static_assert(sizeof(S602930) == 0x30, "wrong size");

static void sub_4D6350(S602930* a1);
static void sub_4D63B0(S602930* a1, TigRect* rect);
static void sub_4D6410(S602930* a1, long long location, int offset_x, int offset_y, TigRect* rect);
static void tb_text_duration_changed();

// 0x5B8EA0
static uint8_t byte_5B8EA0[TB_TYPE_COUNT][3] = {
    { 255, 255, 255 },
    { 255, 0, 0 },
    { 0, 255, 0 },
    { 0, 0, 255 },
};

// 0x5B8EB0
static TigRect stru_5B8EB0 = { 0, 0, 200, 200 };

// 0x602920
static TigRect stru_602920;

// 0x602930
static S602930 stru_602930[EIGHT];

// 0x602AB0
static GameContextF8* dword_602AB0;

// 0x602AB4
static bool dword_602AB4;

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
static tig_font_handle_t dword_602AD0[TB_TYPE_COUNT];

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
    stru_602920.x = 0;
    stru_602920.y = 0;
    stru_602920.width = window_data.rect.width;
    stru_602920.height = window_data.rect.height;
    dword_602AB0 = init_info->field_8;
    tb_view_options.type = VIEW_TYPE_ISOMETRIC;
    dword_602AB4 = 1;
    dword_602AC4 = tig_color_make(0, 0, 255);

    vb_create_info.flags = TIG_VIDEO_BUFFER_CREATE_SYSTEM_MEMORY | TIG_VIDEO_BUFFER_CREATE_COLOR_KEY;
    vb_create_info.width = 200;
    vb_create_info.height = 200;
    vb_create_info.background_color = dword_602AC4;
    vb_create_info.color_key = dword_602AC4;

    dword_602AC0 = tig_color_make(0, 0, 0);

    for (index = 0; index < EIGHT; index++) {
        if (tig_video_buffer_create(&vb_create_info, &(stru_602930[index].video_buffer)) != TIG_OK) {
            while (--index >= 0) {
                tig_video_buffer_destroy(stru_602930[index].video_buffer);
            }
            return false;
        }
    }

    font.flags = TIG_FONT_BLEND_ALPHA_SRC | TIG_FONT_CENTERED | TIG_FONT_SHADOW;
    tig_art_interface_id_create(229, 0, 0, 0, &(font.art_id));

    for (index = 0; index < TB_TYPE_COUNT; index++) {
        font.color = tig_color_make(byte_5B8EA0[index][0], byte_5B8EA0[index][1], byte_5B8EA0[index][2]);
        tig_font_create(&font, &(dword_602AD0[index]));
    }

    settings_add(&settings, TEXT_DURATION_KEY, "6", tb_text_duration_changed);
    tb_text_duration_changed();

    return true;
}

// 0x4D5DB0
void tb_reset()
{
    sub_4D6320();
}

// 0x4D5DC0
void tb_exit()
{
    int index;

    sub_4D6320();

    for (index = 0; index < TB_TYPE_COUNT; index++) {
        tig_font_destroy(dword_602AD0[index]);
    }

    for (index = 0; index < EIGHT; index++) {
        tig_video_buffer_destroy(stru_602930[index].video_buffer);
    }

    tb_iso_window_handle = TIG_WINDOW_HANDLE_INVALID;
    dword_602AB0 = NULL;
}

// 0x4D5E20
void tb_resize(ResizeInfo* resize_info)
{
    stru_602920 = resize_info->field_14;
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
    sub_4D6320();
}

// 0x4D5E90
void sub_4D5E90()
{
    dword_602AB4 = 1 - dword_602AB4;
}

// 0x4D5EB0
void tb_ping(unsigned int time)
{
    int index;
    TigRect rect;

    for (index = 0; index < EIGHT; index++) {
        if ((stru_602930[index].flags & S602930_FLAG_0x1) != 0 && (stru_602930[index].flags & S602930_FLAG_0x2) == 0) {
            if (tig_timer_between(stru_602930[index].time, time) > stru_602930[index].duration) {
                sub_4D63B0(&(stru_602930[index]), &rect);
                dword_602AB0(&rect);
                sub_4D6350(&(stru_602930[index]));
            }
        }
    }
}

// 0x4D5F10
void sub_4D5F10(UnknownContext* ctx)
{
    int index;
    TigRect rect;
    TigRectListNode* node;
    TigRect intersection;
    TigRect src_rect;

    if (dword_602AB4) {
        if (tb_view_options.type == VIEW_TYPE_ISOMETRIC) {
            for (index = 0; index < EIGHT; index++) {
                if ((stru_602930[index].flags & S602930_FLAG_0x1) != 0) {
                    sub_4D63B0(&(stru_602930[index]), &rect);

                    node = *ctx->rects;
                    while (node != NULL) {
                        if (tig_rect_intersection(&rect, &(node->rect), &intersection) == TIG_OK) {
                            src_rect.x = intersection.x + stru_602930[index].rect.x - rect.x;
                            src_rect.y = intersection.y + stru_602930[index].rect.y - rect.y;
                            src_rect.width = intersection.width;
                            src_rect.height = intersection.height;
                            tig_window_copy_from_vbuffer(tb_iso_window_handle, &intersection, stru_602930[index].video_buffer, &src_rect);
                        }
                        node = node->next;
                    }
                }
            }
        }
    }
}

// 0x4D5FE0
void sub_4D5FE0(int64_t obj, int type, const char* str)
{
    int index;
    int v1 = -1;
    int v2 = -1;
    TigRect dirty_rect;

    for (index = 0; index < 8; index++) {
        if ((stru_602930[index].flags & 0x1) != 0) {
            if (stru_602930[index].object_id != OBJ_HANDLE_NULL
                && stru_602930[index].object_id == obj) {
                v1 = index;
                break;
            }

            if (v1 == -1
                || ((stru_602930[index].flags & 0x2) == 0
                    && stru_602930[index].time < stru_602930[index - 1].time)) {
                v1 = index;
            }
        } else {
            v2 = index;
        }
    }

    if (v2 == -1) {
        sub_4D6350(&(stru_602930[v1]));
        v2 = v1;
    }

    tig_video_buffer_fill(stru_602930[v2].video_buffer, &stru_5B8EB0, dword_602AC4);
    tig_font_push(dword_602AD0[type]);
    tig_font_write(stru_602930[v2].video_buffer, str, &stru_5B8EB0, &dirty_rect);
    tig_font_pop();

    stru_602930[v2].time = gamelib_ping_time;
    stru_602930[v2].duration = tb_text_duration;
    stru_602930[v2].flags = 1;
    stru_602930[v2].object_id = obj;
    stru_602930[v2].rect = dirty_rect;
    stru_602930[v2].field_2C = -1;
    sub_43D0E0(obj, OF_TEXT);
    sub_4D63B0(&(stru_602930[v2]), &dirty_rect);
    dword_602AB0(&dirty_rect);
}

// 0x4D6160
void sub_4D6160(object_id_t object_id, int a2)
{
    int index;

    for (index = 0; index < EIGHT; index++) {
        if ((stru_602930[index].flags & S602930_FLAG_0x1) != 0) {
            if (stru_602930[index].object_id == object_id) {
                if (a2 == -2) {
                    stru_602930[index].flags |= S602930_FLAG_0x2;
                } else {
                    stru_602930[index].flags &= ~S602930_FLAG_0x2;
                    stru_602930[index].time = gamelib_ping_time;
                    stru_602930[index].duration = (a2 >= 0) ? 1000 * a2 : tb_text_duration;
                }
                return;
            }
        }
    }
}

// 0x4D6210
void sub_4D6210(object_id_t object_id, long long location, int offset_x, int offset_y)
{
    int index;
    TigRect rect;
    TigRect temp_rect;

    for (index = 0; index < EIGHT; index++) {
        if ((stru_602930[index].flags & S602930_FLAG_0x1) != 0) {
            if (stru_602930[index].object_id == object_id) {
                sub_4D63B0(&(stru_602930[index]), &rect);

                sub_4D6410(&(stru_602930[index]), location, offset_x, offset_y, &temp_rect);

                tig_rect_union(&rect, &temp_rect, &rect);
                dword_602AB0(&rect);

                return;
            }
        }
    }
}

// 0x4D62B0
void sub_4D62B0(object_id_t object_id)
{
    int index;
    unsigned int flags;

    for (index = 0; index < EIGHT; index++) {
        if ((stru_602930[index].flags & S602930_FLAG_0x1) != 0) {
            if (stru_602930[index].object_id == object_id) {
                sub_4D6350(&(stru_602930[index]));
                return;
            }
        }
    }

    flags = obj_field_int32_get(object_id, OBJ_F_FLAGS);
    flags &= ~OF_TEXT;
    obj_field_int32_set(object_id, OBJ_F_FLAGS, flags);
}

// 0x4D6320
void sub_4D6320()
{
    int index;

    for (index = 0; index < EIGHT; index++) {
        if ((stru_602930[index].flags & S602930_FLAG_0x1) != 0) {
            sub_4D6350(&(stru_602930[index]));
        }
    }
}

// 0x4D6350
static void sub_4D6350(S602930* a1)
{
    TigRect rect;
    unsigned int flags;

    sub_4D63B0(a1, &rect);
    dword_602AB0(&rect);

     flags = obj_field_int32_get(a1->object_id, OBJ_F_FLAGS);
    flags &= ~OF_TEXT;
    obj_field_int32_set(a1->object_id, OBJ_F_FLAGS, flags);

    a1->flags = 0;
    a1->object_id = OBJ_HANDLE_NULL;
}

// 0x4D63B0
static void sub_4D63B0(S602930* a1, TigRect* rect)
{
    int64_t location;
    int offset_x;
    int offset_y;

    location = obj_field_int64_get(a1->object_id, OBJ_F_LOCATION);
    offset_x = obj_field_int32_get(a1->object_id, OBJ_F_OFFSET_X);
    offset_y = obj_field_int32_get(a1->object_id, OBJ_F_OFFSET_Y);
    sub_4D6410(a1, location, offset_x, offset_y, rect);
}

// 0x4D6410
static void sub_4D6410(S602930* a1, long long location, int offset_x, int offset_y, TigRect* rect)
{
    // TODO: Incomplete.
}

// 0x4D67F0
static void tb_text_duration_changed()
{
    int index;

    tb_text_duration = settings_get_value(&settings, TEXT_DURATION_KEY) * 125 * 8;

    for (index = 0; index < EIGHT; index++) {
        if ((stru_602930[index].flags & S602930_FLAG_0x1) != 0 && (stru_602930[index].flags & S602930_FLAG_0x2) == 0) {
            stru_602930[index].duration = 1000 * tb_text_duration;
        }
    }
}
