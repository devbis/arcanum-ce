#ifndef ARCANUM_GAME_MAGICTECH_H_
#define ARCANUM_GAME_MAGICTECH_H_

#include "game/context.h"
#include "game/obj.h"
#include "game/mes.h"
#include "game/timeevent.h"

#define MT_80 80
#define MT_140 140
#define MT_SPELL_COUNT 223

typedef enum MagicTechItemTriggers {
    MTIT_USER_ACTIVATE = 0x10000000,
    MTIT_NONE = 0x0,
    MTIT_WEAR = 0x20000000,
    MTIT_UNWEAR = 0x40000000,
    MTIT_PICKUP = 0x80000000,
    MTIT_DROP = 0x1000000,
    MTIT_PARENT_HIT = 0x2000000,
    MTIT_PARENT_STUNNED = 0x4000000,
    MTIT_PARENT_GOING_UNCONSCIOUS = 0x8000000,
    MTIT_PARENT_DYING = 0x100000,
    MTIT_PARENT_HIT_BY_ATK_SPELL = 0x200000,
    MTIT_PARENT_ATKS_OPPONENT = 0x400000,
    MTIT_PARENT_ATKS_LOCATION = 0x800000,
    MTIT_PARENT_DMGS_OPPONENT = 0x10000,
    MTIT_PARENT_DMGS_OPPONENT_W_ITEM = 0x20000,
    MTIT_TARGET_HIT = 0x40000,
    MTIT_TARGET_GOING_UNCONSCIOUS = 0x80000,
    MTIT_ITEM_USED = 0x3000,
    MTIT_TARGET_ATTACKER = 0x2000,
    MTIT_TARGET_ATTACKER_WEAPON = 0x4000,
    MTIT_TARGET_ATTACKER_ARMOR = 0x8000,
    MTIT_TARGET_ATTACKER_WEAPON_MELEE = 0x100,
    MTIT_RANDOM_CHANCE_RARE = 0x200,
    MTIT_RANDOM_CHANCE_UNCOMMON = 0x400,
    MTIT_RANDOM_CHANCE_COMMON = 0x800,
    MTIT_RANDOM_CHANCE_FREQUENT = 0x10,
} MagicTechItemTriggers;

typedef struct MagicTechLock {
    /* 0000 */ int field_0;
    /* 0004 */ int spell;
    /* 0008 */ int action;
    /* 000C */ int field_C;
    /* 0010 */ int64_t source_obj;
    /* 0018 */ int field_18;
    /* 001C */ int field_1C;
    /* 0020 */ int field_20;
    /* 0024 */ int field_24;
    /* 0028 */ int field_28;
    /* 002C */ int field_2C;
    /* 0030 */ int field_30;
    /* 0034 */ int field_34;
    /* 0038 */ int field_38;
    /* 003C */ int field_3C;
    /* 0040 */ int field_40;
    /* 0044 */ int field_44;
    /* 0048 */ int field_48;
    /* 004C */ int field_4C;
    /* 0050 */ int field_50;
    /* 0054 */ int field_54;
    /* 0058 */ int64_t parent_obj;
    /* 0060 */ int field_60;
    /* 0064 */ int field_64;
    /* 0068 */ int field_68;
    /* 006C */ int field_6C;
    /* 0070 */ int field_70;
    /* 0074 */ int field_74;
    /* 0078 */ int field_78;
    /* 007C */ int field_7C;
    /* 0080 */ int field_80;
    /* 0084 */ int field_84;
    /* 0088 */ int field_88;
    /* 008C */ int field_8C;
    /* 0090 */ int field_90;
    /* 0094 */ int field_94;
    /* 0098 */ int field_98;
    /* 009C */ int field_9C;
    /* 00A0 */ int64_t target_obj;
    /* 00A8 */ int field_A8;
    /* 00AC */ int field_AC;
    /* 00B0 */ int field_B0;
    /* 00B4 */ int field_B4;
    /* 00B8 */ int field_B8;
    /* 00BC */ int field_BC;
    /* 00C0 */ int field_C0;
    /* 00C4 */ int field_C4;
    /* 00C8 */ int field_C8;
    /* 00CC */ int field_CC;
    /* 00D0 */ int field_D0;
    /* 00D4 */ int field_D4;
    /* 00D8 */ int field_D8;
    /* 00DC */ int field_DC;
    /* 00E0 */ int field_E0;
    /* 00E4 */ int field_E4;
    /* 00E8 */ int field_E8;
    /* 00EC */ int field_EC;
    /* 00F0 */ int field_F0;
    /* 00F4 */ int field_F4;
    /* 00F8 */ int field_F8;
    /* 00FC */ int field_FC;
    /* 0100 */ int field_100;
    /* 0104 */ int field_104;
    /* 0108 */ int field_108;
    /* 010C */ int field_10C;
    /* 0110 */ int field_110;
    /* 0114 */ int field_114;
    /* 0118 */ int field_118;
    /* 011C */ int field_11C;
    /* 0120 */ int field_120;
    /* 0124 */ int field_124;
    /* 0128 */ int field_128;
    /* 012C */ int field_12C;
    /* 0130 */ int field_130;
    /* 0134 */ int field_134;
    /* 0138 */ int field_138;
    /* 013C */ int field_13C;
    /* 0140 */ int field_140;
    /* 0144 */ int field_144;
    /* 0148 */ DateTime field_148;
    /* 0150 */ int field_150;
    /* 0154 */ int field_154;
} MagicTechLock;

