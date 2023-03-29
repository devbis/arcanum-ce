#include "game/lib/magictech.h"

#include "game/lib/animfx.h"
#include "game/lib/message.h"
#include "game/lib/stat.h"
#include "tig/str_parse.h"

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
};

typedef struct MagicTech_EC {
    void* field_0;
    int field_4;
};

typedef struct MagicTechInfoAI {
    int flee;
    int summon;
    int defensive1;
    int offensive;
    int healing_light;
    int healing_medium;
    int healing_heavy;
    int cure_poison;
    int fatigue_recover;
    int resurrect;
    int defensive2;
};

// See 0x450090.
static_assert(sizeof(MagicTechInfoAI) == 0x28, "wrong size");

typedef struct MagicTechInfo {
    int iq;
    int cost;
    int resist_stat;
    int resist_value;
    int maintain1;
    int maintain2;
    int duration1;
    int duration2;
    int duration_stat;
    int duration_stat_value;
    int duration_trigger_count;
    int range;
    unsigned int flags;
    unsigned int item_triggers;
    int begin_caster;
    int begin_target;
    int maintain_caster;
    int maintain_target;
    int end_caster;
    int end_target;
    int callback_caster;
    int callback_target;
    int end_callback_caster;
    int end_callback_target;
    int missile;
    int casting_anim;
    int field_80;
    MagicTech_EC field_EC[5];
    int no_stack;
    int field_114;
    unsigned int cancels_sf;
    unsigned int disallowed_sf;
    unsigned int disallowed_tsf;
    unsigned int disallowed_tcf;
    unsigned int cancels_envsf;
    MagicTechInfoAI ai;
};

static_assert(sizeof(MagicTechInfo) == 0x158, "wrong size");

typedef struct MagicTechLock {
    int field_0;
    int field_4;
    int field_8;
    int field_C;
    int field_10;
    int field_14;
    int field_18;
    int field_1C;
    int field_20;
    int field_24;
    int field_28;
    int field_2C;
    int field_30;
    int field_34;
    int field_38;
    int field_3C;
    int field_40;
    int field_44;
    int field_48;
    int field_4C;
    int field_50;
    int field_54;
    int field_58;
    int field_5C;
    int field_60;
    int field_64;
    int field_68;
    int field_6C;
    int field_70;
    int field_74;
    int field_78;
    int field_7C;
    int field_80;
    int field_84;
    int field_88;
    int field_8C;
    int field_90;
    int field_94;
    int field_98;
    int field_9C;
    int field_A0;
    int field_A4;
    int field_A8;
    int field_AC;
    int field_B0;
    int field_B4;
    int field_B8;
    int field_BC;
    int field_C0;
    int field_C4;
    int field_C8;
    int field_CC;
    int field_D0;
    int field_D4;
    int field_D8;
    int field_DC;
    int field_E0;
    int field_E4;
    int field_E8;
    int field_EC;
    int field_F0;
    int field_F4;
    int field_F8;
    int field_FC;
    int field_100;
    int field_104;
    int field_108;
    int field_10C;
    int field_110;
    int field_114;
    int field_118;
    int field_11C;
    int field_120;
    int field_124;
    int field_128;
    int field_12C;
    int field_130;
    int field_134;
    int field_138;
    int field_13C;
    int field_140;
    int field_144;
    int field_148;
    int field_14C;
    int field_150;
    int field_154;
};

static_assert(sizeof(MagicTechLock) == 0x158, "wrong size");

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

// 0x5E3510
static bool dword_5E3510;

// 0x5E6D20
static int dword_5E6D20;

// 0x5E6D24
static GameContextF8* dword_5E6D24;

// 0x5E7568
static AnimFxList stru_5E7568;

// 0x5E7594
static int magictech_spell_msg_file;

// 0x5E75F4
static int magictech_msg_file;

// 0x5E75F8
static char** magictech_component_names;

// 0x5E75FC
static int dword_5E75FC;

// 0x5E7618
static bool magictech_initialized;

// 0x6876D8
static MagicTechInfo* magictech_spells;

// 0x6876DC
static int dword_6876DC;

// 0x6876E0
static MagicTechLock* magictech_locks;

