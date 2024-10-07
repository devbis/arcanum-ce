#include "ui/hotkey_ui.h"

#include "game/gsound.h"
#include "game/item.h"
#include "game/object.h"
#include "game/player.h"
#include "game/spell.h"
#include "ui/intgame.h"
#include "ui/inven_ui.h"
#include "ui/skill_ui.h"

static bool intgame_save_hotkey(S683518* a1, TigFile* stream);
static bool intgame_load_hotkey(S683518* a1, TigFile* stream);
static void intgame_hotkey_mouse_load(tig_art_id_t art_id, bool a2);
static void sub_57EE30(int64_t a1, int inventory_location);
static bool button_create_no_art(UiButtonInfo* button_info, int width, int height);

// 0x5CB494
static int dword_5CB494[10] = {
    173,
    174,
    175,
    176,
    177,
    178,
    179,
    180,
    181,
    172,
};

// 0x5CB4BC
static int dword_5CB4BC[10] = {
    198,
    237,
    276,
    315,
    354,
    418,
    456,
    495,
    534,
    573,
};

// 0x5CB4E4
int dword_5CB4E4 = -1;

// 0x683510
static tig_window_handle_t dword_683510;

// 0x683518
static S683518 stru_683518[2];

// 0x6835C8
static TigRect stru_6835C8;

// 0x6835D8
static tig_window_handle_t dword_6835D8;

// 0x6835E0
static S683518 stru_6835E0[10];

// 0x683950
static int dword_683950;

// 0x683958
static int dword_683958;

// 0x683960
static FollowerInfo stru_683960;

// 0x6839A8
static int dword_6839A8;

// 0x6839AC
static int dword_6839AC;

// 0x6839B0
int dword_6839B0;

// 0x57D700
bool hotkey_ui_init(GameInitInfo* init_info)
{
    tig_art_id_t art_id;
    TigArtFrameData art_frame_data;
    int index;
    S683518* v1;

    (void)init_info;

    dword_6835D8 = TIG_WINDOW_HANDLE_INVALID;
    dword_683510 = TIG_WINDOW_HANDLE_INVALID;

    if (tig_art_interface_id_create(dword_5CB494[0], 0, 0, 0, &art_id) != TIG_OK) {
        tig_debug_printf("hotkey_ui_init: ERROR: tig_art_interface_id_create failed!\n");
        exit(EXIT_FAILURE);
    }

    tig_art_frame_data(art_id, &art_frame_data);
    dword_6839A8 = art_frame_data.width;
    dword_6839AC = art_frame_data.height;

    for (index = 0; index < 2; index++) {
        v1 = &(stru_683518[index]);
        v1->field_8 = 1;
        v1->field_C = sub_557B50(index);
        sub_557B20(index)->art_num = sub_579F70(v1->field_C);
    }

    for (index = 0; index < 10; index++) {
        v1 = &(stru_6835E0[index]);
        sub_57E5A0(v1);
        v1->field_0 = index;
        v1->field_C = -1;
        v1->info.art_num = dword_5CB494[index];
        v1->info.button_handle = TIG_BUTTON_HANDLE_INVALID;
        v1->info.x = dword_5CB4BC[index] - stru_6835C8.x;
        v1->info.y = 445;
    }

    return true;
}

// 0x57D800
void hotkey_ui_exit()
{
}

// 0x57D810
void hotkey_ui_resize(ResizeInfo* resize_info)
{
    (void)resize_info;

    sub_557B30(0, 69, 548);
    sub_557B30(1, 114, 548);
}

