#include "ui/follower_ui.h"

#include <stdio.h>

#include "game/broadcast.h"
#include "game/critter.h"
#include "game/mes.h"
#include "game/obj.h"
#include "game/object.h"
#include "game/player.h"
#include "game/portrait.h"
#include "game/stat.h"
#include "game/target.h"
#include "ui/charedit_ui.h"
#include "ui/dialog_ui.h"
#include "ui/intgame.h"
#include "ui/inven_ui.h"
#include "ui/tb_ui.h"

typedef enum FollowerUiCommand {
    FOLLOWER_UI_COMMAND_WALK,
    FOLLOWER_UI_COMMAND_ATTACK,
    FOLLOWER_UI_COMMAND_STAY_CLOSE,
    FOLLOWER_UI_COMMAND_SPREAD_OUT,
    FOLLOWER_UI_COMMAND_BACK_OFF,
    FOLLOWER_UI_COMMAND_WAIT,
    FOLLOWER_UI_COMMAND_INVENTORY,
    FOLLOWER_UI_COMMAND_CHARACTER_SHEET,
    FOLLOWER_UI_COMMAND_COUNT,
} FollowerUiCommand;

static void sub_56A6E0(int index);
static bool sub_56A9D0(TigMessage* msg);
static void follower_ui_drop_down_menu_create(int index);
static void follower_ui_drop_down_menu_destroy();
static void follower_ui_begin_order_mode(int cmd);
static void sub_56B510(tig_window_handle_t window_handle, int num, int x, int y, int src_scale, int dst_scale);
static void follower_ui_toggle();
static void sub_56B850();
static void sub_56B880();
static void follower_ui_drop_down_menu_refresh(int highlighted_cmd);

// 0x5CA360
static TigRect stru_5CA360[9] = {
    { 11, 50, 40, 49 },
    { 11, 112, 40, 49 },
    { 11, 174, 40, 49 },
    { 11, 236, 40, 49 },
    { 11, 298, 40, 49 },
    { 11, 360, 40, 49 },
    { 0, 428, 67, 13 },
    { 0, 415, 33, 13 },
    { 33, 415, 34, 13 },
};

// 0x5CA3F0
static TigRect stru_5CA3F0[3] = {
    { 0, 428, 67, 13 },
    { 0, 415, 33, 13 },
    { 33, 415, 34, 13 },
};

// 0x5CA420
static TigRect stru_5CA420[3] = {
    { 0, 587, 67, 13 },
    { 0, 574, 33, 13 },
    { 33, 574, 34, 13 },
};

// 0x5CA450
static TigRect follower_ui_drop_down_menu_entry_rect = { 10, 3, 118, 15 };

// 0x67BB38
static tig_button_handle_t dword_67BB38[9];

// 0x67BB5C
static tig_font_handle_t follower_ui_drop_down_menu_item_highlighted_color;

// 0x67BB60
static tig_window_handle_t dword_67BB60[9];

// 0x67BB84
static int dword_67BB84;

// 0x67BB88
static TigRect follower_ui_drop_down_menu_rects[8];

// 0x67BC08
static FollowerInfo* dword_67BC08;

// 0x67BC10
static int dword_67BC10;

// 0x67BC14
static int dword_67BC14;

// 0x67BC18
static tig_font_handle_t follower_ui_drop_down_menu_item_disabled_color;

// 0x67BC20
static int64_t follower_ui_commander_obj;

// 0x67BC28
static tig_font_handle_t follower_ui_drop_down_menu_item_normal_color;

// 0x67BC2C
static int follower_ui_mes_file;

// 0x67BC30
static tig_button_handle_t follower_ui_drop_menu_item_buttons[FOLLOWER_UI_COMMAND_COUNT];

// 0x67BC50
static int64_t follower_ui_subordinate_obj;

// 0x67BC58
static int dword_67BC58;

// 0x67BC5C
static bool follower_ui_initialized;

// NOTE: It's `bool`, but needs to be 4 byte integer because of saving/reading
// compatibility.
//
// 0x67BC60
static int follower_ui_visible;

// 0x67BC64
static bool dword_67BC64;

// 0x67BC0C
static tig_window_handle_t follower_ui_drop_down_menu_window;