// 0x44EF50
bool magictech_init(GameContext* ctx)
{
    dword_5E6D24 = ctx->field_8;
    dword_5E3510 = ctx->editor;
    magictech_component_names = (char**)calloc(25, sizeof(char*));
    magictech_locks = (MagicTechLock*)calloc(512, sizeof(MagicTechLock));
    sub_455710();

    if (!message_load("Rules\\magictech.mes", &magictech_msg_file)) {
        free(magictech_component_names);
        free(magictech_locks);
        return false;
    }

    if (!message_load("mes\\spell.mes", &magictech_spell_msg_file)) {
        free(magictech_component_names);
        free(magictech_locks);
        return false;
    }

    MessageListItem msg;
    msg.num = 10;
    for (int index = 0; index < 25; index++) {
        if (!message_find(magictech_msg_file, &msg)) {
            free(magictech_component_names);
            free(magictech_locks);
            return false;
        }

        magictech_component_names[index] = msg.text;
        msg.num++;
    }

    if (dword_5E3510 != 1) {
        if (!animfx_list_init(&stru_5E7568)) {
            free(magictech_component_names);
            free(magictech_locks);
            return false;
        }

        stru_5E7568.path = "Rules\\SpellEyeCandy.mes";
        stru_5E7568.field_18 = 1344;
        stru_5E7568.field_C = 6;
        stru_5E7568.field_10 = 10;
        stru_5E7568.field_20 = 6;
        stru_5E7568.field_24 = dword_5B0DC8;
        if (!animfx_list_load(&stru_5E7568)) {
            free(magictech_component_names);
            free(magictech_locks);
            return false;
        }
    }

    magictech_spells = (MagicTechInfo*)malloc(sizeof(MagicTechInfo) * MT_SPELL_COUNT);

    if (!sub_44FE30(0, "Rules\\spelllist.mes", 1000)) {
        free(magictech_component_names);
        free(magictech_locks);
        return false;
    }

    magictech_initialized = true;
    dword_6876DC = 0;

    return true;
}

// 0x44F150
void magictech_reset()
{
    dword_5E75FC = 0;
}

// 0x44F160
bool magictech_post_init(GameContext* ctx)
{
    if (!sub_44FFA0(0, "Rules\\spelllist.mes", 1000)) {
        free(magictech_component_names);
        free(magictech_locks);
        magictech_initialized = false;
        return false;
    }

    return true;
}

// 0x44F1B0
void magictech_exit()
{
    if (magictech_initialized) {
        if (dword_5E3510 != 1) {
            if (stru_5E7568.field_14) {
                animfx_list_exit(&stru_5E7568);
            }
        }

        sub_450240();

        message_unload(magictech_spell_msg_file);
        message_unload(magictech_msg_file);

        if (magictech_spells) {
            free(magictech_spells);
            magictech_spells = NULL;
        }

        if (magictech_component_names) {
            free(magictech_component_names);
        }

        if (magictech_locks) {
            free(magictech_locks);
        }

        magictech_initialized = false;
    }
}

// 0x44FDC0
bool sub_44FDC0(MessageListItem *msg)
{
    return sub_4D43A0(magictech_spell_msg_file, msg);
}

// 0x44FDE0
const char* sub_44FDE0(int num)
{
    if (num >= 0 && num < MT_SPELL_COUNT) {
        MessageListItem msg;
        msg.num = num;
        sub_4D43A0(magictech_spell_msg_file, &msg);
        return msg.text;
    } else {
        return NULL;
    }
}

// 0x44FE20
void sub_44FE20()
{
    dword_5E75FC = 1;
}

// 0x44FE30
bool sub_44FE30(int a1, const char* path, int a3)
{
    if (!message_load(path, &dword_5E6D20)) {
        return false;
    }

    for (int index = 0; index < MT_SPELL_COUNT; index++) {
        for (int k = 0; k < 5; k++) {
            magictech_spells[index].field_EC[k].field_0 = 0;
            magictech_spells[index].field_EC[k].field_4 = 0;
        }
    }

    int v1 = 0;
    for (int index = 0; index < MT_80; index++) {
        sub_450090(dword_5E6D20, &(magictech_spells[index]), a3, v1++);
    }

    for (int index = v1; index < MT_140; index++) {
        sub_450090(dword_5E6D20, &(magictech_spells[index]), a3, v1++);
        magictech_spells[index].iq = 0;
    }

    for (int index = MT_140; index < MT_SPELL_COUNT; index++) {
        sub_450090(dword_5E6D20, &(magictech_spells[index]), a3 + 2000, v1++);
        magictech_spells[index].flags |= 0x04;
        magictech_spells[index].iq = 0;
    }

    for (int index = v1; index < MT_SPELL_COUNT; index++) {
        sub_450090(dword_5E6D20, &(magictech_spells[index]), a3, v1++);
        magictech_spells[index].iq = 0;
    }

    return true;
}

