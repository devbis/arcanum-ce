#ifndef ARCANUM_GAME_CRITTER_H_
#define ARCANUM_GAME_CRITTER_H_

#include "game/context.h"
#include "game/timeevent.h"

typedef enum SocialClass {
    SOCIAL_CLASS_NOBLE,
    SOCIAL_CLASS_PRIEST,
    SOCIAL_CLASS_WIZARD,
    SOCIAL_CLASS_TECHNOLOGIST,
    SOCIAL_CLASS_SHOPKEEPER,
    SOCIAL_CLASS_GUARD,
    SOCIAL_CLASS_CITY_DWELLER,
    SOCIAL_CLASS_VILLAGER,
    SOCIAL_CLASS_BEGGAR,
    SOCIAL_CLASS_THIEF,
    SOCIAL_CLASS_BANDIT,
    MAX_SOCIAL_CLASS,
} SocialClass;

typedef enum EncumbranceLevel {
    ENCUMBRANCE_LEVEL_NONE,
    ENCUMBRANCE_LEVEL_LIGHT,
    ENCUMBRANCE_LEVEL_MODERATE,
    ENCUMBRANCE_LEVEL_MEDIUM,
    ENCUMBRANCE_LEVEL_SIGNIFICANT,
    ENCUMBRANCE_LEVEL_HEAVY,
    ENCUMBRANCE_LEVEL_SEVERE,
    MAX_ENCUMBRANCE_LEVEL,
} EncumbranceLevel;

bool critter_init(GameInitInfo* init_info);
void critter_exit();
int critter_social_class_get(long long obj);
int critter_social_class_set(long long obj, int value);
const char* critter_social_class_name(int social_class);
int critter_faction_get(long long obj);
int critter_faction_set(long long obj, int value);
bool critter_faction_same(long long a, long long b);
int critter_origin_get(long long obj);
int critter_origin_set(long long obj, int value);
bool critter_origin_same(long long a, long long b);
bool critter_is_pc(long long obj);
int critter_fatigue_pts_get(long long obj);
int critter_fatigue_pts_set(long long obj, int value);
int critter_fatigue_adj_get(long long obj);
int critter_fatigue_adj_set(long long obj, int value);
int critter_fatigue_damage_get(long long obj);
int critter_fatigue_damage_set(long long obj, int value);
int sub_45D670(long long obj);
int sub_45D700(long long obj);
bool sub_45D790(long long obj);
bool sub_45D800(int64_t obj);
bool critter_is_sleeping(int64_t obj);
bool sub_45D8D0(long long obj);
void sub_45D900(int64_t obj);
void sub_45DA20(int64_t a1, int64_t a2, int a3);
int64_t sub_45DDA0(int64_t obj);
int64_t critter_leader_get(int64_t obj);
void critter_leader_set(int64_t follower_obj, int64_t leader_obj);
bool critter_follow(int64_t follower_obj, int64_t leader_obj, bool force);
bool critter_disband(int64_t obj, bool force);
bool sub_45E180(int64_t obj);
void sub_45E1E0(int64_t obj);
bool sub_45E2E0(int64_t a1, int64_t a2);
int sub_45E3F0(int64_t obj, bool exclude_forced_followers);
int64_t sub_45E4F0(int64_t critter_obj);
int64_t sub_45E590(int64_t critter_obj);
bool critter_fatigue_timeevent_process(TimeEvent* timeevent);
bool sub_45E820(int64_t obj, int a2, int a3);
void sub_45E910(int64_t critter_obj, int hours);
bool critter_resting_timeevent_process(TimeEvent* timeevent);
bool sub_45EAB0(int64_t obj);
bool critter_decay_timeevent_process(TimeEvent* timeevent);
bool sub_45EBE0(int64_t obj);
void sub_45EC80(int64_t obj);
bool critter_npc_combat_focus_wipe_timeevent_process(TimeEvent* timeevent);
bool sub_45ED70(int64_t obj);
bool critter_is_concealed(int64_t obj);
void sub_45EE30(int64_t obj, bool a2);
tig_art_id_t sub_45EFA0(tig_art_id_t art_id);
bool sub_45EFF0(int64_t a, int64_t b);
bool sub_45F060(int64_t obj, int stat, int mod);
int sub_45F0B0(int64_t obj);
void sub_45F110(int64_t obj, int a2);
bool critter_enter_bed(int64_t obj, int64_t bed);
void critter_leave_bed(int64_t obj, int64_t bed);
bool critter_has_bad_associates(int64_t obj);
int64_t sub_45F650(int64_t obj);
int critter_teleport_map_get(long long obj);
void sub_45F710(long long obj);
bool sub_45F730(long long obj);
int sub_45F790(long long obj);
void sub_45F820(int64_t obj, int value);
void sub_45F910();
void sub_45F920();
const char* critter_encumbrance_level_name(int level);
int critter_encumbrance_level_ratio(int level);
int critter_description_get(long long a, long long b);
bool critter_can_backstab(int64_t obj, int64_t tgt);
tig_art_id_t sub_45FA70(long long obj, unsigned int* rgb);
bool sub_45FB90(int64_t obj);
bool sub_45FC00(int64_t obj);
void critter_debug_obj(object_id_t obj);

#endif /* ARCANUM_GAME_CRITTER_H_ */
