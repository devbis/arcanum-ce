#include "game/lib/light_scheme.h"

#include <stdio.h>

#include "game/lib/message.h"
#include "tig/color.h"
#include "tig/str_parse.h"

// A lighting scheme contains exactly 24 values - for each hour of the day.
#define LIGHT_SCHEME_HOURS 24

// NOTE: There are no traces of such struct in the code. However it's handy
// as it reduces number of assignments in `light_scheme_parse` significanlty
// and improves code readability.
typedef struct LightSchemeColorComponents {
    int red;
    int green;
    int blue;
};

typedef struct LightSchemeData {
    LightSchemeColorComponents indoor;
    LightSchemeColorComponents outdoor;
};

// See 0x4A7C60.
static_assert(sizeof(LightSchemeData) == 0x18, "wrong size");

// 0x5B5050
static int light_scheme_current_light_scheme_id = -1;

// 0x5B5054
static int light_scheme_map_default = LIGHT_SCHEME_DEFAULT_LIGHTING;

// 0x5F5CA0
static LightSchemeData* light_scheme_colors;

// 0x5F5CA4
static int light_scheme_hour;

// 0x5F5CA8
static int light_schemes_msg_file;

// 0x5F5CAC
static bool light_scheme_changing;

static void light_scheme_parse(int msg_file);

// 0x4A7C60
bool light_scheme_init(GameContext* ctx)
{
    // FIXME: Params should be swapped.
    light_scheme_colors = (LightSchemeData*)calloc(sizeof(*light_scheme_colors), LIGHT_SCHEME_HOURS);

    return true;
}

// 0x4A7C80
void light_scheme_reset()
{
    light_scheme_set(LIGHT_SCHEME_MAP_DEFAULT, 12);
}

// 0x4A7C90
bool light_scheme_mod_load()
{
    if (!message_load("Rules\\Lighting Schemes.mes", &light_schemes_msg_file)) {
        return false;
    }

    light_scheme_set(LIGHT_SCHEME_MAP_DEFAULT, 12);

    return true;
}

// 0x4A7CC0
void light_scheme_mod_unload()
{
    if (light_schemes_msg_file != -1) {
        message_unload(light_schemes_msg_file);
        light_schemes_msg_file = -1;
    }

    light_scheme_current_light_scheme_id = -1;
}

// 0x4A7CF0
void light_scheme_exit()
{
    free(light_scheme_colors);
}

// 0x4A7D00
bool light_scheme_save(TigFile* stream)
{
    if (tig_file_fwrite(&light_scheme_current_light_scheme_id, sizeof(light_scheme_current_light_scheme_id), 1, stream) != 1) return false;
    if (tig_file_fwrite(&light_scheme_hour, sizeof(light_scheme_hour), 1, stream) != 1) return false;

    return true;
}

// 0x4A7D40
bool light_scheme_load(LoadContext* ctx)
{
    int light_scheme;
    int hour;

    if (tig_file_fread(&light_scheme, sizeof(&light_scheme), 1, ctx->stream) != 1) return false;
    if (tig_file_fread(&hour, sizeof(&hour), 1, ctx->stream) != 1) return false;

    light_scheme_set(light_scheme, hour);

    return true;
}

// 0x4A7DA0
bool light_scheme_set_map_default(int light_scheme)
{
    if (light_scheme > 0) {
        light_scheme_map_default = light_scheme;
        return true;
    }

    return false;
}

// 0x4A7DC0
int light_scheme_get_map_default()
{
    return light_scheme_map_default;
}

// 0x4A7DD0
bool light_scheme_set(int light_scheme, int hour)
{
    if (light_schemes_msg_file == -1) {
        return false;
    }

    if (light_scheme == LIGHT_SCHEME_MAP_DEFAULT) {
        light_scheme = light_scheme_map_default;
    }

    if (light_scheme != light_scheme_current_light_scheme_id) {
        MessageListItem message_list_item;
        message_list_item.num = light_scheme;

        if (message_find(light_schemes_msg_file, &message_list_item)) {
            char path[MAX_PATH];
            sprintf(path, "Rules\\%s.mes", message_list_item.text);

            int message_list_handle;
            if (message_load(path, &message_list_handle)) {
                light_scheme_parse(message_list_handle);
                message_unload(message_list_handle);
                light_scheme_current_light_scheme_id = light_scheme;
            }
        }
    }

    if (light_scheme != light_scheme_current_light_scheme_id) {
        return false;
    }

    return light_scheme_set_hour(hour);
}

