#ifndef ARCANUM_GAME_OBJ_H_
#define ARCANUM_GAME_OBJ_H_

#include "game/context.h"

#define OBJ_HANDLE_NULL 0LL

typedef long long object_id_t;

typedef enum ObjectField {
    OBJ_F_BEGIN,
    OBJ_F_CURRENT_AID,
    OBJ_F_LOCATION,
    OBJ_F_OFFSET_X,
    OBJ_F_OFFSET_Y,
    OBJ_F_SHADOW,
    OBJ_F_OVERLAY_FORE,
    OBJ_F_OVERLAY_BACK,
    OBJ_F_UNDERLAY,
    OBJ_F_BLIT_FLAGS,
    OBJ_F_BLIT_COLOR,
    OBJ_F_BLIT_ALPHA,
    OBJ_F_BLIT_SCALE,
    OBJ_F_LIGHT_FLAGS,
    OBJ_F_LIGHT_AID,
    OBJ_F_LIGHT_COLOR,
    OBJ_F_OVERLAY_LIGHT_FLAGS,
    OBJ_F_OVERLAY_LIGHT_AID,
    OBJ_F_OVERLAY_LIGHT_COLOR,
    OBJ_F_FLAGS,
    OBJ_F_SPELL_FLAGS,
    OBJ_F_BLOCKING_MASK,
    OBJ_F_NAME,
    OBJ_F_DESCRIPTION,
    OBJ_F_AID,
    OBJ_F_DESTROYED_AID,
    OBJ_F_AC,
    OBJ_F_HP_PTS,
    OBJ_F_HP_ADJ,
    OBJ_F_HP_DAMAGE,
    OBJ_F_MATERIAL,
    OBJ_F_RESISTANCE_IDX,
    OBJ_F_SCRIPTS_IDX,
    OBJ_F_SOUND_EFFECT,
    OBJ_F_CATEGORY,
    OBJ_F_PAD_IAS_1,
    OBJ_F_PAD_I64AS_1,
    OBJ_F_END,
    OBJ_F_WALL_BEGIN,
    OBJ_F_WALL_FLAGS,
    OBJ_F_WALL_PAD_I_1,
    OBJ_F_WALL_PAD_I_2,
    OBJ_F_WALL_PAD_IAS_1,
    OBJ_F_WALL_PAD_I64AS_1,
    OBJ_F_WALL_END,
    OBJ_F_PORTAL_BEGIN,
    OBJ_F_PORTAL_FLAGS,
    OBJ_F_PORTAL_LOCK_DIFFICULTY,
    OBJ_F_PORTAL_KEY_ID,
    OBJ_F_PORTAL_NOTIFY_NPC,
    OBJ_F_PORTAL_PAD_I_1,
    OBJ_F_PORTAL_PAD_I_2,
    OBJ_F_PORTAL_PAD_IAS_1,
    OBJ_F_PORTAL_PAD_I64AS_1,
    OBJ_F_PORTAL_END,
    OBJ_F_CONTAINER_BEGIN,
    OBJ_F_CONTAINER_FLAGS,
    OBJ_F_CONTAINER_LOCK_DIFFICULTY,
    OBJ_F_CONTAINER_KEY_ID,
    OBJ_F_CONTAINER_INVENTORY_NUM,
    OBJ_F_CONTAINER_INVENTORY_LIST_IDX,
    OBJ_F_CONTAINER_INVENTORY_SOURCE,
    OBJ_F_CONTAINER_NOTIFY_NPC,
    OBJ_F_CONTAINER_PAD_I_1,
    OBJ_F_CONTAINER_PAD_I_2,
    OBJ_F_CONTAINER_PAD_IAS_1,
    OBJ_F_CONTAINER_PAD_I64AS_1,
    OBJ_F_CONTAINER_END,
    OBJ_F_SCENERY_BEGIN,
    OBJ_F_SCENERY_FLAGS,
    OBJ_F_SCENERY_WHOS_IN_ME,
    OBJ_F_SCENERY_RESPAWN_DELAY,
    OBJ_F_SCENERY_PAD_I_2,
    OBJ_F_SCENERY_PAD_IAS_1,
    OBJ_F_SCENERY_PAD_I64AS_1,
    OBJ_F_SCENERY_END,
    OBJ_F_PROJECTILE_BEGIN,
    OBJ_F_PROJECTILE_FLAGS_COMBAT,
    OBJ_F_PROJECTILE_FLAGS_COMBAT_DAMAGE,
    OBJ_F_PROJECTILE_HIT_LOC,
    OBJ_F_PROJECTILE_PARENT_WEAPON,
    OBJ_F_PROJECTILE_PAD_I_1,
    OBJ_F_PROJECTILE_PAD_I_2,
    OBJ_F_PROJECTILE_PAD_IAS_1,
    OBJ_F_PROJECTILE_PAD_I64AS_1,
    OBJ_F_PROJECTILE_END,
    OBJ_F_ITEM_BEGIN,
    OBJ_F_ITEM_FLAGS,
    OBJ_F_ITEM_PARENT,
    OBJ_F_ITEM_WEIGHT,
    OBJ_F_ITEM_MAGIC_WEIGHT_ADJ,
    OBJ_F_ITEM_WORTH,
    OBJ_F_ITEM_MANA_STORE,
    OBJ_F_ITEM_INV_AID,
    OBJ_F_ITEM_INV_LOCATION,
    OBJ_F_ITEM_USE_AID_FRAGMENT,
    OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY,
    OBJ_F_ITEM_DISCIPLINE,
    OBJ_F_ITEM_DESCRIPTION_UNKNOWN,
    OBJ_F_ITEM_DESCRIPTION_EFFECTS,
    OBJ_F_ITEM_SPELL_1,
    OBJ_F_ITEM_SPELL_2,
    OBJ_F_ITEM_SPELL_3,
    OBJ_F_ITEM_SPELL_4,
    OBJ_F_ITEM_SPELL_5,
    OBJ_F_ITEM_SPELL_MANA_STORE,
    OBJ_F_ITEM_AI_ACTION,
    OBJ_F_ITEM_PAD_I_1,
    OBJ_F_ITEM_PAD_IAS_1,
    OBJ_F_ITEM_PAD_I64AS_1,
    OBJ_F_ITEM_END,
    OBJ_F_WEAPON_BEGIN,
    OBJ_F_WEAPON_FLAGS,
    OBJ_F_WEAPON_PAPER_DOLL_AID,
    OBJ_F_WEAPON_BONUS_TO_HIT,
    OBJ_F_WEAPON_MAGIC_HIT_ADJ,
    OBJ_F_WEAPON_DAMAGE_LOWER_IDX,
    OBJ_F_WEAPON_DAMAGE_UPPER_IDX,
    OBJ_F_WEAPON_MAGIC_DAMAGE_ADJ_IDX,
    OBJ_F_WEAPON_SPEED_FACTOR,
    OBJ_F_WEAPON_MAGIC_SPEED_ADJ,
    OBJ_F_WEAPON_RANGE,
    OBJ_F_WEAPON_MAGIC_RANGE_ADJ,
    OBJ_F_WEAPON_MIN_STRENGTH,
    OBJ_F_WEAPON_MAGIC_MIN_STRENGTH_ADJ,
    OBJ_F_WEAPON_AMMO_TYPE,
    OBJ_F_WEAPON_AMMO_CONSUMPTION,
    OBJ_F_WEAPON_MISSILE_AID,
    OBJ_F_WEAPON_VISUAL_EFFECT_AID,
    OBJ_F_WEAPON_CRIT_HIT_CHART,
    OBJ_F_WEAPON_MAGIC_CRIT_HIT_CHANCE,
    OBJ_F_WEAPON_MAGIC_CRIT_HIT_EFFECT,
    OBJ_F_WEAPON_CRIT_MISS_CHART,
    OBJ_F_WEAPON_MAGIC_CRIT_MISS_CHANCE,
    OBJ_F_WEAPON_MAGIC_CRIT_MISS_EFFECT,
    OBJ_F_WEAPON_PAD_I_1,
    OBJ_F_WEAPON_PAD_I_2,
    OBJ_F_WEAPON_PAD_IAS_1,
    OBJ_F_WEAPON_PAD_I64AS_1,
    OBJ_F_WEAPON_END,
    OBJ_F_AMMO_BEGIN,
    OBJ_F_AMMO_FLAGS,
    OBJ_F_AMMO_QUANTITY,
    OBJ_F_AMMO_TYPE,
    OBJ_F_AMMO_PAD_I_1,
    OBJ_F_AMMO_PAD_I_2,
    OBJ_F_AMMO_PAD_IAS_1,
    OBJ_F_AMMO_PAD_I64AS_1,
    OBJ_F_AMMO_END,
    OBJ_F_ARMOR_BEGIN,
    OBJ_F_ARMOR_FLAGS,
    OBJ_F_ARMOR_PAPER_DOLL_AID,
    OBJ_F_ARMOR_AC_ADJ,
    OBJ_F_ARMOR_MAGIC_AC_ADJ,
    OBJ_F_ARMOR_RESISTANCE_ADJ_IDX,
    OBJ_F_ARMOR_MAGIC_RESISTANCE_ADJ_IDX,
    OBJ_F_ARMOR_SILENT_MOVE_ADJ,
    OBJ_F_ARMOR_MAGIC_SILENT_MOVE_ADJ,
    OBJ_F_ARMOR_UNARMED_BONUS_DAMAGE,
    OBJ_F_ARMOR_PAD_I_2,
    OBJ_F_ARMOR_PAD_IAS_1,
    OBJ_F_ARMOR_PAD_I64AS_1,
    OBJ_F_ARMOR_END,
    OBJ_F_GOLD_BEGIN,
    OBJ_F_GOLD_FLAGS,
    OBJ_F_GOLD_QUANTITY,
    OBJ_F_GOLD_PAD_I_1,
    OBJ_F_GOLD_PAD_I_2,
    OBJ_F_GOLD_PAD_IAS_1,
    OBJ_F_GOLD_PAD_I64AS_1,
    OBJ_F_GOLD_END,
    OBJ_F_FOOD_BEGIN,
    OBJ_F_FOOD_FLAGS,
    OBJ_F_FOOD_PAD_I_1,
    OBJ_F_FOOD_PAD_I_2,
    OBJ_F_FOOD_PAD_IAS_1,
    OBJ_F_FOOD_PAD_I64AS_1,
    OBJ_F_FOOD_END,
    OBJ_F_SCROLL_BEGIN,
    OBJ_F_SCROLL_FLAGS,
    OBJ_F_SCROLL_PAD_I_1,
    OBJ_F_SCROLL_PAD_I_2,
    OBJ_F_SCROLL_PAD_IAS_1,
    OBJ_F_SCROLL_PAD_I64AS_1,
    OBJ_F_SCROLL_END,
    OBJ_F_KEY_BEGIN,
    OBJ_F_KEY_KEY_ID,
    OBJ_F_KEY_PAD_I_1,
    OBJ_F_KEY_PAD_I_2,
    OBJ_F_KEY_PAD_IAS_1,
    OBJ_F_KEY_PAD_I64AS_1,
    OBJ_F_KEY_END,
    OBJ_F_KEY_RING_BEGIN,
    OBJ_F_KEY_RING_FLAGS,
    OBJ_F_KEY_RING_LIST_IDX,
    OBJ_F_KEY_RING_PAD_I_1,
    OBJ_F_KEY_RING_PAD_I_2,
    OBJ_F_KEY_RING_PAD_IAS_1,
    OBJ_F_KEY_RING_PAD_I64AS_1,
    OBJ_F_KEY_RING_END,
    OBJ_F_WRITTEN_BEGIN,
    OBJ_F_WRITTEN_FLAGS,
    OBJ_F_WRITTEN_SUBTYPE,
    OBJ_F_WRITTEN_TEXT_START_LINE,
    OBJ_F_WRITTEN_TEXT_END_LINE,
    OBJ_F_WRITTEN_PAD_I_1,
    OBJ_F_WRITTEN_PAD_I_2,
    OBJ_F_WRITTEN_PAD_IAS_1,
    OBJ_F_WRITTEN_PAD_I64AS_1,
    OBJ_F_WRITTEN_END,
    OBJ_F_GENERIC_BEGIN,
    OBJ_F_GENERIC_FLAGS,
    OBJ_F_GENERIC_USAGE_BONUS,
    OBJ_F_GENERIC_USAGE_COUNT_REMAINING,
    OBJ_F_GENERIC_PAD_IAS_1,
    OBJ_F_GENERIC_PAD_I64AS_1,
    OBJ_F_GENERIC_END,
    OBJ_F_CRITTER_BEGIN,
    OBJ_F_CRITTER_FLAGS,
    OBJ_F_CRITTER_FLAGS2,
    OBJ_F_CRITTER_STAT_BASE_IDX,
    OBJ_F_CRITTER_BASIC_SKILL_IDX,
    OBJ_F_CRITTER_TECH_SKILL_IDX,
    OBJ_F_CRITTER_SPELL_TECH_IDX,
    OBJ_F_CRITTER_FATIGUE_PTS,
    OBJ_F_CRITTER_FATIGUE_ADJ,
    OBJ_F_CRITTER_FATIGUE_DAMAGE,
    OBJ_F_CRITTER_CRIT_HIT_CHART,
    OBJ_F_CRITTER_EFFECTS_IDX,
    OBJ_F_CRITTER_EFFECT_CAUSE_IDX,
    OBJ_F_CRITTER_FLEEING_FROM,
    OBJ_F_CRITTER_PORTRAIT,
    OBJ_F_CRITTER_GOLD,
    OBJ_F_CRITTER_ARROWS,
    OBJ_F_CRITTER_BULLETS,
    OBJ_F_CRITTER_POWER_CELLS,
    OBJ_F_CRITTER_FUEL,
    OBJ_F_CRITTER_INVENTORY_NUM,
    OBJ_F_CRITTER_INVENTORY_LIST_IDX,
    OBJ_F_CRITTER_INVENTORY_SOURCE,
    OBJ_F_CRITTER_DESCRIPTION_UNKNOWN,
    OBJ_F_CRITTER_FOLLOWER_IDX,
    OBJ_F_CRITTER_TELEPORT_DEST,
    OBJ_F_CRITTER_TELEPORT_MAP,
    OBJ_F_CRITTER_DEATH_TIME,
    OBJ_F_CRITTER_AUTO_LEVEL_SCHEME,
    OBJ_F_CRITTER_PAD_I_1,
    OBJ_F_CRITTER_PAD_I_2,
    OBJ_F_CRITTER_PAD_I_3,
    OBJ_F_CRITTER_PAD_IAS_1,
    OBJ_F_CRITTER_PAD_I64AS_1,
    OBJ_F_CRITTER_END,
    OBJ_F_PC_BEGIN,
    OBJ_F_PC_FLAGS,
    OBJ_F_PC_FLAGS_FATE,
    OBJ_F_PC_REPUTATION_IDX,
    OBJ_F_PC_REPUTATION_TS_IDX,
    OBJ_F_PC_BACKGROUND,
    OBJ_F_PC_BACKGROUND_TEXT,
    OBJ_F_PC_QUEST_IDX,
    OBJ_F_PC_BLESSING_IDX,
    OBJ_F_PC_BLESSING_TS_IDX,
    OBJ_F_PC_CURSE_IDX,
    OBJ_F_PC_CURSE_TS_IDX,
    OBJ_F_PC_PARTY_ID,
    OBJ_F_PC_RUMOR_IDX,
    OBJ_F_PC_PAD_IAS_2,
    OBJ_F_PC_SCHEMATICS_FOUND_IDX,
    OBJ_F_PC_LOGBOOK_EGO_IDX,
    OBJ_F_PC_FOG_MASK,
    OBJ_F_PC_PLAYER_NAME,
    OBJ_F_PC_BANK_MONEY,
    OBJ_F_PC_GLOBAL_FLAGS,
    OBJ_F_PC_GLOBAL_VARIABLES,
    OBJ_F_PC_PAD_I_1,
    OBJ_F_PC_PAD_I_2,
    OBJ_F_PC_PAD_IAS_1,
    OBJ_F_PC_PAD_I64AS_1,
    OBJ_F_PC_END,
    OBJ_F_NPC_BEGIN,
    OBJ_F_NPC_FLAGS,
    OBJ_F_NPC_LEADER,
    OBJ_F_NPC_AI_DATA,
    OBJ_F_NPC_COMBAT_FOCUS,
    OBJ_F_NPC_WHO_HIT_ME_LAST,
    OBJ_F_NPC_EXPERIENCE_WORTH,
    OBJ_F_NPC_EXPERIENCE_POOL,
    OBJ_F_NPC_WAYPOINTS_IDX,
    OBJ_F_NPC_WAYPOINT_CURRENT,
    OBJ_F_NPC_STANDPOINT_DAY,
    OBJ_F_NPC_STANDPOINT_NIGHT,
    OBJ_F_NPC_ORIGIN,
    OBJ_F_NPC_FACTION,
    OBJ_F_NPC_RETAIL_PRICE_MULTIPLIER,
    OBJ_F_NPC_SUBSTITUTE_INVENTORY,
    OBJ_F_NPC_REACTION_BASE,
    OBJ_F_NPC_SOCIAL_CLASS,
    OBJ_F_NPC_REACTION_PC_IDX,
    OBJ_F_NPC_REACTION_LEVEL_IDX,
    OBJ_F_NPC_REACTION_TIME_IDX,
    OBJ_F_NPC_WAIT,
    OBJ_F_NPC_GENERATOR_DATA,
    OBJ_F_NPC_PAD_I_1,
    OBJ_F_NPC_DAMAGE_IDX,
    OBJ_F_NPC_SHIT_LIST_IDX,
    OBJ_F_NPC_END,
    OBJ_F_TRAP_BEGIN,
    OBJ_F_TRAP_FLAGS,
    OBJ_F_TRAP_DIFFICULTY,
    OBJ_F_TRAP_PAD_I_2,
    OBJ_F_TRAP_PAD_IAS_1,
    OBJ_F_TRAP_PAD_I64AS_1,
    OBJ_F_TRAP_END,
    OBJ_F_TOTAL_NORMAL,
    OBJ_F_TRANSIENT_BEGIN,
    OBJ_F_RENDER_COLOR,
    OBJ_F_RENDER_COLORS,
    OBJ_F_RENDER_PALETTE,
    OBJ_F_RENDER_SCALE,
    OBJ_F_RENDER_ALPHA,
    OBJ_F_RENDER_X,
    OBJ_F_RENDER_Y,
    OBJ_F_RENDER_WIDTH,
    OBJ_F_RENDER_HEIGHT,
    OBJ_F_PALETTE,
    OBJ_F_COLOR,
    OBJ_F_COLORS,
    OBJ_F_RENDER_FLAGS,
    OBJ_F_TEMP_ID,
    OBJ_F_LIGHT_HANDLE,
    OBJ_F_OVERLAY_LIGHT_HANDLES,
    OBJ_F_SHADOW_HANDLES,
    OBJ_F_INTERNAL_FLAGS,
    OBJ_F_FIND_NODE,
    OBJ_F_TRANSIENT_END,
    OBJ_F_TYPE,
    OBJ_F_PROTOTYPE_HANDLE,
    OBJ_F_MAX,
} ObjectField;

