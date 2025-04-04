#include "ui/options_ui.h"

#include "game/anim.h"
#include "game/combat.h"
#include "game/gamelib.h"
#include "game/location.h"
#include "game/mes.h"
#include "game/obj.h"
#include "game/player.h"
#include "game/skill.h"
#include "game/tf.h"
#include "ui/cyclic_ui.h"
#include "ui/gameuilib.h"

typedef void(OptionsUiControlValueGetter)(int* value_ptr, unsigned char* enabled_ptr);
typedef void(OptionsUiControlValueSetter)(int value);

typedef struct OptionsUiControlInfo {
    /* 0000 */ int field_0;
    /* 0004 */ int field_4;
    /* 0008 */ int field_8;
    /* 000C */ const char* name;
    /* 0010 */ const char* mes;
    /* 0014 */ OptionsUiControlValueGetter* getter;
    /* 0018 */ OptionsUiControlValueSetter* setter;
} OptionsUiControlInfo;

static_assert(sizeof(OptionsUiControlInfo) == 0x1C, "wrong size");

typedef struct OptionsUiControlData {
    int id;
    bool initialized;
} OptionsUiControlData;

static void options_ui_module_get(int* value_ptr, unsigned char* enabled_ptr);
static void options_ui_difficulty_get(int* value_ptr, unsigned char* enabled_ptr);
static void options_ui_difficutly_set(int value);
static void options_ui_violence_filter_get(int* value_ptr, unsigned char* enabled_ptr);
static void options_ui_violence_filter_set(int value);
static void options_ui_combat_mode_get(int* value_ptr, unsigned char* enabled_ptr);
static void options_ui_combat_mode_set(int value);
static void options_ui_auto_attack_get(int* value_ptr, unsigned char* enabled_ptr);
static void options_ui_auto_attack_set(int value);
static void options_ui_auto_switch_weapons_get(int* value_ptr, unsigned char* enabled_ptr);
static void options_ui_auto_switch_weapons_set(int value);
static void options_ui_awlays_run_get(int* value_ptr, unsigned char* enabled_ptr);
static void options_ui_always_run_set(int value);
static void options_ui_follower_skills_get(int* value_ptr, unsigned char* enabled_ptr);
static void options_ui_follower_skills_set(int value);
static void options_ui_brightness_get(int* value_ptr, unsigned char* enabled_ptr);
static void options_ui_brightness_set(int value);
static void options_ui_text_duration_get(int* value_ptr, unsigned char* enabled_ptr);
static void options_ui_text_duration_set(int value);
static void options_ui_floats_get(int* value_ptr, unsigned char* enabled_ptr);
static void options_ui_floats_set(int value);
static void options_ui_float_speed_get(int* value_ptr, unsigned char* enabled_ptr);
static void options_ui_float_speed_set(int value);
static void options_ui_combat_taunts_get(int* value_ptr, unsigned char* enabled_ptr);
static void options_ui_combat_taunts_set(int value);
static void options_ui_effects_volume_get(int* value_ptr, unsigned char* enabled_ptr);
static void options_ui_effects_volume_set(int value);
static void options_ui_voice_volume_get(int* value_ptr, unsigned char* enabled_ptr);
static void options_ui_voice_volume_set(int value);
static void options_ui_music_volume_get(int* value_ptr, unsigned char* enabled_ptr);
static void options_ui_music_volume_set(int value);

// 0x5CCD38
static int dword_5CCD38[8] = {
    261,
    261,
    261,
    261,
    531,
    531,
    531,
    531,
};

// 0x5CCD58
static int dword_5CCD58[8] = {
    122,
    197,
    272,
    347,
    122,
    197,
    272,
    347,
};

