#include "ui/inven_ui.h"

#include <stdio.h>

#include "game/ai.h"
#include "game/critter.h"
#include "game/dialog.h"
#include "game/gsound.h"
#include "game/item.h"
#include "game/mes.h"
#include "game/mp_utils.h"
#include "game/multiplayer.h"
#include "game/obj_private.h"
#include "game/obj.h"
#include "game/object.h"
#include "game/player.h"
#include "game/portrait.h"
#include "game/proto.h"
#include "game/script.h"
#include "game/skill.h"
#include "game/spell.h"
#include "game/stat.h"
#include "game/target.h"
#include "ui/hotkey_ui.h"
#include "ui/intgame.h"
#include "ui/item_ui.h"
#include "ui/mainmenu_ui.h"
#include "ui/scrollbar_ui.h"
#include "ui/skill_ui.h"
#include "ui/spell_ui.h"
#include "ui/tb_ui.h"

static bool sub_572340();
static bool inven_ui_message_filter(TigMessage* msg);
static void sub_574FD0(bool a1);
static bool sub_575100(bool* a1);
static bool sub_575180(bool* a1);
static void sub_575200(int a1);
static void sub_575360(int a1);
static void sub_5754C0(int x, int y);
static int sub_575CB0(int x, int y, int64_t* a3);
static int64_t sub_575FA0(int x, int y, int64_t* a3);
static void redraw_inven_fix_bad_inven_obj(int64_t a1);
static bool redraw_inven_fix_bad_inven(int64_t a1, int64_t a2);
static void redraw_inven(bool a1);
static tig_art_id_t sub_5782D0();
static void sub_578330(int64_t a1, int64_t a2);
static void sub_5786C0(int64_t obj);
static void sub_578760(int64_t obj);
static void sub_5788C0(int64_t a1, int64_t a2, int a3, int a4);
static void sub_579770(int64_t from_obj, int64_t to_obj);
static bool sub_579840(int64_t obj, bool a2);
static void sub_579B60(int64_t obj);
static void sub_579C40();
static void sub_579D70();
static void sub_579DA0();
static void sub_579DD0();
static void sub_579E00(int a1);
static void sub_579E30(TigRect* rect);

// 0x5CAC58
static TigRect stru_5CAC58[9] = {
    { 151, 107, 64, 64 },
    { 247, 107, 32, 32 },
    { 279, 107, 32, 32 },
    { 247, 139, 64, 32 },
    { 23, 170, 96, 128 },
    { 247, 171, 96, 128 },
    { 119, 171, 128, 160 },
    { 55, 107, 64, 64 },
    { 150, 331, 64, 64 },
};

// 0x5CACE8
static TigRect stru_5CACE8[9] = {
    { 499, 107, 64, 64 },
    { 595, 107, 32, 32 },
    { 627, 107, 32, 32 },
    { 595, 139, 64, 32 },
    { 371, 170, 96, 128 },
    { 595, 171, 96, 128 },
    { 467, 171, 128, 160 },
    { 403, 107, 64, 64 },
    { 498, 331, 64, 64 },
};

// 0x5CAD78
static TigRect stru_5CAD78[9] = {
    { 157, 182, 42, 42 },
    { 220, 182, 21, 21 },
    { 241, 182, 21, 21 },
    { 220, 203, 42, 21 },
    { 73, 223, 63, 85 },
    { 220, 224, 63, 85 },
    { 136, 224, 84, 106 },
    { 94, 182, 42, 42 },
    { 156, 330, 42, 42 },
};

// 0x5CAE08
static TigRect stru_5CAE08[9] = {
    { 157, 182, 42, 42 },
    { 220, 182, 21, 21 },
    { 241, 182, 21, 21 },
    { 220, 203, 42, 21 },
    { 73, 223, 63, 85 },
    { 220, 224, 63, 85 },
    { 136, 224, 84, 106 },
    { 94, 182, 42, 42 },
    { 156, 330, 42, 42 },
};

// 0x5CAE98
static TigRect stru_5CAE98[3] = {
    { 145, 29, 185, 15 },
    { 145, 44, 185, 15 },
    { 145, 59, 185, 15 },
};

// 0x5CAEC8
static TigRect stru_5CAEC8[3] = {
    { 431, 32, 185, 15 },
    { 431, 47, 185, 15 },
    { 431, 62, 185, 15 },
};

// 0x5CAEF8
static TigRect stru_5CAEF8[2] = {
    { 80, 358, 23, 35 },
    { 261, 358, 23, 35 },
};

// 0x5CAF18
static TigRect stru_5CAF18[2] = {
    { 425, 358, 23, 35 },
    { 609, 358, 23, 35 },
};

// 0x5CAF38
static TigRect stru_5CAF38 = { 105, 354, 22, 16 };

// 0x5CAF48
static TigRect stru_5CAF48 = { 227, 354, 22, 16 };

// 0x5CAF58
static int dword_5CAF58[] = {
    241,
    246,
    247,
    245,
    249,
    248,
    244,
    243,
    242,
};

// 0x5CAF80
static TigRect stru_5CAF80 = { 183, 33, 100, 78 };

// 0x5CAF90
static TigRect stru_5CAF90 = { 330, 168, 17, 224 };

// 0x5CAFA0
static TigRect stru_5CAFA0 = { 330, 136, 17, 256 };

// 0x5CAFB0
static TigRect stru_5CAFB0 = { 180, 29, 155, 65 };

// 0x5CAFC0
static int dword_5CAFC0[5] = {
    729,
    729,
    729,
    729,
    729,
};

// 0x5CAFD4
static int dword_5CAFD4[5] = {
    70,
    93,
    116,
    139,
    162,
};

// 0x6810E0
static int64_t qword_6810E0;

// 0x6810E8
static int dword_6810E8;

// 0x6810F0
static ScrollbarId stru_6810F0;

// 0x6810F8
static mes_file_handle_t inven_ui_mes_file;

// 0x6810FC
static bool dword_6810FC;

// 0x681100
static tig_button_handle_t dword_681100;

// 0x681108
static TigRect stru_681108;

// 0x681118
static int dword_681118;

// 0x68111C
static int dword_68111C[120];

// 0x6812FC
static char byte_6812FC[128];

// 0x681390
static tig_font_handle_t dword_681390;

// 0x68137C
static tig_window_handle_t inven_ui_window_handle;

// 0x681380
static TigRect stru_681380;

// 0x681394
static int dword_681394;

// 0x681398
static tig_button_handle_t dword_681398;

// 0x68139C
static bool inven_ui_created;

// 0x6813A0
static tig_button_handle_t dword_6813A0;

// 0x6813A4
static int dword_6813A4;

// 0x6813A8
static int64_t qword_6813A8;

// 0x6813B0
static TigRect stru_6813B0;

// 0x6813C0
static char byte_6813C0[128];

// 0x681440
static int dword_681440;

// 0x681444
static tig_button_handle_t dword_681444;

// 0x681448
static int dword_681448;

// 0x681450
static int64_t qword_681450;

// 0x681458
static int64_t qword_681458;

// 0x681460
static tig_button_handle_t dword_681460;

// 0x681464
static tig_button_handle_t dword_681464;

// 0x681468
static char byte_681468[128];

// 0x6814E8
static tig_button_handle_t dword_6814E8;

// 0x6814EC
static tig_button_handle_t dword_6814EC;

// 0x6814F0
static int dword_6814F0;

// 0x6814F8
static int64_t qword_6814F8;

// 0x681500
static tig_button_handle_t dword_681500;

// 0x681504
static int dword_681504;

// 0x681508
static int dword_681508;

// 0x68150C
static bool dword_68150C;

// 0x681510
static int dword_681510;

// 0x681514
static bool dword_681514;

// 0x681518
static int dword_681518[960];

// 0x682418
static tig_font_handle_t dword_682418;

// 0x68241C
static char byte_68241C[1000];

// 0x682804
static char byte_682804[1000];

// 0x682BEC
static char byte_682BEC[128];

// 0x682C6C
static tig_button_handle_t dword_682C6C;

// 0x682C70
static int dword_682C70;

// 0x682C74
static tig_font_handle_t dword_682C74;

// 0x682C78
static int64_t qword_682C78;

// 0x682C80
static tig_button_handle_t dword_682C80;

// 0x682C84
static tig_button_handle_t dword_682C84;

// 0x682C88
static tig_button_handle_t dword_682C88;

// 0x682C8C
static char byte_682C8C[MAX_STRING];

// 0x68345C
static tig_font_handle_t dword_68345C;

// 0x683460
static tig_button_handle_t dword_683460;

// 0x683464
static int dword_683464;

// 0x68346C
static bool dword_68346C;

// 0x683470
static int dword_683470;

// 0x739F58
static int dword_739F58;

// 0x739F60
static int dword_739F60;

// 0x739F68
static int64_t qword_739F68;

// 0x739F70
static int64_t qword_739F70;

// 0x739F78
static int64_t qword_739F78;

// 0x739F80
static int dword_739F80;

// 0x739F84
static int dword_739F84;

// 0x572060
bool inven_ui_init(GameInitInfo* init_info)
{
    TigFont font;

    (void)init_info;

    if (!mes_load("mes\\inven_ui.mes", &inven_ui_mes_file)) {
        return false;
    }

    font.flags = 0;
    tig_art_interface_id_create(27, 0, 0, 0, &(font.art_id));
    font.str = NULL;
    font.color = tig_color_make(255, 255, 255);
    tig_font_create(&font, &dword_682418);

    tig_art_interface_id_create(229, 0, 0, 0, &(font.art_id));
    tig_font_create(&font, &dword_682C74);

    tig_art_interface_id_create(486, 0, 0, 0, &(font.art_id));
    tig_font_create(&font, &dword_681390);

    font.flags = TIG_FONT_CENTERED;
    tig_art_interface_id_create(171, 0, 0, 0, &(font.art_id));
    tig_font_create(&font, &dword_68345C);

    return true;
}

