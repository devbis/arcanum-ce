#include "game/magictech.h"

#include "game/animfx.h"
#include "game/combat.h"
#include "game/effect.h"
#include "game/mes.h"
#include "game/mt_item.h"
#include "game/object.h"
#include "game/sector.h"
#include "game/spell.h"
#include "game/stat.h"
#include "game/timeevent.h"

typedef enum MagicTechFlagCollection {
    MTFC_FLAGS,
    MTFC_SPELL_FLAGS,
    MTFC_WALL_FLAGS,
    MTFC_PORTAL_FLAGS,
    MTFC_CONTAINER_FLAGS,
    MTFC_SCENERY_FLAGS,
    MTFC_ITEM_FLAGS,
    MTFC_WEAPON_FLAGS,
    MTFC_AMMO_FLAGS,
    MTFC_ARMOR_FLAGS,
    MTFC_GOLD_FLAGS,
    MTFC_FOOD_FLAGS,
    MTFC_SCROLL_FLAGS,
    MTFC_KEY_RING_FLAGS,
    MTFC_WRITTEN_FLAGS,
    MTFC_GENERIC_FLAGS,
    MTFC_CRITTER_FLAGS,
    MTFC_CRITTER_FLAGS2,
    MTFC_PC_FLAGS,
    MTFC_NPC_FLAGS,
    MTFC_TRAP_FLAGS,
    MTFC_COUNT,
} MagicTechFlagCollection;

static bool sub_44F3C0(MagicTechLock* lock, TigFile* stream);
static bool sub_44F620(MagicTechLock* lock, TigFile* stream);
static bool sub_44FE30(int a1, const char* path, int a3);
static bool sub_44FFA0(int a1, const char* a2, int a3);
static void sub_450090(mes_file_handle_t msg_file, MagicTechInfo* info, int num, int magictech);
static void sub_4501D0(mes_file_handle_t msg_file, MagicTechInfo* info, int num, int magictech);
static void sub_450240();
static bool sub_4507D0(object_id_t obj, int magictech);
static void sub_455710();
static void sub_457530(int magictech);
static void sub_4578F0(MagicTechInfo* info, char* str);
static void sub_457B20(MagicTechInfo* info, char* str);
static void sub_457D00(MagicTechInfo* info, char* str);
static void magictech_build_ai_info(MagicTechInfo* info, char* str);
static void magictech_build_effect_info(MagicTechInfo* info, char* str);
static void sub_459490(int magictech);
static bool sub_4594D0(TimeEvent* timeevent);
static bool sub_459640(TimeEvent* timeevent);
static void sub_45A480(MagicTechLock* a1);
static void sub_45A760(object_id_t obj, const char* msg);

// 0x596140
static uint64_t qword_596140[] = {
    0x00,
    0x01,
    0x02,
    0x04,
    0x0C,
    0x10,
    0x24,
    0x44,
    0x0C4,
    0x144,
    0x244,
    0x444,
    0x844,
    0x1044,
    0x2044,
    0x4044,
    0x8044,
    0x10004,
    0x20004,
    0x40004,
    0x80004,
    0x100004,
    0x200004,
    0x400004,
    0x800004,
    0x1000004,
    0x2000004,
    0x4000004,
    0x8000004,
    0x10000004,
    0x20000004,
    0x40000044,
    0x80000004,
    0x100000044,
    0x200000044,
    0x400000044,
    0x800000044,
    0x1000000044,
    0x2000000044,
    0x4000000044,
    0x8000000004,
    0x10000000004,
    0x20000000004,
    0x40000000004,
    0x80000000004,
    0x100000000004,
    0x200000000004,
    0x400000000004,
    0x800000000000,
    0x1000000000000,
    0x2800000000000,
    0x4800000000000,
    0x8800000000000,
    0x10800000000000,
    0x20800000000000,
    0x40800000000000,
    0x80800000000000,
    0x100800000000000,
    0x200000000000000,
    0x400000000000044,
    0x800000000000044,
    0x1000000000000004,
    0x2000000000000004,
    0x4000000000000004,
    0x8000000000000000,
};

// 0x5B0BA0
static int dword_5B0BA0 = -1;

// 0x5B0BA4
static int dword_5B0BA4 = -1;

// 0x5B0C0C
static const char* off_5B0C0C[] = {
    "[Begin]",
    "[Maintain]",
    "[End]",
    "[Callback]",
    "[EndCallback]",
};

// 0x5B0C20
static const char* off_5B0C20[] = {
    "neutral",
    "friendly",
    "aggressive",
    "may_be_aggressive",
};

// 0x5B0C30
static int dword_5B0C30[] = {
    0,
    1,
    2,
    3,
};

// 0x5B0C40
static const char* off_5B0C40[] = {
    "anim_goal_animate",
    "anim_goal_knockback",
    "anim_goal_follow",
    "anim_goal_floating",
    "anim_goal_destroy_obj",
    "anim_goal_knock_down",
};

// 0x5B0C58
static int dword_5B0C58[6] = {
    0,
    43,
    24,
    44,
    32,
    62,
};

// 0x5B0C70
static const char* off_5B0C70[] = {
    "Dmg_Normal",
    "Dmg_Poison",
    "Dmg_Electrical",
    "Dmg_Fire",
    "Dmg_Magic",
    "Dmg_Acid",
};

// 0x5B0C88
static const char* off_5B0C88[] = {
    "Remove",
    "Add",
};

// 0x5B0C90
static const char* off_5B0C90[] = {
    "FLAG_OFF",
    "FLAG_ON",
};

// 0x5B0C98
static const char* off_5B0C98[] = {
    "Equal",
    "GreaterThan",
    "GreaterThanOrEqual",
    "LessThan",
    "LessThanOrEqual",
};

// 0x5B0CAC
static const char* off_5B0CAC[] = {
    "art_num",
    "Teleport_Tile",
    "obj_f_critter_fleeing_from",
    "HP_Damage",
};

// 0x5B0CBC
static int dword_5B0CBC[] = {
    OBJ_F_CURRENT_AID,
    OBJ_F_CRITTER_TELEPORT_DEST,
    OBJ_F_CRITTER_FLEEING_FROM,
    OBJ_F_HP_DAMAGE,
};

// 0x5B0CCC
static const char* off_5B0CCC[] = {
    "obj_f_critter_stat_base_idx",
    "obj_f_resistance_idx",
};

// 0x5B0CD4
static int dword_5B0CD4[] = {
    OBJ_F_CRITTER_STAT_BASE_IDX,
    OBJ_F_RESISTANCE_IDX,
};

// 0x5B0CDC
static const char* off_5B0CDC[] = {
    "stat_strength",
    "resist_damage",
};

// 0x5B0CE4
static int dword_5B0CE4[] = {
    28,
    5,
};

// 0x5B0CF8
static const char* off_5B0CF8[] = {
    "Target_Tile",
    "Caster_Obj",
    "Caster_Owner_Obj",
};

// 0x5B0D04
static const char* off_5B0D04[] = {
    "Target_Tile",
    "Random_Tile_In_Radius_Self",
    "Teleport_Tile",
    "Teleport_Area_Portal",
};

// 0x5B0D14
static const char* off_5B0D14[] = {
    "None",
    "Full",
    "Resurrect",
    "Reanimate",
    "Death",
    "Stunned",
    "Scars",
    "Scaled",
    "Weap_Dropped",
    "Crippling",
};

// 0x5B0D3C
static int dword_5B0D3C[] = {
    0,
    1,
    2,
    0x400002,
    4,
    0x40,
    0x1000,
    0x800000,
    0x2000,
    0x0E00,
};

// 0x5B0DC8
static int dword_5B0DC8[] = {
    0,
    4,
    5,
    6,
    3,
    7,
};

// 0x5B0DE0
static int dword_5B0DE0[] = {
    6,
    9,
    12,
    15,
    18,
};

// 0x5B0F20
static int dword_5B0F20 = -1;

// 0x5B72AC
static const char* off_5B72AC[] = {
    "None",
    "User_Activate",
    "Wear",
    "Unwear",
    "Pickup",
    "Drop",
    "Parent_Hit",
    "Parent_Stunned",
    "Parent_Going_Unconscious",
    "Parent_Dying",
    "Parent_Hit_By_Atk_Spell",
    "Parent_Atks_Opponent",
    "Parent_Atks_Location",
    "Parent_Dmgs_Opponent",
    "Parent_Dmgs_Opponent_W_Item",
    "Target_Hit",
    "Target_Going_Unconscious",
    "Item_Used",
    "Target_Attacker",
    "Target_Attacker_Weapon",
    "Target_Attacker_Armor",
    "Target_Attacker_Weapon_Melee",
    "Random_Chance_Rare",
    "Random_Chance_Uncommon",
    "Random_Chance_Common",
    "Random_Chance_Frequent",
};

// 0x5B7314
static unsigned int dword_5B7314[] = {
    0x0,
    0x10000000,
    0x20000000,
    0x40000000,
    0x80000000,
    0x1000000,
    0x2000000,
    0x4000000,
    0x8000000,
    0x100000,
    0x200000,
    0x400000,
    0x800000,
    0x10000,
    0x20000,
    0x40000,
    0x80000,
    0x3000,
    0x2000,
    0x4000,
    0x8000,
    0x100,
    0x200,
    0x400,
    0x800,
    0x10,
};

// 0x5B9FBC
static const char* off_5B9FBC[] = {
    "obj_f_flags",
    "obj_f_spell_flags",
    "obj_f_wall_flags",
    "obj_f_portal_flags",
    "obj_f_container_flags",
    "obj_f_scenery_flags",
    "obj_f_item_flags",
    "obj_f_weapon_flags",
    "obj_f_ammo_flags",
    "obj_f_armor_flags",
    "obj_f_gold_flags",
    "obj_f_food_flags",
    "obj_f_scroll_flags",
    "obj_f_key_ring_flags",
    "obj_f_written_flags",
    "obj_f_generic_flags",
    "obj_f_critter_flags",
    "obj_f_critter_flags2",
    "obj_f_pc_flags",
    "obj_f_npc_flags",
    "obj_f_trap_flags",
};