typedef enum ObjectType {
    OBJ_TYPE_WALL,
    OBJ_TYPE_PORTAL,
    OBJ_TYPE_CONTAINER,
    OBJ_TYPE_SCENERY,
    OBJ_TYPE_PROJECTILE,
    OBJ_TYPE_WEAPON,
    OBJ_TYPE_AMMO,
    OBJ_TYPE_ARMOR,
    OBJ_TYPE_GOLD,
    OBJ_TYPE_FOOD,
    OBJ_TYPE_SCROLL,
    OBJ_TYPE_KEY,
    OBJ_TYPE_KEY_RING,
    OBJ_TYPE_WRITTEN,
    OBJ_TYPE_GENERIC,
    OBJ_TYPE_PC,
    OBJ_TYPE_NPC,
    OBJ_TYPE_TRAP,
    OBJ_TYPE_MONSTER,
    OBJ_TYPE_UNIQUE_NPC,
    OBJ_TYPE_COUNT,
} ObjectType;

typedef enum ObjectFlags {
// clang-format off
    OF_DESTROYED        = 0x00000001,
    OF_OFF              = 0x00000002,
    OF_FLAT             = 0x00000004,
    OF_TEXT             = 0x00000008,
    OF_SEE_THROUGH      = 0x00000010,
    OF_SHOOT_THROUGH    = 0x00000020,
    OF_TRANSLUCENT      = 0x00000040,
    OF_SHRUNK           = 0x00000080,
    OF_DONTDRAW         = 0x00000100,
    OF_INVISIBLE        = 0x00000200,
    OF_NO_BLOCK         = 0x00000400,
    OF_CLICK_THROUGH    = 0x00000800,
    OF_INVENTORY        = 0x00001000,
    OF_DYNAMIC          = 0x00002000,
    OF_PROVIDES_COVER   = 0x00004000,
    OF_HAS_OVERLAYS     = 0x00008000,
    OF_HAS_UNDERLAYS    = 0x00010000,
    OF_WADING           = 0x00020000,
    OF_WATER_WALKING    = 0x00040000,
    OF_STONED           = 0x00080000,
    OF_DONTLIGHT        = 0x00100000,
    OF_TEXT_FLOATER     = 0x00200000,
    OF_INVULNERABLE     = 0x00400000,
    OF_EXTINCT          = 0x00800000,
    OF_TRAP_PC          = 0x01000000,
    OF_TRAP_SPOTTED     = 0x02000000,
    OF_DISALLOW_WADING  = 0x04000000,
    OF_MULTIPLAYER_LOCK = 0x08000000,
    OF_FROZEN           = 0x10000000,
    OF_ANIMATED_DEAD    = 0x20000000,
    OF_TELEPORTED       = 0x40000000,
// clang-format on
} ObjectFlags;