// 0x56A4A0
bool follower_ui_init(GameInitInfo* init_info)
{
    int index;
    tig_art_id_t art_id;
    TigArtFrameData art_frame_data;
    TigFont font_desc;

    (void)init_info;

    if (!mes_load("mes\\follower_ui.mes", &follower_ui_mes_file)) {
        return false;
    }

    for (index = 0; index < 9; index++) {
        sub_56A6E0(index);
    }

    tig_art_interface_id_create(826, 0, 0, 0, &art_id);
    tig_art_frame_data(art_id, &art_frame_data);

    for (index = 0; index < 6; index++) {
        follower_ui_drop_down_menu_rects[index].x = stru_5CA360[index].x + stru_5CA360[index].width;
        follower_ui_drop_down_menu_rects[index].y = stru_5CA360[index].y;
        follower_ui_drop_down_menu_rects[index].width = art_frame_data.width;
        follower_ui_drop_down_menu_rects[index].height = art_frame_data.height;
    }

    dword_67BC14 = 10;
    dword_67BC58 = 0;
    dword_67BC10 = 0;
    dword_67BC08 = (FollowerInfo*)MALLOC(sizeof(*dword_67BC08) * dword_67BC14);

    font_desc.flags = 0;
    tig_art_interface_id_create(229, 0, 0, 0, &(font_desc.art_id));
    font_desc.str = NULL;
    font_desc.color = tig_color_make(255, 255, 255);
    tig_font_create(&font_desc, &follower_ui_drop_down_menu_item_normal_color);

    font_desc.flags = 0;
    tig_art_interface_id_create(229, 0, 0, 0, &(font_desc.art_id));
    font_desc.str = NULL;
    font_desc.color = tig_color_make(128, 128, 128);
    tig_font_create(&font_desc, &follower_ui_drop_down_menu_item_disabled_color);

    font_desc.flags = 0;
    tig_art_interface_id_create(229, 0, 0, 0, &(font_desc.art_id));
    font_desc.str = NULL;
    font_desc.color = tig_color_make(255, 210, 0);
    tig_font_create(&font_desc, &follower_ui_drop_down_menu_item_highlighted_color);

    follower_ui_initialized = true;
    follower_ui_visible = true;
    follower_ui_drop_down_menu_window = TIG_WINDOW_HANDLE_INVALID;

    return true;
}

// 0x56A6E0
void sub_56A6E0(int index)
{
    TigWindowData window_data;
    TigButtonData button_data;

    window_data.flags = TIG_WINDOW_HIDDEN;
    if (index == 6) {
        window_data.message_filter = sub_56A9D0;
        window_data.flags |= TIG_WINDOW_MESSAGE_FILTER;
    } else {
        window_data.message_filter = NULL;
    }

    window_data.rect = stru_5CA360[index];
    tig_window_create(&window_data, &(dword_67BB60[index]));

    button_data.flags = TIG_BUTTON_FLAG_0x01;
    button_data.mouse_down_snd_id = -1;
    button_data.mouse_up_snd_id = -1;
    button_data.mouse_enter_snd_id = -1;
    button_data.mouse_exit_snd_id = -1;
    button_data.x = 0;
    button_data.y = 0;

    switch (index) {
    case 6:
        tig_art_interface_id_create(501, 0, 0, 0, &(button_data.art_id));
        break;
    case 7:
        tig_art_interface_id_create(506, 0, 0, 0, &(button_data.art_id));
        break;
    case 8:
        tig_art_interface_id_create(508, 0, 0, 0, &(button_data.art_id));
        break;
    default:
        button_data.art_id = TIG_ART_ID_INVALID;
        button_data.width = stru_5CA360[index].width;
        button_data.height = stru_5CA360[index].height;
        break;
    }

    button_data.window_handle = dword_67BB60[index];
    tig_button_create(&button_data, &(dword_67BB38[index]));
}

// 0x56A820
void follower_ui_exit()
{
    int index;

    mes_unload(follower_ui_mes_file);

    for (index = 0; index < 9; index++) {
        tig_window_destroy(dword_67BB60[index]);
    }

    FREE(dword_67BC08);

    if (follower_ui_drop_down_menu_window != TIG_WINDOW_HANDLE_INVALID) {
        follower_ui_drop_down_menu_destroy();
    }

    follower_ui_initialized = false;

    // FIX: Memory leak.
    tig_font_destroy(follower_ui_drop_down_menu_item_normal_color);
    tig_font_destroy(follower_ui_drop_down_menu_item_disabled_color);
    tig_font_destroy(follower_ui_drop_down_menu_item_highlighted_color);
}

