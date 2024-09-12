#ifndef ARCANUM_GAME_TARGET_H_
#define ARCANUM_GAME_TARGET_H_

#include "game/context.h"

typedef struct S603D20 {
    /* 0004 */ int field_4;
    /* 0000 */ int field_0;
    /* 0008 */ int field_8;
    /* 000C */ int field_C;
    /* 0010 */ int field_10;
    /* 0014 */ int field_14;
} S603D20;

typedef struct S603CB8 {
    /* 0000 */ S603D20* field_0;
    /* 0004 */ int field_4;
    /* 0008 */ int64_t field_8;
    /* 0010 */ int64_t field_10;
    /* 0018 */ int64_t field_18;
    /* 0020 */ int64_t field_20;
    /* 0028 */ int field_28;
    /* 002C */ int field_2C;
    /* 0030 */ int field_30;
    /* 0034 */ int field_34;
    /* 0038 */ int64_t field_38;
    /* 0040 */ int64_t field_40;
    /* 0048 */ int field_48;
    /* 004C */ int field_4C;
    /* 0050 */ int field_50;
    /* 0054 */ int field_54;
    /* 0058 */ int field_58;
    /* 005C */ int field_5C;
    /* 0060 */ int field_60;
    /* 0064 */ int field_64;
} S603CB8;

bool target_init(GameInitInfo* init_info);
void target_exit();
void target_reset();
void target_resize(ResizeInfo* resize_info);
void sub_4F25E0(S603D20* a1);
void sub_4F2600(S603CB8 *a1, S603D20 *a2, int64_t a3);
void sub_4F2C60(int64_t* obj_ptr);
int64_t sub_4F2D10();
bool sub_4F5090(int64_t obj, int index);
void sub_4F5270(int64_t obj, int index);
void sub_4F5320(void(*func)(int64_t obj, int index));
void sub_4F5330(int64_t a1, int64_t a2);
void sub_4F5590(int64_t obj, int* a2);
void sub_4F5690(int64_t a1, int64_t a2, int a3);
int sub_4F5770(int64_t obj, int* a2, int* a3);
int sub_4F57C0(int64_t obj, int a2, int* a3, int* a4);

#endif /* ARCANUM_GAME_TARGET_H_ */