typedef enum ObjectSpellFlags {
// clang-format off
    OSF_INVISIBLE           = 0x00000001,
    OSF_FLOATING            = 0x00000002,
    OSF_BODY_OF_AIR         = 0x00000004,
    OSF_BODY_OF_EARTH       = 0x00000008,
    OSF_BODY_OF_FIRE        = 0x00000010,
    OSF_BODY_OF_WATER       = 0x00000020,
    OSF_DETECTING_MAGIC     = 0x00000040,
    OSF_DETECTING_ALIGNMENT = 0x00000080,
    OSF_DETECTING_TRAPS     = 0x00000100,
    OSF_DETECTING_INVISIBLE = 0x00000200,
    OSF_SHIELDED            = 0x00000400,
    OSF_ANTI_MAGIC_SHELL    = 0x00000800,
    OSF_BONDS_OF_MAGIC      = 0x00001000,
    OSF_FULL_REFLECTION     = 0x00002000,
    OSF_SUMMONED            = 0x00004000,
    OSF_ILLUSION            = 0x00008000,
    OSF_STONED              = 0x00010000,
    OSF_POLYMORPHED         = 0x00020000,
    OSF_MIRRORED            = 0x00040000,
    OSF_SHRUNK              = 0x00080000,
    OSF_PASSWALLED          = 0x00100000,
    OSF_WATER_WALKING       = 0x00200000,
    OSF_MAGNETIC_INVERSION  = 0x00400000,
    OSF_CHARMED             = 0x00800000,
    OSF_ENTANGLED           = 0x01000000,
    OSF_SPOKEN_WITH_DEAD    = 0x02000000,
    OSF_TEMPUS_FUGIT        = 0x04000000,
    OSF_MIND_CONTROLLED     = 0x08000000,
    OSF_DRUNK               = 0x10000000,
    OSF_ENSHROUDED          = 0x20000000,
    OSF_FAMILIAR            = 0x40000000,
    OSF_HARDENED_HANDS      = 0x80000000,
// clang-format on
} ObjectSpellFlags;

