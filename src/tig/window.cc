#include "tig/window.h"

#include "tig/art.h"
#include "tig/button.h"
#include "tig/color.h"
#include "tig/debug.h"
#include "tig/felicity.h"
#include "tig/rect.h"
#include "tig/video.h"

#define TIG_WINDOW_MAX 50
#define TIG_WINDOW_BUTTON_MAX 200
#define TIG_WINDOW_MODAL_BUTTON_MAX 2

#define TIG_WINDOW_USAGE_FLAG_0x01 0x01

typedef struct TigWindow {
    int field_0;
    int flags;
    TigRect rect;
    TigRect field_18;
    int field_28;
    int field_2C;
    TigVideoBuffer* video_buffer;
    // TODO: Figure out purpose and meaning.
    TigVideoBuffer* secondary_video_buffer;
    int buttons_count;
    int buttons[TIG_WINDOW_BUTTON_MAX];
    TigWindowMessageFilterFunc* message_filter_func;
};

static_assert(sizeof(TigWindow) == 0x360, "wrong size");

static int tig_window_free_index();
static int tig_window_handle_to_index(int window_handle);
static int tig_window_index_to_handle(int window_index);
static void push_window_stack(int window_handle);
static bool pop_window_stack(int window_handle);

// 0x5BED98
static int dword_5BED98;

// 0x604778
static TigWindow windows[TIG_WINDOW_MAX];

// 0x60F038
static int dword_60F038[TIG_WINDOW_MODAL_BUTTON_MAX];

// 0x60F040
static int dword_60F040;

// 0x60F044
static int dword_60F044[TIG_WINDOW_MAX];

// 0x60F10C
static int tig_window_ctx_flags;

// 0x60F110
static TigRect tig_window_screen_rect;

// 0x60F120
static int dword_60F120;

// 0x60F124
static bool tig_window_initialized;

// 0x60F128
static int tig_window_num_windows;

// 0x60F12C
static TigRectListNode* tig_window_dirty_rects;

// 0x60F130
static Felicity* off_60F130;

// 0x51CAD0
int tig_window_init(TigContext* ctx)
{
    tig_window_num_windows = 0;
    tig_window_dirty_rects = NULL;

    tig_window_screen_rect.x = 0;
    tig_window_screen_rect.y = 0;
    tig_window_screen_rect.width = ctx->width;
    tig_window_screen_rect.height = ctx->height;

    for (int index = 0; index < TIG_WINDOW_MAX; index++) {
        windows[index].field_0 = 1;
    }

    dword_60F120 = 1;
    tig_window_ctx_flags = ctx->flags;
    tig_window_initialized = true;

    return TIG_OK;
}

// 0x51CB30
void tig_window_exit()
{
    for (int index = 0; index < TIG_WINDOW_MAX; index++) {
        int window_handle = tig_window_index_to_handle(index);
        tig_window_destroy(window_handle);
    }

    while (tig_window_dirty_rects != NULL) {
        TigRectListNode* curr = tig_window_dirty_rects;
        tig_window_dirty_rects = curr->next;
        sub_52D180(curr);
    }

    tig_window_initialized = false;
}