// 0x5CCD78
static OptionsUiControlInfo stru_5CCD78[3][8] = {
    {
        { 1, 2, 0, "Module", "", options_ui_module_get, NULL },
        { 1, 1, 0, "Difficulty", "mes\\OptionsDifficulty.mes", options_ui_difficulty_get, options_ui_difficutly_set },
        { 1, 1, 0, "Violence Filter", "mes\\OptionsOffOn.mes", options_ui_violence_filter_get, options_ui_violence_filter_set },
        { 1, 1, 0, "Default Combat Mode", "mes\\OptionsCombatMode.mes", options_ui_combat_mode_get, options_ui_combat_mode_set },
        { 1, 1, 0, "Auto Attack", "mes\\OptionsOffOn.mes", options_ui_auto_attack_get, options_ui_auto_attack_set },
        { 1, 1, 0, "Auto Switch Weapons", "mes\\OptionsOffOn.mes", options_ui_auto_switch_weapons_get, options_ui_auto_switch_weapons_set },
        { 1, 1, 0, "Always Run", "mes\\OptionsOffOn.mes", options_ui_awlays_run_get, options_ui_always_run_set },
        { 1, 1, 0, "Follower Skills", "mes\\OptionsOffOn.mes", options_ui_follower_skills_get, options_ui_follower_skills_set },
    },
    {
        { 1, 0, 9, "Brightness", "", options_ui_brightness_get, options_ui_brightness_set },
        { 1, 0, 0, "Text Duration", "", options_ui_text_duration_get, options_ui_text_duration_set },
        { 1, 1, 0, "Floats", "mes\\OptionsFloats.mes", options_ui_floats_get, options_ui_floats_set },
        { 1, 0, 0, "Float Speed", "", options_ui_float_speed_get, options_ui_float_speed_set },
        { 1, 1, 0, "Combat Taunts", "mes\\OptionsOffOn.mes", options_ui_combat_taunts_get, options_ui_combat_taunts_set },
        { 0, 0, 0, "", "", NULL, NULL },
        { 0, 0, 0, "", "", NULL, NULL },
        { 0, 0, 0, "", "", NULL, NULL },
    },
    {
        { 1, 0, 0, "Effects", "", options_ui_effects_volume_get, options_ui_effects_volume_set },
        { 1, 0, 0, "Voice", "", options_ui_voice_volume_get, options_ui_voice_volume_set },
        { 1, 0, 0, "Music", "", options_ui_music_volume_get, options_ui_music_volume_set },
        { 0, 0, 0, "", "", NULL, NULL },
        { 0, 0, 0, "", "", NULL, NULL },
        { 0, 0, 0, "", "", NULL, NULL },
        { 0, 0, 0, "", "", NULL, NULL },
        { 0, 0, 0, "", "", NULL, NULL },
    },
};

// 0x687260
static bool dword_687260;

// 0x687268
static GameModuleList options_ui_modlist;

// 0x687274
static mes_file_handle_t options_ui_mes_file;

// 0x687278
static OptionsUiControlData stru_687278[8];

// 0x6872B8
static bool options_ui_initialized;

// 0x6872BC
static bool options_ui_modlist_initialized;

// 0x589280
void options_ui_init(int type, tig_window_handle_t window_handle, bool a3)
{
    int index;
    CyclicUiControlInfo control_info;
    OptionsUiControlInfo* meta;
    MesFileEntry mes_file_entry;
    int value;

    if (options_ui_initialized) {
        options_ui_exit();
    }

    dword_687260 = a3;

    mes_load("mes\\options_ui.mes", &options_ui_mes_file);

    for (index = 0; index < 8; index++) {
        meta = &(stru_5CCD78[type][index]);
        if (!meta->field_0) {
            continue;
        }

        cyclic_ui_control_init(&control_info);

        if (type == 0 && index == 0) {
            if (dword_687260) {
                options_ui_modlist_initialized = false;
                continue;
            }

            gamelib_modlist_create(&options_ui_modlist, 0);
            control_info.text_array = options_ui_modlist.paths;
            control_info.text_array_size = options_ui_modlist.count;
            options_ui_modlist_initialized = true;
        }

        control_info.type = meta->field_4;
        control_info.window_handle = window_handle;
        control_info.max_value = meta->field_8;

        mes_file_entry.num = index + 20 * (5 * type + 5);
        control_info.text = mes_search(options_ui_mes_file, &mes_file_entry)
            ? mes_file_entry.str
            : NULL;

        control_info.mes_file_path = meta->mes;
        control_info.value_changed_callback = meta->setter;
        control_info.x = dword_5CCD38[index];
        control_info.y = dword_5CCD58[index];

        if (meta->getter != NULL) {
            meta->getter(&value, &(control_info.enabled));
        } else {
            value = 0;
            control_info.enabled = false;
        }

        if (cyclic_ui_control_create(&control_info, &(stru_687278[index].id))) {
            stru_687278[index].initialized = true;
            cyclic_ui_control_set(stru_687278[index].id, value);
        } else {
            stru_687278[index].initialized = false;
        }
    }

    if (dword_687260) {
        location_origin_set(obj_field_int64_get(player_get_local_pc_obj(), OBJ_F_LOCATION));
    }

    options_ui_initialized = true;
}

