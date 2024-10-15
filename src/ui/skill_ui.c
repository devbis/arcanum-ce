#include "ui/skill_ui.h"

#include "game/anim.h"
#include "game/critter.h"
#include "game/dialog.h"
#include "game/mes.h"
#include "game/mp_utils.h"
#include "game/multiplayer.h"
#include "game/obj.h"
#include "game/player.h"
#include "game/script.h"
#include "game/skill.h"
#include "game/stat.h"
#include "game/target.h"
#include "game/text_floater.h"
#include "ui/charedit_ui.h"
#include "ui/dialog_ui.h"
#include "ui/intgame.h"
#include "ui/inven_ui.h"
#include "ui/types.h"

#define FOUR 4

static bool sub_57A5E0(int64_t obj);
static void sub_57A620(Tanya* a1);
static void sub_57A6B0(Tanya* a1);
static bool sub_57A710(int64_t a1, int64_t a2);
static bool sub_57A770(int64_t obj, int a2, int a3, bool success);
static bool sub_57A7F0(int64_t obj, int a2, int a3, int a4, int a5, bool a6);
static bool sub_57A8C0(int64_t obj, int a2, int a3, int a4, int a5, bool a6);
static bool sub_57A990(int64_t obj, int a2, int a3, bool success);
static bool skill_ui_trap(int64_t obj, int a2, int a3, bool success);
static bool sub_57AA90(int64_t obj, int a2, int a3, bool success);
static bool skill_ui_no_repair(int64_t obj);
static bool skill_ui_lock_pick(int64_t obj, int a2, int a3, bool success);
static bool skill_ui_no_lock(int64_t obj);

// 0x5CB220
static int dword_5CB220[FOUR] = {
    279,
    280,
    282,
    278,
};

// 0x5CB230
static int dword_5CB230[FOUR] = {
    279,
    280,
    282,
    278,
};

// 0x5CB240
static int dword_5CB240[FOUR] = {
    SKILL_PROWLING,
    SKILL_PICK_POCKET,
    SKILL_DISARM_TRAPS,
    SKILL_REPAIR,
};

// 0x5CB250
static uint64_t qword_5CB250[FOUR] = {
    Tgt_Self,
    Tgt_Obj_No_ST_Critter_Animal,
    Tgt_Tile | Tgt_Object,
    Tgt_Obj_Inven,
};

// 0x5CB270
static int dword_5CB270 = -1;

// 0x683478
static const char* dword_683478[FOUR];

// 0x683488
static mes_file_handle_t skill_ui_mes_file;

// 0x683490
static int64_t qword_683490;

// 0x579E80
bool skill_ui_init(GameInitInfo* init_info)
{
    int index;
    SkillCallbacks callbacks;
    MesFileEntry mes_file_entry;

    (void)init_info;

    callbacks.field_0 = sub_57A770;
    callbacks.field_4 = sub_57A7F0;
    callbacks.field_8 = sub_57A8C0;
    callbacks.field_C = sub_57A990;
    callbacks.trap_output_func = skill_ui_trap;
    callbacks.field_14 = sub_57AA90;
    callbacks.lock_no_repair = skill_ui_no_repair;
    callbacks.lock_pick_output_func = skill_ui_lock_pick;
    callbacks.no_lock_output_func = skill_ui_no_lock;
    skill_set_callbacks(&callbacks);

    if (!mes_load("mes\\skill_ui.mes", &skill_ui_mes_file)) {
        return false;
    }

    for (index = 0; index < FOUR; index++) {
        mes_file_entry.num = 100 + index;
        mes_get_msg(skill_ui_mes_file, &mes_file_entry);
        dword_683478[index] = mes_file_entry.str;
    }

    return true;
}

// 0x579F30
void skill_ui_reset()
{
    qword_683490 = 0;
}

// 0x579F40
void skill_ui_exit()
{
    mes_unload(skill_ui_mes_file);
}

// 0x579F50
int sub_579F50(int index)
{
    return index >= 0 && index < FOUR ? dword_5CB220[index] : dword_5CB220[0];
}

// 0x579F70
int sub_579F70(int index)
{
    return index >= 0 && index < FOUR ? dword_5CB230[index] : dword_5CB230[0];
}

