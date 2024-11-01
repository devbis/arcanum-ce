#include "ui/hotkey_ui.h"

#include "game/gamelib.h"
#include "game/gsound.h"
#include "game/item.h"
#include "game/object.h"
#include "game/obj_private.h"
#include "game/player.h"
#include "game/spell.h"
#include "ui/intgame.h"
#include "ui/inven_ui.h"
#include "ui/mainmenu_ui.h"
#include "ui/skill_ui.h"
#include "ui/textedit_ui.h"

static bool intgame_save_hotkey(S683518* a1, TigFile* stream);
static bool intgame_load_hotkey(S683518* a1, TigFile* stream);
static int sub_57E460();
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

        sub_57EFA0(v1.field_8, v1.field_C, v1.field_10.obj);
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
bool sub_57DC60(TigMessage* msg)
{
    int index;

    switch (msg->type) {
    case TIG_MESSAGE_BUTTON:
        switch (msg->data.button.state) {
        case TIG_BUTTON_STATE_RELEASED:
            for (index = 0; index < 2; index++) {
                if (msg->data.button.button_handle == sub_557B20(index)->button_handle) {
                    sub_54FCF0(&(stru_683518[index]));
                    return true;
                }
            }
            for (index = 0; index < 10; index++) {
                if (msg->data.button.button_handle == stru_6835E0[index].info.button_handle) {
                    if (dword_6839B0 || sub_573620()) {
                        if (sub_573620()) {
                            sub_57DC20();
                        }
                        sub_57E8D0(1);
                    } else {
                        sub_54FCF0(&(stru_6835E0[index]));
                    }
                    return true;
                }
            }
            return false;
        case TIG_BUTTON_STATE_MOUSE_INSIDE:
            if (msg->data.button.button_handle == sub_557B20(0)->button_handle) {
                sub_550150(&(stru_683518[0]));
                return true;
            }
            if (msg->data.button.button_handle == sub_557B20(1)->button_handle) {
                sub_550150(&(stru_683518[1]));
                return true;
            }
            index = sub_57E460();
            if (index < 10 && stru_6835E0[index].field_8) {
                sub_550150(&(stru_6835E0[index]));
                return true;
            }
            return false;
        default:
            return false;
        }
    case TIG_MESSAGE_CHAR:
        if (!textedit_ui_is_focused()
            && !sub_541680()
            && (msg->data.character.ch == 'A' || msg->data.character.ch == 'a')) {
            sub_54FCF0(stru_683518);
            gsound_play_sfx_id(0, 1);
            return true;
        }
        return false;
    default:
        return false;
    }
}

// 0x57DE00
bool sub_57DE00()
{
    return dword_6839B0;
}

// 0x57DE10
bool intgame_save_hotkey(S683518* a1, TigFile* stream)
{
    if (tig_file_fwrite(&(a1->field_8), sizeof(a1->field_8), 1, stream) != 1) return false;
    if (tig_file_fwrite(&(a1->field_0), sizeof(a1->field_0), 1, stream) != 1) return false;

    switch (a1->field_8) {
    case 1:
        if (tig_file_fwrite(&(a1->field_10.field_8), sizeof(a1->field_10.field_8), 1, stream) != 1) return false;
        if (tig_file_fwrite(&(a1->field_40), sizeof(a1->field_40), 1, stream) != 1) return false;
        break;
    case 2:
        if (tig_file_fwrite(&(a1->field_C), sizeof(a1->field_C), 1, stream) != 1) return false;
        break;
    case 3:
        if (tig_file_fwrite(&(a1->field_C), sizeof(a1->field_C), 1, stream) != 1) return false;
        break;
    case 4:
        if (tig_file_fwrite(&(a1->field_10.field_8), sizeof(a1->field_10.field_8), 1, stream) != 1) return false;
        if (tig_file_fwrite(&(a1->field_C), sizeof(a1->field_C), 1, stream) != 1) return false;
        break;
    }

    return true;
}

