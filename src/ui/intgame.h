#ifndef ARCANUM_UI_INTGAME_H_
#define ARCANUM_UI_INTGAME_H_

#include "game/context.h"
#include "ui/types.h"

bool intgame_init(GameInitInfo* init_info);
void intgame_reset();
void intgame_resize(ResizeInfo* resize_info);
void intgame_exit();
bool intgame_save(TigFile* stream);
bool intgame_load(GameLoadInfo* load_info);
bool iso_interface_create(tig_window_handle_t window_handle);
void iso_interface_destroy();
void sub_54AA30();
bool sub_54AA60(tig_window_handle_t window_handle, TigRect* rect, UiButtonInfo* button_info, unsigned int flags);
bool sub_54AAE0(UiButtonInfo* button_info);
void sub_54AEE0(int a1);
void sub_54B3A0();
void sub_54B3C0();
void sub_5506C0(int window_type);
void sub_550720();
void sub_550750(John* a1);
void sub_550770(int a1, const char* str);
void sub_5507D0(void(*func)(John* a1));
void sub_5507E0(int spl);
void sub_550860(int college);
void sub_5508C0(int value);
void sub_550930();
void sub_550DA0(int a1, S550DA0* a2);
bool sub_551000(int x, int y);
void sub_551080();
void sub_551160();
bool sub_5517A0(TigMessage* msg);
int sub_551A00();
void sub_551A80(int a1);
int sub_551F70(int a1);
void sub_552070();
void sub_552080(int window_type);
void sub_552160(const char* str, tig_font_handle_t font);
bool intgame_clock_process_callback();
bool sub_553320(void(*func)(TigMessage* msg));
void sub_553350();
void sub_553370();
void sub_553380(int a1, const char* str);
void sub_5533A0(TigMessage* msg);;
int sub_5533B0();
void sub_553620(int index, tig_art_id_t art_id);
void sub_553910(int index, bool a2);
void sub_553990();
void sub_553A60(int art_num);
void sub_556E60();
bool intgame_big_window_lock(TigWindowMessageFilterFunc* func, tig_window_handle_t* window_handle_ptr);
void intgame_big_window_unlock();
void sub_5576B0();
void sub_557790(int64_t obj);
unsigned int intgame_get_iso_window_flags();
void intgame_set_iso_window_flags(unsigned int flags);
void intgame_set_iso_window_width(int width);
void intgame_set_iso_window_height(int height);
bool intgame_create_iso_window(tig_window_handle_t* window_handle_ptr);
bool intgame_is_compact_interface();
void intgame_set_fullscreen();
void intgame_toggle_interface();
int sub_557AA0();
int sub_557AB0();
int sub_557B60();

#endif /* ARCANUM_UI_INTGAME_H_ */