// 0x51CB90
int tig_window_create(TigWindowData* window_data, int* window_handle)
{
    if (!tig_window_initialized) {
        return 1;
    }

    if (tig_window_num_windows >= TIG_WINDOW_MAX) {
        return 3;
    }

    int window_index = tig_window_free_index();
    TigWindow* window = &(windows[window_index]);

    window->flags = window_data->flags;

    if ((window_data->flags & TIG_WINDOW_FLAG_0x04) != 0) {
        window->flags |= TIG_WINDOW_FLAG_0x02;
    }

    window->message_filter_func = window_data->message_filter_func;
    window->rect.x = window_data->x;
    window->rect.y = window_data->y;
    window->rect.width = window_data->width;
    window->rect.height = window_data->height;
    window->field_18 = 0;
    window->field_1C = 0;
    window->field_20 = window_data->width;
    window->field_24 = window_data->height;
    window->field_28 = window_data->field_14;
    window->field_2C = window_data->field_18;
    window->buttons_count = 0;

    TigVideoBufferSpec video_buffer_spec;
    video_buffer_spec.flags = window_data->flags & TIG_WINDOW_FLAG_0x01;
    if ((window_data->flags & TIG_WINDOW_FLAG_0x10) != 0) {
        video_buffer_spec.flags |= TIG_VIDEO_BUFFER_SPEC_VIDEO_MEMORY;
    } else {
        video_buffer_spec.flags |= TIG_VIDEO_BUFFER_SPEC_SYSTEM_MEMORY;
    }

    if ((window_data->flags & TIG_WINDOW_FLAG_0x40) != 0) {
        video_buffer_spec.flags |= TIG_VIDEO_BUFFER_SPEC_FLUSH_ENABLED;
    }

    video_buffer_spec.width = window_data->width;
    video_buffer_spec.height = window_data->height;
    video_buffer_spec.background_color = window_data->field_14;
    video_buffer_spec.color_key = window_data->field_18;

    int rc = tig_video_buffer_create(&video_buffer_spec, &(window->video_buffer));
    if (rc != TIG_OK) {
        return rc;
    }

    if ((tig_window_ctx_flags & TIG_CONTEXT_FLAG_0x10) != 0) {
        if ((window_data->flags & TIG_WINDOW_FLAG_0x01) != 0) {
            video_buffer_spec.flags &= ~TIG_VIDEO_BUFFER_SPEC_TRANSPARENCY_ENABLED;

            rc = tig_video_buffer_create(&video_buffer_spec, &(window->secondary_video_buffer));
            if (rc != TIG_OK) {
                tig_video_buffer_destroy(window->video_buffer);
                return rc;
            }
        }
    }

    *window_handle = tig_window_index_to_handle(window_index);
    push_window_stack(*window_handle);

    if ((window->flags & TIG_WINDOW_FLAG_HIDDEN) == 0) {
        tig_window_set_needs_display_in_rect(&(window->rect));
    }

    window->field_0 = 0;

    return TIG_OK;
}

// 0x51CD30
int tig_window_destroy(int window_handle)
{
    if (window_handle == -1) {
        tig_debug_printf("tig_window_destroy: ERROR: Attempt to reference Empty WinID!\n");
        return 12;
    }

    if (!tig_window_initialized) {
        return 1;
    }

    int window_index = tig_window_handle_to_index(window_handle);
    TigWindow* window = &(windows[window_index]);

    int rc = tig_window_button_destroy(window_handle);
    if (rc != TIG_OK) {
        return rc;
    }

    if ((window->flags & TIG_WINDOW_FLAG_HIDDEN) == 0) {
        tig_window_set_needs_display_in_rect(&(window->rect));
    }

    tig_video_buffer_destroy(window->video_buffer);

    if ((tig_window_ctx_flags & 0x10) != 0 && (window->flags & TIG_WINDOW_FLAG_0x01) != 0) {
        tig_video_buffer_destroy(window->secondary_video_buffer);
    }

    pop_window_stack(window_handle);

    window->field_0 = 1;

    return TIG_OK;
}

// 0x51CDE0
int tig_window_button_destroy(int window_handle)
{
    if (window_handle) {
        tig_debug_printf("tig_window_button_destroy: ERROR: Attempt to reference Empty WinID!\n");
        return 12;
    }

    if (!tig_window_initialized) {
        return 1;
    }

    int window_index = tig_window_handle_to_index(window_handle);
    TigWindow* window = &(windows[window_index]);

    while (window->buttons_count > 0) {
        int rc = tig_button_destroy(window->buttons[0]);
        if (rc != TIG_OK) {
            return rc;
        }
    }

    return TIG_OK;
}

// 0x51CE50
int tig_window_message_filter_set(int window_handle, TigWindowMessageFilterFunc* func)
{
    if (window_handle == -1) {
        tig_debug_printf("tig_window_message_filter_set: ERROR: Attempt to reference Empty WinID!\n");
        return 12;
    }

    if (!tig_window_initialized) {
        return 1;
    }

    if (func == NULL) {
        return 12;
    }

    int window_index = tig_window_handle_to_index(window_handle);
    TigWindow* window = &(windows[window_index]);

    if ((window->flags & TIG_WINDOW_FLAG_0x02) == 0) {
        return 12;
    }

    window->message_filter_func = func;

    return TIG_OK;
}