// 0x4A7EA0
int light_scheme_get()
{
    return light_scheme_current_light_scheme_id;
}

// 0x4A7EB0
bool light_scheme_set_hour(int hour)
{
    if (light_schemes_msg_file == -1) {
        return false;
    }

    if (hour < 0 || hour >= LIGHT_SCHEME_HOURS) {
        return false;
    }

    light_scheme_hour = hour;

    light_scheme_changing = true;

    LightSchemeData* light_scheme_data = &(light_scheme_colors[hour]);
    int indoor_color = tig_color_rgb_make(light_scheme_data->indoor.red, light_scheme_data->indoor.green, light_scheme_data->indoor.blue);
    int outdoor_color = tig_color_rgb_make(light_scheme_data->outdoor.red, light_scheme_data->outdoor.green, light_scheme_data->outdoor.blue);
    sub_4D8560(indoor_color, outdoor_color);

    light_scheme_changing = false;

    return true;
}

// 0x4A7FA0
int light_scheme_get_hour()
{
    return light_scheme_hour;
}

// 0x4A7FB0
int light_scheme_is_changing()
{
    return light_scheme_changing;
}

// 0x4A8060
void light_scheme_parse(int msg_file)
{
    bool interpolate = false;
    int prev_index = 0;

    LightSchemeColorComponents outdoor;
    LightSchemeColorComponents indoor;
    LightSchemeColorComponents prev_outdoor;
    LightSchemeColorComponents prev_indoor;

    tig_str_parse_set_separator(',');

    for (int index = 0; index < LIGHT_SCHEME_HOURS; index++) {
        MessageListItem message_list_item;
        message_list_item.num = index;

        if (message_find(msg_file, &message_list_item) && message_list_item.text[0] != '\0') {
            char* text = message_list_item.text;
            tig_str_parse_simple_value(&text, &(outdoor.red));
            tig_str_parse_simple_value(&text, &(outdoor.green));
            tig_str_parse_simple_value(&text, &(outdoor.blue));
            tig_str_parse_simple_value(&text, &(indoor.red));
            tig_str_parse_simple_value(&text, &(indoor.green));
            tig_str_parse_simple_value(&text, &(indoor.blue));

            light_scheme_colors[index].outdoor = outdoor;
            light_scheme_colors[index].indoor = indoor;

            if (interpolate) {
                int outdoor_red_step = (outdoor.red - prev_outdoor.red) / (index - prev_index);
                int outdoor_green_step = (outdoor.green - prev_outdoor.green) / (index - prev_index);
                int outdoor_blue_step = (outdoor.blue - prev_outdoor.blue) / (index - prev_index);
                int indoor_red_step = (indoor.red - prev_indoor.red) / (index - prev_index);
                int indoor_green_step = (indoor.green - prev_indoor.green) / (index - prev_index);
                int indoor_blue_step = (indoor.blue - prev_indoor.blue) / (index - prev_index);

                outdoor = prev_outdoor;
                indoor = prev_indoor;

                for (int prev = prev_index; prev < index; prev++) {
                    light_scheme_colors[prev].outdoor = outdoor;
                    light_scheme_colors[prev].indoor = indoor;

                    outdoor.red += outdoor_red_step;
                    outdoor.green += outdoor_green_step;
                    outdoor.blue += outdoor_blue_step;
                    indoor.red += indoor_red_step;
                    indoor.green += indoor_green_step;
                    indoor.blue += indoor_blue_step;
                }

                interpolate = false;
            }

            prev_index = index;
        } else {
            prev_outdoor = outdoor;
            prev_indoor = indoor;
            interpolate = true;
        }
    }
}