static_assert(sizeof(MagicTechLock) == 0x158, "wrong size");

extern const char* off_5BA10C[31];
extern const char* off_5BA188[32];
extern const char* off_5BA208[4];
extern const char* off_5BA218[9];
extern const char* off_5BA23C[9];
extern const char* off_5BA260[9];
extern const char* off_5BA284[23];
extern const char* off_5BA2E0[10];
extern const char* off_5BA308[1];
extern const char* off_5BA30C[5];
extern const char* off_5BA320[1];
extern const char* off_5BA324[1];
extern const char* off_5BA328[1];
extern const char* off_5BA32C[1];
extern const char* off_5BA330[1];
extern const char* off_5BA334[5];
extern const char* off_5BA348[32];
extern const char* off_5BA3C8[27];
extern const char* off_5BA434[6];
extern const char* off_5BA44C[31];
extern const char* off_5BA4C8[1];

bool magictech_init(GameInitInfo* init_info);
void magictech_reset();
bool magictech_post_init(GameInitInfo* init_info);
void magictech_exit();
void magictech_break_nodes_to_map(const char* map);
void magictech_save_nodes_to_map(const char* map);
void magictech_load_nodes_from_map(const char* map);
void sub_44FDC0(MesFileEntry *mes_file_entry);
int magictech_get_range(int magictech);
int sub_4502B0(int magictech);
int sub_4502E0(int a1);
int magictech_get_iq(int magictech);
int magictech_get_cost(int magictech);
bool sub_450370(int magictech);
bool sub_4503A0(int magictech);
int* magictech_get_maintain1(int magictech);
int magictech_get_duration1(int magictech);
void sub_4507B0(object_id_t obj, int magictech);
bool sub_459500(int index);
void sub_459A20(int64_t obj);
void sub_459EA0(int64_t obj);
bool sub_459F20(int magictech, int* a2);
void sub_459F50();
bool sub_459F60(int magictech);
bool sub_459F90(int magictech);
bool sub_459FC0(int magictech);
bool sub_459FF0(int magictech);
bool sub_45A030(int magictech);
void sub_45A4F0(int64_t a1, int a2, int a3);
void sub_45A520(int a1, int a2, int a3, int a4);
void sub_45A540(int a1, int a2);
bool sub_45A580(int64_t a1, int64_t a2);
void magictech_debug_lists();

#endif /* ARCANUM_GAME_MAGICTECH_H_ */
