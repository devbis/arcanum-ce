#ifndef ARCANUM_UI_SCROLLBAR_UI_H_
#define ARCANUM_UI_SCROLLBAR_UI_H_

#include "game/context.h"

typedef struct ScrollbarId {
    /* 0000 */ int index;
    /* 0004 */ int global_index;
} ScrollbarId;

static_assert(sizeof(ScrollbarId) == 0x8, "wrong size");

typedef void(ScrollbarUiControlValueChanged)(int value);
typedef void(ScrollbarUiControlRefresh)(TigRect* rect);

typedef struct ScrollbarUiControlInfo {
    /* 0000 */ unsigned int flags;
    /* 0004 */ TigRect scrollbar_rect;
    /* 0014 */ TigRect content_rect;
    /* 0024 */ int max_value;
    /* 0028 */ int min_value;
    /* 002C */ int field_2C;
    /* 0030 */ int field_30;
    /* 0034 */ int field_34;
    /* 0038 */ int value;
    /* 003C */ ScrollbarUiControlValueChanged* field_3C;
    /* 0040 */ ScrollbarUiControlRefresh* field_40;
} ScrollbarUiControlInfo;

static_assert(sizeof(ScrollbarUiControlInfo) == 0x44, "wrong size");

#define SCROLLBAR_MIN_VALUE 0
#define SCROLLBAR_MAX_VALUE 1
#define SCROLLBAR_CURRENT_VALUE 2

bool scrollbar_ui_init(GameInitInfo* init_info);
void scrollbar_ui_exit();
void scrollbar_ui_reset();
bool scrollbar_ui_control_create(ScrollbarId* id, ScrollbarUiControlInfo* info, tig_window_handle_t window_handle);
void scrollbar_ui_control_destroy(ScrollbarId id);
void scrollbar_ui_control_redraw(ScrollbarId id);
bool scrollbar_ui_control_show(ScrollbarId id);
bool scrollbar_ui_control_hide(ScrollbarId id);
bool scrollbar_ui_process_event(TigMessage* msg);
void scrollbar_ui_control_set(ScrollbarId id, int type, int value);
void scrollbar_ui_begin_ignore_events();
void scrollbar_ui_end_ignore_events();

#endif /* ARCANUM_UI_SCROLLBAR_UI_H_ */