// 0x51CEB0
int tig_window_data(int window_handle, TigWindowData* window_data)
{
    if (window_handle == -1) {
        tig_debug_printf("tig_window_data: ERROR: Attempt to reference Empty WinID!\n");
        return 12;
    }

    if (!tig_window_initialized) {
        return 1;
    }

    int window_index = tig_window_handle_to_index(window_handle);
    window_data->flags = windows[window_index].flags;
    window_data->x = windows[window_index].rect.x;
    window_data->y = windows[window_index].rect.y;
    window_data->width = windows[window_index].rect.width;
    window_data->height = windows[window_index].rect.height;
    window_data->field_14 = windows[window_index].field_28;
    window_data->field_18 = windows[window_index].field_2C;
    window_data->message_filter_func = windows[window_index].message_filter_func;

    return TIG_OK;
}

// 0x51CF40
int tig_window_display()
{
    // TODO: Incomplete.
}

// 0x51D050
void sub_51D050(TigRect* a1, int a2, TigVideoBuffer* a3, int a4, int a5, int a6)
{
    // TODO: Incomplete.
}

// 0x51D570
int tig_window_fill(int window_handle, TigRect* rect, int color)
{
    if (window_handle == -1) {
        tig_debug_printf("tig_window_fill: ERROR: Attempt to reference Empty WinID!\n");
        return TIG_ERR_12;
    }

    if (!tig_window_initialized) {
        return TIG_NOT_INITIALIZED;
    }

    int window_index = tig_window_handle_to_index(window_handle);

    TigRect normalized;
    if (rect != NULL) {
        normalized = *rect;
    } else {
        normalized.x = 0;
        normalized.y = 0;
        normalized.width = windows[window_index].rect.width;
        normalized.height = windows[window_index].rect.height;
    }

    TigRect clamped_normalized_rect;
    if (tig_rect_intersection(&normalized, &(windows[window_index].field_18), &clamped_normalized_rect) != TIG_OK) {
        return TIG_OK;
    }

    int rc = tig_video_buffer_fill(windows[window_index].video_buffer, &clamped_normalized_rect, color);
    if (rc != TIG_OK) {
        return rc;
    }

    clamped_normalized_rect.x += windows[window_index].rect.x;
    clamped_normalized_rect.y += windows[window_index].rect.y;
    if ((windows[window_index].flags & TIG_WINDOW_FLAG_HIDDEN) == 0) {
        tig_window_set_needs_display_in_rect(&clamped_normalized_rect);
        tig_button_refresh_rect(window_handle, &clamped_normalized_rect);
    }

    return TIG_OK;
}

// 0x51D6B0
int tig_window_line(int window_handle, int a2, int a3)
{
    // TODO: Incomplete.
}

// 0x51D7B0
int tig_window_box(int window_handle, TigRect* rect, int color)
{
    int rc;
    TigRect side_rect;

    if (window_handle == -1) {
        tig_debug_printf("tig_window_box: ERROR: Attempt to reference Empty WinID!\n");
        return 12;
    }

    side_rect.x = rect->x;
    side_rect.y = rect->y;
    side_rect.width = rect->width;
    side_rect.height = 1;
    rc = tig_window_fill(window_handle, &side_rect, color);
    if (rc != TIG_OK) {
        return rc;
    }

    side_rect.x = rect->x;
    side_rect.y = rect->y;
    side_rect.width = 1;
    side_rect.height = rect->height;
    rc = tig_window_fill(window_handle, &side_rect, color);
    if (rc != TIG_OK) {
        return rc;
    }

    side_rect.x = rect->x + rect->width - 1;
    side_rect.y = rect->y;
    side_rect.width = 1;
    side_rect.height = rect->height;
    rc = tig_window_fill(window_handle, &side_rect, color);
    if (rc != TIG_OK) {
        return rc;
    }

    side_rect.x = rect->x;
    side_rect.y = rect->y + rect->height - 1;
    side_rect.width = rect->width;
    side_rect.height = 1;
    return tig_window_fill(window_handle, &side_rect, color);
}

// 0x51D8D0
int sub_51D8D0(int a1)
{
    // TODO: Incomplete.
}

// 0x51DA80
int tig_window_blit_art(int window_handle, TigArtBlitSpec* blit_spec)
{
    if (window_handle == -1) {
        tig_debug_printf("tig_window_blit_art: ERROR: Attempt to reference Empty WinID!\n");
        return TIG_ERR_12;
    }

    int window_index = tig_window_handle_to_index(window_handle);
    blit_spec->dst_video_buffer = windows[window_index].video_buffer;

    int rc = tig_art_blit(blit_spec);
    if (rc != TIG_OK) {
        return rc;
    }

    TigRect rect = *(blit_spec->dst_rect);
    rect.x += windows[window_index].rect.x;
    rect.y += windows[window_index].rect.y;

    if ((windows[window_index].flags & TIG_WINDOW_FLAG_HIDDEN) == 0) {
        tig_window_set_needs_display_in_rect(&rect);
        tig_button_refresh_rect(window_handle, &rect);
    }
}

