#ifndef ARCANUM_GAME_COMBAT_H_
#define ARCANUM_GAME_COMBAT_H_

#include "game/context.h"
#include "game/timeevent.h"

typedef void(CombatCallbackF0)(int a1);
typedef void(CombatCallbackF4)();
typedef void(CombatCallbackF8)();
typedef void(CombatCallbackFC)(int a1);
typedef void(CombatCallbackF10)(int64_t obj);

typedef struct CombatCallbacks {
    CombatCallbackF0* field_0;
    CombatCallbackF4* field_4;
    CombatCallbackF8* field_8;
    CombatCallbackFC* field_C;
    CombatCallbackF10* field_10;
} CombatCallbacks;

static_assert(sizeof(CombatCallbacks) == 0x14, "wrong size");

typedef struct CombatContext {
    /* 0000 */ int flags;
    /* 0004 */ int field_4;
    /* 0008 */ int64_t field_8;
    /* 0010 */ int64_t weapon_obj;
    /* 0018 */ int skill;
    /* 001C */ int field_1C;
    /* 0020 */ int64_t field_20;
    /* 0028 */ int64_t field_28;
    /* 0030 */ int64_t field_30;
    /* 0038 */ int64_t target_loc;
    /* 0040 */ int field_40;
    /* 0044 */ int field_44[5];
    /* 0058 */ int field_58;
    /* 005C */ int field_5C;
    /* 0060 */ int field_60;
    /* 0064 */ int field_64;
} CombatContext;

static_assert(sizeof(CombatContext) == 0x68, "wrong size");

bool combat_init(GameInitInfo* init_info);
void combat_exit();
void combat_reset();
void sub_4B2210(int64_t attacker_obj, int64_t target_obj, CombatContext* combat);
int64_t sub_4B23B0(int64_t obj);
void sub_4B2650(int64_t a1, int64_t a2, CombatContext* combat);
void sub_4B3BB0(int64_t attacker_obj, int64_t target_obj, int a3);
bool combat_critter_is_combat_mode_active(int64_t obj);
bool sub_4B3D90(int64_t a1);
void combat_critter_deactivate_combat_mode(int64_t obj);
void combat_critter_activate_combat_mode(int64_t obj);
void sub_4B4320(int64_t obj);
tig_art_id_t sub_4B6B10(tig_art_id_t aid, int v2);
bool combat_set_callbacks(CombatCallbacks* callbacks);
bool combat_is_turn_based();
bool sub_4B6C90(bool turn_based);
void sub_4B6D20();
bool sub_4B6D70();
int64_t sub_4B6D80();
void sub_4B6E70(int64_t obj);
void sub_4B7010(int64_t obj);
bool combat_tb_timeevent_process(TimeEvent* timeevent);
void combat_turn_based_next_subturn();
int combat_get_action_points();
bool sub_4B7790(int64_t obj, int a2);
bool sub_4B7830(int64_t a1, int64_t a2);
bool sub_4B78D0(int64_t a1, int64_t a2);
bool sub_4B7AA0(int64_t obj);
bool sub_4B7AE0(int64_t obj);
int sub_4B7C20();
int sub_4B7C30(int64_t obj);
void sub_4B7C90(int64_t obj);
void sub_4B7CD0(int64_t obj, int action_points);
void combat_turn_based_add_critter(int64_t obj);
bool sub_4B8040(int64_t obj);
bool combat_set_blinded(int64_t obj);
bool combat_auto_attack_get(int64_t obj);
void combat_auto_attack_set(bool value);
bool combat_taunts_get();
void combat_taunts_set(bool value);
bool combat_auto_switch_weapons_get(int64_t obj);
void combat_auto_switch_weapons_set(bool value);

#endif /* ARCANUM_GAME_COMBAT_H_ */
