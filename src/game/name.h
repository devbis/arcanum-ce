#ifndef ARCANUM_GAME_NAME_H_
#define ARCANUM_GAME_NAME_H_

#include "game/context.h"

typedef struct PathCreateInfo {
    /* 0000 */ int64_t obj;
    /* 0008 */ int64_t from;
    /* 0010 */ int64_t to;
    /* 0018 */ int max_rotations;
    /* 001C */ int8_t* rotations;
    /* 0020 */ int field_20;
    /* 0024 */ int field_24;
} PathCreateInfo;

bool name_init(GameInitInfo* init_info);
void name_exit();
tig_art_id_t sub_41D510(tig_art_id_t aid);
int sub_4201C0(int64_t a1, int64_t a2, void* a3);
bool sub_421AC0(int a1);
bool sub_421AE0(int a1);
bool sub_41F3C0(PathCreateInfo* path_create_info);

#endif /* ARCANUM_GAME_NAME_H_ */
