#include "ui/broadcast_ui.h"

#include "game/player.h"
#include "ui/intgame.h"
#include "ui/textedit_ui.h"

static void sub_571950(TextEdit* textedit);
static void sub_571990(TextEdit* textedit);

// 0x5CAC28
static TextEdit stru_5CAC28 = {
    0,
    NULL,
    125,
    sub_571950,
    sub_571990,
    NULL,
};

// 0x681040
static int dword_681040;

// 0x681044
static int dword_681044;

// 0x681048
static char byte_681048[128];

// 0x6810C8
static tig_font_handle_t dword_6810C8;

// 0x6810CC
static int dword_6810CC;

// 0x6810D0
static int dword_6810D0;

// 0x5717F0
bool broadcast_ui_init(GameInitInfo* init_info)
{
    TigFont font_desc;

    (void)init_info;

    byte_681048[0] = '\0';
    stru_5CAC28.buffer = byte_681048;

    font_desc.flags = TIG_FONT_BLEND_ALPHA_SRC | TIG_FONT_SHADOW;
    tig_art_misc_id_create(TIG_ART_SYSTEM_FONT, 0, &(font_desc.art_id));
    font_desc.color = tig_color_make(120, 120, 175);
    tig_font_create(&font_desc, &dword_6810C8);

    return true;
}

// 0x571880
void broadcast_ui_exit()
{
    tig_font_destroy(dword_6810C8);
}

// 0x571890
void broadcast_ui_reset()
{
    byte_681048[0] = '\0';
}

// 0x5718A0
void sub_5718A0()
{
    if (dword_6810D0 == 1 || sub_5533B0() == 7) {
        sub_571910();
        return;
    }

    dword_681040 = 0;
    dword_681044 = 0;
    sub_5506C0(7);
    textedit_ui_focus(&stru_5CAC28);
    dword_6810CC = 1;
    sub_552160(stru_5CAC28.buffer, dword_6810C8);
    dword_6810D0 = 1;
}

// 0x571910
void sub_571910()
{
    if (dword_6810D0 == 1) {
        dword_6810D0 = 0;
        dword_6810CC = 0;
        textedit_ui_unfocus(&stru_5CAC28);
        *stru_5CAC28.buffer = '\0';
        sub_5506C0(0);
    }
}

// 0x571950
void sub_571950(TextEdit* textedit)
{
    if (*textedit->buffer != '\0') {
        sub_551830(&dword_681040);
        sub_4C2F00(player_get_pc_obj(), &dword_681040);
    }
    sub_571910();
}

// 0x571990
void sub_571990(TextEdit* textedit)
{
    if (*textedit->buffer != '\0') {
        sub_552160(textedit->buffer, dword_6810C8);
    } else {
        sub_552160(" ", dword_6810C8);
    }
}
