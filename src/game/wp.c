#include "game/wp.h"

#include "game/obj.h"

typedef struct S5FC668 {
    /* 0000 */ int64_t obj;
    /* 0008 */ int field_8;
    /* 000C */ int field_C;
} S5FC668;

static_assert(sizeof(S5FC668) == 0x10, "wrong size");

static void sub_4C0090();
static void sub_4C00D0(int64_t obj);
static void sub_4C0140();
static bool sub_4C0280(int64_t location, TigRect* rect);
static void sub_4C0370();

// 0x5FC638
static int dword_5FC638;

// 0x5FC640
static ViewOptions wp_view_options;

// 0x5FC648
static GameContextF8* dword_5FC648;

// 0x5FC64C
static bool dword_5FC64C;

// 0x5FC650
static bool wp_initialized;

// 0x5FC654
static int dword_5FC654;

// 0x5FC658
static tig_art_id_t dword_5FC658;

// 0x5FC65C
static bool wp_editor;

// 0x5FC660
static tig_window_handle_t wp_iso_window_handle;

// 0x5FC668
static S5FC668 stru_5FC668[32];

// 0x4BFCE0
bool wp_init(GameInitInfo* init_info)
{
    TigArtAnimData art_anim_data;
    int index;

    wp_iso_window_handle = init_info->iso_window_handle;
    dword_5FC648 = init_info->field_8;
    wp_editor = init_info->editor;
    wp_view_options.type = VIEW_TYPE_ISOMETRIC;

    tig_art_interface_id_create(672, 0, 0, 0, &dword_5FC658);

    if (tig_art_anim_data(dword_5FC658, &art_anim_data) != TIG_OK) {
        return false;
    }

    dword_5FC638 = art_anim_data.num_frames;

    for (index = 0; index < 32; index++) {
        stru_5FC668[index].obj = OBJ_HANDLE_NULL;
    }

    dword_5FC64C = true;
    wp_initialized = true;

    return true;
}

// 0x4BFD80
void wp_exit()
{
    int index;

    for (index = 0; index < 32; index++) {
        stru_5FC668[index].obj = OBJ_HANDLE_NULL;
    }

    wp_initialized = false;
}

// 0x4BFDA0
void wp_resize(ResizeInfo* resize_info)
{
    wp_iso_window_handle = resize_info->iso_window_handle;
}

// 0x4BFDB0
bool sub_4BFDB0(ViewOptions* view_options)
{
    wp_view_options = *view_options;

    return true;
}

// 0x4BFDD0
bool sub_4BFDD0()
{
    return dword_5FC64C;
}

// 0x4BFDE0
void sub_4BFDE0()
{
    dword_5FC64C = !dword_5FC64C;
}

// 0x4BFE00
void sub_4BFE00(void* a1)
{
    int index;

    if (!dword_5FC64C) {
        return;
    }

    if (wp_view_options.type == VIEW_TYPE_TOP_DOWN) {
        return;
    }

    sub_4C0370();

    for (index = 0; index < dword_5FC654; index++) {
        sub_4C0140(&(stru_5FC668[index]), a1);
    }
}

// 0x4C0090
void sub_4C0090()
{
    int index;

    if (!wp_editor) {
        return;
    }

    sub_4C0370();

    for (index = 0; index < dword_5FC654; index++) {
        sub_4C00D0(stru_5FC668[index].obj);
    }
}

// 0x4C00D0
void sub_4C00D0(int64_t obj)
{
    int cnt;
    int index;
    int64_t waypoint;
    TigRect rect;

    if (wp_view_options.type == VIEW_TYPE_TOP_DOWN) {
        return;
    }

    cnt = obj_arrayfield_length_get(obj, OBJ_F_NPC_WAYPOINTS_IDX);
    for (index = 0; index < cnt; index++) {
        waypoint = obj_arrayfield_int64_get(obj, OBJ_F_NPC_WAYPOINTS_IDX, index);
        if (sub_4C0280(waypoint, &rect)) {
            dword_5FC648(&rect);
        }
    }
}

// 0x4C0140
void sub_4C0140()
{
    // TODO: Incomplete.
}

// 0x4C0280
bool sub_4C0280(int64_t location, TigRect* rect)
{
    int64_t x;
    int64_t y;
    TigArtFrameData art_frame_data;

    sub_4B8680(location, &x, &y);

    if (x < INT_MIN || x >= INT_MAX
        || y < INT_MIN && y >= INT_MAX) {
        return false;
    }

    if (wp_view_options.type == VIEW_TYPE_TOP_DOWN) {
        return false;
    }

    if (tig_art_frame_data(dword_5FC658, &art_frame_data) != TIG_OK) {
        return false;
    }

    rect->x = (int)x - art_frame_data.hot_x + 40;
    rect->y = (int)y - (40 - art_frame_data.height) / 2 - art_frame_data.hot_y + 40;
    rect->width = art_frame_data.width;
    rect->height = art_frame_data.height;

    return true;
}

// 0x4C0370
void sub_4C0370()
{
    int index;

    for (index = 0; index < dword_5FC654; index++) {
        // TODO: Incomplete.
    }
}
