#ifndef ARCANUM_GAME_MP_UTILS_H_
#define ARCANUM_GAME_MP_UTILS_H_

#include "game/anim_private.h"
#include "game/context.h"
#include "game/combat.h"
#include "game/dialog.h"
#include "game/obj.h"
#include "game/target.h"
#include "game/magictech.h"
#include "game/script.h"
#include "game/ui.h"

typedef struct Packet2 {
    /* 0000 */ int type;
    /* 0004 */ int field_4;
    /* 0004 */ ObjectID oids[8];
} Packet2;

static_assert(sizeof(Packet2) == 0xC8, "wrong size");

typedef struct PacketGameTime {
    /* 0000 */ int type;
    /* 0008 */ uint64_t game_time;
    /* 0010 */ uint64_t anim_time;
} PacketGameTime;

static_assert(sizeof(PacketGameTime) == 0x18, "wrong size");

typedef struct Packet4 {
    /* 0000 */ int type;
    /* 0004 */ int subtype;
    /* 0008 */ ObjectID oid;
    /* 0020 */ int64_t loc;
} Packet4;

static_assert(sizeof(Packet4) == 0x28, "wrong size");

typedef struct Packet5 {
    /* 0000 */ int type;
    /* 0008 */ DateTime field_8;
    /* 0010 */ AnimGoalData field_10;
    /* 0188 */ int64_t loc;
    /* 0190 */ int offset_x;
    /* 0194 */ int offset_y;
    /* 0198 */ AnimID field_198;
    /* 01A4 */ int field_1A4;
} Packet5;

static_assert(sizeof(Packet5) == 0x1A8, "wrong size");

typedef struct Packet6 {
    /* 0000 */ int type;
    /* 0004 */ int subtype;
    /* 0008 */ tig_art_id_t art_id;
    /* 0010 */ ObjectID self_oid;
    /* 0028 */ ObjectID target_oid;
    /* 0040 */ int64_t loc;
    /* 0048 */ int64_t target_loc;
    /* 0050 */ int spell;
    /* 0054 */ AnimID anim_id;
    /* 0060 */ ObjectID obj_oid;
} Packet6;

static_assert(sizeof(Packet6) == 0x78, "wrong size");

typedef struct Packet7 {
    /* 0000 */ int type;
    /* 0004 */ AnimID anim_id;
    /* 0010 */ AnimGoalData goal_data;
    /* 0188 */ int64_t loc;
    /* 0190 */ int offset_x;
    /* 0194 */ int offset_y;
} Packet7;

static_assert(sizeof(Packet7) == 0x198, "wrong size");

typedef struct Packet8 {
    /* 0000 */ int type;
    /* 0004 */ int field_4;
    /* 0008 */ AGModifyData modify_data;
    /* 0038 */ int offset_x;
    /* 003C */ int offset_y;
    /* 0040 */ int64_t field_40;
    /* 0048 */ int field_48;
    /* 004C */ int field_4C;
} Packet8;

static_assert(sizeof(Packet8) == 0x50, "wrong size");

typedef struct Packet9 {
    /* 0000 */ int type;
    /* 0004 */ int field_4;
    /* 0008 */ int priority_level;
    /* 000C */ int field_C;
    /* 0010 */ int field_10;
    /* 0014 */ int field_14;
    /* 0018 */ FollowerInfo field_18;
    /* 0048 */ int field_48;
    /* 004C */ int field_4C;
    /* 0050 */ int64_t loc;
    /* 0058 */ int offset_x;
    /* 005C */ int offset_y;
    /* 0060 */ int art_id;
    /* 0064 */ int field_64;
} Packet9;

static_assert(sizeof(Packet9) == 0x68, "wrong size");

typedef struct Packet10 {
    /* 0000 */ int type;
    /* 0004 */ AnimID anim_id;
    /* 0010 */ ObjectID oid;
    /* 0028 */ int64_t loc;
    /* 0030 */ int offset_x;
    /* 0034 */ int offset_y;
    /* 0038 */ tig_art_id_t art_id;
    /* 003C */ int flags;
} Packet10;

static_assert(sizeof(Packet10) == 0x40, "wrong size");

typedef struct Packet16 {
    /* 0000 */ int type;
    /* 0004 */ AnimID anim_id;
    /* 0010 */ int64_t loc;
    /* 0018 */ int offset_x;
    /* 001C */ int offset_y;
    /* 0020 */ int art_id;
    /* 0024 */ int anim_flags;
    /* 0028 */ int path_flags;
    /* 002C */ int field_2C;
    /* 0030 */ int path_base_rot;
    /* 0034 */ int path_curr;
    /* 0038 */ int path_max;
    /* 003C */ int path_subsequence;
    /* 0040 */ int path_max_path_length;
    /* 0044 */ int path_abs_max_path_length;
    /* 0048 */ int64_t field_48;
    /* 0050 */ int64_t field_50;
} Packet16;

// NOTE: May be wrong, see 0x4ED510.
static_assert(sizeof(Packet16) == 0x58, "wrong size");

typedef struct PacketCombatModeSet {
    /* 0000 */ int type;
    /* 0008 */ ObjectID oid;
    /* 0020 */ bool active;
} PacketCombatModeSet;

