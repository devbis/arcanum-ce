#include "game/critter.h"

#include "game/background.h"
#include "game/effect.h"
#include "game/mes.h"
#include "game/stat.h"

static bool sub_45E8D0(TimeEvent* timeevent);
static bool sub_45EA80(TimeEvent* timeevent);
static bool sub_45ECB0(TimeEvent* timeevent);

// 0x5B304C
static int dword_5B304C = -1;

// 0x5B306C
static int dword_5B306C[MAX_ENCUMBRANCE_LEVEL] = {
    15,
    30,
    45,
    60,
    75,
    90,
    100,
};

// 0x5E8630
static mes_file_handle_t xp_mes_file;

// 0x5E8634
static char** social_class_names;

// 0x5E8638
static int dword_5E8638;

// 0x5E863C
static bool critter_editor;

// 0x5E8640
static mes_file_handle_t critter_mes_file;

// 0x5E8648
static int64_t qword_5E8648;

// 0x5E8650
static int64_t qword_5E8650;

// 0x45CF30
bool critter_init(GameInitInfo* init_info)
{
    int index;
    MesFileEntry mes_file_entry;

    critter_editor = init_info->editor;

    if (!mes_load("Rules\\xp_critter.mes", &xp_mes_file)) {
        return false;
    }

    if (!mes_load("mes\\critter.mes", &critter_mes_file)) {
        return false;
    }

    social_class_names = (char**)CALLOC(MAX_SOCIAL_CLASS, sizeof(char*));

    for (index = 0; index < MAX_SOCIAL_CLASS; index++) {
        mes_file_entry.num = index;
        sub_4D43A0(critter_mes_file, &mes_file_entry);
        social_class_names[index] = mes_file_entry.str;
    }

    dword_5E8638 = 1;

    return true;
}

// 0x45CFE0
void critter_exit()
{
    FREE(social_class_names);
    mes_unload(critter_mes_file);
    mes_unload(xp_mes_file);
}

// 0x45D010
int critter_social_class_get(long long obj)
{
    return obj_field_int32_get(obj, OBJ_F_NPC_SOCIAL_CLASS);
}

// 0x45D030
int critter_social_class_set(long long obj, int value)
{
    sub_4EA2E0(obj, 2);
    obj_field_int32_set(obj, OBJ_F_NPC_SOCIAL_CLASS, value);
    sub_4E9F70(obj, value + 75, 2);
    return value;
}

// 0x45D070
const char* critter_social_class_name(int social_class)
{
    return social_class_names[social_class];
}

// 0x45D080
int critter_faction_get(long long obj)
{
    if (obj_field_int32_get(obj, OBJ_F_TYPE) == OBJ_TYPE_NPC) {
        return obj_field_int32_get(obj, OBJ_F_NPC_FACTION);
    } else {
        return 0;
    }
}

// 0x45D0C0
int critter_faction_set(long long obj, int value)
{
    if (obj != 0 && obj_field_int32_get(obj, OBJ_F_TYPE) == OBJ_TYPE_NPC) {
        obj_field_int32_set(obj, OBJ_F_NPC_FACTION, value);
    }
    return value;
}

// 0x45D110
bool critter_faction_same(long long a, long long b)
{
    // TODO: Incomplete.
}

// 0x45D290
int critter_origin_get(long long obj)
{
    if (obj_field_int32_get(obj, OBJ_F_TYPE) == OBJ_TYPE_NPC) {
        return obj_field_int32_get(obj, OBJ_F_NPC_ORIGIN);
    } else {
        return 0;
    }
}

// 0x45D2D0
int critter_origin_set(long long obj, int value)
{
    if (obj != 0 && obj_field_int32_get(obj, OBJ_F_TYPE) == OBJ_TYPE_NPC) {
        obj_field_int32_set(obj, OBJ_F_NPC_ORIGIN, value);
    }
    return value;
}

