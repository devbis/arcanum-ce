#ifndef ARCANUM_UI_HOTKEY_UI_H_
#define ARCANUM_UI_HOTKEY_UI_H_

#include "game/context.h"
#include "game/obj.h"
#include "ui/types.h"

extern int dword_5CB4E4;
extern int dword_6839B0;

bool hotkey_ui_init(GameInitInfo* init_info);
void hotkey_ui_exit();
void hotkey_ui_resize(GameResizeInfo* resize_info);
bool hotkey_ui_start(tig_window_handle_t a1, TigRect* rect, tig_window_handle_t a3, bool a4);
void sub_57DA50();
void sub_57DAB0();
bool hotkey_ui_save(TigFile* stream);
bool hotkey_ui_load(GameLoadInfo* load_info);
void sub_57DC20();
bool hotkey_ui_process_event(TigMessage* msg);
bool sub_57DE00();
void intgame_hotkey_refresh(int index);
void sub_57E5A0(Hotkey* hotkey);
bool sub_57E5D0();
void sub_57EF90(int index);
void sub_57EFA0(int type, int data, int64_t item_obj);
void sub_57E8B0();
bool sub_57E8D0(int a1);
bool sub_57EDA0(int a1);
void sub_57EDF0(int64_t obj, int64_t a2, int a3);
bool sub_57EED0(int64_t obj, int a2);
void sub_57F1D0(int index);
void sub_57F210(int index);
void sub_57F2C0(int64_t obj, int a3);
Hotkey* sub_57F240(int index);
bool sub_57F260();
void sub_57F340(int a1);
void intgame_hotkeys_recover();

#endif /* ARCANUM_UI_HOTKEY_UI_H_ */