typedef enum ObjectWallFlags {
// clang-format off
    OWAF_TRANS_DISALLOW = 0x0001,
    OWAF_TRANS_LEFT     = 0x0002,
    OWAF_TRANS_RIGHT    = 0x0004,
    OWAF_TRANS_ALL      = 0x0008,
// clang-format on
} ObjectWallFlags;

typedef enum ObjectPortalFlags {
// clang-format off
    OPF_LOCKED         = 0x0001,
    OPF_JAMMED         = 0x0002,
    OPF_MAGICALLY_HELD = 0x0004,
    OPF_NEVER_LOCKED   = 0x0008,
    OPF_ALWAYS_LOCKED  = 0x0010,
    OPF_LOCKED_DAY     = 0x0020,
    OPF_LOCKED_NIGHT   = 0x0040,
    OPF_BUSTED         = 0x0080,
    OPF_STICKY         = 0x0100,
// clang-format on
} ObjectPortalFlags;

typedef enum ObjectContainerFlags {
// clang-format off
    OCOF_LOCKED         = 0x0001,
    OCOF_JAMMED         = 0x0002,
    OCOF_MAGICALLY_HELD = 0x0004,
    OCOF_NEVER_LOCKED   = 0x0008,
    OCOF_ALWAYS_LOCKED  = 0x0010,
    OCOF_LOCKED_DAY     = 0x0020,
    OCOF_LOCKED_NIGHT   = 0x0040,
    OCOF_BUSTED         = 0x0080,
    OCOF_STICKY         = 0x0100,
// clang-format on
} ObjectContainerFlags;

