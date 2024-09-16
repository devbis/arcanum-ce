#ifndef ARCANUM_GAME_TERRAIN_H_
#define ARCANUM_GAME_TERRAIN_H_

#include "game/context.h"
#include "game/sector.h"

bool terrain_init(GameInitInfo* init_info);
void terrain_reset();
void terrain_exit();
int sub_4E8DC0(uint16_t a1);
int sub_4E8DD0(uint16_t a1);
int sub_4E8DE0(uint16_t a1);
int sub_4E8DF0(uint16_t a1);
void sub_4E86F0(Sector* sector);

#endif /* ARCANUM_GAME_TERRAIN_H_ */