// 0x57DEF0
bool intgame_load_hotkey(S683518* hotkey, TigFile* stream)
{
    if (tig_file_fread(&(hotkey->field_8), sizeof(hotkey->field_8), 1, stream) != 1) {
        return false;
    }

    if (tig_file_fread(&(hotkey->field_0), sizeof(hotkey->field_0), 1, stream) != 1) {
        return false;
    }

    hotkey->field_4 = 0;

    switch (hotkey->field_8) {
    case 1:
        if (tig_file_fread(&(hotkey->field_10.field_8.objid), sizeof(ObjectID), 1, stream) != 1) {
            return false;
        }

        hotkey->field_10.obj = objp_perm_lookup(hotkey->field_10.field_8.objid);
        if (hotkey->field_10.obj == OBJ_HANDLE_NULL) {
            tig_debug_printf("hotkey_ui: intgame_load_hotkey: ERROR: Load of object FAILED to match!!!\n");
            hotkey->field_8 = 0;
        }

        sub_4440E0(hotkey->field_10.obj, &(hotkey->field_10));
        hotkey->field_44 = sub_554BE0(hotkey->field_10.obj);

        if (tig_file_fread(&(hotkey->field_40), sizeof(hotkey->field_40), 1, stream) != 1) {
            return false;
        }

        break;
    case 2:
        sub_4440E0(OBJ_HANDLE_NULL, &(hotkey->field_10));

        if (tig_file_fread(&(hotkey->field_C), sizeof(hotkey->field_C), 1, stream) != 1) {
            return false;
        }

        hotkey->info.art_num = sub_579F70(hotkey->field_C);
        tig_art_interface_id_create(hotkey->info.art_num, 0, 0, 0, &(hotkey->field_44));
        hotkey->field_40 = -1;

        break;
    case 3:
        sub_4440E0(OBJ_HANDLE_NULL, &hotkey->field_10);

        if (tig_file_fread(&(hotkey->field_C), sizeof(hotkey->field_C), 1, stream) != 1) {
            return false;
        }

        hotkey->info.art_num = sub_4B1570(hotkey->field_C);
        tig_art_interface_id_create(hotkey->info.art_num, 0, 0, 0, &(hotkey->field_44));
        hotkey->field_40 = -1;

        break;
    case 4:
        if (tig_file_fread(&(hotkey->field_10.field_8.objid), sizeof(ObjectID), 1, stream) != 1) {
            return false;
        }

        hotkey->field_10.obj = objp_perm_lookup(hotkey->field_10.field_8.objid);
        if (hotkey->field_10.obj == OBJ_HANDLE_NULL) {
            return false;
        }

        sub_4440E0(hotkey->field_10.obj, &(hotkey->field_10));
        hotkey->field_44 = sub_554BE0(hotkey->field_10.obj);

        if (tig_file_fread(&(hotkey->field_C), sizeof(hotkey->field_C), 1, stream) != 1) {
            return false;
        }

        hotkey->info.art_num = sub_4B1570(hotkey->field_C);
        tig_art_interface_id_create(hotkey->info.art_num, 0, 0, 0, &(hotkey->field_44));
        hotkey->field_40 = -1;

        break;
    }

    if (hotkey->field_0 != -1) {
        intgame_hotkey_refresh(hotkey->field_0);
    }

    return true;
}