typedef enum ObjectSceneryFlags {
// clang-format off
    OSCF_NO_AUTO_ANIMATE   = 0x0001,
    OSCF_BUSTED            = 0x0002,
    OSCF_NOCTURNAL         = 0x0004,
    OSCF_MARKS_TOWNMAP     = 0x0008,
    OSCF_IS_FIRE           = 0x0010,
    OSCF_RESPAWNABLE       = 0x0020,
    OSCF_SOUND_SMALL       = 0x0040,
    OSCF_SOUND_MEDIUM      = 0x0080,
    OSCF_SOUND_EXTRA_LARGE = 0x0100,
// clang-format on
} ObjectSceneryFlags;

typedef enum ObjectItemFlags {
// clang-format off
    OIF_IDENTIFIED      = 0x00000001,
    OIF_WONT_SELL       = 0x00000002,
    OIF_IS_MAGICAL      = 0x00000004,
    OIF_TRANSFER_LIGHT  = 0x00000008,
    OIF_NO_DISPLAY      = 0x00000010,
    OIF_NO_DROP         = 0x00000020,
    OIF_HEXED           = 0x00000040,
    OIF_CAN_USE_BOX     = 0x00000080,
    OIF_NEEDS_TARGET    = 0x00000100,
    OIF_LIGHT_SMALL     = 0x00000200,
    OIF_LIGHT_MEDIUM    = 0x00000400,
    OIF_LIGHT_LARGE     = 0x00000800,
    OIF_LIGHT_XLARGE    = 0x00001000,
    OIF_PERSISTENT      = 0x00002000,
    OIF_MT_TRIGGERED    = 0x00004000,
    OIF_STOLEN          = 0x00008000,
    OIF_USE_IS_THROW    = 0x00010000,
    OIF_NO_DECAY        = 0x00020000,
    OIF_UBER            = 0x00040000,
    OIF_NO_NPC_PICKUP   = 0x00080000,
    OIF_NO_RANGED_USE   = 0x00100000,
    OIF_VALID_AI_ACTION = 0x00200000,
    OIF_MP_INSERTED     = 0x00400000,
// clang-format on
} ObjectItemFlags;

#define OIF_LIGHT_ANY (OIF_LIGHT_XLARGE | OIF_LIGHT_LARGE | OIF_LIGHT_MEDIUM | OIF_LIGHT_SMALL)

