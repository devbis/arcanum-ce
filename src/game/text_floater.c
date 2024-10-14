#include "game/text_floater.h"

#include "game/gamelib.h"

#define TEXT_FLOATERS_KEY "text floaters"
#define FLOAT_SPEED_KEY "float speed"

#define FIVE 5

typedef struct TextFloater6028F0 {
    int field_0;
    TigVideoBuffer* video_buffer;
    int field_8;
    int field_C;
    int field_10;
    int field_14;
    int field_18;
    struct TextFloater6028F0* next;
} TextFloater6028F0;

// See 0x4D5950.
static_assert(sizeof(TextFloater6028F0) == 0x20, "wrong size");

typedef struct TextFloater60289C {
    int field_0;
    int field_4;
    int field_8;
    struct TextFloater60289C* next;
} TextFloater60289C;

// See 0x4D57E0.
static_assert(sizeof(TextFloater60289C) == 0x10, "wrong size");

static void sub_4D5780();
static void text_floaters_set_internal(int value);
static void text_floaters_changed();
static void float_speed_changed();

// 0x5B8E6C
static uint8_t byte_5B8E6C[FIVE][3] = {
    { 255, 255, 255 },
    { 255, 0, 0 },
    { 0, 255, 0 },
    { 64, 64, 255 },
    { 255, 255, 0 },
};

// 0x602898
static int dword_602898;

// 0x60289C
static TextFloater60289C* off_60289C;

// 0x6028A0
static int float_speed;

// 0x6028A8
static ViewOptions text_floater_view_options;

// 0x6028B0
static TigRect stru_6028B0;

// 0x6028C0
static int dword_6028C0;

// 0x6028C4
static GameContextF8* dword_6028C4;

// 0x6028C8
static int dword_6028C8;

// 0x6028D0
static TigRect stru_6028D0;

// 0x6028E8
static int dword_6028E8;

// 0x6028EC
static int text_floaters;

// 0x6028F0
static TextFloater6028F0* off_6028F0;

// 0x6028F8
static TigRect stru_6028F8;

// 0x602908
static tig_font_handle_t dword_602908[FIVE];

// 0x4D4E20
bool text_floater_init(GameInitInfo* init_info)
{
    TigWindowData window_data;
    TigFont font;
    TigArtFrameData art_frame_data;
    int index;

    if (tig_window_data(init_info->iso_window_handle, &window_data) != TIG_OK) {
        return false;
    }

    dword_6028E8 = init_info->iso_window_handle;

    stru_6028B0.x = 0;
    stru_6028B0.y = 0;
    stru_6028B0.width = window_data.rect.width;
    stru_6028B0.height = window_data.rect.height;

    text_floater_view_options.type = VIEW_TYPE_ISOMETRIC;
    dword_6028C4 = init_info->field_8;

    dword_6028C8 = tig_color_make(0, 0, 255);
    dword_6028C0 = tig_color_make(0, 0, 0);

    font.flags = TIG_FONT_BLEND_ALPHA_SRC | TIG_FONT_SHADOW;
    tig_art_interface_id_create(229, 0, 0, 0, &(font.art_id));

    tig_art_frame_data(font.art_id, &art_frame_data);
    dword_602898 = art_frame_data.height + 2;

    for (index = 0; index < FIVE; index++) {
        font.color = tig_color_make(byte_5B8E6C[index][0], byte_5B8E6C[index][1], byte_5B8E6C[index][2]);
        tig_font_create(&font, &(dword_602908[index]));
    }

    stru_6028F8.x = 0;
    stru_6028F8.y = 0;
    stru_6028F8.width = 200;
    stru_6028F8.height = 5 * dword_602898;

    stru_6028D0.x = 0;
    stru_6028D0.y = 0;
    stru_6028D0.width = 200;
    stru_6028D0.height = dword_602898;

    settings_add(&settings, TEXT_FLOATERS_KEY, "1", text_floaters_changed);
    text_floaters_changed();

    settings_add(&settings, FLOAT_SPEED_KEY, "2", float_speed_changed);
    float_speed_changed();

    return true;
}

// 0x4D5050
void text_floater_resize(ResizeContext* ctx)
{
    stru_6028B0 = ctx->field_14;
    dword_6028E8 = ctx->iso_window_handle;
}