static_assert(sizeof(PacketCombatModeSet) == 0x28, "wrong size");

typedef struct Packet20 {
    /* 0000 */ int type;
    /* 0008 */ CombatContext combat;
    /* 0070 */ ObjectID field_70;
    /* 0088 */ ObjectID field_88;
    /* 00A0 */ ObjectID field_A0;
    /* 00B8 */ ObjectID field_B8;
    /* 00D0 */ ObjectID field_D0;
} Packet20;

static_assert(sizeof(Packet20) == 0xE8, "wrong size");

typedef struct Packet21 {
    /* 0000 */ int type;
    /* 0004 */ int field_4;
    /* 0008 */ CombatContext combat;
    /* 0070 */ ObjectID field_70;
    /* 0088 */ ObjectID field_88;
    /* 00A0 */ ObjectID field_A0;
    /* 00B8 */ ObjectID field_B8;
    /* 00D0 */ ObjectID field_D0;
} Packet21;

static_assert(sizeof(Packet21) == 0xE8, "wrong size");

typedef struct Packet22 {
    /* 0000 */ int type;
    /* 0008 */ ObjectID oid;
    /* 0020 */ int64_t loc;
} Packet22;

static_assert(sizeof(Packet22) == 0x28, "wrong size");

typedef struct Packet23 {
    /* 0000 */ int type;
    /* 0008 */ ObjectID item_oid;
    /* 0020 */ ObjectID parent_oid;
} Packet23;

static_assert(sizeof(Packet23) == 0x38, "wrong size");

typedef struct Packet24 {
    /* 0000 */ int type;
    /* 0008 */ ObjectID item_oid;
    /* 0020 */ ObjectID parent_oid;
    /* 0038 */ int inventory_location;
} Packet24;

static_assert(sizeof(Packet24) == 0x40, "wrong size");

typedef struct Packet25 {
    /* 0000 */ int type;
    /* 0004 */ int field_4;
    /* 0008 */ ObjectID item_oid;
    /* 0020 */ ObjectID parent_oid;
    /* 0038 */ int field_38;
    /* 003C */ int field_3C;
} Packet25;

typedef struct Packet26 {
    /* 0000 */ int type;
    /* 0008 */ ObjectID oid;
    /* 0020 */ int field_20;
} Packet26;

static_assert(sizeof(Packet26) == 0x28, "wrong size");

typedef struct Packet27 {
    /* 0000 */ int type;
    /* 0008 */ ObjectID oid;
    /* 0020 */ int64_t loc;
} Packet27;

static_assert(sizeof(Packet27) == 0x28, "wrong size");

typedef struct Packet28 {
    /* 0000 */ int type;
    /* 0008 */ ObjectID item_oid;
    /* 0020 */ ObjectID critter_oid;
    /* 0038 */ int inventory_location;
} Packet28;

static_assert(sizeof(Packet28) == 0x40, "wrong size");

typedef struct Packet29 {
    /* 0000 */ int type;
    /* 0008 */ ObjectID oid;
} Packet29;

static_assert(sizeof(Packet29) == 0x20, "wrong size");

typedef struct Packet30 {
    /* 0000 */ int type;
    /* 0004 */ int field_4;
    /* 0008 */ FollowerInfo field_8;
    /* 0038 */ int field_38;
} Packet30;

static_assert(sizeof(Packet30) == 0x40, "wrong size");

typedef struct Packet31 {
    /* 0000 */ int type;
    /* 0004 */ int field_4;
    /* 0008 */ FollowerInfo field_8;
    /* 0038 */ int field_38;
    /* 003C */ int field_3C;
    /* 0040 */ S4F2810 field_40;
    /* 0050 */ ObjectID field_50;
} Packet31;

static_assert(sizeof(Packet31) == 0x68, "wrong size");

typedef struct PacketCritterConcealSet {
    /* 0000 */ int type;
    /* 0008 */ FollowerInfo field_8;
    /* 0038 */ int concealed;
} PacketCritterConcealSet;

static_assert(sizeof(PacketCritterConcealSet) == 0x40, "wrong size");

typedef struct Packet34 {
    /* 0000 */ int type;
    /* 0008 */ ObjectID oid;
    /* 0020 */ int hours;
} Packet34;

static_assert(sizeof(Packet34) == 0x28, "wrong size");

typedef struct PacketCritterFatigueDamageSet {
    /* 0000 */ int type;
    /* 0008 */ ObjectID oid;
    /* 0020 */ int dam;
} PacketCritterFatigueDamageSet;

static_assert(sizeof(PacketCritterFatigueDamageSet) == 0x28, "wrong size");

typedef struct PacketFateStateSet {
    /* 0000 */ int type;
    /* 0004 */ int field_4;
    /* 0008 */ FollowerInfo oid;
    /* 0038 */ int fate;
    /* 003C */ int action;
} PacketFateStateSet;

static_assert(sizeof(PacketFateStateSet) == 0x40, "wrong size");

typedef struct PacketRumorQStateSet {
    int type;
    int rumor;
} PacketRumorQStateSet;