typedef enum ObjectWeaponFlags {
// clang-format off
    OWF_LOUD              = 0x0001,
    OWF_SILENT            = 0x0002,
    OWF_TWO_HANDED        = 0x0004,
    OWF_HAND_COUNT_FIXED  = 0x0008,
    OWF_THROWABLE         = 0x0010,
    OWF_TRANS_PROJECTILE  = 0x0020,
    OWF_BOOMERANGS        = 0x0040,
    OWF_IGNORE_RESISTANCE = 0x0080,
    OWF_DAMAGE_ARMOR      = 0x0100,
    OWF_DEFAULT_THROWS    = 0x0200,
// clang-format on
} ObjectWeaponFlags;

typedef enum ObjectAmmoFlags {
// clang-format off
    OAF_NONE = 0x0001,
// clang-format on
} ObjectAmmoFlags;

typedef enum ObjectArmorFlags {
// clang-format off
    OARF_SIZE_SMALL  = 0x0001,
    OARF_SIZE_MEDIUM = 0x0002,
    OARF_SIZE_LARGE  = 0x0004,
    OARF_MALE_ONLY   = 0x0008,
    OARF_FEMALE_ONLY = 0x0010,
// clang-format on
} ObjectArmorFlags;

typedef enum ObjectGoldFlags {
// clang-format off
    OGOF_NONE = 0x0001,
// clang-format on
} ObjectGoldFlags;

typedef enum ObjectFoodFlags {
// clang-format off
    OFF_NONE = 0x0001,
// clang-format on
} ObjectFoodFlags;

typedef enum ObjectScrollFlags {
// clang-format off
    OSRF_NONE = 0x0001,
// clang-format on
} ObjectScrollFlags;

typedef enum ObjectKeyRingFlags {
// clang-format off
    OKRF_PRIMARY_RING = 0x0001,
// clang-format on
} ObjectKeyFingFlags;

typedef enum ObjectWrittenFlags {
// clang-format off
    OWRF_NONE = 0x0001,
// clang-format on
} ObjectWrittenFlags;

typedef enum ObjectGenericFlags {
// clang-format off
    OGF_USES_TORCH_SHIELD_LOCATION = 0x0001,
    OGF_IS_LOCKPICK                = 0x0002,
    OGF_IS_TRAP_DEVICE             = 0x0004,
    OGF_IS_HEALING_ITEM            = 0x0008,
// clang-format on
} ObjectGenericFlags;

typedef enum ObjectCritterFlags {
// clang-format off
    OCF_IS_CONCEALED       = 0x00000001,
    OCF_MOVING_SILENTLY    = 0x00000002,
    OCF_UNDEAD             = 0x00000004,
    OCF_ANIMAL             = 0x00000008,
    OCF_FLEEING            = 0x00000010,
    OCF_STUNNED            = 0x00000020,
    OCF_PARALYZED          = 0x00000040,
    OCF_BLINDED            = 0x00000080,
    OCF_CRIPPLED_ARMS_ONE  = 0x00000100,
    OCF_CRIPPLED_ARMS_BOTH = 0x00000200,
    OCF_CRIPPLED_LEGS_BOTH = 0x00000400,
    OCF_UNUSED             = 0x00000800,
    OCF_SLEEPING           = 0x00001000,
    OCF_MUTE               = 0x00002000,
    OCF_SURRENDERED        = 0x00004000,
    OCF_MONSTER            = 0x00008000,
    OCF_SPELL_FLEE         = 0x00010000,
    OCF_ENCOUNTER          = 0x00020000,
    OCF_COMBAT_MODE_ACTIVE = 0x00040000,
    OCF_LIGHT_SMALL        = 0x00080000,
    OCF_LIGHT_MEDIUM       = 0x00100000,
    OCF_LIGHT_LARGE        = 0x00200000,
    OCF_LIGHT_XLARGE       = 0x00400000,
    OCF_UNREVIVIFIABLE     = 0x00800000,
    OCF_UNRESSURECTABLE    = 0x01000000,
    OCF_DEMON              = 0x02000000,
    OCF_FATIGUE_IMMUNE     = 0x04000000,
    OCF_NO_FLEE            = 0x08000000,
    OCF_NON_LETHAL_COMBAT  = 0x10000000,
    OCF_MECHANICAL         = 0x20000000,
    OCF_ANIMAL_ENSHROUD    = 0x40000000,
    OCF_FATIGUE_LIMITING   = 0x80000000,
// clang-format on
} ObjectCritterFlags;

#define OCF_CRIPPLED (OCF_CRIPPLED_ARMS_ONE | OCF_CRIPPLED_ARMS_BOTH | OCF_CRIPPLED_LEGS_BOTH)
#define OCF_INJURED (OCF_BLINDED | OCF_CRIPPLED)
#define OCF_LIGHT_ANY (OCF_LIGHT_XLARGE | OCF_LIGHT_LARGE | OCF_LIGHT_MEDIUM | OCF_LIGHT_SMALL)