// 0x45D320
bool critter_origin_same(long long a, long long b)
{
    return critter_origin_get(a) == critter_origin_get(b);
}

// 0x45D360
bool critter_is_pc(long long obj)
{
    return obj_field_int32_get(obj, OBJ_F_TYPE) == OBJ_TYPE_PC;
}

// 0x45D390
int critter_fatigue_pts_get(long long obj)
{
    if (obj_field_int32_get(obj, OBJ_F_TYPE) == OBJ_TYPE_PC
        || obj_field_int32_get(obj, OBJ_F_TYPE) == OBJ_TYPE_NPC) {
        return obj_field_int32_get(obj, OBJ_F_CRITTER_FATIGUE_PTS);
    } else {
        return 0;
    }
}

// 0x45D3E0
int critter_fatigue_pts_set(long long obj, int value)
{
    // TODO: Incomplete.
}

// 0x45D4A0
int critter_fatigue_adj_get(long long obj)
{
    if (obj_field_int32_get(obj, OBJ_F_TYPE) == OBJ_TYPE_PC
        || obj_field_int32_get(obj, OBJ_F_TYPE) == OBJ_TYPE_NPC) {
        return obj_field_int32_get(obj, OBJ_F_CRITTER_FATIGUE_ADJ);
    } else {
        return 0;
    }
}

// 0x45D4F0
int critter_fatigue_adj_set(long long obj, int value)
{
    if (obj_field_int32_get(obj, OBJ_F_TYPE) == OBJ_TYPE_PC
        || obj_field_int32_get(obj, OBJ_F_TYPE) == OBJ_TYPE_NPC) {
        obj_field_int32_set(obj, OBJ_F_CRITTER_FATIGUE_ADJ, value);
        sub_460260(obj);
    } else {
        return 0;
    }
}

// 0x45D550
int critter_fatigue_damage_get(long long obj)
{
    if (obj_field_int32_get(obj, OBJ_F_TYPE) == OBJ_TYPE_PC
        || obj_field_int32_get(obj, OBJ_F_TYPE) == OBJ_TYPE_NPC) {
        return obj_field_int32_get(obj, OBJ_F_CRITTER_FATIGUE_DAMAGE);
    } else {
        return 0;
    }
}

// 0x45D5A0
int critter_fatigue_damage_set(long long obj, int value)
{
    // TODO: Incomplete.
}

// 0x45D670
int sub_45D670(long long obj)
{
    int fatigue_pts;
    int fatigue_adj;
    int level;
    int cn;
    int wp;

    if (obj_field_int32_get(obj, OBJ_F_TYPE) == OBJ_TYPE_PC
        || obj_field_int32_get(obj, OBJ_F_TYPE) == OBJ_TYPE_NPC) {
        fatigue_pts = critter_fatigue_pts_get(obj);
        fatigue_adj = critter_fatigue_adj_get(obj);
        level = stat_level(obj, STAT_LEVEL);
        cn = stat_level(obj, STAT_CONSTITUTION);
        wp = stat_level(obj, STAT_WILLPOWER);
        return effect_adjust_max_fatigue(obj, fatigue_pts * 4
            + fatigue_adj
            + 2 * (level + cn)
            + wp
            + 4);
    } else {
        return 0;
    }
}

// 0x45D700
int sub_45D700(long long obj)
{
    return sub_45D670(obj) - critter_fatigue_damage_get(obj);
}

// 0x45D730
bool sub_45D730(long long obj)
{
    int type;
    tig_art_id_t aid;

    if ((obj_field_int32_get(obj, OBJ_F_FLAGS) & (OF_DESTROYED | OF_OFF)) != 0) {
        return false;
    }

    type = obj_field_int32_get(obj, OBJ_F_TYPE);
    if (type != OBJ_TYPE_PC && type != OBJ_TYPE_NPC) {
        return false;
    }

    // NOTE: Looks wrong, `sub_503E20` always returns 0 for pc/npc.
    aid = obj_field_int32_get(obj, OBJ_F_CURRENT_AID);
    if (sub_503E20(aid) != 7) {
        return false;
    }

    return true;
}

