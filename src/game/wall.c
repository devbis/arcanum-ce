#include "game/wall.h"

#include "game/obj.h"
#include "game/obj_private.h"
#include "game/object.h"

static void sub_4E1C00(UnknownContext* render_info);
static void sub_4E1EB0(UnknownContext* render_info);
static void sub_4E20A0(int64_t obj);
static void sub_4E25B0(int64_t obj);
static void sub_4E2C50(int64_t obj);

// 0x603420
static TigRect stru_603420;

// 0x603430
static bool wall_editor;

// 0x603434
static int dword_603434;

// 0x603438
static GameContextF8* wall_invalidate_rect;

// 0x60343C
static tig_window_handle_t wall_iso_window_handle;

// 0x603440
static ViewOptions wall_view_options;

// 0x603448
static bool dword_603448;

// 0x4DF390
bool wall_init(GameInitInfo* init_info)
{
    TigWindowData window_data;

    wall_iso_window_handle = init_info->iso_window_handle;
    wall_invalidate_rect = init_info->field_8;

    if (tig_window_data(init_info->iso_window_handle, &window_data) != TIG_OK) {
        return false;
    }

    stru_603420.x = 0;
    stru_603420.y = 0;
    stru_603420.width = window_data.rect.width;
    stru_603420.height = window_data.rect.height;

    wall_view_options.type = VIEW_TYPE_ISOMETRIC;
    wall_editor = init_info->editor;
    dword_603448 = true;
    dword_603434 = tig_color_make(255, 0, 100);

    return true;
}

// 0x4DF460
void wall_exit()
{
    wall_iso_window_handle = TIG_WINDOW_HANDLE_INVALID;
    wall_invalidate_rect = NULL;
}

// 0x4DF480
void wall_resize(ResizeInfo* resize_info)
{
    wall_iso_window_handle = resize_info->iso_window_handle;
    stru_603420 = resize_info->field_14;
}

// 0x4DF4C0
bool wall_update_view(ViewOptions* view_options)
{
    wall_view_options = *view_options;

    return true;
}

// 0x4DF4E0
void wall_toggle()
{
    dword_603448 = !dword_603448;
    wall_invalidate_rect(NULL);
}

// 0x4DF500
void wall_render(UnknownContext* render_info)
{
    if (wall_view_options.type == VIEW_TYPE_TOP_DOWN) {
        if (dword_603448) {
            sub_4E1C00(render_info);
            sub_4E1EB0(render_info);
        }
    }
}

// 0x4DF740
void sub_4DF740()
{
    // TODO: Incomplete.
}

// 0x4E0240
void sub_4E0240()
{
    // TODO: Incomplete.
}

// 0x4E1490
void sub_4E1490()
{
    // TODO: Incomplete.
}

// 0x4E18F0
void sub_4E18F0(int64_t obj, bool a2)
{
    tig_art_id_t art_id;
    int p_piece;

    art_id = obj_field_int32_get(obj, OBJ_F_CURRENT_AID);
    p_piece = tig_art_wall_id_p_piece_get(art_id);

    if (p_piece >= 9 && p_piece <= 20) {
        sub_4E20A0(obj);
    } else if (p_piece >= 21 && p_piece <= 33) {
        sub_4E25B0(obj);
    } else if (p_piece >= 34 && p_piece <= 45) {
        sub_4E2C50(obj);
    }

    if (a2) {
        if (sub_4E5470(obj)) {
            sub_43CF70(obj);
            sub_43CFF0(obj);
        }
    }
}

// 0x4E1C00
void sub_4E1C00(UnknownContext* render_info)
{
    // TODO: Incomplete.
}

// 0x4E1EB0
void sub_4E1EB0(UnknownContext* render_info)
{
    // TODO: Incomplete.
}

// 0x4E20A0
void sub_4E20A0(int64_t obj)
{
    // TODO: Incomplete.
}

// 0x4E25B0
void sub_4E25B0(int64_t obj)
{
    // TODO: Incomplete.
}

// 0x4E2C50
void sub_4E2C50(int64_t obj)
{
    // TODO: Incomplete.
}
