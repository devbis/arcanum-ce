#ifndef ARCANUM_GAME_MAGICTECH_H_
#define ARCANUM_GAME_MAGICTECH_H_

#include "game/combat.h"
#include "game/context.h"
#include "game/mes.h"
#include "game/mt_obj_node.h"
#include "game/obj.h"
#include "game/object.h"
#include "game/timeevent.h"

#define MT_80 80
#define MT_140 140
#define MT_SPELL_COUNT 223

typedef enum MagicTechAction {
    MAGICTECH_ACTION_BEGIN,
    MAGICTECH_ACTION_MAINTAIN,
    MAGICTECH_ACTION_END,
    MAGICTECH_ACTION_CALLBACK,
    MAGICTECH_ACTION_END_CALLBACK,
    MAGICTECH_ACTION_COUNT,
} MagicTechAction;

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

typedef struct MagicTechAoe {
    /* 0000 */ uint64_t flags;
    /* 0008 */ unsigned int spell_flags;
    /* 000C */ unsigned int no_spell_flags;
    /* 0010 */ int radius;
    /* 0014 */ int count;
} MagicTechAoe;

static_assert(sizeof(MagicTechAoe) == 0x18, "wrong size");

typedef struct MagicTechComponentTrait {
    /* 0004 */ int fld;
    /* 0004 */ int field_4;
    /* 0008 */ int field_8;
    /* 000C */ int field_C;
    /* 0010 */ int value;
    /* 0014 */ int palette;
} MagicTechComponentTrait;

typedef struct MagicTechComponentInfo {
    /* 0000 */ int type;
    /* 0008 */ MagicTechAoe aoe;
    /* 0020 */ MagicTechAoe apply_aoe;
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
        MagicTechComponentTrait trait;
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
} MagicTechComponentInfo;

// See 0x4580C6.
static_assert(sizeof(MagicTechComponentInfo) == 0x68, "wrong size");

typedef struct MagicTechComponentList {
    int cnt;
    MagicTechComponentInfo* entries;
} MagicTechComponentList;

// NOTE: Usage in 0x4CC310 implies array-like access.
typedef union MagicTechInfoAI {
    struct {
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
    };
    int values[10];
} MagicTechInfoAI;

// See 0x450090.
static_assert(sizeof(MagicTechInfoAI) == 0x28, "wrong size");

typedef struct MagicTechResistance {
    int stat;
    int value;
} MagicTechResistance;

static_assert(sizeof(MagicTechResistance) == 0x8, "wrong size");

typedef struct MagicTechMaintenanceInfo {
    /* 0000 */ int cost;
    /* 0004 */ int period;
} MagicTechMaintenanceInfo;

static_assert(sizeof(MagicTechMaintenanceInfo) == 0x8, "wrong size");

typedef struct MagicTechDurationInfo {
    /* 0000 */ int period;
    /* 0004 */ int stat;
    /* 0008 */ int level;
    /* 000C */ int modifier;
} MagicTechDurationInfo;

static_assert(sizeof(MagicTechDurationInfo) == 0x10, "wrong size");

typedef struct MagicTechCasterTargetPair {
    int caster;
    int target;
} MagicTechCasterTargetPair;

typedef uint32_t MagicTechFlags;

#define MAGICTECH_FRIENDLY 0x0001u
#define MAGICTECH_AGGRESSIVE 0x0002u
#define MAGICTECH_IS_TECH 0x0004u
#define MAGICTECH_IS_ENABLED 0x0008u
#define MAGICTECH_NO_RESIST 0x0200u
#define MAGICTECH_NO_REFLECT 0x0400u