// 0x45D790
bool sub_45D790(long long obj)
{
    return (obj_field_int32_get(obj, OBJ_F_FLAGS) & (OF_DESTROYED | OF_OFF)) == 0
        && !sub_45D8D0(obj)
        && !sub_45D800(obj)
        && (obj_field_int32_get(obj, OBJ_F_CRITTER_FLAGS) & (OCF_STUNNED | OCF_PARALYZED)) == 0
        && (obj_field_int32_get(obj, OBJ_F_SPELL_FLAGS) & OSF_STONED) == 0;
}

// 0x45D800
bool sub_45D800(int64_t obj)
{
    return obj == OBJ_HANDLE_NULL
        || (obj_field_int32_get(obj, OBJ_F_TYPE) != OBJ_TYPE_PC
            && obj_field_int32_get(obj, OBJ_F_TYPE) != OBJ_TYPE_NPC)
        || ((obj_field_int32_get(obj, OBJ_F_CRITTER_FLAGS) & OCF_UNDEAD) == 0
            && sub_45D700(obj) <= 0);
}

// 0x45D870
bool critter_is_sleeping(int64_t obj)
{
    return obj != OBJ_HANDLE_NULL
        && obj_type_is_critter(obj_field_int32_get(obj, OBJ_F_TYPE))
        && (obj_field_int32_get(obj, OBJ_F_CRITTER_FLAGS) & OCF_SLEEPING) != 0;
}

// 0x45D8D0
bool sub_45D8D0(long long obj)
{
    if (obj != 0) {
        return sub_43D600(obj) <= 0;
    } else {
        return true;
    }
}

// 0x45D900
void sub_45D900()
{
    // TODO: Incomplete.
}

// 0x45DA20
void sub_45DA20()
{
    // TODO: Incomplete.
}

// 0x45DC90
void sub_45DC90()
{
    // TODO: Incomplete.
}

// 0x45DDA0
void sub_45DDA0()
{
    // TODO: Incomplete.
}

// 0x45DE00
long long critter_leader_get(long long obj)
{
    long long leader_obj;

    if (obj_field_int32_get(obj, OBJ_F_TYPE) == OBJ_TYPE_NPC) {
        obj_f_get_obj(obj, OBJ_F_NPC_LEADER, &leader_obj);
        return obj;
    } else {
        return 0;
    }
}

// 0x45DE50
void critter_leader_set(long long follower_obj, long long leader_obj)
{
    if (obj_field_int32_get(follower_obj, OBJ_F_TYPE) == OBJ_TYPE_NPC) {
        obj_f_set_obj(follower_obj, OBJ_F_NPC_LEADER, leader_obj);
    }
}

// 0x45DE90
void critter_follow()
{
    // TODO: Incomplete.
}

// 0x45DFC0
void critter_disband()
{
    // TODO: Incomplete.
}

// 0x45E040
void sub_45E040()
{
    // TODO: Incomplete.
}

// 0x45E180
void sub_45E180()
{
    // TODO: Incomplete.
}

// 0x45E1E0
void sub_45E1E0()
{
    // TODO: Incomplete.
}

// 0x45E2E0
void sub_45E2E0()
{
    // TODO: Incomplete.
}

// 0x45E3F0
void sub_45E3F0()
{
    // TODO: Incomplete.
}

// 0x45E460
void sub_45E460()
{
    // TODO: Incomplete.
}

// 0x45E4F0
void sub_45E4F0()
{
    // TODO: Incomplete.
}

// 0x45E590
void sub_45E590()
{
    // TODO: Incomplete.
}

// 0x45E610
void critter_fatigue_timeevent_process()
{
    // TODO: Incomplete.
}

