#ifndef ARCANUM_GAME_MAGICTECH_H_
#define ARCANUM_GAME_MAGICTECH_H_

#include "game/context.h"
#include "game/obj.h"
#include "game/object.h"
#include "game/mes.h"
#include "game/mt_obj_node.h"
#include "game/timeevent.h"

#define MT_80 80
#define MT_140 140
#define MT_SPELL_COUNT 223

typedef enum MagicTechComponent {
    MTC_NOOP,
    MTC_AGOAL,
    MTC_AGOALTERMINATE,
    MTC_AIREDIRECT,
    MTC_CAST,
    MTC_CHARGENBRANCH,
    MTC_DAMAGE,
    MTC_DESTROY,
    MTC_DISPEL,
    MTC_EFFECT,
    MTC_ENVFLAG,
    MTC_EYECANDY,
    MTC_HEAL,
    MTC_IDENTIFY,
    MTC_INTERRUPT,
    MTC_MOVEMENT,
    MTC_OBJFLAG,
    MTC_RECHARGE,
    MTC_SUMMON,
    MTC_TERMINATE,
    MTC_TESTNBRANCH,
    MTC_TRAIT,
    MTC_TRAITIDX,
    MTC_TRAIT64,
    MTC_USE,
} MagicTechComponent;

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

typedef struct MagicTechEffectInfo {
    /* 0000 */ int type;
    /* 0004 */ int field_4;
    /* 0008 */ uint64_t aoe;
    /* 0010 */ unsigned int aoe_spell_flags;
    /* 0014 */ unsigned int aoe_no_spell_flags;
    /* 0018 */ int radius;
    /* 001C */ int count;
    /* 0020 */ uint64_t apply_aoe;
    /* 0028 */ int field_28;
    /* 002C */ int field_2C;
    /* 0030 */ int field_30;
    /* 0034 */ int field_34;
    /* 0038 */ unsigned int item_triggers;
    /* 003C */ int field_3C;
    union {
        struct {
            /* 0040 */ int goal;
            /* 0044 */ int subgoal;
        } agoal;
        struct {
            /* 0040 */ int goal;
        } agoal_terminate;
        struct {
            /* 0040 */ unsigned int critter_flags;
            /* 0044 */ int min_iq;
        } ai_redirect;
        struct {
            /* 0040 */ int spell;
        } cast;
        struct {
            /* 0040 */ int cost;
            /* 0044 */ int branch;
        } charge_branch;
        struct {
            /* 0040 */ int damage_min;
            /* 0044 */ int damage_max;
            /* 0048 */ int damage_type;
            /* 004C */ unsigned int damage_flags;
        } damage;
        struct {
            /* 0040 */ int num;
            /* 0044 */ int add_remove;
            /* 0048 */ int count;
            /* 004C */ int cause;
            /* 0050 */ int scaled;
        } effect;
        struct {
            /* 0040 */ unsigned int flags;
            /* 0044 */ int state;
        } env_flags;
        struct {
            /* 0040 */ int num;
            /* 0044 */ int add_remove;
            /* 0048 */ unsigned int flags;
        } eye_candy;
        struct {
            /* 0040 */ int damage_min;
            /* 0044 */ int damage_max;
            /* 0048 */ int damage_type;
            /* 004C */ unsigned int damage_flags;
        } heal;
        struct {
            /* 0040 */ int magictech;
        } interrupt;
        struct {
            /* 0040 */ int move_location;
            /* 0044 */ int tile_radius;
        } movement;
        struct {
            /* 0040 */ int flags_fld;
            /* 0044 */ unsigned int value;
            /* 0048 */ int state;
        } obj_flag;
        struct {
            /* 0040 */ int num;
            /* 0044 */ int max;
        } recharge;
        struct {
            /* 0040 */ ObjectID oid;
            /* 0058 */ int clear_faction;
            /* 005C */ int list;
            /* 0060 */ int palette;
        } summon;
        struct {
            /* 0040 */ int field_40;
            /* 0044 */ int field_44;
            /* 0048 */ int field_48;
            /* 004C */ int field_4C;
        } test_in_branch;
        struct {
            /* 0040 */ int field_40;
            /* 0044 */ int field_44;
            /* 0048 */ int field_48;
            /* 004C */ int field_4C;
            /* 0050 */ int field_50;
            /* 0054 */ int field_54;
        } trait;
        struct {
            /* 0040 */ int field_40;
            /* 0044 */ int field_44;
            /* 0048 */ int field_48;
            /* 004C */ int field_4C;
            /* 0050 */ int field_50;
            /* 0054 */ int field_54;
        } trait_idx;
        struct {
            /* 0040 */ int field_40;
            /* 0044 */ int field_44;
        } trait64;
    } data;
} MagicTechEffectInfo;