// 0x51DB40
int tig_window_scroll(int window_handle, int dx, int dy)
{
    if (window_handle == -1) {
        tig_debug_printf("tig_window_scroll: ERROR: Attempt to reference Empty WinID!\n");
        return TIG_ERR_12;
    }

    int window_index = tig_window_handle_to_index(window_handle);
    TigWindow* window = &(windows[window_index]);

    TigRect src_rect = window->field_18;
    TigRect dst_rect = window->field_18;

    if (dx < 0) {
        dst_rect.width += dx;
        src_rect.x -= dx;
        src_rect.width += dx;
    } else {
        dst_rect.x += dx;
        dst_rect.width -= dx;
        src_rect.width -= dx;
    }

    if (dy < 0) {
        dst_rect.height += dy;
        src_rect.y -= dy;
        src_rect.height += dy;
    } else {
        dst_rect.y += dy;
        dst_rect.height -= dy;
        src_rect.height -= dy;
    }

    TigVideoBufferBlitSpec blit_spec;
    blit_spec.field_0 = 0;
    blit_spec.src_video_buffer = window->video_buffer;
    blit_spec.src_rect = &src_rect;
    blit_spec.dst_video_buffer = window->video_buffer;
    blit_spec.dst_rect = &dst_rect;

    int rc = tig_video_buffer_blit(&blit_spec);
    if (rc != TIG_OK) {
        return rc;
    }

    if ((window->flags & TIG_WINDOW_FLAG_HIDDEN) == 0) {
        tig_window_set_needs_display_in_rect(&(window->rect));
    }

    return TIG_OK;
}

// 0x51DC90
int tig_window_scroll_rect(int window_handle, TigRect* rect, int dx, int dy)
{
    if (window_handle == -1) {
        tig_debug_printf("tig_window_scroll_rect: ERROR: Attempt to reference Empty WinID!\n");
        return TIG_ERR_12;
    }

    int window_index = tig_window_handle_to_index(window_handle);
    TigWindow* window = &(windows[window_index]);

    TigRect src_rect = *rect;
    TigRect dst_rect = *rect;

    TigVideoBufferBlitSpec blit_spec;
    blit_spec.field_0 = 0;
    blit_spec.src_video_buffer = window->video_buffer;
    blit_spec.src_rect = &src_rect;
    blit_spec.dst_video_buffer = window->video_buffer;
    blit_spec.dst_rect = &dst_rect;

    int rc = tig_video_buffer_blit(&blit_spec);
    if (rc != TIG_OK) {
        return rc;
    }

    if ((window->flags & TIG_WINDOW_FLAG_HIDDEN) == 0) {
        tig_window_set_needs_display_in_rect(&(window->rect));
    }

    return TIG_OK;
}

// 0x51DDC0
int tig_window_copy(int dst_window_handle, TigRect* dst_rect, int src_window_handle, TigRect* src_rect)
{
    if (dst_window_handle == -1 || src_window_handle == -1) {
        tig_debug_printf("tig_window_copy: ERROR: Attempt to reference Empty WinID!\n");
        return TIG_ERR_12;
    }

    int dst_window_index = tig_window_handle_to_index(dst_window_handle);
    int src_window_index = tig_window_handle_to_index(src_window_index);

    TigVideoBufferBlitSpec blit_spec;
    blit_spec.field_0 = 0;
    blit_spec.src_video_buffer = windows[src_window_index].video_buffer;
    blit_spec.src_rect = src_rect;
    blit_spec.dst_video_buffer = windows[dst_window_index].video_buffer;
    blit_spec.dst_rect = dst_rect;

    int rc = tig_video_buffer_blit(&blit_spec);
    if (rc != TIG_OK) {
        return rc;
    }

    if ((windows[dst_window_index].flags & TIG_WINDOW_FLAG_HIDDEN) == 0) {
        TigRect dirty_screen_rect = *dst_rect;
        dirty_screen_rect.x += windows[dst_window_index].rect.x;
        dirty_screen_rect.y = windows[dst_window_index].rect.y;
        tig_window_set_needs_display_in_rect(&dirty_screen_rect);
    }

    return TIG_OK;
}