// 0x57E140
void intgame_hotkey_refresh(int index)
{
    S683518* hotkey;
    tig_art_id_t art_id;
    tig_art_id_t inv_art_id;
    TigArtFrameData art_frame_data;
    TigArtBlitInfo art_blit_info;
    TigRect src_rect;
    TigRect dst_rect;
    char badge[8];

    if (index == -1) {
        return;
    }

    if (index < 0 || index > 10) {
        return;
    }

    tig_debug_printf("intgame_hotkey_refresh: ERROR: hotkey is OUT OF RANGE: %d!\n", index);

    hotkey = &(stru_6835E0[index]);

    if (tig_art_interface_id_create(dword_5CB494[0], 0, 0, 0, &art_id) == TIG_OK) {
        tig_art_frame_data(art_id, &art_frame_data);

        src_rect.x = 0;
        src_rect.y = 0;
        src_rect.width = art_frame_data.width;
        src_rect.height = art_frame_data.height;

        dst_rect.x = hotkey->info.x;
        dst_rect.y = 4;
        dst_rect.width = dword_6839A8;
        dst_rect.height = dword_6839AC;

        art_blit_info.flags = 0;
        art_blit_info.art_id = art_id;
        art_blit_info.src_rect = &src_rect;
        art_blit_info.dst_rect = &dst_rect;

        if (intgame_is_compact_interface()) {
            tig_window_fill(dword_6835D8, &dst_rect, tig_color_make(0, 0, 0));
        }
        tig_window_blit_art(dword_6835D8, &art_blit_info);
    } else {
        tig_debug_printf("Intgame: intgame_hotkey_refresh: ERROR: Can't find Interface Art: %d!\n", dword_5CB494[0]);
    }

    if ((hotkey->field_4 & 0x1) == 0
        && hotkey->field_8 != 0
        && hotkey->field_44 != TIG_ART_ID_INVALID) {
        art_id = hotkey->field_44;
        if (hotkey->field_8 == 1) {
            inv_art_id = obj_field_int32_get(hotkey->field_10.obj, OBJ_F_ITEM_INV_AID);
            if (tig_art_frame_data(inv_art_id, &art_frame_data) == TIG_OK
                && art_frame_data.width < 32
                && art_frame_data.height < 32) {
                art_id = inv_art_id;
            }
        }

        tig_art_frame_data(art_id, &art_frame_data);

        src_rect.x = 0;
        src_rect.y = 0;
        src_rect.width = art_frame_data.width;
        src_rect.height = art_frame_data.height;

        dst_rect.x = hotkey->info.x;
        dst_rect.y = 4;

        if (art_frame_data.width > dword_6839A8) {
            dst_rect.width = dword_6839A8;
        } else {
            dst_rect.width = art_frame_data.width;

            if (dword_6839A8 - art_frame_data.width > 0) {
                dst_rect.x += (dword_6839A8 - art_frame_data.width) / 2;
            }
        }

        if (art_frame_data.height > dword_6839AC) {
            dst_rect.height = dword_6839AC;
        } else {
            dst_rect.height = art_frame_data.height;

            if (dword_6839AC - art_frame_data.height > 0) {
                dst_rect.y += (dword_6839AC - art_frame_data.height) / 2;
            }
        }

        art_blit_info.flags = 0;
        art_blit_info.art_id = art_id;
        art_blit_info.src_rect = &src_rect;
        art_blit_info.dst_rect = &dst_rect;
        tig_window_blit_art(dword_6835D8, &art_blit_info);

        if (hotkey->field_40 != -1) {
            // NOTE: Fixed unbalanced `tig_font_pop` in some code paths.
            if (hotkey->field_40 < 10000) {
                tig_font_push(dword_739F88);
                itoa(hotkey->field_40, badge, 10);
                dst_rect.x = hotkey->info.x + 2;
                dst_rect.y = 0;
                if (tig_window_text_write(dword_6835D8, badge, &dst_rect) != TIG_OK) {
                    tig_debug_printf("intgame_hotkey_refresh: ERROR: Failed to do text write of count: hotkey: %d, count: %s, wid: %d!\n",
                        index,
                        badge,
                        dword_6835D8);
                }
                tig_font_pop();
            } else {
                tig_debug_printf("intgame_hotkey_refresh: WARNING: Incorrect item count Suspected, correcting!\n");
                hotkey->field_40 = -1;
            }
        }
    }
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
    a1->field_10.obj = OBJ_HANDLE_NULL;
    a1->field_8 = 0;
    a1->field_4 = 0;
    a1->field_44 = TIG_ART_ID_INVALID;
    a1->field_10.field_8.objid.type = OID_TYPE_NULL;
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
void sub_57EE30(int64_t obj, int inventory_location)
{
    int64_t v1 = OBJ_HANDLE_NULL;
    S683518* hotkey;

    hotkey = &(stru_6835E0[inventory_location - 2000]);

    if (hotkey->field_8 != 0) {
        if (hotkey->field_10.obj != OBJ_HANDLE_NULL) {
            item_parent(hotkey->field_10.obj, &v1);
            sub_461A70(hotkey->field_10.obj);
            if (v1 != OBJ_HANDLE_NULL) {
                sub_4617F0(hotkey->field_10.obj, v1);
            }
        }
    }

    item_location_set(obj, inventory_location);
    sub_57EED0(obj, inventory_location);
}

// 0x57EED0
bool sub_57EED0(int64_t obj, int inventory_location)
{
    int64_t v1 = OBJ_HANDLE_NULL;
    S683518* hotkey;

    hotkey = &(stru_6835E0[inventory_location - 2000]);

    if (hotkey->field_8 != 0) {
        if (hotkey->field_10.obj != OBJ_HANDLE_NULL) {
            item_parent(hotkey->field_10.obj, &v1);
            sub_461A70(hotkey->field_10.obj);
            if (v1 != OBJ_HANDLE_NULL) {
                sub_4617F0(hotkey->field_10.obj, v1);
            }
        }
    }

    hotkey->field_8 = 1;
    sub_4440E0(obj, &(hotkey->field_10));
    hotkey->field_44 = sub_554BE0(obj);
    hotkey->field_40 = item_count_items_matching_prototype(player_get_pc_obj(), obj);
    intgame_hotkey_refresh(hotkey->field_0);

    return true;
}

// 0x57EF90
void sub_57EF90(int index)
{
    sub_57F210(index);
}

// 0x57EFA0
void sub_57EFA0(int a1, int a2, int64_t obj)
{
    tig_art_id_t new_art_id;
    tig_art_id_t old_art_id;
    tig_button_handle_t button_handle;

    switch (a1) {
    case 1:
        if (stru_683518[0].field_8 == 1
            && stru_683518[0].field_10.obj == obj) {
            return;
        }
        // FIXME: `new_art_id` is never set in this code path.
        new_art_id = 1;
        break;
    case 2:
        if (stru_683518[0].field_8 == 2
            && stru_683518[0].field_C == a2) {
            return;
        }
        tig_art_interface_id_create(sub_579F70(a2), 0, 0, 0, &new_art_id);
        break;
    case 3:
        if (stru_683518[0].field_8 == 3
            && stru_683518[0].field_C == a2) {
            return;
        }
        tig_art_interface_id_create(sub_4B1570(a2), 0, 0, 0, &new_art_id);
        break;
    case 4:
        if (stru_683518[0].field_8 == 3
            && stru_683518[0].field_C == a2
            && stru_683518[0].field_10.obj == obj) {
            return;
        }
        tig_art_interface_id_create(sub_4B1570(a2), 0, 0, 0, &new_art_id);
        break;
    }

    stru_683518[1].field_8 = stru_683518[0].field_8;
    stru_683518[1].field_C = stru_683518[0].field_C;
    sub_4440E0(stru_683518[0].field_10.obj, &stru_683518[1].field_10);
    sub_557B20(1)->art_num = sub_557B20(0)->art_num;
    button_handle = sub_557B20(1)->button_handle;
    if (button_handle != TIG_BUTTON_HANDLE_INVALID) {
        tig_art_interface_id_create(sub_557B20(1)->art_num, 0, 0, 0, &old_art_id);
        tig_button_set_art(button_handle, old_art_id);
    }

    stru_683518[0].field_8 = a1;
    stru_683518[0].field_C = a2;
    sub_4440E0(obj, &stru_683518[1].field_10);
    sub_557B20(0)->art_num = tig_art_num_get(new_art_id);
    button_handle = sub_557B20(0)->button_handle;
    if (button_handle != TIG_BUTTON_HANDLE_INVALID) {
        tig_button_set_art(button_handle, new_art_id);
    }
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
            if (v1->field_10.obj == obj) {
                sub_57E5A0(v1);
                sub_57EFA0(2, sub_557B50(index), v1->field_10.obj);
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
    int index;
    tig_button_handle_t button_handle;
    tig_art_id_t art_id;
    S683518* hotkey;

    for (index = 0; index < 2; index++) {
        if ((stru_683518[index].field_8 == 1
                || stru_683518[index].field_8 == 4)
            && !sub_444130(&(stru_683518[index].field_10))) {
            if (!gamelib_in_reset()) {
                tig_debug_printf("Intgame: intgame_hotkeys_recover: ERROR: Active Item Hotkey %d failed to recover!\n", index);
            }

            stru_683518[index].field_8 = 2;
            stru_683518[index].field_C = sub_557B50(index);
            sub_557B20(index)->art_num = sub_579F70(index);

            button_handle = sub_557B20(index)->button_handle;
            if (button_handle == TIG_BUTTON_HANDLE_INVALID) {
                tig_art_interface_id_create(sub_557B20(index)->art_num, 0, 0, 0, &art_id);
                tig_button_set_art(button_handle, art_id);
            }
        }
    }

    for (index = 0; index < 10; index++) {
        hotkey = sub_57F240(index);
        if (hotkey->field_8 == 1 || hotkey->field_8 == 4) {
            if (hotkey->field_10.obj != OBJ_HANDLE_NULL) {
                if (!sub_444130(&(hotkey->field_10))) {
                    tig_debug_printf("Intgame: intgame_hotkeys_recover: ERROR: Item Hotkey %d failed to recover!\n", index);
                    sub_57F210(index);
                }
            }
        }
    }
}
