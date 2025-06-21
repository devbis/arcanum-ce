#ifndef ARCANUM_UI_SCHEMATIC_UI_H_
#define ARCANUM_UI_SCHEMATIC_UI_H_

#include "game/context.h"
#include "game/schematic.h"

bool schematic_ui_init(GameInitInfo* init_info);
void schematic_ui_exit();
void schematic_ui_reset();
void schematic_ui_toggle(int64_t primary_obj, int64_t secondary_obj);
void schematic_ui_close();
void schematic_ui_info_get(int schematic, SchematicInfo* schematic_info);
bool sub_56E720(int schematic, int64_t a2, int64_t a3);
bool sub_56E950(int a1, int64_t a2, int64_t obj);
char* sub_56E9D0(int schematic);
char* schematic_ui_product_get(int tech, int degree);
void schematic_ui_components_get(int tech, int degree, char* item1, char* item2);

#endif /* ARCANUM_UI_SCHEMATIC_UI_H_ */