// 0x589430
bool sub_589430()
{
    unsigned int selected;

    if (!options_ui_initialized
        || !options_ui_modlist_initialized
        || dword_687260) {
        return true;
    }

    selected = cyclic_ui_control_get(stru_687278[0].id);
    if (selected == options_ui_modlist.selected) {
        return true;
    }

    if (!gamelib_mod_load(options_ui_modlist.paths[selected])
        || !gameuilib_mod_load()) {
        tig_debug_printf("Can't load module %s\n", options_ui_modlist.paths[selected]);
        return false;
    }

    return true;
}

// 0x5894C0
void options_ui_exit()
{
    int index;

    if (!options_ui_initialized) {
        return;
    }

    for (index = 0; index < 8; index++) {
        if (stru_687278[index].initialized) {
            cyclic_ui_control_destroy(stru_687278[index].id, false);
            stru_687278[index].initialized = false;
        }
    }

    if (options_ui_modlist_initialized) {
        gamelib_modlist_destroy(&options_ui_modlist);
        options_ui_modlist_initialized = false;
    }

    mes_unload(options_ui_mes_file);

    options_ui_initialized = false;
}

// 0x589530
int sub_589530(int a1)
{
    return sub_57F6D0(a1);
}

// 0x589540
void options_ui_module_get(int* value_ptr, unsigned char* enabled_ptr)
{
    *value_ptr = options_ui_modlist.selected;
    *enabled_ptr = !dword_687260;
}

// 0x589560
void options_ui_difficulty_get(int* value_ptr, unsigned char* enabled_ptr)
{
    *value_ptr = settings_get_value(&settings, "difficulty");
    *enabled_ptr = true;
}

// 0x589580
void options_ui_difficutly_set(int value)
{
    settings_set_value(&settings, "difficulty", value);
}

// 0x5895A0
void options_ui_violence_filter_get(int* value_ptr, unsigned char* enabled_ptr)
{
    *value_ptr = settings_get_value(&settings, "violence filter");
    *enabled_ptr = true;
}

// 0x5895D0
void options_ui_violence_filter_set(int value)
{
    settings_set_value(&settings, "difficulty", value ? 1 : 0);
}

// 0x589610
void options_ui_combat_mode_get(int* value_ptr, unsigned char* enabled_ptr)
{
    if (tig_net_is_active()) {
        if (combat_is_turn_based()) {
            settings_set_value(&settings, "turn-based", 0);
            settings_set_value(&settings, "fast turn-based", 0);
        }

        *value_ptr = 0;
        *enabled_ptr = false;
    } else {
        if (combat_is_turn_based()) {
            *value_ptr = settings_get_value(&settings, "fast turn-based") ? 2 : 1;
        } else {
            *value_ptr = 0;
        }
        *enabled_ptr = true;
    }
}

// 0x5896A0
void options_ui_combat_mode_set(int value)
{
    if (!tig_net_is_active()) {
        switch (value) {
        case 0:
            settings_set_value(&settings, "turn-based", 0);
            settings_set_value(&settings, "fast turn-based", 0);
            break;
        case 1:
            settings_set_value(&settings, "turn-based", 1);
            settings_set_value(&settings, "fast turn-based", 0);
            break;
        case 2:
            settings_set_value(&settings, "turn-based", 1);
            settings_set_value(&settings, "fast turn-based", 1);
            break;
        }
    }
}

