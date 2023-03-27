#include "game/lib/tb.h"

#include "game/lib/gamelib.h"
#include "game/lib/settings.h"
#include "tig/art.h"
#include "tig/color.h"
#include "tig/font.h"
#include "tig/rect.h"
#include "tig/timer.h"
#include "tig/video.h"
#include "tig/window.h"

#define TEXT_DURATION_KEY "text duration"
#define FOUR 4
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
    int field_1C;
    int field_20;
    int field_24;
    int field_28;
    int field_2C;
};

static_assert(sizeof(S602930) == 0x30, "wrong size");

static void sub_4D6350(S602930* a1);
static void sub_4D63B0(S602930* a1, TigRect* rect);
static void sub_4D6410(S602930* a1, long long location, int offset_x, int offset_y, TigRect* rect);
static void tb_text_duration_changed();

// 0x5B8EA0
static uint8_t byte_5B8EA0[FOUR][3] = {
    { 255, 255, 255 },
    { 255, 0, 0 },
    { 0, 255, 0 },
    { 0, 0, 255 },
};

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
static int dword_602ABC;

// 0x602AC0
static int dword_602AC0;

// 0x602AC4
static int dword_602AC4;

// 0x602AC8
static ViewOptions tb_view_options;

// 0x602AD0
static TigFont* dword_602AD0[FOUR];