// 0x5BA010
static int dword_5BA010[] = {
    OBJ_F_FLAGS,
    OBJ_F_SPELL_FLAGS,
    OBJ_F_WALL_FLAGS,
    OBJ_F_PORTAL_FLAGS,
    OBJ_F_CONTAINER_FLAGS,
    OBJ_F_SCENERY_FLAGS,
    OBJ_F_ITEM_FLAGS,
    OBJ_F_WEAPON_FLAGS,
    OBJ_F_AMMO_FLAGS,
    OBJ_F_ARMOR_FLAGS,
    OBJ_F_GOLD_FLAGS,
    OBJ_F_FOOD_FLAGS,
    OBJ_F_SCROLL_FLAGS,
    OBJ_F_KEY_RING_FLAGS,
    OBJ_F_WRITTEN_FLAGS,
    OBJ_F_GENERIC_FLAGS,
    OBJ_F_CRITTER_FLAGS,
    OBJ_F_CRITTER_FLAGS2,
    OBJ_F_PC_FLAGS,
    OBJ_F_NPC_FLAGS,
    OBJ_F_TRAP_FLAGS,
};

// 0x5BA064
static const char** off_5BA064[] = {
    off_5BA10C,
    off_5BA188,
    off_5BA208,
    off_5BA218,
    off_5BA23C,
    off_5BA260,
    off_5BA284,
    off_5BA2E0,
    off_5BA308,
    off_5BA30C,
    off_5BA320,
    off_5BA324,
    off_5BA328,
    off_5BA32C,
    off_5BA330,
    off_5BA334,
    off_5BA348,
    off_5BA3C8,
    off_5BA434,
    off_5BA44C,
    off_5BA4C8,
};

// 0x5BA0B8
static int dword_5BA0B8[] = {
    sizeof(off_5BA10C) / sizeof(off_5BA10C[0]),
    sizeof(off_5BA188) / sizeof(off_5BA188[0]),
    sizeof(off_5BA208) / sizeof(off_5BA208[0]),
    sizeof(off_5BA218) / sizeof(off_5BA218[0]),
    sizeof(off_5BA23C) / sizeof(off_5BA23C[0]),
    sizeof(off_5BA260) / sizeof(off_5BA260[0]),
    sizeof(off_5BA284) / sizeof(off_5BA284[0]),
    sizeof(off_5BA2E0) / sizeof(off_5BA2E0[0]),
    sizeof(off_5BA308) / sizeof(off_5BA308[0]),
    sizeof(off_5BA30C) / sizeof(off_5BA30C[0]),
    sizeof(off_5BA320) / sizeof(off_5BA320[0]),
    sizeof(off_5BA324) / sizeof(off_5BA324[0]),
    sizeof(off_5BA328) / sizeof(off_5BA328[0]),
    sizeof(off_5BA32C) / sizeof(off_5BA32C[0]),
    sizeof(off_5BA330) / sizeof(off_5BA330[0]),
    sizeof(off_5BA334) / sizeof(off_5BA334[0]),
    sizeof(off_5BA348) / sizeof(off_5BA348[0]),
    sizeof(off_5BA3C8) / sizeof(off_5BA3C8[0]),
    sizeof(off_5BA434) / sizeof(off_5BA434[0]),
    sizeof(off_5BA44C) / sizeof(off_5BA44C[0]),
    sizeof(off_5BA4C8) / sizeof(off_5BA4C8[0]),
};

// 0x5BA10C
const char* off_5BA10C[] = {
    "OF_DESTROYED",
    "OF_OFF",
    "OF_FLAT",
    "OF_TEXT",
    "OF_SEE_THROUGH",
    "OF_SHOOT_THROUGH",
    "OF_TRANSLUCENT",
    "OF_SHRUNK",
    "OF_DONTDRAW",
    "OF_INVISIBLE",
    "OF_NO_BLOCK",
    "OF_CLICK_THROUGH",
    "OF_INVENTORY",
    "OF_DYNAMIC",
    "OF_PROVIDES_COVER",
    "OF_HAS_OVERLAYS",
    "OF_HAS_UNDERLAYS",
    "OF_WADING",
    "OF_WATER_WALKING",
    "OF_STONED",
    "OF_DONTLIGHT",
    "OF_TEXT_FLOATER",
    "OF_INVULNERABLE",
    "OF_EXTINCT",
    "OF_TRAP_PC",
    "OF_TRAP_SPOTTED",
    "OF_DISALLOW_WADING",
    "OF_MULTIPLAYER_LOCK",
    "OF_FROZEN",
    "OF_ANIMATED_DEAD",
    "OF_TELEPORTED",
};

// 0x5BA188
const char* off_5BA188[] = {
    "OSF_INVISIBLE",
    "OSF_FLOATING",
    "OSF_BODY_OF_AIR",
    "OSF_BODY_OF_EARTH",
    "OSF_BODY_OF_FIRE",
    "OSF_BODY_OF_WATER",
    "OSF_DETECTING_MAGIC",
    "OSF_DETECTING_ALIGNMENT",
    "OSF_DETECTING_TRAPS",
    "OSF_DETECTING_INVISIBLE",
    "OSF_SHIELDED",
    "OSF_ANTI_MAGIC_SHELL",
    "OSF_BONDS_OF_MAGIC",
    "OSF_FULL_REFLECTION",
    "OSF_SUMMONED",
    "OSF_ILLUSION",
    "OSF_STONED",
    "OSF_POLYMORPHED",
    "OSF_MIRRORED",
    "OSF_SHRUNK",
    "OSF_PASSWALLED",
    "OSF_WATER_WALKING",
    "OSF_MAGNETIC_INVERSION",
    "OSF_CHARMED",
    "OSF_ENTANGLED",
    "OSF_SPOKEN_WITH_DEAD",
    "OSF_TEMPUS_FUGIT",
    "OSF_MIND_CONTROLLED",
    "OSF_DRUNK",
    "OSF_ENSHROUDED",
    "OSF_FAMILIAR",
    "OSF_HARDENED_HANDS",
};

// 0x5BA208
const char* off_5BA208[] = {
    "OWAF_TRANS_DISALLOW",
    "OWAF_TRANS_LEFT",
    "OWAF_TRANS_RIGHT",
    "OWAF_TRANS_ALL",
};

// 0x5BA218
const char* off_5BA218[] = {
    "OPF_LOCKED",
    "OPF_JAMMED",
    "OPF_MAGICALLY_HELD",
    "OPF_NEVER_LOCKED",
    "OPF_ALWAYS_LOCKED",
    "OPF_LOCKED_DAY",
    "OPF_LOCKED_NIGHT",
    "OPF_BUSTED",
    "OPF_STICKY",
};

// 0x5BA23C
const char* off_5BA23C[] = {
    "OCOF_LOCKED",
    "OCOF_JAMMED",
    "OCOF_MAGICALLY_HELD",
    "OCOF_NEVER_LOCKED",
    "OCOF_ALWAYS_LOCKED",
    "OCOF_LOCKED_DAY",
    "OCOF_LOCKED_NIGHT",
    "OCOF_BUSTED",
    "OCOF_STICKY",
};

// 0x5BA260
const char* off_5BA260[] = {
    "OSCF_NO_AUTO_ANIMATE",
    "OSCF_BUSTED",
    "OSCF_NOCTURNAL",
    "OSCF_MARKS_TOWNMAP",
    "OSCF_IS_FIRE",
    "OSCF_RESPAWNABLE",
    "OSCF_SOUND_SMALL",
    "OSCF_SOUND_MEDIUM",
    "OSCF_SOUND_EXTRA_LARGE",
};

// 0x5BA284
const char* off_5BA284[] = {
    "OIF_IDENTIFIED",
    "OIF_WONT_SELL",
    "OIF_IS_MAGICAL",
    "OIF_TRANSFER_LIGHT",
    "OIF_NO_DISPLAY",
    "OIF_NO_DROP",
    "OIF_HEXED",
    "OIF_CAN_USE_BOX",
    "OIF_NEEDS_TARGET",
    "OIF_LIGHT_SMALL",
    "OIF_LIGHT_MEDIUM",
    "OIF_LIGHT_LARGE",
    "OIF_LIGHT_XLARGE",
    "OIF_PERSISTENT",
    "OIF_MT_TRIGGERED",
    "OIF_STOLEN",
    "OIF_USE_IS_THROW",
    "OIF_NO_DECAY",
    "OIF_UBER",
    "OIF_NO_NPC_PICKUP",
    "OIF_NO_RANGED_USE",
    "OIF_VALID_AI_ACTION",
    "OIF_MP_INSERTED",
};

// 0x5BA2E0
const char* off_5BA2E0[] = {
    "OWF_LOUD",
    "OWF_SILENT",
    "OWF_TWO_HANDED",
    "OWF_HAND_COUNT_FIXED",
    "OWF_THROWABLE",
    "OWF_TRANS_PROJECTILE",
    "OWF_BOOMERANGS",
    "OWF_IGNORE_RESISTANCE",
    "OWF_DAMAGE_ARMOR",
    "OWF_DEFAULT_THROWS",
};

// 0x5BA308
const char* off_5BA308[] = {
    "OAF_NONE",
};

// 0x5BA30C
const char* off_5BA30C[] = {
    "OARF_SIZE_SMALL",
    "OARF_SIZE_MEDIUM",
    "OARF_SIZE_LARGE",
    "OARF_MALE_ONLY",
    "OARF_FEMALE_ONLY",
};

// 0x5BA320
const char* off_5BA320[] = {
    "OGOF_NONE",
};

// 0x5BA324
const char* off_5BA324[] = {
    "OFF_NONE",
};

// 0x5BA328
const char* off_5BA328[] = {
    "OSRF_NONE",
};

// 0x5BA32C
const char* off_5BA32C[] = {
    "OKRF_PRIMARY_RING",
};

// 0x5BA330
const char* off_5BA330[] = {
    "OWRF_NONE"
};

// 0x5BA334
const char* off_5BA334[] = {
    "OGF_USES_TORCH_SHIELD_LOCATION",
    "OGF_IS_LOCKPICK",
    "OGF_IS_TRAP_DEVICE",
    NULL,
    NULL,
};

