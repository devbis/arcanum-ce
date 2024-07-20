#include "game/obj.h"

#include "game/obj_private.h"

#define OBJ_FILE_VERSION 119

typedef struct ObjectFieldInfo {
    /* 0000 */ int field_0;
    /* 0004 */ int field_4;
    /* 0008 */ int field_8;
    /* 000C */ int field_C;
    /* 0010 */ int field_10;
    /* 0014 */ int field_14;
    /* 0018 */ int type;
} ObjectFieldInfo;

static_assert(sizeof(ObjectFieldInfo) == 0x1C, "wrong size");

static void object_field_not_exists(Object* object, int fld);
static Object* sub_408710(int64_t* obj_handle_ptr);
static void sub_409000(int64_t obj);
static void sub_409640(int64_t obj, int subtype);
static bool sub_4096B0(TigFile* stream, int64_t obj);
static bool sub_409980(TigFile* stream, int64_t obj);
static void sub_409CB0(void* mem, int64_t obj);
static void sub_409E80(void* mem, int64_t obj);
static bool sub_40C560();
static bool sub_40C6B0(Object* object, int fld);
static bool obj_enumerate_fields_in_range(Object* obj, int begin, int end, ObjEnumerateCallback* callback);
static int sub_40CB40(Object* object, int fld);
static bool sub_40D560(TigFile* stream);
static bool obj_check_version_stream(TigFile* stream);
static void sub_40D5D0(void* mem);
static bool obj_check_version_memory(void* mem);
static bool sub_40D670(Object* object, int a2, ObjectFieldInfo* field_info);

// 0x59BE58
static struct {
    int hp;
    int worth;
} obj_item_defaults[] = {
    { 8, 8 },
    { 5, 1 },
    { 8, 8 },
    { 10, 1 },
    { 2, 2 },
    { 1, 6 },
    { 3, 2 },
    { 3, 2 },
    { 1, 1 },
    { 3, 2 },
};

// 0x59BEA8
const char* object_field_names[] = {
    "obj_f_begin",
    "obj_f_current_aid",
    "obj_f_location",
    "obj_f_offset_x",
    "obj_f_offset_y",
    "obj_f_shadow",
    "obj_f_overlay_fore",
    "obj_f_overlay_back",
    "obj_f_underlay",
    "obj_f_blit_flags",
    "obj_f_blit_color",
    "obj_f_blit_alpha",
    "obj_f_blit_scale",
    "obj_f_light_flags",
    "obj_f_light_aid",
    "obj_f_light_color",
    "obj_f_overlay_light_flags",
    "obj_f_overlay_light_aid",
    "obj_f_overlay_light_color",
    "obj_f_flags",
    "obj_f_spell_flags",
    "obj_f_blocking_mask",
    "obj_f_name",
    "obj_f_description",
    "obj_f_aid",
    "obj_f_destroyed_aid",
    "obj_f_AC",
    "obj_f_hp_pts",
    "obj_f_hp_adj",
    "obj_f_hp_damage",
    "obj_f_material",
    "obj_f_resistance_idx",
    "obj_f_scripts_idx",
    "obj_f_sound_effect",
    "obj_f_category",
    "obj_f_pad_ias_1",
    "obj_f_pad_i64as_1",
    "obj_f_end",
    "obj_f_wall_begin",
    "obj_f_wall_flags",
    "obj_f_wall_pad_i_1",
    "obj_f_wall_pad_i_2",
    "obj_f_wall_pad_ias_1",
    "obj_f_wall_pad_i64as_1",
    "obj_f_wall_end",
    "obj_f_portal_begin",
    "obj_f_portal_flags",
    "obj_f_portal_lock_difficulty",
    "obj_f_portal_key_id",
    "obj_f_portal_notify_npc",
    "obj_f_portal_pad_i_1",
    "obj_f_portal_pad_i_2",
    "obj_f_portal_pad_ias_1",
    "obj_f_portal_pad_i64as_1",
    "obj_f_portal_end",
    "obj_f_container_begin",
    "obj_f_container_flags",
    "obj_f_container_lock_difficulty",
    "obj_f_container_key_id",
    "obj_f_container_inventory_num",
    "obj_f_container_inventory_list_idx",
    "obj_f_container_inventory_source",
    "obj_f_container_notify_npc",
    "obj_f_container_pad_i_1",
    "obj_f_container_pad_i_2",
    "obj_f_container_pad_ias_1",
    "obj_f_container_pad_i64as_1",
    "obj_f_container_end",
    "obj_f_scenery_begin",
    "obj_f_scenery_flags",
    "obj_f_scenery_whos_in_me",
    "obj_f_scenery_respawn_delay",
    "obj_f_scenery_pad_i_2",
    "obj_f_scenery_pad_ias_1",
    "obj_f_scenery_pad_i64as_1",
    "obj_f_scenery_end",
    "obj_f_projectile_begin",
    "obj_f_projectile_flags_combat",
    "obj_f_projectile_flags_combat_damage",
    "obj_f_projectile_hit_loc",
    "obj_f_projectile_parent_weapon",
    "obj_f_projectile_pad_i_1",
    "obj_f_projectile_pad_i_2",
    "obj_f_projectile_pad_ias_1",
    "obj_f_projectile_pad_i64as_1",
    "obj_f_projectile_end",
    "obj_f_item_begin",
    "obj_f_item_flags",
    "obj_f_item_parent",
    "obj_f_item_weight",
    "obj_f_item_magic_weight_adj",
    "obj_f_item_worth",
    "obj_f_item_mana_store",
    "obj_f_item_inv_aid",
    "obj_f_item_inv_location",
    "obj_f_item_use_aid_fragment",
    "obj_f_item_magic_tech_complexity",
    "obj_f_item_discipline",
    "obj_f_item_description_unknown",
    "obj_f_item_description_effects",
    "obj_f_item_spell_1",
    "obj_f_item_spell_2",
    "obj_f_item_spell_3",
    "obj_f_item_spell_4",
    "obj_f_item_spell_5",
    "obj_f_item_spell_mana_store",
    "obj_f_item_ai_action",
    "obj_f_item_pad_i_1",
    "obj_f_item_pad_ias_1",
    "obj_f_item_pad_i64as_1",
    "obj_f_item_end",
    "obj_f_weapon_begin",
    "obj_f_weapon_flags",
    "obj_f_weapon_paper_doll_aid",
    "obj_f_weapon_bonus_to_hit",
    "obj_f_weapon_magic_hit_adj",
    "obj_f_weapon_damage_lower_idx",
    "obj_f_weapon_damage_upper_idx",
    "obj_f_weapon_magic_damage_adj_idx",
    "obj_f_weapon_speed_factor",
    "obj_f_weapon_magic_speed_adj",
    "obj_f_weapon_range",
    "obj_f_weapon_magic_range_adj",
    "obj_f_weapon_min_strength",
    "obj_f_weapon_magic_min_strength_adj",
    "obj_f_weapon_ammo_type",
    "obj_f_weapon_ammo_consumption",
    "obj_f_weapon_missile_aid",
    "obj_f_weapon_visual_effect_aid",
    "obj_f_weapon_crit_hit_chart",
    "obj_f_weapon_magic_crit_hit_chance",
    "obj_f_weapon_magic_crit_hit_effect",
    "obj_f_weapon_crit_miss_chart",
    "obj_f_weapon_magic_crit_miss_chance",
    "obj_f_weapon_magic_crit_miss_effect",
    "obj_f_weapon_pad_i_1",
    "obj_f_weapon_pad_i_2",
    "obj_f_weapon_pad_ias_1",
    "obj_f_weapon_pad_i64as_1",
    "obj_f_weapon_end",
    "obj_f_ammo_begin",
    "obj_f_ammo_flags",
    "obj_f_ammo_quantity",
    "obj_f_ammo_type",
    "obj_f_ammo_pad_i_1",
    "obj_f_ammo_pad_i_2",
    "obj_f_ammo_pad_ias_1",
    "obj_f_ammo_pad_i64as_1",
    "obj_f_ammo_end",
    "obj_f_armor_begin",
    "obj_f_armor_flags",
    "obj_f_armor_paper_doll_aid",
    "obj_f_armor_ac_adj",
    "obj_f_armor_magic_ac_adj",
    "obj_f_armor_resistance_adj_idx",
    "obj_f_armor_magic_resistance_adj_idx",
    "obj_f_armor_silent_move_adj",
    "obj_f_armor_magic_silent_move_adj",
    "obj_f_armor_unarmed_bonus_damage",
    "obj_f_armor_pad_i_2",
    "obj_f_armor_pad_ias_1",
    "obj_f_armor_pad_i64as_1",
    "obj_f_armor_end",
    "obj_f_gold_begin",
    "obj_f_gold_flags",
    "obj_f_gold_quantity",
    "obj_f_gold_pad_i_1",
    "obj_f_gold_pad_i_2",
    "obj_f_gold_pad_ias_1",
    "obj_f_gold_pad_i64as_1",
    "obj_f_gold_end",
    "obj_f_food_begin",
    "obj_f_food_flags",
    "obj_f_food_pad_i_1",
    "obj_f_food_pad_i_2",
    "obj_f_food_pad_ias_1",
    "obj_f_food_pad_i64as_1",
    "obj_f_food_end",
    "obj_f_scroll_begin",
    "obj_f_scroll_flags",
    "obj_f_scroll_pad_i_1",
    "obj_f_scroll_pad_i_2",
    "obj_f_scroll_pad_ias_1",
    "obj_f_scroll_pad_i64as_1",
    "obj_f_scroll_end",
    "obj_f_key_begin",
    "obj_f_key_key_id",
    "obj_f_key_pad_i_1",
    "obj_f_key_pad_i_2",
    "obj_f_key_pad_ias_1",
    "obj_f_key_pad_i64as_1",
    "obj_f_key_end",
    "obj_f_key_ring_begin",
    "obj_f_key_ring_flags",
    "obj_f_key_ring_list_idx",
    "obj_f_key_ring_pad_i_1",
    "obj_f_key_ring_pad_i_2",
    "obj_f_key_ring_pad_ias_1",
    "obj_f_key_ring_pad_i64as_1",
    "obj_f_key_ring_end",
    "obj_f_written_begin",
    "obj_f_written_flags",
    "obj_f_written_subtype",
    "obj_f_written_text_start_line",
    "obj_f_written_text_end_line",
    "obj_f_written_pad_i_1",
    "obj_f_written_pad_i_2",
    "obj_f_written_pad_ias_1",
    "obj_f_written_pad_i64as_1",
    "obj_f_written_end",
    "obj_f_generic_begin",
    "obj_f_generic_flags",
    "obj_f_generic_usage_bonus",
    "obj_f_generic_usage_count_remaining",
    "obj_f_generic_pad_ias_1",
    "obj_f_generic_pad_i64as_1",
    "obj_f_generic_end",
    "obj_f_critter_begin",
    "obj_f_critter_flags",
    "obj_f_critter_flags2",
    "obj_f_critter_stat_base_idx",
    "obj_f_critter_basic_skill_idx",
    "obj_f_critter_tech_skill_idx",
    "obj_f_critter_spell_tech_idx",
    "obj_f_critter_fatigue_pts",
    "obj_f_critter_fatigue_adj",
    "obj_f_critter_fatigue_damage",
    "obj_f_critter_crit_hit_chart",
    "obj_f_critter_effects_idx",
    "obj_f_critter_effect_cause_idx",
    "obj_f_critter_fleeing_from",
    "obj_f_critter_portrait",
    "obj_f_critter_gold",
    "obj_f_critter_arrows",
    "obj_f_critter_bullets",
    "obj_f_critter_power_cells",
    "obj_f_critter_fuel",
    "obj_f_critter_inventory_num",
    "obj_f_critter_inventory_list_idx",
    "obj_f_critter_inventory_source",
    "obj_f_critter_description_unknown",
    "obj_f_critter_follower_idx",
    "obj_f_critter_teleport_dest",
    "obj_f_critter_teleport_map",
    "obj_f_critter_death_time",
    "obj_f_critter_auto_level_scheme",
    "obj_f_critter_pad_i_1",
    "obj_f_critter_pad_i_2",
    "obj_f_critter_pad_i_3",
    "obj_f_critter_pad_ias_1",
    "obj_f_critter_pad_i64as_1",
    "obj_f_critter_end",
    "obj_f_pc_begin",
    "obj_f_pc_flags",
    "obj_f_pc_flags_fate",
    "obj_f_pc_reputation_idx",
    "obj_f_pc_reputation_ts_idx",
    "obj_f_pc_background",
    "obj_f_pc_background_text",
    "obj_f_pc_quest_idx",
    "obj_f_pc_blessing_idx",
    "obj_f_pc_blessing_ts_idx",
    "obj_f_pc_curse_idx",
    "obj_f_pc_curse_ts_idx",
    "obj_f_pc_party_id",
    "obj_f_pc_rumor_idx",
    "obj_f_pc_pad_ias_2",
    "obj_f_pc_schematics_found_idx",
    "obj_f_pc_logbook_ego_idx",
    "obj_f_pc_fog_mask",
    "obj_f_pc_player_name",
    "obj_f_pc_bank_money",
    "obj_f_pc_global_flags",
    "obj_f_pc_global_variables",
    "obj_f_pc_pad_i_1",
    "obj_f_pc_pad_i_2",
    "obj_f_pc_pad_ias_1",
    "obj_f_pc_pad_i64as_1",
    "obj_f_pc_end",
    "obj_f_npc_begin",
    "obj_f_npc_flags",
    "obj_f_npc_leader",
    "obj_f_npc_ai_data",
    "obj_f_npc_combat_focus",
    "obj_f_npc_who_hit_me_last",
    "obj_f_npc_experience_worth",
    "obj_f_npc_experience_pool",
    "obj_f_npc_waypoints_idx",
    "obj_f_npc_waypoint_current",
    "obj_f_npc_standpoint_day",
    "obj_f_npc_standpoint_night",
    "obj_f_npc_origin",
    "obj_f_npc_faction",
    "obj_f_npc_retail_price_multiplier",
    "obj_f_npc_substitute_inventory",
    "obj_f_npc_reaction_base",
    "obj_f_npc_social_class",
    "obj_f_npc_reaction_pc_idx",
    "obj_f_npc_reaction_level_idx",
    "obj_f_npc_reaction_time_idx",
    "obj_f_npc_wait",
    "obj_f_npc_generator_data",
    "obj_f_npc_pad_i_1",
    "obj_f_npc_damage_idx",
    "obj_f_npc_shit_list_idx",
    "obj_f_npc_end",
    "obj_f_trap_begin",
    "obj_f_trap_flags",
    "obj_f_trap_difficulty",
    "obj_f_trap_pad_i_2",
    "obj_f_trap_pad_ias_1",
    "obj_f_trap_pad_i64as_1",
    "obj_f_trap_end",
    "obj_f_total_normal",
    "obj_f_transient_begin",
    "obj_f_render_color",
    "obj_f_render_colors",
    "obj_f_render_palette",
    "obj_f_render_scale",
    "obj_f_render_alpha",
    "obj_f_render_x",
    "obj_f_render_y",
    "obj_f_render_width",
    "obj_f_render_height",
    "obj_f_palette",
    "obj_f_color",
    "obj_f_colors",
    "obj_f_render_flags",
    "obj_f_temp_id",
    "obj_f_light_handle",
    "obj_f_overlay_light_handles",
    "obj_f_shadow_handles",
    "obj_f_internal_flags",
    "obj_f_find_node",
    "obj_f_transient_end",
    "obj_f_type",
    "obj_f_prototype_handle",
    "obj_f_max",
};