// 0x56A880
void follower_ui_reset()
{
    int index;

    follower_ui_visible = true;
    dword_67BC58 = 0;
    dword_67BC10 = 0;

    for (index = 0; index < 9; index++) {
        tig_window_hide(dword_67BB60[index]);
    }

    if (follower_ui_drop_down_menu_window != TIG_WINDOW_HANDLE_INVALID) {
        follower_ui_drop_down_menu_destroy();
    }
}

// 0x56A8D0
void follower_ui_resize(GameResizeInfo* resize_info)
{
    TigRect* rects;
    int index;

    (void)resize_info;

    rects = intgame_is_compact_interface() ? stru_5CA420 : stru_5CA3F0;
    for (index = 6; index < 9; index++) {
        stru_5CA360[index] = rects[index - 6];
    }

    for (index = 6; index < 9; index++) {
        tig_window_destroy(dword_67BB60[index]);
    }

    for (index = 6; index < 9; index++) {
        sub_56A6E0(index);
    }

    follower_ui_refresh();
    follower_ui_toggle();
    follower_ui_toggle();
}

// 0x56A940
bool follower_ui_load(GameLoadInfo* load_info)
{
    if (tig_file_fread(&follower_ui_visible, sizeof(follower_ui_visible), 1, load_info->stream) != 1) return false;
    if (tig_file_fread(&dword_67BC10, sizeof(dword_67BC10), 1, load_info->stream) != 1) return false;

    follower_ui_refresh();

    return true;
}

// 0x56A990
bool follower_ui_save(TigFile* stream)
{
    if (tig_file_fwrite(&follower_ui_visible, sizeof(follower_ui_visible), 1, stream) != 1) return false;
    if (tig_file_fwrite(&dword_67BC10, sizeof(dword_67BC10), 1, stream) != 1) return false;

    return true;
}