// 0x5BA348
const char* off_5BA348[] = {
    "OCF_IS_CONCEALED",
    "OCF_MOVING_SILENTLY",
    "OCF_UNDEAD",
    "OCF_ANIMAL",
    "OCF_FLEEING",
    "OCF_STUNNED",
    "OCF_PARALYZED",
    "OCF_BLINDED",
    "OCF_CRIPPLED_ARMS_ONE",
    "OCF_CRIPPLED_ARMS_BOTH",
    "OCF_CRIPPLED_LEGS_BOTH",
    "OCF_UNUSED",
    "OCF_SLEEPING",
    "OCF_MUTE",
    "OCF_SURRENDERED",
    "OCF_MONSTER",
    "OCF_SPELL_FLEE",
    "OCF_ENCOUNTER",
    "OCF_COMBAT_MODE_ACTIVE",
    "OCF_LIGHT_SMALL",
    "OCF_LIGHT_MEDIUM",
    "OCF_LIGHT_LARGE",
    "OCF_LIGHT_XLARGE",
    "OCF_UNREVIVIFIABLE",
    "OCF_UNRESSURECTABLE",
    "OCF_DEMON",
    "OCF_FATIGUE_IMMUNE",
    "OCF_NO_FLEE",
    "OCF_NON_LETHAL_COMBAT",
    "OCF_MECHANICAL",
    "OCF_ANIMAL_ENSHROUD",
    "OCF_FATIGUE_LIMITING",
};

// 0x5BA3C8
const char* off_5BA3C8[] = {
    "OCF2_ITEM_STOLEN",
    "OCF2_AUTO_ANIMATES",
    "OCF2_USING_BOOMERANG",
    "OCF2_FATIGUE_DRAINING",
    "OCF2_SLOW_PARTY",
    "OCF2_COMBAT_TOGGLE_FX",
    "OCF2_NO_DECAY",
    "OCF2_NO_PICKPOCKET",
    "OCF2_NO_BLOOD_SPLOTCHES",
    "OCF2_NIGH_INVULNERABLE",
    "OCF2_ELEMENTAL",
    "OCF2_DARK_SIGHT",
    "OCF2_NO_SLIP",
    "OCF2_NO_DISINTEGRATE",
    "OCF2_REACTION_0",
    "OCF2_REACTION_1",
    "OCF2_REACTION_2",
    "OCF2_REACTION_3",
    "OCF2_REACTION_4",
    "OCF2_REACTION_5",
    "OCF2_REACTION_6",
    "OCF2_TARGET_LOCK",
    "OCF2_PERMA_POLYMORPH",
    "OCF2_SAFE_OFF",
    "OCF2_CHECK_REACTION_BAD",
    "OCF2_CHECK_ALIGN_GOOD",
    "OCF2_CHECK_ALIGN_BAD",
};

// 0x5BA434
const char* off_5BA434[] = {
    "OPCF_unused_1",
    "OPCF_unused_2",
    "OPCF_USE_ALT_DATA",
    "OPCF_unused_4",
    "OPCF_unused_5",
    "OPCF_FOLLOWER_SKILLS_ON",
};

// 0x5BA44C
const char* off_5BA44C[] = {
    "ONF_FIGHTING",
    "ONF_WAYPOINTS_DAY",
    "ONF_WAYPOINTS_NIGHT",
    "ONF_AI_WAIT_HERE",
    "ONF_AI_SPREAD_OUT",
    "ONF_JILTED",
    "ONF_CHECK_WIELD",
    "ONF_CHECK_WEAPON",
    "ONF_KOS",
    "ONF_WAYPOINTS_BED",
    "ONF_FORCED_FOLLOWER",
    "ONF_KOS_OVERRIDE",
    "ONF_WANDERS",
    "ONF_WANDERS_IN_DARK",
    "ONF_FENCE",
    "ONF_FAMILIAR",
    "ONF_CHECK_LEADER",
    "ONF_ALOOF",
    "ONF_CAST_HIGHEST",
    "ONF_GENERATOR",
    "ONF_GENERATED",
    "ONF_GENERATOR_RATE1",
    "ONF_GENERATOR_RATE2",
    "ONF_GENERATOR_RATE3",
    "ONF_DEMAINTAIN_SPELLS",
    "ONF_LOOK_FOR_WEAPON",
    "ONF_LOOK_FOR_ARMOR",
    "ONF_LOOK_FOR_AMMO",
    "ONF_BACKING_OFF",
    "ONF_NO_ATTACK",
    NULL,
};

// 0x5BA4C8
const char* off_5BA4C8[] = {
    "OTF_BUSTED",
};

// 0x5BBD70
const char* off_5BBD70[] = {
    "Tgt_None",
    "Tgt_Self",
    "Tgt_Source",
    "Tgt_Object",
    "Tgt_Obj_Self",
    "Tgt_Obj_Radius",
    "Tgt_Obj_T_PC",
    "Tgt_Obj_T_Critter",
    "Tgt_Obj_ST_Critter_Animal",
    "Tgt_Obj_ST_Critter_Dead",
    "Tgt_Obj_ST_Critter_Undead",
    "Tgt_Obj_ST_Critter_Demon",
    "Tgt_Obj_ST_Critter_Mechanical",
    "Tgt_Obj_ST_Critter_Good",
    "Tgt_Obj_ST_Critter_Evil",
    "Tgt_Obj_ST_Critter_Unrevivifiable",
    "Tgt_Obj_ST_Critter_Unresurrectable",
    "Tgt_Obj_T_Portal",
    "Tgt_Obj_T_Container",
    "Tgt_Obj_ST_Openable_Locked",
    "Tgt_Obj_T_Wall",
    "Tgt_Obj_Damaged",
    "Tgt_Obj_Damaged_Poisoned",
    "Tgt_Obj_Poisoned",
    "Tgt_Obj_M_Stone",
    "Tgt_Obj_M_Flesh",
    "Tgt_Obj_Inven",
    "Tgt_Obj_Weight_Below_5",
    "Tgt_Obj_In_Wall",
    "Tgt_Obj_No_Self",
    "Tgt_Obj_No_T_PC",
    "Tgt_Obj_No_ST_Critter_Animal",
    "Tgt_Obj_No_ST_Critter_Dead",
    "Tgt_Obj_No_ST_Critter_Undead",
    "Tgt_Obj_No_ST_Critter_Demon",
    "Tgt_Obj_No_ST_Critter_Mechanical",
    "Tgt_Obj_No_ST_Critter_Good",
    "Tgt_Obj_No_ST_Critter_Evil",
    "Tgt_Obj_No_ST_Critter_Unrevivifiable",
    "Tgt_Obj_No_ST_Critter_Unresurrectable",
    "Tgt_Obj_No_ST_Openable_Locked",
    "Tgt_Obj_No_ST_Magically_Held",
    "Tgt_Obj_No_T_Wall",
    "Tgt_Obj_No_Damaged",
    "Tgt_Obj_No_M_Stone",
    "Tgt_Obj_No_Inven",
    "Tgt_Obj_No_Invulnerable",
    "Tgt_Summoned",
    "Tgt_Tile",
    "Tgt_Tile_Self",
    "Tgt_Tile_Pathable_To",
    "Tgt_Tile_Empty",
    "Tgt_Tile_Empty_Immobiles",
    "Tgt_Tile_No_Empty",
    "Tgt_Tile_Radius",
    "Tgt_Tile_Radius_Wall",
    "Tgt_Tile_Offscreen",
    "Tgt_Tile_Indoor_Or_Outdoor_Match",
    "Tgt_Cone",
    "Tgt_All_Party_Critters",
    "Tgt_Party_Critter",
    "Tgt_Non_Party_Critters",
    "Tgt_Parent_Obj",
    "Tgt_Attacker_Obj",
    "Tgt_List",
};

// 0x5E3510
static bool magictech_editor;

// 0x5E6D20
static mes_file_handle_t dword_5E6D20;

// 0x5E6D24
static GameContextF8* dword_5E6D24;

// 0x5E7568
static AnimFxList stru_5E7568;

// 0x5E7594
static int magictech_spell_mes_file;

// 0x5E75AC
static int dword_5E75AC;

// 0x5E75CC
static int dword_5E75CC;

// 0x5E75D4
static int dword_5E75D4;

// 0x5E75D8
static int dword_5E75D8;

// 0x5E75DC
static int dword_5E75DC;

// 0x5E75E0
static int dword_5E75E0;

// 0x5E75E4
static int dword_5E75E4;

// 0x5E75E8
static int dword_5E75E8;

// 0x5E75F0
static MagicTechLock* dword_5E75F0;

// 0x5E75F4
static mes_file_handle_t magictech_mes_file;

// 0x5E75F8
static char** magictech_component_names;

// 0x5E75FC
static bool dword_5E75FC;

// 0x5E7608
static int dword_5E7608;

// 0x5E760C
static int dword_5E760C;

// 0x5E7610
static int64_t qword_5E7610;

// 0x5E7618
static bool magictech_initialized;

// 0x5E7628
static int dword_5E7628;

// 0x5E7634
static int dword_5E7634;

// 0x6876D8
MagicTechInfo* magictech_spells;

// 0x6876DC
static int dword_6876DC;

// 0x6876E0
MagicTechLock* magictech_locks;

// 0x44EF50
bool magictech_init(GameContext* init_info)
{
    MesFileEntry mes_file_entry;
    int index;

    dword_5E6D24 = init_info->field_8;
    magictech_editor = init_info->editor;
    magictech_component_names = (char**)CALLOC(25, sizeof(char*));
    magictech_locks = (MagicTechLock*)CALLOC(512, sizeof(MagicTechLock));
    sub_455710();

    if (!mes_load("Rules\\magictech.mes", &magictech_mes_file)) {
        FREE(magictech_component_names);
        FREE(magictech_locks);
        return false;
    }

    if (!mes_load("mes\\spell.mes", &magictech_spell_mes_file)) {
        FREE(magictech_component_names);
        FREE(magictech_locks);
        return false;
    }

    mes_file_entry.num = 10;
    for (index = 0; index < 25; index++) {
        if (!mes_search(magictech_mes_file, &mes_file_entry)) {
            FREE(magictech_component_names);
            FREE(magictech_locks);
            return false;
        }

        magictech_component_names[index] = mes_file_entry.str;
        mes_file_entry.num++;
    }

    if (!magictech_editor) {
        if (!animfx_list_init(&stru_5E7568)) {
            FREE(magictech_component_names);
            FREE(magictech_locks);
            return false;
        }

        stru_5E7568.path = "Rules\\SpellEyeCandy.mes";
        stru_5E7568.field_18 = 1344;
        stru_5E7568.field_C = 6;
        stru_5E7568.field_10 = 10;
        stru_5E7568.field_20 = 6;
        stru_5E7568.field_24 = dword_5B0DC8;
        if (!animfx_list_load(&stru_5E7568)) {
            FREE(magictech_component_names);
            FREE(magictech_locks);
            return false;
        }
    }

    magictech_spells = (MagicTechInfo*)MALLOC(sizeof(MagicTechInfo) * MT_SPELL_COUNT);

    if (!sub_44FE30(0, "Rules\\spelllist.mes", 1000)) {
        FREE(magictech_component_names);
        FREE(magictech_locks);
        return false;
    }

    magictech_initialized = true;
    dword_6876DC = 0;

    return true;
}