// 0x45E820
bool sub_45E820(int64_t obj, int a2, int a3)
{
    TimeEvent timeevent;
    DateTime datetime;

    if ((tig_net_flags & TIG_NET_CONNECTED) != 0
        && (tig_net_flags & TIG_NET_HOST) == 0) {
        return true;
    }

    if (obj == OBJ_HANDLE_NULL) {
        return true;
    }

    if (sub_405BC0(obj)) {
        return true;
    }

    qword_5E8648 = obj;
    dword_5B304C = a2;

    if (sub_45C140(TIMEEVENT_TYPE_FATIGUE, sub_45E8D0)) {
        return true;
    }

    timeevent.type = TIMEEVENT_TYPE_FATIGUE;
    timeevent.params[0].integer_value = a2;
    timeevent.params[1].object_value = obj;
    timeevent.params[2].integer_value = sub_45A7F0();
    sub_45A950(&datetime, a3);
    return sub_45B800(&timeevent, &datetime);
}

// 0x45E8D0
bool sub_45E8D0(TimeEvent* timeevent)
{
    return timeevent->params[0].integer_value == dword_5B304C
        && timeevent->params[1].object_value == qword_5E8648;
}

// 0x45E910
void sub_45E910()
{
    // TODO: Incomplete.
}

// 0x45EA00
bool critter_resting_timeevent_process(TimeEvent* timeevent)
{
    int64_t obj;
    int hours;

    if (timeevent == NULL) {
        return true;
    }

    obj = timeevent->params[0].object_value;
    if (obj == OBJ_HANDLE_NULL) {
        return true;
    }

    hours = sub_45A820(timeevent->params[1].integer_value) / 3600;
    if (hours < 1) {
        hours = 1;
    }

    if (obj_field_int32_get(obj, OBJ_F_TYPE) == OBJ_TYPE_NPC
        && !sub_45DDA0(obj)) {
        sub_45E910(obj, hours);
    }

    return true;
}

// 0x45EA80
bool sub_45EA80(TimeEvent* timeevent)
{
    return timeevent != NULL && timeevent->params[0].object_value == qword_5E8648;
}

// 0x45EAB0
bool sub_45EAB0(int64_t obj)
{
    TimeEvent timeevent;
    DateTime datetime;

    if ((tig_net_flags & TIG_NET_CONNECTED) != 0
        && (tig_net_flags & TIG_NET_HOST) == 0) {
        return true;
    }

    if (obj == OBJ_HANDLE_NULL) {
        return true;
    }

    if (sub_405BC0(obj)) {
        return true;
    }

    qword_5E8648 = obj;

    if (sub_45C140(TIMEEVENT_TYPE_RESTING, sub_45EA80)) {
        return true;
    }

    timeevent.type = TIMEEVENT_TYPE_RESTING;
    timeevent.params[0].object_value = obj;
    timeevent.params[1].integer_value = sub_45A7F0();
    sub_45A950(&datetime, 3600000);
    return sub_45B800(&timeevent, &datetime);
}

// 0x45EB50
bool critter_decay_timeevent_process(TimeEvent* timeevent)
{
    int64_t obj = timeevent->params[0].object_value;

    if (obj == OBJ_HANDLE_NULL) {
        tig_debug_printf("Critter: Error: Attempt to decay a NULL object!\n");
        return true;
    }

    if (obj_type_is_critter(obj_field_int32_get(obj, OBJ_F_TYPE))
        && sub_45D8D0(obj)) {
        tig_debug_printf("Critter: critter_decay_timeevent_process: ERROR: Attempt to Decay a LIVE critter!\n");
        return false;
    }

    sub_463860(obj, 1);
    sub_43CCA0(obj);

    return true;
}