// 0x56A9D0
bool sub_56A9D0(TigMessage* msg)
{
    int64_t pc_obj;
    int index;
    MesFileEntry mes_file_entry;
    char str[MAX_STRING];
    Broadcast bcast;
    S4F2810 v1;

    pc_obj = player_get_pc_obj();
    if (sub_567400(pc_obj)) {
        return false;
    }

    if (follower_ui_drop_down_menu_window != TIG_WINDOW_HANDLE_INVALID) {
        switch (msg->type) {
        case TIG_MESSAGE_BUTTON:
            switch (msg->data.button.state) {
            case TIG_BUTTON_STATE_MOUSE_INSIDE:
                for (index = 0; index < FOLLOWER_UI_COMMAND_COUNT; index++) {
                    if (msg->data.button.button_handle == follower_ui_drop_menu_item_buttons[index]) {
                        follower_ui_drop_down_menu_refresh(index);
                        return true;
                    }
                }
                return false;
            case TIG_BUTTON_STATE_MOUSE_OUTSIDE:
                for (index = 0; index < FOLLOWER_UI_COMMAND_COUNT; index++) {
                    if (msg->data.button.button_handle == follower_ui_drop_menu_item_buttons[index]) {
                        follower_ui_drop_down_menu_refresh(-1);
                        return true;
                    }
                }
                return false;
            case TIG_BUTTON_STATE_RELEASED:
                for (index = 0; index < FOLLOWER_UI_COMMAND_COUNT; index++) {
                    if (msg->data.button.button_handle == follower_ui_drop_menu_item_buttons[index]) {
                        follower_ui_drop_down_menu_destroy();
                        switch (index) {
                        case FOLLOWER_UI_COMMAND_WALK:
                        case FOLLOWER_UI_COMMAND_ATTACK:
                            follower_ui_begin_order_mode(index);
                            break;
                        case FOLLOWER_UI_COMMAND_INVENTORY:
                            if (sub_575080(follower_ui_commander_obj, follower_ui_subordinate_obj)) {
                                inven_ui_open(follower_ui_commander_obj, follower_ui_subordinate_obj, INVEN_UI_MODE_BARTER);
                            }
                            break;
                        case FOLLOWER_UI_COMMAND_CHARACTER_SHEET:
                            charedit_open(follower_ui_subordinate_obj, CHAREDIT_MODE_PASSIVE);
                            break;
                        case FOLLOWER_UI_COMMAND_WAIT:
                            if ((obj_field_int32_get(follower_ui_subordinate_obj, OBJ_F_SPELL_FLAGS) & OSF_MIND_CONTROLLED) != 0) {
                                return true;
                            }
                            // FALLTHROUGH
                        default:
                            mes_file_entry.num = index;
                            if (mes_search(follower_ui_mes_file, &mes_file_entry)) {
                                sub_441B60(follower_ui_subordinate_obj, follower_ui_commander_obj, str);
                                sprintf(bcast.field_8, "%s %s", str, mes_file_entry.str);
                                broadcast_msg(follower_ui_commander_obj, &bcast);
                            }
                            break;
                        }
                        return true;
                    }
                }
                return false;
            }
            break;
        case TIG_MESSAGE_MOUSE:
            switch (msg->data.mouse.event) {
            case TIG_MESSAGE_MOUSE_LEFT_BUTTON_UP:
            case TIG_MESSAGE_MOUSE_RIGHT_BUTTON_UP:
                follower_ui_drop_down_menu_destroy();
                break;
            }
            break;
        }

        return false;
    }

    switch (msg->type) {
    case TIG_MESSAGE_BUTTON:
        switch (msg->data.button.state) {
        case TIG_BUTTON_STATE_MOUSE_INSIDE:
            for (index = 0; index < 6; index++) {
                if (msg->data.button.button_handle == dword_67BB38[index]) {
                    follower_ui_commander_obj = player_get_pc_obj();
                    sub_444130(&dword_67BC08[dword_67BC10 + index]);
                    follower_ui_subordinate_obj = dword_67BC08[dword_67BC10 + index].obj;
                    if (follower_ui_subordinate_obj != OBJ_HANDLE_NULL) {
                        sub_57CD60(follower_ui_commander_obj, follower_ui_subordinate_obj, str);
                        sub_553BE0(follower_ui_commander_obj, follower_ui_subordinate_obj, str);
                        sub_43C270(follower_ui_subordinate_obj);
                    }
                    return true;
                }
            }
            return false;
        case TIG_BUTTON_STATE_MOUSE_OUTSIDE:
            for (index = 0; index < 9; index++) {
                if (msg->data.button.button_handle == dword_67BB38[index]) {
                    sub_550720();
                    sub_43C270(OBJ_HANDLE_NULL);
                    return true;
                }
            }
            return false;
        case TIG_BUTTON_STATE_RELEASED:
            if (msg->data.button.button_handle == dword_67BB38[6]) {
                follower_ui_toggle();
                return true;
            }

            if (msg->data.button.button_handle == dword_67BB38[7]) {
                if (dword_67BC10 > 0) {
                    dword_67BC10--;
                    sub_56B880();
                    sub_56B290();
                }
                return true;
            }

            if (msg->data.button.button_handle == dword_67BB38[8]) {
                if (dword_67BC10 < dword_67BC58 - 6) {
                    dword_67BC10++;
                    sub_56B880();
                    sub_56B290();
                }
                return true;
            }

            for (index = 0; index < 9; index++) {
                if (msg->data.button.button_handle == dword_67BB38[index]) {
                    sub_444130(&dword_67BC08[dword_67BC10 + index]);
                    sub_4F2810(&v1, dword_67BC08[dword_67BC10 + index].obj);
                    sub_54EA80(&v1);
                    return true;
                }
            }
            return false;
        }
        return false;
    case TIG_MESSAGE_MOUSE:
        if (msg->data.mouse.event == TIG_MESSAGE_MOUSE_RIGHT_BUTTON_UP && follower_ui_visible) {
            tig_window_handle_t window_handle;

            for (index = 0; index < 6; index++) {
                if (dword_67BC10 + index >= dword_67BC58) {
                    break;
                }

                if (msg->data.mouse.x >= stru_5CA360[index].x
                    && msg->data.mouse.y >= stru_5CA360[index].y
                    && msg->data.mouse.x < stru_5CA360[index].x + stru_5CA360[index].width
                    && msg->data.mouse.y < stru_5CA360[index].y + stru_5CA360[index].height) {
                    if (tig_window_get_at_position(msg->data.mouse.x, msg->data.mouse.y, &window_handle) == TIG_OK
                        && window_handle == dword_67BB60[index]) {
                        follower_ui_drop_down_menu_create(index);
                    }
                    break;
                }
            }
        }
        return false;
    default:
        return false;
    }
}