// 0x44F150
void magictech_reset()
{
    dword_5E75FC = false;
}

// 0x44F160
bool magictech_post_init(GameInitInfo* init_info)
{
    (void)init_info;

    if (!sub_44FFA0(0, "Rules\\spelllist.mes", 1000)) {
        FREE(magictech_component_names);
        FREE(magictech_locks);
        magictech_initialized = false;
        return false;
    }

    return true;
}

// 0x44F1B0
void magictech_exit()
{
    if (magictech_initialized) {
        if (!magictech_editor) {
            if (stru_5E7568.field_14) {
                animfx_list_exit(&stru_5E7568);
            }
        }

        sub_450240();

        mes_unload(magictech_spell_mes_file);
        mes_unload(magictech_mes_file);

        if (magictech_spells != NULL) {
            FREE(magictech_spells);
            magictech_spells = NULL;
        }

        if (magictech_component_names != NULL) {
            FREE(magictech_component_names);
        }

        if (magictech_locks != NULL) {
            FREE(magictech_locks);
        }

        magictech_initialized = false;
    }
}

// 0x44F250
bool magictech_post_save(TigFile* stream)
{
    int cnt;
    int index;
    int start;
    int extent;

    if (stream == NULL) {
        return false;
    }

    if (tig_file_fwrite(&dword_6876DC, sizeof(dword_6876DC), 1, stream) != 1) {
        return false;
    }

    cnt = 512;
    if (tig_file_fwrite(&cnt, sizeof(cnt), 1, stream) != 1) {
        return false;
    }

    index = 0;
    while (index < cnt) {
        start = index;
        while (index < cnt && (magictech_locks[index].field_13C & 0x1) != 0) {
            index++;
        }

        extent = index - start;
        if (extent > 0) {
            if (tig_file_fwrite(extent, sizeof(extent), 1, stream) != 1) {
                return false;
            }

            while (start < index) {
                if (!sub_44F3C0(&(magictech_locks[start]), stream)) {
                    return false;
                }
            }
        }

        while (index < cnt && (magictech_locks[index].field_13C & 0x1) == 0) {
            index++;
        }

        extent = index - start;
        if (extent > 0) {
            extent = -extent;
            if (tig_file_fwrite(&extent, sizeof(extent), 1, stream) != 1) {
                return false;
            }
        }
    }

    return true;
}

// 0x44F3C0
bool sub_44F3C0(MagicTechLock* lock, TigFile* stream)
{
    if (stream == NULL) return false;
    if (tig_file_fwrite(&(lock->field_0), sizeof(lock->field_0), 1, stream) != 1) return false;
    if (tig_file_fwrite(&(lock->spell), sizeof(lock->spell), 1, stream) != 1) return false;
    if (tig_file_fwrite(&(lock->action), sizeof(lock->action), 1, stream) != 1) return false;
    if (!sub_4BAEE0(&(lock->source_obj), stream)) return false;
    if (!sub_4BAEE0(&(lock->parent_obj), stream)) return false;
    if (!sub_4BAEE0(&(lock->target_obj), stream)) return false;
    if (!sub_4BAEE0(&(lock->field_E8), stream)) return false;
    if (mt_obj_node_save(&(lock->objlist), stream)) return false;
    if (mt_obj_node_save(&(lock->summoned_obj), stream)) return false;
    if (tig_file_fwrite(&(lock->field_138), sizeof(lock->field_138), 1, stream) != 1) return false;
    if (tig_file_fwrite(&(lock->field_13C), sizeof(lock->field_13C), 1, stream) != 1) return false;
    if (tig_file_fwrite(&(lock->field_140), sizeof(lock->field_140), 1, stream) != 1) return false;
    if (tig_file_fwrite(&(lock->field_144), sizeof(lock->field_144), 1, stream) != 1) return false;
    if (tig_file_fwrite(&(lock->field_148), sizeof(lock->field_148), 1, stream) != 1) return false;
    if (tig_file_fwrite(&(lock->field_150), sizeof(lock->field_150), 1, stream) != 1) return false;

    return true;
}

// 0x44F540
bool magictech_post_load(GameLoadInfo* load_info)
{
    int cnt;
    int index;
    int extent;
    int j;

    if (load_info->stream == NULL) {
        return false;
    }

    if (tig_file_fread(&dword_6876DC, sizeof(dword_6876DC), 1, load_info->stream) != 1) {
        return false;
    }

    if (tig_file_fread(&cnt, sizeof(cnt), 1, load_info->stream) != 1) {
        return false;
    }

    index = 0;
    while (index < cnt) {
        if (tig_file_fread(&extent, sizeof(extent), 1, load_info->stream) != 1) {
            return false;
        }

        if (extent > 0) {
            for (j = 0; j < extent; j++) {
                if (!sub_44F620(&(magictech_locks[index]), load_info->stream)) {
                    return false;
                }

                index++;
            }
        } else if (extent < 0) {
            index += -extent;
        }
    }

    return true;
}

// 0x44F620
bool sub_44F620(MagicTechLock* lock, TigFile* stream)
{
    if (stream == NULL) return false;
    if (tig_file_fread(&(lock->field_0), sizeof(lock->field_0), 1, stream) != 1) return false;
    if (tig_file_fread(&(lock->spell), sizeof(lock->spell), 1, stream) != 1) return false;
    if (tig_file_fread(&(lock->action), sizeof(lock->action), 1, stream) != 1) return false;
    if (!sub_4BAF50(&(lock->source_obj), stream)) return false;
    if (!sub_4BAF50(&(lock->parent_obj), stream)) return false;
    if (!sub_4BAF50(&(lock->target_obj), stream)) return false;
    if (!sub_4BAF50(&(lock->field_E8), stream)) return false;
    if (mt_obj_node_load(&(lock->objlist), stream)) return false;
    if (mt_obj_node_load(&(lock->summoned_obj), stream)) return false;
    if (tig_file_fread(&(lock->field_138), sizeof(lock->field_138), 1, stream) != 1) return false;
    if (tig_file_fread(&(lock->field_13C), sizeof(lock->field_13C), 1, stream) != 1) return false;
    if (tig_file_fread(&(lock->field_140), sizeof(lock->field_140), 1, stream) != 1) return false;
    if (tig_file_fread(&(lock->field_144), sizeof(lock->field_144), 1, stream) != 1) return false;
    if (tig_file_fread(&(lock->field_148), sizeof(lock->field_148), 1, stream) != 1) return false;
    if (tig_file_fread(&(lock->field_150), sizeof(lock->field_150), 1, stream) != 1) return false;

    return true;
}

// 0x44F7A0
void magictech_break_nodes_to_map(const char* map)
{
    // TODO: Incomplete.
}

// 0x44FA70
void magictech_save_nodes_to_map(const char* map)
{
    // TODO: Incomplete.
}

// 0x44FC30
void magictech_load_nodes_from_map(const char* map)
{
    // TODO: Incomplete.
}

// 0x44FDC0
void sub_44FDC0(MesFileEntry *mes_file_entry)
{
    mes_get_msg(magictech_spell_mes_file, mes_file_entry);
}

// 0x44FDE0
const char* magictech_spell_name(int num)
{
    MesFileEntry mes_file_entry;

    if (num >= 0 && num < MT_SPELL_COUNT) {
        mes_file_entry.num = num;
        mes_get_msg(magictech_spell_mes_file, &mes_file_entry);
        return mes_file_entry.str;
    } else {
        return NULL;
    }
}

// 0x44FE20
void sub_44FE20()
{
    dword_5E75FC = true;
}

// 0x44FE30
bool sub_44FE30(int a1, const char* path, int a3)
{
    int magictech = 0;
    int v1;

    (void)a1;

    if (!mes_load(path, &dword_5E6D20)) {
        return false;
    }

    for (magictech = 0; magictech < MT_SPELL_COUNT; magictech++) {
        for (v1 = 0; v1 < 5; v1++) {
            magictech_spells[magictech].field_E8[v1].field_0 = 0;
            magictech_spells[magictech].field_E8[v1].field_4 = NULL;
        }
    }

    while (magictech < MT_80) {
        sub_450090(dword_5E6D20, &(magictech_spells[magictech]), a3, magictech);
        magictech++;
    }

    while (magictech < MT_140) {
        sub_450090(dword_5E6D20, &(magictech_spells[magictech]), a3, magictech);
        magictech_spells[magictech].iq = 0;
        magictech++;
    }

    while (magictech < MT_SPELL_COUNT) {
        sub_450090(dword_5E6D20, &(magictech_spells[magictech]), a3 + 2000, magictech);
        magictech_spells[magictech].flags |= 0x04;
        magictech_spells[magictech].iq = 0;
        magictech++;
    }

    // NOTE: Meaningless?
    while (magictech < MT_SPELL_COUNT) {
        sub_450090(dword_5E6D20, &(magictech_spells[magictech]), a3, magictech);
        magictech_spells[magictech].iq = 0;
        magictech++;
    }

    return true;
}

// 0x44FFA0
bool sub_44FFA0(int a1, const char* a2, int a3)
{
    int magictech = 0;

    (void)a1;

    while (magictech < MT_80) {
        sub_4501D0(dword_5E6D20, &(magictech_spells[magictech]), a3, magictech);
        magictech++;
    }

    while (magictech < MT_140) {
        sub_4501D0(dword_5E6D20, &(magictech_spells[magictech]), a3, magictech);
        magictech_spells[magictech].iq = 0;
        magictech++;
    }

    while (magictech < MT_SPELL_COUNT) {
        sub_4501D0(dword_5E6D20, &(magictech_spells[magictech]), a3 + 2000, magictech);
        magictech_spells[magictech].flags |= 0x04;
        magictech_spells[magictech].iq = 0;
        magictech++;
    }

    mes_unload(dword_5E6D20);

    return true;
}

