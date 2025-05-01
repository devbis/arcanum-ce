#ifndef ARCANUM_GAME_MONSTERGEN_H_
#define ARCANUM_GAME_MONSTERGEN_H_

#include "game/context.h"
#include "game/timeevent.h"

bool monstergen_init(GameInitInfo* init_info);
void monstergen_reset();
void monstergen_exit();
void monstergen_resize(GameResizeInfo* resize_info);
bool monstergen_load(GameLoadInfo* load_info);
bool monstergen_save(TigFile* stream);
bool sub_4BA790(int64_t obj, DateTime* datetime);
void monstergen_notify_killed(int64_t obj);
bool monstergen_is_disabled(int index);
void monstergen_enable(int index);
void monstergen_disable(int index);

#endif /* ARCANUM_GAME_MONSTERGEN_H_ */