// 0x572190
void inven_ui_exit()
{
    tig_font_destroy(dword_682418);
    tig_font_destroy(dword_682C74);
    tig_font_destroy(dword_681390);
    tig_font_destroy(dword_68345C);
    mes_unload(inven_ui_mes_file);
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
bool sub_572240(int64_t a1, int64_t a2, int a3)
{
    Packet100 pkt;

    if (sub_572340()) {
        return true;
    }

    if ((tig_net_flags & TIG_NET_CONNECTED) != 0
        && (tig_net_flags & TIG_NET_HOST) == 0) {
        pkt.type = 100;
        pkt.subtype = 12;
        sub_4F0640(a1, &(pkt.field_8));
        sub_4F0640(a2, &(pkt.field_20));
        pkt.field_38 = a3;
        pkt.field_3C = 0;
        tig_net_send_app_all(&pkt, sizeof(pkt));

        return true;
    }

    if (!sub_572370(a1, a2, a3)) {
        return false;
    }

    if (!sub_572510(a1, a2, a3)) {
        return false;
    }

    sub_572640(a1, a2, a3);
    return inven_ui_create(a1, a2, a3);
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
bool sub_572370(int64_t a1, int64_t a2, int a3)
{
    int rotation;
    tig_art_id_t art_id;
    int err;
    MesFileEntry mes_file_entry;
    John v1;

    if (a1 == OBJ_HANDLE_NULL) {
        return false;
    }

    if (sub_45D8D0(a1)) {
        return false;
    }

    if (obj_field_int32_get(a1, OBJ_F_TYPE) != OBJ_TYPE_PC) {
        return false;
    }

    if (a3 == 2 || a3 == 3) {
        rotation = sub_441B20(a1, a2);
        art_id = obj_field_int32_get(a1, OBJ_F_CURRENT_AID);
        art_id = tig_art_id_rotation_set(art_id, rotation);
        object_set_current_aid(a1, art_id);
        if (sub_4AF260(a1, a2) != 0) {
            return false;
        }
    }

    if (a2 != OBJ_HANDLE_NULL && (a3 == 2 || a3 == 4)) {
        if (obj_field_int32_get(a2, OBJ_F_TYPE) == OBJ_TYPE_CONTAINER) {
            if (a3 == 2) {
                err = sub_4AED80(a1, a2);
                if (err != 0) {
                    mes_file_entry.num = err;
                    mes_get_msg(inven_ui_mes_file, &mes_file_entry);

                    v1.type = 4;
                    v1.str = mes_file_entry.str;
                    sub_550750(&v1);

                    gsound_play_sfx_id(sub_4F0FD0(a2, 2), 1);
                    return false;
                }

                if (!sub_441980(a1, a2, OBJ_HANDLE_NULL, SAP_USE, 0)) {
                    gsound_play_sfx_id(sub_4F0FD0(a2, 2), 1);
                    return false;
                }
            }
        } else {
            if (a3 == 2) {
                if (!sub_441980(a1, a2, OBJ_HANDLE_NULL, SAP_USE, 0)) {
                    return false;
                }
            }
        }
    }

    return true;
}

// 0x572510
bool sub_572510(int64_t a1, int64_t a2, int a3)
{
    if (!sub_551A80(0)) {
        return false;
    }

    switch (a3) {
    case 0:
        if (!sub_551A80(8)) {
            return false;
        }
        return true;
    case 1:
        if (!sub_551A80(4)) {
            return false;
        }
        if (a1 == a2) {
            return false;
        }
        return true;
    case 2:
    case 4:
        if (!sub_551A80(10)) {
            return false;
        }
        if (a1 == a2) {
            return false;
        }
        return true;
    case 3:
        if (!sub_551A80(11)) {
            return false;
        }
        if (a1 == a2) {
            return false;
        }
        return true;
    case 5:
        if (!sub_551A80(19)) {
            return false;
        }
        if (a1 == a2) {
            return false;
        }
        return true;
    case 6:
        if (!sub_551A80(20)) {
            return false;
        }
        if (a1 == a2) {
            return false;
        }
        return true;
    default:
        return false;
    }
}

// 0x572640
void sub_572640(int64_t a1, int64_t a2, int a3)
{
    int64_t v1;
    int64_t v2;
    int64_t v3;
    int amt;

    if (a2 == OBJ_HANDLE_NULL) {
        return;
    }

    if (a3 == 1) {
        v1 = sub_45F650(a2);
        if (v1 != OBJ_HANDLE_NULL) {
            v2 = sub_468570(8);
            v3 = sub_462540(v1, v2, 0);
            while (v3 != OBJ_HANDLE_NULL) {
                amt = item_gold_get(v3);
                sub_464830(v1, v2, amt, v3);
                v3 = sub_462540(v1, v2, 0);
            }
        }

        if (!sub_45DDA0(a2)) {
            item_identify_all(a2);
            item_identify_all(v1);
        }
    } else if (a3 == 2 || a3 == 4) {
        if (obj_field_int32_get(a2, OBJ_F_TYPE) == OBJ_TYPE_CONTAINER) {
            if (a3 == 2) {
                if (tig_art_id_frame_get(obj_field_int32_get(a2, OBJ_F_CURRENT_AID)) == 0) {
                    sub_4EEF20(a2);
                    gsound_play_sfx_id(sub_4F0FD0(a2, 0), 1);
                }
            }
            if ((obj_field_int32_get(a2, OBJ_F_CONTAINER_FLAGS) & 0x200) != 0) {
                sub_463E20(a2);
            }
        } else {
            if (a3 == 2) {
                gsound_play_sfx_id(3007, 1);
            }
        }
    }
}

// 0x5727B0
bool inven_ui_create(int64_t a1, int64_t a2, int a3)
{
    TigRect rect;
    TigButtonData button_data;
    MesFileEntry mes_file_entry;
    John v1;
    S550DA0 v2;
    tig_art_id_t art_id;
    tig_button_handle_t button_group[2];
    unsigned int critter_flags2;

    dword_683464 = a3;
    qword_6813A8 = a2;
    qword_682C78 = a2;
    dword_681514 = false;
    dword_68150C = 0;

    if (!intgame_big_window_lock(inven_ui_message_filter, &inven_ui_window_handle)) {
        sub_551A80(0);
        return false;
    }

    button_data.flags = TIG_BUTTON_FLAG_0x01;
    button_data.window_handle = inven_ui_window_handle;
    tig_art_interface_id_create(340, 0, 0, 0, &(button_data.art_id));
    button_data.mouse_down_snd_id = 3000;
    button_data.mouse_up_snd_id = 3001;
    button_data.mouse_enter_snd_id = -1;
    button_data.mouse_exit_snd_id = -1;
    button_data.x = 763;
    button_data.y = 197;
    if (tig_button_create(&button_data, &dword_681444) != TIG_OK) {
        intgame_big_window_unlock();
        sub_551A80(0);
        return false;
    }

    dword_682C80 = TIG_BUTTON_HANDLE_INVALID;
    dword_6814EC = TIG_BUTTON_HANDLE_INVALID;
    dword_681500 = TIG_BUTTON_HANDLE_INVALID;
    dword_683460 = TIG_BUTTON_HANDLE_INVALID;
    dword_682C6C = TIG_BUTTON_HANDLE_INVALID;
    dword_6814E8 = TIG_BUTTON_HANDLE_INVALID;
    dword_681460 = TIG_BUTTON_HANDLE_INVALID;
    dword_682C88 = TIG_BUTTON_HANDLE_INVALID;
    dword_6813A0 = TIG_BUTTON_HANDLE_INVALID;
    dword_682C84 = TIG_BUTTON_HANDLE_INVALID;
    dword_681464 = TIG_BUTTON_HANDLE_INVALID;
    dword_681100 = TIG_BUTTON_HANDLE_INVALID;
    dword_681398 = TIG_BUTTON_HANDLE_INVALID;

    if (dword_683464 == 0
        || dword_683464 == 5
        || dword_683464 == 6) {
        button_data.mouse_down_snd_id = -1;
        button_data.mouse_up_snd_id = -1;
        tig_art_interface_id_create(676, 0, 0, 0, &(button_data.art_id));
        button_data.x = 29;
        button_data.y = 318;
        if (tig_button_create(&button_data, &dword_682C6C) != TIG_OK) {
            intgame_big_window_unlock();
            sub_551A80(0);
            return false;
        }

        tig_art_interface_id_create(677, 0, 0, 0, &(button_data.art_id));
        button_data.x = 254;
        button_data.y = 319;
        if (tig_button_create(&button_data, &dword_6814E8) != TIG_OK) {
            intgame_big_window_unlock();
            sub_551A80(0);
            return false;
        }

        button_data.y = stru_5CAEF8[0].y;
        button_data.x = stru_5CAEF8[0].x;
        button_data.art_id = TIG_ART_ID_INVALID;
        button_data.width = stru_5CAEF8[0].width;
        button_data.height = stru_5CAEF8[0].height;
        if (tig_button_create(&button_data, &dword_681460)) {
            intgame_big_window_unlock();
            sub_551A80(0);
            return false;
        }

        button_data.x = stru_5CAEF8[1].x;
        button_data.y = stru_5CAEF8[1].y;
        button_data.art_id = TIG_ART_ID_INVALID;
        button_data.width = stru_5CAEF8[1].width;
        button_data.height = stru_5CAEF8[1].height;
        if (tig_button_create(&button_data, &dword_682C88) != TIG_OK) {
            intgame_big_window_unlock();
            sub_551A80(0);
            return false;
        }

        button_data.mouse_down_snd_id = 3000;
        button_data.mouse_up_snd_id = 3001;
    } else {
        button_data.flags = TIG_BUTTON_FLAG_HIDDEN;
        button_data.mouse_down_snd_id = -1;
        button_data.mouse_up_snd_id = -1;
        tig_art_interface_id_create(676, 0, 0, 0, &(button_data.art_id));
        button_data.x = 374;
        button_data.y = 318;
        if (tig_button_create(&button_data, &dword_682C6C) != TIG_OK) {
            intgame_big_window_unlock();
            sub_551A80(0);
            return false;
        }

        tig_art_interface_id_create(677, 0, 0, 0, &(button_data.art_id));
        button_data.x = 599;
        button_data.y = 319;
        if (tig_button_create(&button_data, &dword_6814E8) != TIG_OK) {
            intgame_big_window_unlock();
            sub_551A80(0);
            return false;
        }

        button_data.y = stru_5CAF18[0].y;
        button_data.x = stru_5CAF18[0].x;
        button_data.art_id = TIG_ART_ID_INVALID;
        button_data.width = stru_5CAF18[0].width;
        button_data.height = stru_5CAF18[0].height;
        if (tig_button_create(&button_data, &dword_681460) != TIG_OK) {
            intgame_big_window_unlock();
            sub_551A80(0);
            return false;
        }

        button_data.x = stru_5CAF18[1].x;
        button_data.y = stru_5CAF18[1].y;
        button_data.art_id = TIG_ART_ID_INVALID;
        button_data.width = stru_5CAF18[1].width;
        button_data.height = stru_5CAF18[1].height;
        if (tig_button_create(&button_data, &dword_682C88) != TIG_OK) {
            intgame_big_window_unlock();
            sub_551A80(0);
            return false;
        }

        button_data.flags = TIG_BUTTON_FLAG_0x01;
        button_data.mouse_down_snd_id = 3000;
        button_data.mouse_up_snd_id = 3001;
        if ( dword_683464 == 2 )
        {
            tig_art_interface_id_create(808, 0, 0, 0, &(button_data.art_id));
            button_data.x = 301;
            button_data.y = 47;
            if (tig_button_create(&button_data, &dword_681464) != TIG_OK) {
                intgame_big_window_unlock();
                sub_551A80(0);
                return false;
            }
        }

        button_data.flags = TIG_BUTTON_FLAG_0x02 | TIG_BUTTON_FLAG_0x04;
        tig_art_interface_id_create(338, 0, 0, 0, &(button_data.art_id));
        button_data.x = 713;
        button_data.y = 37;
        if (tig_button_create(&button_data, &dword_682C80) != TIG_OK) {
            intgame_big_window_unlock();
            sub_551A80(0);
            return false;
        }

        tig_art_interface_id_create(339, 0, 0, 0, &(button_data.art_id));
        button_data.x = 751;
        button_data.y = 37;
        if (tig_button_create(&button_data, &dword_6814EC) != TIG_OK) {
            intgame_big_window_unlock();
            sub_551A80(0);
            return false;
        }

        button_group[0] = dword_682C80;
        button_group[1] = dword_6814EC;
        tig_button_radio_group_create(2, button_group, 0);

        if (obj_field_int32_get(qword_6813A8, OBJ_F_TYPE) != OBJ_TYPE_CONTAINER) {
            button_data.flags = TIG_BUTTON_FLAG_0x02 | TIG_BUTTON_FLAG_0x04;
            tig_art_interface_id_create(338, 0, 0, 0, &button_data.art_id);
            if (dword_683464 == 1) {
                button_data.x = 8;
                button_data.y = 123;
            } else {
                button_data.x = 23;
                button_data.y = 108;
            }
            if (tig_button_create(&button_data, &dword_681500) != TIG_OK) {
                intgame_big_window_unlock();
                sub_551A80(0);
                return false;
            }

            tig_art_interface_id_create(339, 0, 0, 0, &button_data.art_id);
            if (dword_683464 == 1) {
                button_data.x = 46;
                button_data.y = 123;
            } else {
                button_data.x = 74;
                button_data.y = 108;
            }
            if (tig_button_create(&button_data, &dword_683460) != TIG_OK) {
                intgame_big_window_unlock();
                sub_551A80(0);
                return false;
            }

            button_group[0] = dword_681500;
            button_group[1] = dword_683460;
            tig_button_radio_group_create(2, button_group, 0);

            button_data.flags = TIG_BUTTON_FLAG_HIDDEN;
            button_data.mouse_down_snd_id = -1;
            button_data.mouse_up_snd_id = -1;
            button_data.art_id = TIG_ART_ID_INVALID;
            button_data.x = 75;
            button_data.y = 329;
            button_data.width = 55;
            button_data.height = 40;
            if (tig_button_create(&button_data, &dword_6813A0) != TIG_OK) {
                intgame_big_window_unlock();
                sub_551A80(0);
                return false;
            }

            button_data.art_id = TIG_ART_ID_INVALID;
            button_data.x = 225;
            button_data.y = 329;
            button_data.width = 55;
            button_data.height = 40;
            if (tig_button_create(&button_data, &dword_682C84) != TIG_OK) {
                intgame_big_window_unlock();
                sub_551A80(0);
                return false;
            }
        }
    }

    qword_6814F8 = a1;
    sub_466260(qword_6814F8, dword_68111C);

    if (dword_683464 == 4
        || dword_683464 == 5
        || dword_683464 == 6) {
        dword_681394 = 0;
        dword_681118 = 0;
    } else {
        dword_6813A4 = 224;
        dword_681394 = 1;
        stru_6813B0.x = 703;
        stru_6813B0.y = 385;
        dword_681118 = 1;
        dword_681504 = 342;
        stru_681380.x = 703;
        stru_681380.y = 331;
    }

    if (dword_683464 == 1) {
        dword_681448 = 1;
        dword_682C70 = 345;
        stru_681108.x = 703;
        stru_681108.y = 277;
    } else {
        dword_681448 = 0;
    }

    dword_681510 = 5;

    if (qword_6813A8 != OBJ_HANDLE_NULL) {
        sub_441B60(qword_682C78, qword_6814F8, byte_682C8C);
        dword_681510 = 4;
        if (dword_683464 == 1) {
            dword_681510 = 5;
            qword_6813A8 = sub_45F650(qword_682C78);
            if (qword_6813A8 != OBJ_HANDLE_NULL) {
                dword_681510 = 89;
            } else {
                qword_6813A8 = qword_682C78;
            }
        } else if ((dword_683464 == 2 || dword_683464 == 4)
            && obj_field_int32_get(qword_6813A8, OBJ_F_TYPE) == OBJ_TYPE_CONTAINER) {
            dword_681510 = 88;
            sub_468190();
        }

        sub_466260(qword_6813A8, dword_681518);
    }

    byte_68241C[0] = '\0';
    dword_681440 = -1;

    if (dword_683464 == 1 && critter_leader_get(qword_682C78) == a1) {
        dword_6810FC = 1;
        if (sub_45E590(qword_682C78) != qword_682C78) {
            button_data.flags = TIG_BUTTON_FLAG_0x01;
            button_data.window_handle = inven_ui_window_handle;
            tig_art_interface_id_create(757, 0, 0, 0, &(button_data.art_id));
            button_data.x = 155;
            button_data.y = 55;
            if (tig_button_create(&button_data, &dword_681100) != TIG_OK) {
                intgame_big_window_unlock();
                sub_551A80(0);
                return false;
            }

            tig_art_interface_id_create(758, 0, 0, 0, &(button_data.art_id));
            button_data.x = 253;
            button_data.y = 55;
            if (tig_button_create(&button_data, &dword_681398) != TIG_OK) {
                intgame_big_window_unlock();
                sub_551A80(0);
                return false;
            }
        }
    } else {
        dword_6810FC = 0;
    }

    dword_681508 = 0;
    qword_681458 = OBJ_HANDLE_NULL;

    sub_4A53B0(qword_6814F8, qword_6813A8);
    redraw_inven_fix_bad_inven_obj(qword_6814F8);
    redraw_inven_fix_bad_inven_obj(qword_6813A8);

    if (!dword_6810FC
        && qword_6813A8 != OBJ_HANDLE_NULL
        && obj_field_int32_get(qword_6813A8, OBJ_F_TYPE) != OBJ_TYPE_PC
        && sub_4A5460(qword_6813A8) <= 1) {
        sub_4670A0(qword_6813A8, 0);
        sub_466260(qword_6813A8, dword_681518);
    }

    sub_579C40();
    redraw_inven(true);
    qword_6810E0 = OBJ_HANDLE_NULL;
    sub_4B8CE0(obj_field_int64_get(qword_6814F8, OBJ_F_LOCATION));

    v2.window_handle = inven_ui_window_handle;
    if (dword_683464 == 0
        || dword_683464 == 5
        || dword_683464 == 6) {
        rect.x = 11;
        rect.y = 9;
        art_id = 233;
    } else {
        rect.x = 16;
        rect.y = 17;
        art_id = dword_683464 == 1 ? 231 : 232;
    }

    rect.width = 89;
    rect.height = 89;
    v2.rect = &rect;
    tig_art_interface_id_create(art_id, 0, 0, 0, &(v2.art_id));
    if (!sub_541680()) {
        sub_550DA0(1, &v2);
    }

    dword_739F58 = 0;

    if (player_is_pc_obj(a1)) {
        sub_460790(4, 0);
    }

    if (qword_6813A8 != OBJ_HANDLE_NULL) {
        sub_4640C0(qword_6813A8);
    }

    critter_flags2 = obj_field_int32_get(qword_6814F8, OBJ_F_CRITTER_FLAGS2);
    if ((critter_flags2 & OCF2_ITEM_STOLEN) != 0) {
        critter_flags2 &= ~OCF2_ITEM_STOLEN;
        obj_field_int32_set(qword_6814F8, OBJ_F_CRITTER_FLAGS2, critter_flags2);

        mes_file_entry.num = 7;
        mes_get_msg(inven_ui_mes_file, &mes_file_entry);

        v1.type = 4;
        v1.str = mes_file_entry.str;
        sub_550750(&v1);
    }

    dword_6814F0 = 0;
    inven_ui_created = true;

    return true;
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
        && obj_field_int32_get(qword_6813A8, OBJ_F_TYPE) == OBJ_TYPE_CONTAINER) {
        if (dword_683464 == 2) {
            art_id = obj_field_int32_get(qword_6813A8, OBJ_F_CURRENT_AID);
            if (tig_art_id_frame_get(art_id) == 1
                && (obj_field_int32_get(qword_6813A8, OBJ_F_CONTAINER_FLAGS) & OCOF_STICKY) == 0) {
                object_dec_current_aid(qword_6813A8);

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
void sub_573590(int64_t obj)
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
int inven_ui_is_created()
{
    return inven_ui_created;
}

// 0x573600
int64_t sub_573600()
{
    return inven_ui_created ? qword_682C78 : OBJ_HANDLE_NULL;
}

// 0x573620
int64_t sub_573620()
{
    return qword_6810E0;
}

// 0x573630
void sub_573630(int64_t obj)
{
    tig_art_id_t inv_art_id;
    tig_art_id_t mouse_art_id;

    if (!inven_ui_created && qword_6814F8 == 0) {
        qword_6814F8 = player_get_pc_obj();
    }

    qword_681450 = qword_6814F8;
    qword_6810E0 = obj;
    dword_6810E8 = obj_field_int32_get(obj, OBJ_F_ITEM_INV_LOCATION);

    inv_art_id = obj_field_int32_get(qword_6810E0, OBJ_F_ITEM_INV_AID);
    sub_553990();
    mouse_art_id = tig_mouse_cursor_get_art_id();
    tig_mouse_hide();
    tig_mouse_cursor_set_art_id(inv_art_id);
    tig_mouse_cursor_overlay(mouse_art_id, 2, 2);
    tig_mouse_cursor_set_offset(2, 2);
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

    inv_art_id = obj_field_int32_get(qword_6810E0, OBJ_F_ITEM_INV_AID);
    mouse_art_id = tig_mouse_cursor_get_art_id();
    tig_mouse_hide();
    tig_mouse_cursor_set_art_id(inv_art_id);
    tig_mouse_cursor_overlay(mouse_art_id, 2, 2);
    tig_mouse_cursor_set_offset(2, 2);
    tig_mouse_show();
}

// 0x573730
void sub_573730()
{
    dword_68346C = false;
}

// 0x573740
void sub_573740(int64_t a1, bool a2)
{
    tig_art_id_t mouse_art_id;
    tig_art_id_t inv_art_id;

    if (!inven_ui_created && qword_6814F8 == OBJ_HANDLE_NULL) {
        qword_6814F8 = player_get_pc_obj();
    }

    mouse_art_id = tig_mouse_cursor_get_art_id();
    qword_681450 = qword_6814F8;
    qword_6810E0 = a1;
    dword_6810E8 = obj_field_int32_get(a1, OBJ_F_ITEM_INV_LOCATION);

    if (!inven_ui_created) {
        sub_5788C0(qword_6810E0, OBJ_HANDLE_NULL, -1, a2 ? 4 : 36);
        if (qword_6810E0 == OBJ_HANDLE_NULL) {
            return;
        }
    }

    inv_art_id = obj_field_int32_get(qword_6810E0, OBJ_F_ITEM_INV_AID);
    tig_mouse_hide();
    tig_mouse_cursor_set_art_id(inv_art_id);
    tig_mouse_cursor_overlay(mouse_art_id, 2, 2);
    tig_mouse_cursor_set_offset(2, 2);
    tig_mouse_show();
}

// 0x573840
void sub_573840()
{
    qword_6810E0 = 0;
}

// 0x573897
static inline bool inven_ui_message_filter_handle_button_pressed(TigMessage* msg)
{
    if (msg->data.button.button_handle == dword_6814EC) {
        dword_68150C = false;
        tig_button_hide(dword_682C6C);
        tig_button_hide(dword_6814E8);
        tig_button_hide(dword_681460);
        tig_button_hide(dword_682C88);
        redraw_inven(false);
        return true;
    }

    if (msg->data.button.button_handle == dword_682C80) {
        dword_68150C = true;
        tig_button_show(dword_682C6C);
        tig_button_show(dword_6814E8);
        tig_button_show(dword_681460);
        tig_button_show(dword_682C88);
        redraw_inven(false);
        return true;
    }

    if (msg->data.button.button_handle == dword_683460) {
        dword_681514 = false;
        sub_579DA0();
        tig_button_hide(dword_6813A0);
        tig_button_hide(dword_682C84);
        redraw_inven(false);
        redraw_inven(false);
        return true;
    }

    if (msg->data.button.button_handle == dword_681500) {
        dword_681514 = true;
        sub_579DD0();
        tig_button_show(dword_6813A0);
        tig_button_show(dword_682C84);
        redraw_inven(false);
        return true;
    }

    if (msg->data.button.button_handle == dword_682C6C || msg->data.button.button_handle == dword_681460) {
        sub_5506C0(0);
        sub_556220(qword_6814F8);
        return true;
    }

    if (msg->data.button.button_handle == dword_6814E8 || msg->data.button.button_handle == dword_682C88) {
        sub_5506C0(0);
        sub_5566B0(qword_6814F8);
        return true;
    }

    if (msg->data.button.button_handle == dword_6813A0) {
        sub_5506C0(0);
        sub_556220(qword_682C78);
        return true;
    }

    if (msg->data.button.button_handle == dword_682C84) {
        sub_5506C0(0);
        sub_5566B0(qword_682C78);
        return true;
    }

    return false;
}

// 0x573A95
static inline bool inven_ui_message_filter_handle_button_released(TigMessage* msg)
{
    if (msg->data.button.button_handle == dword_681444) {
        sub_4670A0(qword_6814F8, dword_6814F0);
        sub_466260(qword_6814F8, dword_68111C);
        if (dword_6810FC) {
            sub_4670A0(qword_6813A8, dword_6814F0);
            sub_466260(qword_6813A8, dword_681518);
        }
        dword_6814F0 = 1 - dword_6814F0;
        redraw_inven(false);
        return true;
    }

    if (msg->data.button.button_handle == dword_681464) {
        sub_579770(qword_6813A8, qword_6814F8);
        return true;
    }

    if (msg->data.button.button_handle == dword_681100) {
        sub_574FD0(0);
        return true;
    }

    if (msg->data.button.button_handle == dword_681398) {
        sub_574FD0(1);
        return true;
    }

    return false;
}

// 0x573BAF
static inline bool inven_ui_mssage_filter_handle_button_hovered(TigMessage* msg)
{
    if (msg->data.button.button_handle == dword_682C6C || msg->data.button.button_handle == dword_681460) {
        sub_556220(qword_6814F8);
        return true;
    }

    if (msg->data.button.button_handle == dword_6814E8 || msg->data.button.button_handle == dword_682C88) {
        sub_5566B0(qword_6814F8);
        return true;
    }

    if (msg->data.button.button_handle == dword_6813A0) {
        sub_556220(qword_682C78);
        return true;
    }

    if (msg->data.button.button_handle == dword_682C84) {
        sub_5566B0(qword_682C78);
        return true;
    }

    return false;
}

// 0x573C80
static inline void inven_ui_message_filter_handle_mouse_idle(TigMessage* msg)
{
    int64_t v1;
    int64_t v2;

    if (qword_6810E0 == OBJ_HANDLE_NULL) {
        v1 = sub_575FA0(msg->data.mouse.x, msg->data.mouse.y, &v2);
        if (v1 != qword_681458) {
            qword_681458 = v1;
            if (qword_681458 != OBJ_HANDLE_NULL) {
                sub_57CCF0(qword_6814F8, qword_681458);
                switch (dword_683464) {
                case 1:
                    sub_578330(qword_681458, v2);
                    break;
                case 5:
                    sub_5786C0(qword_681458);
                    break;
                case 6:
                    sub_578760(qword_681458);
                    break;
                default:
                    redraw_inven(false);
                    break;
                }
            } else {
                byte_68241C[0] = '\0';
                dword_681440 = -1;
                redraw_inven(false);
            }
        }
    }
}

// 0x573D61
static inline bool inven_ui_message_filter_handle_mouse_move(TigMessage* msg)
{
    if (qword_6810E0 != OBJ_HANDLE_NULL) {
        if (dword_681394) {
            if (msg->data.mouse.x >= stru_6813B0.x
                && msg->data.mouse.x < stru_6813B0.x + stru_6813B0.width
                && msg->data.mouse.y >= stru_6813B0.y
                && msg->data.mouse.y < stru_6813B0.y + stru_6813B0.height) {
                if (dword_6813A4 == 224) {
                    if (qword_681450 == qword_6813A8) {
                        switch (dword_683464) {
                        case 1:
                            if (dword_6810FC) {
                                dword_6813A4 = 225;
                            } else {
                                dword_6813A4 = 348;
                            }
                            break;
                        case 2:
                        case 3:
                            dword_6813A4 = 348;
                            break;
                        default:
                            dword_6813A4 = 225;
                            break;
                        }
                    } else {
                        dword_6813A4 = 225;
                    }

                    redraw_inven(false);
                    return true;
                }
            } else {
                if (dword_6813A4 != 224) {
                    dword_6813A4 = 224;
                    redraw_inven(false);
                    return true;
                }
            }
        }

        if (dword_681448) {
            if (msg->data.mouse.x >= stru_681108.x
                && msg->data.mouse.x < stru_681108.x + stru_681108.width
                && msg->data.mouse.y >= stru_681108.y
                && msg->data.mouse.y < stru_681108.y + stru_681108.height) {
                if (dword_682C70 == 345) {
                    // TODO: Wrong.
                    dword_682C70 = sub_579840(qword_6810E0, 0) ? 346 : 347;
                }
            } else {
                if (dword_682C70 != 345) {
                    dword_682C70 = 345;
                    redraw_inven(false);
                    return true;
                }
            }
        }

        if (dword_681118) {
            if (msg->data.mouse.x >= stru_681380.x
                && msg->data.mouse.x < stru_681380.x + stru_681380.width
                && msg->data.mouse.y >= stru_681380.y
                && msg->data.mouse.y < stru_681380.y + stru_681380.height) {
                if (dword_681504 == 342) {
                    if ((qword_681450 == qword_6813A8
                            && (dword_683464 == 1
                                || dword_683464 == 2
                                || dword_683464 == 3))
                        || sub_462C30(qword_6814F8, qword_6810E0)) {
                        dword_681504 = 347;
                    } else {
                        dword_681504 = 343;
                    }
                    redraw_inven(false);
                    return true;
                }
            } else {
                if (dword_681504 != 342) {
                    dword_681504 = 342;
                    redraw_inven(false);
                    return true;
                }
            }
        }
    }

    return false;
}

// 0x574080
static inline bool inven_ui_message_filter_handle_mouse_lbutton_down(TigMessage* msg)
{
    int64_t v1;
    int64_t v2;
    S4F2810 v3;

    switch (sub_551A00()) {
    case 8:
    case 10:
    case 11:
    case 4:
        dword_68346C = true;
        if (qword_6810E0 == OBJ_HANDLE_NULL) {
            qword_6810E0 = sub_575FA0(msg->data.mouse.x, msg->data.mouse.y, &qword_681450);
            if (qword_6810E0 != OBJ_HANDLE_NULL) {
                dword_683470 = sub_575CB0(msg->data.mouse.x, msg->data.mouse.y, &v2);
                dword_6810E8 = item_inventory_location_get(qword_6810E0);
                dword_683470 -= dword_6810E8;
                sub_5754C0(msg->data.mouse.x, msg->data.mouse.y);
            }
            return true;
        }
        break;
    case 2:
        v1 = sub_575FA0(msg->data.mouse.x, msg->data.mouse.y, &qword_681450);
        if (v1 != OBJ_HANDLE_NULL) {
            sub_4F2810(&v3, v1);
            sub_57A1F0(&v3);
            sub_57A1A0();
            return true;
        }
        break;
    case 1:
        v1 = sub_575FA0(msg->data.mouse.x, msg->data.mouse.y, &qword_681450);
        if (v1 != OBJ_HANDLE_NULL) {
            sub_4F2810(&v3, v1);
            sub_57C110(&v3);
            return true;
        }
        break;
    case 16:
        v1 = sub_575FA0(msg->data.mouse.x, msg->data.mouse.y, &qword_681450);
        if (v1 != OBJ_HANDLE_NULL) {
            sub_4F2810(&v3, v1);
            sub_571CB0(&v3);
            return true;
        }
        break;
    }

    return false;
}

// 0x5747E4
static inline bool inven_ui_message_filter_handle_mouse_lbutton_up_accept_drop(TigMessage* msg, bool* v45)
{
    int64_t v3;
    int64_t v2;
    int inventory_location;
    int new_inventory_location;
    int err;
    int64_t old_item_obj;
    int v5;

    if (dword_681394
        && msg->data.mouse.x >= stru_6813B0.x
        && msg->data.mouse.x < stru_6813B0.x + stru_6813B0.width
        && msg->data.mouse.y >= stru_6813B0.y
        && msg->data.mouse.y < stru_6813B0.y + stru_6813B0.height) {
        if (qword_681450 == qword_6813A8) {
            switch (dword_683464) {
            case 1:
                if (dword_6810FC) {
                    sub_5788C0(qword_6810E0, OBJ_HANDLE_NULL, -1, 2);
                } else {
                    sub_575770();
                }
                break;
            case 2:
            case 3:
                sub_575770();
                break;
            default:
                sub_5788C0(qword_6810E0, OBJ_HANDLE_NULL, -1, 2);
                break;
            }
        } else {
            sub_5788C0(qword_6810E0, OBJ_HANDLE_NULL, -1, 2);
        }

        qword_6810E0 = OBJ_HANDLE_NULL;
        dword_6813A4 = 224;

        return false;
    }

    if (dword_681448
        && msg->data.mouse.x >= stru_681108.x
        && msg->data.mouse.x < stru_681108.x + stru_681108.width
        && msg->data.mouse.y >= stru_681108.y
        && msg->data.mouse.y < stru_681108.y + stru_681108.height) {
        v3 = qword_6810E0;
        sub_575770();
        sub_579B60(v3);
        qword_6810E0 = OBJ_HANDLE_NULL;
        dword_682C70 = 345;

        return false;
    }

    if (dword_681118
        && msg->data.mouse.x >= stru_681380.x
        && msg->data.mouse.x < stru_681380.x + stru_681380.width
        && msg->data.mouse.y >= stru_681380.y
        && msg->data.mouse.y < stru_681380.y + stru_681380.height) {
        if ((qword_681450 == qword_6813A8
                && (dword_683464 == 1
                    || dword_683464 == 2
                    || dword_683464 == 3)
            || sub_462C30(qword_6814F8, qword_6810E0))) {
            sub_575770();
            qword_6810E0 = OBJ_HANDLE_NULL;
            dword_681504 = 342;

            return false;
        }

        if ((obj_field_int32_get(qword_6810E0, OBJ_F_ITEM_FLAGS) & OIF_USE_IS_THROW) == 0
            || dword_683464) {
            v3 = qword_6810E0;
            sub_575770();
            if (!sub_4B6D70() || sub_4B6D80() == qword_6814F8) {
                if (tig_kb_is_key_pressed(DIK_LSHIFT)
                    || tig_kb_is_key_pressed(DIK_RSHIFT)) {
                    sub_462CC0(qword_6814F8, v3, qword_6814F8);
                } else {
                    sub_462CC0(qword_6814F8, v3, OBJ_HANDLE_NULL);
                }
            }

            if (!inven_ui_created) {
                return true;
            }

            qword_6810E0 = OBJ_HANDLE_NULL;
            dword_681504 = 342;
        } else {
            sub_5788C0(qword_6810E0, OBJ_HANDLE_NULL, -1, 4);
            redraw_inven(false);
            sub_466260(qword_6814F8, dword_68111C);
            if (qword_6813A8 != OBJ_HANDLE_NULL) {
                sub_466260(qword_6813A8, dword_681518);
            }
        }

        return false;
    }

    inventory_location = sub_575CB0(msg->data.mouse.x, msg->data.mouse.y, &v2);
    if (inventory_location >= 2000 && inventory_location <= 2009) {
        return false;
    }

    if (inventory_location >= 1000 && inventory_location <= 1008) {
        if (dword_683464 == 1) {
            if (qword_681450 != qword_6814F8 && v2 != qword_6814F8 && !dword_6810FC) {
                sub_575770();
                return false;
            }
        } else {
            if (dword_683464 == 3 && qword_681450 != qword_6814F8 && v2 != qword_6814F8) {
                sub_575770();
                return false;
            }
        }

        err = sub_464D20(qword_6810E0, inventory_location, v2);
        if (err != 0) {
            sub_4673F0(v2, err);
            sub_575770();
        }

        old_item_obj = item_wield_get(v2, inventory_location);
        if (old_item_obj == OBJ_HANDLE_NULL || old_item_obj == qword_6810E0) {
            if (v2 == qword_6814F8) {
                if (!sub_575180(v45)) {
                    sub_575200(inventory_location);
                }
            } else {
                if (!sub_575100(v45)) {
                    sub_575360(inventory_location);
                }
            }

            qword_6810E0 = OBJ_HANDLE_NULL;
            return false;
        }

        if (v2 != qword_6814F8) {
            sub_575770();
            return false;
        }

        err = sub_466DA0(old_item_obj);
        if (err != 0) {
            if (v2 == qword_6814F8) {
                sub_4673F0(v2, err);
            }
            sub_575770();
            return false;
        }

        if ((dword_683464 == 1 || dword_683464 == 3) && qword_681450 != v2) {
            sub_575770();
            return false;
        }

        sub_45F920();
        item_remove(qword_6810E0);
        new_inventory_location = item_inventory_location_get(old_item_obj);
        item_remove(old_item_obj);
        err = sub_466510(old_item_obj, v2, &v5);
        if (err == 0) {
            item_insert(qword_6810E0, qword_681450, dword_6810E8);
            sub_5788C0(qword_6810E0, v2, inventory_location, 1);
            item_insert(old_item_obj, v2, v5);
            qword_6810E0 = old_item_obj;
            dword_6810E8 = v5;
            qword_681450 = v2;
            dword_683470 = 0;
            sub_5754C0(-1, -1);
        } else {
            sub_4673F0(v2, err);
            item_insert(old_item_obj, v2, new_inventory_location);
            sub_575930();
        }
        sub_45F910();

        return false;
    }

    if (inventory_location - dword_683470 < 0) {
        if (!sub_57EDA0(1)) {
            sub_575770();
        }
    } else if (v2 == qword_6814F8) {
        if (!sub_575180(v45)) {
            sub_575200(inventory_location - dword_683470);
        }
    } else {
        if (sub_575100(v45)) {
            sub_575360(inventory_location - dword_683470);
        }
    }

    sub_57F260();

    qword_6810E0 = OBJ_HANDLE_NULL;
    return false;
}

// 0x574559
static inline bool inven_ui_message_filter_handle_mouse_lbutton_up(TigMessage* msg, bool* v45)
{
    int64_t v1;
    int64_t v2;
    int64_t v3;
    int inventory_location;

    dword_68346C = 0;
    if (sub_551000(msg->data.mouse.x, msg->data.mouse.y)) {
        if (!sub_541680()) {
            if (qword_6810E0 != OBJ_HANDLE_NULL) {
                if (!dword_683464) {
                    sub_5788C0(qword_6810E0, OBJ_HANDLE_NULL, -1, 4);
                    redraw_inven(false);
                    sub_466260(qword_6814F8, dword_68111C);
                    if (qword_6813A8) {
                        sub_466260(qword_6813A8, dword_681518);
                    }
                    return false;
                }

                sub_575770();
            }

            *v45 = true;
        }

        return false;
    }

    if (qword_6810E0 == OBJ_HANDLE_NULL) {
        v1 = sub_575FA0(msg->data.mouse.x, msg->data.mouse.y, &v2);
        if (v1 == OBJ_HANDLE_NULL) {
            return false;
        }

        switch (dword_683464) {
        case 5:
            sub_5786C0(v1);
            if (dword_681440 > 0
                && item_gold_get(qword_6814F8) >= dword_681440) {
                sub_467440(v1, qword_682C78, qword_6814F8, dword_681440);
            } else {
                sub_413910(qword_682C78, qword_6814F8, byte_68241C);
            }
            break;
        case 6:
            sub_578760(v1);
            if (dword_681440 > 0
                && item_gold_get(qword_6814F8) >= dword_681440) {
                sub_4C8E60(v1, qword_682C78, qword_6814F8, dword_681440);
            } else {
                sub_413910(qword_682C78, qword_6814F8, byte_68241C);
            }
            break;
        default:
            return false;
        }

        byte_68241C[0] = '\0';
        dword_681440 = -1;
        redraw_inven(false);

        return false;
    }



    redraw_inven(false);
    sub_466260(qword_6814F8, dword_68111C);
    if (qword_6813A8) {
        sub_466260(qword_6813A8, dword_681518);
    }

    if (!qword_6810E0) {
        sub_553990();
    }
    if (!v45) {
        return true;
    }

    return false;
}

// 0x5742A4
static inline bool inven_ui_message_filter_handle_mouse_rbutton_up(TigMessage* msg, bool* v45)
{
    int64_t v1;
    int64_t v2;
    int64_t v3;
    int rc;
    int v5;
    int64_t v6;
    int inventory_location;

    v1 = sub_575FA0(msg->data.mouse.x, msg->data.mouse.y, &v2);
    if (v1 == OBJ_HANDLE_NULL) {
        return false;
    }

    if (dword_683464 == 0) {
        inventory_location = item_inventory_location_get(v1);
        if (inventory_location < 1000 || inventory_location > 1008) {
            inventory_location = item_location_get(v1);
            if (inventory_location == -1
                && sub_462C30(qword_6814F8, v1) == ITEM_CANNOT_OK) {
                if (!sub_4B6D70()
                    || sub_4B6D80() == qword_6814F8) {
                    sub_462CC0(qword_6814F8, v1, qword_6814F8);
                }

                if (!inven_ui_created) {
                    return false;
                }
            }
        }
    } else if (dword_683464 > 0 && dword_683464 <= 3) {
        v3 = qword_6814F8;
        if (v2 == qword_6814F8) {
            v3 = qword_6813A8;
            v6 = qword_6813A8;
        } else {
            v6 = qword_6814F8;
        }

        rc = sub_466510(v1, v3, &v5);
        if (rc != ITEM_CANNOT_OK) {
            sub_4673F0(v2, rc);
            return true;
        }

        qword_6810E0 = v1;
        qword_681450 = v2;
        dword_683470 = sub_575CB0(msg->data.mouse.x, msg->data.mouse.y, &v1);
        dword_6810E8 = item_inventory_location_get(qword_6810E0);
        dword_683470 -= dword_6810E8;
        sub_5754C0(msg->data.mouse.x, msg->data.mouse.y);
        if (qword_6810E0 != OBJ_HANDLE_NULL) {
            if (v6 == qword_6814F8) {
                if (!sub_575180(v45)) {
                    sub_575200(v5);
                }
            } else {
                if (!sub_575100(v45)) {
                    sub_575360(v5);
                }
            }
            qword_6810E0 = OBJ_HANDLE_NULL;
        }
    }

    redraw_inven(false);
    sub_466260(qword_6814F8, dword_68111C);
    if (qword_6813A8 != OBJ_HANDLE_NULL) {
        sub_466260(qword_6813A8, dword_681518);
    }

    if (qword_6810E0 == OBJ_HANDLE_NULL) {
        sub_553990();
    }

    if (!v45) {
        return true;
    }

    return false;
}

// NOTE: This function is huge and hard to work with. Some chunks were
// externalized into separate inline functions above. This leads to some
// duplication of the code and acts as an optimization fence for compiler.
//
// 0x573850
bool inven_ui_message_filter(TigMessage* msg)
{
    bool v45 = false;
    int64_t v49;
    int64_t v8;

    if (qword_6810E0 != OBJ_HANDLE_NULL) {
        sub_5811A0();
    } else {
        sub_5811B0();
    }

    switch (msg->type) {
    case TIG_MESSAGE_BUTTON:
        switch (msg->data.button.state) {
        case TIG_BUTTON_STATE_PRESSED:
            // 0x573897
            if (inven_ui_message_filter_handle_button_pressed(msg)) {
                return true;
            }
            break;
        case TIG_BUTTON_STATE_RELEASED:
            // 0x573A95
            if (inven_ui_message_filter_handle_button_released(msg)) {
                return true;
            }
            break;
        case TIG_BUTTON_STATE_MOUSE_INSIDE:
            // 0x573BAF
            if (inven_ui_mssage_filter_handle_button_hovered(msg)) {
                return true;
            }
            break;
        }
        break;
    case TIG_MESSAGE_MOUSE:
        if (msg->type == TIG_MESSAGE_MOUSE_IDLE) {
            // 0x573C80
            inven_ui_message_filter_handle_mouse_idle(msg);
        } else if (msg->type == TIG_MESSAGE_MOUSE_MOVE) {
            // 0x573D61
            if (inven_ui_message_filter_handle_mouse_move(msg)) {
                return true;
            }
        } else if (msg->type == TIG_MESSAGE_MOUSE_LEFT_BUTTON_DOWN
            && !dword_68346C
            && dword_683464 != 4
            && dword_683464 != 5
            && dword_683464 != 6) {
            // 0x574080
            if (inven_ui_message_filter_handle_mouse_lbutton_down(msg)) {
                return true;
            }
        } else if (msg->type == TIG_MESSAGE_MOUSE_LEFT_BUTTON_UP
            || (msg->type == TIG_MESSAGE_MOUSE_RIGHT_BUTTON_UP && sub_54EB50())) {
            // 0x574559
            if (inven_ui_message_filter_handle_mouse_lbutton_up(msg, &v45)) {
                return true;
            }
        } else if (msg->type == TIG_MESSAGE_MOUSE_RIGHT_BUTTON_UP
            && qword_6810E0 == OBJ_HANDLE_NULL
            && (dword_683464 == 0
                || dword_683464 == 1
                || dword_683464 == 2
                || dword_683464 == 3)) {
            // 0x5742A4
            if (inven_ui_message_filter_handle_mouse_rbutton_up(msg, &v45)) {
                return true;
            }
        }
        break;
    }

    if (dword_683464 == 1
        && qword_6810E0 == OBJ_HANDLE_NULL
        && sub_551A00() != 13
        && (critter_is_sleeping(qword_682C78) || !sub_45D790(qword_682C78))) {
        v45 = true;
    }

    if (v45 || dword_739F58) {
        inven_ui_destroy();
        return true;
    }

    return false;
}

// 0x574FD0
void sub_574FD0(bool a1)
{
    int64_t v1;
    int64_t v2;
    int v3;

    sub_575770();

    v1 = qword_6813A8;

    if (a1) {
        do {
            v1 = sub_45E590(v1);
        } while (!sub_575080(qword_6814F8, v1));
    } else {
        do {
            v1 = sub_45E4F0(v1);
        } while (!sub_575080(qword_6814F8, v1));
    }

    if (v1 != qword_6813A8) {
        v2 = qword_6814F8;
        v3 = dword_683464;
        inven_ui_destroy();
        sub_572240(v2, v1, v3);
    }
}

// 0x575080
bool sub_575080(int64_t a1, int64_t a2)
{
    if (inven_ui_created && a2 == qword_6813A8) {
        return true;
    }

    if (sub_4AD800(a2, a1, 0) != 0) {
        return false;
    }

    if ((obj_field_int32_get(a2, OBJ_F_CRITTER_FLAGS) & (OCF_ANIMAL | OCF_MECHANICAL)) != 0) {
        return false;
    }

    if ((obj_field_int32_get(a2, OBJ_F_SPELL_FLAGS) & OSF_SUMMONED) != 0) {
        return false;
    }

    return true;
}

// 0x575100
bool sub_575100(bool* a1)
{
    int64_t v1;

    if (dword_683464 != 3 || qword_681450 != qword_6814F8) {
        return false;
    }

    v1 = qword_6810E0;
    sub_575770();

    if (sub_4C7010(qword_6814F8, qword_682C78, v1)) {
        *a1 = true;
    }

    return true;
}

// 0x575180
bool sub_575180(bool* a1)
{
    int64_t v1;

    if (dword_683464 != 3 || qword_681450 == qword_6814F8) {
        return false;
    }

    v1 = qword_6810E0;
    sub_575770();

    if (sub_4C6FD0(qword_6814F8, qword_682C78, v1)) {
        *a1 = true;
    }

    return true;
}

// 0x575200
void sub_575200(int a1)
{
    if (dword_683464 == 1 && qword_681450 != qword_6814F8) {
        if (dword_681440 == -1 || qword_6810E0 != qword_681458) {
            sub_578330(qword_6810E0, qword_682C78);
        }

        if (dword_681440 != 0 || dword_6810FC) {
            if (item_gold_get(qword_6814F8) < dword_681440) {
                sub_413910(qword_682C78, qword_6814F8, byte_68241C);
                sub_575770();
            } else {
                sub_5788C0(qword_6810E0, qword_6814F8, a1, 1);
            }
        } else {
            sub_575770();
            dword_681440 = -1;
        }
    } else {
        sub_5788C0(qword_6810E0, qword_6814F8, a1, 1);
    }

    qword_681458 = OBJ_HANDLE_NULL;
}

// 0x575360
void sub_575360(int a1)
{
    int cost;

    if (dword_683464 != 1) {
        sub_5788C0(qword_6810E0, qword_6813A8, a1, 1);
        qword_681458 = OBJ_HANDLE_NULL;
        return;
    }

    if (qword_681450 != qword_6814F8) {
        if (!dword_6810FC) {
            sub_575770();
        } else {
            sub_5788C0(qword_6810E0, qword_6813A8, a1, 1);
        }
        qword_681458 = OBJ_HANDLE_NULL;
        return;
    }

    if (dword_681440 == -1 || qword_6810E0 != qword_681458) {
        sub_578330(qword_6810E0, qword_6814F8);
    }

    if (dword_681440 != 0 || dword_6810FC) {
        cost = item_gold_get(qword_682C78);
        if (cost < dword_681440) {
            dword_681440 = cost;
        }

        sub_5788C0(qword_6810E0, qword_6813A8, a1, 1);
        qword_681458 = OBJ_HANDLE_NULL;
        dword_681440 = -1;
    } else {
        sub_575770();
        qword_681458 = OBJ_HANDLE_NULL;
        dword_681440 = -1;
    }
}

// 0x5754C0
void sub_5754C0(int x, int y)
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
    int64_t location;
    tig_sound_handle_t sound_id;

    location = obj_field_int64_get(qword_681450, OBJ_F_LOCATION);
    sub_466E50(qword_6810E0, location);

    sound_id = sub_4F0BF0(qword_6810E0, qword_681450, 0, 1);
    gsound_play_sfx_id(sound_id, 1);
}

// 0x575C50
void sub_575C50(int64_t obj)
{
    if (qword_6810E0 != OBJ_HANDLE_NULL && obj == qword_6810E0) {
        sub_4A51C0(player_get_pc_obj(), qword_6810E0);
        qword_6810E0 = OBJ_HANDLE_NULL;
        sub_553990();
        redraw_inven(false);
    }
}

// 0x575CB0
int sub_575CB0(int x, int y, int64_t* a3)
{
    // TODO: Incomplete.
}

// 0x575FA0
int64_t sub_575FA0(int x, int y, int64_t* a3)
{
    // TODO: Incomplete.
}

// 0x576100
void sub_576100(int64_t obj)
{
    if (inven_ui_created) {
        if (obj == qword_6814F8) {
            sub_466260(qword_6814F8, dword_68111C);
            redraw_inven(false);
        } else if (obj == qword_6813A8 || obj == qword_682C78) {
            sub_466260(qword_6813A8, dword_681518);
            redraw_inven(false);
        } else if (obj != OBJ_HANDLE_NULL) {
            sub_466260(qword_6814F8, dword_68111C);
            if (qword_6813A8 != OBJ_HANDLE_NULL) {
                sub_466260(qword_6813A8, dword_681518);
            }
            redraw_inven(false);
        }
    }
}

// 0x5761D0
void redraw_inven_fix_bad_inven_obj(int64_t a1)
{
    // TODO: Incomplete.
}

// 0x5764B0
bool redraw_inven_fix_bad_inven(int64_t a1, int64_t a2)
{
    int64_t parent_obj;

    if (!item_parent(a1, &parent_obj)) {
        tig_debug_printf("Warning: redraw_inven_fix_bad_inven called on item that doesn't think it has a parent.\n");
        item_force_remove(a1, a2);
        return true;
    }

    if (parent_obj != a2) {
        tig_debug_printf("Warning: redraw_inven_fix_bad_inven called on item with different parent.\n");
        item_force_remove(a1, a2);
        return true;
    }

    return false;
}

// 0x576520
void redraw_inven(bool a1)
{
    TigArtBlitInfo art_blit_info;
    TigArtFrameData art_frame_data;
    TigArtAnimData art_anim_data;
    TigRect src_rect;
    TigRect dst_rect;
    TigFont font_desc;
    TigPaletteModifyInfo palette_modify_info;
    MesFileEntry mes_file_entry1;
    MesFileEntry mes_file_entry2;
    TigRect* text_rects;
    char str[80];
    int portrait;
    size_t pos;
    int index;
    bool v1[9];
    int inventory_cnt;
    int64_t item_obj;
    int item_type;
    unsigned int item_flags;
    int inventory_location;
    int weapon_min_str;
    bool weapon_too_heavy;

    art_blit_info.art_id = sub_5782D0();
    if (tig_art_frame_data(art_blit_info.art_id, &art_frame_data) != TIG_OK) {
        return;
    }

    src_rect.x = 0;
    src_rect.y = 0;
    src_rect.width = art_frame_data.width;
    src_rect.height = art_frame_data.height;

    dst_rect.x = 0;
    dst_rect.y = 0;
    dst_rect.width = art_frame_data.width;
    dst_rect.height = art_frame_data.height;

    art_blit_info.flags = 0;
    art_blit_info.src_rect = &src_rect;
    art_blit_info.dst_rect = &dst_rect;
    tig_window_blit_art(inven_ui_window_handle, &art_blit_info);

    // 0x5765A5
    switch (dword_683464) {
    case 1:
        if (sub_553D10(qword_6814F8, qword_682C78, &portrait)) {
            portrait_draw_native(qword_682C78, portrait, inven_ui_window_handle, 183, 33);
        } else {
            tig_art_interface_id_create(portrait, 0, 0, 0, &(art_blit_info.art_id));
            tig_art_frame_data(art_blit_info.art_id, &art_frame_data);

            src_rect.x = 0;
            src_rect.y = 0;
            src_rect.width = art_frame_data.width;
            src_rect.height = art_frame_data.width;

            dst_rect.x = 183;
            dst_rect.y = 33;
            dst_rect.width = art_frame_data.width;
            dst_rect.height = art_frame_data.width;

            art_blit_info.flags = 0;
            art_blit_info.src_rect = &src_rect;
            art_blit_info.dst_rect = &dst_rect;
            tig_window_blit_art(inven_ui_window_handle, &art_blit_info);
        }

        tig_font_push(dword_682418);
        src_rect.x = 215;
        src_rect.y = 102;
        src_rect.width = 200;
        src_rect.height = 20;

        font_desc.str = byte_682C8C;
        font_desc.width = 0;
        sub_535390(&font_desc);

        src_rect.x -= font_desc.width / 2;
        tig_window_text_write(inven_ui_window_handle, font_desc.str, &src_rect);
        tig_font_pop();

        tig_font_push(dword_682C74);
        src_rect.x = 89;
        src_rect.y = 128;
        src_rect.width = 253;
        src_rect.height = 30;

        if (dword_6810FC && byte_68241C[0] == '\0') {
            int encumbrance_level;
            int carry_weight;

            mes_file_entry1.num = 9;
            mes_get_msg(inven_ui_mes_file, &mes_file_entry1);

            mes_file_entry2.num = 6;
            mes_get_msg(inven_ui_mes_file, &mes_file_entry2);

            sprintf(byte_68241C,
                "%s: %d %s\n",
                mes_file_entry1.str,
                item_total_weight(qword_682C78),
                mes_file_entry2.str);
            pos = strlen(byte_68241C);

            mes_file_entry1.num = 10;
            mes_get_msg(inven_ui_mes_file, &mes_file_entry1);

            encumbrance_level = sub_45F790(qword_682C78);
            carry_weight = stat_level(qword_682C78, STAT_CARRY_WEIGHT);

            sprintf(&(byte_68241C[pos]),
                "%s: %s (%d)\n",
                mes_file_entry1.str,
                critter_encumbrance_level_name(encumbrance_level),
                critter_encumbrance_level_ratio(encumbrance_level) * carry_weight / 100);
        } else {
            pos = 0;
        }

        tig_window_text_write(inven_ui_window_handle, byte_68241C, &src_rect);

        if (pos != 0) {
            byte_68241C[0] = '\0';
        }
        tig_font_pop();
        break;
    case 2:
    case 3:
    case 4:
        if (sub_553D10(qword_6814F8, qword_682C78, &portrait)) {
            portrait_draw_native(qword_682C78, portrait, inven_ui_window_handle, stru_5CAF80.x, stru_5CAF80.y);
        } else {
            tig_art_interface_id_create(portrait, 0, 0, 0, &(art_blit_info.art_id));
            tig_art_frame_data(art_blit_info.art_id, &art_frame_data);

            src_rect.x = 0;
            src_rect.y = 0;
            src_rect.width = art_frame_data.width;
            src_rect.height = art_frame_data.width;

            dst_rect.x = stru_5CAF80.x;
            dst_rect.y = stru_5CAF80.y;
            dst_rect.width = art_frame_data.width;
            dst_rect.height = art_frame_data.width;

            art_blit_info.flags = 0;
            art_blit_info.src_rect = &src_rect;
            art_blit_info.dst_rect = &dst_rect;
            tig_window_blit_art(inven_ui_window_handle, &art_blit_info);
        }
        break;
    case 5:
    case 6:
        if (sub_553D10(qword_6814F8, qword_682C78, &portrait)) {
            portrait_draw_32x32(qword_682C78, portrait, inven_ui_window_handle, 141, 36);
        } else {
            tig_art_interface_id_create(portrait, 0, 0, 0, &(art_blit_info.art_id));
            tig_art_frame_data(art_blit_info.art_id, &art_frame_data);

            src_rect.x = 0;
            src_rect.y = 0;
            src_rect.width = art_frame_data.width;
            src_rect.height = art_frame_data.width;

            dst_rect.x = 141;
            dst_rect.y = 36;
            dst_rect.width = art_frame_data.width;
            dst_rect.height = art_frame_data.width;

            art_blit_info.flags = 0;
            art_blit_info.src_rect = &src_rect;
            art_blit_info.dst_rect = &dst_rect;
            tig_window_blit_art(inven_ui_window_handle, &art_blit_info);
        }

        tig_font_push(dword_682C74);
        src_rect = stru_5CAFB0;
        tig_window_text_write(inven_ui_window_handle, byte_68241C, &src_rect);
        tig_font_pop();
        break;
    }

    // 0x576B04
    tig_art_interface_id_create(221, 0, 0, 0, &(art_blit_info.art_id));
    if (tig_art_frame_data(art_blit_info.art_id, &art_frame_data) != TIG_OK) {
        return;
    }

    src_rect.x = 0;
    src_rect.y = 0;
    src_rect.width = art_frame_data.width;
    src_rect.height = art_frame_data.width;

    dst_rect.x = 358;
    dst_rect.y = 0;
    dst_rect.width = art_frame_data.width;
    dst_rect.height = art_frame_data.width;

    art_blit_info.flags = 0;
    art_blit_info.src_rect = &src_rect;
    art_blit_info.dst_rect = &dst_rect;
    tig_window_blit_art(inven_ui_window_handle, &art_blit_info);

    if (dword_68150C) {
        tig_art_interface_id_create(341, 0, 0, 0, &(art_blit_info.art_id));
        if (tig_art_frame_data(art_blit_info.art_id, &art_frame_data) != TIG_OK) {
            return;
        }

        src_rect.x = 0;
        src_rect.y = 0;
        src_rect.width = art_frame_data.width;
        src_rect.height = art_frame_data.width;

        dst_rect.x = 361;
        dst_rect.y = 0;
        dst_rect.width = art_frame_data.width;
        dst_rect.height = art_frame_data.width;

        art_blit_info.flags = 0;
        art_blit_info.src_rect = &src_rect;
        art_blit_info.dst_rect = &dst_rect;
        tig_window_blit_art(inven_ui_window_handle, &art_blit_info);
    }

    // 0x576C38
    if (dword_683464 == 0) {
        text_rects = stru_5CAE98;
    } else {
        if (dword_68150C) {
            text_rects = stru_5CAEC8;
        } else {
            text_rects = NULL;
        }
    }

    if (text_rects != NULL) {
        int encumbrance_level;
        int carry_weight;

        tig_font_push(dword_682C74);

        mes_file_entry1.num = 9;
        mes_get_msg(inven_ui_mes_file, &mes_file_entry1);

        mes_file_entry2.num = 6;
        mes_get_msg(inven_ui_mes_file, &mes_file_entry2);

        sprintf(str,
            "%s: %d %s",
            mes_file_entry1.str,
            item_total_weight(qword_6814F8),
            mes_file_entry2.str);
        tig_window_text_write(inven_ui_window_handle, str, &(text_rects[0]));

        mes_file_entry1.num = 10;
        mes_get_msg(inven_ui_mes_file, &mes_file_entry1);

        encumbrance_level = sub_45F790(qword_6814F8);
        carry_weight = stat_level(qword_6814F8, STAT_CARRY_WEIGHT);

        sprintf(str,
            "%s: %s (%d)",
            mes_file_entry1.str,
            critter_encumbrance_level_name(encumbrance_level),
            critter_encumbrance_level_ratio(encumbrance_level) * carry_weight / 100);
        tig_window_text_write(inven_ui_window_handle, str, &(text_rects[1]));

        mes_file_entry1.num = 11;
        mes_get_msg(inven_ui_mes_file, &mes_file_entry1);

        sprintf(str,
            "%s: %d",
            mes_file_entry1.str,
            stat_level(qword_6814F8, STAT_SPEED));
        tig_window_text_write(inven_ui_window_handle, str, &(text_rects[2]));

        tig_font_pop();
    }

    // 0x576DFE
    if (dword_683464 == 0
        || dword_683464 == 5
        || dword_683464 == 6) {
        text_rects = stru_5CAEF8;
    } else {
        if (dword_68150C) {
            text_rects = stru_5CAF18;
        } else {
            text_rects = NULL;
        }
    }

    if (text_rects != NULL) {
        tig_font_push(dword_681390);

        sprintf(str, "%d", item_total_attack(qword_6814F8));

        font_desc.str = str;
        font_desc.width = 0;
        sub_535390(&font_desc);

        src_rect.x = text_rects[0].x + text_rects[0].width - font_desc.width;
        src_rect.y = text_rects[0].y;
        src_rect.width = text_rects[0].width;
        src_rect.height = text_rects[0].height;
        tig_window_text_write(inven_ui_window_handle, str, &src_rect);

        sprintf(str, "%d", item_total_defence(qword_6814F8));
        tig_window_text_write(inven_ui_window_handle, str, &(text_rects[1]));

        tig_font_pop();
    }

    // 0x576F05
    tig_font_push(dword_68345C);
    for (index = 0; index < 5; index++) {
        int value;

        if (index == 0) {
            value = item_gold_get(qword_6814F8);
        } else {
            value = item_ammo_quantity_get(qword_6814F8, index - 1);
        }

        src_rect.x = dword_5CAFC0[index];
        src_rect.y = dword_5CAFD4[index];
        src_rect.width = 58;
        src_rect.height = 20;

        sprintf(str, "%d", value);
        tig_window_text_write(inven_ui_window_handle, str, &src_rect);
    }
    tig_font_pop();

    // 0x576FBE
    if (dword_681394) {
        tig_art_interface_id_create(dword_6813A4, 0, 0, 0, &(art_blit_info.art_id));
        if (tig_art_frame_data(art_blit_info.art_id, &art_frame_data) != TIG_OK) {
            return;
        }

        src_rect.x = 0;
        src_rect.y = 0;
        src_rect.width = art_frame_data.width;
        src_rect.height = art_frame_data.width;

        dst_rect.x = 703;
        dst_rect.y = 344;
        dst_rect.width = art_frame_data.width;
        dst_rect.height = art_frame_data.width;

        art_blit_info.flags = 0;
        art_blit_info.src_rect = &src_rect;
        art_blit_info.dst_rect = &dst_rect;
        tig_window_blit_art(inven_ui_window_handle, &art_blit_info);

        stru_6813B0.width = art_frame_data.width;
        stru_6813B0.height = art_frame_data.height;
    }

    // 0x577070
    if (dword_681448) {
        tig_art_interface_id_create(dword_682C70, 0, 0, 0, &(art_blit_info.art_id));
        if (tig_art_frame_data(art_blit_info.art_id, &art_frame_data) != TIG_OK) {
            return;
        }

        src_rect.x = 0;
        src_rect.y = 0;
        src_rect.width = art_frame_data.width;
        src_rect.height = art_frame_data.width;

        dst_rect.x = 703;
        dst_rect.y = 236;
        dst_rect.width = art_frame_data.width;
        dst_rect.height = art_frame_data.width;

        art_blit_info.flags = 0;
        art_blit_info.src_rect = &src_rect;
        art_blit_info.dst_rect = &dst_rect;
        tig_window_blit_art(inven_ui_window_handle, &art_blit_info);

        stru_681108.width = art_frame_data.width;
        stru_681108.height = art_frame_data.height;
    }

    // 0x57711C
    if (dword_681118) {
        tig_art_interface_id_create(dword_681504, 0, 0, 0, &(art_blit_info.art_id));
        if (tig_art_frame_data(art_blit_info.art_id, &art_frame_data) != TIG_OK) {
            return;
        }

        src_rect.x = 0;
        src_rect.y = 0;
        src_rect.width = art_frame_data.width;
        src_rect.height = art_frame_data.width;

        dst_rect.x = 703;
        dst_rect.y = 290;
        dst_rect.width = art_frame_data.width;
        dst_rect.height = art_frame_data.width;

        art_blit_info.flags = 0;
        art_blit_info.src_rect = &src_rect;
        art_blit_info.dst_rect = &dst_rect;
        tig_window_blit_art(inven_ui_window_handle, &art_blit_info);

        stru_681380.width = art_frame_data.width;
        stru_681380.height = art_frame_data.height;
    }

    // 0x5771C8
    if (dword_683464 == 0
        || dword_683464 == 5
        || dword_683464 == 6) {
        text_rects = stru_5CAC58;
        memset(v1, 0, sizeof(v1));
    } else {
        if (dword_68150C) {
            text_rects = stru_5CACE8;
            memset(v1, 0, sizeof(v1));
        } else {
            text_rects = NULL;
        }
    }

    inventory_cnt = obj_field_int32_get(qword_6814F8, OBJ_F_CRITTER_INVENTORY_NUM);
    dst_rect = src_rect;
    art_blit_info.src_rect = &src_rect;
    art_blit_info.dst_rect = &dst_rect;

    for (index = 0; index < inventory_cnt; index++) {
        item_obj = obj_arrayfield_handle_get(qword_6814F8, OBJ_F_CRITTER_INVENTORY_LIST_IDX, index);
        if ((obj_field_int32_get(item_obj, OBJ_F_FLAGS) & OF_OFF) != 0) {
            continue;
        }

        item_flags = obj_field_int32_get(item_obj, OBJ_F_ITEM_FLAGS);
        if ((item_flags & OIF_NO_DISPLAY) != 0) {
            if (!a1) {
                continue;
            }
            item_flags &= ~OIF_NO_DISPLAY;
            obj_field_int32_set(item_obj, OBJ_F_ITEM_FLAGS, item_flags);
        }

        // 0x5772EE
        inventory_location = item_inventory_location_get(item_obj);
        if (inventory_location >= 2000 && inventory_location <= 2009) {
            continue;
        }

        // 0x57730A
        if (inventory_location >= 1000 && inventory_location <= 1008) {
            if (dword_683464 == 0
                || dword_68150C == 1
                || dword_683464 == 5
                || dword_683464 == 6) {
                weapon_too_heavy = false;
                if (inventory_location == 1004) {
                    weapon_min_str = item_weapon_min_strength(item_obj, qword_6814F8);
                    if (stat_level(qword_6814F8, STAT_STRENGTH) < weapon_min_str) {
                        dst_rect = text_rects[4];
                        tig_window_tint(inven_ui_window_handle,
                            &dst_rect,
                            tig_color_make(64, 0, 0),
                            0);
                        weapon_too_heavy = true;
                    }
                }

                switch (obj_field_int32_get(item_obj, OBJ_F_TYPE)) {
                case OBJ_TYPE_WEAPON:
                    art_blit_info.art_id = obj_field_int32_get(item_obj, OBJ_F_WEAPON_PAPER_DOLL_AID);
                    break;
                case OBJ_TYPE_ITEM_ARMOR:
                    art_blit_info.art_id = obj_field_int32_get(item_obj, OBJ_F_ARMOR_PAPER_DOLL_AID);
                    break;
                default:
                    art_blit_info.art_id = obj_field_int32_get(item_obj, OBJ_F_ITEM_INV_AID);
                    break;
                }

                if (tig_art_frame_data(art_blit_info.art_id, &art_frame_data) == TIG_OK) {
                    src_rect.height = art_frame_data.height;
                    src_rect.width = art_frame_data.width;

                    dst_rect.x = text_rects[inventory_location - 1000].x
                        + (text_rects[inventory_location - 1000].width - art_frame_data.width) / 2;
                    dst_rect.y = text_rects[inventory_location - 1000].y
                        + (text_rects[inventory_location - 1000].height - art_frame_data.height) / 2;
                    dst_rect.width = art_frame_data.width;
                    dst_rect.height = art_frame_data.height;

                    tig_window_blit_art(inven_ui_window_handle, &art_blit_info);

                    if (weapon_too_heavy) {
                        mes_file_entry1.num = 8;
                        mes_get_msg(inven_ui_mes_file, &mes_file_entry1);

                        sprintf(str, "%s: %d", mes_file_entry1.str, weapon_min_str);
                        tig_font_push(dword_682C74);
                        font_desc.str = str;
                        font_desc.width = 0;
                        sub_535390(&font_desc);
                        dst_rect.height = font_desc.height;
                        dst_rect.y += dst_rect.height - font_desc.height;
                        tig_window_text_write(inven_ui_window_handle, str, &dst_rect);
                        tig_font_pop();
                    }
                }

                v1[inventory_location - 1000] = true;
            }

            continue;
        }

        // 0x5775A7
        if (!dword_68150C) {
            int x;
            int y;
            int width;
            int height;

            x = 32 * (inventory_location % 10) + 368;
            y = 32 * (inventory_location / 10) + 8;
            item_inv_icon_size(item_obj, &width, &height);
            width *= 32;
            height *= 32;

            if (item_obj == qword_681458) {
                dst_rect.x = x;
                dst_rect.y = y;
                dst_rect.width = width;
                dst_rect.height = height;

                tig_window_tint(inven_ui_window_handle,
                    &dst_rect,
                    tig_color_make(64, 0, 0),
                    0);
            }

            art_blit_info.art_id = obj_field_int32_get(item_obj, OBJ_F_ITEM_INV_AID);
            if (tig_art_item_id_destroyed_get(obj_field_int32_get(item_obj, OBJ_F_CURRENT_AID)) != 0) {
                art_blit_info.art_id = tig_art_item_id_destroyed_set(art_blit_info.art_id, 0);
                tig_art_anim_data(art_blit_info.art_id, &art_anim_data);

                palette_modify_info.dst_palette = tig_palette_create();
                palette_modify_info.src_palette = art_anim_data.palette1;
                palette_modify_info.flags = TIG_PALETTE_MODIFY_TINT;
                palette_modify_info.tint_color = tig_color_make(255, 0, 0);
                tig_palette_modify(&palette_modify_info);

                art_blit_info.flags = TIG_ART_BLT_PALETTE_OVERRIDE;
                art_blit_info.palette = palette_modify_info.dst_palette;
            } else {
                palette_modify_info.dst_palette = NULL;
            }

            if (tig_art_frame_data(art_blit_info.art_id, &art_frame_data) == TIG_OK) {
                src_rect.height = art_frame_data.height;
                src_rect.width = art_frame_data.width;

                dst_rect.x = x + (width - art_frame_data.width) / 2;
                dst_rect.y = y + (height - art_frame_data.height) / 2;
                dst_rect.width = art_frame_data.width;
                dst_rect.height = art_frame_data.height;

                tig_window_blit_art(inven_ui_window_handle, &art_blit_info);
            }

            if (palette_modify_info.dst_palette != NULL) {
                tig_palette_destroy(palette_modify_info.dst_palette);
            }
        }
    }

    // 0x577837
    art_blit_info.flags = 0;
    if (text_rects != NULL) {
        for (index = 0; index < 9; index++) {
            if (!v1[index]) {
                tig_art_interface_id_create(dword_5CAF58[index], 0, 0, 0, &(art_blit_info.art_id));

                dst_rect.x = text_rects[index].x;
                dst_rect.y = text_rects[index].y;
                dst_rect.width = text_rects[index].width;
                dst_rect.height = text_rects[index].height;

                src_rect.width = stru_5CAC58[index].width;
                src_rect.height = stru_5CAC58[index].height;

                tig_window_blit_art(inven_ui_window_handle, &art_blit_info);
            }
        }
    }

    // 0x5778D8
    if (qword_6813A8 != OBJ_HANDLE_NULL
        && dword_683464 != 5
        && dword_683464 != 6) {
        int64_t target_obj;
        int v102;
        int v109;
        int v112;
        int inventory_list_fld;

        if (dword_681514) {
            target_obj = qword_682C78;
            memset(v1, 9, sizeof(v1));

            if (dword_683464 == 1) {
                tig_art_interface_id_create(337, 0, 0, 0, &(art_blit_info.art_id));
            } else {
                tig_art_interface_id_create(349, 0, 0, 0, &(art_blit_info.art_id));
            }
            if (tig_art_frame_data(art_blit_info.art_id, &art_frame_data) != TIG_OK) {
                return;
            }

            src_rect.x = 0;
            src_rect.y = 0;
            src_rect.width = art_frame_data.width;
            src_rect.height = art_frame_data.width;

            dst_rect.x = 0;
            dst_rect.y = 0;
            dst_rect.width = art_frame_data.width;
            dst_rect.height = art_frame_data.width;

            if (dword_683464 == 1) {
                dst_rect.y = 157;
                text_rects = stru_5CAD78;
            } else {
                dst_rect.y = 134;
                text_rects = stru_5CAE08;
            }

            art_blit_info.flags = 0;
            art_blit_info.src_rect = &src_rect;
            art_blit_info.dst_rect = &dst_rect;
            tig_window_blit_art(inven_ui_window_handle, &art_blit_info);

            tig_font_push(dword_682C74);
            sprintf(str, "%d", item_total_attack(qword_682C78));
            font_desc.str = str;
            font_desc.width = 0;
            sub_535390(&font_desc);
            src_rect.x = stru_5CAF38.x + stru_5CAF38.width - font_desc.width;
            src_rect.y = stru_5CAF38.y;
            src_rect.width = stru_5CAF38.width;
            src_rect.height = stru_5CAF38.height;
            tig_window_text_write(inven_ui_window_handle, str, &src_rect);
            sprintf(str, "%d", item_total_defence(qword_682C78));
            tig_window_text_write(inven_ui_window_handle, str, &src_rect);
            tig_font_pop();
        } else {
            target_obj = qword_6813A8;
            v112 = 8;
            if (dword_683464 == 1) {
                v102 = 168;
                v109 = 224;
            } else {
                v102 = 136;
                v109 = 256;
            }
        }

        if (obj_field_int32_get(target_obj, OBJ_F_TYPE) == OBJ_TYPE_CONTAINER) {
            inventory_cnt = obj_field_int32_get(target_obj, OBJ_F_CONTAINER_INVENTORY_NUM);
            inventory_list_fld = OBJ_F_CONTAINER_INVENTORY_LIST_IDX;
        } else {
            inventory_cnt = obj_field_int32_get(target_obj, OBJ_F_CRITTER_INVENTORY_NUM);
            inventory_list_fld = OBJ_F_CRITTER_INVENTORY_LIST_IDX;
        }

        src_rect.x = 0;
        src_rect.y = 0;

        art_blit_info.src_rect = &src_rect;
        art_blit_info.dst_rect = &dst_rect;

        for (index = 0; index < inventory_cnt; index++) {
            item_obj = obj_arrayfield_handle_get(target_obj, inventory_list_fld, index);
            if ((obj_field_int32_get(item_obj, OBJ_F_FLAGS) & OF_OFF) != 0) {
                continue;
            }

            item_type = obj_field_int32_get(item_obj, OBJ_F_TYPE);
            item_flags = obj_field_int32_get(item_obj, OBJ_F_ITEM_FLAGS);
            if ((item_flags & OIF_NO_DISPLAY) != 0) {
                if (!a1 || (item_type != OBJ_TYPE_ITEM_GOLD || item_type != OBJ_TYPE_AMMO)) {
                    continue;
                }
                item_flags &= ~OIF_NO_DISPLAY;
                obj_field_int32_set(item_obj, OBJ_F_ITEM_FLAGS, item_flags);
            }

            inventory_location = item_inventory_location_get(item_obj);

            art_blit_info.flags = 0;
            if (inventory_location >= 2000 && inventory_location <= 2009) {
                continue;
            }

            if (inventory_location >= 1000 && inventory_location <= 1008) {
                // 0x577CA0
                if (dword_681514) {
                    weapon_too_heavy = false;
                    if (inventory_location == 1004) {
                        weapon_min_str = item_weapon_min_strength(item_obj, target_obj);
                        if (stat_level(target_obj, STAT_STRENGTH) < weapon_min_str) {
                            dst_rect = text_rects[4];
                            tig_window_tint(inven_ui_window_handle,
                                &dst_rect,
                                tig_color_make(64, 0, 0),
                                0);
                            weapon_too_heavy = true;
                        }
                    }

                    switch (item_type) {
                    case OBJ_TYPE_WEAPON:
                        art_blit_info.art_id = obj_field_int32_get(item_obj, OBJ_F_WEAPON_PAPER_DOLL_AID);
                        break;
                    case OBJ_TYPE_ITEM_ARMOR:
                        art_blit_info.art_id = obj_field_int32_get(item_obj, OBJ_F_ARMOR_PAPER_DOLL_AID);
                        break;
                    default:
                        art_blit_info.art_id = obj_field_int32_get(item_obj, OBJ_F_ITEM_INV_AID);
                        break;
                    }

                    if (tig_art_frame_data(art_blit_info.art_id, &art_frame_data) == TIG_OK) {
                        src_rect.width = art_frame_data.width;
                        src_rect.height = art_frame_data.height;

                        dst_rect.x = text_rects[inventory_location - 1000].x;
                        dst_rect.y = text_rects[inventory_location - 1000].y;

                        if (weapon_too_heavy) {
                            mes_file_entry1.num = 8;
                            mes_get_msg(inven_ui_mes_file, &mes_file_entry1);
                            sprintf(str, "%s: %d", mes_file_entry1.str, weapon_min_str);
                            tig_font_push(dword_682C74);
                            font_desc.str = str;
                            font_desc.width = 0;
                            sub_535390(&font_desc);
                            dst_rect.y += dst_rect.height - font_desc.height;
                            dst_rect.height = font_desc.height;
                            tig_window_text_write(inven_ui_window_handle, str, &dst_rect);
                            tig_font_pop();
                        }

                        v1[inventory_location - 1000] = true;
                    }
                }

                continue;
            }

            // 0x577F41
            if (!dword_681514) {
                int x;
                int y;
                int width;
                int height;

                x = v112 + 32 * (inventory_location % 10);
                y = v102 + 32 * (inventory_location / 10 - dword_681508);

                if (y < v109 + v102) {
                    art_blit_info.art_id = obj_field_int32_get(item_obj, OBJ_F_ITEM_INV_AID);
                    if (tig_art_item_id_destroyed_get(obj_field_int32_get(item_obj, OBJ_F_CURRENT_AID))) {
                        art_blit_info.art_id = tig_art_item_id_destroyed_set(art_blit_info.art_id, 0);
                        tig_art_anim_data(art_blit_info.art_id, &art_anim_data);

                        palette_modify_info.dst_palette = tig_palette_create();
                        palette_modify_info.src_palette = art_anim_data.palette1;
                        palette_modify_info.flags = TIG_PALETTE_MODIFY_TINT;
                        palette_modify_info.tint_color = tig_color_make(255, 0, 0);
                        tig_palette_modify(&palette_modify_info);

                        art_blit_info.flags = TIG_ART_BLT_PALETTE_OVERRIDE;
                        art_blit_info.palette = palette_modify_info.dst_palette;
                    } else {
                        palette_modify_info.dst_palette = NULL;
                    }

                    if (tig_art_frame_data(art_blit_info.art_id, &art_frame_data) == TIG_OK) {
                        if (y + art_frame_data.height > v102) {
                            src_rect.x = 0;
                            src_rect.y = 0;
                            src_rect.width = art_frame_data.width;
                            src_rect.height = art_frame_data.height;

                            item_inv_icon_size(item_obj, &width, &height);
                            x += (32 * width - art_frame_data.width) / 2;
                            y += (32 * height - art_frame_data.height) / 2;

                            if (y < v102) {
                                height = y - v102 + src_rect.height;
                                src_rect.y += v102 - y;
                                src_rect.height = height;
                                y = v102;
                            } else {
                                height = src_rect.height;
                            }

                            if (y + height > v102 + v109) {
                                height = v102 + v109 - y;
                                src_rect.height = height;
                            }

                            dst_rect.x = x;
                            dst_rect.y = y;
                            dst_rect.width = src_rect.width;
                            dst_rect.height = height;

                            tig_window_blit_art(inven_ui_window_handle, &art_blit_info);
                        }
                    }

                    if (palette_modify_info.dst_palette != NULL) {
                        tig_palette_destroy(palette_modify_info.dst_palette);
                    }
                }
            }
        }

        art_blit_info.flags = 0;
        if (dword_681514) {
            for (index = 0; index < 9; index++) {
                if (!v1[index]) {
                    tig_art_interface_id_create(dword_5CAF58[index], 0, 0, 0, &(art_blit_info.art_id));

                    dst_rect.x = text_rects[index].x;
                    dst_rect.y = text_rects[index].y;
                    dst_rect.width = text_rects[index].width;
                    dst_rect.height = text_rects[index].height;

                    src_rect.width = stru_5CAC58[index].width;
                    src_rect.height = stru_5CAC58[index].height;

                    tig_window_blit_art(inven_ui_window_handle, &art_blit_info);
                }
            }
        }
    }

    sub_551080();
    sub_5806F0(stru_6810F0);
}

// 0x5782D0
tig_art_id_t sub_5782D0()
{
    tig_art_id_t art_id;
    int num;

    if (dword_683464 == 0 || dword_683464 == 5 || dword_683464 == 6) {
        num = 223;
    } else if (dword_683464 == 1) {
        num = dword_681100 != -1 ? 825 : 235;
    } else {
        num = 234;
    }

    tig_art_interface_id_create(num, 0, 0, 0, &art_id);

    return art_id;
}

// 0x578330
void sub_578330(int64_t a1, int64_t a2)
{
    int gold;
    int pos;

    if (dword_6810FC) {
        dword_681440 = 0;
        return;
    }

    if (a2 == qword_6814F8) {
        dword_681440 = sub_461F80(a1, qword_6814F8, qword_682C78, 0);
        if (dword_681440 != 0) {
            gold = item_gold_get(qword_682C78);
            if (sub_441980(a1, qword_682C78, qword_6814F8, SAP_BUY_OBJECT, 0)) {
                pos = 0;
            } else {
                sub_4137F0(qword_682C78, qword_6814F8, byte_68241C);
                pos = (int)strlen(byte_68241C);
            }

            if (dword_681440 > gold) {
                sub_413880(qword_682C78, qword_6814F8, byte_682804);
                sprintf(&(byte_68241C[pos]), byte_682804, dword_681440, gold);
            } else {
                sub_413640(qword_682C78, qword_6814F8, byte_682804);
                sprintf(&(byte_68241C[pos]), byte_682804, dword_681440);
            }
        } else {
            if ((obj_field_int32_get(a1, OBJ_F_ITEM_FLAGS) & OIF_STOLEN) == 0) {
                sub_4136D0(qword_682C78, qword_6814F8, byte_68241C);
            } else {
                sub_413760(qword_682C78, qword_6814F8, byte_68241C);
            }
        }
    } else {
        dword_681440 = sub_461F80(a1, qword_682C78, qword_6814F8, 0);
        if (dword_681440 != 0) {
            if ((obj_field_int32_get(a1, OBJ_F_ITEM_FLAGS) & OIF_WONT_SELL) != 0
                || (item_inventory_location_get(a1) >= 1000
                    && item_inventory_location_get(a1) <= 1008)) {
                sub_4135B0(qword_682C78, qword_6814F8, byte_68241C);
                pos = strlen(byte_68241C);
            } else {
                pos = 0;
            }

            sub_413490(qword_682C78, qword_6814F8, byte_682804);
            sprintf(&(byte_68241C[pos]), byte_682804, dword_681440);

            if (basic_skill_get_training(qword_6814F8, BASIC_SKILL_HAGGLE) >= TRAINING_APPRENTICE) {
                int worth;
                int discount;

                worth = item_worth(a1);
                discount = 100 * (dword_681440 - worth) / worth;
                if (discount == 0) {
                    discount = 1;
                }

                sprintf(byte_682804, "(%d%%)", discount);
                strcat(byte_68241C, byte_682804);
            }
        } else {
            sub_413520(qword_682C78, qword_6814F8, byte_68241C);
        }
    }

    redraw_inven(false);
}

// 0x5786C0
void sub_5786C0(int64_t obj)
{
    if (item_is_identified(obj)) {
        sub_414540(qword_682C78, qword_6814F8, byte_68241C);
    } else {
        dword_681440 = sub_4B1740(9);
        sub_4145D0(qword_682C78, qword_6814F8, byte_682804);
        sprintf(byte_68241C, byte_682804, dword_681440);
    }
    redraw_inven(false);
}

// 0x578760
void sub_578760(int64_t obj)
{
    int v1;
    int v2;
    tig_art_id_t art_id;

    v1 = sub_43D5A0(obj);
    art_id = obj_field_int32_get(obj, OBJ_F_CURRENT_AID);
    if (tig_art_item_id_destroyed_get(art_id) != 0) {
        if (tech_skill_get_training(qword_682C78, TECH_SKILL_REPAIR) != TRAINING_MASTER) {
            sub_414660(qword_682C78, qword_6814F8, byte_68241C);
            redraw_inven(false);
            return;
        }

        v2 = v1;
    } else {
        v2 = object_get_hp_damage(obj);
    }

    if (v2 != 0) {
        dword_681440 = v1 * (sub_461F80(obj, qword_6814F8, qword_682C78, 1) / 2) / v1;
        if (dword_681440 < 2) {
            dword_681440 = 2;
        }

        sub_414780(qword_682C78, qword_6814F8, byte_682804);
        sprintf(byte_68241C, byte_682804, dword_681440);
        redraw_inven(false);
    } else {
        sub_4146F0(qword_682C78, qword_6814F8, byte_68241C);
        redraw_inven(false);
    }
}

// 0x5788C0
void sub_5788C0(int64_t item_obj, int64_t target_obj, int new_inventory_location, int a4)
{
    int reason;
    int64_t parent_obj;
    int old_inventory_location;
    int sound_id;
    int obj_type;
    int qty_fld;
    int qty;

    sub_4A51C0(player_get_pc_obj(), item_obj);

    if (target_obj == qword_681450) {
        reason = sub_466DA0(item_obj);
        if (reason == 0) {
            if (!item_parent(item_obj, &parent_obj)) {
                return;
            }

            old_inventory_location = item_inventory_location_get(item_obj);
            if (new_inventory_location < 1000 || new_inventory_location > 1008) {
                if (target_obj == qword_6814F8) {
                    sub_466310(item_obj, old_inventory_location, dword_68111C, 0);
                    if (!sub_466390(item_obj, target_obj, new_inventory_location, dword_68111C)) {
                        return;
                    }
                } else {
                    sub_466310(item_obj, old_inventory_location, dword_681518, 0);
                    if (!sub_466390(item_obj, target_obj, new_inventory_location, dword_681518)) {
                        return;
                    }
                }
            }

            sub_45F920();

            item_remove(item_obj);

            if (sub_466510(item_obj, target_obj, NULL)) {
                item_insert(item_obj, parent_obj, old_inventory_location);
            } else {
                item_insert(item_obj, target_obj, new_inventory_location);
            }
            sub_45F910();
        } else {
            if (item_parent(item_obj, &parent_obj)) {
                sub_4673F0(item_obj, reason);
            }
        }

        sound_id = sub_4F0BF0(item_obj, target_obj, OBJ_HANDLE_NULL, 1);
        gsound_play_sfx_id(sound_id, 1);
    } else {
        obj_type = obj_field_int32_get(item_obj, OBJ_F_TYPE);
        qword_739F70 = qword_681450;
        dword_739F80 = new_inventory_location;
        qword_739F78 = target_obj;
        qword_739F68 = item_obj;

        if ((a4 & 0x6) != 0) {
            dword_681440 = -1;
        }

        dword_739F84 = dword_681440;
        dword_739F60 = a4;

        if ((obj_type == OBJ_TYPE_ITEM_GOLD || obj_type == OBJ_TYPE_AMMO)) {
            sub_575770();

            sub_462410(item_obj, &qty_fld);
            qty = obj_field_int32_get(item_obj, qty_fld);
            if (qty != 1) {
                if (dword_683464 == 2
                    && (a4 & 0x01) != 0
                    && target_obj == qword_6814F8
                    || (a4 & 0x20) != 0) {
                    sub_578B80(qty);
                } else {
                    sub_551A80(13);
                    sub_5506C0(9);
                    sub_553990();
                }

                return;
            }
        }

        sub_578B80(1);
    }
}

// 0x578B80
void sub_578B80(int a1)
{
    Packet81* pkt;

    pkt = (Packet81*)MALLOC(sizeof(*pkt));
    pkt->type = 81;
    pkt->field_5C = dword_739F84;
    pkt->field_8 = dword_739F60;

    if (qword_739F70 != OBJ_HANDLE_NULL) {
        pkt->field_28 = sub_407EF0(qword_739F70);
    } else {
        pkt->field_28.type = OID_TYPE_NULL;
    }

    pkt->field_58 = dword_739F80;

    if (qword_739F68 != OBJ_HANDLE_NULL) {
        pkt->field_10 = sub_407EF0(qword_739F68);
    } else {
        pkt->field_10.type = OID_TYPE_NULL;
    }

    if (qword_739F78 != OBJ_HANDLE_NULL) {
        pkt->field_40 = sub_407EF0(qword_739F78);
    } else {
        pkt->field_40.type = OID_TYPE_NULL;
    }

    if (qword_682C78 != OBJ_HANDLE_NULL) {
        pkt->field_70 = sub_407EF0(qword_682C78);
    } else {
        pkt->field_70.type = OID_TYPE_NULL;
    }

    if (qword_6813A8 != OBJ_HANDLE_NULL) {
        pkt->field_88 = sub_407EF0(qword_6813A8);
    } else {
        pkt->field_88.type = OID_TYPE_NULL;
    }

    if (qword_6814F8 != OBJ_HANDLE_NULL) {
        pkt->field_A0 = sub_407EF0(qword_6814F8);
    } else {
        pkt->field_A0.type = OID_TYPE_NULL;
    }

    pkt->field_60 = a1;
    pkt->field_68 = sub_529520();

    if ((tig_net_flags & TIG_NET_CONNECTED) != 0) {
        sub_441B60(objp_perm_lookup(pkt->field_10),
            objp_perm_lookup(pkt->field_10),
            byte_682BEC);
        objid_id_to_str(byte_6812FC, pkt->field_10);

        if (pkt->field_40.type != OID_TYPE_NULL) {
            sub_441B60(objp_perm_lookup(pkt->field_40),
                objp_perm_lookup(pkt->field_40),
                byte_6813C0);
        } else {
            strcpy(byte_6813C0, "nobody");
        }

        if (pkt->field_28.type != OID_TYPE_NULL) {
            sub_441B60(objp_perm_lookup(pkt->field_28),
                objp_perm_lookup(pkt->field_28),
                byte_681468);
        } else {
            strcpy(byte_681468, "nobody");
        }

        if (pkt->field_60 > 1) {
            tig_debug_printf("UI: InvenItemTransfer - %d %ss(%s) going from %s to %s\n",
                pkt->field_60,
                byte_682BEC,
                byte_6812FC,
                byte_681468,
                byte_6813C0);
        } else {
            tig_debug_printf("UI: InvenItemTransfer - %s(%s) going from %s to %s\n",
                byte_682BEC,
                byte_6812FC,
                byte_681468,
                byte_6813C0);
        }
    }

    sub_578EA0(pkt);
}

// 0x578EA0
bool sub_578EA0(Packet81* pkt)
{
    // TODO: Incomplete.
}

// 0x579760
int64_t sub_579760()
{
    return qword_739F68;
}

// 0x579770
void sub_579770(int64_t from_obj, int64_t to_obj)
{
    int64_t* items;
    int cnt;
    int idx;
    int inventory_location;

    qword_681450 = from_obj;

    cnt = item_list_get(from_obj, &items);
    for (idx = 0; idx < cnt; idx++) {
        if ((obj_field_int32_get(items[idx], OBJ_F_FLAGS) & OF_OFF) == 0
            && (obj_field_int32_get(items[idx], OBJ_F_ITEM_FLAGS) & OIF_NO_DISPLAY) == 0
            && !sub_466510(items[idx], to_obj, &inventory_location)) {
            sub_5788C0(items[idx], to_obj, inventory_location, 0x21);
        }
    }

    item_list_free(items);
}

// 0x579840
bool sub_579840(int64_t obj, bool a2)
{
    int v1;
    bool v2 = false;
    int training;
    size_t pos;

    if (qword_681450 == qword_6814F8) {
        if (!a2) {
            sub_414020(qword_682C78, qword_6814F8, 2, byte_68241C);
        }
        return false;
    }

    if (basic_skill_get_base(qword_682C78, BASIC_SKILL_GAMBLING) == 0) {
        if (!a2) {
            sub_414020(qword_682C78, qword_6814F8, 9, byte_68241C);
        }
        return false;
    }

    v1 = sub_461F80(obj, qword_682C78, qword_6814F8, 0);
    if (v1 > sub_4C6520(qword_6814F8)) {
        if (!a2) {
            sub_414020(qword_682C78, qword_6814F8, 3, byte_68241C);
        }
        return false;
    }

    if (v1 > item_gold_get(qword_6814F8)) {
        if (!a2) {
            sub_414020(qword_682C78, qword_6814F8, 4, byte_682804);
            sprintf(byte_68241C, byte_682804, v1);
        }
        return false;
    }

    training = basic_skill_get_training(qword_682C78, BASIC_SKILL_GAMBLING);
    if (item_inventory_location_get(obj) >= 1000 && item_inventory_location_get(obj) <= 1008) {
        if (training < TRAINING_EXPERT) {
            if (!a2) {
                sub_414020(qword_682C78, qword_6814F8, 5, byte_68241C);
            }
            return false;
        }

        v2 = true;
    }

    if ((obj_field_int32_get(obj, OBJ_F_ITEM_FLAGS) & OIF_WONT_SELL) != 0) {
        if (training < TRAINING_MASTER) {
            if (!a2) {
                sub_414020(qword_682C78, qword_6814F8, 6, byte_68241C);
            }
            return false;
        }

        v2 = true;
    }

    if (!a2) {
        if (v2) {
            sub_414020(qword_682C78, qword_6814F8, 7, byte_68241C);
            strcat(byte_68241C, " ");
            pos = strlen(byte_68241C);
        } else {
            pos = 0;
        }

        sub_414020(qword_682C78, qword_6814F8, 8, byte_682804);
        sprintf(&(byte_68241C[pos]), byte_682804, v1);
    }

    return true;
}

// 0x579B60
void sub_579B60(int64_t obj)
{
    Tanya v1;

    if (sub_579840(obj, 1)) {
        sub_4C7090(&v1);
        sub_4440E0(qword_6814F8, &(v1.field_0));
        sub_4440E0(qword_682C78, &(v1.field_30));
        sub_4440E0(obj, &(v1.field_68));
        v1.field_9C = 8;
        v1.field_A0 = 0;
        if (sub_4C7160(&v1)) {
            // FIXME: Unclear flags.
            sub_5788C0(obj, qword_6814F8, 0, (v1.field_98 & 0x1) != 0 ? 0x12 : 0x11);
            dword_681440 = -1;
        }
    }
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
    scrollbar_ui_control_create(&stru_6810F0, &info, inven_ui_window_handle);
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
    redraw_inven(false);
}

// 0x579E30
void sub_579E30(TigRect* rect)
{
    TigArtBlitInfo blit_info;

    if (dword_683464 == 0
        || dword_683464 == 5
        || dword_683464 == 6) {
        return;
    }

    blit_info.flags = 0;
    blit_info.art_id = sub_5782D0();
    blit_info.src_rect = rect;
    blit_info.dst_rect = rect;
    tig_window_blit_art(inven_ui_window_handle, &blit_info);
}