static_assert(sizeof(PacketRumorQStateSet) == 0x8, "wrong size");

typedef struct PacketRumorKnownSet {
    /* 0000 */ int type;
    /* 0004 */ int rumor;
    /* 0008 */ ObjectID oid;
    /* 0020 */ DateTime datetime;
} PacketRumorKnownSet;

static_assert(sizeof(PacketRumorKnownSet) == 0x28, "wrong size");

typedef struct Packet39 {
    /* 0000 */ int type;
    /* 0004 */ int field_4;
    /* 0008 */ FollowerInfo field_8;
    /* 0038 */ int quest;
    /* 003C */ int state;
    /* 0040 */ FollowerInfo field_40;
} Packet39;

static_assert(sizeof(Packet39) == 0x70, "wrong size");

typedef struct Packet40 {
    /* 0000 */ int type;
    /* 0004 */ int field_4;
    /* 0008 */ FollowerInfo field_8;
    /* 0038 */ int field_38;
    /* 003C */ int field_3C;
} Packet40;

static_assert(sizeof(Packet40) == 0x40, "wrong size");

typedef struct ChangeBlessPacket {
    /* 0000 */ int type;
    /* 0004 */ int field_4;
    /* 0008 */ FollowerInfo field_8;
    /* 0038 */ int bless;
    /* 003C */ int add;
} ChangeBlessPacket;

static_assert(sizeof(ChangeBlessPacket) == 0x40, "wrong size");

typedef struct ChangeCursePacket {
    /* 0000 */ int type;
    /* 0004 */ int field_4;
    /* 0008 */ FollowerInfo field_8;
    /* 0038 */ int curse;
    /* 003C */ int add;
} ChangeCursePacket;

static_assert(sizeof(ChangeCursePacket) == 0x40, "wrong size");

typedef struct Packet44 {
    /* 0000 */ int type;
    /* 0004 */ int subtype;
    union {
        struct {
            /* 0008 */ ObjectID field_8;
        } b;
        struct {
            /* 0008 */ ObjectID field_8;
            /* 0020 */ ObjectID field_20;
            /* 0038 */ int field_38;
            /* 003C */ int field_3C;
            /* 0040 */ int field_40;
            /* 0044 */ char field_44[1000];
        } d;
        struct {
            /* 0008 */ int field_8;
            /* 000C */ int field_C;
            /* 0010 */ int field_10;
            /* 0014 */ int field_14;
            /* 0018 */ DialogSerializedData serialized_data;
        } e;
        struct {
            /* 0008 */ ObjectID field_8;
            /* 0020 */ int field_20;
            /* 0024 */ int field_24;
        } f;
    } d;
} Packet44;

static_assert(sizeof(Packet44) == 0x430, "wrong size");

typedef struct Packet46 {
    /* 0000 */ int type;
    /* 0004 */ int player;
    /* 0008 */ int field_8;
    /* 000C */ int field_C;
    /* 0010 */ int field_10;
    /* 0014 */ int field_14;
    /* 0018 */ int field_18;
    /* 001C */ int field_1C;
} Packet46;

static_assert(sizeof(Packet46) == 0x20, "wrong size");

typedef struct PlayerBuySpellPacket {
    /* 0000 */ int type;
    /* 0004 */ int player;
    /* 0008 */ int spell;
    /* 000C */ bool field_C;
} PlayerBuySpellPacket;

static_assert(sizeof(PlayerBuySpellPacket) == 0x10, "wrong size");

typedef struct SetBaseStatPacket {
    /* 0000 */ int type;
    /* 0004 */ int stat;
    /* 0008 */ int value;
    /* 0010 */ ObjectID oid;
} SetBaseStatPacket;

static_assert(sizeof(SetBaseStatPacket) == 0x28, "wrong size");

typedef struct Packet51 {
    /* 0000 */ int type;
    /* 0004 */ int field_4;
    /* 0008 */ ObjectID field_8;
    /* 0020 */ int field_20;
} Packet51;

static_assert(sizeof(Packet51) == 0x28, "wrong size");

typedef struct SetSkillTrainingPacket {
    /* 0000 */ int type;
    /* 0008 */ FollowerInfo field_8;
    /* 0038 */ int skill;
    /* 003C */ int training;
} SetSkillTrainingPacket;

static_assert(sizeof(SetSkillTrainingPacket) == 0x40, "wrong size");

typedef struct Packet54 {
    /* 0000 */ int type;
    /* 0004 */ int magictech_id;
} Packet54;

static_assert(sizeof(Packet54) == 0x08, "wrong size");

typedef struct Packet57 {
    /* 0000 */ int type;
    /* 0004 */ int field_4;
    /* 0008 */ MagicTechSerializedData field_8;
} Packet57;

static_assert(sizeof(Packet57) == 0xE8, "wrong size");

typedef struct Packet58 {
    /* 0000 */ int type;
    /* 0004 */ int field_4;
    /* 0008 */ MagicTechSerializedData field_8;
} Packet58;

static_assert(sizeof(Packet58) == 0xE8, "wrong size");

typedef struct Packet59 {
    /* 0000 */ int type;
    /* 0004 */ int field_4;
} Packet59;

