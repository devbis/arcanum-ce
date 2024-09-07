#include "ui/charedit_ui.h"

#include "game/mes.h"
#include "ui/scrollbar_ui.h"

static void sub_55C2E0(int a1);
static void sub_55EBA0();
static void sub_55EFB0();
static void sub_55EFE0();
static void sub_55EFF0();
static void sub_55F0D0();
static void sub_55F0E0(int value);
static void sub_55F110(TigRect* rect);

// 0x5C8F40
static TigRect stru_5C8F40 = { 209, 60, 17, 255 };

// 0x5C8F70
static int dword_5C8F70[] = {
    8,
    8,
    8,
    8,
    8,
    8,
    8,
};

// 0x5C8F8C
static int dword_5C8F8C[] = {
    96,
    129,
    162,
    195,
    228,
    261,
    294,
};

// 0x5C8FA8
static int dword_5C8FA8[] = {
    653,
    649,
    650,
    651,
    652,
    654,
    655,
    656,
}:

// 0x64C7A0
static tig_font_handle_t dword_64C7A0;

// 0x64C7A8
static ScrollbarId stru_64C7A8;

// 0x64C7B0
static tig_window_handle_t dword_64C7B0;

// 0x64C828
static tig_font_handle_t dword_64C828;

// 0x64C840
static tig_font_handle_t dword_64C840;

// 0x64C848
static tig_font_handle_t dword_64C848;

// 0x64C9D0
static tig_font_handle_t dword_64C9D0;

// 0x64CA60
static tig_window_handle_t dword_64CA60;

// 0x64CA68
static tig_font_handle_t dword_64CA68;

// 0x64CA6C
static tig_window_handle_t dword_64CA6C;

// 0x64CA70
static mes_file_handle_t charedit_mes_file;

// 0x64CA8C
static tig_window_handle_t dword_64CA8C;

// 0x64CDB0
static tig_font_handle_t dword_64CDB0;

// 0x64CDB8
static tig_font_handle_t dword_64CDB8;

// 0x64CDBC
static tig_font_handle_t dword_64CDBC;

// 0x64CDC0
static tig_font_handle_t dword_64CDC0;

// 0x64CDC8
static int dword_64CDC8;

// 0x64CDD0
static tig_font_handle_t dword_64CDD0;

// 0x64CFE0
static tig_font_handle_t dword_64CFE0;

// 0x64D3A4
static tig_font_handle_t dword_64D3A4;

// 0x64D3A8
static tig_font_handle_t dword_64D3A8;

// 0x64D3B0
static tig_font_handle_t dword_64D3B0;

// 0x64D3BC
static tig_font_handle_t dword_64D3BC;

// 0x64D420
static tig_font_handle_t dword_64D420;

// 0x64D424
static int dword_64D424;

// 0x64D42C
static tig_font_handle_t dword_64D42C;

// 0x64DEE4
static bool dword_64DEE4;

// 0x64DF0C
static tig_font_handle_t dword_64DF0C;

// 0x64E018
static bool dword_64E018;

// 0x64E01C
static int dword_64E01C;

// 0x64E028
static int dword_64E028;

// 0x559690
bool charedit_init(GameInitInfo* init_info)
{
    (void)init_info;

    if (!sub_55E110()) {
        return false;
    }

    if (!sub_55D060()) {
        sub_55EBA0();
        return false;
    }

    if (!sub_55C890()) {
        sub_55EBA0();
        tig_window_destroy(dword_64CA60);
        return false;
    }

    if (!sub_55C110()) {
        sub_55EBA0();
        tig_window_destroy(dword_64C7B0);
        tig_window_destroy(dword_64CA60);
        return false;
    }

    if (!sub_55BAB0()) {
        sub_55EBA0();
        tig_window_destroy(dword_64C7B0);
        tig_window_destroy(dword_64CA8C);
        tig_window_destroy(dword_64CA60);
        return false;
    }

    tig_window_hide(dword_64CA6C);
    tig_window_hide(dword_64C7B0);
    tig_window_hide(dword_64CA8C);
    tig_window_hide(dword_64CA60);

    return true;
}

