#include "game/lib/light_scheme.h"

#include "tig/color.h"
#include "tig/str_parse.h"

typedef struct LightSchemeData {
    int field_0;
    int field_4;
    int field_8;
    int field_C;
    int field_10;
    int field_14;
};

static_assert(sizeof(LightSchemeData) == 0x18, "wrong size");

// 0x5B5050
static int dword_5B5050 = -1;

// 0x5B5054
static int dword_5B5054 = 1;

// 0x5F5CA0
static LightSchemeData* light_schemes;

// 0x5F5CA4
static int light_scheme_current;

// 0x5F5CA8
static int dword_5F5CA8;

// 0x5F5CAC
static bool light_scheme_changing;

static void light_scheme_parse(int message_list_handle);

// 0x4A7C60
bool light_scheme_init(GameContext* ctx)
{
    // FIXME: Params should be swapped.
    light_schemes = (LightSchemeData*)calloc(sizeof(*light_schemes), LIGHT_SCHEME_MAX);

    return true;
}

// 0x4A7C80
void light_scheme_reset()
{
    sub_4A7DD0(0, 12);
}

// 0x4A7C90
bool light_scheme_mod_load()
{
    if (!message_load("Rules\\Lighting Schemes.mes", &dword_5F5CA8)) {
        return false;
    }

    sub_4A7DD0(0, 12);

    return true;
}

// 0x4A7CC0
void light_scheme_mod_unload()
{
    if (dword_5F5CA8 != -1) {
        message_unload(dword_5F5CA8);
        dword_5F5CA8 = -1;
    }

    dword_5B5050 = -1;
}

// 0x4A7CF0
void light_scheme_exit()
{
    free(light_schemes);
}

// 0x4A7D00
bool light_scheme_save(TigFile* stream)
{
    if (tig_file_fwrite(&dword_5B5050, sizeof(dword_5B5050), 1, stream) != 1) return false;
    if (tig_file_fwrite(&light_scheme_current, sizeof(light_scheme_current), 1, stream) != 1) return false;

    return true;
}

// 0x4A7D40
bool light_scheme_load(LoadContext* ctx)
{
    int v1;
    int v2;

    if (tig_file_fread(&v1, sizeof(&v1), 1, ctx->stream) != 1) return false;
    if (tig_file_fread(&v2, sizeof(&v2), 1, ctx->stream) != 1) return false;

    sub_4A7DD0(v1, v2);

    return true;
}

// 0x4A7DA0
bool sub_4A7DA0(int a1)
{
    if (a1 > 0) {
        dword_5B5054 = a1;
        return true;
    }

    return false;
}

// 0x4A7DC0
int sub_4A7DC0()
{
    return dword_5B5054;
}

// 0x4A7DD0
bool sub_4A7DD0(int a1, int a2)
{
    if (dword_5F5CA8 == -1) {
        return false;
    }

    if (a1 == 0) {
        a1 = dword_5B5054;
    }

    if (a1 != dword_5B5050) {
        MessageListItem message_list_item;
        message_list_item.num = a1;

        if (sub_4D45E0(dword_5F5CA8, &message_list_item)) {
            char path[MAX_PATH];
            sprintf(path, "Rules\\%s.mes", message_list_item.text);

            int message_list_handle;
            if (message_load(path, &message_list_handle)) {
                light_scheme_parse(message_list_handle);
                message_unload(message_list_handle);
                dword_5B5050 = a1;
            }
        }
    }

    if (a1 != dword_5B5050) {
        return false;
    }

    return light_scheme_set_current(a2);
}

// 0x4A7EA0
int sub_4A7EA0()
{
    return dword_5B5050;
}

// 0x4A7EB0
bool light_scheme_set_current(int a2)
{
    if (dword_5F5CA8 == -1) {
        return false;
    }

    if (a2 < 0 || a2 >= LIGHT_SCHEME_MAX) {
        return false;
    }

    light_scheme_current = a2;

    light_scheme_changing = true;

    LightSchemeData* light_scheme_data = &(light_schemes[a2]);
    int color1 = tig_color_rgb_make(light_scheme_data->field_0, light_scheme_data->field_4, light_scheme_data->field_8);
    int color2 = tig_color_rgb_make(light_scheme_data->field_C, light_scheme_data->field_10, light_scheme_data->field_14);
    sub_4D8560(color1, color2);

    light_scheme_changing = false;

    return true;
}

// 0x4A7FA0
int light_scheme_get_current()
{
    return light_scheme_current;
}

// 0x4A7FB0
int light_scheme_is_changing()
{
    return light_scheme_changing;
}

// 0x4A8060
void light_scheme_parse(int message_list_handle)
{
    bool interpolate = false;
    int last_parsed_index = 0;

    int v10;
    int v11;
    int v12;
    int v13;
    int v14;
    int v15;

    int v16;
    int v17;
    int v18;
    int v19;
    int v20;
    int v21;

    tig_str_parse_set_separator(',');

    for (int index = 0; index < LIGHT_SCHEME_MAX; index++) {
        MessageListItem message_list_item;
        message_list_item.num = index;

        if (sub_4D45E0(message_list_handle, &message_list_item) && message_list_item.text[0] != '\0') {
            char* text = message_list_item.text;
            sub_531990(&text, &v10);
            sub_531990(&text, &v11);
            sub_531990(&text, &v12);
            sub_531990(&text, &v13);
            sub_531990(&text, &v14);
            sub_531990(&text, &v15);

            light_schemes[index].field_C = v10;
            light_schemes[index].field_10 = v11;
            light_schemes[index].field_14 = v12;
            light_schemes[index].field_0 = v13;
            light_schemes[index].field_4 = v14;
            light_schemes[index].field_8 = v15;

            if (interpolate) {
                int v5 = (light_schemes[index].field_C - v16) / (index - last_parsed_index);
                int v22 = (light_schemes[index].field_10 - v17) / (index - last_parsed_index);
                int v23 = (light_schemes[index].field_14 - v18) / (index - last_parsed_index);
                int v24 = (light_schemes[index].field_0 - v19) / (index - last_parsed_index);
                int v25 = (light_schemes[index].field_4 - v20) / (index - last_parsed_index);
                int v6 = (light_schemes[index].field_8 - v21) / (index - last_parsed_index);

                v10 = v16;
                v11 = v17;
                v12 = v18;
                v13 = v19;
                v14 = v20;
                v15 = v21;

                for (int prev = last_parsed_index; prev < index; prev++) {
                    light_schemes[prev].field_C = v10;
                    light_schemes[prev].field_10 = v11;
                    light_schemes[prev].field_14 = v12;
                    light_schemes[prev].field_0 = v13;
                    light_schemes[prev].field_4 = v14;
                    light_schemes[prev].field_8 = v15;

                    v10 += v5;
                    v11 += v22;
                    v12 += v23;
                    v13 += v24;
                    v14 += v25;
                    v15 += v6;
                }

                interpolate = false;
            }

            last_parsed_index = index;
        } else {
            v16 = v10;
            v17 = v11;
            v18 = v12;
            v19 = v13;
            v20 = v14;
            v21 = v15;
            interpolate = true;
        }
    }
}
