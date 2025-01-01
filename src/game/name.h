#ifndef ARCANUM_GAME_NAME_H_
#define ARCANUM_GAME_NAME_H_

#include "game/context.h"

bool name_init(GameInitInfo* init_info);
void name_exit();
tig_art_id_t sub_41D510(tig_art_id_t aid);
int sub_41DAE0(tig_art_id_t aid, char* path);
bool sub_41F090(int a1, int* a2, int* a3);

#endif /* ARCANUM_GAME_NAME_H_ */