typedef enum ObjectCritterFlags2 {
// clang-format off
    OCF2_ITEM_STOLEN        = 0x00000001,
    OCF2_AUTO_ANIMATES      = 0x00000002,
    OCF2_USING_BOOMERANG    = 0x00000004,
    OCF2_FATIGUE_DRAINING   = 0x00000008,
    OCF2_SLOW_PARTY         = 0x00000010,
    OCF2_COMBAT_TOGGLE_FX   = 0x00000020,
    OCF2_NO_DECAY           = 0x00000040,
    OCF2_NO_PICKPOCKET      = 0x00000080,
    OCF2_NO_BLOOD_SPLOTCHES = 0x00000100,
    OCF2_NIGH_INVULNERABLE  = 0x00000200,
    OCF2_ELEMENTAL          = 0x00000400,
    OCF2_DARK_SIGHT         = 0x00000800,
    OCF2_NO_SLIP            = 0x00001000,
    OCF2_NO_DISINTEGRATE    = 0x00002000,
    OCF2_REACTION_0         = 0x00004000,
    OCF2_REACTION_1         = 0x00008000,
    OCF2_REACTION_2         = 0x00010000,
    OCF2_REACTION_3         = 0x00020000,
    OCF2_REACTION_4         = 0x00040000,
    OCF2_REACTION_5         = 0x00080000,
    OCF2_REACTION_6         = 0x00100000,
    OCF2_TARGET_LOCK        = 0x00200000,
    OCF2_PERMA_POLYMORPH    = 0x00400000,
    OCF2_SAFE_OFF           = 0x00800000,
    OCF2_CHECK_REACTION_BAD = 0x01000000,
    OCF2_CHECK_ALIGN_GOOD   = 0x02000000,
    OCF2_CHECK_ALIGN_BAD    = 0x04000000,
// clang-format on
} ObjectCritterFlags2;

typedef enum ObjectPcFlags {
// clang-format off
    OPCF_unused_1           = 0x0001,
    OPCF_unused_2           = 0x0002,
    OPCF_USE_ALT_DATA       = 0x0004,
    OPCF_unused_4           = 0x0008,
    OPCF_unused_5           = 0x0010,
    OPCF_FOLLOWER_SKILLS_ON = 0x0020,
// clang-format on
} ObjectPcFlags;

typedef enum ObjectNpcFlags {
// clang-format off
    ONF_FIGHTING          = 0x00000001,
    ONF_WAYPOINTS_DAY     = 0x00000002,
    ONF_WAYPOINTS_NIGHT   = 0x00000004,
    ONF_AI_WAIT_HERE      = 0x00000008,
    ONF_AI_SPREAD_OUT     = 0x00000010,
    ONF_JILTED            = 0x00000020,
    ONF_CHECK_WIELD       = 0x00000040,
    ONF_CHECK_WEAPON      = 0x00000080,
    ONF_KOS               = 0x00000100,
    ONF_WAYPOINTS_BED     = 0x00000200,
    ONF_FORCED_FOLLOWER   = 0x00000400,
    ONF_KOS_OVERRIDE      = 0x00000800,
    ONF_WANDERS           = 0x00001000,
    ONF_WANDERS_IN_DARK   = 0x00002000,
    ONF_FENCE             = 0x00004000,
    ONF_FAMILIAR          = 0x00008000,
    ONF_CHECK_LEADER      = 0x00010000,
    ONF_ALOOF             = 0x00020000,
    ONF_CAST_HIGHEST      = 0x00040000,
    ONF_GENERATOR         = 0x00080000,
    ONF_GENERATED         = 0x00100000,
    ONF_GENERATOR_RATE1   = 0x00200000,
    ONF_GENERATOR_RATE2   = 0x00400000,
    ONF_GENERATOR_RATE3   = 0x00800000,
    ONF_DEMAINTAIN_SPELLS = 0x01000000,
    ONF_LOOK_FOR_WEAPON   = 0x02000000,
    ONF_LOOK_FOR_ARMOR    = 0x04000000,
    ONF_LOOK_FOR_AMMO     = 0x08000000,
    ONF_BACKING_OFF       = 0x10000000,
    ONF_NO_ATTACK         = 0x20000000,
    ONF_40000000          = 0x40000000,
// clang-format on
} ObjectNpcFlags;

typedef enum ObjectTrapFlags {
// clang-format off
    OTF_BUSTED = 0x0001,
// clang-format on
} ObjectTrapFlags;

#define ORF_01000000 0x01000000
#define ORF_02000000 0x02000000
#define ORF_04000000 0x04000000
#define ORF_08000000 0x08000000
#define ORF_10000000 0x10000000
#define ORF_20000000 0x20000000
#define ORF_40000000 0x40000000
#define ORF_80000000 0x80000000

typedef struct ObjectID_P {
    long long location;
    int temp_id;
    int map;
} ObjectID_P;

#define OID_TYPE_HANDLE ((int16_t)-2)
#define OID_TYPE_BLOCKED ((int16_t)-1)
#define OID_TYPE_NULL ((int16_t)0)
#define OID_TYPE_A ((int16_t)1)
#define OID_TYPE_GUID ((int16_t)2)
#define OID_TYPE_P ((int16_t)3)

typedef struct ObjectID {
    int16_t type;
    union {
        int64_t h;
        int a;
        GUID g;
        ObjectID_P p;
    } d;
} ObjectID;

static_assert(sizeof(ObjectID) == 0x18, "wrong size");

typedef struct Object {
    /* 0000 */ int type;
    /* 0008 */ ObjectID oid;
    /* 0020 */ ObjectID prototype_oid;
    /* 0038 */ int64_t prototype_obj;
    /* 0040 */ int field_40;
    /* 0044 */ int16_t modified;
    /* 0046 */ int16_t num_fields;
    /* 0048 */ int* field_48;
    /* 004C */ int* field_4C;
    /* 0050 */ int* data;
    /* 0054 */ int transient_properties[19];
} Object;

static_assert(sizeof(Object) == 0xA0, "wrong size");

typedef bool (ObjEnumerateCallback)(Object* object, int fld);