// 0x44FFA0
bool sub_44FFA0(int a1, const char* a2, int a3)
{
    int v1 = 0;
    for (int index = 0; index < MT_80; index++) {
        sub_4501D0(dword_5E6D20, &(magictech_spells[index]), a3, v1++);
    }

    for (int index = v1; index < MT_140; index++) {
        sub_4501D0(dword_5E6D20, &(magictech_spells[index]), a3, v1++);
        magictech_spells[index].iq = 0;
    }

    for (int index = MT_140; index < MT_SPELL_COUNT; index++) {
        sub_4501D0(dword_5E6D20, &(magictech_spells[index]), a3 + 2000, v1++);
        magictech_spells[index].flags |= 0x04;
        magictech_spells[index].iq = 0;
    }

    message_unload(dword_5E6D20);

    return true;
}

// 0x450090
void sub_450090(int msg_file, MagicTechInfo* info, int num, int index)
{
    MessageListItem msg;

    dword_5E7628 = index;

    sub_457580(info, index);

    msg.num = num;
    sub_4D43A0(magictech_spell_msg_file, &msg);
    info->field_0 = msg.text;

    msg.num = num + 50 * index;
    sub_4D43A0(magictech_spell_msg_file, &msg);
    magictech_build_aoe_info(info, msg.text);

    msg.num++;
    sub_4D43A0(magictech_spell_msg_file, &msg);
    sub_4578F0(info, msg.text);

    msg.num++;
    if (message_find(msg_file, &msg)) {
        sub_4D43A0(magictech_spell_msg_file, &msg);
        sub_457B20(info, msg.text);
    }

    msg.num++;
    if (message_find(msg_file, &msg)) {
        sub_4D43A0(msg_file, &msg);
        sub_457D00(info, msg.text);
    }

    memset(&(info->ai), 0xFF, sizeof(info->ai));
    info->ai.defensive2 = 0;

    msg.num++;
    if (message_find(msg_file, &msg)) {
        sub_4D43A0(msg_file, &msg);
        magictech_build_ai_info(info, msg.text);
    }
}

// 0x4501D0
void sub_4501D0(int msg_file, MagicTechInfo* info, int num, int index)
{
    MessageListItem msg;
    msg.num = num + 50 * index + 5;
    for (int index = 0; index < 44; index++) {
        if (!message_find(msg_file, &msg)) {
            return;
        }

        sub_4D43A0(msg_file, &msg);
        magictech_build_effect_info(msg_file, msg.text);

        msg.num++;
    }
}

// 0x450240
void sub_450240()
{
    for (int index = 0; index < MT_SPELL_COUNT; index++) {
        for (int k = 0; k < 5; k++) {
            if (magictech_spells[index].field_EC[k].field_0 != NULL) {
                free(magictech_spells[index].field_EC[k].field_0);
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
int sub_4502E0()
{
    return 5;
}

// 0x4502F0
int magictech_get_iq(int magictech)
{
    if (magictech_initialized) {
        return magictech_spells[magictech].iq;
    } else {
        if (magictech >= 0 && magictech < MT_80) {
            return dword_5B0DE0[magictech % 5];
        } else {
            dword_5B0DE0[1];
        }
    }
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
        return magictech_spells[magictech].maintain2 > 0;
    } else {
        return false;
    }
}

// 0x4503E0
int magictech_get_maintain1(int magictech)
{
    return magictech_spells[magictech].maintain1;
}

// 0x450400
int magictech_get_duration1(int magictech)
{
    return magictech_spells[magictech].duration1;
}

// 0x450B90
int sub_450B90(object_id_t obj)
{
    return stat_level(obj, STAT_INTELLIGENCE) / 4;
}

// 0x450C10
void sub_450C10(object_id_t obj, unsigned int flags)
{
    object_field_set(obj, OBJ_F_SPELL_FLAGS, object_field_get(obj, OBJ_F_SPELL_FLAGS) | flags);
}

// 0x455710
void sub_455710()
{
    for (int index = 0; index < 512; index++) {
        MagicTechLock* lock = &(magictech_locks[index]);
        lock->field_10 = 0;
        lock->field_0 = -1;
        lock->field_14 = 0;
        lock->field_13C = 0;
    }
}

// 0x4578F0
void sub_4578F0(MagicTechInfo* info, char* str)
{
    tig_str_parse_set_separator(',');

    char* curr = str;
    int value1;
    int value2;

    if (tig_str_parse_named_value(&curr, "IQ:", &value1)) {
        info->iq = value1;
    }

    if (tig_str_parse_named_value(&curr, "Cost:", &value1)) {
        info->cost = value1;
    }

    if (tig_str_parse_named_complex_value(&curr, "Maintain:", '@', &value1, &value2)) {
        info->maintain1 = value1;
        info->maintain2 = value2;
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
    tig_str_parse_set_separator(',');

    char* curr = str;
    int value;

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
    tig_str_parse_set_separator(',');

    char* curr = str;
    unsigned int flags;

    int value;
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
    tig_str_parse_set_separator(',');

    char* curr = str;
    int value1;
    int value2;

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