// 0x4D5DC0
void text_floater_exit()
{
    sub_4D5780();

    for (int index = 0; index < FIVE; index++) {
        tig_font_destroy(dword_602908[index]);
    }

    while (off_6028F0 != NULL) {
        TextFloater6028F0* next = off_6028F0->next;
        tig_video_buffer_destroy(off_6028F0->video_buffer);
        free(off_6028F0);
        off_6028F0 = next;
    }

    while (off_60289C != NULL) {
        TextFloater60289C* next = off_60289C->next;
        free(off_60289C);
        off_60289C = next;
    }

    dword_6028E8 = -1;
    dword_6028C4 = 0;
}

// 0x4D5130
bool text_floater_update_view(ViewOptions* view_options)
{
    text_floater_view_options = *view_options;
    return true;
}

// 0x4D5150
void text_floater_map_close()
{
    sub_4D5780();
}

// 0x4D5160
int text_floaters_set(int value)
{
    text_floaters_set_internal(value);
    return text_floaters;
}

// 0x4D5180
int text_floaters_get()
{
    return text_floaters;
}

// 0x4D5190
void tf_ping()
{
    // TODO: Incomplete.
}

// 0x4D5310
void sub_4D5310()
{
    // TODO: Incomplete.
}

// 0x4D5450
void sub_4D5450(int64_t obj, int type, const char* str)
{
    // TODO: Incomplete.
}

// 0x4D5570
void sub_4D5570()
{
    // TODO: Incomplete.
}

// 0x4D5620
void sub_4D5620()
{
    // TODO: Incomplete.
}

// 0x4D56C0
void sub_4D56C0(int64_t obj)
{
    // TODO: Incomplete.
}

// 0x4D5780
void sub_4D5780()
{
    // TODO: Incomplete.
}

// 0x4D57E0
TextFloater60289C* sub_4D57E0()
{
    TextFloater60289C* node;

    node = off_60289C;
    if (node != NULL) {
        off_60289C = node->next;
    } else {
        node = (TextFloater60289C*)malloc(sizeof(*node));
    }

    node->field_0 = 0;
    node->field_4 = 0;
    node->field_8 = 0;
    node->next = NULL;

    return node;
}

// 0x4D5820
void sub_4D5820()
{
    // TODO: Incomplete.
}

// 0x4D5850
void sub_4D5850(TextFloater60289C* node)
{
    node->next = off_60289C;
    off_60289C = node;
}

// 0x4D5870
void sub_4D5870()
{
    // TODO: Incomplete.
}

// 0x4D58C0
void sub_4D58C0()
{
    // TODO: Incomplete.
}

// 0x4D5950
TextFloater6028F0* sub_4D5950()
{
    TextFloater6028F0* node;
    TigVideoBufferCreateInfo vb_create_info;

    node = off_6028F0;
    if (node != NULL) {
        off_6028F0 = off_6028F0->next;
        node->next = NULL;
    } else {
        node = (TextFloater6028F0*)malloc(sizeof(*node));
        node->next = NULL;

        vb_create_info.flags = TIG_VIDEO_BUFFER_CREATE_COLOR_KEY | TIG_VIDEO_BUFFER_CREATE_VIDEO_MEMORY;
        vb_create_info.width = 200;
        vb_create_info.height = dword_602898;
        vb_create_info.background_color = dword_6028C8;
        vb_create_info.color_key = dword_6028C8;
        tig_video_buffer_create(&vb_create_info, &(node->video_buffer));
    }

    return node;
}

// 0x4D59D0
void sub_4D59D0(TextFloater6028F0* node)
{
    node->next = off_6028F0;
    off_6028F0 = node;
}

// 0x4D59F0
void sub_4D59F0()
{
    // TODO: Incomplete.
}

// 0x4D5A30
void sub_4D5A30()
{
    // TODO: Incomplete.
}

// 0x4D5A60
void sub_4D5A60()
{
    // TODO: Incomplete.
}

// 0x4D5AA0
void sub_4D5AA0()
{
    // TODO: Incomplete.
}

// 0x4D5B10
void text_floaters_set_internal(int value)
{
    if (value >= 0 && value <= 2) {
        settings_set_value(&settings, TEXT_FLOATERS_KEY, value);
    }
}

// 0x4D5B40
void text_floaters_changed()
{
    text_floaters = settings_get_value(&settings, TEXT_FLOATERS_KEY);
}

// 0x4D5B60
void float_speed_changed()
{
    float_speed = settings_get_value(&settings, FLOAT_SPEED_KEY) + 1;
}