// 0x579F90
int sub_579F90()
{
  return dword_5CB270;
}

// 0x579FA0
void sub_579FA0(int64_t obj, int type)
{
    int64_t pc_obj;
    Packet30 pkt;

    pc_obj = player_get_pc_obj();
    if (pc_obj != obj) {
        if (sub_45D8D0(pc_obj)
            || sub_45D800(pc_obj)) {
            return;
        }

        if (sub_45D8D0(obj)
            || sub_45D800(obj)) {
            return;
        }
    }

    if (sub_551A00() == 2) {
        sub_57A1A0();
    }

    qword_683490 = obj;
    dword_5CB270 = type;

    if ((qword_5CB250[type] & 0x2000000) != 0
        || sub_551A80(0)) {
        if (!sub_4A2BA0()) {
            pkt.type = 30;
            sub_4440E0(pc_obj, &(pkt.field_8));
            pkt.field_38 = type;
            tig_net_send_app_all(&pkt, sizeof(pkt));
            if ((tig_net_flags & TIG_NET_HOST) == 0) {
                return;
            }
        }

        skill_ui_preprocess(obj, type);
    }
}

// 0x57A0A0
void skill_ui_preprocess(int64_t obj, int type)
{
    bool is_pc;
    uint64_t tgt;
    Tanya v3;

    is_pc = player_is_pc_obj(obj);
    qword_683490 = obj;
    dword_5CB270 = type;

    tgt = qword_5CB250[type];
    sub_4C7090(&v3);

    switch (type) {
    case 0:
        sub_45EE30(obj, !critter_is_concealed(obj));
        break;
    case 1:
    case 2:
        break;
    case 3:
        if (!is_pc) {
            return;
        }

        if (!inven_ui_is_created()) {
            sub_572240(obj, OBJ_HANDLE_NULL, 0);
        }
        break;
    default:
        tig_debug_printf("skill_ui_preprocess: ERROR: type out of range!\n");
        return;
    }

    if (is_pc && tgt != Tgt_None && tgt != Tgt_Self) {
        if (sub_551A80(2)) {
            sub_4F25B0(tgt);
        }
    }
}

// 0x57A1A0
void sub_57A1A0()
{
    sub_551A80(0);
    qword_683490 = 0;
}

// 0x57A1F0
void sub_57A1F0(S4F2810* a1)
{
    int64_t pc_obj;
    Packet31 pkt;

    pc_obj = player_get_pc_obj();

    if (!sub_4A2BA0()) {
        pkt.type = 31;
        sub_4440E0(qword_683490, &(pkt.field_8));
        pkt.field_38 = dword_5CB270;
        pkt.field_40 = *a1;
        if (!a1->field_8) {
            pkt.field_50 = sub_407EF0(a1->field_0);
        }

        if ((tig_net_flags & TIG_NET_HOST) == 0) {
            return;
        }
    }

    if ((pc_obj == qword_683490 || !sub_45D800(pc_obj))
        && !sub_45D800(qword_683490)) {
        sub_57A320(a1, qword_683490, dword_5CB270);
    }
}