// 0x57D840
bool hotkey_ui_start(int a1, TigRect* rect, int a3, bool a4)
{
    tig_art_id_t art_id;
    TigArtFrameData art_frame_data;
    TigArtBlitInfo art_blit_info;
    TigRect src_rect;
    TigRect dst_rect;
    int index;
    S683518* v1;

    stru_6835C8 = *rect;
    dword_6835D8 = a1;
    dword_683510 = a3;

    if (tig_art_interface_id_create(dword_5CB494[0], 0, 0, 0, &art_id) != TIG_OK) {
        tig_debug_printf("hotkey_ui_start: ERROR: tig_art_interface_id_create failed!\n");
        exit(EXIT_FAILURE);
    }

    tig_art_frame_data(art_id, &art_frame_data);

    dword_6839A8 = art_frame_data.width;
    dword_6839AC = art_frame_data.height;

    if (a3 != -1) {
        for (index = 0; index < 2; index++) {
            v1 = &(stru_683518[index]);
            sub_557B20(v1->field_C)->art_num = v1->field_8 == 2 || v1->field_8 != 3
                ? sub_579F70(v1->field_C)
                : sub_4B1570(v1->field_C);
            sub_54AA60(dword_683510, &stru_6835C8, sub_557B20(index), 0x1);
        }
    }

    if (a4) {
        if (tig_art_interface_id_create(184, 0, 0, 0, &art_id) == TIG_OK) {
            tig_art_frame_data(art_id, &art_frame_data);

            src_rect.x = dword_5CB4BC[0] - 2;
            src_rect.y = 1;
            src_rect.width = 411;
            src_rect.height = 37;

            dst_rect.x = 0;
            dst_rect.y = 0;
            dst_rect.width = 411;
            dst_rect.height = 37;

            art_blit_info.flags = 0;
            art_blit_info.art_id = art_id;
            art_blit_info.src_rect = &src_rect;
            art_blit_info.dst_rect = &dst_rect;
            tig_window_blit_art(dword_6835D8, &art_blit_info);
        }
    }

    for (index = 0; index < 10; index++) {
        v1 = &(stru_6835E0[index]);
        v1->info.x = dword_5CB4BC[index] - stru_6835C8.x;
        v1->info.y = 445;
        intgame_hotkey_refresh(index);

        if (!button_create_no_art(&(v1->info), dword_6839A8, dword_6839AC)) {
            tig_debug_printf("hotkey_ui_start: ERROR: button_create_no_art failed!\n");
            exit(EXIT_FAILURE);
        }
    }

    return true;
}

// 0x57DA50
void sub_57DA50()
{
    int index;
    tig_button_handle_t button_handle;

    for (index = 0; index < 2; index++) {
        button_handle = sub_557B20(index)->button_handle;
        if (button_handle != TIG_BUTTON_HANDLE_INVALID) {
            tig_button_destroy(button_handle);
            sub_557B20(index)->button_handle = TIG_BUTTON_HANDLE_INVALID;
        }
    }

    for (index = 0; index < 10; index++) {
        if (stru_6835E0[index].info.button_handle != TIG_BUTTON_HANDLE_INVALID) {
            tig_button_destroy(stru_6835E0[index].info.button_handle);
            stru_6835E0[index].info.button_handle = TIG_BUTTON_HANDLE_INVALID;
        }
    }
}

// 0x57DAB0
void sub_57DAB0()
{
    int index;
    tig_button_handle_t button_handle;
    tig_art_id_t art_id;

    dword_6839B0 = 0;

    for (index = 0; index < 2; index++) {
        sub_57E5A0(&(stru_683518[index]));
        stru_683518[index].field_8 = 2;
        stru_683518[index].field_C = sub_557B50(index);
        sub_557B20(index)->art_num = sub_579F70(stru_683518[index].field_C);

        button_handle = sub_557B20(index)->button_handle;
        if (button_handle != TIG_BUTTON_HANDLE_INVALID) {
            tig_art_interface_id_create(sub_557B20(index)->art_num, 0, 0, 0, &art_id);
            tig_button_set_art(button_handle, art_id);
        }
    }
}

// 0x57DB40
bool sub_57DB40(TigFile* stream)
{
    int index;

    for (index = 0; index < 2; index++) {
        stru_683518[index].field_0 = -1;
        if (!intgame_save_hotkey(&(stru_683518[index]), stream)) {
            return false;
        }
    }

    for (index = 0; index < 10; index++) {
        if (!intgame_save_hotkey(&(stru_6835E0[index]), stream)) {
            return false;
        }
    }

    return true;
}

// 0x57DBA0
bool sub_57DBA0(GameLoadInfo* load_info)
{
    int index;
    S683518 v1;

    for (index = 0; index < 2; index++) {
        if (!intgame_load_hotkey(&v1, load_info->stream)) {
            return false;
        }

        sub_57EFA0(v1.field_8, v1.field_C, v1.field_10);
    }

    for (index = 0; index < 10; index++) {
        if (!intgame_load_hotkey(&(stru_6835E0[index]), load_info->stream)) {
            return false;
        }
    }

    return true;
}

// 0x57DC20
void sub_57DC20()
{
    dword_683950 = -1;
    dword_683958 = 1;
    sub_4440E0(sub_573620(), &stru_683960);
    dword_6839B0 = 1;
}