static_assert(sizeof(Packet59) == 0x08, "wrong size");

typedef struct Packet60 {
    /* 0000 */ int type;
    /* 0004 */ int mt_id;
    /* 0008 */ int player;
} Packet60;

static_assert(sizeof(Packet60) == 0xC, "wrong size");

typedef struct Packet64 {
    /* 0000 */ int type;
    /* 0004 */ int player;
    /* 0008 */ int map;
    /* 000C */ char name[TIG_MAX_PATH];
    /* 0110 */ int field_110;
    /* 0114 */ int field_114;
    /* 0118 */ int field_118;
    /* 011C */ int field_11C;
} Packet64;

static_assert(sizeof(Packet64) == 0x120, "wrong size");

typedef struct Packet67 {
    /* 0000 */ int type;
    /* 0004 */ int field_4;
} Packet67;

static_assert(sizeof(Packet67) == 0x08, "wrong size");

typedef struct Packet68 {
    /* 0000 */ int type;
    /* 0008 */ FollowerInfo field_8;
} Packet68;

static_assert(sizeof(Packet68) == 0x38, "wrong size");

typedef struct Packet69 {
    /* 0000 */ int type;
    /* 0008 */ FollowerInfo field_8;
} Packet69;

static_assert(sizeof(Packet69) == 0x38, "wrong size");

typedef struct Packet70 {
    /* 0000 */ int type;
    /* 0004 */ int subtype;
    union {
        struct {
            /* 0008 */ int name;
            /* 000C */ int field_C;
            /* 0010 */ ObjectID oid;
            /* 0028 */ int64_t loc;
            /* 0030 */ int field_30;
            /* 0034 */ int field_34;
            /* 0038 */ ObjectID field_38;
            /* 0050 */ int field_50;
            /* 0054 */ int field_54;
            /* 0058 */ int field_58;
            /* 005C */ int field_5C;
            /* 0060 */ int field_60;
            /* 0064 */ int field_64;
        } s0;
        struct {
            /* 0008 */ ObjectID oid;
            /* 0020 */ int64_t loc;
            /* 0028 */ int offset_x;
            /* 002C */ int offset_y;
        } s5;
    };
} Packet70;

static_assert(sizeof(Packet70) == 0x68, "wrong size");

typedef struct Packet72 {
    /* 0000 */ int type;
    /* 0008 */ ObjectID oid;
} Packet72;

static_assert(sizeof(Packet72) == 0x20, "wrong size");

typedef struct Packet73 {
    /* 0000 */ int type;
    /* 0008 */ MagicTechSummonInfo summon_info;
} Packet73;

static_assert(sizeof(Packet73) == 0xD8, "wrong size");

typedef struct Packet74 {
    /* 0000 */ int type;
    /* 0004 */ int subtype;
    /* 0008 */ ObjectID oid;
    /* 0020 */ int mt_id;
    /* 0024 */ int field_24;
    /* 0028 */ int field_28;
    /* 002C */ int field_2C;
    /* 0030 */ int field_30;
    /* 0034 */ int field_34;
    /* 0038 */ int field_38;
    /* 003C */ int field_3C;
} Packet74;

static_assert(sizeof(Packet74) == 0x40, "wrong size");

typedef struct Packet75 {
    /* 0000 */  int type;
    /* 0004 */  int field_4;
    /* 0008 */  ObjectID field_8;
    /* 0020 */  ObjectID field_20;
    /* 0038 */  int field_38;
    /* 003C */  int field_3C;
    /* 0040 */  int field_40;
    /* 0044 */  int field_44;
    /* 0048 */  ObjectID field_48;
    /* 0060 */  ObjectID field_60;
} Packet75;

static_assert(sizeof(Packet75) == 0x78, "wrong size");

typedef struct Packet76 {
    /* 0000 */ int type;
    /* 0008 */ ObjectID oid;
    /* 0020 */ int cost;
    /* 0024 */ bool field_24;
    /* 0028 */ int magictech;
} Packet76;

static_assert(sizeof(Packet76) == 0x30, "wrong size");

typedef struct Packet77 {
    /* 0000 */ int type;
    /* 0004 */ int subtype;
    /* 0008 */ ObjectID oid;
    /* 0020 */ int field_20;
    /* 0024 */ int field_24;
} Packet77;

static_assert(sizeof(Packet77) == 0x28, "wrong size");

typedef struct Packet79 {
    /* 0000 */ int type;
    /* 0004 */ int field_4;
    /* 0008 */ ObjectID field_8;
    /* 0020 */ ObjectID field_20;
    /* 003C */ int field_3C;
} Packet79;

static_assert(sizeof(Packet79) == 0x40, "wrong size");

typedef struct Packet80 {
    /* 0000 */ int type;
    /* 0008 */ ObjectID item_oid;
    /* 0020 */ ObjectID parent_oid;
    /* 0038 */ int idx;
    /* 003C */ int field_3C;
} Packet80;

