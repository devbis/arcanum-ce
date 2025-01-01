#ifndef ARCANUM_GAME_DIALOG_H_
#define ARCANUM_GAME_DIALOG_H_

#include "game/context.h"
#include "game/obj.h"
#include "game/object.h"

// TODO: Better name.
typedef struct DialogEntryNode {
    /* 0000 */ int field_0;
    /* 0004 */ char field_4;
    /* 0005 */ char field_5;
    /* 0006 */ char field_6;
    /* 0007 */ char field_7;
    /* 0008 */ int64_t pc_obj;
    /* 0010 */ Ryan field_10;
    /* 0038 */ int64_t npc_obj;
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
    /* 182C */ char* field_182C[5];
    /* 1840 */ int field_1840;
    /* 1844 */ int field_1844;
} DialogEntryNode;

static_assert(sizeof(DialogEntryNode) == 0x1848, "wrong size");

typedef struct DialogSerializedData {
    /* 0000 */ int field_0;
    /* 0004 */ int field_4;
    /* 0008 */ ObjectID field_8;
    /* 0020 */ ObjectID field_20;
    /* 0038 */ int field_38;
    /* 003C */ int field_3C;
    /* 0040 */ int field_40;
    /* 0044 */ int field_44;
    /* 0048 */ int field_48;
    /* 004C */ int field_4C;
    /* 0050 */ int field_50[5];
    /* 0064 */ int field_64[5];
    /* 0078 */ int field_78;
    /* 007C */ int field_7C;
    /* 0080 */ int field_80[5];
    /* 0094 */ int field_94[5];
    /* 00A8 */ int field_A8[5];
    /* 00BC */ int field_BC;
    /* 00C0 */ int field_C0;
} DialogSerializedData;

// TODO: Figure out size.
// static_assert(sizeof(DialogSerializedData) == 0xC4, "wrong size");

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
void sub_4133B0(int64_t a1, int64_t a2, char* buffer, int* a4);
void sub_413490(int64_t a1, int64_t a2, char* a3);
void sub_413520(int64_t a1, int64_t a2, char* a3);
void sub_4135B0(int64_t a1, int64_t a2, char* a3);
void sub_413640(int64_t a1, int64_t a2, char* a3);
void sub_4136D0(int64_t a1, int64_t a2, char* a3);
void sub_413760(int64_t a1, int64_t a2, char* a3);
void sub_4137F0(int64_t a1, int64_t a2, char* a3);
void sub_413880(int64_t a1, int64_t a2, char* a3);
void sub_413910(int64_t a1, int64_t a2, char* a3);
void sub_4139A0(int64_t a1, int64_t a2, char* a3);
void sub_413A30(DialogEntryNode* a1, bool a2);
void sub_413A90(int64_t a1, int64_t a2, int a3, char* a4, int* a5);
void sub_413EA0(int64_t a1, int64_t a2, char* a3, int* a4);
void sub_413F50(int64_t a1, int64_t a2, int a3, char* a4, int* a5);
void sub_414020(int64_t a1, int64_t a2, int a3, char* a4);
void sub_414130(int64_t a1, int64_t a2, char* a3, int* a4);
void sub_4141E0(int64_t a1, int64_t a2, char* a3, int* a4);
void sub_414290(int64_t a1, int64_t a2, int a3, char* a4, int* a5);
void sub_414370(int64_t a1, int64_t a2, int a3, char* a4, int* a5);
void sub_414490(int64_t a1, int64_t a2, char* a3, int* a4);
void sub_414540(int64_t a1, int64_t a2, char* a3);
void sub_4145D0(int64_t a1, int64_t a2, char* a3);
void sub_414660(int64_t a1, int64_t a2, char* a3);
void sub_4146F0(int64_t a1, int64_t a2, char* a3);
void sub_414780(int64_t a1, int64_t a2, char* a3);
void dialog_check();
void dialog_enable_numbers();
void sub_418A00(int a1, int* a2, int* a3);

#endif /* ARCANUM_GAME_DIALOG_H_ */