// 0x450090
void sub_450090(mes_file_handle_t msg_file, MagicTechInfo* info, int num, int magictech)
{
    MesFileEntry mes_file_entry;

    dword_5E7628 = magictech;

    sub_457580(info, magictech);

    mes_file_entry.num = num;
    mes_get_msg(magictech_spell_mes_file, &mes_file_entry);
    info->field_0 = mes_file_entry.str;

    mes_file_entry.num = num + 50 * magictech;
    mes_get_msg(magictech_spell_mes_file, &mes_file_entry);
    magictech_build_aoe_info(info, mes_file_entry.str);

    mes_file_entry.num++;
    mes_get_msg(magictech_spell_mes_file, &mes_file_entry);
    sub_4578F0(info, mes_file_entry.str);

    mes_file_entry.num++;
    if (mes_search(msg_file, &mes_file_entry)) {
        mes_get_msg(magictech_spell_mes_file, &mes_file_entry);
        sub_457B20(info, mes_file_entry.str);
    }

    mes_file_entry.num++;
    if (mes_search(msg_file, &mes_file_entry)) {
        mes_get_msg(msg_file, &mes_file_entry);
        sub_457D00(info, mes_file_entry.str);
    }

    memset(&(info->ai), 0xFF, sizeof(info->ai));
    info->ai.defensive2 = 0;

    mes_file_entry.num++;
    if (mes_search(msg_file, &mes_file_entry)) {
        mes_get_msg(msg_file, &mes_file_entry);
        magictech_build_ai_info(info, mes_file_entry.str);
    }
}

// 0x4501D0
void sub_4501D0(mes_file_handle_t msg_file, MagicTechInfo* info, int num, int magictech)
{
    MesFileEntry mes_file_entry;
    int index;

    mes_file_entry.num = num + 50 * magictech + 5;
    for (index = 0; index < 44; index++) {
        if (!mes_search(msg_file, &mes_file_entry)) {
            return;
        }

        mes_get_msg(msg_file, &mes_file_entry);
        magictech_build_effect_info(info, mes_file_entry.str);

        mes_file_entry.num++;
    }
}

// 0x450240
void sub_450240()
{
    int index;
    int v1;

    for (index = 0; index < MT_SPELL_COUNT; index++) {
        for (v1 = 0; v1 < 5; v1++) {
            if (magictech_spells[index].field_E8[v1].field_4 != NULL) {
                FREE(magictech_spells[index].field_E8[v1].field_4);
            }
        }
    }
}

// 0x450280
int magictech_get_range(int magictech)
{
    if (magictech_initialized) {
        return magictech_spells[magictech].range;
    } else {
        return 1;
    }
}

// 0x4502B0
int sub_4502B0(int magictech)
{
    if (magictech_initialized) {
        return magictech_spells[magictech].field_80;
    } else {
        return 0;
    }
}

// 0x4502E0
int sub_4502E0(int a1)
{
    return 5;
}

// 0x4502F0
int magictech_get_iq(int magictech)
{
    if (magictech_initialized) {
        return magictech_spells[magictech].iq;
    }

    if (magictech >= 0 && magictech < MT_80) {
        return dword_5B0DE0[magictech % 5];
    }

    return dword_5B0DE0[1];
}

// 0x450340
int magictech_get_cost(int magictech)
{
    if (magictech_initialized) {
        return magictech_spells[magictech].cost;
    } else {
        return 1;
    }
}

// 0x450370
bool sub_450370(int magictech)
{
    if (magictech_initialized) {
        return (magictech_spells[magictech].flags & 0x100) != 0;
    } else {
        return false;
    }
}

// 0x4503A0
bool sub_4503A0(int magictech)
{
    if (magictech_initialized) {
        return magictech_spells[magictech].maintain[1] > 0;
    } else {
        return false;
    }
}

// 0x4503E0
int* magictech_get_maintain1(int magictech)
{
    return magictech_spells[magictech].maintain;
}

// 0x450400
int magictech_get_duration1(int magictech)
{
    return magictech_spells[magictech].duration1;
}

// 0x4507B0
void sub_4507B0(object_id_t obj, int magictech)
{
    sub_4507D0(obj, magictech);
}

// 0x4507D0
bool sub_4507D0(object_id_t obj, int magictech)
{
    int cost;

    if (obj == OBJ_HANDLE_NULL) {
        return true;
    }

    cost = magictech_spells[magictech].cost;
    if (magictech >= 0 && magictech < MT_80 && sub_4B1CB0(obj) == magictech / 5) {
        cost /= 2;
        if ((cost & 1) != 0) {
            cost++;
        }
    }

    if (dword_5E75FC) {
        cost = 1;
    }

    if ((obj_field_int32_get(obj, OBJ_F_TYPE) == OBJ_TYPE_PC
            || obj_field_int32_get(obj, OBJ_F_TYPE) == OBJ_TYPE_NPC)
        && stat_level(obj, STAT_RACE) == RACE_DWARF) {
        cost *= 2;
    }

    return sub_450420(obj, cost, true, magictech);
}

// 0x4508A0
bool magictech_can_charge_spell_fatigue(object_id_t obj, int magictech)
{
    int cost;

    if (obj == OBJ_HANDLE_NULL) {
        tig_debug_printf("\tMagicTech: magictech_can_charge_spell_fatigue: Maintain cannot charge!\n");
        return false;
    }

    cost = magictech_spells[magictech].cost;
    if (dword_5E75FC) {
        cost = 1;
    }

    if ((obj_field_int32_get(obj, OBJ_F_TYPE) == OBJ_TYPE_PC
            || obj_field_int32_get(obj, OBJ_F_TYPE) == OBJ_TYPE_NPC)
        && stat_level(obj, STAT_RACE) == RACE_DWARF) {
        cost *= 2;
    }

    return sub_450420(obj, cost, false, magictech);
}

// 0x450940
bool sub_450940(int magictech)
{
    MagicTechLock* v1;
    int* maintain;
    int cost;

    if (!sub_4557C0(magictech, &v1)) {
        tig_debug_printf("\tMagicTech: Maintain cannot charge!\n");
        return false;
    }

    if ((v1->field_13C & 0x2) != 0
        || v1->source_obj.obj == OBJ_HANDLE_NULL) {
        return true;
    }

    if (v1->action == 0) {
        cost = magictech_spells[magictech].cost;
    } else {
        maintain = magictech_get_maintain1(magictech);
        cost = maintain[0];
    }

    if (dword_5E75FC) {
        cost = 1;
    }

    if ((obj_field_int32_get(v1->source_obj.obj, OBJ_F_TYPE) == OBJ_TYPE_PC
            || obj_field_int32_get(v1->source_obj.obj, OBJ_F_TYPE) == OBJ_TYPE_NPC)
        && stat_level(v1->source_obj.obj, STAT_RACE) == RACE_DWARF) {
        cost *= 2;
    }

    return sub_450420(v1->source_obj.obj, cost, 0, v1->spell);
}

// 0x450A50
int64_t sub_450A50(object_id_t obj)
{
    int type;

    if (obj == OBJ_HANDLE_NULL) {
        return OBJ_HANDLE_NULL;
    }

    type = obj_field_int32_get(obj, OBJ_F_TYPE);
    if (type != OBJ_TYPE_PC
        && type != OBJ_TYPE_NPC
        && type >= OBJ_TYPE_WEAPON
        && type <= OBJ_TYPE_ITEM_GENERIC
        && (obj_field_int32_get(obj, OBJ_F_FLAGS) & OF_INVENTORY) != 0) {
        return obj_field_handle_get(obj, OBJ_F_ITEM_PARENT);
    }

    return obj;
}

// 0x450AC0
int sub_450AC0(int64_t obj)
{
    int index;

    if (obj == OBJ_HANDLE_NULL) {
        return 0;
    }

    if (!obj_type_is_critter(obj_field_int32_get(obj, OBJ_F_TYPE))) {
        return 0;
    }

    for (index = 0; index < 512; index++) {
        // NOTE: Unclear, likely no-op.
    }

    return 0;
}

// 0x450B40
int sub_450B40(int64_t obj)
{
    if (obj == OBJ_HANDLE_NULL) {
        return 0;
    }

    if (!obj_type_is_critter(obj_field_int32_get(obj, OBJ_F_TYPE))) {
        return 0;
    }

    return sub_450B90(obj) - sub_450AC0(obj);
}

// 0x450B90
int sub_450B90(object_id_t obj)
{
    return stat_level(obj, STAT_INTELLIGENCE) / 4;
}

// 0x450C10
void sub_450C10(object_id_t obj, unsigned int flags)
{
    obj_field_int32_set(obj, OBJ_F_SPELL_FLAGS, obj_field_int32_get(obj, OBJ_F_SPELL_FLAGS) | flags);
}

// 0x451070
void sub_451070(MagicTechLock* a1)
{
    if (dword_5B0BA4 != -1 && dword_5B0BA4 != a1->field_0) {
        tig_debug_printf("\n\nMagicTech: ERROR: Process function is NOT Re-Entrant, Spell: %d (%s)!\n",
            a1->spell,
            magictech_get_name(a1->spell));
        return;
    }

    dword_5E75F0 = a1;
    dword_5E75AC = -1;
    dword_5E75CC = 0;
    dword_5E75D4 = 0;
    dword_5E75D8 = 0;
    dword_5E75DC = 0;
    dword_5E75E0 = 0;
    dword_5E75E4 = 0;
    dword_5E75E8 = a1->action;
    sub_4510F0();
}

// 0x4574D0
void sub_4574D0(int64_t obj)
{
    int index;

    if (obj == OBJ_HANDLE_NULL) {
        return;
    }

    for (index = 0; index < 512; index++) {
        if (magictech_locks[index].parent_obj.obj == obj
            || magictech_locks[index].source_obj.obj == obj) {
            sub_457530(magictech_locks[index].field_0);
        }
    }
}

// 0x455710
void sub_455710()
{
    int index;
    MagicTechLock* lock;

    for (index = 0; index < 512; index++) {
        lock = &(magictech_locks[index]);
        lock->source_obj.obj = OBJ_HANDLE_NULL;
        lock->field_0 = -1;
        lock->field_13C = 0;
    }
}