// See 0x4580C6.
static_assert(sizeof(MagicTechEffectInfo) == 0x68, "wrong size");

typedef struct MagicTechE8 {
    int field_0;
    MagicTechEffectInfo* field_4;
} MagicTechE8;

typedef struct MagicTechInfoAI {
    /* 0000 */ int flee;
    /* 0004 */ int summon;
    /* 0008 */ int defensive1;
    /* 000C */ int offensive;
    /* 0010 */ int healing_light;
    /* 0014 */ int healing_medium;
    /* 0018 */ int healing_heavy;
    /* 001C */ int cure_poison;
    /* 0020 */ int fatigue_recover;
    /* 0024 */ int resurrect;
    /* 0028 */ int defensive2;
} MagicTechInfoAI;

// See 0x450090.
static_assert(sizeof(MagicTechInfoAI) == 0x2C, "wrong size");

typedef struct MagicTechInfo {
    /* 0000 */ const char* field_0;
    /* 0004 */ int iq;
    /* 0008 */ int cost;
    /* 000C */ int resist_stat;
    /* 0010 */ int resist_value;
    /* 0014 */ int maintain[2];
    /* 001C */ int duration1;
    /* 0020 */ int duration2;
    /* 0024 */ int duration_stat;
    /* 0028 */ int duration_stat_value;
    /* 002C */ int duration_trigger_count;
    /* 0030 */ int range;
    /* 0034 */ unsigned int flags;
    /* 0038 */ unsigned int item_triggers;
    /* 003C */ int begin_caster;
    /* 0040 */ int begin_target;
    /* 0044 */ int maintain_caster;
    /* 0048 */ int maintain_target;
    /* 004C */ int end_caster;
    /* 0050 */ int end_target;
    /* 0054 */ int callback_caster;
    /* 0058 */ int callback_target;
    /* 005C */ int end_callback_caster;
    /* 0060 */ int end_callback_target;
    /* 0064 */ int missile;
    /* 0068 */ int casting_anim;
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
    /* 00A0 */ int field_A0;
    /* 00A4 */ int field_A4;
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
    /* 00E8 */ MagicTechE8 field_E8[5];
    /* 0110 */ int no_stack;
    /* 0114 */ int field_114;
    /* 0118 */ unsigned int cancels_sf;
    /* 011C */ unsigned int disallowed_sf;
    /* 0120 */ unsigned int disallowed_tsf;
    /* 0124 */ unsigned int disallowed_tcf;
    /* 0128 */ unsigned int cancels_envsf;
    /* 012C */ MagicTechInfoAI ai;
} MagicTechInfo;

static_assert(sizeof(MagicTechInfo) == 0x158, "wrong size");

typedef struct MagicTechLock {
    /* 0000 */ int field_0;
    /* 0004 */ int spell;
    /* 0008 */ int action;
    /* 000C */ int field_C;
    /* 0010 */ MagicTechObjectNode source_obj;
    /* 0058 */ MagicTechObjectNode parent_obj;
    /* 00A0 */ MagicTechObjectNode target_obj;
    /* 00E8 */ MagicTechObjectNode field_E8;
    /* 0130 */ MagicTechObjectNode* objlist;
    /* 0134 */ MagicTechObjectNode* summoned_obj;
    /* 0138 */ int field_138;
    /* 013C */ int field_13C;
    /* 0140 */ int field_140;
    /* 0144 */ int field_144;
    /* 0148 */ DateTime field_148;
    /* 0150 */ int field_150;
    /* 0154 */ int field_154;
} MagicTechLock;