typedef struct Packet81 {
    /* 0000 */ int type;
    /* 0004 */ int field_4;
    /* 0008 */ int field_8;
    /* 000C */ int field_C;
    /* 0010 */ ObjectID field_10;
    /* 0028 */ ObjectID field_28;
    /* 0040 */ ObjectID field_40;
    /* 0058 */ int field_58;
    /* 005C */ int field_5C;
    /* 0060 */ int field_60;
    /* 0064 */ int field_64;
    /* 0068 */ int field_68;
    /* 006C */ int field_6C;
    /* 0070 */ ObjectID field_70;
    /* 0088 */ ObjectID field_88;
    /* 00A0 */ ObjectID field_A0;
} Packet81;

static_assert(sizeof(Packet81) == 0xB8, "wrong size");

typedef struct Packet82 {
    /* 0000 */ int type;
    /* 0004 */ int field_4;
    /* 0008 */ ObjectID field_8;
    /* 0020 */ ObjectID field_20;
    /* 0038 */ int field_38;
    /* 003C */ int field_3C;
    /* 0040 */ ObjectID field_40;
} Packet82;

static_assert(sizeof(Packet82) == 0x58, "wrong size");

typedef struct Packet83 {
    /* 0000 */ int type;
    /* 0004 */ int field_4;
    /* 0008 */ int field_8;
    /* 000C */ char field_C[128];
} Packet83;

static_assert(sizeof(Packet83) == 0x8C, "wrong size");

typedef struct Packet84 {
    /* 0000 */ int type;
    /* 0004 */ int extra_length;
    /* 0008 */ int field_8;
    /* 0010 */ UiMessage ui_message;
} Packet84;

static_assert(sizeof(Packet84) == 0x28, "wrong size");

typedef struct Packet85 {
    /* 0000 */ int type;
    /* 0004 */ int extra_length;
    /* 0008 */ ObjectID oid;
    /* 0020 */ int field_20;
} Packet85;

static_assert(sizeof(Packet85) == 0x28, "wrong size");

typedef struct Packet86 {
    /* 0000 */ int type;
    /* 0004 */ int subtype;
    /* 0008 */ ObjectID oid;
    /* 0020 */ int field_20;
    /* 0024 */ int field_24;
} Packet86;

static_assert(sizeof(Packet86) == 0x28, "wrong size");

typedef struct Packet87 {
    /* 0000 */ int type;
    /* 0008 */ ObjectID field_8;
    /* 0020 */ ObjectID field_20;
    /* 0038 */ int field_38;
} Packet87;

static_assert(sizeof(Packet87) == 0x40, "wrong size");

typedef struct Packet88 {
    /* 0000 */ int type;
    /* 0004 */ int field_4;
    /* 0008 */ int field_8;
    /* 000C */ int field_C;
    /* 0010 */ ObjectID target_oid;
    /* 0028 */ ObjectID source_oid;
    /* 0040 */ int field_40;
} Packet88;

static_assert(sizeof(Packet88) == 0x48, "wrong size");

typedef struct Packet89 {
    /* 0000 */ int type;
    /* 0004 */ int field_4;
    /* 0008 */ ObjectID source_oid;
    /* 0020 */ ObjectID target_oid;
    /* 0038 */ int field_38;
} Packet89;

static_assert(sizeof(Packet89) == 0x40, "wrong size");

typedef struct Packet90 {
    /* 0000 */ int type;
    /* 0008 */ ObjectID field_8;
    /* 0020 */ ObjectID field_20;
    /* 0038 */ int field_38;
} Packet90;

static_assert(sizeof(Packet90) == 0x40, "wrong size");

typedef struct Packet91 {
    /* 0000 */ int type;
    /* 0008 */ ObjectID source_oid;
    /* 0020 */ ObjectID target_oid;
    /* 0038 */ int field_38;
    /* 003C */ int field_3C;
} Packet91;

static_assert(sizeof(Packet91) == 0x40, "wrong size");

typedef struct Packet92 {
    /* 0000 */ int type;
    /* 0008 */ ObjectID oid;
    /* 0020 */ tig_art_id_t art_id;
} Packet92;

static_assert(sizeof(Packet92) == 0x28, "wrong size");

typedef struct Packet93 {
    /* 0000 */ int type;
    /* 0000 */ int field_4;
    /* 0008 */ ObjectID oid;
    /* 0020 */ int field_20;
} Packet93;

static_assert(sizeof(Packet93) == 0x28, "wrong size");

typedef struct Packet94 {
    /* 0000 */ int type;
    /* 0008 */ ObjectID oid;
} Packet94;

static_assert(sizeof(Packet94) == 0x20, "wrong size");

typedef struct Packet95 {
    /* 0000 */ int type;
    /* 0008 */ ObjectID oid;
} Packet95;

static_assert(sizeof(Packet95) == 0x20, "wrong size");

typedef struct Packet96 {
    /* 0000 */ int type;
    /* 0008 */ ObjectID oid;
    /* 0020 */ int field_20;
} Packet96;

static_assert(sizeof(Packet96) == 0x28, "wrong size");

typedef struct Packet97 {
    /* 0000 */ int type;
    /* 0008 */ ObjectID field_8;
    /* 0020 */ ObjectID field_20;
} Packet97;

static_assert(sizeof(Packet97) == 0x38, "wrong size");

