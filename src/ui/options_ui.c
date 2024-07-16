#include "ui/options_ui.h"

#include "game/gamelib.h"
#include "game/player.h"
#include "game/text_floater.h"

typedef void(OptionsUiControlValueGetter)(int* value_ptr, bool* enabled_ptr);
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

static void options_ui_module_get(int* value_ptr, bool* enabled_ptr);
static void options_ui_difficulty_get(int* value_ptr, bool* enabled_ptr);
static void options_ui_difficutly_set(int value);
static void options_ui_violence_filter_get(int* value_ptr, bool* enabled_ptr);
static void options_ui_violence_filter_set(int value);
static void options_ui_combat_mode_get(int* value_ptr, bool* enabled_ptr);
static void options_ui_combat_mode_set(int value);
static void options_ui_auto_attack_get(int* value_ptr, bool* enabled_ptr);
static void options_ui_auto_attack_set(int value);
static void options_ui_auto_switch_weapons_get(int* value_ptr, bool* enabled_ptr);
static void options_ui_auto_switch_weapons_set(int value);
static void options_ui_awlays_run_get(int* value_ptr, bool* enabled_ptr);
static void options_ui_always_run_set(int value);
static void options_ui_follower_skills_get(int* value_ptr, bool* enabled_ptr);
static void options_ui_follower_skills_set(int value);
static void options_ui_brightness_get(int* value_ptr, bool* enabled_ptr);
static void options_ui_brightness_set(int value);
static void options_ui_text_duration_get(int* value_ptr, bool* enabled_ptr);
static void options_ui_text_duration_set(int value);
static void options_ui_floats_get(int* value_ptr, int* enabled_ptr);
static void options_ui_floats_set(int value);
static void options_ui_float_speed_get(int* value_ptr, bool* enabled_ptr);
static void options_ui_float_speed_set(int value);
static void options_ui_combat_taunts_get(int* value_ptr, bool* enabled_ptr);
static void options_ui_combat_taunts_set(int value);
static void options_ui_effects_volume_get(int* value_ptr, bool* enabled_ptr);
static void options_ui_effects_volume_set(int value);
static void options_ui_voice_volume_get(int* value_ptr, bool* enabled_ptr);
static void options_ui_voice_volume_set(int value);
static void options_ui_music_volume_get(int* value_ptr, bool* enabled_ptr);
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
        { 1, 1, 0, "Default Combat Mode", "mes\\OptionsCombatMode.mes", options_ui_combat_mode_get, options_ui_combat_mode_get },
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

// 0x589530
int sub_589530(int a1)
{
    return sub_57F6D0(a1);
}

// 0x589540
void options_ui_module_get(int* a1, bool* enabled_ptr)
{
    *a1 = dword_68726C;
    *enabled_ptr = dword_687260 == 0;
}

// 0x589560
void options_ui_difficulty_get(int* value_ptr, bool* enabled_ptr)
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
void options_ui_violence_filter_get(int* value_ptr, bool* enabled_ptr)
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
void options_ui_combat_mode_get(int* value_ptr, bool* enabled_ptr)
{
    if ((tig_net_flags & 0x1) != 0) {
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
    if ((tig_net_flags & 0x1) == 0) {
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
void options_ui_auto_attack_get(int* value_ptr, bool* enabled_ptr)
{
    *value_ptr = sub_4B8230(player_get_pc_obj()) != 0;
    *enabled_ptr = true;
}

// 0x589740
void options_ui_auto_attack_set(int value)
{
    sub_4B8280(value);
}

// 0x589750
void options_ui_auto_switch_weapons_get(int* value_ptr, bool* enabled_ptr)
{
    *value_ptr = sub_4B8330(player_get_pc_obj()) != 0;
    *enabled_ptr = true;
}

// 0x589780
void options_ui_auto_switch_weapons_set(int value)
{
    sub_4B8380(value);
}

// 0x589790
void options_ui_awlays_run_get(int* value_ptr, bool* enabled_ptr)
{
    *value_ptr = sub_4304C0(player_get_pc_obj()) != 0;
    *enabled_ptr = true;
}

// 0x5897C0
void options_ui_always_run_set(int value)
{
    sub_430580(value);
}

// 0x5897D0
void options_ui_follower_skills_get(int* value_ptr, bool* enabled_ptr)
{
    *value_ptr = sub_4C8FA0(player_get_pc_obj()) != 0;
    *enabled_ptr = true;
}

// 0x589800
void options_ui_follower_skills_set(int value)
{
    sub_4C8FF0(value);
}

// 0x589810
void options_ui_brightness_get(int* value_ptr, bool* enabled_ptr)
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
void options_ui_text_duration_get(int* value_ptr, bool* enabled_ptr)
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
void options_ui_floats_get(int* value_ptr, int* enabled_ptr)
{
    *value_ptr = text_floaters_get();
    *enabled_ptr = true;
}

// 0x5898C0
void options_ui_floats_set(int value)
{
    if (text_floaters_get() != value) {
        text_floaters_set(value);
    }
}

// 0x5898E0
void options_ui_float_speed_get(int* value_ptr, bool* enabled_ptr)
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
void options_ui_combat_taunts_get(int* value_ptr, bool* enabled_ptr)
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
void options_ui_effects_volume_get(int* value_ptr, bool* enabled_ptr)
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
void options_ui_voice_volume_get(int* value_ptr, bool* enabled_ptr)
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
void options_ui_music_volume_get(int* value_ptr, bool* enabled_ptr)
{
    *value_ptr = settings_get_value(&settings, "music volume");
    *enabled_ptr = true;
}

// 0x589A00
void options_ui_music_volume_set(int value)
{
    settings_set_value(&settings, "music volume", value);
}
