#ifndef ARCANUM_GAME_BROADCAST_H_
#define ARCANUM_GAME_BROADCAST_H_

#include "game/context.h"

typedef struct Broadcast {
    int64_t loc;
    char field_8[128];
} Broadcast;

static_assert(sizeof(Broadcast) == 0x88, "wrong size");

typedef void(Func5FDC88)(int64_t a1, int64_t a2, const char* a3, int a4);

bool broadcast_init(GameInitInfo* init_info);
void broadcast_exit();
void sub_4C2EA0(Func5FDC88* func);
void sub_4C2F00(int64_t obj, Broadcast* bcast);
void broadcast_msg_client(int64_t obj, Broadcast* bcast);
void sub_4C3BE0(unsigned int a1, int64_t loc);

#endif /* ARCANUM_GAME_BROADCAST_H_ */
