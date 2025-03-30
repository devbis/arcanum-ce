#ifndef ARCANUM_GAME_PATH_H_
#define ARCANUM_GAME_PATH_H_

#include "game/context.h"

typedef struct PathCreateInfo {
    /* 0000 */ int64_t obj;
    /* 0008 */ int64_t from;
    /* 0010 */ int64_t to;
    /* 0018 */ int max_rotations;
    /* 001C */ int8_t* rotations;
    /* 0020 */ unsigned int flags;
    /* 0024 */ int field_24;
} PathCreateInfo;

typedef struct WmapPathInfo {
    /* 0000 */ int64_t from;
    /* 0008 */ int64_t to;
    /* 0010 */ int max_rotations;
    /* 0014 */ int8_t* rotations;
    /* 0018 */ int field_18;
} WmapPathInfo;

int sub_41F3C0(PathCreateInfo* path_create_info);
unsigned int sub_41F570(unsigned int flags);
int sub_4201C0(int64_t from, int64_t to, uint8_t* rotations);
int sub_4207D0(WmapPathInfo* path_info);
bool path_set_limit(int a1);
bool path_set_time_limit(int a1);

#endif /* ARCANUM_GAME_PATH_H_ */