// 0x51DEA0
int tig_window_copy_from_vbuffer(int dst_window_handle, TigRect* dst_rect, TigVideoBuffer* src_video_buffer, TigRect* src_rect)
{
    if (dst_window_handle == -1) {
        tig_debug_printf("tig_window_copy_from_vbuffer: ERROR: Attempt to reference Empty WinID!\n");
        return TIG_ERR_12;
    }

    int dst_window_index = tig_window_handle_to_index(dst_window_handle);

    TigVideoBufferBlitSpec blit_spec;
    blit_spec.field_0 = 0;
    blit_spec.src_video_buffer = src_video_buffer;
    blit_spec.src_rect = src_rect;
    blit_spec.dst_video_buffer = windows[dst_window_index].video_buffer;
    blit_spec.dst_rect = dst_rect;

    int rc = tig_video_buffer_blit(&blit_spec);
    if (rc != TIG_OK) {
        return rc;
    }

    if ((windows[dst_window_index].flags & TIG_WINDOW_FLAG_HIDDEN) == 0) {
        TigRect dirty_screen_rect = *dst_rect;
        dirty_screen_rect.x += windows[dst_window_index].rect.x;
        dirty_screen_rect.y = windows[dst_window_index].rect.y;
        tig_window_set_needs_display_in_rect(&dirty_screen_rect);
    }

    return TIG_OK;
}

// 0x51DF80
int tig_window_copy_to_vbuffer(int src_window_handle, TigRect* src_rect, TigVideoBuffer* dst_video_buffer, TigRect* dst_rect)
{
    if (src_window_handle == -1) {
        tig_debug_printf("tig_window_copy_to_vbuffer: ERROR: Attempt to reference Empty WinID!\n");
        return TIG_ERR_12;
    }

    int src_window_index = tig_window_handle_to_index(src_window_handle);

    TigVideoBufferBlitSpec blit_spec;
    blit_spec.field_0 = 0;
    blit_spec.src_video_buffer = windows[src_window_index].video_buffer;
    blit_spec.src_rect = src_rect;
    blit_spec.dst_video_buffer = dst_video_buffer;
    blit_spec.dst_rect = dst_rect;

    return tig_video_buffer_blit(&blit_spec);
}

// 0x51DFF0
int tig_window_copy_from_bmp(int a1, int a2, int a3, int a4)
{
    // TODO: Incomplete.
}

// 0x51E0A0
int tig_window_tint(int window_handle, TigRect* rect, int a3, int a4)
{
    // TODO: Incomplete.
}

// 0x51E190
int tig_window_text_write(int window_handle, int a2, TigRect* rect)
{
    // TODO: Incomplete.
}

// 0x51E2A0
int tig_window_free_index()
{
    for (int index = 0; index < TIG_WINDOW_MAX; index++) {
        if ((windows[index].field_0 & TIG_WINDOW_USAGE_FLAG_0x01) != 0) {
            return index;
        }
    }

    return -1;
}

// 0x51E2C0
int tig_window_handle_to_index(int window_handle)
{
    return window_handle;
}

// 0x51E2D0
int tig_window_index_to_handle(int window_index)
{
    return window_index;
}

// 0x51E2E0
void push_window_stack(int window_handle)
{
    // TODO: Incomplete.
}

// 0x51E3E0
bool pop_window_stack(int window_handle)
{
    // TODO: Incomplete.
}

// 0x51E430
void tig_window_set_needs_display_in_rect(TigRect* rect)
{
    if (tig_window_initialized) {
        TigRect temp_rect;
        if (rect != NULL) {
            temp_rect = *rect;
            if (tig_rect_intersection(&temp_rect, &tig_window_screen_rect, &temp_rect) != TIG_OK) {
                return;
            }
        } else {
            temp_rect = tig_window_screen_rect;
        }

        if (tig_window_dirty_rects != NULL) {
            TigRectListNode* node = tig_rect_node_create();
            node->rect = temp_rect;
            node->next = tig_window_dirty_rects;
            tig_window_dirty_rects = node;
        } else {
            tig_window_dirty_rects = tig_rect_node_create();
            if (tig_window_dirty_rects != NULL) {
                tig_window_dirty_rects->rect = temp_rect;
                tig_window_dirty_rects->next = NULL;
            }
        }
    }
}