// 0x45EBE0
bool sub_45EBE0(int64_t obj)
{
    int type;
    TimeEvent timeevent;
    DateTime datetime;

    if (obj == OBJ_HANDLE_NULL
        || sub_405BC0(obj)) {
        return false;
    }

    type = obj_field_int32_get(obj, OBJ_F_TYPE);

    timeevent.type = TIMEEVENT_TYPE_DECAY_DEAD_BODIE;
    timeevent.params[0].object_value = obj;
    timeevent.params[1].integer_value = sub_45A7F0();
    if (obj_type_is_critter(type)) {
        sub_45A950(&datetime, 86400000);
    } else {
        sub_45A950(&datetime, 172800000);
    }
    return sub_45B800(&timeevent, &datetime);
}

// 0x45EC80
void sub_45EC80(int64_t obj)
{
    qword_5E8650 = obj;
    timeevent_clear_one_ex(TIMEEVENT_TYPE_DECAY_DEAD_BODIE, sub_45ECB0);
}

// 0x45ECB0
bool sub_45ECB0(TimeEvent* timeevent)
{
    return timeevent->params[0].object_value == qword_5E8650;
}

// 0x45ECE0
void critter_npc_combat_focus_wipe_timeevent_process(TimeEvent* timeevent)
{
    int64_t obj;
    int64_t focus_obj;

    obj = timeevent->params[0].object_value;
    if (obj == OBJ_HANDLE_NULL
        || !sub_45D8D0(obj)) {
        return true;
    }

    obj_field_obj_get(obj, OBJ_F_NPC_COMBAT_FOCUS, &focus_obj);
    if (focus_obj == OBJ_HANDLE_NULL) {
        return true;
    }

    if (sub_441AE0(obj, focus_obj) >= 30) {
        obj_field_handle_set(obj, OBJ_F_NPC_COMBAT_FOCUS, OBJ_HANDLE_NULL);
    } else {
        sub_45ED70(obj);
    }

    return true;
}

// 0x45ED70
bool sub_45ED70(int64_t obj)
{
    TimeEvent timeevent;
    DateTime datetime;

    if (obj == OBJ_HANDLE_NULL
        || !sub_405BC0(obj)) {
        return false;
    }

    timeevent.type = TIMEEVENT_TYPE_COMBAT_FOCUS_WIPE;
    timeevent.params[0].object_value = obj;
    timeevent.params[1].integer_value = sub_45A7F0();
    sub_45A950(&datetime, 600000);
    return sub_45B800(&timeevent, &datetime);
}

// 0x45EDE0
bool critter_is_concealed(int64_t obj)
{
    return obj != OBJ_HANDLE_NULL
        && obj_type_is_critter(obj_field_int32_get(obj, OBJ_F_TYPE))
        && (obj_field_int32_get(obj, OBJ_F_CRITTER_FLAGS) & OCF_IS_CONCEALED) != 0;
}

// 0x45EE30
void sub_45EE30()
{
    // TODO: Incomplete.
}

// 0x45EE90
void sub_45EE90()
{
    // TODO: Incomplete.
}

// 0x45EFA0
tig_art_id_t sub_45EFA0(tig_art_id_t art_id)
{
    tig_art_id_t new_art_id;
    TigArtAnimData art_anim_data;

    new_art_id = sub_503E50(art_id, 5);
    if (tig_art_exists(new_art_id) == TIG_OK
        && tig_art_data(new_art_id, &art_anim_data) == TIG_OK) {
        return tig_art_id_frame_set(new_art_id, art_anim_data.num_frames - 1);
    } else {
        return art_id;
    }
}

// 0x45EFF0
void sub_45EFF0()
{
    // TODO: Incomplete.
}

// 0x45F060
void sub_45F060()
{
    // TODO: Incomplete.
}

// 0x45F0B0
void sub_45F0B0()
{
    // TODO: Incomplete.
}

// 0x45F110
void sub_45F110()
{
    // TODO: Incomplete.
}

// 0x45F2D0
void sub_45F2D0()
{
    // TODO: Incomplete.
}