// 0x559770
void charedit_exit()
{
    sub_55EBA0();
    tig_window_destroy(dword_64CA6C);
    tig_window_destroy(dword_64C7B0);
    tig_window_destroy(dword_64CA8C);
    tig_window_destroy(dword_64CA60);
}

// 0x5597B0
void charedit_reset()
{
    if (sub_55A220()) {
        sub_55A150();
    }
}

// 0x5597C0
void sub_5597C0()
{
    // TODO: Incomplete.
}

// 0x55A150
void sub_55A150()
{
    // TODO: Incomplete.
}

// 0x55A220
bool sub_55A220()
{
    return dword_64E018;
}

// 0x55A230
void sub_55A230()
{
    if (dword_64E018) {
        sub_55B150();
    }
}

// 0x55A240
void sub_55A240()
{
    // TODO: Incomplete.
}

// 0x55A5C0
void sub_55A5C0()
{
    // TODO: Incomplete.
}

// 0x55AE70
void sub_55AE70()
{
    // TODO: Incomplete.
}

// 0x55B0E0
void sub_55B0E0()
{
    // TODO: Incomplete.
}

// 0x55B150
void sub_55B150()
{
    // TODO: Incomplete.
}

// 0x55B1B0
void sub_55B1B0()
{
    // TODO: Incomplete.
}

// 0x55B280
void sub_55B280()
{
    // TODO: Incomplete.
}

// 0x55B2A0
void sub_55B2A0()
{
    // TODO: Incomplete.
}

// 0x55B410
void sub_55B410()
{
    // TODO: Incomplete.
}

// 0x55B4D0
void sub_55B4D0()
{
    // TODO: Incomplete.
}

// 0x55B720
void sub_55B720()
{
    // TODO: Incomplete.
}

// 0x55B880
void sub_55B880()
{
    // TODO: Incomplete.
}

// 0x55BAB0
void sub_55BAB0()
{
    // TODO: Incomplete.
}

// 0x55BD10
void sub_55BD10()
{
    // TODO: Incomplete.
}

// 0x55BF20
void sub_55BF20()
{
    // TODO: Incomplete.
}

// 0x55C110
void sub_55C110()
{
    // TODO: Incomplete.
}

// 0x55C2E0
void sub_55C2E0(int a1)
{
    tig_art_id_t art_id;
    TigArtFrameData art_frame_data;
    TigArtBlitInfo art_blit_info;
    TigRect src_rect;
    TigRect dst_rect;

    tig_art_interface_id_create(dword_5C8FA8[dword_64E028], 0, 0, 0, &art_id);
    tig_art_frame_data(art_id, &art_frame_data);

    src_rect.x = 0;
    src_rect.y = 0;
    src_rect.width = art_frame_data.width;
    src_rect.height = art_frame_data.height;

    dst_rect.x = dword_5C8F8C[a1];
    dst_rect.y = dword_5C8F70[a1];
    dst_rect.width = art_frame_data.width;
    dst_rect.height = art_frame_data.height;

    art_blit_info.flags = 0;
    art_blit_info.art_id = art_id;
    art_blit_info.src_rect = &src_rect;
    art_blit_info.dst_rect = &dst_rect;
    tig_window_blit_art(dword_64CA8C, &art_blit_info);
}

// 0x55C3A0
void sub_55C3A0()
{
    // TODO: Incomplete.
}

// 0x55C890
void sub_55C890()
{
    // TODO: Incomplete.
}

// 0x55CA70
void sub_55CA70()
{
    // TODO: Incomplete.
}

// 0x55CBC0
void spells_print_all()
{
    // TODO: Incomplete.
}

// 0x55D060
void sub_55D060()
{
    // TODO: Incomplete.
}

// 0x55D210
void sub_55D210()
{
    // TODO: Incomplete.
}

// 0x55D3A0
void sub_55D3A0()
{
    // TODO: Incomplete.
}

// 0x55D6F0
void sub_55D6F0()
{
    // TODO: Incomplete.
}

// 0x55D940
void sub_55D940()
{
    // TODO: Incomplete.
}

// 0x55DC60
void sub_55DC60()
{
    // TODO: Incomplete.
}

