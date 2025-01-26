#ifndef ARCANUM_GAME_NEWSPAPER_H_
#define ARCANUM_GAME_NEWSPAPER_H_

#include "game/context.h"
#include "game/timeevent.h"

bool newspaper_init(GameInitInfo* init_info);
void newspaper_reset();
void newspaper_exit();
bool newspaper_load(GameLoadInfo* load_info);
bool newspaper_save(TigFile* stream);
void newspaper_queue(int num, bool priority);
bool newspaper_is_queued(int num);
int sub_4BF200(int index);
int64_t newspaper_create(int start, int64_t loc);
bool newspaper_timeevent_process(TimeEvent* timeevent);

#endif /* ARCANUM_GAME_NEWSPAPER_H_ */
