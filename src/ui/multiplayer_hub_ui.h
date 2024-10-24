#ifndef ARCANUM_UI_MULTIPLAYER_HUB_UI_H_
#define ARCANUM_UI_MULTIPLAYER_HUB_UI_H_

#include "game/context.h"
#include "ui/types.h"

extern MainMenuButtonInfo stru_5CC058[3];
extern MainMenuButtonInfo stru_5CC0E8[5];
extern MainMenuButtonInfo stru_5CC278[7];
extern MainMenuButtonInfo stru_5CC4C0[2];
extern MainMenuButtonInfo stru_5CC520[2];
extern MainMenuButtonInfo stru_5CC6C8[20];

void multiplayer_hub_ui_init();
void multiplayer_hub_ui_exit();
void sub_581910(TigRect* rect);
void sub_581950(int x, int y);
bool sub_5819D0(int a1);
void multiplayer_hub_ui_create_chat();
void multiplayer_hub_ui_destroy_chat();
bool multiplayer_hub_ui_execute_chat(int a1);
void multiplayer_hub_ui_refresh_chat(TigRect* rect);
void sub_581E60(int x, int y);
void sub_581F10();
void sub_582510(const char* name);
void sub_5825B0(const char* name);
void sub_582650(const char* a1, const char* a2);
void sub_582670(const char* a1, const char* a2);
void sub_582690(const char* a1, const char* a2);
void sub_5826B0(int a1, int a2);
void sub_5826C0();
void mainmenu_ui_create_multiplayer_hub();
void mainmenu_ui_destroy_multiplayer_hub();
void mainmenu_ui_refresh_multiplayer_hub(TigRect* rect);
bool mainmenu_ui_execute_multiplayer_hub(int button);
void sub_583A00();
void sub_583A80();
void sub_583A90();
void sub_583B10();
void sub_583B20(int x, int y);
void sub_583C80(int x, int y);
void sub_583D90(TigRect* rect);
void sub_584150(TigRect* rect);
bool sub_5845E0(int btn);
bool sub_5847D0(int btn);
const char* sub_584A80();
const char* sub_584A90();
void sub_584AA0(const char* str);
void sub_584AC0(const char* str);
void sub_584AE0();
void sub_584C00();
void sub_584CB0(TigRect* rect);
void sub_5850C0(int x, int y);
bool sub_585270(int num);

#endif /* ARCANUM_UI_MULTIPLAYER_HUB_UI_H_ */