// 0x5D10F0
static int* dword_5D10F0;

// 0x5D10F4
static int dword_5D10F4;

// 0x5D10F8
static bool obj_is_editor;

// 0x5D10FC
static int16_t word_5D10FC;

// 0x5D1100
static int* dword_5D1100;

// 0x5D1104
static ObjectFieldInfo* object_fields;

// 0x5D110C
static TigFile* dword_5D110C;

// 0x5D1118
static void* dword_5D1118;

// 0x5D1120
static int16_t* dword_5D1120;

// 0x5D1124
static bool obj_initialized;

// 0x5D1128
static void* dword_5D1128;

// 0x5D112C
static void* dword_5D112C;

// 0x405110
bool obj_init(GameInitInfo* init_info)
{
    int index;
    Object object;

    object_fields = (ObjectFieldInfo*)CALLOC(OBJ_F_MAX, sizeof(ObjectFieldInfo));
    dword_5D10F0 = (int*)CALLOC(21, sizeof(int));
    dword_5D1100 = (int*)CALLOC(21, sizeof(int));
    dword_5D1120 = (int16_t*)CALLOC(18, sizeof(int16_t));
    obj_is_editor = init_info->editor;
    sub_4E59B0();
    sub_4E4CD0(sizeof(Object), obj_is_editor);
    sub_4E3F80();
    obj_find_init();
    sub_40A400();
    sub_40BAC0();

    object.field_20.field_0 = -1;
    for (index = 0; index < 18; index++) {
        object.type = index;
        word_5D10FC = 0;
        dword_5D10F4 = 0;
        obj_enumerate_fields(&object, sub_40C560);
        dword_5D1120[index] = word_5D10FC;
    }

    obj_initialized = true;

    return true;
}

// 0x4051F0
void obj_exit()
{
    sub_40BBB0();
    obj_find_exit();
    sub_4E4DB0();
    sub_4E3F90();
    sub_4E5A50();

    FREE(object_fields);
    FREE(dword_5D10F0);
    FREE(dword_5D1100);
    FREE(dword_5D1120);

    obj_initialized = false;
}

// 0x405250
void sub_405250()
{
    sub_4E4DB0();
    sub_4E3F90();
    sub_4E5A50();
    sub_4E59B0();
    sub_4E4CD0(sizeof(Object), obj_is_editor);
    sub_4E3F80();
}

// 0x405800
void sub_405800(int type, int64_t* obj_ptr)
{
    int64_t handle;
    Object* object;
    int index;

    object = sub_408710(&handle);
    object->type = type;
    sub_4E62A0(&(object->field_8));
    sub_4E4FD0(object->field_8, handle);
    object->field_20.field_0 = -1;
    object->prototype_handle = OBJ_HANDLE_NULL;
    object->field_40 = 0;
    sub_40C610(&object);

    for (index = 0; index < 19; index++) {
        object->transient_properties[index] = -1;
    }

    object->field_46 = dword_5D1120[type];
    object->field_50 = MALLOC(4 * object->field_46);

    dword_5D10F4 = 0;
    obj_enumerate_fields(object, sub_40C6B0);

    obj_unlock(handle);

    sub_409000(handle);
    sub_40C690(object);

    *obj_ptr = handle;
}

// 0x408710
Object* sub_408710(int64_t* obj_handle_ptr)
{
    return sub_4E4E60(obj_handle_ptr);
}

// 0x408720
Object* obj_lock(int64_t obj_handle)
{
    return sub_4E4F80(obj_handle);
}

// 0x408740
void obj_unlock(int64_t obj_handle)
{
    sub_4E4FA0(obj_handle);
}

// 0x408D60
void sub_408D60(Object* object, int fld, int* value_ptr)
{
    Unknown2 v1;
    int64_t proto_handle;
    Object* proto;

    v1.type = object_fields[fld].type;

    if (object->field_20.field_0 == -1) {
        v1.ptr = &(object->field_50[sub_40CB40(object, fld)]);
        *value_ptr = sub_4E4BA0(&v1);
    } else {
        if (fld > OBJ_F_TRANSIENT_BEGIN && fld < OBJ_F_TRANSIENT_END) {
            v1.ptr = &(object->transient_properties[fld - OBJ_F_TRANSIENT_BEGIN - 1]);
            *value_ptr = sub_4E4BA0(&v1);
        }

        if (sub_40D320(object, fld)) {
            v1.ptr = &(object->field_50[sub_40D230(object, fld)]);
            *value_ptr = sub_4E4BA0(&v1);
        } else {
            proto_handle = obj_get_prototype_handle(object);
            proto = obj_lock(proto_handle);
            v1.ptr = &(proto->field_50[sub_40CB40(proto, fld)]);
            *value_ptr = sub_4E4BA0(&v1);
            obj_unlock(proto_handle);
        }
    }
}

// 0x408E70
void sub_408E70(Object* object, int fld, int value)
{
    Unknown1 v1;

    if (object->field_20.field_0 == -1) {
        v1.ptr = &(object->field_50[sub_40CB40(object, fld)]);
        sub_40D400(object, fld, true);
    } else if (fld > OBJ_F_TRANSIENT_BEGIN && fld < OBJ_F_TRANSIENT_END) {
        v1.ptr = &(object->transient_properties[fld - OBJ_F_TRANSIENT_BEGIN - 1]);
    } else {
        if (!sub_40D350(object, fld)) {
            if (!sub_40D320(object, fld)) {
                sub_40D2A0(object, fld);
            }
        }

        v1.ptr = &(object->field_50[sub_40D230(object, fld)]);
        sub_40D400(object, fld, true);
        object->field_44 = 1;
    }

    v1.type = object_fields[fld].type;
    v1.value = value;
    sub_4E4B70(&v1);
}

// 0x408F40
bool sub_408F40(Object* object, int fld, int* a3, int64_t* proto_handle_ptr)
{
    Object* proto;

    if (object->field_20.field_0 == -1) {
        *a3 = &(object->field_50[sub_40CB40(object, fld)]);
        return false;
    }

    if (fld > OBJ_F_TRANSIENT_BEGIN && fld <= OBJ_F_TRANSIENT_END) {
        *a3 = &(object->transient_properties[fld - OBJ_F_TRANSIENT_BEGIN - 1]);
        return false;
    }

    if (sub_40D320(object, fld)) {
        *a3 = &(object->field_50[sub_40D230(object, fld)]);
        return false;
    }

    *proto_handle_ptr = obj_get_prototype_handle(object);

    proto = obj_lock(*proto_handle_ptr);
    *a3 = &(proto->field_50[sub_40CB40(proto, fld)]);

    return true;
}