bool obj_init(GameInitInfo* init_info);
void obj_exit();
void sub_405250();
bool obj_validate_system(unsigned int flags);
void sub_405790(int64_t obj_handle);
void sub_405800(int type, int64_t* obj_ptr);
void sub_4058E0(int64_t proto_obj, int64_t loc, int64_t* obj_ptr);
void sub_405B30(int64_t proto_obj, int64_t loc, ObjectID oid, int64_t* obj_ptr);
void sub_408D60(Object* object, int fld, int* value_ptr);
void sub_408E70(Object* object, int fld, int value);
bool sub_405BC0(int64_t obj);
void sub_405BF0(int64_t obj);
void sub_405CC0(int64_t obj);
void sub_405D60(int64_t* new_obj_handle_ptr, int64_t obj_handle);
void obj_perm_dup(int64_t* copy_obj_ptr, int64_t existing_obj);
void sub_406210(int64_t* copy, int64_t obj, ObjectID* oids);
void sub_4063A0(int64_t obj, ObjectID** oids_ptr, int* cnt_ptr);
void sub_4064B0(int64_t obj_handle);
void sub_406520(int64_t obj_handle);
bool obj_write(TigFile* stream, int64_t obj_handle);
bool obj_read(TigFile* stream, int64_t* obj_handle_ptr);
void sub_4066B0(uint8_t** a1, int* a2, int64_t obj);
bool sub_406730(uint8_t* data, int64_t* obj_ptr);
int obj_is_modified(int64_t obj);
bool obj_dif_write(TigFile* stream, int64_t obj_handle);
bool obj_dif_read(TigFile* stream, int64_t obj);
void sub_406B80(int64_t obj_handle);
int obj_field_int32_get(object_id_t obj_handle, int field);
void obj_field_int32_set(object_id_t obj_handle, int field, int value);
int64_t obj_field_int64_get(object_id_t obj_handle, int field);
void obj_field_int64_set(object_id_t obj_handle, int fld, int64_t value);
int64_t obj_field_handle_get(int64_t obj_handle, int fld);
void obj_field_handle_set(int64_t obj_handle, int fld, int64_t value);
bool obj_field_obj_get(int64_t obj_handle, int fld, int64_t* value_ptr);
ObjectID sub_407100(int64_t obj_handle, int fld);
void obj_field_string_get(int64_t obj_handle, int fld, char** value_ptr);
void obj_field_string_set(int64_t obj_handle, int fld, const char* value);
int obj_arrayfield_int32_get(int64_t obj_handle, int fld, int index);
void obj_arrayfield_int32_set(int64_t obj_handle, int fld, int index, int value);
unsigned int obj_arrayfield_uint32_get(int64_t obj, int fld, int index);
void obj_arrayfield_uint32_set(int64_t obj, int fld, int index, unsigned int value);
int64_t obj_arrayfield_int64_get(int64_t obj_handle, int fld, int index);
void obj_arrayfield_int64_set(int64_t obj_handle, int fld, int index, int64_t value);
int64_t obj_arrayfield_handle_get(int64_t obj_handle, int fld, int index);
bool obj_arrayfield_obj_get(int64_t obj_handle, int fld, int index, int64_t* value_ptr);
void obj_arrayfield_obj_set(int64_t obj_handle, int fld, int index, int64_t value);
void obj_arrayfield_script_get(int64_t obj, int fld, int index, void* value);
void obj_arrayfield_script_set(int64_t obj, int fld, int index, void* value);
void obj_arrayfield_pc_quest_get(int64_t obj, int fld, int index, void* value);
void obj_arrayfield_pc_quest_set(int64_t obj, int fld, int index, void* value);
int obj_arrayfield_length_get(int64_t obj_handle, int fld);
void obj_arrayfield_length_set(int64_t obj_handle, int fld, int length);
void obj_arrayfield_pc_rumor_copy_to_flat(int64_t obj, int fld, int cnt, void* data);
void obj_arrayfield_pc_quest_copy_to_flat(int64_t obj, int fld, int cnt, void* data);
void sub_407D50(int64_t obj, int fld);
ObjectID sub_407EF0(int64_t obj);
ObjectID sub_408020(int64_t obj, int a2);
bool sub_4082C0(int64_t* obj_ptr, int* iter_ptr);
bool sub_408390(int64_t* obj_ptr, int* iter_ptr);
Object* obj_lock(int64_t obj_handle);
void obj_unlock(int64_t obj_handle);
int sub_40C030(ObjectType object_type);
bool sub_40C260(int type, int fld);
bool obj_enumerate_fields(Object* object, ObjEnumerateCallback* callback);
int64_t obj_get_prototype_handle(Object* object);

// NOTE: Seen in some assertions in `anim.c`.
static inline bool obj_type_is_critter(int type)
{
    return type == OBJ_TYPE_PC || type == OBJ_TYPE_NPC;
}

// If `obj_type_is_critter` exists, `obj_type_is_item` should also be there.
static inline bool obj_type_is_item(int type)
{
    return type >= OBJ_TYPE_WEAPON && type <= OBJ_TYPE_GENERIC;
}

// NOTE: Convenience.
static inline bool inventory_fields_from_obj_type(int type, int* inventory_num_fld, int* inventory_list_idx_fld)
{
    if (type == OBJ_TYPE_CONTAINER) {
        *inventory_num_fld = OBJ_F_CONTAINER_INVENTORY_NUM;
        *inventory_list_idx_fld = OBJ_F_CONTAINER_INVENTORY_LIST_IDX;
        return true;
    }

    if (obj_type_is_critter(type)) {
        *inventory_num_fld = OBJ_F_CRITTER_INVENTORY_NUM;
        *inventory_list_idx_fld = OBJ_F_CRITTER_INVENTORY_LIST_IDX;
        return true;
    }

    return false;
}

#endif /* ARCANUM_GAME_OBJ_H_ */
