#include "ui/inven_ui.h"

#include <tig/tig.h>

#include "game/lib/object.h"
#include "ui/scrollbar_ui.h"

// 0x572060
void inven_ui_init()
{
    // TODO: Incomplete.
}

// 0x572190
void inven_ui_exit()
{
    tig_font_destroy(dword_682418);
    tig_font_destroy(dword_682C74);
    tig_font_destroy(dword_681390);
    tig_font_destroy(dword_68345C);
    message_unload(inven_ui_mes_file);
}

// 0x5721D0
void inven_ui_reset()
{
    if (inven_ui_created) {
        inven_ui_destroy();
    }

    qword_682C78 = 0;
    qword_6813A8 = 0;
    qword_6814F8 = 0;
    qword_6810E0 = 0;
    qword_681450 = 0;
    qword_681458 = 0;
    dword_6810E8 = -1;
}

// 0x572240
void sub_572240()
{
    // TODO: Incomplete.
}

// 0x572340
bool sub_572340()
{
    if (!inven_ui_created) {
        return false;
    }

    if (qword_6810E0 != 0) {
        sub_575770();
    }

    inven_ui_destroy();

    return true;
}

// 0x572370
void sub_572370()
{
    // TODO: Incomplete.
}

// 0x572510
void sub_572510()
{
    // TODO: Incomplete.
}

// 0x572640
void sub_572640()
{
    // TODO: Incomplete.
}

// 0x5727B0
void inven_ui_create(long long a1, long long a2, int a3)
{
    // TODO: Incomplete.
}

// 0x573440
void inven_ui_destroy()
{
    tig_art_id_t art_id;
    int sound_id;
    int v2;

    if (!inven_ui_created) {
        return;
    }

    inven_ui_created = false;
    sub_4A53B0(qword_6814F8, 0);

    if ((dword_683464 == 2 || dword_683464 == 4)
        && obj_f_get_int32(qword_6813A8, OBJ_F_TYPE) == OBJ_TYPE_CONTAINER) {
        if (dword_683464 == 2) {
            art_id = obj_f_get_int32(qword_6813A8, OBJ_F_CURRENT_AID);
            if (tig_art_id_frame_get(art_id) == 1
                && (obj_f_get_int32(qword_6813A8, OBJ_F_CONTAINER_FLAGS) & OCOF_STICKY) == 0) {
                sub_43F8F0(qword_6813A8);

                sound_id = sub_4F0FD0(qword_6813A8, 1);
                gsound_play_sfx_id(sound_id, 1);
            }
        }
        sub_468180();
        sub_4ED6C0(qword_6813A8);
    }

    sub_550DA0(0, 0);
    sub_579D70();
    intgame_big_window_unlock();
    sub_551160();
    v2 = sub_551A00();
    if (v2 != 8
        && v2 != 10
        && v2 != 11
        && v2 != 4
        && v2 != 19
        && v2 != 20) {
      sub_551A80(0);
    }
    sub_551A80(0);
}

// 0x573590
void sub_573590(long long obj)
{
    if (!inven_ui_created) {
        return;
    }

    if (obj == qword_6814F8
        || obj == qword_6813A8
        || obj == qword_682C78) {
        inven_ui_destroy();
    }
}

// 0x5735F0
int sub_5735F0()
{
    return inven_ui_created;
}

// 0x573600
long long sub_573600()
{
    return inven_ui_created ? qword_682C78 : 0;
}

// 0x573620
long long sub_573620()
{
    return qword_6810E0;
}

// 0x573630
void sub_573630(long long obj)
{
    tig_art_id_t inv_art_id;
    tig_art_id_t mouse_art_id;

    if (!inven_ui_created && qword_6814F8 == 0) {
        qword_6814F8 = sub_40DA50();
    }

    qword_681450 = qword_6814F8;
    qword_6810E0 = obj;
    dword_6810E8 = obj_f_get_int32(obj, OBJ_F_ITEM_INV_LOCATION);

    inv_art_id = obj_f_get_int32(qword_6810E0, OBJ_F_ITEM_INV_AID);
    sub_553990();
    mouse_art_id = tig_mouse_get_cursor_art_id();
    tig_mouse_hide();
    tig_mouse_cursor_set_art_id(inv_art_id);
    tig_mouse_cursor_overlay(mouse_art_id, 2, 2);
    tig_mouse_set_hotspot(2, 2);
    tig_mouse_show();
}

// 0x5736E0
void sub_5736E0()
{
    tig_art_id_t inv_art_id;
    tig_art_id_t mouse_art_id;

    if (qword_6810E0 == 0) {
        return;
    }

    inv_art_id = obj_f_get_int32(qword_6810E0, OBJ_F_ITEM_INV_AID);
    mouse_art_id = tig_mouse_get_cursor_art_id();
    tig_mouse_hide();
    tig_mouse_cursor_set_art_id(inv_art_id);
    tig_mouse_cursor_overlay(mouse_art_id, 2, 2);
    tig_mouse_set_hotspot(2, 2);
    tig_mouse_show();
}

// 0x573730
void sub_573730()
{
    dword_68346C = 0;
}

// 0x573740
void sub_573740()
{
    // TODO: Incomplete.
}

// 0x573840
void sub_573840()
{
    qword_6810E0 = 0;
}

// 0x573850
void inven_ui_message_filter(TigMessage* msg)
{
    // TODO: Incomplete.
}