// 0x56AFD0
void follower_ui_drop_down_menu_create(int index)
{
    TigWindowData window_data;
    TigButtonData button_data;
    int cmd;

    follower_ui_commander_obj = player_get_pc_obj();

    sub_444130(&(dword_67BC08[dword_67BC10 + index]));
    follower_ui_subordinate_obj = dword_67BC08[dword_67BC10 + index].obj;

    window_data.flags = 0;
    window_data.rect = follower_ui_drop_down_menu_rects[index];
    tig_window_create(&window_data, &follower_ui_drop_down_menu_window);

    follower_ui_drop_down_menu_refresh(-1);

    button_data.x = follower_ui_drop_down_menu_entry_rect.x;
    button_data.y = follower_ui_drop_down_menu_entry_rect.y;
    button_data.mouse_down_snd_id = -1;
    button_data.mouse_up_snd_id = -1;
    button_data.mouse_enter_snd_id = -1;
    button_data.mouse_exit_snd_id = -1;
    button_data.art_id = TIG_ART_ID_INVALID;
    button_data.flags = TIG_BUTTON_FLAG_0x01;
    button_data.width = follower_ui_drop_down_menu_entry_rect.width;
    button_data.height = follower_ui_drop_down_menu_entry_rect.height;
    button_data.window_handle = follower_ui_drop_down_menu_window;

    for (cmd = 0; cmd < FOLLOWER_UI_COMMAND_COUNT; cmd++) {
        tig_button_create(&button_data, &(follower_ui_drop_menu_item_buttons[cmd]));
        button_data.y += 18;
    }
}

// 0x56B0F0
void follower_ui_drop_down_menu_destroy()
{
    if (follower_ui_drop_down_menu_window != TIG_WINDOW_HANDLE_INVALID) {
        tig_window_destroy(follower_ui_drop_down_menu_window);
        follower_ui_drop_down_menu_window = TIG_WINDOW_HANDLE_INVALID;
    }
}

// 0x56B110
void follower_ui_begin_order_mode(int cmd)
{
    uint64_t tgt;

    sub_551A80(18);

    if (cmd == FOLLOWER_UI_COMMAND_WALK) {
        tgt = Tgt_Tile;
    } else {
        if (tig_kb_is_key_pressed(DIK_LMENU) || tig_kb_is_key_pressed(DIK_RMENU)) {
            tgt = Tgt_Obj_No_Self | Tgt_Obj_No_T_Wall;
        } else {
            tgt = Tgt_Obj_No_ST_Critter_Dead | Tgt_Obj_No_Self | Tgt_Obj_No_T_Wall | Tgt_Non_Party_Critters;
        }
    }

    sub_4F25B0(tgt);
}

// 0x56B180
void follower_ui_execute_order(S4F2810* a1)
{
    int num;
    Broadcast bcast;
    MesFileEntry mes_file_entry;
    char str[MAX_STRING];

    follower_ui_end_order_mode();

    if (!critter_is_dead(follower_ui_commander_obj) && !critter_is_unconscious(follower_ui_commander_obj)) {
        if (a1->is_loc) {
            // Walk
            num = 0;
            bcast.loc = a1->loc;
        } else {
            // Attack
            num = 1;
            bcast.loc = obj_field_int64_get(a1->obj, OBJ_F_LOCATION);
        }

        mes_file_entry.num = num;
        if (mes_search(follower_ui_mes_file, &mes_file_entry)) {
            sub_441B60(follower_ui_subordinate_obj, follower_ui_commander_obj, str);
            sprintf(bcast.field_8, "%s %s", str, mes_file_entry.str);
            broadcast_msg(follower_ui_commander_obj, &bcast);
        }
    }
}

// 0x56B280
void follower_ui_end_order_mode()
{
    sub_551A80(0);
}

