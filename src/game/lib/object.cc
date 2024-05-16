#include "game/lib/object.h"

typedef struct ObjectFieldInfo {
    /* 0000 */ int field_0;
    /* 0004 */ int field_4;
    /* 0008 */ int field_8;
    /* 000C */ int field_C;
    /* 0010 */ int field_10;
    /* 0014 */ int field_14;
    /* 0018 */ int field_18;
} ObjectFieldInfo;

static_assert(sizeof(ObjectFieldInfo) == 0x1C, "wrong size");

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

// 0x5D1104
static ObjectFieldInfo* object_fields;

// 0x5D1128
static void* dword_5D1128;

// 0x5D112C
static void* dword_5D112C;

// 0x406CA0
int object_field_get(object_id_t object_id, int field)
{
    // TODO: Incomplete.

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
bool sub_40C260(ObjectType object_type, ObjectField field)
{
    if (object_fields[field].field_18 <= 2) {
        return false;
    }

    if ((field > OBJ_F_BEGIN && field < OBJ_F_END)
        || (field > OBJ_F_TRANSIENT_BEGIN && field < OBJ_F_TRANSIENT_END)
        || field == OBJ_F_TYPE
        || field == OBJ_F_PROTOTYPE_HANDLE) {
        return true;
    }

    switch (object_type) {
    case OBJ_TYPE_WALL:
        return field > OBJ_F_WALL_BEGIN && field < OBJ_F_WALL_END;
    case OBJ_TYPE_PORTAL:
        return field > OBJ_F_PORTAL_BEGIN && field < OBJ_F_PORTAL_END;
    case OBJ_TYPE_CONTAINER:
        return field > OBJ_F_CONTAINER_BEGIN && field < OBJ_F_CONTAINER_END;
    case OBJ_TYPE_SCENERY:
        return field > OBJ_F_SCENERY_BEGIN && field < OBJ_F_SCENERY_END;
    case OBJ_TYPE_PROJECTILE:
        return field > OBJ_F_PROJECTILE_BEGIN && field < OBJ_F_PROJECTILE_END;
    case OBJ_TYPE_WEAPON:
        return (field > OBJ_F_ITEM_BEGIN && field < OBJ_F_ITEM_END)
            || (field > OBJ_F_WEAPON_BEGIN && field < OBJ_F_WEAPON_END);
    case OBJ_TYPE_AMMO:
        return (field > OBJ_F_ITEM_BEGIN && field < OBJ_F_ITEM_END)
            || (field > OBJ_F_AMMO_BEGIN && field < OBJ_F_AMMO_END);
    case OBJ_TYPE_ITEM_ARMOR:
        return (field > OBJ_F_ITEM_BEGIN && field < OBJ_F_ITEM_END)
            || (field > OBJ_F_ARMOR_BEGIN && field < OBJ_F_ARMOR_END);
    case OBJ_TYPE_ITEM_GOLD:
        return (field > OBJ_F_ITEM_BEGIN && field < OBJ_F_ITEM_END)
            || (field > OBJ_F_GOLD_BEGIN && field < OBJ_F_GOLD_END);
    case OBJ_TYPE_ITEM_FOOD:
        return (field > OBJ_F_ITEM_BEGIN && field < OBJ_F_ITEM_END)
            || (field > OBJ_F_FOOD_BEGIN && field < OBJ_F_FOOD_END);
    case OBJ_TYPE_ITEM_SCROLL:
        return (field > OBJ_F_ITEM_BEGIN && field < OBJ_F_ITEM_END)
            || (field > OBJ_F_SCROLL_BEGIN && field < OBJ_F_SCROLL_END);
    case OBJ_TYPE_ITEM_KEY:
        return (field > OBJ_F_ITEM_BEGIN && field < OBJ_F_ITEM_END)
            || (field > OBJ_F_KEY_BEGIN && field < OBJ_F_KEY_END);
    case OBJ_TYPE_ITEM_KEY_RING:
        return (field > OBJ_F_ITEM_BEGIN && field < OBJ_F_ITEM_END)
            || (field > OBJ_F_KEY_RING_BEGIN && field < OBJ_F_KEY_RING_END);
    case OBJ_TYPE_ITEM_WRITTEN:
        return (field > OBJ_F_ITEM_BEGIN && field < OBJ_F_ITEM_END)
            || (field > OBJ_F_WRITTEN_BEGIN && field < OBJ_F_WRITTEN_END);
    case OBJ_TYPE_ITEM_GENERIC:
        return (field > OBJ_F_ITEM_BEGIN && field < OBJ_F_ITEM_END)
            || (field > OBJ_F_GENERIC_BEGIN && field < OBJ_F_GENERIC_END);
    case OBJ_TYPE_PC:
        return (field > OBJ_F_CRITTER_BEGIN && field < OBJ_F_CRITTER_END)
            || (field > OBJ_F_PC_BEGIN && field < OBJ_F_PC_END);
    case OBJ_TYPE_NPC:
        return (field > OBJ_F_CRITTER_BEGIN && field < OBJ_F_CRITTER_END)
            || (field > OBJ_F_NPC_BEGIN && field < OBJ_F_NPC_END);
    case OBJ_TYPE_TRAP:
        return field > OBJ_F_TRAP_BEGIN && field < OBJ_F_TRAP_END;
    }

    return false;
}

// 0x40A7B0
void sub_40A7B0()
{
    int index;

    for (index = OBJ_F_BEGIN; index <= OBJ_F_TOTAL_NORMAL; index++) {
        object_fields[index].field_14 = object_fields[index].field_18 <= 0 || object_fields[index].field_14 > 2;
    }

    for (index = OBJ_F_TOTAL_NORMAL; index < OBJ_F_END; index++) {
        object_fields[index].field_14 = false;
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
    object_fields[0].field_18 = 1;
    object_fields[1].field_18 = 3;
    object_fields[2].field_18 = 4;
    object_fields[3].field_18 = 3;
    object_fields[4].field_18 = 3;
    object_fields[5].field_18 = 3;
    object_fields[6].field_18 = 7;
    object_fields[7].field_18 = 7;
    object_fields[8].field_18 = 7;
    object_fields[9].field_18 = 3;
    object_fields[10].field_18 = 3;
    object_fields[11].field_18 = 3;
    object_fields[12].field_18 = 3;
    object_fields[13].field_18 = 3;
    object_fields[14].field_18 = 3;
    object_fields[15].field_18 = 3;
    object_fields[16].field_18 = 7;
    object_fields[17].field_18 = 7;
    object_fields[18].field_18 = 7;
    object_fields[19].field_18 = 3;
    object_fields[20].field_18 = 3;
    object_fields[21].field_18 = 3;
    object_fields[22].field_18 = 3;
    object_fields[23].field_18 = 3;
    object_fields[24].field_18 = 3;
    object_fields[25].field_18 = 3;
    object_fields[26].field_18 = 3;
    object_fields[27].field_18 = 3;
    object_fields[28].field_18 = 3;
    object_fields[30].field_18 = 3;
    object_fields[29].field_18 = 3;
    object_fields[31].field_18 = 5;
    object_fields[32].field_18 = 9;
    object_fields[33].field_18 = 3;
    object_fields[34].field_18 = 3;
    object_fields[35].field_18 = 7;
    object_fields[36].field_18 = 8;
    object_fields[37].field_18 = 2;
    object_fields[38].field_18 = 1;
    object_fields[39].field_18 = 3;
    object_fields[40].field_18 = 3;
    object_fields[41].field_18 = 3;
    object_fields[42].field_18 = 7;
    object_fields[43].field_18 = 8;
    object_fields[44].field_18 = 2;
    object_fields[45].field_18 = 1;
    object_fields[46].field_18 = 3;
    object_fields[47].field_18 = 3;
    object_fields[48].field_18 = 3;
    object_fields[49].field_18 = 3;
    object_fields[50].field_18 = 3;
    object_fields[51].field_18 = 3;
    object_fields[52].field_18 = 7;
    object_fields[53].field_18 = 8;
    object_fields[54].field_18 = 2;
    object_fields[55].field_18 = 1;
    object_fields[56].field_18 = 3;
    object_fields[57].field_18 = 3;
    object_fields[58].field_18 = 3;
    object_fields[59].field_18 = 3;
    object_fields[60].field_18 = 13;
    object_fields[61].field_18 = 3;
    object_fields[62].field_18 = 3;
    object_fields[63].field_18 = 3;
    object_fields[64].field_18 = 3;
    object_fields[65].field_18 = 7;
    object_fields[66].field_18 = 8;
    object_fields[67].field_18 = 2;
    object_fields[68].field_18 = 1;
    object_fields[69].field_18 = 3;
    object_fields[70].field_18 = 12;
    object_fields[71].field_18 = 3;
    object_fields[72].field_18 = 3;
    object_fields[73].field_18 = 7;
    object_fields[74].field_18 = 8;
    object_fields[75].field_18 = 2;
    object_fields[76].field_18 = 1;
    object_fields[77].field_18 = 3;
    object_fields[78].field_18 = 3;
    object_fields[79].field_18 = 3;
    object_fields[80].field_18 = 12;
    object_fields[81].field_18 = 3;
    object_fields[82].field_18 = 3;
    object_fields[83].field_18 = 7;
    object_fields[84].field_18 = 8;
    object_fields[85].field_18 = 2;
    object_fields[86].field_18 = 1;
    object_fields[87].field_18 = 3;
    object_fields[88].field_18 = 12;
    object_fields[89].field_18 = 3;
    object_fields[90].field_18 = 3;
    object_fields[91].field_18 = 3;
    object_fields[92].field_18 = 3;
    object_fields[93].field_18 = 3;
    object_fields[94].field_18 = 3;
    object_fields[95].field_18 = 3;
    object_fields[96].field_18 = 3;
    object_fields[97].field_18 = 3;
    object_fields[98].field_18 = 3;
    object_fields[99].field_18 = 3;
    object_fields[100].field_18 = 3;
    object_fields[101].field_18 = 3;
    object_fields[102].field_18 = 3;
    object_fields[103].field_18 = 3;
    object_fields[104].field_18 = 3;
    object_fields[105].field_18 = 3;
    object_fields[106].field_18 = 3;
    object_fields[107].field_18 = 3;
    object_fields[108].field_18 = 7;
    object_fields[109].field_18 = 8;
    object_fields[110].field_18 = 2;
    object_fields[111].field_18 = 1;
    object_fields[112].field_18 = 3;
    object_fields[113].field_18 = 3;
    object_fields[114].field_18 = 3;
    object_fields[115].field_18 = 3;
    object_fields[116].field_18 = 5;
    object_fields[117].field_18 = 5;
    object_fields[118].field_18 = 5;
    object_fields[119].field_18 = 3;
    object_fields[120].field_18 = 3;
    object_fields[121].field_18 = 3;
    object_fields[122].field_18 = 3;
    object_fields[123].field_18 = 3;
    object_fields[124].field_18 = 3;
    object_fields[125].field_18 = 3;
    object_fields[126].field_18 = 3;
    object_fields[127].field_18 = 3;
    object_fields[128].field_18 = 3;
    object_fields[129].field_18 = 3;
    object_fields[130].field_18 = 3;
    object_fields[131].field_18 = 3;
    object_fields[132].field_18 = 3;
    object_fields[133].field_18 = 3;
    object_fields[134].field_18 = 3;
    object_fields[135].field_18 = 3;
    object_fields[136].field_18 = 3;
    object_fields[137].field_18 = 7;
    object_fields[138].field_18 = 8;
    object_fields[139].field_18 = 2;
    object_fields[140].field_18 = 1;
    object_fields[141].field_18 = 3;
    object_fields[142].field_18 = 3;
    object_fields[143].field_18 = 3;
    object_fields[144].field_18 = 3;
    object_fields[145].field_18 = 3;
    object_fields[146].field_18 = 7;
    object_fields[147].field_18 = 8;
    object_fields[148].field_18 = 2;
    object_fields[149].field_18 = 1;
    object_fields[150].field_18 = 3;
    object_fields[151].field_18 = 3;
    object_fields[152].field_18 = 3;
    object_fields[153].field_18 = 3;
    object_fields[154].field_18 = 5;
    object_fields[155].field_18 = 5;
    object_fields[156].field_18 = 3;
    object_fields[157].field_18 = 3;
    object_fields[158].field_18 = 3;
    object_fields[159].field_18 = 3;
    object_fields[160].field_18 = 7;
    object_fields[161].field_18 = 8;
    object_fields[162].field_18 = 2;
    object_fields[163].field_18 = 1;
    object_fields[164].field_18 = 3;
    object_fields[165].field_18 = 3;
    object_fields[166].field_18 = 3;
    object_fields[167].field_18 = 3;
    object_fields[168].field_18 = 7;
    object_fields[169].field_18 = 8;
    object_fields[170].field_18 = 2;
    object_fields[171].field_18 = 1;
    object_fields[172].field_18 = 3;
    object_fields[173].field_18 = 3;
    object_fields[174].field_18 = 3;
    object_fields[175].field_18 = 7;
    object_fields[176].field_18 = 8;
    object_fields[177].field_18 = 2;
    object_fields[178].field_18 = 1;
    object_fields[179].field_18 = 3;
    object_fields[180].field_18 = 3;
    object_fields[181].field_18 = 3;
    object_fields[182].field_18 = 7;
    object_fields[183].field_18 = 8;
    object_fields[184].field_18 = 2;
    object_fields[185].field_18 = 1;
    object_fields[186].field_18 = 3;
    object_fields[187].field_18 = 3;
    object_fields[188].field_18 = 3;
    object_fields[189].field_18 = 7;
    object_fields[190].field_18 = 8;
    object_fields[191].field_18 = 2;
    object_fields[192].field_18 = 1;
    object_fields[193].field_18 = 3;
    object_fields[194].field_18 = 7;
    object_fields[195].field_18 = 3;
    object_fields[196].field_18 = 3;
    object_fields[197].field_18 = 7;
    object_fields[198].field_18 = 8;
    object_fields[199].field_18 = 2;
    object_fields[200].field_18 = 1;
    object_fields[201].field_18 = 3;
    object_fields[202].field_18 = 3;
    object_fields[203].field_18 = 3;
    object_fields[204].field_18 = 3;
    object_fields[205].field_18 = 3;
    object_fields[206].field_18 = 3;
    object_fields[207].field_18 = 7;
    object_fields[208].field_18 = 8;
    object_fields[209].field_18 = 2;
    object_fields[210].field_18 = 1;
    object_fields[211].field_18 = 3;
    object_fields[212].field_18 = 3;
    object_fields[213].field_18 = 3;
    object_fields[214].field_18 = 7;
    object_fields[215].field_18 = 8;
    object_fields[216].field_18 = 2;
    object_fields[217].field_18 = 1;
    object_fields[218].field_18 = 3;
    object_fields[219].field_18 = 3;
    object_fields[220].field_18 = 5;
    object_fields[221].field_18 = 5;
    object_fields[222].field_18 = 5;
    object_fields[223].field_18 = 7;
    object_fields[224].field_18 = 3;
    object_fields[225].field_18 = 3;
    object_fields[226].field_18 = 3;
    object_fields[227].field_18 = 3;
    object_fields[228].field_18 = 7;
    object_fields[229].field_18 = 7;
    object_fields[230].field_18 = 12;
    object_fields[231].field_18 = 3;
    object_fields[232].field_18 = 12;
    object_fields[233].field_18 = 12;
    object_fields[234].field_18 = 12;
    object_fields[235].field_18 = 12;
    object_fields[236].field_18 = 12;
    object_fields[237].field_18 = 3;
    object_fields[238].field_18 = 13;
    object_fields[239].field_18 = 3;
    object_fields[240].field_18 = 3;
    object_fields[241].field_18 = 13;
    object_fields[242].field_18 = 4;
    object_fields[243].field_18 = 3;
    object_fields[244].field_18 = 3;
    object_fields[245].field_18 = 3;
    object_fields[246].field_18 = 3;
    object_fields[247].field_18 = 3;
    object_fields[248].field_18 = 3;
    object_fields[249].field_18 = 7;
    object_fields[250].field_18 = 8;
    object_fields[251].field_18 = 2;
    object_fields[252].field_18 = 1;
    object_fields[253].field_18 = 3;
    object_fields[254].field_18 = 3;
    object_fields[255].field_18 = 7;
    object_fields[257].field_18 = 3;
    object_fields[258].field_18 = 3;
    object_fields[256].field_18 = 8;
    object_fields[257].field_18 = 3;
    object_fields[259].field_18 = 10;
    object_fields[260].field_18 = 7;
    object_fields[261].field_18 = 8;
    object_fields[262].field_18 = 7;
    object_fields[263].field_18 = 8;
    object_fields[264].field_18 = 3;
    object_fields[265].field_18 = 8;
    object_fields[266].field_18 = 7;
    object_fields[267].field_18 = 7;
    object_fields[268].field_18 = 7;
    object_fields[269].field_18 = 3;
    object_fields[270].field_18 = 11;
    object_fields[271].field_18 = 3;
    object_fields[272].field_18 = 7;
    object_fields[273].field_18 = 7;
    object_fields[274].field_18 = 3;
    object_fields[275].field_18 = 3;
    object_fields[276].field_18 = 7;
    object_fields[277].field_18 = 8;
    object_fields[278].field_18 = 2;
    object_fields[279].field_18 = 1;
    object_fields[280].field_18 = 3;
    object_fields[281].field_18 = 12;
    object_fields[282].field_18 = 3;
    object_fields[283].field_18 = 12;
    object_fields[284].field_18 = 12;
    object_fields[285].field_18 = 3;
    object_fields[286].field_18 = 3;
    object_fields[287].field_18 = 8;
    object_fields[288].field_18 = 3;
    object_fields[289].field_18 = 4;
    object_fields[290].field_18 = 4;
    object_fields[291].field_18 = 3;
    object_fields[292].field_18 = 3;
    object_fields[293].field_18 = 3;
    object_fields[294].field_18 = 12;
    object_fields[295].field_18 = 3;
    object_fields[296].field_18 = 3;
    object_fields[297].field_18 = 13;
    object_fields[298].field_18 = 7;
    object_fields[299].field_18 = 7;
    object_fields[300].field_18 = 3;
    object_fields[301].field_18 = 3;
    object_fields[302].field_18 = 3;
    object_fields[303].field_18 = 7;
    object_fields[304].field_18 = 13;
    object_fields[305].field_18 = 2;
    object_fields[306].field_18 = 1;
    object_fields[307].field_18 = 3;
    object_fields[308].field_18 = 3;
    object_fields[309].field_18 = 3;
    object_fields[310].field_18 = 7;
    object_fields[311].field_18 = 8;
    object_fields[312].field_18 = 2;
    object_fields[313].field_18 = 0;
    object_fields[315].field_18 = 3;
    object_fields[316].field_18 = 3;
    object_fields[317].field_18 = 3;
    object_fields[318].field_18 = 3;
    object_fields[319].field_18 = 5;
    object_fields[320].field_18 = 3;
    object_fields[321].field_18 = 3;
    object_fields[322].field_18 = 3;
    object_fields[323].field_18 = 3;
    object_fields[324].field_18 = 3;
    object_fields[325].field_18 = 3;
    object_fields[326].field_18 = 3;
    object_fields[327].field_18 = 3;
    object_fields[328].field_18 = 3;
    object_fields[329].field_18 = 3;
    object_fields[330].field_18 = 7;
    object_fields[331].field_18 = 5;
    object_fields[332].field_18 = 3;
    object_fields[333].field_18 = 3;
    object_fields[335].field_18 = 3;
    object_fields[336].field_18 = 12;
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