typedef struct MagicTechInfo {
    /* 0000 */ const char* name;
    /* 0004 */ int iq;
    /* 0008 */ int cost;
    /* 000C */ MagicTechResistance resistance;
    /* 0014 */ MagicTechMaintenanceInfo maintenance;
    /* 001C */ MagicTechDurationInfo duration;
    /* 002C */ int duration_trigger_count;
    /* 0030 */ int range;
    /* 0034 */ MagicTechFlags flags;
    /* 0038 */ unsigned int item_triggers;
    /* 003C */ MagicTechCasterTargetPair pairs[MAGICTECH_ACTION_COUNT];
    /* 0064 */ int missile;
    /* 0068 */ int casting_anim;
    /* 006C */ int field_6C;
    /* 0070 */ MagicTechAoe field_70[MAGICTECH_ACTION_COUNT];
    /* 00E8 */ MagicTechComponentList components[MAGICTECH_ACTION_COUNT];
    /* 0110 */ int no_stack;
    /* 0114 */ int field_114;
    /* 0118 */ unsigned int cancels_sf;
    /* 011C */ unsigned int disallowed_sf;
    /* 0120 */ unsigned int disallowed_tsf;
    /* 0124 */ unsigned int disallowed_tcf;
    /* 0128 */ unsigned int cancels_envsf;
    /* 012C */ MagicTechInfoAI ai;
    /* 0154 */ int defensive2;
} MagicTechInfo;

static_assert(sizeof(MagicTechInfo) == 0x158, "wrong size");

typedef struct MagicTechRunInfo {
    /* 0000 */ int id;
    /* 0004 */ int spell;
    /* 0008 */ MagicTechAction action;
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
} MagicTechRunInfo;

static_assert(sizeof(MagicTechRunInfo) == 0x158, "wrong size");

typedef struct MagicTechSerializedData {
    /* 0000 */ int spell;
    /* 0004 */ int field_4;
    /* 0008 */ FollowerInfo source_obj;
    /* 0038 */ int64_t loc;
    /* 0040 */ FollowerInfo parent_obj;
    /* 0070 */ FollowerInfo target_obj;
    /* 00A0 */ FollowerInfo field_A0;
    /* 00D0 */ int64_t target_loc;
    /* 00D8 */ int field_D8;
    /* 00DC */ unsigned int flags;
} MagicTechSerializedData;

static_assert(sizeof(MagicTechSerializedData) == 0xE0, "wrong size");

typedef struct MagicTechSummonInfo {
    /* 0000 */ FollowerInfo field_0;
    /* 0030 */ FollowerInfo field_30;
    /* 0060 */ ObjectID field_60;
    /* 0078 */ int64_t loc;
    /* 0080 */ int64_t* summoned_obj_ptr;
    /* 0084 */ int field_84;
    /* 0088 */ ObjectID field_88;
    /* 00A0 */ int64_t field_A0;
    /* 00A8 */ ObjectID field_A8;
    /* 00C0 */ int palette;
    /* 00C4 */ int field_C4;
    /* 00C8 */ int field_C8;
    /* 00CC */ int field_CC;
} MagicTechSummonInfo;

static_assert(sizeof(MagicTechSummonInfo) == 0xD0, "wrong size");

extern MagicTechInfo* magictech_spells;
extern MagicTechRunInfo* magictech_run_info;