// 0x45F3A0
void sub_45F3A0()
{
    // TODO: Incomplete.
}

// 0x45F460
void critter_has_bad_associates()
{
    // TODO: Incomplete.
}

// 0x45F550
int sub_45F550(long long obj)
{
    return sub_4AEB10(obj);
}

// 0x45F570
bool sub_45F570(long long obj)
{
    tig_art_id_t art_id;
    int type;

    art_id = obj_field_int32_get(obj, OBJ_F_CURRENT_AID);
    type = tig_art_type(art_id);
    switch (type) {
    case TIG_ART_TYPE_CRITTER:
        return true;
    case TIG_ART_TYPE_MONSTER:
        return sub_503F20(art_id) == 2;
    default:
        return false;
    }
}

// 0x45F5C0
void sub_45F5C0(long long obj)
{
    int flags;

    flags = obj_field_int32_get(obj, OBJ_F_NPC_FLAGS);
    if ((flags & ONF_AI_SPREAD_OUT) != 0) {
        flags &= ~ONF_AI_SPREAD_OUT;
        obj_field_int32_set(obj, OBJ_F_NPC_FLAGS, flags);
    }
}

// 0x45F600
void sub_45F600(long long obj)
{
    int flags;

    flags = obj_field_int32_get(obj, OBJ_F_NPC_FLAGS);
    if ((flags & ONF_AI_SPREAD_OUT) == 0) {
        flags |= ONF_AI_SPREAD_OUT;
        obj_field_int32_set(obj, OBJ_F_NPC_FLAGS, flags);
        sub_436960(obj, critter_leader_get(obj));
    }
}

// 0x45F650
void sub_45F650()
{
    // TODO: Incomplete.
}

// 0x45F6D0
int critter_teleport_map_get(long long obj)
{
    int map;

    map = obj_field_int32_get(obj, OBJ_F_CRITTER_TELEPORT_MAP);
    if (map == 0) {
        map = sub_40FF40();
        obj_field_int32_set(obj, OBJ_F_CRITTER_TELEPORT_MAP, map);
    }

    return map;
}

// 0x45F710
void sub_45F710(long long obj)
{
    critter_teleport_map_get(obj);
}

// 0x45F730
bool sub_45F730(long long obj)
{
    if (obj == 0) {
        return false;
    }

    if (obj_field_int32_get(obj, OBJ_F_TYPE) != OBJ_TYPE_PC
        && obj_field_int32_get(obj, OBJ_F_TYPE) != OBJ_TYPE_NPC) {
        return false;
    }

    // TODO: Use constants.
    if ((obj_field_int32_get(obj, OBJ_F_CRITTER_FLAGS) & 0x2000800C) == 0) {
        return false;
    }

    return true;
}

// 0x45F790
int sub_45F790(long long obj)
{
    int current_weight;
    int max_weight;
    int encumbrance_level;

    if (obj_field_int32_get(obj, OBJ_F_TYPE) != OBJ_TYPE_PC
        && obj_field_int32_get(obj, OBJ_F_TYPE) != OBJ_TYPE_NPC) {
        return ENCUMBRANCE_LEVEL_SEVERE;
    }

    current_weight = item_total_weight(obj);
    max_weight = stat_level(obj, STAT_CARRY_WEIGHT);

    for (encumbrance_level = 0; encumbrance_level < MAX_ENCUMBRANCE_LEVEL; encumbrance_level++) {
        if (current_weight < max_weight * critter_encumbrance_level_ratio(encumbrance_level) / 100) {
            return encumbrance_level;
        }
    }

    return ENCUMBRANCE_LEVEL_SEVERE;
}

// 0x45F820
void sub_45F820()
{
    // TODO: Incomplete.
}

// 0x45F910
void sub_45F910()
{
    dword_5E8638++;
}

// 0x45F920
void sub_45F920()
{
    dword_5E8638--;
}

