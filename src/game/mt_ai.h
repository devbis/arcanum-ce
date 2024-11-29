#ifndef ARCANUM_GAME_MT_AI_H_
#define ARCANUM_GAME_MT_AI_H_

#include "game/context.h"

typedef struct S600A20_Entry {
    int spell;
    int64_t obj;
} S600A20_Entry;

// See 0x4CC3C0.
static_assert(sizeof(S600A20_Entry) == 0x10, "wrong size");

typedef struct S600A20 {
    int cnt;
    S600A20_Entry* entries;
} S600A20;

static_assert(sizeof(S600A20) == 0x8, "wrong size");

typedef struct S5FF620 {
    /* 0000 */ int64_t obj;
    /* 0008 */ int64_t field_8;
    /* 0010 */ int field_10;
    /* 0014 */ int field_14;
    /* 0018 */ int64_t field_18;
    /* 0020 */ int field_20;
    /* 0024 */ int field_24;
    /* 0028 */ int field_28;
    /* 002C */ int field_2C;
    /* 0030 */ S600A20 field_30[10];
} S5FF620;

static_assert(sizeof(S5FF620) == 0x80, "wrong size");

bool mt_ai_init(GameInitInfo* init_info);
void mt_ai_reset();
void mt_ai_exit();
int sub_4CC2A0(int spl);
void sub_4CCA90(S5FF620* a1, int64_t obj, int a3);
void sub_4CCBF0(S5FF620* a1);
void sub_4CCC00(int64_t obj);
void sub_4CCC40(int64_t a1, int64_t a2);

#endif /* ARCANUM_GAME_MT_AI_H_ */
