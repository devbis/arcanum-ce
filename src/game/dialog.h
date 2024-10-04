#ifndef ARCANUM_GAME_DIALOG_H_
#define ARCANUM_GAME_DIALOG_H_

#include "game/context.h"
#include "game/object.h"

// TODO: Better name.
typedef struct DialogEntryNode {
    /* 0000 */ int field_0;
    /* 0004 */ char field_4;
    /* 0005 */ char field_5;
    /* 0006 */ char field_6;
    /* 0007 */ char field_7;
    /* 0008 */ int64_t field_8;
    /* 0010 */ Ryan field_10;
    /* 0038 */ int64_t field_38;
    /* 0040 */ Ryan field_40;
    /* 0068 */ int field_68;
    /* 006C */ int field_6C;
    /* 0070 */ char field_70[1000];
    /* 0458 */ int field_458;
    /* 045C */ int field_45C;
    /* 0460 */ char field_460[5][1000];
    /* 17E8 */ int field_17E8;
    /* 17EC */ int field_17EC;
    /* 17F0 */ int field_17F0[5];
    /* 1804 */ int field_1804[5];
    /* 1818 */ int field_1818[5];
    /* 182C */ int field_182C[5];
    /* 1840 */ int field_1840;
    /* 1844 */ int field_1844;
} DialogEntryNode;

static_assert(sizeof(DialogEntryNode) == 0x1848, "wrong size");

bool dialog_init(GameInitInfo* init_info);
void dialog_exit();
bool sub_412E10(const char* path, int* a2);
void sub_412F40(int dlg);
void sub_412F60(int dlg);
bool sub_412FD0(DialogEntryNode* a1);
void sub_413130(DialogEntryNode* a1, int a2);
void sub_413280(DialogEntryNode* a1);
void sub_4132A0(int64_t a1, int64_t a2, char* buffer);
void sub_413360(int64_t a1, int64_t a2, DialogEntryNode* a3);
void sub_418030();
void sub_4182C0();

#endif /* ARCANUM_GAME_DIALOG_H_ */
