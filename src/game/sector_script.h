#ifndef ARCANUM_GAME_SECTOR_SCRIPT_H_
#define ARCANUM_GAME_SECTOR_SCRIPT_H_

#include "game/context.h"

bool sector_script_init(GameInitInfo* init_info);
void sector_script_reset();
void sector_script_exit();
bool sub_4C0440(int64_t a1, void* a2);
bool sub_4C0490(int64_t v1, void* a2);

#endif /* ARCANUM_GAME_SECTOR_SCRIPT_H_ */