// 0x57A320
void sub_57A320(S4F2810 *a1, int64_t obj, int a3)
{
    bool is_pc;
    int skill;
    unsigned int spell_flags;
    MesFileEntry mes_file_entry;
    Tanya v2;

    if (sub_45D8D0(obj)) {
        return;
    }

    is_pc = player_is_pc_obj(obj);
    skill = sub_57A6C0(a3, a1);

    switch (skill) {
    case SKILL_PICK_POCKET:
        if (!a1->field_8
            && is_pc
            && obj_field_int32_get(obj, OBJ_F_TYPE) == OBJ_TYPE_PC) {
            sub_57A1A0();

            spell_flags = obj_field_int32_get(obj, OBJ_F_SPELL_FLAGS);

            if ((obj_field_int32_get(a1->field_0, OBJ_F_SPELL_FLAGS) & OSF_STONED) == 0) {
                if (sub_57A5E0(a1->field_0)) {
                    if (player_is_pc_obj(obj)) {
                        mes_file_entry.num = 525;
                        mes_get_msg(skill_ui_mes_file, &mes_file_entry);
                        sub_4D5450(obj, 0, mes_file_entry.str);
                    }
                } else if (sub_45D8D0(a1->field_0)) {
                    if (sub_441980(obj, a1->field_0, obj, SAP_USE, 0)
                        && (spell_flags & OSF_POLYMORPHED) == 0
                        && !sub_423300(a1->field_0, 0)) {
                        sub_4602D0(obj, a1->field_0);
                    }
                } else {
                    sub_57A710(obj, a1->field_0);
                }
            }
        }
        return;
    case SKILL_REPAIR:
        if (!a1->field_8) {
            sub_4C7090(&v2);
            sub_4440E0(obj, &(v2.field_0));
            sub_4440E0(a1->field_0, &(v2.field_30));
            sub_4440E0(OBJ_HANDLE_NULL, &(v2.field_68));
            v2.field_98 = 0;
            v2.field_9C = 12;
            sub_57A6B0(&v2);
        }
        return;
    case SKILL_DISARM_TRAPS:
        if (!a1->field_8 && sub_4B7CD0(obj, 4)) {
            sub_4C7090(&v2);
            sub_4440E0(obj, &(v2.field_0));
            sub_4440E0(a1->field_0, &(v2.field_30));
            sub_4440E0(OBJ_HANDLE_NULL, &(v2.field_68));
            v2.field_98 = 4;
            v2.field_9C = 15;
            sub_57A620(&v2);
            sub_57A6B0(&v2);
        }
        return;
    default:
        if (!a1->field_8) {
            sub_4C6F90(obj, skill, a1->field_0, 0);
        }
        return;
    }
}

// 0x57A5E0
bool sub_57A5E0(int64_t obj)
{
    return (obj_field_int32_get(obj, OBJ_F_CRITTER_FLAGS2) & OCF2_NO_PICKPOCKET) != 0
        || (obj_field_int32_get(obj, OBJ_F_CRITTER_FLAGS) & OCF_MECHANICAL) != 0;
}

// 0x57A620
void sub_57A620(Tanya* a1)
{
    int64_t obj;
    char str[1000];

    if (!tig_kb_is_key_pressed(DIK_LCONTROL) && !tig_kb_is_key_pressed(DIK_RCONTROL)) {
        obj = a1->field_0.obj;
        sub_4C9050(a1);
        if (obj != a1->field_0.obj) {
            sub_4139A0(a1->field_0.obj, obj, str);
            sub_568430(a1->field_0.obj, obj, str, 0);
        }
    }
}

// 0x57A6A0
int sub_57A6A0(int index)
{
    return dword_5CB240[index];
}

// 0x57A6B0
void sub_57A6B0(Tanya* a1)
{
    sub_4C7160(a1);
}

// 0x57A6C0
int sub_57A6C0(int a1, S4F2810* a2)
{
    int v1;
    int complexity;

    v1 = dword_5CB240[a1];
    if (v1 == -1) {
        complexity = obj_field_int32_get(a2->field_0, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY);
        if (a1 == 3 && complexity >= 20) {
            v1 = SKILL_REPAIR;
        }
    }

    return v1;
}

// 0x57A700
const char* sub_57A700(int index)
{
    return dword_683478[index];
}

// 0x57A710
bool sub_57A710(int64_t a1, int64_t a2)
{
    int type;

    if (a1 != a2) {
        type = obj_field_int32_get(a2, OBJ_F_TYPE);
        sub_572240(a1, a2, type == OBJ_TYPE_PC || type == OBJ_TYPE_NPC ? 3 : 2);
    }

    return true;
}

// 0x57A770
bool sub_57A770(int64_t obj, int a2, int a3, bool success)
{
    MesFileEntry mes_file_entry;
    John v1;
    int font;

    if (success) {
        // You succeed.
        mes_file_entry.num = 500;
        font = 0;
    } else {
        // You fail.
        mes_file_entry.num = 501;
        font = 1;
    }

    mes_get_msg(skill_ui_mes_file, &mes_file_entry);

    v1.type = 6;
    v1.str = mes_file_entry.str;
    sub_550750(&v1);

    sub_4D5450(obj, font, mes_file_entry.str);

    return true;
}