static_assert(sizeof(MagicTechLock) == 0x158, "wrong size");

typedef struct MagicTechSerializedData {
    /* 0000 */ int field_0;
    /* 0004 */ int field_4;
    /* 0008 */ FollowerInfo field_8;
    /* 0038 */ int64_t loc;
    /* 0040 */ FollowerInfo field_40;
    /* 0070 */ FollowerInfo field_70;
    /* 00A0 */ FollowerInfo field_A0;
    /* 00D0 */ int64_t field_D0;
    /* 00D8 */ int field_D8;
    /* 00DC */ int field_DC;
} MagicTechSerializedData;

static_assert(sizeof(MagicTechSerializedData) == 0xE0, "wrong size");

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
extern MagicTechInfo* magictech_spells;
extern MagicTechLock* magictech_locks;

bool magictech_init(GameInitInfo* init_info);
void magictech_reset();
bool magictech_post_init(GameInitInfo* init_info);
void magictech_exit();
bool magictech_post_save(TigFile* stream);
bool magictech_post_load(GameLoadInfo* load_info);
void magictech_break_nodes_to_map(const char* map);
void magictech_save_nodes_to_map(const char* map);
void magictech_load_nodes_from_map(const char* map);
void sub_44FDC0(MesFileEntry *mes_file_entry);
const char* magictech_spell_name(int num);
void sub_44FE20();
int magictech_get_range(int magictech);
int sub_4502B0(int magictech);
int sub_4502E0(int a1);
int magictech_get_iq(int magictech);
int magictech_get_cost(int magictech);
bool sub_450370(int magictech);
bool sub_4503A0(int magictech);
int* magictech_get_maintain1(int magictech);
int magictech_get_duration1(int magictech);
bool sub_450420(int64_t obj, int cost, bool a3, int magictech);
void sub_4507B0(object_id_t obj, int magictech);
bool sub_450940(int magictech);
int sub_450B40(int64_t obj);
void sub_451070(MagicTechLock* a1);
bool sub_4557C0(int slot, MagicTechLock** lock_ptr);
void sub_455A20(MagicTechSerializedData* a1, int64_t obj, int a3);
void sub_455AC0(MagicTechSerializedData* a1);
bool sub_456A10(int64_t a1, int64_t a2, int64_t a3);
void sub_456E00(int magictech);
void sub_456E60(int64_t obj, int a2);
void sub_456FA0(int magictech, unsigned int flags);
void sub_457060(MagicTechLock* a1);
void sub_457100();
void sub_457110(int magictech);
void magictech_interrupt_delayed(int magictech);
void sub_457450(int64_t obj);
void sub_4574D0(int64_t obj);
bool sub_458A80(unsigned int flags);
tig_art_id_t sub_458AE0(int magictech);
const char* magictech_get_name(int magictech);
tig_art_id_t sub_458B70(int magictech);
int sub_458CA0(int magictech);
bool sub_459040(int64_t a1, unsigned int flags, int64_t* a3);
bool sub_459170(int64_t a1, unsigned int flags, int64_t* a3);
bool sub_459380(int64_t obj, int magictech);
bool sub_4593F0(int64_t obj, int a2);
bool magictech_timeevent_process(TimeEvent* timeevent);
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
int magictech_get_aptitude_adj(int64_t sector_id);
void sub_45A4F0(int64_t a1, int a2, int a3);
void sub_45A520(int64_t a1, int64_t a2);
void sub_45A540(int64_t a1);
bool sub_45A580(int64_t a1, int64_t a2);
void magictech_debug_lists();

#endif /* ARCANUM_GAME_MAGICTECH_H_ */