// 0x45F930
const char* critter_encumbrance_level_name(int level)
{
    MesFileEntry mes_file_entry;

    mes_file_entry.num = level + 18;
    sub_4D43A0(critter_mes_file, &mes_file_entry);

    return mes_file_entry.str;
}

// 0x45F960
int critter_encumbrance_level_ratio(int level)
{
    return dword_5B306C[level];
}

// 0x45F970
int critter_description_get(long long a, long long b)
{
    if (a == b || b != NULL && sub_4C0C40(a, b)) {
        return obj_field_int32_get(a, OBJ_F_DESCRIPTION);
    } else {
        return obj_field_int32_get(a, OBJ_F_CRITTER_DESCRIPTION_UNKNOWN);
    }
}

// 0x45F9D0
void sub_45F9D0()
{
    // TODO: Incomplete.
}

// 0x45FA70
tig_art_id_t sub_45FA70(long long obj, unsigned int* rgb)
{
    tig_art_id_t art_id;
    int flags;

    flags = obj_field_int32_get(obj, OBJ_F_CRITTER_FLAGS);
    if ((flags & OCF_LIGHT_XLARGE) != 0) {
        tig_art_light_id_create(1, 0, 0, 0, &art_id);
        light_build_color(244, 255, 255, rgb);
    } else if ((flags & OCF_LIGHT_LARGE) != 0) {
        tig_art_light_id_create(1, 0, 0, 0, &art_id);
        light_build_color(255, 214, 172, rgb);
    } else if ((flags & OCF_LIGHT_MEDIUM) != 0) {
        tig_art_light_id_create(1, 0, 0, 0, &art_id);
        light_build_color(236, 138, 85, rgb);
    } else if ((flags & OCF_LIGHT_SMALL) != 0) {
        tig_art_light_id_create(1, 0, 0, 0, &art_id);
        light_build_color(167, 88, 58, rgb);
    } else {
        art_id = TIG_ART_ID_INVALID;
        light_build_color(0, 0, 0, rgb);
    }

    return art_id;
}

// 0x45FB90
bool sub_45FB90(int64_t obj)
{
    if (obj == OBJ_HANDLE_NULL
        || !obj_type_is_critter(obj_field_int32_get(obj, OBJ_F_TYPE))) {
        return false;
    }

    if (background_obj_get_background(obj) == 78
        || (obj_field_int32_get(obj, OBJ_F_CRITTER_FLAGS2) & OCF2_DARK_SIGHT) != 0) {
        return true;
    }

    return false;
}

// 0x45FC00
bool sub_45FC00(int64_t obj)
{
    int background;

    if (obj == OBJ_HANDLE_NULL
        || !obj_type_is_critter(obj_field_int32_get(obj, OBJ_F_TYPE))) {
        return false;
    }

    if (stat_level(obj, STAT_INTELLIGENCE) <= 4) {
        return true;
    }

    background = background_obj_get_background(obj);
    if (background == 55
        || background == 74
        || background == 75) {
        return true;
    }

    return false;
}

// 0x45FC70
void critter_debug_obj(object_id_t obj)
{
    char name[1000];

    if (obj != OBJ_HANDLE_NULL) {
        if (obj_type_is_critter(obj_field_int32_get(obj, OBJ_F_TYPE))) {
            tig_debug_println("\n\n--------------------------------------");
            tig_debug_println("Critter Debug Obj:\n\n");
            sub_441B60(obj, obj, name);
            tig_debug_printf("Name: %s\n", name);
            tig_debug_println("Stats:  Level (Base)");

            for (int stat = 0; stat < STAT_COUNT; stat++) {
                int level = stat_level(obj, stat);
                int base = stat_get_base(obj, stat);
                tig_debug_printf("\t[%s]:\t%d (%d)\n",
                    stat_get_name(stat),
                    level,
                    base);
            }

            effect_debug_obj(obj);
        }
    }
}
