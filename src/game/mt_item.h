#ifndef ARCANUM_GAME_MT_ITEM_H_
#define ARCANUM_GAME_MT_ITEM_H_

#include "game/context.h"

bool mt_item_init(GameInitInfo* init_info);
void mt_item_exit();
int mt_item_triggers(int index);
int sub_4CB790(int a1);

#endif /* ARCANUM_GAME_MT_ITEM_H_ */