// 0x51E530
int tig_window_button_add(int window_handle, int button_handle)
{
    if (window_handle == -1) {
        tig_debug_printf("tig_window_button_add: ERROR: Attempt to reference Empty WinID!\n");
        return 12;
    }

    int window_index = tig_window_handle_to_index(window_handle);
    if (windows[window_index].buttons_count == TIG_WINDOW_BUTTON_MAX) {
        return 3;
    }

    windows[window_index].buttons[windows[window_index].buttons_count++] = button_handle;

    return TIG_OK;
}

// 0x51E5A0
int tig_window_button_remove(int window_handle, int button_handle)
{
    if (window_handle == -1) {
        tig_debug_printf("tig_window_button_remove: ERROR: Attempt to reference Empty WinID!\n");
        return 12;
    }

    // TODO: Incomplete.

    windows[window_index].buttons_count--;

    return TIG_OK;
}

// 0x51E640
int tig_window_button_list(int window_handle, int** buttons)
{
    if (window_handle == -1) {
        tig_debug_printf("tig_window_button_list: ERROR: Attempt to reference Empty WinID!\n");
        *buttons = NULL;
        return 0;
    }

    int window_index = tig_window_handle_to_index(window_handle);
    *buttons = windows[window_index].buttons;
    return windows[window_index].buttons_count;
}

// 0x51E690
int sub_51E690(int a1, int a2, int a3)
{
    // TODO: Incomplete.
}

// 0x51E790
bool sub_51E790(int a1)
{
    // TODO: Incomplete.
}

// 0x51E850
int sub_51E850(int window_handle)
{
    if (!pop_window_stack(window_handle)) {
        return 16;
    }

    push_window_stack(window_handle);

    int window_index = tig_window_handle_to_index(window_handle);
    if ((windows[window_index].flags & TIG_WINDOW_FLAG_HIDDEN) == 0) {
        tig_window_set_needs_display_in_rect(&(windows[window_index].rect));
    }

    return TIG_OK;
}

// 0x51E8F0
int tig_window_show(int window_handle)
{
    int window_index = tig_window_handle_to_index(window_handle);
    TigWindow* window = &(windows[window_index]);
    window->flags &= ~TIG_WINDOW_FLAG_HIDDEN;
    tig_window_set_needs_display_in_rect(&(window->rect));

    for (int index = 0; index < window->buttons_count; index++) {
        tig_button_show(window->buttons[index]);
    }

    return TIG_OK;
}

// 0x51E950
int tig_window_hide(int window_handle)
{
    int window_index = tig_window_handle_to_index(window_handle);
    TigWindow* window = &(windows[window_index]);
    window->flags |= TIG_WINDOW_FLAG_HIDDEN;
    tig_window_set_needs_display_in_rect(&(window->rect));

    for (int index = 0; index < window->buttons_count; index++) {
        tig_button_hide(window->buttons[index]);
    }

    return TIG_OK;
}

// 0x51E9B0
bool tig_window_is_hidden(int window_handle)
{
    int window_index = tig_window_handle_to_index(window_handle);
    return (windows[window_index].flags & TIG_WINDOW_FLAG_HIDDEN) != 0;
}

// 0x51EA10
int tig_window_vbid_get(int window_handle, TigVideoBuffer** video_buffer)
{
    if (video_buffer == NULL) {
        return 16;
    }

    if (!tig_window_initialized) {
        return 1;
    }

    int window_index = tig_window_handle_to_index(window_handle);
    *video_buffer = windows[window_index].video_buffer;

    return TIG_OK;
}

// 0x51EA60
int sub_51EA60(int a1, int a2)
{
    // TODO: Incomplete.
}

// 0x51EBF0
bool sub_51EBF0(int a1)
{
    // TODO: Incomplete.
}

// 0x51ED10
void sub_51ED10()
{
    if (dword_5BED98 != -1) {
        if (tig_window_destroy(dword_5BED98) == TIG_OK) {
            dword_5BED98 = -1;
        }
    }
}

