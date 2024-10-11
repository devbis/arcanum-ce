#include "ui/follower_ui.h"

#include "game/critter.h"
#include "game/mes.h"
#include "game/obj.h"
#include "game/object.h"
#include "game/player.h"
#include "ui/intgame.h"

static void sub_56A6E0(int index);
static bool sub_56A9D0(TigMessage* msg);
static void sub_56AFD0(int a1);
static void sub_56B0F0();
static void sub_56B110(bool a1);
static void sub_56B510(tig_window_handle_t window_handle, int num, int x, int y, int src_scale, int dst_scale);
static void sub_56B620();
static void sub_56B850();
static void sub_56B880();
static void sub_56B970(int a1);

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
static TigRect stru_5CA450 = { 10, 3, 118, 15 };

// 0x67BB38
static tig_button_handle_t dword_67BB38[9];

// 0x67BB5C
static tig_font_handle_t dword_67BB5C;

// 0x67BB60
static tig_window_handle_t dword_67BB60[9];

// 0x67BB84
static int dword_67BB84;

// 0x67BB88
static TigRect stru_67BB88[8];

// 0x67BC08
static FollowerInfo* dword_67BC08;

// 0x67BC10
static int dword_67BC10;

// 0x67BC14
static int dword_67BC14;

// 0x67BC18
static tig_font_handle_t dword_67BC18;

// 0x67BC20
static long long qword_67BC20;

// 0x67BC28
static tig_font_handle_t dword_67BC28;

// 0x67BC2C
static int follower_ui_mes_file;

// 0x67BC30
static tig_button_handle_t dword_67BC30[8];

// 0x67BC50
static long long qword_67BC50;

// 0x67BC58
static int dword_67BC58;

// 0x67BC5C
static bool follower_ui_initialized;

// 0x67BC60
static bool dword_67BC60;

// 0x67BC0C
static tig_window_handle_t dword_67BC0C;

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
        stru_67BB88[index].x = stru_5CA360[index].x + stru_5CA360[index].width;
        stru_67BB88[index].y = stru_5CA360[index].y;
        stru_67BB88[index].width = art_frame_data.width;
        stru_67BB88[index].height = art_frame_data.height;
    }

    dword_67BC14 = 10;
    dword_67BC58 = 0;
    dword_67BC10 = 0;
    dword_67BC08 = (FollowerInfo*)MALLOC(sizeof(*dword_67BC08) * dword_67BC14);

    font_desc.flags = 0;
    tig_art_interface_id_create(229, 0, 0, 0, &(font_desc.art_id));
    font_desc.str = NULL;
    font_desc.color = tig_color_make(255, 255, 255);
    tig_font_create(&font_desc, &dword_67BC28);

    font_desc.flags = 0;
    tig_art_interface_id_create(229, 0, 0, 0, &(font_desc.art_id));
    font_desc.str = NULL;
    font_desc.color = tig_color_make(128, 128, 128);
    tig_font_create(&font_desc, &dword_67BC18);

    font_desc.flags = 0;
    tig_art_interface_id_create(229, 0, 0, 0, &(font_desc.art_id));
    font_desc.str = NULL;
    font_desc.color = tig_color_make(255, 210, 0);
    tig_font_create(&font_desc, &dword_67BB5C);

    follower_ui_initialized = true;
    dword_67BC60 = true;
    dword_67BC0C = TIG_WINDOW_HANDLE_INVALID;

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
        window_data.flags |= TIG_WINDOW_FLAG_0x02;
    } else {
        window_data.message_filter = NULL;
    }

    window_data.rect = stru_5CA360[index];
    tig_window_create(&window_data, &(dword_67BB60[index]));

    button_data.flags = TIG_BUTTON_FLAG_0x01;
    button_data.mouse_down_snd_id = TIG_SOUND_HANDLE_INVALID;
    button_data.mouse_up_snd_id = TIG_SOUND_HANDLE_INVALID;
    button_data.mouse_enter_snd_id = TIG_SOUND_HANDLE_INVALID;
    button_data.mouse_exit_snd_id = TIG_SOUND_HANDLE_INVALID;
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

    if (dword_67BC0C != TIG_WINDOW_HANDLE_INVALID) {
        sub_56B0F0();
    }

    follower_ui_initialized = false;
}

// 0x56A880
void follower_ui_reset()
{
    int index;

    dword_67BC60 = true;
    dword_67BC58 = 0;
    dword_67BC10 = 0;

    for (index = 0; index < 9; index++) {
        tig_window_hide(dword_67BB60[index]);
    }

    if (dword_67BC0C != TIG_WINDOW_HANDLE_INVALID) {
        sub_56B0F0();
    }
}

// 0x56A8D0
void follower_ui_resize(ResizeInfo* resize_info)
{
    TigRect* rects;
    int index;

    (void)resize_info;

    rects = intgame_is_compact_interface() ? stru_5CA420 : stru_5CA3F0;
    for (index = 6; index < 9; index++) {
        stru_5CA360[index] = rects[index];
    }

    for (index = 6; index < 9; index++) {
        tig_window_destroy(dword_67BB60[index]);
    }

    for (index = 6; index < 9; index++) {
        sub_56A6E0(index);
    }

    sub_56B6F0();
    sub_56B620();
    sub_56B620();
}