typedef struct Packet98 {
    /* 0000 */ int type;
    /* 0004 */ int client_id;
    /* 0008 */ unsigned int flags;
} Packet98;

static_assert(sizeof(Packet98) == 0xC, "wrong size");

// typedef struct Packet100 {
//     /* 0000 */ int type;
//     /* 0004 */ int subtype;
//     /* 0008 */ ObjectID field_8;
//     /* 0020 */ ObjectID field_20;
//     /* 0038 */ int field_38;
//     /* 003C */ int field_3C;
// } Packet100;

// static_assert(sizeof(Packet100) == 0x40, "wrong size");

typedef struct Packet99 {
    /* 0000 */ int type;
    /* 0008 */ ObjectID oid;
    /* 0020 */ int64_t location;
    /* 0028 */ int dx;
    /* 002C */ int dy;
    /* 0030 */ int field_30;
} Packet99;

static_assert(sizeof(Packet99) == 0x38, "wrong size");

typedef struct Packet100 {
    /* 0000 */ int type;
    /* 0004 */ int subtype;
    union {
        struct {
            /* 0008 */ int field_8;
            /* 000C */ int field_C;
        } a;
        struct {
            /* 0008 */ ObjectID field_8;
        } b;
        struct {
            /* 0008 */ int field_8;
            /* 0010 */ ObjectID field_10;
            /* 0028 */ ObjectID field_28;
        } c;
        struct {
            /* 0008 */ ObjectID field_8;
            /* 0020 */ ObjectID field_20;
        } s;
        struct {
            /* 0008 */ ObjectID field_8;
            /* 0020 */ int field_20;
        } x;
        struct {
            /* 0008 */ ObjectID field_8;
            /* 0020 */ ObjectID field_20;
            /* 0038 */ int field_38;
            /* 003C */ int field_3C;
        } z;
    } d;
} Packet100;

static_assert(sizeof(Packet100) == 0x40, "wrong size");

typedef struct Packet101 {
    /* 0000 */ int type;
    /* 0008 */ ObjectID oid;
    /* 0020 */ int area;
} Packet101;

static_assert(sizeof(Packet101) == 0x28, "wrong size");

typedef struct Packet102 {
    /* 0000 */ int type;
    /* 0008 */ ObjectID oid;
} Packet102;

static_assert(sizeof(Packet102) == 0x20, "wrong size");

typedef struct PacketObjectLock {
    /* 0000 */ int type;
    /* 0008 */ ObjectID oid;
    /* 0020 */ int locked;
} PacketObjectLock;

static_assert(sizeof(PacketObjectLock) == 0x28, "wrong size");

typedef struct Packet104 {
    /* 0000 */ int type;
    /* 0008 */ ObjectID oid;
} Packet104;

static_assert(sizeof(Packet104) == 0x20, "wrong size");

typedef struct PacketPlaySound {
    /* 0000 */ int type;
    /* 0004 */ int subtype;
    union {
        struct {
        /* 0008 */ int sound_id;
        /* 000C */ int loops;
        /* 0010 */ ObjectID oid;
        };
        struct {
        /* 0008 */ int music_scheme_idx;
        /* 000C */ int ambient_scheme_idx;
        };
    };
} PacketPlaySound;

static_assert(sizeof(PacketPlaySound) == 0x28, "wrong size");

typedef struct PacketPortalToggle {
    /* 0000 */ int type;
    /* 0008 */ ObjectID oid;
} PacketPortalToggle;

static_assert(sizeof(PacketPortalToggle) == 0x20, "wrong size");

typedef struct PacketSectorBlockSet {
    /* 0000 */ int type;
    /* 0008 */ int64_t sec;
    /* 0010 */ bool blocked;
} PacketSectorBlockSet;

static_assert(sizeof(PacketSectorBlockSet) == 0x18, "wrong size");

typedef struct PacketSpellMasterySet {
    /* 0000 */ int type;
    /* 0008 */ ObjectID oid;
    /* 0020 */ int college;
} PacketSpellMasterySet;

static_assert(sizeof(PacketSpellMasterySet) == 0x28, "wrong size");

typedef struct Packet110 {
    /* 0000 */ int type;
    /* 0004 */ int map;
    /* 0008 */ int field_8;
} Packet110;

static_assert(sizeof(Packet110) == 0xC, "wrong size");

typedef struct Packet111 {
    /* 0000 */ int type;
    /* 0004 */ tig_art_id_t art_id;
} Packet111;

static_assert(sizeof(Packet111) == 0x8, "wrong size");

typedef struct Packet112 {
    /* 0000 */ int type;
    /* 0004 */ int field_4;
    /* 0008 */ int64_t field_8;
    /* 0010 */ ObjectID oid;
} Packet112;

static_assert(sizeof(Packet112) == 0x28, "wrong size");

typedef struct PacketTextRemove {
    /* 0000 */ int type;
    /* 0004 */ int subtype;
    /* 0008 */ int action;
    /* 0010 */ ObjectID oid;
} PacketTextRemove;

static_assert(sizeof(PacketTextRemove) == 0x28, "wrong size");

