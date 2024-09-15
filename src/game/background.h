#ifndef ARCANUM_GAME_BACKGROUND_H_
#define ARCANUM_GAME_BACKGROUND_H_

#include "game/context.h"
#include "game/obj.h"

#define BACKGROUND_DAY_MAGE 62
#define BACKGROUND_NIGHT_MAGE 63
#define BACKGROUND_SKY_MAGE 64
#define BACKGROUND_NATURE_MAGE 65
#define BACKGROUND_AGORAPHOBIC 66
#define BACKGROUND_HYDROPHOBIC 67
#define BACKGROUND_AFRAID_OF_THE_DARK 68

bool background_init(GameInitInfo* init_info);
void background_exit();
bool background_find_first(int64_t obj, int* background_ptr);
bool background_find_next(int64_t obj, int* background_ptr);
bool background_find_prev(int64_t obj, int* background_ptr);
int background_get_description(int background);
const char* background_description_get_text(int num);
const char* background_description_get_body(int num);
const char* background_description_get_name(int num);
void sub_4C25E0(int64_t obj, int background, int background_text);
void background_obj_clear(object_id_t obj);
int background_obj_get_background(object_id_t obj);
int background_obj_get_background_text(object_id_t obj);
int background_adjust_money(int amount, int background);
bool background_get_items(char* dest, size_t size, int background);

#endif /* ARCANUM_GAME_BACKGROUND_H_ */