// 0x589710
void options_ui_auto_attack_get(int* value_ptr, unsigned char* enabled_ptr)
{
    *value_ptr = combat_auto_attack_get(player_get_local_pc_obj());
    *enabled_ptr = true;
}

// 0x589740
void options_ui_auto_attack_set(int value)
{
    combat_auto_attack_set(value);
}

// 0x589750
void options_ui_auto_switch_weapons_get(int* value_ptr, unsigned char* enabled_ptr)
{
    *value_ptr = combat_auto_switch_weapons_get(player_get_local_pc_obj());
    *enabled_ptr = true;
}

// 0x589780
void options_ui_auto_switch_weapons_set(int value)
{
    combat_auto_switch_weapons_set(value);
}

// 0x589790
void options_ui_awlays_run_get(int* value_ptr, unsigned char* enabled_ptr)
{
    *value_ptr = get_always_run(player_get_local_pc_obj());
    *enabled_ptr = true;
}

// 0x5897C0
void options_ui_always_run_set(int value)
{
    set_always_run(value);
}

// 0x5897D0
void options_ui_follower_skills_get(int* value_ptr, unsigned char* enabled_ptr)
{
    *value_ptr = get_follower_skills(player_get_local_pc_obj());
    *enabled_ptr = true;
}

// 0x589800
void options_ui_follower_skills_set(int value)
{
    set_follower_skills(value);
}

// 0x589810
void options_ui_brightness_get(int* value_ptr, unsigned char* enabled_ptr)
{
    *value_ptr = settings_get_value(&settings, "brightness");
    *enabled_ptr = tig_video_check_gamma_control() == TIG_OK;
}

// 0x589840
void options_ui_brightness_set(int value)
{
    settings_set_value(&settings, "brightness", value);
}

// 0x589860
void options_ui_text_duration_get(int* value_ptr, unsigned char* enabled_ptr)
{
    *value_ptr = settings_get_value(&settings, "text duration");
    *enabled_ptr = true;
}

// 0x589880
void options_ui_text_duration_set(int value)
{
    settings_set_value(&settings, "text duration", value);
}

// 0x5898A0
void options_ui_floats_get(int* value_ptr, unsigned char* enabled_ptr)
{
    *value_ptr = tf_level_get();
    *enabled_ptr = true;
}

// 0x5898C0
void options_ui_floats_set(int value)
{
    if (tf_level_get() != value) {
        tf_level_set(value);
    }
}

// 0x5898E0
void options_ui_float_speed_get(int* value_ptr, unsigned char* enabled_ptr)
{
    *value_ptr = settings_get_value(&settings, "float speed");
    *enabled_ptr = true;
}

// 0x589900
void options_ui_float_speed_set(int value)
{
    settings_set_value(&settings, "float speed", value);
}

// 0x589920
void options_ui_combat_taunts_get(int* value_ptr, unsigned char* enabled_ptr)
{
    *value_ptr = settings_get_value(&settings, "combat taunts");
    *enabled_ptr = true;
}

// 0x589940
void options_ui_combat_taunts_set(int value)
{
    settings_set_value(&settings, "combat taunts", value);
}

// 0x589960
void options_ui_effects_volume_get(int* value_ptr, unsigned char* enabled_ptr)
{
    *value_ptr = settings_get_value(&settings, "effects volume");
    *enabled_ptr = true;
}

// 0x589980
void options_ui_effects_volume_set(int value)
{
    settings_set_value(&settings, "effects volume", value);
}

// 0x5899A0
void options_ui_voice_volume_get(int* value_ptr, unsigned char* enabled_ptr)
{
    *value_ptr = settings_get_value(&settings, "voice volume");
    *enabled_ptr = true;
}

// 0x5899C0
void options_ui_voice_volume_set(int value)
{
    settings_set_value(&settings, "voice volume", value);
}

// 0x5899E0
void options_ui_music_volume_get(int* value_ptr, unsigned char* enabled_ptr)
{
    *value_ptr = settings_get_value(&settings, "music volume");
    *enabled_ptr = true;
}

// 0x589A00
void options_ui_music_volume_set(int value)
{
    settings_set_value(&settings, "music volume", value);
}
