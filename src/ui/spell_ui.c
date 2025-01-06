#include "ui/spell_ui.h"

#include "game/critter.h"
#include "game/item.h"
#include "game/magictech.h"
#include "game/mp_utils.h"
#include "game/mt_item.h"
#include "game/multiplayer.h"
#include "game/obj.h"
#include "game/player.h"
#include "game/spell.h"
#include "game/stat.h"
#include "game/target.h"
#include "game/ui.h"
#include "ui/charedit_ui.h"
#include "ui/hotkey_ui.h"
#include "ui/intgame.h"
#include "ui/inven_ui.h"

#define FIVE 5

typedef struct S5CB3A8 {
    int magictech;
    int field_4;
} S5CB3A8;

static void sub_57C470();

// 0x5CB3A0
static int dword_5CB3A0 = 10000;

// 0x5CB3A8
static S5CB3A8 stru_5CB3A8[FIVE] = {
    { -1, 0 },
    { -1, 0 },
    { -1, 0 },
    { -1, 0 },
    { -1, 0 },
};

// 0x5CB3D0
static int dword_5CB3D0 = -1;

// 0x6834F8
static int64_t qword_6834F8;

// 0x683500
static int64_t qword_683500;

// 0x683508
static bool dword_683508;

// 0x68350C
static bool dword_68350C;

// 0x57BB30
bool spell_ui_init(GameInitInfo* init_info)
{
    (void)init_info;

    return true;
}

// 0x57BB40
void spell_ui_reset()
{
    sub_57C470();
}

// 0x57BB50
void spell_ui_exit()
{
}

// 0x57BB60
bool spell_ui_save(TigFile* stream)
{
    int index;

    for (index = 0; index < FIVE; index++) {
        if (tig_file_fwrite(&(stru_5CB3A8[index].magictech), sizeof(stru_5CB3A8[index].magictech), 1, stream) != 1) return false;
        if (tig_file_fwrite(&(stru_5CB3A8[index].field_4), sizeof(stru_5CB3A8[index].field_4), 1, stream) != 1) return false;
    }

    return true;
}

// 0x57BBB0
bool spell_ui_load(GameLoadInfo* load_info)
{
    int index;

    for (index = 0; index < FIVE; index++) {
        if (tig_file_fread(&(stru_5CB3A8[index].magictech), sizeof(stru_5CB3A8[index].magictech), 1, load_info->stream) != 1) return false;
        if (tig_file_fread(&(stru_5CB3A8[index].field_4), sizeof(stru_5CB3A8[index].field_4), 1, load_info->stream) != 1) return false;

        if (stru_5CB3A8[index].field_4) {
            sub_553620(index, sub_458AE0(stru_5CB3A8[index].magictech));
        }
    }

    return true;
}

// 0x57BC30
void sub_57BC30()
{
    MesFileEntry mes_file_entry;
    UiMessage ui_message;

    mes_file_entry.num = 606;
    magictech_get_msg(&mes_file_entry);

    ui_message.type = UI_MSG_TYPE_FEEDBACK;
    ui_message.str = mes_file_entry.str;
    sub_460630(&ui_message);
}

// FIXME: Probably returns error code where 0 is success.
//
// 0x57BC70
bool sub_57BC70(int64_t obj, int spl)
{
    uint64_t* tgt_ptr;
    uint64_t tgt;
    bool v1 = false;
    S4F2810 v2;

    if (obj == OBJ_HANDLE_NULL) {
        return true;
    }

    dword_683508 = true;

    if (obj_type_is_item(obj_field_int32_get(obj, OBJ_F_TYPE))) {
        if (!item_parent(obj, &qword_683500)) {
            return true;
        }
    } else {
        qword_683500 = obj;
    }

    if (qword_683500 == OBJ_HANDLE_NULL) {
        return true;
    }

    if (!sub_40DA20(qword_683500)) {
        return true;
    }

    if ((obj_field_int32_get(qword_683500, OBJ_F_FLAGS) & OF_OFF) != 0) {
        return true;
    }

    if (critter_is_dead(qword_683500)) {
        return true;
    }

    if (critter_is_unconscious(qword_683500)) {
        return true;
    }

    if (sub_551A00() == 1) {
        sub_57BFF0();
    }

    if (!sub_459F60(spl)) {
        dword_683508 = false;
        return true;
    }

    dword_5CB3A0 = spl;
    qword_6834F8 = obj;

    sub_459F20(spl, &tgt_ptr);
    tgt = *tgt_ptr;

    if (sub_450370(dword_5CB3A0)
        && !tig_kb_is_key_pressed(DIK_LALT)
        && !tig_kb_is_key_pressed(DIK_RALT)) {
        // FIXME: Looks odd.
        tgt |= (Tgt_Non_Party_Critters & ~Tgt_Object);
    }

    if ((tgt & (Tgt_Obj_Inven & ~Tgt_Object)) != 0) {
        if (sub_551A00() == 4 || sub_551A00() == 11) {
            inven_ui_destroy();
        }

        if (inven_ui_is_created()) {
            v1 = true;
        } else {
            sub_572240(qword_6834F8, OBJ_HANDLE_NULL, 0);
        }
    } else {
        if (!sub_551A80(0)) {
            dword_683508 = false;
            return true;
        }
    }

    // TODO: Check, probably wrong.
    if (*tgt_ptr == Tgt_None
        || ((*tgt_ptr & Tgt_Self) != 0
            && (*tgt_ptr & Tgt_Tile) == 0)
        || *tgt_ptr == Tgt_Obj_Radius) {
        sub_4F27F0(&v2, obj_field_int64_get(qword_683500, OBJ_F_LOCATION));
        sub_57C110(&v2);
        dword_683508 = false;
        return true;
    }

    if (!sub_4B7AA0(qword_683500)) {
        dword_683508 = false;
        return false;
    }

    sub_4F25B0(tgt);
    if (sub_551A80(1)) {
        sub_4F25B0(tgt);
        dword_68350C = sub_500560();
        if (!dword_68350C) {
            sub_500570();
        }

        if ((*tgt_ptr & Tgt_Object) != 0
            && (tig_kb_is_key_pressed(DIK_LSHIFT)
                || tig_kb_is_key_pressed(DIK_RSHIFT))) {
            sub_4F2810(&v2, player_get_pc_obj());
            // FIXME: Odd.
            if ((*tgt_ptr & (Tgt_Obj_No_Self & ~Tgt_Object)) == 0) {
                sub_57C110(&v2);
                dword_683508 = false;
                return true;
            }
        }
    }

    if (v1) {
        if (!inven_ui_is_created()) {
            sub_572240(qword_6834F8, OBJ_HANDLE_NULL, 0);
        }
    }

    dword_683508 = false;
    return false;
}