typedef struct Packet117 {
    /* 0000 */ int type;
    /* 0008 */ ObjectID field_8;
    /* 0020 */ ObjectID field_20;
    /* 0038 */ ObjectID field_38;
} Packet117;

static_assert(sizeof(Packet117) == 0x50, "wrong size");

typedef struct Packet118 {
    /* 0000 */ int type;
    /* 0008 */ ObjectID field_8;
    /* 0020 */ ObjectID field_20;
} Packet118;

static_assert(sizeof(Packet118) == 0x38, "wrong size");

typedef struct Packet119 {
    /* 0000 */ int type;
    /* 0008 */ ObjectID oid;
    /* 0020 */ int64_t loc;
    /* 0028 */ int field_28;
    /* 002C */ int field_2C;
} Packet119;

static_assert(sizeof(Packet119) == 0x30, "wrong size");

typedef struct Packet120 {
    /* 0000 */ int type;
    /* 0004 */ AnimID anim_id;
} Packet120;

static_assert(sizeof(Packet120) == 0x10, "wrong size");

typedef struct Packet121 {
    /* 0000 */ int type;
    /* 0004 */ int field_4;
    /* 0008 */ ObjectID oid;
    /* 0020 */ int field_20;
} Packet121;

static_assert(sizeof(Packet121) == 0x28, "wrong size");

typedef struct Packet122 {
    /* 0000 */ int type;
    /* 0004 */ int field_4;
    /* 0008 */ ObjectID oid;
} Packet122;

static_assert(sizeof(Packet122) == 0x20, "wrong size");

typedef struct Packet123 {
    /* 0000 */ int type;
    /* 0004 */ int total_size;
    /* 0008 */ int player;
    /* 000C */ int field_C;
    /* 0010 */ int field_10;
    /* 0014 */ int field_14;
} Packet123;

static_assert(sizeof(Packet123) == 0x18, "wrong size");

typedef struct Packet105 {
    /* 0000 */ int type;
    /* 0004 */ int field_4;
    /* 0008 */ ObjectID pc_oid;
    /* 0020 */ int reputation;
    /* 0024 */ int field_24;
} Packet105;

static_assert(sizeof(Packet105) == 0x28, "wrong size");

typedef struct Packet124 {
    int type;
    int subtype;
    int field_8;
    int field_C;
} Packet124;

static_assert(sizeof(Packet124) == 0x10, "wrong size");

typedef struct Packet125 {
    /* 0000 */ int type;
    /* 0008 */ ObjectID field_8;
    /* 0020 */ ObjectID field_20;
    /* 0038 */ ObjectID field_38;
    /* 0050 */ int field_50;
} Packet125;

static_assert(sizeof(Packet125) == 0x58, "wrong size");

typedef struct Packet126 {
    /* 0000 */ int type;
    /* 0008 */ ObjectID field_8;
    /* 0020 */ ObjectID field_20;
    /* 0038 */ ObjectID field_38;
    /* 0050 */ int field_50;
} Packet126;

static_assert(sizeof(Packet126) == 0x58, "wrong size");

typedef struct Packet127 {
    /* 0000 */ int type;
    /* 0004 */ int field_4;
    /* 0008 */ int field_8;
    /* 000C */ int field_C;
} Packet127;

static_assert(sizeof(Packet127) == 0x10, "wrong size");

typedef struct Packet128 {
    /* 0000 */ int type;
    /* 0008 */ ObjectID target_oid;
    /* 0020 */ ObjectID item_oid;
} Packet128;

static_assert(sizeof(Packet128) == 0x38, "wrong size");

#define P129_SUBTYPE_SCRIPT 6
#define P129_SUBTYPE_INT32_ARRAY 7

typedef struct Packet129 {
    /* 0000 */ int type;
    /* 0004 */ int field_4;
    /* 0008 */ ObjectID oid;
    /* 0020 */ int fld;
    /* 0024 */ int subtype;
    union {
        union {
            struct {
                /* 0028 */ int field_28;
            } a;
            struct {
                /* 0028 */ int field_28;
            } b;
            struct {
                /* 0028 */ int field_28;
            } c;
            struct {
                /* 0028 */ ObjectID oid;
            } d;
            struct {
                /* 0028 */ int field_28;
                /* 0030 */ ObjectID oid;
            } e;
            struct {
                /* 0028 */ int field_28;
                /* 002C */ int field_2C;
            } f;
            struct {
                /* 0028 */ int field_28;
                /* 002C */ int field_2C;
            } g;
            struct {
                /* 0028 */ tig_art_id_t art_id;
            } h;
            struct {
                /* 0028 */ int field_28;
                /* 002C */ int field_2C;
                /* 0030 */ int field_30;
            } i;
            struct {
                /* 0028 */ unsigned int flags;
            } j;
        } d;
        /* 0028 */ int val;
        /* 0028 */ int64_t val64;
        struct {
            /* 0028 */ int idx;
            /* 002C */ int value;
        } int32_array_val;
        struct {
            /* 0028 */ int idx;
            /* 002C */ Script scr;
        } scr_val;
    };
} Packet129;

static_assert(sizeof(Packet129) == 0x48, "wrong size");