// 0x457530
void sub_457530(int magictech)
{
    MagicTechLock* v1;

    if (sub_4557C0(magictech, &v1)) {
        v1->source_obj.obj = OBJ_HANDLE_NULL;
        sub_443EB0(OBJ_HANDLE_NULL, &(v1->source_obj.field_8));
        v1->source_obj.type = -1;
    }
}

// 0x4578F0
void sub_4578F0(MagicTechInfo* info, char* str)
{
    char* curr = str;
    int value1;
    int value2;

    tig_str_parse_set_separator(',');

    if (tig_str_parse_named_value(&curr, "IQ:", &value1)) {
        info->iq = value1;
    }

    if (tig_str_parse_named_value(&curr, "Cost:", &value1)) {
        info->cost = value1;
    }

    if (tig_str_parse_named_complex_value(&curr, "Maintain:", '@', &value1, &value2)) {
        info->maintain[0] = value1;
        info->maintain[1] = value2;
    }

    if (tig_str_parse_named_complex_value(&curr, "Duration:", '@', &value1, &value2)) {
        info->duration1 = value1;
        info->duration2 = value2;

        if (info->duration2 != -1) {
            if (tig_str_parse_named_complex_value(&curr, "DurationStatInfo:", '@', &value1, &value2)) {
                info->duration_stat = value1;
                info->duration_stat_value = value2;
            }
        }
    }

    if (tig_str_parse_named_value(&curr, "DurationTriggerCount:", &value1)) {
        info->duration_trigger_count = value1;
    }

    if (tig_str_parse_named_complex_str_value(&curr, "Resist:", '@', off_5B5384, STAT_COUNT, &value1, &value2)) {
        info->resist_stat = value1;
        info->resist_value = value2;
    }

    if (tig_str_parse_named_value(&curr, "Range:", &value1)) {
        info->range = value1;
    }

    if (tig_str_parse_named_value(&curr, "ChargesFromCells:", &value1)) {
        if (value1 != 0) {
            info->flags |= 0x20;
        }
    }

    if (tig_str_parse_named_value(&curr, "ChargeBeginCost:", &value1)) {
        if (value1 == 0) {
            info->flags |= 0x0100;
        }
    }

    if (tig_str_match_named_str_to_list(&curr, "Info:", off_5B0C20, 4, &value1)) {
        info->flags |= dword_5B0C30[value1];
    }

    if (!tig_str_parse_named_value(&curr, "Disabled:", &value1) || value1 == 0) {
        info->flags |= 0x08;
    }
}

// 0x457B20
void sub_457B20(MagicTechInfo* info, char* str)
{
    char* curr = str;
    int value;

    tig_str_parse_set_separator(',');

    if (tig_str_parse_named_value(&curr, "CastingAnim:", &value)) {
        info->casting_anim = value;
    }

    if (tig_str_parse_named_value(&curr, "Missile:", &value)) {
        info->missile = value;
    }

    if (tig_str_parse_named_value(&curr, "[Begin]Caster:", &value)) {
        info->begin_caster = value;
    }

    if (tig_str_parse_named_value(&curr, "[Begin]Target:", &value)) {
        info->begin_target = value;
    }

    if (tig_str_parse_named_value(&curr, "[Maintain]Caster:", &value)) {
        info->maintain_caster = value;
    }

    if (tig_str_parse_named_value(&curr, "[Maintain]Target:", &value)) {
        info->maintain_target = value;
    }

    if (tig_str_parse_named_value(&curr, "[End]Caster:", &value)) {
        info->end_caster = value;
    }

    if (tig_str_parse_named_value(&curr, "[End]Target:", &value)) {
        info->end_target = value;
    }

    if (tig_str_parse_named_value(&curr, "[Callback]Caster:", &value)) {
        info->callback_caster = value;
    }

    if (tig_str_parse_named_value(&curr, "[Callback]Target:", &value)) {
        info->callback_target = value;
    }

    if (tig_str_parse_named_value(&curr, "[EndCallback]Caster:", &value)) {
        info->end_callback_caster = value;
    }

    if (tig_str_parse_named_value(&curr, "[EndCallback]Target:", &value)) {
        info->end_callback_target = value;
    }

    if (tig_str_parse_named_value(&curr, "Is_Tech:", &value)) {
        if (value != 0) {
            info->flags |= 0x04;
        }
    }
}

// 0x457D00
void sub_457D00(MagicTechInfo* info, char* str)
{
    char* curr = str;
    unsigned int flags;
    int value;

    tig_str_parse_set_separator(',');

    if (tig_str_parse_named_value(&curr, "No_Stack:", &value)) {
        info->no_stack = value;
    }

    if (tig_str_parse_named_flag_list_direct(&curr, "Cancels_SF:", off_5BA064[MTFC_SPELL_FLAGS], dword_5BA0B8[MTFC_SPELL_FLAGS], &flags)) {
        info->cancels_sf = flags;
    }

    if (tig_str_parse_named_flag_list_direct(&curr, "Cancels_EnvSF:", off_5BA064[MTFC_SPELL_FLAGS], dword_5BA0B8[MTFC_SPELL_FLAGS], &flags)) {
        info->cancels_envsf = flags;
    }

    if (tig_str_parse_named_flag_list_direct(&curr, "Disallowed_SF:", off_5BA064[MTFC_SPELL_FLAGS], dword_5BA0B8[MTFC_SPELL_FLAGS], &flags)) {
        info->disallowed_sf = flags;
    }

    if (tig_str_parse_named_flag_list_direct(&curr, "Disallowed_TSF:", off_5BA064[MTFC_SPELL_FLAGS], dword_5BA0B8[MTFC_SPELL_FLAGS], &flags)) {
        info->disallowed_tsf = flags;
    }

    if (tig_str_parse_named_flag_list_direct(&curr, "Disallowed_TCF:", off_5BA064[MTFC_CRITTER_FLAGS], dword_5BA0B8[MTFC_CRITTER_FLAGS], &flags)) {
        info->disallowed_tcf = flags;
    }

    if (tig_str_parse_named_flag_list(&curr, "ItemTriggers:", off_5B72AC, dword_5B7314, 26, &flags)) {
        info->item_triggers = flags;
    }
}

// 0x457E70
void magictech_build_ai_info(MagicTechInfo* info, char* str)
{
    char* curr = str;
    int value1;
    int value2;

    tig_str_parse_set_separator(',');

    if (tig_str_parse_named_value(&curr, "AI_Flee:", &value1)) {
        info->ai.flee = value1;
    }

    if (tig_str_parse_named_value(&curr, "AI_Summon:", &value1)) {
        info->ai.summon = value1;
    }

    if (tig_str_parse_named_complex_value(&curr, "AI_Defensive:", '@', &value1, &value2)) {
        info->ai.defensive1 = value1;
        info->ai.defensive2 = value2;
    }

    if (tig_str_parse_named_value(&curr, "AI_Offensive:", &value1)) {
        info->ai.offensive = value1;
    }

    if (tig_str_parse_named_value(&curr, "AI_HealingLight:", &value1)) {
        info->ai.healing_light = value1;
    }

    if (tig_str_parse_named_value(&curr, "AI_HealingMedium:", &value1)) {
        info->ai.healing_medium = value1;
    }

    if (tig_str_parse_named_value(&curr, "AI_HealingHeavy:", &value1)) {
        info->ai.healing_heavy = value1;
    }

    if (tig_str_parse_named_value(&curr, "AI_CurePoison:", &value1)) {
        info->ai.cure_poison = value1;
    }

    if (tig_str_parse_named_value(&curr, "AI_FatigueRecover:", &value1)) {
        info->ai.fatigue_recover = value1;
    }

    if (tig_str_parse_named_value(&curr, "AI_Resurrect:", &value1)) {
        info->ai.resurrect = value1;
    }

    if (tig_str_parse_named_value(&curr, "No_Resist:", &value1)) {
        info->flags |= 0x200;
    }

    if (tig_str_parse_named_value(&curr, "No_Reflect:", &value1)) {
        info->flags |= 0x400;
    }
}