// 0x57BFF0
void sub_57BFF0()
{
    if (!dword_68350C) {
        sub_500570();
    }

    sub_551A80(0);

    if (!dword_683508) {
        dword_5CB3A0 = 10000;
        qword_6834F8 = OBJ_HANDLE_NULL;
        qword_683500 = OBJ_HANDLE_NULL;
    }
}

// 0x57C040
void sub_57C040(int64_t obj, int index)
{
    int spl;

    spl = mt_item_spell(obj, index);
    if (!sub_45A030(spl) && spl != -1) {
        sub_57BC70(obj, spl);
    }
}

// 0x57C080
void sub_57C080(int64_t obj, int spl)
{
    if (!sub_45A030(spl) && spl != -1) {
        sub_57BC70(obj, spl);
    }
}

// 0x57C0B0
void sub_57C0B0()
{
    int64_t v1;

    if (sub_450370(dword_5CB3A0)) {
        v1 = sub_4F25D0();
        v1 &= ~0x1000000000000000;
        sub_4F25B0(v1);
    }
}

// 0x57C0E0
void sub_57C0E0()
{
    int64_t v1;

    if (sub_450370(dword_5CB3A0)) {
        v1 = sub_4F25D0();
        v1 |= 0x1000000000000000;
        sub_4F25B0(v1);
    }
}

// 0x57C110
void sub_57C110(S4F2810* a1)
{
    MesFileEntry mes_file_entry;
    UiMessage ui_message;
    MagicTechSerializedData v2;

    if (!critter_is_dead(qword_683500) && !critter_is_unconscious(qword_683500)) {
        if (obj_type_is_critter(obj_field_int32_get(qword_6834F8, OBJ_F_TYPE))
            && !magictech_can_charge_spell_fatigue(qword_683500, dword_5CB3A0)) {
            // Not enough Energy.
            mes_file_entry.num = 600;
            magictech_get_msg(&mes_file_entry);
            sub_460610(mes_file_entry.str);
        } else {
            sub_455A20(&v2, qword_6834F8, dword_5CB3A0);
            if (a1->is_loc) {
                v2.target_loc = a1->loc;
            } else {
                sub_4440E0(a1->obj, &(v2.target_obj));
            }

            if (sub_456BC0(&v2)) {
                sub_455AC0(&v2);
            } else {
                // You cannot see the target.
                mes_file_entry.num = 604;
                magictech_get_msg(&mes_file_entry);

                ui_message.type = UI_MSG_TYPE_FEEDBACK;
                ui_message.str = mes_file_entry.str;
                sub_460630(&ui_message);
            }
        }
    }
}

// 0x57C290
bool sub_57C290(int magictech)
{
    int candidate = -1;
    int64_t pc_obj;
    int cnt;
    int index;

    if (!sub_459FF0(magictech)) {
        return false;
    }

    pc_obj = player_get_pc_obj();
    cnt = stat_level(pc_obj, STAT_INTELLIGENCE) / 4;

    for (index = 0; index < cnt; index++) {
        if (stru_5CB3A8[index].field_4 == 1 && stru_5CB3A8[index].magictech == magictech) {
            return true;
        }

        if (stru_5CB3A8[index].magictech == -1 && candidate == -1) {
            candidate = index;
        }
    }

    if (candidate >= cnt) {
        return false;
    }

    stru_5CB3A8[candidate].magictech = magictech;
    stru_5CB3A8[candidate].field_4 = 1;
    sub_553620(candidate, sub_458AE0(magictech));

    return true;
}

