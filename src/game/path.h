#ifndef ARCANUM_GAME_PATH_H_
#define ARCANUM_GAME_PATH_H_

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

int sub_41F3C0(PathCreateInfo* path_create_info);
unsigned int sub_41F570(unsigned int flags);
int sub_4201C0(int64_t from, int64_t to, uint8_t* rotations);
bool path_set_limit(int a1);
bool path_set_time_limit(int a1);

#endif /* ARCANUM_GAME_PATH_H_ */