// 0x4D5B80
bool tb_init(GameContext* ctx)
{
    TigWindowData window_data;
    if (tig_window_data(ctx->iso_window_handle, &window_data) != TIG_OK) {
        return false;
    }

    dword_602ABC = ctx->iso_window_handle;
    stru_602920.x = 0;
    stru_602920.y = 0;
    stru_602920.width = window_data.rect.width;
    stru_602920.height = window_data.rect.height;
    dword_602AB0 = ctx->field_8;
    tb_view_options.type = VIEW_TYPE_ISOMETRIC;
    dword_602AB4 = 1;
    dword_602AC4 = tig_color_rgb_make(0, 0, 255);

    TigVideoBufferSpec video_buffer_spec;
    video_buffer_spec.flags = TIG_VIDEO_BUFFER_SPEC_SYSTEM_MEMORY | TIG_VIDEO_BUFFER_SPEC_TRANSPARENCY_ENABLED;
    video_buffer_spec.width = 200;
    video_buffer_spec.height = 200;
    video_buffer_spec.background_color = dword_602AC4;
    video_buffer_spec.color_key = dword_602AC4;

    dword_602AC0 = tig_color_rgb_make(0, 0, 0);

    for (int index = 0; index < EIGHT; index++) {
        if (tig_video_buffer_create(&video_buffer_spec, &(stru_602930[index].video_buffer)) != TIG_OK) {
            while (--index >= 0) {
                tig_video_buffer_destroy(stru_602930[index].video_buffer);
            }
            return false;
        }
    }

    TigFont font;
    font.flags = TIG_FONT_FLAG_0x80 | TIG_FONT_FLAG_0x10 | TIG_FONT_FLAG_0x08;
    tig_art_interface_id_create(229, 0, 0, 0, &(font.art_id));

    for (int index = 0; index < FOUR; index++) {
        font.field_8 = tig_color_rgb_make(byte_5B8EA0[index][0], byte_5B8EA0[index][1], byte_5B8EA0[index][2]);
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
    sub_4D6320();

    for (int index = 0; index < FOUR; index++) {
        tig_font_destroy(dword_602AD0[index]);
    }

    for (int index = 0; index < EIGHT; index++) {
        tig_video_buffer_destroy(stru_602930[index].video_buffer);
    }

    dword_602ABC = -1;
    dword_602AB0 = NULL;
}

// 0x4D5E20
void tb_resize(ResizeContext* ctx)
{
    stru_602920 = ctx->field_14;
    dword_602ABC = ctx->iso_window_handle;
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
    for (int index = 0; index < EIGHT; index++) {
        if ((stru_602930[index].flags & S602930_FLAG_0x1) != 0 && (stru_602930[index].flags & S602930_FLAG_0x2) == 0) {
            if (tig_timer_between(stru_602930[index].time, time) > stru_602930[index].duration) {
                TigRect rect;
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
    if (dword_602AB4) {
        if (tb_view_options.type == VIEW_TYPE_ISOMETRIC) {
            for (int index = 0; index < EIGHT; index++) {
                if ((stru_602930[index].flags & S602930_FLAG_0x1) != 0) {
                    TigRect rect;
                    sub_4D63B0(&(stru_602930[index]), &rect);

                    TigRectListNode* node = ctx->rects;
                    while (node != NULL) {
                        TigRect intersection;
                        if (tig_rect_intersection(&rect, &(node->rect), &intersection) == TIG_OK) {
                            TigRect src_rect;
                            src_rect.x = intersection.x + stru_602930[index].field_1C - rect.x;
                            src_rect.y = intersection.y + stru_602930[index].field_20 - rect.y;
                            src_rect.width = intersection.width;
                            src_rect.height = intersection.height;
                            tig_window_copy_from_vbuffer(dword_602ABC, &intersection, stru_602930[index].video_buffer, &src_rect);
                        }
                        node = node->next;
                    }
                }
            }
        }
    }
}

// 0x4D5FE0
void sub_4D5FE0(object_id_t object_id, int font, const char* text)
{
    // TODO: Incomplete.
}

// 0x4D6160
void sub_4D6160(object_id_t object_id, int a2)
{
    for (int index = 0; index < EIGHT; index++) {
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
    for (int index = 0; index < EIGHT; index++) {
        if ((stru_602930[index].flags & S602930_FLAG_0x1) != 0) {
            if (stru_602930[index].object_id == object_id) {
                TigRect rect;
                sub_4D63B0(&(stru_602930[index]), &rect);

                TigRect temp_rect;
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
    for (int index = 0; index < EIGHT; index++) {
        if ((stru_602930[index].flags & S602930_FLAG_0x1) != 0) {
            if (stru_602930[index].object_id == object_id) {
                sub_4D6350(&(stru_602930[index]));
                return;
            }
        }
    }

    unsigned int flags = object_field_get(object_id, OBJ_F_FLAGS);
    flags &= ~OBJECT_FLAG_0x0008;
    object_field_set(object_id, OBJ_F_FLAGS, flags);
}

// 0x4D6320
void sub_4D6320()
{
    for (int index = 0; index < EIGHT; index++) {
        if ((stru_602930[index].flags & S602930_FLAG_0x1) != 0) {
            sub_4D6350(&(stru_602930[index]));
        }
    }
}

// 0x4D6350
static void sub_4D6350(S602930* a1)
{
    TigRect rect;
    sub_4D63B0(a1, &rect);
    dword_602AB0(&rect);

    unsigned int flags = object_field_get(a1->object_id, OBJ_F_FLAGS);
    flags &= ~OBJECT_FLAG_0x0008;
    object_field_set(a1->object_id, OBJ_F_FLAGS, flags);

    a1->flags = 0;
    a1->object_id = 0;
}

// 0x4D63B0
static void sub_4D63B0(S602930* a1, TigRect* rect)
{
    long long location = object_field_get_64(a1->object_id, OBJ_F_LOCATION);
    int offset_x = object_field_get(a1->object_id, OBJ_F_OFFSET_X);
    int offset_y = object_field_get(a1->object_id, OBJ_F_OFFSET_Y);
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
    tb_text_duration = settings_get_value(&settings, TEXT_DURATION_KEY) * 125 * 8;

    for (int index = 0; index < EIGHT; index++) {
        if ((stru_602930[index].flags & S602930_FLAG_0x1) != 0 && (stru_602930[index].flags & S602930_FLAG_0x2) == 0) {
            stru_602930[index].duration = 1000 * tb_text_duration;
        }
    }
}