// 0x57C320
void sub_57C320(int magictech)
{
    int index;

    if (magictech == -1) {
        return;
    }

    for (index = 0; index < FIVE; index++) {
        if (stru_5CB3A8[index].magictech == magictech
            && stru_5CB3A8[index].field_4 >= 1) {
            break;
        }
    }

    if (index < FIVE) {
        stru_5CB3A8[index].magictech = -1;
        stru_5CB3A8[index].field_4 = 0;
        sub_553620(index, TIG_ART_ID_INVALID);
    }
}

// 0x57C370
void sub_57C370(int index)
{
    Packet59 pkt;

    if (stru_5CB3A8[index].magictech != -1 && stru_5CB3A8[index].field_4 == 1) {
        stru_5CB3A8[index].field_4 = 2;

        if ((tig_net_flags & TIG_NET_CONNECTED) != 0) {
            pkt.type = 59;
            pkt.field_4 = index;
            tig_net_send_app_all(&pkt, sizeof(pkt));

            if ((tig_net_flags & TIG_NET_HOST) != 0) {
                sub_457110(stru_5CB3A8[index].magictech);
            }
        } else {
            sub_457110(stru_5CB3A8[index].magictech);
        }
    }
}

// 0x57C3F0
void sub_57C3F0(int index)
{
    MagicTechLock* lock;
    int64_t obj;

    // FIXME: Unused.
    player_get_pc_obj();

    if (stru_5CB3A8[index].field_4 == 1
        && sub_4557C0(stru_5CB3A8[index].magictech, &lock)) {
        obj = lock->target_obj.obj;
        if (obj == OBJ_HANDLE_NULL) {
            if (lock->summoned_obj != NULL) {
                obj = lock->summoned_obj->obj;
            }
            if (obj == OBJ_HANDLE_NULL) {
                obj = lock->parent_obj.obj;
            }
        }

        sub_5507E0(lock->spell);

        if (obj != OBJ_HANDLE_NULL) {
            sub_43C270(obj);
        }
    }
}

// 0x57C470
void sub_57C470()
{
    int index;

    for (index = 0; index < FIVE; index++) {
        stru_5CB3A8[index].magictech = -1;
        stru_5CB3A8[index].field_4 = 0;
        sub_553620(index, TIG_ART_ID_INVALID);
    }

    dword_5CB3D0 = -1;
}

// 0x57C4B0
void sub_57C4B0()
{
    int64_t obj;
    int v1;
    int v2;

    obj = player_get_pc_obj();
    if (obj != OBJ_HANDLE_NULL) {
        v1 = stat_level(obj, STAT_INTELLIGENCE) / 4;
        if (v1 != dword_5CB3D0) {
            v2 = 0;
            while (v2 < v1) {
                sub_553910(v2++, true);
            }
            while (v2 < FIVE) {
                sub_553910(v2++, false);
            }

            dword_5CB3D0 = v1;
        }
    }
}

// 0x57C520
bool sub_57C520(int index)
{
    return stru_5CB3A8[index].field_4 != 0;
}

// 0x57C540
void sub_57C540(int64_t obj, int index)
{
    int cost;
    int unspent_points;

    cost = sub_4B1650(index);
    unspent_points = stat_level(obj, STAT_UNSPENT_POINTS);

    if (cost > unspent_points) {
        sub_55F160();
        return;
    }

    if ((tig_net_flags & TIG_NET_CONNECTED) == 0 || multiplayer_is_locked()) {
        if (!spell_add(obj, index, false)) {
            if (spell_min_level(index) > stat_level(obj, STAT_LEVEL)) {
                sub_55F180();
                return;
            }

            if (spell_min_intelligence(index) > stat_level(obj, STAT_INTELLIGENCE)) {
                sub_55F1A0();
            } else {
                sub_55F200(6);
            }

            return;
        }
    } else {
        if ((tig_net_flags & TIG_NET_HOST) == 0) {
            spell_add(obj, index, false);
            return;
        }

        if (!spell_add(obj, index, false)) {
            if (spell_min_level(index) > stat_level(obj, STAT_LEVEL)) {
                sub_55F180();
                return;
            }

            if (spell_min_intelligence(index) > stat_level(obj, STAT_INTELLIGENCE)) {
                sub_55F1A0();
            } else {
                sub_55F200(6);
            }

            return;
        }
    }

    stat_set_base(obj, STAT_UNSPENT_POINTS, unspent_points - cost);
    if (charedit_is_created()) {
        sub_55A230();
        sub_550720();
    }
}

// 0x57C670
void spell_ui_remove(int64_t obj, int spell)
{
    int cost;
    int unspent_points;

    cost = sub_4B1650(spell);
    unspent_points = stat_level(obj, STAT_UNSPENT_POINTS);
    if (spell_remove(obj, spell)) {
        sub_57F340(spell);
        stat_set_base(obj, STAT_UNSPENT_POINTS, unspent_points + cost);

        if (charedit_is_created()) {
            sub_55A230();
            sub_550720();
        }
    }
}
