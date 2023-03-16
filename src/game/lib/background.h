#ifndef ARCANUM_GAME_LIB_BACKGROUND_H_
#define ARCANUM_GAME_LIB_BACKGROUND_H_

#include "game/context.h"

bool background_init(GameContext* ctx);
void background_exit();
int background_get_description(int background);
const char* background_description_get_text(int num);
const char* background_description_get_body(int num);
const char* background_description_get_name(int num);
bool background_get_items(char* dest, size_t size, int background);

#endif /* ARCANUM_GAME_LIB_BACKGROUND_H_ */