// 0x55DF90
void sub_55DF90()
{
    // TODO: Incomplete.
}

// 0x55E110
void sub_55E110()
{
    // TODO: Incomplete.
}

// 0x55EBA0
void sub_55EBA0()
{
    mes_unload(charedit_mes_file);
    tig_font_destroy(dword_64CDBC);
    tig_font_destroy(dword_64D3A8);
    tig_font_destroy(dword_64CDD0);
    tig_font_destroy(dword_64C828);
    tig_font_destroy(dword_64C7A0);
    tig_font_destroy(dword_64D3BC);
    tig_font_destroy(dword_64DF0C);
    tig_font_destroy(dword_64D420);
    tig_font_destroy(dword_64CDB0);
    tig_font_destroy(dword_64C848);
    tig_font_destroy(dword_64D3A4);
    tig_font_destroy(dword_64CFE0);
    tig_font_destroy(dword_64CDC0);
    tig_font_destroy(dword_64D3B0);
    tig_font_destroy(dword_64CA68);
    tig_font_destroy(dword_64C9D0);
    tig_font_destroy(dword_64C840);
    tig_font_destroy(dword_64CDB8);
    tig_font_destroy(dword_64D42C);
}

// 0x55EC90
void sub_55EC90()
{
    // TODO: Incomplete.
}

// 0x55EFB0
void sub_55EFB0()
{
    if (dword_64DEE4) {
        scrollbar_ui_control_destroy(stru_64C7A8);
        dword_64DEE4 = false;
    }
}

// 0x55EFE0
void sub_55EFE0()
{
    sub_55EFF0();
}

// 0x55EFF0
void sub_55EFF0()
{
    ScrollbarUiControlInfo sb;

    if (!dword_64DEE4) {
        sb.flags = 0x39F;
        sb.field_38 = dword_64D424;
        sb.field_3C = sub_55F0E0;
        if (dword_64CDC8 > 15) {
            sb.field_24 = dword_64CDC8 - 15;
        } else {
            sb.field_24 = 0;
        }
        sb.field_4 = stru_5C8F40;
        sb.field_28 = 0;
        sb.rect.width = stru_5C8F40.width + 160;
        sb.rect.x = 23;
        sb.rect.y = 66;
        sb.rect.height = 240;
        sb.field_40 = sub_55F110;
        sb.field_2C = 1;
        scrollbar_ui_control_create(&stru_64C7A8, &sb, dword_64CA60);
        sub_5806F0(stru_64C7A8);
        dword_64DEE4 = true;
    }
}

// 0x55F0D0
void sub_55F0D0()
{
    sub_55EFB0();
}

// 0x55F0E0
void sub_55F0E0(int value)
{
    if (dword_64DEE4) {
        dword_64D424 = value;
        if (dword_64E01C == 3) {
            sub_55D210();
        }
    }
}

// 0x55F110
void sub_55F110(TigRect* rect)
{
    TigArtBlitInfo blit_info;

    if (dword_64DEE4) {
        tig_art_interface_id_create(567, 0, 0, 0, &(blit_info.art_id));
        blit_info.flags = 0;
        blit_info.src_rect = rect;
        blit_info.dst_rect = rect;
        tig_window_blit_art(dword_64CA60, &blit_info);
    }
}

// 0x55F160
void sub_55F160()
{
    // TODO: Incomplete.
}

// 0x55F180
void sub_55F180()
{
    // TODO: Incomplete.
}

// 0x55F1A0
void sub_55F1A0()
{
    // TODO: Incomplete.
}

// 0x55F1E0
void sub_55F1E0()
{
    // TODO: Incomplete.
}

// 0x55F200
void sub_55F200()
{
    // TODO: Incomplete.
}

// 0x55F320
void sub_55F320()
{
    // TODO: Incomplete.
}

// 0x55F340
void sub_55F340()
{
    // TODO: Incomplete.
}

// 0x55F360
void sub_55F360()
{
    // TODO: Incomplete.
}

// 0x55F450
void sub_55F450()
{
    // TODO: Incomplete.
}

// 0x55F5F0
void sub_55F5F0()
{
    // TODO: Incomplete.
}