// 0x57DC60
void sub_57DC60()
{
    // TODO: Incomplete.
}

// 0x57DE00
void sub_57DE00()
{
    // TODO: Incomplete.
}

// 0x57DE10
bool intgame_save_hotkey(S683518* a1, TigFile* stream)
{
    if (tig_file_fwrite(&(a1->field_8), sizeof(a1->field_8), 1, stream) != 1) return false;
    if (tig_file_fwrite(&(a1->field_0), sizeof(a1->field_0), 1, stream) != 1) return false;

    switch (a1->field_8) {
    case 1:
        if (tig_file_fwrite(&(a1->field_18), sizeof(a1->field_18), 1, stream) != 1) return false;
        if (tig_file_fwrite(&(a1->field_40), sizeof(a1->field_40), 1, stream) != 1) return false;
        break;
    case 2:
        if (tig_file_fwrite(&(a1->field_C), sizeof(a1->field_C), 1, stream) != 1) return false;
        break;
    case 3:
        if (tig_file_fwrite(&(a1->field_C), sizeof(a1->field_C), 1, stream) != 1) return false;
        break;
    case 4:
        if (tig_file_fwrite(&(a1->field_18), sizeof(a1->field_18), 1, stream) != 1) return false;
        if (tig_file_fwrite(&(a1->field_C), sizeof(a1->field_C), 1, stream) != 1) return false;
        break;
    }

    return true;
}

// 0x57DEF0
bool intgame_load_hotkey(S683518* a1, TigFile* stream)
{
    // TODO: Incomplete.
}

// 0x57E140
void intgame_hotkey_refresh(int index)
{
    // TODO: Incomplete.
}

// 0x57E460
int sub_57E460()
{
    TigMouseState mouse_state;
    int v1;
    int index;

    tig_mouse_get_state(&mouse_state);

    v1 = 440;
    if (intgame_is_compact_interface()) {
        v1 = 563;
    }

    if (mouse_state.x < dword_5CB4BC[0] - 5
        || mouse_state.y < v1
        || mouse_state.x >= dword_5CB4BC[9] + dword_5CB4BC[0] - 5 + 32
        || mouse_state.y >= v1 + 37) {
        return 11;
    }

    for (index = 0; index < 10; index++) {
        if (mouse_state.x >= dword_5CB4BC[index]
            && mouse_state.x < dword_5CB4BC[index] + 32
            && mouse_state.y < v1 + 37) {
            break;
        }
    }

    return index;
}

// 0x57E500
void intgame_hotkey_mouse_load(tig_art_id_t art_id, bool a2)
{
    int dx = 0;
    int dy = 0;
    tig_art_id_t cursor_art_id;
    TigArtFrameData art_frame_data;

    sub_553990();
    if (art_id != TIG_ART_ID_INVALID) {
        if (!a2) {
            dx = 2;
            dy = 2;
        }

        cursor_art_id = tig_mouse_cursor_get_art_id();
        tig_mouse_hide();
        tig_mouse_cursor_set_art_id(art_id);
        tig_mouse_cursor_overlay(cursor_art_id, dx, dy);

        if (a2) {
            tig_mouse_cursor_set_art_id(art_id);
            if (tig_art_frame_data(art_id, &art_frame_data) != TIG_OK) {
                tig_debug_printf("intgame_hotkey_mouse_load: ERROR: tig_art_frame_data failed!\n");
                exit(EXIT_FAILURE);
            }

            dy += art_frame_data.height / 2;
        }

        tig_mouse_cursor_set_offset(dx, dy);
        tig_mouse_show();
    }
}

// 0x57E5A0
void sub_57E5A0(S683518* a1)
{
    a1->field_10 = OBJ_HANDLE_NULL;
    a1->field_8 = 0;
    a1->field_4 = 0;
    a1->field_44 = -1;
    a1->field_18.type = 0;
    dword_5CB4E4 = -1;
}

// 0x57E5D0
bool sub_57E5D0()
{
    // TODO: Incomplete.
}

// 0x57E8B0
void sub_57E8B0()
{
    sub_575770();
    sub_553990();
    dword_6839B0 = 0;
    dword_5CB4E4 = -1;
}

// 0x57E8D0
bool sub_57E8D0(int a1)
{
    // TODO: Incomplete.
}

