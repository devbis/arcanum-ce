#ifndef ARCANUM_TIG_WINDOW_H_
#define ARCANUM_TIG_WINDOW_H_

#include "tig/art.h"
#include "tig/types.h"

#define TIG_WINDOW_FLAG_0x01 0x01
#define TIG_WINDOW_FLAG_0x02 0x02
#define TIG_WINDOW_FLAG_0x04 0x04
#define TIG_WINDOW_FLAG_0x08 0x08
#define TIG_WINDOW_FLAG_0x10 0x10
#define TIG_WINDOW_FLAG_HIDDEN 0x20
#define TIG_WINDOW_FLAG_0x40 0x40

typedef void(TigWindowMessageFilterFunc)();

typedef struct TigWindowData {
    int flags;
    // TODO: Remove separate fields.
    union {
        struct {
            int x;
            int y;
            int width;
            int height;
        };
        TigRect rect;
    };
    int field_14;
    int field_18;
    TigWindowMessageFilterFunc* message_filter_func;
};

static_assert(sizeof(TigWindowData) == 0x20, "wrong size");

typedef struct TigWindowDialogSpec {
    int field_0;
    int field_4;
    int field_8;
    int field_C;
    int field_10;
    int field_14;
    int field_18;
};

// See 0x51EA60.
static_assert(sizeof(TigWindowDialogSpec) == 0x1C, "wrong size");

int tig_window_init(TigContext* ctx);
void tig_window_exit();
int tig_window_create(TigWindowData* window_data, int* window_handle);
int tig_window_destroy(int window_handle);
int tig_window_button_destroy(int window_handle);
int tig_window_message_filter_set(int window_handle, TigWindowMessageFilterFunc* func);
int tig_window_data(int window_handle, TigWindowData* window_data);
int tig_window_display();
void sub_51D050(TigRect* a1, int a2, TigVideoBuffer* a3, int a4, int a5, int a6);
int tig_window_fill(int window_handle, TigRect* rect, int color);
int tig_window_line(int window_handle, int a2, int a3);
int tig_window_box(int window_handle, TigRect* rect, int color);
int sub_51D8D0(int a1);
int tig_window_blit_art(int window_handle, TigArtBlitSpec* blit_spec);
int tig_window_scroll(int window_handle, int dx, int dy);
int tig_window_scroll_rect(int window_handle, TigRect* rect, int dx, int dy);
int tig_window_copy(int dst_window_handle, TigRect* dst_rect, int src_window_handle, TigRect* src_rect);
int tig_window_copy_from_vbuffer(int dst_window_handle, TigRect* dst_rect, TigVideoBuffer* src_video_buffer, TigRect* src_rect);
int tig_window_copy_to_vbuffer(int src_window_handle, TigRect* src_rect, TigVideoBuffer* dst_video_buffer, TigRect* dst_rect);
int tig_window_copy_from_bmp(int a1, int a2, int a3, int a4);
int tig_window_tint(int window_handle, TigRect* rect, int a3, int a4);
int tig_window_text_write(int window_handle, int a2, TigRect* rect);
void tig_window_set_needs_display_in_rect(TigRect* rect);
int tig_window_button_add(int window_handle, int button_handle);
int tig_window_button_remove(int window_handle, int button_handle);
int tig_window_button_list(int window_handle, int** buttons);
int sub_51E850(int window_handle);
int tig_window_show(int window_handle);
int tig_window_hide(int window_handle);
bool tig_window_is_hidden(int window_handle);
int tig_window_vbid_get(int window_handle, TigVideoBuffer** video_buffer);

#endif /* ARCANUM_TIG_WINDOW_H_ */
