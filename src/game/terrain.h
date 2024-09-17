#ifndef ARCANUM_GAME_TERRAIN_H_
#define ARCANUM_GAME_TERRAIN_H_

#include "game/context.h"
#include "game/sector.h"

bool terrain_init(GameInitInfo* init_info);
void terrain_reset();
void terrain_exit();
void sub_4E7B90(MapResetInfo* reset_info);
int sub_4E8DC0(uint16_t a1);
int sub_4E8DD0(uint16_t a1);
int sub_4E8DE0(uint16_t a1);
int sub_4E8DF0(uint16_t a1);
bool sub_4E8E00(int64_t a1);
void sub_4E86F0(Sector* sector);
void terrain_flush();

#endif /* ARCANUM_GAME_TERRAIN_H_ */