typedef struct Packet130 {
    /* 0000 */ int type;
    /* 0004 */ int field_4;
    /* 0008 */ ObjectID oid;
    /* 0020 */ int fld;
    /* 0024 */ int length;
} Packet130;

static_assert(sizeof(Packet130) == 0x28, "wrong size");

void sub_4ED510(AnimID anim_id, int64_t loc, AnimRunInfo* run_info);
bool sub_4ED6C0(int64_t obj);
void sub_4ED720(int64_t obj, int damage);
bool sub_4ED780(int64_t obj, int quest, int state, int64_t a4);
bool mp_object_create(int name, int64_t loc, int64_t* obj_ptr);
void sub_4ED9E0(int64_t obj);
void sub_4EDA60(UiMessage* ui_message, int player, int a3);
void sub_4EDB70(int64_t obj, int a3, const char* str);
void sub_4EDC00(int64_t a1, int64_t a2, int a3);
void sub_4EDC70(int64_t a1, int64_t a2, int a3);
void sub_4EDCE0(int64_t obj, tig_art_id_t art_id);
void sub_4EDDE0(int64_t obj);
void sub_4EDE80(int64_t obj, int a2);
void sub_4EDF20(int64_t obj, int64_t location, int dx, int dy, bool a7);
void sub_4EE060(int64_t a1, int64_t a2);
void sub_4EE0F0(int a1, int64_t a2, int64_t a3);
void mp_ui_follower_refresh();
void sub_4EE1D0(int64_t obj);
void mp_ui_toggle_primary_button(UiPrimaryButton btn, bool on, int client_id);
void sub_4EE290(int64_t obj, int a2, int a3);
void sub_4EE310(int64_t obj, int64_t a2);
void sub_4EE3A0(int64_t obj, int64_t a2);
void mp_ui_show_inven_identify(int64_t pc_obj, int64_t target_obj);
void sub_4EE4C0(int64_t obj, int64_t a2);
void mp_ui_show_inven_npc_identify(int64_t pc_obj, int64_t target_obj);
void mp_container_close(int64_t obj);
void mp_container_open(int64_t obj);
void mp_portal_toggle(int64_t obj);
void mp_sector_block_set(int64_t sec, bool blocked);
void mp_spell_mastery_set(int64_t obj, int college);
void sub_4EF120(int map, int a2);
void sub_4EF190(tig_art_id_t art_id);
void sub_4EF1E0(int64_t a1, int64_t obj);
void mp_tf_remove(int64_t obj);
void mp_tb_remove(int64_t obj);
void sub_4EF6F0(int64_t a1, int64_t a2, int64_t a3);
void sub_4EF830(int64_t a1, int64_t a2);
bool sub_4EF920(int64_t obj, int64_t loc, int64_t* obj_ptr);
void sub_4EFAE0(int64_t obj, int a2);
void sub_4EFBA0(int64_t obj);
void mp_object_locked_set(int64_t obj, int a2);
void sub_4EFC30(int64_t pc_obj, const char* a2, const char* a3);
void mp_gsound_play_sfx(int sound_id);
void sub_4EED00(int64_t obj, int sound_id);
void mp_gsound_play_sfx_on_obj(int sound_id, int loops, int64_t obj);
void mp_gsound_play_scheme(int music_scheme_idx, int ambient_scheme_idx);
void mp_obj_field_int32_set(int64_t obj, int fld, int value);
void mp_obj_field_int64_set(int64_t obj, int fld, int64_t value);
void mp_object_flags_unset(int64_t obj, unsigned int flags);
void mp_object_flags_set(int64_t obj, unsigned int flags);
void mp_obj_field_obj_set(int64_t obj, int fld, int64_t value);
void sub_4F0070(int64_t obj, int fld, int index, int64_t value);
void mp_obj_arrayfield_int32_set(int64_t obj, int fld, int index, int value);
void mp_obj_arrayfield_script_set(int64_t obj, int fld, int index, Script* value);
void mp_obj_arrayfield_uint32_set(int64_t obj, int fld, int index, int value);
void mp_object_set_current_aid(int64_t obj, tig_art_id_t art_id);
void mp_object_overlay_set(int64_t obj, int fld, int index, tig_art_id_t aid);
void mp_item_flags_set(int64_t obj, unsigned int flags_to_add);
void mp_item_flags_unset(int64_t obj, unsigned int flags_to_remove);
void sub_4F0500(int64_t obj, int fld);
void sub_4F0570(int64_t obj, int fld, int length);
void sub_4F0640(int64_t obj, ObjectID* oid_ptr);
void sub_4F0690(ObjectID oid, int64_t* obj_ptr);
void sub_4F06E0(int64_t a1, int64_t a2);
void sub_4F0790(int64_t obj, bool a2);
int sub_4F0BF0(int64_t item_obj, int64_t parent_obj, int64_t target_obj, int type);
int sub_4F0ED0(int64_t obj, int a2);
int sub_4F0FD0(int64_t obj, int a2);
int sub_4F1010(int64_t obj, int a2);
int sub_4F1050(int64_t obj, int a2);

#endif /* ARCANUM_GAME_MP_UTILS_H_ */