// 0x57A7F0
bool sub_57A7F0(int64_t obj, int a2, int a3, int a4, int a5, bool a6)
{
    MesFileEntry mes_file_entry;
    John v1;
    bool v2;
    int client_id;

    if (a6) {
        // You succeed.
        mes_file_entry.num = 500;
        v2 = false;
    } else {
        // You fail.
        mes_file_entry.num = 501;
        v2 = true;
    }

    mes_get_msg(skill_ui_mes_file, &mes_file_entry);

    v1.type = 6;
    v1.str = mes_file_entry.str;

    if (player_is_pc_obj(obj)) {
        sub_550750(&v1);
    } else {
        client_id = sub_4A2B10(obj);
        if (client_id != -1) {
            sub_4EDA60(&v1, client_id, 0);
        }
    }

    sub_4D5450(obj, v2, mes_file_entry.str);

    if ((tig_net_flags & TIG_NET_CONNECTED) != 0) {
        sub_4EDB70(obj, v2, mes_file_entry.str);
    }

    return true;
}

// NOTE: Exactly the same implementation as above.
//
// 0x57A8C0
bool sub_57A8C0(int64_t obj, int a2, int a3, int a4, int a5, bool a6)
{
    MesFileEntry mes_file_entry;
    John v1;
    bool v2;
    int client_id;

    if (a6) {
        // You succeed.
        mes_file_entry.num = 500;
        v2 = false;
    } else {
        // You fail.
        mes_file_entry.num = 501;
        v2 = true;
    }

    mes_get_msg(skill_ui_mes_file, &mes_file_entry);

    v1.type = 6;
    v1.str = mes_file_entry.str;

    if (player_is_pc_obj(obj)) {
        sub_550750(&v1);
    } else {
        client_id = sub_4A2B10(obj);
        if (client_id != -1) {
            sub_4EDA60(&v1, client_id, 0);
        }
    }

    sub_4D5450(obj, v2, mes_file_entry.str);

    if ((tig_net_flags & TIG_NET_CONNECTED) != 0) {
        sub_4EDB70(obj, v2, mes_file_entry.str);
    }

    return true;
}

// 0x57A990
bool sub_57A990(int64_t obj, int a2, int a3, bool success)
{
    MesFileEntry mes_file_entry;
    John v1;
    int font;

    if (success) {
        // You succeed.
        mes_file_entry.num = 500;
        font = 0;
    } else {
        // You fail.
        mes_file_entry.num = 501;
        font = 1;
    }

    mes_get_msg(skill_ui_mes_file, &mes_file_entry);

    v1.type = 6;
    v1.str = mes_file_entry.str;
    sub_550750(&v1);

    sub_4D5450(obj, font, mes_file_entry.str);

    return true;
}

// 0x57AA10
bool skill_ui_trap(int64_t obj, int a2, int a3, bool success)
{
    MesFileEntry mes_file_entry;
    John v1;
    int font;

    if (success) {
        // Trap successfully disarmed.
        mes_file_entry.num = 560;
        font = 0;
    } else {
        // Trap disarm failed.
        mes_file_entry.num = 561;
        font = 1;
    }

    mes_get_msg(skill_ui_mes_file, &mes_file_entry);

    v1.type = 6;
    v1.str = mes_file_entry.str;
    sub_550750(&v1);

    sub_4D5450(obj, font, mes_file_entry.str);

    return true;
}

// 0x57AA90
bool sub_57AA90(int64_t obj, int a2, int a3, bool success)
{
    MesFileEntry mes_file_entry;
    John v1;
    int font;

    if (success) {
        // You succeed.
        mes_file_entry.num = 500;
        font = 0;
    } else {
        // You fail.
        mes_file_entry.num = 501;
        font = 1;
    }

    mes_get_msg(skill_ui_mes_file, &mes_file_entry);

    v1.type = 6;
    v1.str = mes_file_entry.str;
    sub_550750(&v1);

    sub_4D5450(obj, font, mes_file_entry.str);

    return true;
}