bool magictech_init(GameInitInfo* init_info);
void magictech_reset();
bool magictech_post_init(GameInitInfo* init_info);
void magictech_exit();
bool magictech_post_save(TigFile* stream);
bool magictech_post_load(GameLoadInfo* load_info);
void magictech_break_nodes_to_map(const char* map);
void magictech_save_nodes_to_map(const char* map);
void magictech_load_nodes_from_map(const char* map);
void magictech_get_msg(MesFileEntry *mes_file_entry);
char* magictech_spell_name(int num);
void sub_44FE20();
int magictech_get_range(int magictech);
int sub_4502B0(int magictech);
int sub_4502E0(int a1);
int magictech_get_iq(int magictech);
int magictech_get_cost(int magictech);
bool magictech_is_aggressive(int magictech);
bool sub_4503A0(int magictech);
MagicTechMaintenanceInfo* magictech_get_maintenance(int magictech);
MagicTechDurationInfo* magictech_get_duration(int magictech);
bool sub_450420(int64_t obj, int cost, bool a3, int magictech);
void sub_4507B0(int64_t obj, int magictech);
bool magictech_can_charge_spell_fatigue(int64_t obj, int magictech);
bool sub_450940(int mt_id);
int sub_450B40(int64_t obj);
void magictech_effect_summon(MagicTechSummonInfo* summon_info);
void sub_451070(MagicTechRunInfo* a1);
void sub_451BB0(int64_t obj, int mt_id);
void sub_452650(int64_t obj);
void magictech_process(int64_t obj, MagicTechComponentTrait* trait, int obj_type);
int sub_453B20(int64_t attacker_obj, int64_t target_obj, int spell);
int sub_453CC0(int64_t a1, int64_t item_obj, int64_t a3);
bool sub_454920(int64_t obj, int num, int max);
void magictech_component_obj_flag(int64_t a1, int64_t a2, int a3, int a4, int a5, int64_t a6, int64_t a7);
bool magictech_id_to_run_info(int mt_id, MagicTechRunInfo** lock_ptr);
void sub_455A20(MagicTechSerializedData* a1, int64_t obj, int a3);
void sub_455AC0(MagicTechSerializedData* a1);
bool sub_4564E0(MagicTechSerializedData* a1);
bool sub_456A10(int64_t a1, int64_t a2, int64_t a3);
bool sub_456A90(int mt_id);
bool sub_456BC0(MagicTechSerializedData* a1);
bool sub_456D20(int mt_id, tig_art_id_t* art_id_ptr, tig_art_id_t* light_art_id_ptr, tig_color_t* light_color_ptr, int* a5, int* a6, int* a7, int* a8);
void sub_456E00(int mt_id);
void sub_456E60(int64_t obj, int a2);
void sub_456EC0(int64_t obj, int spell);
void sub_456F70(int mt_id);
void sub_456FA0(int mt_id, unsigned int flags);
void sub_457000(int mt_id, int action);
void sub_457060(MagicTechRunInfo* a1);
void sub_457100();
void sub_457110(int mt_id);
void magictech_interrupt_delayed(int mt_id);
void sub_4573D0(MagicTechSerializedData* a1);
void sub_457450(int64_t obj);
void sub_4574D0(int64_t obj);
bool sub_458A80(unsigned int flags);
tig_art_id_t sub_458AE0(int mt_id);
const char* magictech_get_name(int magictech);
tig_art_id_t sub_458B70(int mt_id);
void sub_458C00(int spell, int64_t obj);
int sub_458CA0(int mt_id);
bool sub_459040(int64_t obj, unsigned int flags, int64_t* parent_obj_ptr);
bool sub_459170(int64_t obj, unsigned int flags, int* index_ptr);
bool sub_459380(int64_t obj, int magictech);
bool sub_4593F0(int64_t obj, int magictech);
bool magictech_timeevent_process(TimeEvent* timeevent);
bool sub_459500(int index);
bool magictech_recharge_timeevent_process(TimeEvent* timeevent);
void sub_459740(int64_t obj);
void sub_4598D0(int64_t obj);
void sub_459A20(int64_t obj);
void magictech_anim_play_hit_fx(int64_t obj, CombatContext* combat);
bool sub_459C10(int64_t obj, int mt_id);
void sub_459EA0(int64_t obj);
bool sub_459F20(int magictech, uint64_t** a2);
void sub_459F50();
bool magictech_is_enabled(int magictech);
bool magictech_is_magic(int magictech);
bool magictech_is_tech(int magictech);
bool sub_459FF0(int mt_id);
bool sub_45A030(int magictech);
int magictech_get_aptitude_adj(int64_t sector_id);
void sub_45A4F0(int64_t a1, int a2, int a3);
void sub_45A520(int64_t a1, int64_t a2);
void sub_45A540(int64_t a1);
bool sub_45A580(int64_t a1, int64_t a2);
void magictech_debug_lists();

#endif /* ARCANUM_GAME_MAGICTECH_H_ */
