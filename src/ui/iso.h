#ifndef ARCANUM_UI_ISO_H_
#define ARCANUM_UI_ISO_H_

#include "game/context.h"

bool iso_init(GameInitInfo* init_info);
void iso_exit();
void iso_reset();
void iso_resize(GameResizeInfo* resize_info);
void sub_557FD0(const char* str);
bool iso_redraw();
void sub_558130(TigRect* rect);
void sub_5581A0(TigRect* rect);

#endif /* ARCANUM_UI_ISO_H_ */