// 0x458060
void magictech_build_effect_info(MagicTechInfo* info, char* str)
{
    int type;
    MagicTechE8* v1;
    MagicTechEffectInfo* effect_info;
    uint64_t aoe;
    unsigned int flags;
    int value;

    tig_str_parse_set_separator(',');

    tig_str_match_str_to_list(&str, off_5B0C0C, 5, &type);

    v1 = &(info->field_E8[type]);
    if (v1->field_4 != NULL) {
        v1->field_4 = REALLOC(v1->field_4, sizeof(*v1->field_4) * (v1->field_0 + 1));
        if (v1->field_4 == NULL) {
            tig_debug_printf("magictech_build_effect_info: Error: failed realloc component list!\n");
            exit(EXIT_FAILURE);
        }
    } else {
        v1->field_4 = MALLOC(sizeof(*v1->field_4));
    }

    effect_info = &(v1->field_4[v1->field_0++]);
    effect_info->aoe = 0;
    effect_info->aoe_spell_flags = 0;
    effect_info->aoe_no_spell_flags = 0;
    effect_info->radius = 0;
    effect_info->count = -1;

    if (tig_str_parse_named_flag_list_64(&str, "AoE:", off_5BBD70, qword_596140, 65, &aoe)) {
        effect_info->aoe = aoe;
    }

    if (tig_str_parse_named_flag_list_direct(&str, "AoE_SF:", off_5BA064[MTFC_SPELL_FLAGS], dword_5BA0B8[MTFC_SPELL_FLAGS], &flags)) {
        effect_info->aoe_spell_flags |= flags;
    }

    if (tig_str_parse_named_flag_list_direct(&str, "AoE_NO_SF:", off_5BA064[MTFC_SPELL_FLAGS], dword_5BA0B8[MTFC_SPELL_FLAGS], &flags)) {
        effect_info->aoe_no_spell_flags |= flags;
    }

    if (tig_str_parse_named_value(&str, "Radius:", &value)) {
        effect_info->radius = value;
    }

    if (tig_str_parse_named_value(&str, "Count:", &value)) {
        effect_info->count = value;
    }

    effect_info->apply_aoe = 0;
    effect_info->field_28 = 0;
    effect_info->field_2C = 0;
    effect_info->field_30 = 0;
    effect_info->field_34 = -1;

    tig_str_parse_named_flag_list_64(&str, "Apply_AoE:", off_5BBD70, qword_596140, 65, &(effect_info->apply_aoe));

    if (tig_str_parse_named_flag_list(&str, "ItemTriggers:", off_5B72AC, dword_5B7314, 26, &flags)) {
        effect_info->item_triggers = flags;
    } else {
        effect_info->item_triggers = 0;
    }

    if (tig_str_match_named_str_to_list(&str, "Type:", magictech_component_names, 25, &(effect_info->type))) {
        switch (effect_info->type) {
        case MTC_AGOAL:
            tig_str_match_str_to_list(&str, off_5B0C40, 6, &value);
            effect_info->data.agoal.goal = dword_5B0C58[value];
            if (tig_str_parse_named_value(&str, "SubGoal:", &value)) {
                effect_info->data.agoal.subgoal = value;
            } else {
                effect_info->data.agoal.subgoal = 0;
            }
            break;
        case MTC_AGOALTERMINATE:
            tig_str_match_str_to_list(&str, off_5B0C40, 6, &value);
            effect_info->data.agoal_terminate.goal = dword_5B0C58[value];
            break;
        case MTC_AIREDIRECT:
            tig_str_match_str_to_list(&str, off_5BA348, 32, &value);
            effect_info->data.ai_redirect.critter_flags = 1 << value;
            if (tig_str_parse_named_value(&str, "MinIQ:", &value)) {
                effect_info->data.ai_redirect.min_iq = value;
            } else {
                effect_info->data.ai_redirect.min_iq = -1;
            }
            break;
        case MTC_CAST:
            if (tig_parse_named_value(&str, "Spell:", &value)) {
                effect_info->data.cast.spell = value;
            } else {
                effect_info->data.cast.spell = 10000;
                tig_debug_printf("MagicTech: ERROR: Cast Component has no spell!\n");
            }
            break;
        case MTC_CHARGENBRANCH:
            effect_info->data.charge_branch.branch = -1;
            effect_info->data.charge_branch.cost = 0;
            tig_str_parse_named_value(&str, "Cost:", &(effect_info->data.charge_branch.cost));
            if (tig_str_parse_named_value(&str, "Branch:", &value)) {
                effect_info->data.charge_branch.branch = value;
            }
            break;
        case MTC_DAMAGE:
            tig_str_match_named_str_to_list(&str, "DmgType:", off_5B0C70, 6, &(effect_info->data.damage.damage_type));
            tig_str_parse_named_range(&str, "Dmg:", &(effect_info->data.damage.damage_min), &(effect_info->data.damage.damage_max));
            effect_info->data.damage.damage_flags = 0;
            tig_str_parse_named_flag_list(&str, "Dmg_Flags:", off_5B0D14, dword_5B0D3C, 10, &(effect_info->data.damage.damage_flags));
            effect_info->data.damage.damage_flags |= 0x80;
            break;
        case MTC_EFFECT:
            tig_str_parse_value(&str, &(effect_info->data.effect.num));
            tig_str_match_str_to_list(&str, off_5B0C88, 2, &(effect_info->data.effect.add_remove));
            if (tig_str_parse_named_value(&str, "Count:", &value)) {
                effect_info->data.effect.count = value;
            } else {
                effect_info->data.effect.count = 1;
            }

            if (tig_str_match_named_str_to_list(&str, "Cause:", off_5B9CB4, 10, &value)) {
                effect_info->data.effect.cause = value;
            } else {
                effect_info->data.effect.cause = 6;
            }

            if (tig_str_parse_named_value(&str, "Scaled:", &value)) {
                effect_info->data.effect.scaled = value;
            } else {
                effect_info->data.effect.scaled = 0;
            }
            break;
        case MTC_ENVFLAG:
            tig_str_match_str_to_list(&str, off_5BA064[MTFC_SPELL_FLAGS], dword_5BA0B8[MTFC_SPELL_FLAGS], &value);
            effect_info->data.env_flags.flags = 1 << value;
            tig_str_match_str_to_list(&str, off_5B0C90, 2, &(effect_info->data.env_flags.state));
            break;
        case MTC_EYECANDY:
            tig_str_parse_value(&str, &(effect_info->data.eye_candy.num));
            tig_str_match_str_to_list(&str, off_5B0C88, 2, &(effect_info->data.eye_candy.add_remove));
            tig_str_parse_named_flag_list(&str, "Play:", off_5B7658, dword_5B7680, 10, &(effect_info->data.eye_candy.flags));
            break;
        case MTC_HEAL:
            tig_str_match_str_to_list(&str, "DmgType:", off_5B0C70, &(effect_info->data.heal.damage_type));
            tig_str_parse_named_range(&str, "Dmg:", &(effect_info->data.heal.damage_min), &(effect_info->data.heal.damage_max));
            effect_info->data.heal.damage_flags = 0;
            tig_str_parse_named_flag_list(&str, "Dmg_Flags:", off_5B0D14, dword_5B0D3C, 10, &(effect_info->data.heal.damage_flags));
            break;
        case MTC_INTERRUPT:
            if (tig_str_parse_named_value(&str, "MagicTech:", &value)) {
                effect_info->data.interrupt.magictech = value;
            } else {
                effect_info->data.interrupt.magictech = 10000;
                tig_debug_printf("MagicTech: ERROR: Interrupt Component has no spell!\n");
            }
            break;
        case MTC_MOVEMENT:
            if (tig_str_match_named_str_to_list(&str, "Move_Location:", off_5B0D04, 4, &(effect_info->data.movement.move_location))
                && effect_info->data.movement.move_location == 1) {
                tig_str_parse_named_value(&str, "Tile_Radius:", &(effect_info->data.movement.tile_radius));
            }
            break;
        case MTC_OBJFLAG:
            tig_str_match_str_to_list(&str, off_5B9FBC, 21, &value);
            effect_info->data.obj_flag.flags_fld = dword_5BA010[value];
            tig_str_match_str_to_list(&str, off_5BA064[value], dword_5BA0B8[value], &value);
            effect_info->data.obj_flag.value = 1 << value;
            tig_str_match_str_to_list(&str, off_5B0C90, 2, &(effect_info->data.obj_flag.state));
            if (effect_info->data.obj_flag.flags_fld == OBJ_F_SPELL_FLAGS
                && effect_info->data.obj_flag.state) {
                info->field_114 |= effect_info->data.obj_flag.value;
            }
            break;
        case MTC_RECHARGE:
            tig_str_parse_value(&str, &(effect_info->data.recharge.num));
            if (!tig_str_parse_named_value(&str, "Max:", &(effect_info->data.recharge.max))) {
                effect_info->data.recharge.max = 9999;
            }
            break;
        case MTC_SUMMON:
            if (!tig_str_parse_named_value(&str, "Proto:", &value)) {
                tig_debug_printf("magictech_build_effect_info: MTComponentSummon: Error: failed to find proto #!\n");
            }

            if (value > 0) {
                effect_info->data.summon.oid = sub_407EF0(value);
            } else {
                effect_info->data.summon.oid.type = 0;
            }

            if (tig_str_parse_named_value(&str, "Clear_Faction:", &value)) {
                effect_info->data.summon.clear_faction = value;
            } else {
                effect_info->data.summon.clear_faction = 0;
            }

            if (tig_str_parse_named_value(&str, "Palette:", &value)) {
                effect_info->data.summon.palette = value;
            } else {
                effect_info->data.summon.palette = 0;
            }

            if (tig_str_parse_named_value(&str, "List:", &value)) {
                effect_info->data.summon.list = value;
            } else {
                effect_info->data.summon.list = -1;
            }
            break;
        case MTC_TESTNBRANCH:
            effect_info->data.test_in_branch.field_48 = 0;
            effect_info->data.test_in_branch.field_4C = -1;
            tig_str_match_str_to_list(&str, off_5B0CAC, 4, &value);
            effect_info->data.test_in_branch.field_40 = dword_5B0CBC[value];
            tig_str_match_str_to_list(&str, off_5B0C98, 5, &value);
            effect_info->data.test_in_branch.field_44 = value;
            if (tig_str_parse_named_value(&str, "TestVal:", &value)) {
                effect_info->data.test_in_branch.field_48 = value;
            }
            if (tig_str_parse_named_value(&str, "Branch:", &value)) {
                effect_info->data.test_in_branch.field_4C = value;
            }
            break;
        case MTC_TRAIT:
            tig_str_match_str_to_list(&str, off_5B0CAC, 4, &value);
            effect_info->data.trait.field_40 = dword_5B0CBC[value];
            effect_info->data.trait.field_44 = 0;
            tig_str_parse_value(&str, &value);
            effect_info->data.trait.field_50 = value;
            effect_info->data.trait.field_4C = 1;
            effect_info->data.trait.field_48 = 1;
            tig_str_parse_named_value(&str, "Palette:", &(effect_info->data.trait.field_54));
            break;
        case MTC_TRAITIDX:
            tig_str_match_str_to_list(&str, off_5B0CCC, 2, &value);
            effect_info->data.trait_idx.field_44 = dword_5B0CD4[value];
            tig_str_match_str_to_list(&str, off_5B0CDC[value], dword_5B0CE4[value], &value);
            effect_info->data.trait_idx.field_40 = value;
            effect_info->data.trait_idx.field_48 = 0;
            tig_str_parse_value(&str, &value);
            effect_info->data.trait_idx.field_54 = value;
            effect_info->data.trait_idx.field_50 = 1;
            effect_info->data.trait_idx.field_4C = 1;
            break;
        case MTC_TRAIT64:
            tig_str_match_str_to_list(&str, off_5B0CAC, 4, &value);
            effect_info->data.trait64.field_40 = dword_5B0CBC[value];
            tig_str_match_str_to_list(&str, off_5B0CF8, 3, &value);
            effect_info->data.trait64.field_44 = value;
            break;
        }
    }
}

// 0x458A80
int sub_458A80(unsigned int flags)
{
    int64_t pc_obj;

    pc_obj = player_get_pc_obj();
    if (pc_obj == OBJ_HANDLE_NULL) {
        return false;
    }

    if ((obj_field_int32_get(pc_obj, OBJ_F_SPELL_FLAGS) & flags) == flags) {
        return true;
    }

    if (flags == OSF_DETECTING_INVISIBLE
        && stat_is_maximized(pc_obj, STAT_PERCEPTION)) {
        return true;
    }

    return false;
}