// 0x56B290
void sub_56B290()
{
    int64_t pc_obj;
    int64_t follower_obj;
    tig_color_t color;
    TigRect rect;
    int index;
    int portrait;
    int hp;
    int fatigue;

    if (!follower_ui_visible) {
        return;
    }

    dword_67BC64 = true;

    pc_obj = player_get_pc_obj();
    color = tig_color_make(255, 0, 0);

    rect.x = 4;
    rect.y = 4;
    rect.width = 32;
    rect.height = 32;

    for (index = 0; index < 6; index++) {
        if (dword_67BC10 + index >= dword_67BC58) {
            break;
        }

        sub_444130(&(dword_67BC08[dword_67BC10 + index]));

        follower_obj = dword_67BC08[dword_67BC10 + index].obj;
        if (follower_obj == OBJ_HANDLE_NULL) {
            follower_ui_refresh();
            break;
        }

        sub_56B510(dword_67BB60[index], 503, 0, 0, 100, 100);

        if (sub_553D10(pc_obj, follower_obj, &portrait)) {
            portrait_draw_32x32(follower_obj,
                portrait,
                dword_67BB60[index],
                4,
                4);
        } else {
            sub_56B510(dword_67BB60[index], portrait, 4, 4, 100, 50);
        }

        hp = 100 * object_hp_current(follower_obj) / object_hp_max(follower_obj);
        if (stat_level_get(follower_obj, STAT_POISON_LEVEL) > 20) {
            sub_56B510(dword_67BB60[index], 616, 3, 39, hp, 100);
        } else {
            sub_56B510(dword_67BB60[index], 505, 3, 39, hp, 100);
        }

        if (hp < 20) {
            tig_window_tint(dword_67BB60[index], &rect, color, 0);
        }

        fatigue = 100 * critter_fatigue_current(follower_obj) / critter_fatigue_max(follower_obj);
        sub_56B510(dword_67BB60[index], 504, 3, 44, fatigue, 100);
    }

    dword_67BC64 = false;
}

// 0x56B4D0
void sub_56B4D0(int64_t obj)
{
    if (follower_ui_visible) {
        if (critter_pc_leader_get(obj) == player_get_pc_obj()) {
            sub_56B290();
        }
    }
}

// 0x56B510
void sub_56B510(tig_window_handle_t window_handle, int num, int x, int y, int src_scale, int dst_scale)
{
    tig_art_id_t art_id;
    TigArtFrameData art_frame_data;
    TigRect src_rect;
    TigRect dst_rect;
    TigArtBlitInfo blit_info;

    if (src_scale <= 0) {
        return;
    }

    if (src_scale > 100) {
        src_scale = 100;
    }

    if (dst_scale <= 0) {
        return;
    }

    tig_art_interface_id_create(num, 0, 0, 0, &art_id);
    tig_art_frame_data(art_id, &art_frame_data);

    src_rect.x = 0;
    src_rect.y = 0;
    src_rect.width = src_scale * art_frame_data.width / 100;
    // NOTE: Why height is not scaled?
    src_rect.height = art_frame_data.height;

    dst_rect.x = x;
    dst_rect.y = y;
    dst_rect.width = dst_scale * src_rect.width / 100;
    dst_rect.height = dst_scale * src_rect.height / 100;

    blit_info.flags = 0;
    blit_info.art_id = art_id;
    blit_info.src_rect = &src_rect;
    blit_info.dst_rect = &dst_rect;
    tig_window_blit_art(window_handle, &blit_info);
}

// 0x56B620
void follower_ui_toggle()
{
    int index;
    tig_art_id_t art_id;

    follower_ui_visible = !follower_ui_visible;
    if (follower_ui_visible) {
        for (index = 0; index < 6 && index < dword_67BC58; index++) {
            tig_window_show(dword_67BB60[index]);
        }

        tig_art_interface_id_create(501, 0, 0, 0, &art_id);
        tig_button_set_art(dword_67BB38[6], art_id);
        sub_56B850();
        sub_56B880();
        sub_56B290();
    } else {
        for (index = 0; index < 9; index++) {
            tig_window_hide(dword_67BB60[index]);
        }

        tig_art_interface_id_create(502, 0, 0, 0, &art_id);
        tig_button_set_art(dword_67BB38[6], art_id);
        sub_56B850();
        sub_56B880();
    }
}