// 0x57AB10
bool skill_ui_no_repair(int64_t obj)
{
    MesFileEntry mes_file_entry;
    John v1;

    // That is not in need of repair.
    mes_file_entry.num = 505;
    mes_get_msg(skill_ui_mes_file, &mes_file_entry);

    v1.type = 6;
    v1.str = mes_file_entry.str;
    sub_550750(&v1);

    sub_4D5450(obj, 1, mes_file_entry.str);

    return true;
}

// 0x57AB70
bool skill_ui_lock_pick(int64_t obj, int a2, int a3, bool success)
{
    MesFileEntry mes_file_entry;
    John v1;
    int font;

    if (success) {
        // Lock successfully picked.
        mes_file_entry.num = 550;
        font = 0;
    } else {
        // Lock picking failed.
        mes_file_entry.num = 551;
        font = 1;
    }

    mes_get_msg(skill_ui_mes_file, &mes_file_entry);

    v1.type = 6;
    v1.str = mes_file_entry.str;
    sub_550750(&v1);

    sub_4D5450(obj, font, mes_file_entry.str);

    return true;
}

// 0x57ABF0
bool skill_ui_no_lock(int64_t obj)
{
    MesFileEntry mes_file_entry;
    John v1;

    // That has no lock.
    mes_file_entry.num = 512;
    mes_get_msg(skill_ui_mes_file, &mes_file_entry);

    v1.type = 6;
    v1.str = mes_file_entry.str;
    sub_550750(&v1);

    sub_4D5450(obj, 1, mes_file_entry.str);

    return true;
}

// 0x57AC50
void sub_57AC50(int64_t obj, int skill, int a3)
{
    switch (a3) {
    case 1:
        sub_57ACD0(obj, skill);
        break;
    case -1:
        sub_57AEB0(obj, skill);
        break;
    }
}

// 0x57AC90
void sub_57AC90(int64_t obj, int skill, int a3)
{
    if (IS_TECH_SKILL(skill)) {
        basic_skill_set_training(obj, GET_TECH_SKILL(skill), a3);
    } else {
        tech_skill_set_training(obj, GET_BASIC_SKILL(skill), a3);
    }
}

// 0x57ACD0
void sub_57ACD0(int64_t obj, int skill)
{
    bool is_pc;
    int base;
    int level;
    int cost;
    int points;

    is_pc = player_is_pc_obj(obj);
    if (IS_TECH_SKILL(skill)) {
        skill = GET_TECH_SKILL(skill);
        base = tech_skill_get_base(obj, skill);
        level = tech_skill_level(obj, skill);
        cost = sub_4C6AF0(obj, skill);
        points = stat_level(obj, STAT_UNSPENT_POINTS);
        if (cost > points) {
            if (is_pc && charedit_is_created()) {
                sub_55F160();
            }
            return;
        }

        if (tech_skill_set_base(obj, skill, base + cost) != base + cost) {
            if (level == 20) {
                if (is_pc && charedit_is_created()) {
                    sub_55F1E0();
                }
            } else {
                if (is_pc && charedit_is_created()) {
                    sub_55F200(tech_skill_get_stat(skill));
                }
            }
            return;
        }

        stat_set_base(obj, STAT_UNSPENT_POINTS, points - cost);
        if (is_pc && charedit_is_created()) {
            sub_55A230();
            sub_550720();
        }
    } else {
        skill = GET_BASIC_SKILL(skill);
        base = basic_skill_get_base(obj, skill);
        level = basic_skill_level(obj, skill);
        cost = sub_4C64B0(obj, skill);
        points = stat_level(obj, STAT_UNSPENT_POINTS);
        if (cost > points) {
            if (is_pc && charedit_is_created()) {
                sub_55F160();
            }
            return;
        }

        if (basic_skill_set_base(obj, skill, base + cost) != base + cost) {
            if (level == 20) {
                if (is_pc && charedit_is_created()) {
                    sub_55F1E0();
                }
            } else {
                if (is_pc && charedit_is_created()) {
                    sub_55F200(tech_skill_get_stat(skill));
                }
            }
            return;
        }

        stat_set_base(obj, STAT_UNSPENT_POINTS, points - cost);
        if (is_pc && charedit_is_created()) {
            sub_55A230();
            sub_550720();
        }
    }
}

// 0x57AEB0
void sub_57AEB0(int64_t obj, int skill)
{
    // TODO: Incomplete.
}
