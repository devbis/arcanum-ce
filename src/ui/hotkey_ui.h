#ifndef ARCANUM_UI_HOTKEY_UI_H_
#define ARCANUM_UI_HOTKEY_UI_H_

#include "game/context.h"
#include "game/obj.h"
#include "ui/types.h"

typedef struct S683518 {
    /* 0000 */ int field_0;
    /* 0004 */ int field_4;
    /* 0008 */ int field_8;
    /* 000C */ int field_C;
    /* 0010 */ int field_10;
    /* 0014 */ int field_14;
    /* 0018 */ ObjectID field_18;
    /* 0030 */ int field_30;
    /* 0034 */ int field_34;
    /* 0038 */ int field_38;
    /* 003C */ int field_3C;
    /* 0040 */ int field_40;
    /* 0044 */ int field_44;
    /* 0048 */ UiButtonInfo info;
} S683518;

static_assert(sizeof(S683518) == 0x58, "wrong size");

bool hotkey_ui_init(GameInitInfo* init_info);
void hotkey_ui_exit();
void hotkey_ui_resize(ResizeInfo* resize_info);
bool hotkey_ui_start(int a1, TigRect* rect, int a3, bool a4);
void sub_57E5A0(S683518* a1);
void sub_57E8B0();
void sub_57F1D0(int index);
void sub_57F210(int index);
S683518* sub_57F240(int index);
bool sub_57F260();

#endif /* ARCANUM_UI_HOTKEY_UI_H_ */