// 0x57ED60
void sub_57ED60(S683518* a1, int a2)
{
    stru_6835E0[dword_5CB4E4].field_4 = 0;
    stru_6835E0[dword_5CB4E4].field_8 = a1->field_8;
    stru_6835E0[dword_5CB4E4].field_C = a1->field_C;
    intgame_hotkey_refresh(a2);
}

// 0x57EDA0
bool sub_57EDA0(int a1)
{
    if (sub_573620() != OBJ_HANDLE_NULL) {
        dword_683950 = -1;
        dword_683958 = 1;
        sub_4440E0(sub_573620(), &stru_683960);
        dword_6839B0 = 1;
    }

    return sub_57E8D0(a1) != 0;
}

// 0x57EDF0
void sub_57EDF0(int64_t obj, int64_t a2, int a3)
{
    if (player_is_pc_obj(obj)) {
        sub_57EE30(a2, a3);
    } else {
        item_location_set(a2, a3);
    }
}

// 0x57EE30
void sub_57EE30(int64_t a1, int a2)
{
    // TODO: Incomplete.
}

// 0x57EED0
bool sub_57EED0(int64_t obj, int a2)
{
    // TODO: Incomplete.
}

// 0x57EF90
void sub_57EF90(int index)
{
    sub_57F210(index);
}

// 0x57EFA0
void sub_57EFA0(int a1, int a2, int64_t obj)
{
    // TODO: Incomplete.
}

// 0x57F160
bool button_create_no_art(UiButtonInfo* button_info, int width, int height)
{
    TigButtonData button_data;

    button_data.flags = TIG_BUTTON_FLAG_0x01;
    button_data.window_handle = dword_6835D8;
    button_data.x = button_info->x;
    button_data.y = button_info->y - 441;
    button_data.art_id = TIG_ART_ID_INVALID;
    button_data.width = width;
    button_data.height = height;
    button_data.mouse_down_snd_id = -1;
    button_data.mouse_up_snd_id = -1;
    button_data.mouse_enter_snd_id = -1;
    button_data.mouse_exit_snd_id = -1;
    return tig_button_create(&button_data, &(button_info->button_handle)) == TIG_OK;
}

// 0x57F1D0
void sub_57F1D0(int index)
{
    if ((stru_6835E0[index].field_4 & 0x1) == 0
        && stru_6835E0[index].field_8) {
        gsound_play_sfx_id(0, 1);
        sub_54FCF0(&(stru_6835E0[index]));
    }
}

// 0x57F210
void sub_57F210(int index)
{
    sub_57E5A0(&(stru_6835E0[index]));
    intgame_hotkey_refresh(index);
}

// 0x57F240
S683518* sub_57F240(int index)
{
    return &(stru_6835E0[index]);
}

// 0x57F260
bool sub_57F260()
{
    bool ret = false;
    int index;
    S683518* v1;

    for (index = 0; index < 10; index++) {
        v1 = sub_57F240(index);
        if (v1->field_8 == 1 || v1->field_8 == 4) {
            if ((v1->field_4 & 0x1) != 0) {
                v1->field_4 &= ~0x1;
                intgame_hotkey_refresh(index);
                ret = true;
            }
        }
    }

    return ret;
}

// 0x57F2C0
void sub_57F2C0(int64_t obj, int a3)
{
    int index;
    S683518* v1;

    if (a3) {
        for (index = 0; index < 2; index++) {
            v1 = &(stru_683518[index]);
            if (v1->field_10 == obj) {
                sub_57E5A0(v1);
                sub_57EFA0(2, sub_557B50(index), v1->field_10);
            }
        }
    }
}

// 0x57F340
void sub_57F340(int a1)
{
    int index;
    S683518* v1;

    for (index = 0; index < 2; index++) {
        v1 = &(stru_683518[index]);
        if (v1->field_8 == 3 && v1->field_C == a1) {
            sub_57E5A0(v1);
            v1->field_C = sub_557B50(index);
            sub_57EFA0(2, v1->field_C, OBJ_HANDLE_NULL);
        }
    }

    for (index = 0; index < 10; index++) {
        v1 = sub_57F240(index);
        if (v1->field_8 == 3 && v1->field_C == a1) {
            sub_57E5A0(v1);
            intgame_hotkey_refresh(v1->field_0);
        }
    }
}

// 0x57F3C0
void intgame_hotkeys_recover()
{
    // TODO: Incomplete.
}
