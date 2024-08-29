#ifndef ARCANUM_GAME_GAMELIB_H_
#define ARCANUM_GAME_GAMELIB_H_

#include "game/context.h"
#include "game/settings.h"
#include "tig/video.h"

extern unsigned int gamelib_ping_time;
extern Settings settings;
extern TigVideoBuffer* dword_739E7C;

bool gamelib_init(GameInitInfo* init_info);
void gamelib_reset();
void gamelib_resize(ResizeInfo* resize_info);
bool gamelib_mod_guid(GUID* guid_ptr);
void sub_402D30(TigRect* rect);
bool sub_402E50();

#endif /* ARCANUM_GAME_GAMELIB_H_ */
