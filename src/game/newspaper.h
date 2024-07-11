#ifndef ARCANUM_GAME_NEWSPAPER_H_
#define ARCANUM_GAME_NEWSPAPER_H_

#include "game/context.h"
#include "game/obj.h"

bool newspaper_init(GameInitInfo* init_info);
void newspaper_reset();
void newspaper_exit();
bool newspaper_load(GameLoadInfo* load_info);
bool newspaper_save(TigFile* stream);
void newspaper_queue(int num, bool priority);
bool newspaper_is_queued(int num);
int sub_4BF200(int index);
object_id_t sub_4BF210(int start, int64_t a2);

#endif /* ARCANUM_GAME_NEWSPAPER_H_ */
