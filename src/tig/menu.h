#ifndef ARCANUM_TIG_ANNA_H_
#define ARCANUM_TIG_ANNA_H_

#include "tig/types.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct TigMenuBarDesc {
    /* 0000 */ tig_window_handle_t window_handle;
    /* 0004 */ int x;
    /* 0008 */ int y;
    /* 000C */ int num_drop_downs;
    /* 0010 */ const char** drop_downs;
    /* 0014 */ int* num_menu_items;
    /* 0018 */ const char** menu_items;
    /* 001C */ unsigned int* flags;
} TigMenuBarDesc;

static_assert(sizeof(TigMenuBarDesc) == 0x20, "wrong size");

typedef struct TigMenuColors {
    /* 0000 */ unsigned int text_color;
    /* 0004 */ unsigned int background_color;
    /* 0008 */ unsigned int selected_text_color;
    /* 000C */ unsigned int disabled_text_color;
    /* 0010 */ unsigned int selected_menu_item_outline_color;
} TigMenuColors;

static_assert(sizeof(TigMenuColors) == 0x14, "wrong size");

int tig_menu_init(TigInitializeInfo* init_info);
void tig_menu_exit();
int tig_menu_set_colors(TigMenuColors* colors);
int sub_539130(const char* title, const char** menu_items, int num_menu_items, int x, int y);
int tig_menu_bar_set_flags(int menu_bar_handle, unsigned int* flags);
void tig_menu_bar_on_click(tig_button_handle_t button_handle);
int tig_menu_do_context_menu(const char** menu_items, int num_menu_items, int x, int y, unsigned int* flags);
int tig_menu_bar_create(TigMenuBarDesc* desc);

#ifdef __cplusplus
}
#endif

#endif /* ARCANUM_TIG_ANNA_H_ */