// 0x56B6F0
void follower_ui_refresh()
{
    int index;
    int64_t pc_obj;
    ObjectList followers;
    ObjectNode* node;

    if (!follower_ui_initialized) {
        return;
    }

    for (index = 0; index < 9; index++) {
        tig_window_hide(dword_67BB60[index]);
    }

    dword_67BC58 = 0;
    pc_obj = player_get_pc_obj();
    object_list_followers(pc_obj, &followers);
    node = followers.head;
    while (node != NULL) {
        dword_67BC58++;
        node = node->next;
    }

    if (dword_67BC58 > dword_67BC14) {
        dword_67BC14 = dword_67BC58 + 10;
        dword_67BC08 = (FollowerInfo*)REALLOC(dword_67BC08, sizeof(*dword_67BC08) * dword_67BC14);
    }

    index = 0;
    node = followers.head;
    while (node != NULL) {
        sub_4440E0(node->obj, &(dword_67BC08[index++]));
        node = node->next;
    }
    dword_67BC58 = index;

    object_list_destroy(&followers);

    if (follower_ui_visible) {
        for (index = 0; index < 6; index++) {
            if (index >= dword_67BC58) {
                break;
            }

            tig_window_show(dword_67BB60[index]);
        }
    }

    if (dword_67BC58 > 6) {
        if (dword_67BC10 > dword_67BC58 - 6) {
            dword_67BC10 = dword_67BC58 - 6;
        }
    } else {
        dword_67BC10 = 0;
    }

    sub_56B850();
    sub_56B880();

    if (!dword_67BC64) {
        sub_56B290();
    }
}

// 0x56B850
void sub_56B850()
{
    if (dword_67BC58 > 0) {
        tig_window_show(dword_67BB60[6]);
    } else {
        tig_window_hide(dword_67BB60[6]);
    }
}

// 0x56B880
void sub_56B880()
{
    tig_art_id_t art_id;

    if (dword_67BC58 > 6 && follower_ui_visible) {
        if (dword_67BC10 != 0) {
            tig_art_interface_id_create(506, 0, 0, 0, &art_id);
        } else {
            tig_art_interface_id_create(507, 0, 0, 0, &art_id);
        }
        tig_button_set_art(dword_67BB38[7], art_id);
        tig_window_show(dword_67BB60[7]);

        if (dword_67BC10 != dword_67BC58 - 6) {
            tig_art_interface_id_create(508, 0, 0, 0, &art_id);
        } else {
            tig_art_interface_id_create(509, 0, 0, 0, &art_id);
        }
        tig_button_set_art(dword_67BB38[8], art_id);
        tig_window_show(dword_67BB60[8]);
    } else {
        tig_window_hide(dword_67BB60[7]);
        tig_window_hide(dword_67BB60[8]);
    }
}

// 0x56B970
void follower_ui_drop_down_menu_refresh(int highlighted_cmd)
{
    TigRect rect;
    int cmd;
    MesFileEntry mes_file_entry;

    sub_56B510(follower_ui_drop_down_menu_window, 826, 0, 0, 100, 100);

    rect = follower_ui_drop_down_menu_entry_rect;
    for (cmd = 0; cmd < FOLLOWER_UI_COMMAND_COUNT; cmd++) {
        mes_file_entry.num = cmd;
        if (mes_search(follower_ui_mes_file, &mes_file_entry)) {
            if ((cmd == FOLLOWER_UI_COMMAND_INVENTORY && sub_575080(follower_ui_commander_obj, follower_ui_subordinate_obj))
                || (cmd != FOLLOWER_UI_COMMAND_WAIT || (obj_field_int32_get(follower_ui_subordinate_obj, OBJ_F_SPELL_FLAGS) & OSF_MIND_CONTROLLED) == 0)) {
                if (cmd == highlighted_cmd) {
                    tig_font_push(follower_ui_drop_down_menu_item_highlighted_color);
                } else {
                    tig_font_push(follower_ui_drop_down_menu_item_normal_color);
                }
            } else {
                tig_font_push(follower_ui_drop_down_menu_item_disabled_color);
            }
            tig_window_text_write(follower_ui_drop_down_menu_window, mes_file_entry.str, &rect);
            tig_font_pop();
        }
        rect.y += 18;
    }
}
