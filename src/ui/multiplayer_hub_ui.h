#ifndef ARCANUM_UI_MULTIPLAYER_HUB_UI_H_
#define ARCANUM_UI_MULTIPLAYER_HUB_UI_H_

#include "game/context.h"

void multiplayer_hub_ui_init();
void multiplayer_hub_ui_exit();
void sub_581910(TigRect* rect);
bool sub_5819D0(int a1);
void sub_581A60();
void sub_581A80();
void sub_581B10(TigRect* rect);
void sub_581E60(int x, int y);
void sub_581F10();
void sub_582510(const char* name);
void sub_5825B0(const char* name);
void sub_582650(const char* a1, const char* a2);
void sub_582670(const char* a1, const char* a2);
void sub_582690(const char* a1, const char* a2);
void sub_5826B0();
void sub_583200();
void sub_583510();
void sub_583540(TigRect* rect);
bool mainmenu_ui_execute_multiplayer_hub(int button);
void sub_583A00();
void sub_583A80();
void sub_583A90();
void sub_583B10();
void sub_583B20(int x, int y);
void sub_583C80(int x, int y);
void sub_584AE0();
void sub_584C00();
void sub_584CB0(TigRect* rect);

#endif /* ARCANUM_UI_MULTIPLAYER_HUB_UI_H_ */