// 0x574FD0
void sub_574FD0()
{
    // TODO: Incomplete.
}

// 0x575080
void sub_575080()
{
    // TODO: Incomplete.
}

// 0x575100
void sub_575100()
{
    // TODO: Incomplete.
}

// 0x575180
void sub_575180()
{
    // TODO: Incomplete.
}

// 0x575200
void sub_575200()
{
    // TODO: Incomplete.
}

// 0x575360
void sub_575360()
{
    // TODO: Incomplete.
}

// 0x5754C0
void sub_5754C0()
{
    // TODO: Incomplete.
}

// 0x575580
void sub_575580()
{
    // TODO: Incomplete.
}

// 0x5755A0
void sub_5755A0()
{
    // TODO: Incomplete.
}

// 0x575770
void sub_575770()
{
    // TODO: Incomplete.
}

// 0x575930
void sub_575930()
{
    // TODO: Incomplete.
}

// 0x575BE0
void sub_575BE0()
{
    location_t location;
    int sound_id;

    location = obj_f_get_int64(qword_681450, OBJ_F_LOCATION);
    sub_466E50(qword_6810E0, location);

    sound_id = sub_4F0BF0(qword_6810E0, qword_681450, 0, 1);
    gsound_play_sfx_id(sound, 1);
}

// 0x575C50
void sub_575C50()
{
    // TODO: Incomplete.
}

// 0x575CB0
void sub_575CB0()
{
    // TODO: Incomplete.
}

// 0x575FA0
void sub_575FA0()
{
    // TODO: Incomplete.
}

// 0x576100
void sub_576100()
{
    // TODO: Incomplete.
}

// 0x5761D0
void redraw_inven_fix_bad_inven_obj()
{
    // TODO: Incomplete.
}

// 0x5764B0
void redraw_inven_fix_bad_inven()
{
    // TODO: Incomplete.
}

// 0x576520
void sub_576520()
{
    // TODO: Incomplete.
}

// 0x5782D0
void sub_5782D0()
{
    // TODO: Incomplete.
}

// 0x578330
void sub_578330()
{
    // TODO: Incomplete.
}

// 0x5786C0
void sub_5786C0()
{
    // TODO: Incomplete.
}

// 0x578760
void sub_578760()
{
    // TODO: Incomplete.
}

// 0x5788C0
void sub_5788C0()
{
    // TODO: Incomplete.
}

// 0x578B80
void sub_578B80()
{
    // TODO: Incomplete.
}

// 0x578EA0
void sub_578EA0()
{
    // TODO: Incomplete.
}

// 0x579760
long long sub_579760()
{
    return qword_739F68;
}

// 0x579770
void sub_579770()
{
    // TODO: Incomplete.
}

// 0x579840
void sub_579840()
{
    // TODO: Incomplete.
}

// 0x579B60
void sub_579B60()
{
    // TODO: Incomplete.
}

// 0x579C40
void sub_579C40()
{
    ScrollbarUiControlInfo info;

    sub_5811B0();

    if (dword_683464 == 0
        || dword_683464 == 5
        || dword_683464 == 6) {
        return;
    }

    info.flags = 0x39F;
    info.field_3C = sub_579E00;
    info.field_38 = dword_681508;
    info.field_24 = dword_681510;
    info.field_28 = 0;

    if (dword_683464 == 1) {
        info.field_4 = stru_5CAF90;
        info.rect.x = 8;
        info.rect.y = 168;
        info.rect.width = stru_5CAF90.width + 320;
        info.rect.height = 224;
    } else {
        info.field_4 = stru_5CAFA0;
        info.rect.x = 8;
        info.rect.y = 136;
        info.rect.width = stru_5CAFA0.width + 320;
        info.rect.height = 256;
    }

    info.field_40 = sub_579E30;
    info.field_2C = 1;
    scrollbar_ui_control_create(&stru_6810F0, &info, dword_68137C);
    sub_5806F0(stru_6810F0);
}

// 0x579D70
void sub_579D70()
{
    if (dword_683464 == 0
        || dword_683464 == 5
        || dword_683464 == 6) {
        return;
    }

    scrollbar_ui_control_destroy(stru_6810F0);
    sub_5811B0();
}

// 0x579DA0
void sub_579DA0()
{
    if (dword_683464 == 0
        || dword_683464 == 5
        || dword_683464 == 6) {
        return;
    }

    scrollbar_ui_control_show(stru_6810F0);
}

// 0x579DD0
void sub_579DD0()
{
    if (dword_683464 == 0
        || dword_683464 == 5
        || dword_683464 == 6) {
        return;
    }

    scrollbar_ui_control_hide(stru_6810F0);
}

// 0x579E00
void sub_579E00(int a1)
{
    if (dword_683464 == 0
        || dword_683464 == 5
        || dword_683464 == 6) {
        return;
    }

    dword_681508 = a1;
    sub_576520(0);
}

// 0x579E30
void sub_579E30(TigRect* rect)
{
    TigArtBlitSpec blit_info;

    if (dword_683464 == 0
        || dword_683464 == 5
        || dword_683464 == 6) {
        return;
    }

    blit_info.flags = 0;
    blit_info.art_id = sub_5782D0();
    blit_info.src_rect = rect;
    blit_info.dst_rect = rect;
    tig_window_blit_art(dword_68137C, &blit_info);
}