// 0x409000
void sub_409000(int64_t obj)
{
    int index;
    int type;
    unsigned int flags;
    tig_art_id_t art_id = TIG_ART_ID_INVALID;

    obj_f_set_int32(obj, OBJ_F_SHADOW, -1);
    obj_f_set_int32(obj, OBJ_F_AID, TIG_ART_ID_INVALID);
    obj_f_set_int32(obj, OBJ_F_DESTROYED_AID, TIG_ART_ID_INVALID);
    obj_f_set_int32(obj, OBJ_F_CURRENT_AID, TIG_ART_ID_INVALID);
    obj_f_set_int32(obj, OBJ_F_BLIT_COLOR, tig_color_make(255, 255, 255));
    obj_f_set_int32(obj, OBJ_F_BLIT_ALPHA, 255);
    obj_f_set_int32(obj, OBJ_F_BLIT_SCALE, 100);
    obj_f_set_int32(obj, OBJ_F_LIGHT_AID, TIG_ART_ID_INVALID);
    obj_f_set_int32(obj, OBJ_F_LIGHT_COLOR, tig_color_make(255, 255, 255));

    for (index = 0; index < 7; index++) {
        sub_4074E0(obj, OBJ_F_OVERLAY_FORE, index, TIG_ART_ID_INVALID);
        sub_4074E0(obj, OBJ_F_OVERLAY_BACK, index, TIG_ART_ID_INVALID);
    }

    for (index = 0; index < 4; index++) {
        sub_4074E0(obj, OBJ_F_OVERLAY_LIGHT_AID, index, TIG_ART_ID_INVALID);
    }

    for (index = 0; index < 4; index++) {
        sub_4074E0(obj, OBJ_F_UNDERLAY, index, TIG_ART_ID_INVALID);
    }

    type = obj_f_get_int32(obj, OBJ_F_TYPE);
    switch (type) {
    case OBJ_TYPE_WEAPON:
    case OBJ_TYPE_AMMO:
    case OBJ_TYPE_ITEM_ARMOR:
    case OBJ_TYPE_ITEM_GOLD:
    case OBJ_TYPE_ITEM_FOOD:
    case OBJ_TYPE_ITEM_SCROLL:
    case OBJ_TYPE_ITEM_KEY:
    case OBJ_TYPE_ITEM_KEY_RING:
    case OBJ_TYPE_ITEM_WRITTEN:
    case OBJ_TYPE_ITEM_GENERIC:
        if (type == OBJ_TYPE_ITEM_KEY) {
            obj_f_set_int32(obj, OBJ_F_ITEM_WEIGHT, 0);
        } else if (type == OBJ_TYPE_ITEM_GOLD) {
            obj_f_set_int32(obj, OBJ_F_ITEM_WEIGHT, 1);
        } else {
            obj_f_set_int32(obj, OBJ_F_ITEM_WEIGHT, 10);
        }

        obj_f_set_int32(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, TIG_ART_ID_INVALID);
        obj_f_set_int32(obj, OBJ_F_ITEM_SPELL_1, 10000);
        obj_f_set_int32(obj, OBJ_F_ITEM_SPELL_2, 10000);
        obj_f_set_int32(obj, OBJ_F_ITEM_SPELL_3, 10000);
        obj_f_set_int32(obj, OBJ_F_ITEM_SPELL_4, 10000);
        obj_f_set_int32(obj, OBJ_F_ITEM_SPELL_5, 10000);

        flags = obj_f_get_int32(obj, OBJ_F_FLAGS);
        flags |= 0x434;
        obj_f_set_int32(obj, OBJ_F_FLAGS, flags);
        break;
    case OBJ_TYPE_PC:
    case OBJ_TYPE_NPC:
        stat_set_defaults(obj);
        skill_set_defaults(obj);
        spell_set_defaults(obj);
        tech_set_defaults(obj);

        flags = obj_f_get_int32(obj, OBJ_F_FLAGS);
        flags |= 0x4030;
        obj_f_set_int32(obj, OBJ_F_FLAGS, flags);
        break;
    }

    switch (type) {
    case OBJ_TYPE_WALL:
        obj_f_set_int32(obj, OBJ_F_HP_PTS, 500);

        flags = obj_f_get_int32(obj, OBJ_F_FLAGS);
        flags |= 0x4000;
        obj_f_set_int32(obj, OBJ_F_FLAGS, flags);

        tig_art_wall_id_create(0, 0, 0, 6, 0, 0, &art_id);
        break;
    case OBJ_TYPE_PORTAL:
        obj_f_set_int32(obj, OBJ_F_HP_PTS, 100);

        flags = obj_f_get_int32(obj, OBJ_F_FLAGS);
        flags |= 0x4000;
        obj_f_set_int32(obj, OBJ_F_FLAGS, flags);

        tig_art_portal_id_create(0, 1, 0, 0, 6, 0, &art_id);
        break;
    case OBJ_TYPE_CONTAINER:
        obj_f_set_int32(obj, OBJ_F_HP_PTS, 100);

        flags = obj_f_get_int32(obj, OBJ_F_FLAGS);
        flags |= 0x4030;
        obj_f_set_int32(obj, OBJ_F_FLAGS, flags);

        tig_art_container_id_create(0, 1, 0, 0, 0, &art_id);
        break;
    case OBJ_TYPE_SCENERY:
        obj_f_set_int32(obj, OBJ_F_HP_PTS, 100);

        flags = obj_f_get_int32(obj, OBJ_F_FLAGS);
        flags |= 0x4830;
        obj_f_set_int32(obj, OBJ_F_FLAGS, flags);

        tig_art_scenery_id_create(0, 0, 0, 0, 0, &art_id);
        break;
    case OBJ_TYPE_PROJECTILE:
        flags = obj_f_get_int32(obj, OBJ_F_FLAGS);
        flags |= 0x430;
        obj_f_set_int32(obj, OBJ_F_FLAGS, flags);

        tig_art_scenery_id_create(0, 0, 0, 0, 0, &art_id);
        break;
    case OBJ_TYPE_WEAPON:
        sub_409640(obj, TIG_ART_ITEM_TYPE_WEAPON);
        obj_f_set_int32(obj, OBJ_F_WEAPON_AMMO_TYPE, 10000);
        obj_f_set_int32(obj, OBJ_F_WEAPON_SPEED_FACTOR, 10);
        sub_407340(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 0, 1);
        sub_407340(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 0, 4);
        tig_art_item_id_create(0, 1, 0, 0, 3, 0, 0, 0, &art_id);
        obj_f_set_int32(obj, OBJ_F_ITEM_INV_AID, art_id);
        tig_art_item_id_create(0, 2, 0, 0, 3, 0, 0, 0, &art_id);
        obj_f_set_int32(obj, OBJ_F_WEAPON_PAPER_DOLL_AID, art_id);
        tig_art_item_id_create(0, 0, 0, 0, 3, 0, 0, 0, &art_id);
        obj_f_set_int32(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
        obj_f_set_int32(obj, OBJ_F_WEAPON_MISSILE_AID, TIG_ART_ID_INVALID);
        obj_f_set_int32(obj, OBJ_F_WEAPON_VISUAL_EFFECT_AID, TIG_ART_ID_INVALID);
        break;
    case OBJ_TYPE_AMMO:
        sub_409640(obj, TIG_ART_ITEM_TYPE_AMMO);
        tig_art_item_id_create(0, 0, 0, 0, 0, 1, 0, 0, &art_id);
        break;
    case OBJ_TYPE_ITEM_ARMOR:
        sub_409640(obj, TIG_ART_ITEM_TYPE_ARMOR);
        tig_art_item_id_create(0, 1, 0, 0, 4, 2, 0, 0, &art_id);
        obj_f_set_int32(obj, OBJ_F_ITEM_INV_AID, art_id);
        tig_art_item_id_create(0, 2, 0, 0, 4, 2, 0, 0, &art_id);
        obj_f_set_int32(obj, OBJ_F_ARMOR_PAPER_DOLL_AID, art_id);
        tig_art_item_id_create(0, 0, 0, 0, 4, 2, 0, 0, &art_id);
        obj_f_set_int32(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
        obj_f_set_int32(obj, OBJ_F_ARMOR_FLAGS, OARF_SIZE_MEDIUM);
        break;
    case OBJ_TYPE_ITEM_GOLD:
        sub_409640(obj, TIG_ART_ITEM_TYPE_GOLD);
        obj_f_set_int32(obj, OBJ_F_GOLD_QUANTITY, 1);
        tig_art_item_id_create(0, 0, 0, 0, 0, 3, 0, 0, &art_id);
        break;
    case OBJ_TYPE_ITEM_FOOD:
        sub_409640(obj, TIG_ART_ITEM_TYPE_FOOD);
        tig_art_item_id_create(0, 0, 0, 0, 0, 4, 0, 0, &art_id);
        break;
    case OBJ_TYPE_ITEM_SCROLL:
        sub_409640(obj, TIG_ART_ITEM_TYPE_SCROLL);
        tig_art_item_id_create(0, 0, 0, 0, 0, 5, 0, 0, &art_id);
        break;
    case OBJ_TYPE_ITEM_KEY:
        sub_409640(obj, TIG_ART_ITEM_TYPE_KEY);
        tig_art_item_id_create(0, 0, 0, 0, 0, 6, 0, 0, &art_id);
        break;
    case OBJ_TYPE_ITEM_KEY_RING:
        sub_409640(obj, TIG_ART_ITEM_TYPE_KEY_RING);
        tig_art_item_id_create(0, 0, 0, 0, 0, 7, 0, 0, &art_id);
        break;
    case OBJ_TYPE_ITEM_WRITTEN:
        sub_409640(obj, TIG_ART_ITEM_TYPE_WRITTEN);
        tig_art_item_id_create(0, 0, 0, 0, 0, 8, 0, 0, &art_id);
        break;
    case OBJ_TYPE_ITEM_GENERIC:
        sub_409640(obj, TIG_ART_ITEM_TYPE_GENERIC);
        tig_art_item_id_create(0, 0, 0, 0, 0, 9, 0, 0, &art_id);
        break;
    case OBJ_TYPE_PC:
    case OBJ_TYPE_NPC:
        tig_art_critter_id_create(1, 0, 0, 0, 0, 4, 0, 0, 0, &art_id);
        break;
    case OBJ_TYPE_TRAP:
        obj_f_set_int32(obj, OBJ_F_HP_PTS, 100);

        flags = obj_f_get_int32(obj, OBJ_F_FLAGS);
        flags |= 0x100434;
        obj_f_set_int32(obj, OBJ_F_FLAGS, flags);

        tig_art_scenery_id_create(0, 0, 0, 0, 0, &art_id);
        break;
    }

    obj_f_set_int32(obj, OBJ_F_AID, art_id);
    obj_f_set_int32(obj, OBJ_F_CURRENT_AID, art_id);
}

// 0x409640
void sub_409640(int64_t obj, int subtype)
{
    tig_art_id_t art_id;

    if (tig_art_item_id_create(0, 1, 0, 0, 0, subtype, 0, 0, &art_id) != TIG_OK) {
        art_id = TIG_ART_ID_INVALID;
    }

    obj_f_set_int32(obj, OBJ_F_ITEM_INV_AID, art_id);
    obj_f_set_int32(obj, OBJ_F_ITEM_WORTH, obj_item_defaults[subtype].worth);
    obj_f_set_int32(obj, OBJ_F_HP_PTS, obj_item_defaults[subtype].hp);
}

// 0x4096B0
bool sub_4096B0(TigFile* stream, int64_t obj)
{
    Object* object;
    int cnt;

    object = obj_lock(obj);

    if (!obj_write_raw(&(object->field_20), sizeof(object->field_20), stream)) {
        obj_unlock(obj);
        return false;
    }

    if (!obj_write_raw(&(object->field_8), sizeof(object->field_8), stream)) {
        obj_unlock(obj);
        return false;
    }

    if (!obj_write_raw(&(object->type), sizeof(object->type), stream)) {
        obj_unlock(obj);
        return false;
    }

    cnt = sub_40C030(object->type);
    if (!obj_write_raw(object->field_4C, sizeof(object->field_4C[0]) * cnt, stream)) {
        obj_unlock(obj);
        return false;
    }

    dword_5D10F4 = 0;
    dword_5D110C = stream;
    if (!obj_enumerate_fields(object, sub_40A070)) {
        obj_unlock(obj);
        return false;
    }

    obj_unlock(obj);
    return true;
}

// 0x409980
bool sub_409980(TigFile* stream, int64_t obj)
{
    Object* object;
    int cnt;

    object = obj_lock(obj);

    if (!obj_write_raw(&(object->field_20), sizeof(object->field_20), stream)) {
        obj_unlock(obj);
        return false;
    }

    if (!obj_write_raw(&(object->field_8), sizeof(object->field_8), stream)) {
        obj_unlock(obj);
        return false;
    }

    if (!obj_write_raw(&(object->type), sizeof(object->type), stream)) {
        obj_unlock(obj);
        return false;
    }

    if (!obj_write_raw(&(object->field_46), sizeof(object->field_46), stream)) {
        obj_unlock(obj);
        return false;
    }

    cnt = sub_40C030(object->type);
    if (!obj_write_raw(object->field_48, sizeof(object->field_48[0]) * cnt, stream)) {
        obj_unlock(obj);
        return false;
    }

    dword_5D110C = stream;
    if (!sub_40CBA0(object, object_field_write)) {
        obj_unlock(obj);
        return false;
    }

    obj_unlock(obj);
    return true;
}

// 0x409CB0
void sub_409CB0(void* mem, int64_t obj)
{
    Object* object;
    int cnt;

    object = obj_lock(obj);
    sub_4E4C00(&(object->field_20), sizeof(object->field_20), mem);
    sub_4E4C00(&(object->field_8), sizeof(object->field_8), mem);
    sub_4E4C00(&(object->type), sizeof(object->type), mem);
    cnt = sub_40C030(object->type);
    sub_4E4C00(object->field_4C, sizeof(object->field_4C[0]) * cnt, mem);
    dword_5D10F4 = 0;
    dword_5D1118 = mem;
    obj_enumerate_fields(object, sub_40A0E0);
    obj_unlock(obj);
}

// 0x409E80
void sub_409E80(void* mem, int64_t obj)
{
    Object* object;
    int cnt;

    object = obj_lock(obj);
    sub_4E4C00(&(object->field_20), sizeof(object->field_20), mem);
    sub_4E4C00(&(object->field_8), sizeof(object->field_8), mem);
    sub_4E4C00(&(object->type), sizeof(object->type), mem);
    sub_4E4C00(&(object->field_46), sizeof(object->field_46), mem);
    cnt = sub_40C030(object->type);
    sub_4E4C00(object->field_48, sizeof(object->field_48[0]) * cnt, mem);
    dword_5D1118 = mem;
    sub_40CBA0(object, sub_40A250);
    obj_unlock(obj);
}

// 0x405BC0
bool sub_405BC0(int64_t obj)
{
    Object* object;
    bool ret;

    object = obj_lock(obj);
    ret = object->field_20.field_0 == -1;
    obj_unlock(obj);

    return ret;
}

// 0x405BF0
void sub_405BF0(int64_t obj)
{
    Object* object;
    int fld;

    object = obj_lock(obj);
    if (object->field_20.field_0 != -1) {
        obj_find_remove(obj);
    }

    if (object->field_8.field_0 != 0) {
        sub_4E52F0(object->field_8);
    }

    if (object->field_20.field_0 != -1) {
        sub_40CBA0(object, sub_40CB60);
        sub_40C5B0(object);

        for (fld = OBJ_F_TRANSIENT_BEGIN + 1; fld < OBJ_F_TRANSIENT_END; fld++) {
            sub_40C840(object, fld);
        }
    } else {
        dword_5D10F4 = 0;
        obj_enumerate_fields(object, sub_40C6E0);
        sub_40C640(object);
    }

    if (object->field_50 != NULL) {
        FREE(object->field_50);
    }

    obj_unlock(obj);
    sub_4E4FB0(obj);
}

// 0x405CC0
void sub_405CC0(int64_t obj)
{
    Object* object;
    int fld;

    object = obj_lock(obj);
    if (object->field_20.field_0 != -1) {
        sub_40CBA0(object, sub_40CB60);
        sub_40C5B0(object);

        for (fld = OBJ_F_TRANSIENT_BEGIN + 1; fld < OBJ_F_TRANSIENT_END; fld++) {
            sub_40C840(object, fld);
        }
    } else {
        dword_5D10F4 = 0;
        obj_enumerate_fields(object, sub_40C6E0);
        sub_40C640(object);
    }

    if (object->field_50 != NULL) {
        FREE(object->field_50);
    }

    obj_unlock(obj);
    sub_4E4FB0(obj);
}

// 0x405D60
void sub_405D60(int64_t* new_obj_handle_ptr, int64_t obj_handle)
{
    int64_t new_obj_handle;
    Object* new_object;
    Object* object;
    int fld;

    object = obj_lock(obj_handle);
    new_object = sub_408710(&new_obj_handle);

    new_object->type = object->type;
    new_object->field_8 = object->field_8;
    new_object->field_20 = object->field_20;
    new_object->field_44 = object->field_44;
    new_object->field_46 = object->field_46;
    new_object->field_50 = (int*)CALLOC(4 * object->field_46, 1);

    if (object->field_20.field_0 != -1) {
        sub_40C5C0(new_object, object);

        dword_5D1108 = object;
        sub_40CBA0(new_object, sub_40C7A0);

        memset(new_object->transient_properties, 0, sizeof(new_object->transient_properties));
        for (fld = OBJ_F_TRANSIENT_BEGIN + 1; fld < OBJ_F_TRANSIENT_END; fld++) {
            sub_40C7F0(new_object, object, fld);
        }
    } else {
        sub_40C650(new_object, object);

        dword_5D10F4 = 0;
        dword_5D1110 = object;
        obj_enumerate_fields(new_object, sub_40C730);
    }

    obj_unlock(obj_handle);
    obj_unlock(new_obj_handle);
    sub_464470(new_obj_handle, NULL, NULL);

    *new_obj_handle_ptr = new_obj_handle;
}

// 0x4064B0
void sub_4064B0(int64_t obj_handle)
{
    Object* object;
    unsigned int flags;

    object = obj_lock(obj_handle);
    sub_408A20(object, OBJ_F_INTERNAL_FLAGS, &flags);
    if ((flags & 0x1) == 0) {
        sub_40BBF0(object);
        sub_40BD20(object);
        sub_408760(object, OBJ_F_INTERNAL_FLAGS, flags | 0x1);
        // NOTE: Probably should be outside of this condition block, otherwise
        // object might remain locked.
        obj_unlock(obj_handle);
    }
}

// 0x406520
void sub_406520(int64_t obj_handle)
{
    Object* object;
    unsigned int flags;

    object = obj_lock(obj_handle);
    sub_408A20(object, OBJ_F_INTERNAL_FLAGS, &flags);
    if ((flags & 0x1) != 0) {
        sub_40BDB0(object);
        sub_40BE70(object);
        sub_408760(object, OBJ_F_INTERNAL_FLAGS, flags & ~0x1);
        // NOTE: Probably should be outside of this condition block, otherwise
        // object might remain locked.
        obj_unlock(obj_handle);
    }
}

// 0x406590
bool obj_write(TigFile* stream, int64_t obj_handle)
{
    Object* object;
    bool v1;

    object = obj_lock(obj_handle);
    if (!sub_40D560(stream)) {
        // FIXME: Object not unlocked.
        return false;
    }

    v1 = object->field_20.field_0 == -1;
    obj_unlock(obj_handle);

    if (v1) {
        return sub_4096B0(stream, obj_handle);
    } else {
        return sub_409980(stream, obj_handle);
    }
}

// 0x406600
bool obj_read(TigFile* stream, int64_t obj_handle_ptr)
{
    ObjectID object_id;

    if (!obj_check_version_stream(stream)) {
        return false;
    }

    if (!obj_read_raw(&object_id, sizeof(ObjectID), stream)) {
        return false;
    }

    if (object_id.field_0 == -1) {
        return sub_4097B0(stream, obj_handle_ptr, object_id);
    } else {
        return sub_409AA0(stream, obj_handle_ptr);
    }
}

// 0x4067F0
bool obj_dif_write(TigFile* stream, int64_t obj_handle)
{
    Object* object;
    int marker;
    int index;
    int cnt;
    bool written;

    object = obj_lock(obj_handle);
    if (object->field_44 == 0) {
        // FIXME: Object not unlocked.
        return false;
    }

    // FIXME: Unused.
    obj_f_get_int32(obj_handle, OBJ_F_FLAGS);

    if (!sub_40D560(stream)) {
        obj_unlock(obj_handle);
        return false;
    }

    marker = 0x12344321;
    if (!obj_write_raw(&marker, sizeof(marker), stream)) {
        obj_unlock(obj_handle);
        return false;
    }

    if (!obj_write_raw(&(object->field_8), sizeof(object->field_8), stream)) {
        obj_unlock(obj_handle);
        return false;
    }

    cnt = sub_40C030(object->type);
    for (index = 0; index < cnt; index++) {
        if (!obj_write_raw(&(object->field_4C[index]), sizeof(object->field_4C[0]), stream)) {
            obj_unlock(obj_handle);
            return false;
        }
    }

    dword_5D110C = stream;
    written = sub_40CBA0(object, object_field_write_if_dif);
    obj_unlock(obj_handle);

    marker = 0x23455432;
    if (!obj_write_raw(&marker, sizeof(marker), stream)) {
        return false;
    }

    return written;
}

// 0x406B80
void sub_406B80(int64_t obj_handle)
{
    Object* object;
    int cnt;
    int index;

    object = obj_lock(obj_handle);
    if (object->field_44 != 0) {
        cnt = sub_40C030(object->type);
        for (index = 0; index < cnt; index++) {
            object->field_4C[index] = 0;
        }
        object->field_44 = 0;
    }
}

// 0x406CA0
int object_field_get(object_id_t object_id, int field)
{
    // TODO: Incomplete.

}

// 0x406D10
void object_field_not_exists(Object* object, int fld)
{
  tig_debug_printf("Error: Accessing non-existant field [%s : %d] in object type [%d].\n",
           object_field_names[fld],
           fld,
           object->type);
}

// 0x406D40
void object_field_set(object_id_t object_id, int field, int value)
{
    // TODO: Incomplete.
}

// 0x406DA0
long long object_field_get_64(object_id_t object_id, int field)
{
    // TODO: Incomplete.
}

// 0x40C030
int sub_40C030(ObjectType object_type)
{
    int v1;

    switch (object_type) {
    case OBJ_TYPE_WALL:
        v1 = object_fields[OBJ_F_WALL_PAD_I64AS_1].field_8;
        break;
    case OBJ_TYPE_PORTAL:
        v1 = object_fields[OBJ_F_PORTAL_PAD_I64AS_1].field_8;
        break;
    case OBJ_TYPE_CONTAINER:
        v1 = object_fields[OBJ_F_CONTAINER_PAD_I64AS_1].field_8;
        break;
    case OBJ_TYPE_SCENERY:
        v1 = object_fields[OBJ_F_SCENERY_PAD_I64AS_1].field_8;
        break;
    case OBJ_TYPE_PROJECTILE:
        v1 = object_fields[OBJ_F_PROJECTILE_PAD_I64AS_1].field_8;
        break;
    case OBJ_TYPE_WEAPON:
        v1 = object_fields[OBJ_F_WEAPON_PAD_I64AS_1].field_8;
        if (v1 == -1) {
            v1 = object_fields[OBJ_F_ITEM_PAD_I64AS_1].field_8;
        }
        break;
    case OBJ_TYPE_AMMO:
        v1 = object_fields[OBJ_F_AMMO_PAD_I64AS_1].field_8;
        if (v1 == -1) {
            v1 = object_fields[OBJ_F_ITEM_PAD_I64AS_1].field_8;
        }
        break;
    case OBJ_TYPE_ITEM_ARMOR:
        v1 = object_fields[OBJ_F_ARMOR_PAD_I64AS_1].field_8;
        if (v1 == -1) {
            v1 = object_fields[OBJ_F_ITEM_PAD_I64AS_1].field_8;
        }
        break;
    case OBJ_TYPE_ITEM_GOLD:
        v1 = object_fields[OBJ_F_GOLD_PAD_I64AS_1].field_8;
        if (v1 == -1) {
            v1 = object_fields[OBJ_F_ITEM_PAD_I64AS_1].field_8;
        }
        break;
    case OBJ_TYPE_ITEM_FOOD:
        v1 = object_fields[OBJ_F_FOOD_PAD_I64AS_1].field_8;
        if (v1 == -1) {
            v1 = object_fields[OBJ_F_ITEM_PAD_I64AS_1].field_8;
        }
        break;
    case OBJ_TYPE_ITEM_SCROLL:
        v1 = object_fields[OBJ_F_SCROLL_PAD_I64AS_1].field_8;
        if (v1 == -1) {
            v1 = object_fields[OBJ_F_ITEM_PAD_I64AS_1].field_8;
        }
        break;
    case OBJ_TYPE_ITEM_KEY:
        v1 = object_fields[OBJ_F_KEY_PAD_I64AS_1].field_8;
        if (v1 == -1) {
            v1 = object_fields[OBJ_F_ITEM_PAD_I64AS_1].field_8;
        }
        break;
    case OBJ_TYPE_ITEM_KEY_RING:
        v1 = object_fields[OBJ_F_KEY_RING_PAD_I64AS_1].field_8;
        if (v1 == -1) {
            v1 = object_fields[OBJ_F_ITEM_PAD_I64AS_1].field_8;
        }
        break;
    case OBJ_TYPE_ITEM_WRITTEN:
        v1 = object_fields[OBJ_F_WRITTEN_PAD_I64AS_1].field_8;
        if (v1 == -1) {
            v1 = object_fields[OBJ_F_ITEM_PAD_I64AS_1].field_8;
        }
        break;
    case OBJ_TYPE_ITEM_GENERIC:
        v1 = object_fields[OBJ_F_GENERIC_PAD_I64AS_1].field_8;
        if (v1 == -1) {
            v1 = object_fields[OBJ_F_ITEM_PAD_I64AS_1].field_8;
        }
        break;
    case OBJ_TYPE_PC:
        v1 = object_fields[OBJ_F_PC_PAD_I64AS_1].field_8;
        if (v1 == -1) {
            v1 = object_fields[OBJ_F_CRITTER_PAD_I64AS_1].field_8;
        }
        break;
    case OBJ_TYPE_NPC:
        // NOTE: Probably wrong.
        v1 = object_fields[OBJ_F_NPC_SHIT_LIST_IDX].field_8;
        if (v1 == -1) {
            v1 = object_fields[OBJ_F_CRITTER_PAD_I64AS_1].field_8;
        }
        break;
    case OBJ_TYPE_TRAP:
        v1 = object_fields[OBJ_F_TRAP_PAD_I64AS_1].field_8;
        break;
    default:
        // NOTE: Original code returns `object_type`.
        __assume(0);
    }

    if (v1 == -1) {
        v1 = object_fields[OBJ_F_PAD_I64AS_1].field_8;
    }

    return v1 + 1;
}

// 0x40C260
bool sub_40C260(int type, int fld)
{
    if (object_fields[fld].type <= 2) {
        return false;
    }

    if ((fld > OBJ_F_BEGIN && fld < OBJ_F_END)
        || (fld > OBJ_F_TRANSIENT_BEGIN && fld < OBJ_F_TRANSIENT_END)
        || fld == OBJ_F_TYPE
        || fld == OBJ_F_PROTOTYPE_HANDLE) {
        return true;
    }

    switch (type) {
    case OBJ_TYPE_WALL:
        return fld > OBJ_F_WALL_BEGIN && fld < OBJ_F_WALL_END;
    case OBJ_TYPE_PORTAL:
        return fld > OBJ_F_PORTAL_BEGIN && fld < OBJ_F_PORTAL_END;
    case OBJ_TYPE_CONTAINER:
        return fld > OBJ_F_CONTAINER_BEGIN && fld < OBJ_F_CONTAINER_END;
    case OBJ_TYPE_SCENERY:
        return fld > OBJ_F_SCENERY_BEGIN && fld < OBJ_F_SCENERY_END;
    case OBJ_TYPE_PROJECTILE:
        return fld > OBJ_F_PROJECTILE_BEGIN && fld < OBJ_F_PROJECTILE_END;
    case OBJ_TYPE_WEAPON:
        return (fld > OBJ_F_ITEM_BEGIN && fld < OBJ_F_ITEM_END)
            || (fld > OBJ_F_WEAPON_BEGIN && fld < OBJ_F_WEAPON_END);
    case OBJ_TYPE_AMMO:
        return (fld > OBJ_F_ITEM_BEGIN && fld < OBJ_F_ITEM_END)
            || (fld > OBJ_F_AMMO_BEGIN && fld < OBJ_F_AMMO_END);
    case OBJ_TYPE_ITEM_ARMOR:
        return (fld > OBJ_F_ITEM_BEGIN && fld < OBJ_F_ITEM_END)
            || (fld > OBJ_F_ARMOR_BEGIN && fld < OBJ_F_ARMOR_END);
    case OBJ_TYPE_ITEM_GOLD:
        return (fld > OBJ_F_ITEM_BEGIN && fld < OBJ_F_ITEM_END)
            || (fld > OBJ_F_GOLD_BEGIN && fld < OBJ_F_GOLD_END);
    case OBJ_TYPE_ITEM_FOOD:
        return (fld > OBJ_F_ITEM_BEGIN && fld < OBJ_F_ITEM_END)
            || (fld > OBJ_F_FOOD_BEGIN && fld < OBJ_F_FOOD_END);
    case OBJ_TYPE_ITEM_SCROLL:
        return (fld > OBJ_F_ITEM_BEGIN && fld < OBJ_F_ITEM_END)
            || (fld > OBJ_F_SCROLL_BEGIN && fld < OBJ_F_SCROLL_END);
    case OBJ_TYPE_ITEM_KEY:
        return (fld > OBJ_F_ITEM_BEGIN && fld < OBJ_F_ITEM_END)
            || (fld > OBJ_F_KEY_BEGIN && fld < OBJ_F_KEY_END);
    case OBJ_TYPE_ITEM_KEY_RING:
        return (fld > OBJ_F_ITEM_BEGIN && fld < OBJ_F_ITEM_END)
            || (fld > OBJ_F_KEY_RING_BEGIN && fld < OBJ_F_KEY_RING_END);
    case OBJ_TYPE_ITEM_WRITTEN:
        return (fld > OBJ_F_ITEM_BEGIN && fld < OBJ_F_ITEM_END)
            || (fld > OBJ_F_WRITTEN_BEGIN && fld < OBJ_F_WRITTEN_END);
    case OBJ_TYPE_ITEM_GENERIC:
        return (fld > OBJ_F_ITEM_BEGIN && fld < OBJ_F_ITEM_END)
            || (fld > OBJ_F_GENERIC_BEGIN && fld < OBJ_F_GENERIC_END);
    case OBJ_TYPE_PC:
        return (fld > OBJ_F_CRITTER_BEGIN && fld < OBJ_F_CRITTER_END)
            || (fld > OBJ_F_PC_BEGIN && fld < OBJ_F_PC_END);
    case OBJ_TYPE_NPC:
        return (fld > OBJ_F_CRITTER_BEGIN && fld < OBJ_F_CRITTER_END)
            || (fld > OBJ_F_NPC_BEGIN && fld < OBJ_F_NPC_END);
    case OBJ_TYPE_TRAP:
        return fld > OBJ_F_TRAP_BEGIN && fld < OBJ_F_TRAP_END;
    }

    return false;
}

// 0x40A7B0
void sub_40A7B0()
{
    int fld;

    for (fld = OBJ_F_BEGIN; fld <= OBJ_F_TOTAL_NORMAL; fld++) {
        object_fields[fld].field_14 = object_fields[fld].type <= 0 || object_fields[fld].field_14 > 2;
    }

    for (fld = OBJ_F_TOTAL_NORMAL; fld < OBJ_F_END; fld++) {
        object_fields[fld].field_14 = 0;
    }

    object_fields[OBJ_F_RESISTANCE_IDX].field_14 = 5;
    object_fields[OBJ_F_WEAPON_DAMAGE_LOWER_IDX].field_14 = 5;
    object_fields[OBJ_F_WEAPON_DAMAGE_UPPER_IDX].field_14 = 5;
    object_fields[OBJ_F_WEAPON_MAGIC_DAMAGE_ADJ_IDX].field_14 = 5;
    object_fields[OBJ_F_ARMOR_RESISTANCE_ADJ_IDX].field_14 = 5;
    object_fields[OBJ_F_ARMOR_MAGIC_RESISTANCE_ADJ_IDX].field_14 = 5;
    object_fields[OBJ_F_CRITTER_STAT_BASE_IDX].field_14 = 28;
    object_fields[OBJ_F_CRITTER_BASIC_SKILL_IDX].field_14 = 12;
    object_fields[OBJ_F_CRITTER_TECH_SKILL_IDX].field_14 = 4;
    object_fields[OBJ_F_RENDER_ALPHA].field_14 = 4;
    object_fields[OBJ_F_SHADOW_HANDLES].field_14 = 5;
}

// 0x40A8A0
void sub_40A8A0()
{
    object_fields[OBJ_F_BEGIN].type = 1;
    object_fields[OBJ_F_CURRENT_AID].type = 3;
    object_fields[OBJ_F_LOCATION].type = 4;
    object_fields[OBJ_F_OFFSET_X].type = 3;
    object_fields[OBJ_F_OFFSET_Y].type = 3;
    object_fields[OBJ_F_SHADOW].type = 3;
    object_fields[OBJ_F_OVERLAY_FORE].type = 7;
    object_fields[OBJ_F_OVERLAY_BACK].type = 7;
    object_fields[OBJ_F_UNDERLAY].type = 7;
    object_fields[OBJ_F_BLIT_FLAGS].type = 3;
    object_fields[OBJ_F_BLIT_COLOR].type = 3;
    object_fields[OBJ_F_BLIT_ALPHA].type = 3;
    object_fields[OBJ_F_BLIT_SCALE].type = 3;
    object_fields[OBJ_F_LIGHT_FLAGS].type = 3;
    object_fields[OBJ_F_LIGHT_AID].type = 3;
    object_fields[OBJ_F_LIGHT_COLOR].type = 3;
    object_fields[OBJ_F_OVERLAY_LIGHT_FLAGS].type = 7;
    object_fields[OBJ_F_OVERLAY_LIGHT_AID].type = 7;
    object_fields[OBJ_F_OVERLAY_LIGHT_COLOR].type = 7;
    object_fields[OBJ_F_FLAGS].type = 3;
    object_fields[OBJ_F_SPELL_FLAGS].type = 3;
    object_fields[OBJ_F_BLOCKING_MASK].type = 3;
    object_fields[OBJ_F_NAME].type = 3;
    object_fields[OBJ_F_DESCRIPTION].type = 3;
    object_fields[OBJ_F_AID].type = 3;
    object_fields[OBJ_F_DESTROYED_AID].type = 3;
    object_fields[OBJ_F_AC].type = 3;
    object_fields[OBJ_F_HP_PTS].type = 3;
    object_fields[OBJ_F_HP_ADJ].type = 3;
    object_fields[OBJ_F_MATERIAL].type = 3;
    object_fields[OBJ_F_HP_DAMAGE].type = 3;
    object_fields[OBJ_F_RESISTANCE_IDX].type = 5;
    object_fields[OBJ_F_SCRIPTS_IDX].type = 9;
    object_fields[OBJ_F_SOUND_EFFECT].type = 3;
    object_fields[OBJ_F_CATEGORY].type = 3;
    object_fields[OBJ_F_PAD_IAS_1].type = 7;
    object_fields[OBJ_F_PAD_I64AS_1].type = 8;
    object_fields[OBJ_F_END].type = 2;
    object_fields[OBJ_F_WALL_BEGIN].type = 1;
    object_fields[OBJ_F_WALL_FLAGS].type = 3;
    object_fields[OBJ_F_WALL_PAD_I_1].type = 3;
    object_fields[OBJ_F_WALL_PAD_I_2].type = 3;
    object_fields[OBJ_F_WALL_PAD_IAS_1].type = 7;
    object_fields[OBJ_F_WALL_PAD_I64AS_1].type = 8;
    object_fields[OBJ_F_WALL_END].type = 2;
    object_fields[OBJ_F_PORTAL_BEGIN].type = 1;
    object_fields[OBJ_F_PORTAL_FLAGS].type = 3;
    object_fields[OBJ_F_PORTAL_LOCK_DIFFICULTY].type = 3;
    object_fields[OBJ_F_PORTAL_KEY_ID].type = 3;
    object_fields[OBJ_F_PORTAL_NOTIFY_NPC].type = 3;
    object_fields[OBJ_F_PORTAL_PAD_I_1].type = 3;
    object_fields[OBJ_F_PORTAL_PAD_I_2].type = 3;
    object_fields[OBJ_F_PORTAL_PAD_IAS_1].type = 7;
    object_fields[OBJ_F_PORTAL_PAD_I64AS_1].type = 8;
    object_fields[OBJ_F_PORTAL_END].type = 2;
    object_fields[OBJ_F_CONTAINER_BEGIN].type = 1;
    object_fields[OBJ_F_CONTAINER_FLAGS].type = 3;
    object_fields[OBJ_F_CONTAINER_LOCK_DIFFICULTY].type = 3;
    object_fields[OBJ_F_CONTAINER_KEY_ID].type = 3;
    object_fields[OBJ_F_CONTAINER_INVENTORY_NUM].type = 3;
    object_fields[OBJ_F_CONTAINER_INVENTORY_LIST_IDX].type = 13;
    object_fields[OBJ_F_CONTAINER_INVENTORY_SOURCE].type = 3;
    object_fields[OBJ_F_CONTAINER_NOTIFY_NPC].type = 3;
    object_fields[OBJ_F_CONTAINER_PAD_I_1].type = 3;
    object_fields[OBJ_F_CONTAINER_PAD_I_2].type = 3;
    object_fields[OBJ_F_CONTAINER_PAD_IAS_1].type = 7;
    object_fields[OBJ_F_CONTAINER_PAD_I64AS_1].type = 8;
    object_fields[OBJ_F_CONTAINER_END].type = 2;
    object_fields[OBJ_F_SCENERY_BEGIN].type = 1;
    object_fields[OBJ_F_SCENERY_FLAGS].type = 3;
    object_fields[OBJ_F_SCENERY_WHOS_IN_ME].type = 12;
    object_fields[OBJ_F_SCENERY_RESPAWN_DELAY].type = 3;
    object_fields[OBJ_F_SCENERY_PAD_I_2].type = 3;
    object_fields[OBJ_F_SCENERY_PAD_IAS_1].type = 7;
    object_fields[OBJ_F_SCENERY_PAD_I64AS_1].type = 8;
    object_fields[OBJ_F_SCENERY_END].type = 2;
    object_fields[OBJ_F_PROJECTILE_BEGIN].type = 1;
    object_fields[OBJ_F_PROJECTILE_FLAGS_COMBAT].type = 3;
    object_fields[OBJ_F_PROJECTILE_FLAGS_COMBAT_DAMAGE].type = 3;
    object_fields[OBJ_F_PROJECTILE_HIT_LOC].type = 3;
    object_fields[OBJ_F_PROJECTILE_PARENT_WEAPON].type = 12;
    object_fields[OBJ_F_PROJECTILE_PAD_I_1].type = 3;
    object_fields[OBJ_F_PROJECTILE_PAD_I_2].type = 3;
    object_fields[OBJ_F_PROJECTILE_PAD_IAS_1].type = 7;
    object_fields[OBJ_F_PROJECTILE_PAD_I64AS_1].type = 8;
    object_fields[OBJ_F_PROJECTILE_END].type = 2;
    object_fields[OBJ_F_ITEM_BEGIN].type = 1;
    object_fields[OBJ_F_ITEM_FLAGS].type = 3;
    object_fields[OBJ_F_ITEM_PARENT].type = 12;
    object_fields[OBJ_F_ITEM_WEIGHT].type = 3;
    object_fields[OBJ_F_ITEM_MAGIC_WEIGHT_ADJ].type = 3;
    object_fields[OBJ_F_ITEM_WORTH].type = 3;
    object_fields[OBJ_F_ITEM_MANA_STORE].type = 3;
    object_fields[OBJ_F_ITEM_INV_AID].type = 3;
    object_fields[OBJ_F_ITEM_INV_LOCATION].type = 3;
    object_fields[OBJ_F_ITEM_USE_AID_FRAGMENT].type = 3;
    object_fields[OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY].type = 3;
    object_fields[OBJ_F_ITEM_DISCIPLINE].type = 3;
    object_fields[OBJ_F_ITEM_DESCRIPTION_UNKNOWN].type = 3;
    object_fields[OBJ_F_ITEM_DESCRIPTION_EFFECTS].type = 3;
    object_fields[OBJ_F_ITEM_SPELL_1].type = 3;
    object_fields[OBJ_F_ITEM_SPELL_2].type = 3;
    object_fields[OBJ_F_ITEM_SPELL_3].type = 3;
    object_fields[OBJ_F_ITEM_SPELL_4].type = 3;
    object_fields[OBJ_F_ITEM_SPELL_5].type = 3;
    object_fields[OBJ_F_ITEM_SPELL_MANA_STORE].type = 3;
    object_fields[OBJ_F_ITEM_AI_ACTION].type = 3;
    object_fields[OBJ_F_ITEM_PAD_I_1].type = 3;
    object_fields[OBJ_F_ITEM_PAD_IAS_1].type = 7;
    object_fields[OBJ_F_ITEM_PAD_I64AS_1].type = 8;
    object_fields[OBJ_F_ITEM_END].type = 2;
    object_fields[OBJ_F_WEAPON_BEGIN].type = 1;
    object_fields[OBJ_F_WEAPON_FLAGS].type = 3;
    object_fields[OBJ_F_WEAPON_PAPER_DOLL_AID].type = 3;
    object_fields[OBJ_F_WEAPON_BONUS_TO_HIT].type = 3;
    object_fields[OBJ_F_WEAPON_MAGIC_HIT_ADJ].type = 3;
    object_fields[OBJ_F_WEAPON_DAMAGE_LOWER_IDX].type = 5;
    object_fields[OBJ_F_WEAPON_DAMAGE_UPPER_IDX].type = 5;
    object_fields[OBJ_F_WEAPON_MAGIC_DAMAGE_ADJ_IDX].type = 5;
    object_fields[OBJ_F_WEAPON_SPEED_FACTOR].type = 3;
    object_fields[OBJ_F_WEAPON_MAGIC_SPEED_ADJ].type = 3;
    object_fields[OBJ_F_WEAPON_RANGE].type = 3;
    object_fields[OBJ_F_WEAPON_MAGIC_RANGE_ADJ].type = 3;
    object_fields[OBJ_F_WEAPON_MIN_STRENGTH].type = 3;
    object_fields[OBJ_F_WEAPON_MAGIC_MIN_STRENGTH_ADJ].type = 3;
    object_fields[OBJ_F_WEAPON_AMMO_TYPE].type = 3;
    object_fields[OBJ_F_WEAPON_AMMO_CONSUMPTION].type = 3;
    object_fields[OBJ_F_WEAPON_MISSILE_AID].type = 3;
    object_fields[OBJ_F_WEAPON_VISUAL_EFFECT_AID].type = 3;
    object_fields[OBJ_F_WEAPON_CRIT_HIT_CHART].type = 3;
    object_fields[OBJ_F_WEAPON_MAGIC_CRIT_HIT_CHANCE].type = 3;
    object_fields[OBJ_F_WEAPON_MAGIC_CRIT_HIT_EFFECT].type = 3;
    object_fields[OBJ_F_WEAPON_CRIT_MISS_CHART].type = 3;
    object_fields[OBJ_F_WEAPON_MAGIC_CRIT_MISS_CHANCE].type = 3;
    object_fields[OBJ_F_WEAPON_MAGIC_CRIT_MISS_EFFECT].type = 3;
    object_fields[OBJ_F_WEAPON_PAD_I_1].type = 3;
    object_fields[OBJ_F_WEAPON_PAD_I_2].type = 3;
    object_fields[OBJ_F_WEAPON_PAD_IAS_1].type = 7;
    object_fields[OBJ_F_WEAPON_PAD_I64AS_1].type = 8;
    object_fields[OBJ_F_WEAPON_END].type = 2;
    object_fields[OBJ_F_AMMO_BEGIN].type = 1;
    object_fields[OBJ_F_AMMO_FLAGS].type = 3;
    object_fields[OBJ_F_AMMO_QUANTITY].type = 3;
    object_fields[OBJ_F_AMMO_TYPE].type = 3;
    object_fields[OBJ_F_AMMO_PAD_I_1].type = 3;
    object_fields[OBJ_F_AMMO_PAD_I_2].type = 3;
    object_fields[OBJ_F_AMMO_PAD_IAS_1].type = 7;
    object_fields[OBJ_F_AMMO_PAD_I64AS_1].type = 8;
    object_fields[OBJ_F_AMMO_END].type = 2;
    object_fields[OBJ_F_ARMOR_BEGIN].type = 1;
    object_fields[OBJ_F_ARMOR_FLAGS].type = 3;
    object_fields[OBJ_F_ARMOR_PAPER_DOLL_AID].type = 3;
    object_fields[OBJ_F_ARMOR_AC_ADJ].type = 3;
    object_fields[OBJ_F_ARMOR_MAGIC_AC_ADJ].type = 3;
    object_fields[OBJ_F_ARMOR_RESISTANCE_ADJ_IDX].type = 5;
    object_fields[OBJ_F_ARMOR_MAGIC_RESISTANCE_ADJ_IDX].type = 5;
    object_fields[OBJ_F_ARMOR_SILENT_MOVE_ADJ].type = 3;
    object_fields[OBJ_F_ARMOR_MAGIC_SILENT_MOVE_ADJ].type = 3;
    object_fields[OBJ_F_ARMOR_UNARMED_BONUS_DAMAGE].type = 3;
    object_fields[OBJ_F_ARMOR_PAD_I_2].type = 3;
    object_fields[OBJ_F_ARMOR_PAD_IAS_1].type = 7;
    object_fields[OBJ_F_ARMOR_PAD_I64AS_1].type = 8;
    object_fields[OBJ_F_ARMOR_END].type = 2;
    object_fields[OBJ_F_GOLD_BEGIN].type = 1;
    object_fields[OBJ_F_GOLD_FLAGS].type = 3;
    object_fields[OBJ_F_GOLD_QUANTITY].type = 3;
    object_fields[OBJ_F_GOLD_PAD_I_1].type = 3;
    object_fields[OBJ_F_GOLD_PAD_I_2].type = 3;
    object_fields[OBJ_F_GOLD_PAD_IAS_1].type = 7;
    object_fields[OBJ_F_GOLD_PAD_I64AS_1].type = 8;
    object_fields[OBJ_F_GOLD_END].type = 2;
    object_fields[OBJ_F_FOOD_BEGIN].type = 1;
    object_fields[OBJ_F_FOOD_FLAGS].type = 3;
    object_fields[OBJ_F_FOOD_PAD_I_1].type = 3;
    object_fields[OBJ_F_FOOD_PAD_I_2].type = 3;
    object_fields[OBJ_F_FOOD_PAD_IAS_1].type = 7;
    object_fields[OBJ_F_FOOD_PAD_I64AS_1].type = 8;
    object_fields[OBJ_F_FOOD_END].type = 2;
    object_fields[OBJ_F_SCROLL_BEGIN].type = 1;
    object_fields[OBJ_F_SCROLL_FLAGS].type = 3;
    object_fields[OBJ_F_SCROLL_PAD_I_1].type = 3;
    object_fields[OBJ_F_SCROLL_PAD_I_2].type = 3;
    object_fields[OBJ_F_SCROLL_PAD_IAS_1].type = 7;
    object_fields[OBJ_F_SCROLL_PAD_I64AS_1].type = 8;
    object_fields[OBJ_F_SCROLL_END].type = 2;
    object_fields[OBJ_F_KEY_BEGIN].type = 1;
    object_fields[OBJ_F_KEY_KEY_ID].type = 3;
    object_fields[OBJ_F_KEY_PAD_I_1].type = 3;
    object_fields[OBJ_F_KEY_PAD_I_2].type = 3;
    object_fields[OBJ_F_KEY_PAD_IAS_1].type = 7;
    object_fields[OBJ_F_KEY_PAD_I64AS_1].type = 8;
    object_fields[OBJ_F_KEY_END].type = 2;
    object_fields[OBJ_F_KEY_RING_BEGIN].type = 1;
    object_fields[OBJ_F_KEY_RING_FLAGS].type = 3;
    object_fields[OBJ_F_KEY_RING_LIST_IDX].type = 7;
    object_fields[OBJ_F_KEY_RING_PAD_I_1].type = 3;
    object_fields[OBJ_F_KEY_RING_PAD_I_2].type = 3;
    object_fields[OBJ_F_KEY_RING_PAD_IAS_1].type = 7;
    object_fields[OBJ_F_KEY_RING_PAD_I64AS_1].type = 8;
    object_fields[OBJ_F_KEY_RING_END].type = 2;
    object_fields[OBJ_F_WRITTEN_BEGIN].type = 1;
    object_fields[OBJ_F_WRITTEN_FLAGS].type = 3;
    object_fields[OBJ_F_WRITTEN_SUBTYPE].type = 3;
    object_fields[OBJ_F_WRITTEN_TEXT_START_LINE].type = 3;
    object_fields[OBJ_F_WRITTEN_TEXT_END_LINE].type = 3;
    object_fields[OBJ_F_WRITTEN_PAD_I_1].type = 3;
    object_fields[OBJ_F_WRITTEN_PAD_I_2].type = 3;
    object_fields[OBJ_F_WRITTEN_PAD_IAS_1].type = 7;
    object_fields[OBJ_F_WRITTEN_PAD_I64AS_1].type = 8;
    object_fields[OBJ_F_WRITTEN_END].type = 2;
    object_fields[OBJ_F_GENERIC_BEGIN].type = 1;
    object_fields[OBJ_F_GENERIC_FLAGS].type = 3;
    object_fields[OBJ_F_GENERIC_USAGE_BONUS].type = 3;
    object_fields[OBJ_F_GENERIC_USAGE_COUNT_REMAINING].type = 3;
    object_fields[OBJ_F_GENERIC_PAD_IAS_1].type = 7;
    object_fields[OBJ_F_GENERIC_PAD_I64AS_1].type = 8;
    object_fields[OBJ_F_GENERIC_END].type = 2;
    object_fields[OBJ_F_CRITTER_BEGIN].type = 1;
    object_fields[OBJ_F_CRITTER_FLAGS].type = 3;
    object_fields[OBJ_F_CRITTER_FLAGS2].type = 3;
    object_fields[OBJ_F_CRITTER_STAT_BASE_IDX].type = 5;
    object_fields[OBJ_F_CRITTER_BASIC_SKILL_IDX].type = 5;
    object_fields[OBJ_F_CRITTER_TECH_SKILL_IDX].type = 5;
    object_fields[OBJ_F_CRITTER_SPELL_TECH_IDX].type = 7;
    object_fields[OBJ_F_CRITTER_FATIGUE_PTS].type = 3;
    object_fields[OBJ_F_CRITTER_FATIGUE_ADJ].type = 3;
    object_fields[OBJ_F_CRITTER_FATIGUE_DAMAGE].type = 3;
    object_fields[OBJ_F_CRITTER_CRIT_HIT_CHART].type = 3;
    object_fields[OBJ_F_CRITTER_EFFECTS_IDX].type = 7;
    object_fields[OBJ_F_CRITTER_EFFECT_CAUSE_IDX].type = 7;
    object_fields[OBJ_F_CRITTER_FLEEING_FROM].type = 12;
    object_fields[OBJ_F_CRITTER_PORTRAIT].type = 3;
    object_fields[OBJ_F_CRITTER_GOLD].type = 12;
    object_fields[OBJ_F_CRITTER_ARROWS].type = 12;
    object_fields[OBJ_F_CRITTER_BULLETS].type = 12;
    object_fields[OBJ_F_CRITTER_POWER_CELLS].type = 12;
    object_fields[OBJ_F_CRITTER_FUEL].type = 12;
    object_fields[OBJ_F_CRITTER_INVENTORY_NUM].type = 3;
    object_fields[OBJ_F_CRITTER_INVENTORY_LIST_IDX].type = 13;
    object_fields[OBJ_F_CRITTER_INVENTORY_SOURCE].type = 3;
    object_fields[OBJ_F_CRITTER_DESCRIPTION_UNKNOWN].type = 3;
    object_fields[OBJ_F_CRITTER_FOLLOWER_IDX].type = 13;
    object_fields[OBJ_F_CRITTER_TELEPORT_DEST].type = 4;
    object_fields[OBJ_F_CRITTER_TELEPORT_MAP].type = 3;
    object_fields[OBJ_F_CRITTER_DEATH_TIME].type = 3;
    object_fields[OBJ_F_CRITTER_AUTO_LEVEL_SCHEME].type = 3;
    object_fields[OBJ_F_CRITTER_PAD_I_1].type = 3;
    object_fields[OBJ_F_CRITTER_PAD_I_2].type = 3;
    object_fields[OBJ_F_CRITTER_PAD_I_3].type = 3;
    object_fields[OBJ_F_CRITTER_PAD_IAS_1].type = 7;
    object_fields[OBJ_F_CRITTER_PAD_I64AS_1].type = 8;
    object_fields[OBJ_F_CRITTER_END].type = 2;
    object_fields[OBJ_F_PC_BEGIN].type = 1;
    object_fields[OBJ_F_PC_FLAGS].type = 3;
    object_fields[OBJ_F_PC_FLAGS_FATE].type = 3;
    object_fields[OBJ_F_PC_REPUTATION_IDX].type = 7;
    object_fields[OBJ_F_PC_BACKGROUND].type = 3;
    object_fields[OBJ_F_PC_BACKGROUND_TEXT].type = 3;
    object_fields[OBJ_F_PC_REPUTATION_TS_IDX].type = 8;
    object_fields[OBJ_F_PC_BACKGROUND].type = 3;
    object_fields[OBJ_F_PC_QUEST_IDX].type = 10;
    object_fields[OBJ_F_PC_BLESSING_IDX].type = 7;
    object_fields[OBJ_F_PC_BLESSING_TS_IDX].type = 8;
    object_fields[OBJ_F_PC_CURSE_IDX].type = 7;
    object_fields[OBJ_F_PC_CURSE_TS_IDX].type = 8;
    object_fields[OBJ_F_PC_PARTY_ID].type = 3;
    object_fields[OBJ_F_PC_RUMOR_IDX].type = 8;
    object_fields[OBJ_F_PC_PAD_IAS_2].type = 7;
    object_fields[OBJ_F_PC_SCHEMATICS_FOUND_IDX].type = 7;
    object_fields[OBJ_F_PC_LOGBOOK_EGO_IDX].type = 7;
    object_fields[OBJ_F_PC_FOG_MASK].type = 3;
    object_fields[OBJ_F_PC_PLAYER_NAME].type = 11;
    object_fields[OBJ_F_PC_BANK_MONEY].type = 3;
    object_fields[OBJ_F_PC_GLOBAL_FLAGS].type = 7;
    object_fields[OBJ_F_PC_GLOBAL_VARIABLES].type = 7;
    object_fields[OBJ_F_PC_PAD_I_1].type = 3;
    object_fields[OBJ_F_PC_PAD_I_2].type = 3;
    object_fields[OBJ_F_PC_PAD_IAS_1].type = 7;
    object_fields[OBJ_F_PC_PAD_I64AS_1].type = 8;
    object_fields[OBJ_F_PC_END].type = 2;
    object_fields[OBJ_F_NPC_BEGIN].type = 1;
    object_fields[OBJ_F_NPC_FLAGS].type = 3;
    object_fields[OBJ_F_NPC_LEADER].type = 12;
    object_fields[OBJ_F_NPC_AI_DATA].type = 3;
    object_fields[OBJ_F_NPC_COMBAT_FOCUS].type = 12;
    object_fields[OBJ_F_NPC_WHO_HIT_ME_LAST].type = 12;
    object_fields[OBJ_F_NPC_EXPERIENCE_WORTH].type = 3;
    object_fields[OBJ_F_NPC_EXPERIENCE_POOL].type = 3;
    object_fields[OBJ_F_NPC_WAYPOINTS_IDX].type = 8;
    object_fields[OBJ_F_NPC_WAYPOINT_CURRENT].type = 3;
    object_fields[OBJ_F_NPC_STANDPOINT_DAY].type = 4;
    object_fields[OBJ_F_NPC_STANDPOINT_NIGHT].type = 4;
    object_fields[OBJ_F_NPC_ORIGIN].type = 3;
    object_fields[OBJ_F_NPC_FACTION].type = 3;
    object_fields[OBJ_F_NPC_RETAIL_PRICE_MULTIPLIER].type = 3;
    object_fields[OBJ_F_NPC_SUBSTITUTE_INVENTORY].type = 12;
    object_fields[OBJ_F_NPC_REACTION_BASE].type = 3;
    object_fields[OBJ_F_NPC_SOCIAL_CLASS].type = 3;
    object_fields[OBJ_F_NPC_REACTION_PC_IDX].type = 13;
    object_fields[OBJ_F_NPC_REACTION_LEVEL_IDX].type = 7;
    object_fields[OBJ_F_NPC_REACTION_TIME_IDX].type = 7;
    object_fields[OBJ_F_NPC_WAIT].type = 3;
    object_fields[OBJ_F_NPC_GENERATOR_DATA].type = 3;
    object_fields[OBJ_F_NPC_PAD_I_1].type = 3;
    object_fields[OBJ_F_NPC_DAMAGE_IDX].type = 7;
    object_fields[OBJ_F_NPC_SHIT_LIST_IDX].type = 13;
    object_fields[OBJ_F_NPC_END].type = 2;
    object_fields[OBJ_F_TRAP_BEGIN].type = 1;
    object_fields[OBJ_F_TRAP_FLAGS].type = 3;
    object_fields[OBJ_F_TRAP_DIFFICULTY].type = 3;
    object_fields[OBJ_F_TRAP_PAD_I_2].type = 3;
    object_fields[OBJ_F_TRAP_PAD_IAS_1].type = 7;
    object_fields[OBJ_F_TRAP_PAD_I64AS_1].type = 8;
    object_fields[OBJ_F_TRAP_END].type = 2;
    object_fields[OBJ_F_TOTAL_NORMAL].type = 0;
    object_fields[OBJ_F_RENDER_COLOR].type = 3;
    object_fields[OBJ_F_RENDER_COLORS].type = 3;
    object_fields[OBJ_F_RENDER_PALETTE].type = 3;
    object_fields[OBJ_F_RENDER_SCALE].type = 3;
    object_fields[OBJ_F_RENDER_ALPHA].type = 5;
    object_fields[OBJ_F_RENDER_X].type = 3;
    object_fields[OBJ_F_RENDER_Y].type = 3;
    object_fields[OBJ_F_RENDER_WIDTH].type = 3;
    object_fields[OBJ_F_RENDER_HEIGHT].type = 3;
    object_fields[OBJ_F_PALETTE].type = 3;
    object_fields[OBJ_F_COLOR].type = 3;
    object_fields[OBJ_F_COLORS].type = 3;
    object_fields[OBJ_F_RENDER_FLAGS].type = 3;
    object_fields[OBJ_F_TEMP_ID].type = 3;
    object_fields[OBJ_F_LIGHT_HANDLE].type = 3;
    object_fields[OBJ_F_OVERLAY_LIGHT_HANDLES].type = 7;
    object_fields[OBJ_F_SHADOW_HANDLES].type = 5;
    object_fields[OBJ_F_INTERNAL_FLAGS].type = 3;
    object_fields[OBJ_F_FIND_NODE].type = 3;
    object_fields[OBJ_F_TYPE].type = 3;
    object_fields[OBJ_F_PROTOTYPE_HANDLE].type = 12;
}

// 0x40BBB0
void sub_40BBB0()
{
    if (dword_5D1128 != NULL) {
        free(dword_5D1128);
        dword_5D1128 = NULL;
    }

    if (dword_5D112C != NULL) {
        free(dword_5D112C);
        dword_5D112C = NULL;
    }
}

// 0x4F0270
void object_field_set_with_network(object_id_t object_id, int field, int a3, int a4)
{
    // TODO: Incomplete.
}

// 0x40C560
bool sub_40C560(Object* object, int fld)
{
    (void)object;
    (void)fld;

    word_5D10FC++;
    dword_5D10F4++;

    return true;
}

// 0x40C6B0
bool sub_40C6B0(Object* object, int fld)
{
    (void)fld;

    object->field_50[dword_5D10F4++] = 0;

    return true;
}

// 0x40C880
bool obj_enumerate_fields(Object* object, ObjEnumerateCallback* callback)
{
    if (!obj_enumerate_fields_in_range(object, OBJ_F_BEGIN, OBJ_F_END, callback)) {
        return false;
    }

    switch (object->type) {
    case OBJ_TYPE_WALL:
        if (!obj_enumerate_fields_in_range(object, OBJ_F_WALL_BEGIN, OBJ_F_WALL_END, callback)) {
            return false;
        }
        break;
    case OBJ_TYPE_PORTAL:
        if (!obj_enumerate_fields_in_range(object, OBJ_F_PORTAL_BEGIN, OBJ_F_PORTAL_END, callback)) {
            return false;
        }
        break;
    case OBJ_TYPE_CONTAINER:
        if (!obj_enumerate_fields_in_range(object, OBJ_F_CONTAINER_BEGIN, OBJ_F_CONTAINER_END, callback)) {
            return false;
        }
        break;
    case OBJ_TYPE_SCENERY:
        if (!obj_enumerate_fields_in_range(object, OBJ_F_SCENERY_BEGIN, OBJ_F_SCENERY_END, callback)) {
            return false;
        }
        break;
    case OBJ_TYPE_PROJECTILE:
        if (!obj_enumerate_fields_in_range(object, OBJ_F_PROJECTILE_BEGIN, OBJ_F_PROJECTILE_END, callback)) {
            return false;
        }
        break;
    case OBJ_TYPE_WEAPON:
        if (!obj_enumerate_fields_in_range(object, OBJ_F_ITEM_BEGIN, OBJ_F_ITEM_END, callback)) {
            return false;
        }
        if (!obj_enumerate_fields_in_range(object, OBJ_F_WEAPON_BEGIN, OBJ_F_WEAPON_END, callback)) {
            return false;
        }
        break;
    case OBJ_TYPE_AMMO:
        if (!obj_enumerate_fields_in_range(object, OBJ_F_ITEM_BEGIN, OBJ_F_ITEM_END, callback)) {
            return false;
        }
        if (!obj_enumerate_fields_in_range(object, OBJ_F_AMMO_BEGIN, OBJ_F_AMMO_END, callback)) {
            return false;
        }
        break;
    case OBJ_TYPE_ITEM_ARMOR:
        if (!obj_enumerate_fields_in_range(object, OBJ_F_ITEM_BEGIN, OBJ_F_ITEM_END, callback)) {
            return false;
        }
        if (!obj_enumerate_fields_in_range(object, OBJ_F_ARMOR_BEGIN, OBJ_F_ARMOR_END, callback)) {
            return false;
        }
        break;
    case OBJ_TYPE_ITEM_GOLD:
        if (!obj_enumerate_fields_in_range(object, OBJ_F_ITEM_BEGIN, OBJ_F_ITEM_END, callback)) {
            return false;
        }
        if (!obj_enumerate_fields_in_range(object, OBJ_F_GOLD_BEGIN, OBJ_F_GOLD_END, callback)) {
            return false;
        }
        break;
    case OBJ_TYPE_ITEM_FOOD:
        if (!obj_enumerate_fields_in_range(object, OBJ_F_ITEM_BEGIN, OBJ_F_ITEM_END, callback)) {
            return false;
        }
        if (!obj_enumerate_fields_in_range(object, OBJ_F_FOOD_BEGIN, OBJ_F_FOOD_END, callback)) {
            return false;
        }
        break;
    case OBJ_TYPE_ITEM_SCROLL:
        if (!obj_enumerate_fields_in_range(object, OBJ_F_ITEM_BEGIN, OBJ_F_ITEM_END, callback)) {
            return false;
        }
        if (!obj_enumerate_fields_in_range(object, OBJ_F_SCROLL_BEGIN, OBJ_F_SCROLL_END, callback)) {
            return false;
        }
        break;
    case OBJ_TYPE_ITEM_KEY:
        if (!obj_enumerate_fields_in_range(object, OBJ_F_ITEM_BEGIN, OBJ_F_ITEM_END, callback)) {
            return false;
        }
        if (!obj_enumerate_fields_in_range(object, OBJ_F_KEY_BEGIN, OBJ_F_KEY_END, callback)) {
            return false;
        }
        break;
    case OBJ_TYPE_ITEM_KEY_RING:
        if (!obj_enumerate_fields_in_range(object, OBJ_F_ITEM_BEGIN, OBJ_F_ITEM_END, callback)) {
            return false;
        }
        if (!obj_enumerate_fields_in_range(object, OBJ_F_KEY_RING_BEGIN, OBJ_F_KEY_RING_END, callback)) {
            return false;
        }
        break;
    case OBJ_TYPE_ITEM_WRITTEN:
        if (!obj_enumerate_fields_in_range(object, OBJ_F_ITEM_BEGIN, OBJ_F_ITEM_END, callback)) {
            return false;
        }
        if (!obj_enumerate_fields_in_range(object, OBJ_F_WRITTEN_BEGIN, OBJ_F_WRITTEN_END, callback)) {
            return false;
        }
        break;
    case OBJ_TYPE_ITEM_GENERIC:
        if (!obj_enumerate_fields_in_range(object, OBJ_F_ITEM_BEGIN, OBJ_F_ITEM_END, callback)) {
            return false;
        }
        if (!obj_enumerate_fields_in_range(object, OBJ_F_GENERIC_BEGIN, OBJ_F_GENERIC_END, callback)) {
            return false;
        }
        break;
    case OBJ_TYPE_PC:
        if (!obj_enumerate_fields_in_range(object, OBJ_F_CRITTER_BEGIN, OBJ_F_CRITTER_END, callback)) {
            return false;
        }
        if (!obj_enumerate_fields_in_range(object, OBJ_F_PC_BEGIN, OBJ_F_PC_END, callback)) {
            return false;
        }
        break;
    case OBJ_TYPE_NPC:
        if (!obj_enumerate_fields_in_range(object, OBJ_F_NPC_BEGIN, OBJ_F_NPC_END, callback)) {
            return false;
        }
        break;
    case OBJ_TYPE_TRAP:
        if (!obj_enumerate_fields_in_range(object, OBJ_F_TRAP_BEGIN, OBJ_F_TRAP_END, callback)) {
            return false;
        }
        break;
    }

    return true;
}

// 0x40CB40
int sub_40CB40(Object* object, int fld)
{
    (void)object;

    return object_fields[fld].field_0;
}

// 0x40CB00
bool obj_enumerate_fields_in_range(Object* obj, int begin, int end, ObjEnumerateCallback* callback)
{
    int index;

    // +1 to skip `OBJ_F_{TYPE}_BEGIN`.
    for (index = begin + 1; index < end; index++) {
        if (!callback(obj, index)) {
            return false;
        }
    }

    return true;
}

// 0x40D560
bool sub_40D560(TigFile* stream)
{
    int version = OBJ_FILE_VERSION;
    return sub_4E7010(&version, sizeof(version), stream);
}

// 0x40D590
bool obj_check_version_stream(TigFile* stream)
{
    int version;

    if (!obj_read_raw(version, sizeof(version), stream)
        || version != OBJ_FILE_VERSION) {
        tig_debug_printf("Object file format version mismatch (read: %d, expected: %d).\n", version, OBJ_FILE_VERSION);
        return false;
    }

    return true;
}

// 0x40D5D0
void sub_40D5D0(void* mem)
{
    int version = OBJ_FILE_VERSION;
    sub_4E4C00(&version, sizeof(version), mem);
}

// 0x40D5F0
bool obj_check_version_memory(void* mem)
{
    int version;

    sub_4E4C50(version, sizeof(version, mem));
    if (version != OBJ_FILE_VERSION) {
        tig_debug_printf("Object file format version mismatch: (read: %d, expected: %d).\n", version, OBJ_FILE_VERSION);
    }

    return true;
}

// 0x40D630
int64_t obj_get_prototype_handle(Object* object)
{
    if (object->prototype_handle == OBJ_HANDLE_NULL) {
        object->prototype_handle = objp_perm_lookup(object->field_20);
    }

    return object->prototype_handle;
}

// 0x40D670
bool sub_40D670(Object* object, int a2, ObjectFieldInfo* field_info)
{
    if ((object->field_4C[field_info->field_8] & field_info->field_C) != 0) {
        tig_debug_printf("\t #%d", field_info - object_fields);
    }
    return true;
}
