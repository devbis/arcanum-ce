#ifndef ARCANUM_GAME_TERRAIN_H_
#define ARCANUM_GAME_TERRAIN_H_

#include "game/context.h"
#include "game/sector.h"

bool terrain_init(GameInitInfo* init_info);
void terrain_reset();
void terrain_exit();
bool terrain_map_new(MapNewInfo* new_map_info);
bool terrain_open(const char* base_path, const char* save_path);
void terrain_map_close();
void terrain_sector_path(int64_t sector_id, char* path);
int sub_4E8DC0(uint16_t a1);
int sub_4E8DD0(uint16_t a1);
int sub_4E8DE0(uint16_t a1);
int sub_4E8DF0(uint16_t a1);
bool terrain_is_blocked(int64_t sec);
void sub_4E86F0(Sector* sector);
bool terrain_flush();
uint16_t sub_4E87F0(int64_t sec);

#endif /* ARCANUM_GAME_TERRAIN_H_ */