// 0x458AE0
tig_art_id_t sub_458AE0(int magictech)
{
    MagicTechLock* v1;
    tig_art_id_t art_id;

    if (!sub_4557C0(magictech, &v1)) {
        return TIG_ART_ID_INVALID;
    }

    if (tig_art_interface_id_create(sub_4B1570(v1->spell), 0, 0, 0, &art_id) != TIG_OK) {
        return TIG_ART_ID_INVALID;
    }

    return art_id;
}

// 0x458B60
int magictech_get_name(int magictech)
{
    return spell_get_name(magictech);
}


// 0x459380
bool sub_459380(int64_t obj, int magictech)
{
    int v1;
    MagicTechLock* v2;

    if (sub_458CF0(obj, &v1)) {
        do {
            if (sub_4557C0(v1, &v2) && v2->spell == magictech) {
                return true;
            }
        } while (sub_458D90(obj, &v2));
    }

    return false;
}

// 0x4593F0
bool sub_4593F0(int64_t obj, int a2)
{
    int magictech;

    if (!sub_459290(obj, a2, &magictech)) {
        return false;
    }

    sub_457110(magictech);

    return true;
}

// 0x459430
bool magictech_timeevent_process(TimeEvent* timeevent)
{
    MagicTechLock* v1;

    switch (timeevent->params[2].integer_value) {
    case 1:
        sub_459490(timeevent->params[0].integer_value);
        break;
    case 2:
        sub_457110(timeevent->params[0].integer_value);
        break;
    case 3:
        if (sub_4557C0(timeevent->params[0].integer_value, &v1)) {
            sub_451070(v1);
        }
        break;
    }

    return true;
}

// 0x459490
void sub_459490(int magictech)
{
    MagicTechLock* v1;

    if (sub_4557C0(magictech, &v1)) {
        if (v1->action == 0) {
            v1->action = 1;
        }
        sub_451070(v1);
    }
}

// 0x4594D0
bool sub_4594D0(TimeEvent* timeevent)
{
    return timeevent->params[0].integer_value == dword_5B0F20
        && timeevent->params[2].integer_value == dword_5E7634;
}

// 0x459500
bool sub_459500(int index)
{
    MagicTechLock* v1;
    TimeEvent timeevent;

    if (index == -1
        || !sub_4557C0(index, &v1)
        || (v1->field_13C & 0x1) == 0) {
        return false;
    }

    dword_5B0F20 = index;
    dword_5E7634 = 1;
    timeevent_clear_one_ex(TIMEEVENT_TYPE_MAGICTECH, sub_4594D0);

    timeevent.type = TIMEEVENT_TYPE_MAGICTECH;
    timeevent.params[0].integer_value = v1->field_0;
    timeevent.params[2].integer_value = 1;
    return sub_45B860(&timeevent, &(v1->field_148));
}

// 0x459590
bool sub_459590(object_id_t obj, int a2, bool a3)
{
    DateTime datetime;
    TimeEvent timeevent;

    dword_5E7608 = a2;
    qword_5E7610 = obj;
    dword_5E760C = sub_45A7F0();

    if (a3 && sub_45C140(TIMEEVENT_TYPE_RECHARGE_MAGIC_ITEM, sub_459640)) {
        return true;
    }

    timeevent.type = TIMEEVENT_TYPE_RECHARGE_MAGIC_ITEM;
    timeevent.params[0].object_value = obj;
    timeevent.params[1].integer_value = dword_5E760C;
    timeevent.params[2].integer_value = dword_5E7608;
    sub_45A950(&datetime, 60000);
    datetime.milliseconds *= 8;
    return sub_45B800(&timeevent, &datetime);
}

// 0x459640
bool sub_459640(TimeEvent* timeevent)
{
    if (timeevent->params[0].object_value != qword_5E7610) {
        return false;
    }

    timeevent->params[2].integer_value += dword_5E7608;

    return true;
}

// 0x459A20
void sub_459A20(int64_t obj)
{
}

// 0x459EA0
void sub_459EA0(int64_t obj)
{
    unsigned int flags;
    int64_t leader_obj;

    if (obj != OBJ_HANDLE_NULL) {
        flags = obj_field_int32_get(obj, OBJ_F_SPELL_FLAGS);
        if ((flags & OSF_TEMPUS_FUGIT) != 0) {
            flags &= ~OSF_TEMPUS_FUGIT;
        }
        sub_4EFDD0(obj, OBJ_F_SPELL_FLAGS, flags);

        if ((obj_field_int32_get(obj, OBJ_F_NPC_FLAGS) & ONF_FAMILIAR) != 0) {
            leader_obj = critter_leader_get(obj);
            if (leader_obj != OBJ_HANDLE_NULL) {
                flags = obj_field_int32_get(leader_obj, OBJ_F_SPELL_FLAGS);
                flags &= ~OSF_FAMILIAR;
                sub_4EFDD0(leader_obj, OBJ_F_SPELL_FLAGS, flags);
            }
        }
    }
}

// 0x459F20
bool sub_459F20(int magictech, int* a2)
{
    *a2 = magictech_spells[magictech].field_70;
    return true;
}

// 0x459F50
void sub_459F50()
{
    mt_ai_reset();
}

// 0x459F60
bool sub_459F60(int magictech)
{
    return (magictech_spells[magictech].flags & 8) != 0;
}

// 0x459F90
bool sub_459F90(int magictech)
{
    return (magictech_spells[magictech].flags & 4) == 0;
}

// 0x459FC0
bool sub_459FC0(int magictech)
{
    return (magictech_spells[magictech].flags & 4) != 0;
}

// 0x459FF0
bool sub_459FF0(int magictech)
{
    MagicTechLock* v1;

    if (!sub_4557C0(magictech, &v1)) {
        return false;
    }

    return v1->spell >= 0 && v1->spell < MT_SPELL_COUNT;
}

// 0x45A030
bool sub_45A030(int magictech)
{
    return magictech_spells[magictech].item_triggers != 0;
}

// 0x45A060
int magictech_get_aptitude_adj(int64_t sector_id)
{
    int aptitude_adj;
    Sector* sector;

    aptitude_adj = 0;
    if (sector_lock(sector_id, &sector)) {
        aptitude_adj = sector->aptitude_adj;
        sector_unlock(sector_id);
    }

    return aptitude_adj;
}

// 0x45A480
void sub_45A480(MagicTechLock* a1)
{
    MagicTechObjectNode* node;

    while (a1->objlist != NULL) {
        node = a1->objlist;
        a1->objlist = node->next;
        mt_obj_node_destroy(node);
    }

    while (a1->summoned_obj != NULL) {
        node = a1->summoned_obj;
        a1->summoned_obj = node->next;
        mt_obj_node_destroy(node);
    }

    FREE(a1);
}

// 0x45A4F0
void sub_45A4F0(int64_t a1, int a2, int a3)
{
    animfx_remove(&stru_5E7568, a1, a2, a3);
}

// 0x45A520
void sub_45A520(int a1, int a2, int a3, int a4)
{
    sub_456E60(a3, a4, 643);
}

// 0x45A540
void sub_45A540(int a1, int a2)
{
    MesFileEntry mes_file_entry;

    mes_file_entry.num = 605;
    sub_44FDC0(&mes_file_entry);
    sub_460610(mes_file_entry.str);
    sub_456E60(a1, a2, 643);
}

// 0x45A580
bool sub_45A580(int64_t a1, int64_t a2)
{
    if ((obj_field_int32_get(a2, OBJ_F_SPELL_FLAGS) & OSF_INVISIBLE) == 0
        || (obj_field_int32_get(a2, OBJ_F_SPELL_FLAGS) & OSF_DETECTING_INVISIBLE) != 0) {
        return true;
    }

    if ((obj_field_int32_get(a2, OBJ_F_FLAGS) & OF_INVISIBLE) != 0) {
        return false;
    }

    if ((obj_field_int32_get(a2, OBJ_F_TYPE) != OBJ_TYPE_PC)) {
        return true;
    }

    if (sub_44E830(a2, 19, 0)) {
        return true;
    }

    if (sub_44E830(a2, 27, 0)) {
        return true;
    }

    if (combat_critter_is_combat_mode_active(a2)) {
        return true;
    }

    return false;
}

// 0x45A620
void magictech_debug_lists()
{
    int index;
    MagicTechLock* v1;
    MagicTechObjectNode* node;

    tig_debug_printf("\n\nMagicTech DEBUG Lists:\n");
    tig_debug_printf("----------------------\n\n");

    for (index = 0; index < 512; index++) {
        v1 = &(magictech_locks[index]);
        if ((v1->field_13C & 0x1) != 0) {
            tig_debug_printf("mtID: [%d], Spell: %s(%d)\n",
                index,
                magictech_spell_name(v1->spell),
                v1->spell);
            tig_debug_printf("\tAction: %s\n", off_5B0C0C[v1->action]);
            sub_45A760(v1->parent_obj.obj, "Parent");
            sub_45A760(v1->source_obj.obj, "Source");
            sub_45A760(v1->target_obj.obj, "Target");
            node = v1->summoned_obj;
            while (node != NULL) {
                sub_45A760(node->obj, "SummonedObj");
                node = node->next;
            }
            node = v1->objlist;
            while (node != NULL) {
                sub_45A760(node->obj, "ObjList");
                node = node->next;
            }
            tig_debug_printf("\n");
        }
    }

    tig_debug_printf("Local PC Has %d Maintained Spells\n",
        sub_450AC0(player_get_pc_obj()));
    tig_debug_printf("\n\n");
}

// 0x45A760
void sub_45A760(object_id_t obj, const char* msg)
{
    // 0x5E6D94
    static char buffer[2000];

    if (obj != OBJ_HANDLE_NULL) {
        sub_441B60(obj, obj, buffer);
        tig_debug_printf("\t%s: %s(%I64d), Loc: [%I64d x %I64d]\n",
            msg,
            buffer,
            obj,
            obj_field_int64_get(obj, OBJ_F_LOCATION),
            obj_field_int64_get(obj, OBJ_F_LOCATION));
    }
}