// 0x56A940
bool follower_ui_load(GameLoadInfo* load_info)
{
    if (tig_file_fread(&dword_67BC60, sizeof(dword_67BC60), 1, load_info->stream) != 1) return false;
    if (tig_file_fread(&dword_67BC10, sizeof(dword_67BC10), 1, load_info->stream) != 1) return false;

    sub_56B6F0();

    return true;
}

// 0x56A990
bool follower_ui_save(TigFile* stream)
{
    if (tig_file_fwrite(&dword_67BC60, sizeof(dword_67BC60), 1, stream) != 1) return false;
    if (tig_file_fwrite(&dword_67BC10, sizeof(dword_67BC10), 1, stream) != 1) return false;

    return true;
}

// 0x56A9D0
bool sub_56A9D0(TigMessage* msg)
{
    // TODO: Incomplete.
}

// 0x56AFD0
void sub_56AFD0(int a1)
{
    TigWindowData window_data;
    TigButtonData button_data;
    int index;

    qword_67BC20 = player_get_pc_obj();
    sub_444130(&(dword_67BC08[a1 + dword_67BC10]));
    qword_67BC50 = dword_67BC08[dword_67BC10 + a1].obj;

    window_data.flags = 0;
    window_data.rect = stru_67BB88[a1];
    tig_window_create(&window_data, &dword_67BC0C);

    sub_56B970(-1);

    button_data.x = stru_5CA450.x;
    button_data.y = stru_5CA450.y;
    button_data.mouse_down_snd_id = TIG_SOUND_HANDLE_INVALID;
    button_data.mouse_up_snd_id = TIG_SOUND_HANDLE_INVALID;
    button_data.mouse_enter_snd_id = TIG_SOUND_HANDLE_INVALID;
    button_data.mouse_exit_snd_id = TIG_SOUND_HANDLE_INVALID;
    button_data.art_id = TIG_ART_ID_INVALID;
    button_data.flags = TIG_BUTTON_FLAG_0x01;
    button_data.width = stru_5CA450.width;
    button_data.height = stru_5CA450.height;
    button_data.window_handle = dword_67BC0C;

    for (index = 0; index < 8; index++) {
        tig_button_create(&button_data, &(dword_67BC30[index]));
        button_data.y += 18;
    }
}

// 0x56B0F0
void sub_56B0F0()
{
    if (dword_67BC0C != TIG_WINDOW_HANDLE_INVALID) {
        tig_window_destroy(dword_67BC0C);
        dword_67BC0C = TIG_WINDOW_HANDLE_INVALID;
    }
}

// 0x56B110
void sub_56B110(bool a1)
{
    sub_551A80(18);
    if (!a1) {
        sub_4F25B0(0, 0x8000);
        return;
    }

    if (tig_kb_is_key_pressed(DIK_LMENU) || tig_kb_is_key_pressed(DIK_RMENU)) {
        sub_4F25B0(0x10000004, 0x200);
        return;
    }

    sub_4F25B0(0x90000004, 0x10000200);
}

// 0x56B180
void sub_56B180()
{
    // TODO: Incomplete.
}

// 0x56B280
void sub_56B280()
{
    sub_551A80(0);
}

// 0x56B290
void sub_56B290()
{
    // TODO: Incomplete.
}

// 0x56B4D0
void sub_56B4D0(int64_t obj)
{
    if (dword_67BC60) {
        if (sub_45DDA0(obj) == player_get_pc_obj()) {
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
void sub_56B620()
{
    int index;
    tig_art_id_t art_id;

    dword_67BC60 = !dword_67BC60;
    if (dword_67BC60) {
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
void sub_56B6F0()
{
    // TODO: Incomplete.
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

    if (dword_67BC58 > 6 && dword_67BC60) {
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
void sub_56B970(int a1)
{
    TigRect rect;
    int index;
    MesFileEntry mes_file_entry;

    sub_56B510(dword_67BC0C, 826, 0, 0, 100, 100);

    rect = stru_5CA450;
    for (index = 0; index < 8; index++) {
        mes_file_entry.num = index;
        if (mes_search(follower_ui_mes_file, &mes_file_entry)) {
            if ((index == 6 && sub_575080(qword_67BC20, qword_67BC50))
                || (index != 5 || (obj_field_int32_get(qword_67BC50, OBJ_F_SPELL_FLAGS) & OSF_MIND_CONTROLLED) == 0)) {
                if (index == a1) {
                    tig_font_push(dword_67BB5C);
                } else {
                    tig_font_push(dword_67BC28);
                }
            } else {
                tig_font_push(dword_67BC18);
            }
            tig_window_text_write(dword_67BC0C, mes_file_entry.str, &rect);
            tig_font_pop();
        }
        rect.y += 18;
    }
}
