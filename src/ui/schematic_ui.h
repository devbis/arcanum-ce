#ifndef ARCANUM_UI_SCHEMATIC_UI_H_
#define ARCANUM_UI_SCHEMATIC_UI_H_

#include "game/context.h"

bool schematic_ui_init(GameInitInfo* init_info);
void schematic_ui_exit();
void schematic_ui_reset();
void sub_56D130(long long a1, long long a2);
void sub_56D2D0();
void sub_56DBD0(int schematic, SchematicInfo* schematic_info);
void sub_56E720();
bool sub_56E950(int a1, int a2, int a3, long long obj);
const char* sub_56E9D0(int schematic);
const char* sub_56EA10(int a1, int a2);
void sub_56EA30(int a1, int a2, char* a3, char* a4);

#endif /* ARCANUM_UI_SCHEMATIC_UI_H_ */