// 0x51ED40
void sub_51ED40(TigRect* rect)
{
    // 0x5BEDA0
    static TigRect default_rect = { 0, 0, 325, 136 };

    TigRect text_rect = { 30, 14, 265, 65 };

    if (rect == NULL) {
        rect = &default_rect;
    }

    if (rect->x < default_rect.x + default_rect.width
        && rect->y < default_rect.y + default_rect.height
        && default_rect.x < rect->x + rect->width
        && default_rect.y < rect->y + rect->height) {
        TigArtBlitSpec blit_spec;
        blit_spec.src_rect = rect;
        blit_spec.dst_rect = rect;
        blit_spec.field_0 = 0;
        tig_art_interface_id_create(822, 0, 0, 0, &(blit_spec.src_art_id));
        if (tig_window_blit_art(dword_5BED98, &blit_spec) == TIG_OK) {
            if (dword_604764) {
                sub_535340(dword_60F130);
                tig_window_text_write(dword_5BED98, dword_604764, &text_rect);
                sub_535380();
            }
        }
    }
}

// 0x51EE30
bool sub_51EE30(int type, int window_handle)
{
    TigButtonData button_data1;
    button_data1.field_20 = -1;
    button_data1.field_1C = -1;
    button_data1.field_24 = -1;
    button_data1.field_28 = -1;
    button_data1.flags = TIG_BUTTON_FLAG_0x01;
    button_data1.window_handle = window_handle;

    TigButtonData button_data2;
    button_data1.field_20 = -1;
    button_data1.field_1C = -1;
    button_data1.field_24 = -1;
    button_data1.field_28 = -1;
    button_data1.flags = TIG_BUTTON_FLAG_0x01;
    button_data1.window_handle = window_handle;

    TigArtFrameData art_frame_data1;
    TigArtFrameData art_frame_data2;

    switch (type) {
    case 0:
        tig_art_interface_id_create(823, 0, 0, 0, &(button_data1.art_id));
        tig_art_frame_data(button_data1.art_id, &art_frame_data1);
        button_data1.x = 149;
        button_data1.y = 102;
        button_data1.width = art_frame_data1.width;
        button_data1.height = art_frame_data1.height;
        tig_button_create(&button_data1, &(dword_60F038[0]));
        break;
    case 1:
        tig_art_interface_id_create(824, 0, 0, 0, &(button_data2.art_id));
        tig_art_frame_data(button_data2.art_id, &art_frame_data2);
        button_data2.x = 149;
        button_data2.y = 102;
        button_data2.width = art_frame_data2.width;
        button_data2.height = art_frame_data2.height;
        tig_button_create(&button_data2, &(dword_60F038[1]));
        break;
    case 2:
        if (tig_art_interface_id_create(823, 0, 0, 0, &(button_data1.art_id)) != TIG_OK)
            break;
        if (tig_art_frame_data(button_data1.art_id, &art_frame_data1) != TIG_OK)
            break;
        if (tig_art_interface_id_create(824, 0, 0, 0, &(button_data2.art_id)) != TIG_OK)
            break;
        if (tig_art_frame_data(button_data2.art_id, &art_frame_data2) != TIG_OK)
            break;

        button_data1.x = 73;
        button_data1.y = 102;
        button_data1.width = art_frame_data1.width;
        button_data1.height = art_frame_data1.height;
        if (tig_button_create(&button_data1, &(dword_60F038[0])) != TIG_OK) {
            return false;
        }

        button_data2.x = 225;
        button_data2.y = 102;
        button_data2.width = art_frame_data2.width;
        button_data2.height = art_frame_data2.height;
        if (tig_button_create(&button_data2, &(dword_60F038[1])) != TIG_OK) {
            return false;
        }
    }

    return true;
}

// 0x51F050
bool sub_51F050()
{
    Felicity felicity;
    felicity.field_0 = 0;
    tig_art_interface_id_create(229, 0, 0, 0, &(felicity.art_id));
    felicity.field_1C = 0;
    felicity.field_8 = tig_color_rgb_make(255, 255, 255);
    tig_felicity_create(&felicity, &off_60F130);

    dword_60F038[0] = -1;
    dword_60F038[1] = -1;

    return true;
}

// 0x51F0F0
void sub_51F0F0()
{
    if (off_60F130 != NULL) {
        tig_felicity_destroy(off_60F130);
        off_60F130 = NULL;
    }

    for (int index = 0; index < TIG_WINDOW_MODAL_BUTTON_MAX; index++) {
        if (dword_60F038[index] != -1) {
            tig_button_destroy(dword_60F038[index]);
        }
    }
}
