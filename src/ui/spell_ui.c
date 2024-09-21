#include "ui/spell_ui.h"

#include "game/magictech.h"
#include "game/mp_utils.h"
#include "game/mt_item.h"
#include "game/obj.h"
#include "game/player.h"
#include "game/spell.h"
#include "game/stat.h"
#include "ui/intgame.h"

#define FIVE 5

typedef struct S5CB3A8 {
    int field_0;
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
        if (tig_file_fwrite(&(stru_5CB3A8[index].field_0), sizeof(stru_5CB3A8[index].field_0), 1, stream) != 1) return false;
        if (tig_file_fwrite(&(stru_5CB3A8[index].field_4), sizeof(stru_5CB3A8[index].field_4), 1, stream) != 1) return false;
    }

    return true;
}

// 0x57BBB0
bool spell_ui_load(GameLoadInfo* load_info)
{
    int index;

    for (index = 0; index < FIVE; index++) {
        if (tig_file_fread(&(stru_5CB3A8[index].field_0), sizeof(stru_5CB3A8[index].field_0), 1, load_info->stream) != 1) return false;
        if (tig_file_fread(&(stru_5CB3A8[index].field_4), sizeof(stru_5CB3A8[index].field_4), 1, load_info->stream) != 1) return false;

        if (stru_5CB3A8[index].field_4) {
            sub_553620(index, sub_458AE0(stru_5CB3A8[index].field_0));
        }
    }

    return true;
}

// 0x57BC30
void sub_57BC30()
{
    MesFileEntry mes_file_entry;
    John v1;

    mes_file_entry.num = 606;
    sub_44FDC0(&mes_file_entry);

    v1.type = 6;
    v1.str = mes_file_entry.str;
    sub_460630(&v1);
}

// 0x57BC70
void sub_57BC70()
{
    // TODO: Incomplete.
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
void sub_57C110()
{
    // TODO: Incomplete.
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
        if (stru_5CB3A8[index].field_4 == 1 && stru_5CB3A8[index].field_0 == magictech) {
            return true;
        }

        if (stru_5CB3A8[index].field_0 == -1 && candidate == -1) {
            candidate = index;
        }
    }

    if (candidate >= cnt) {
        return false;
    }

    stru_5CB3A8[candidate].field_0 = magictech;
    stru_5CB3A8[candidate].field_4 = 1;
    sub_553620(candidate, sub_458AE0(magictech));

    return true;
}

// 0x57C320
void sub_57C320(int a1)
{
    int index;

    if (a1 == -1) {
        return;
    }

    for (index = 0; index < FIVE; index++) {
        if (stru_5CB3A8[index].field_0 == a1
            && stru_5CB3A8[index].field_4 >= 1) {
            break;
        }
    }

    if (index < FIVE) {
        stru_5CB3A8[index].field_0 = -1;
        stru_5CB3A8[index].field_4 = 0;
        sub_553620(index, -1);
    }
}

// 0x57C370
void sub_57C370(int index)
{
    Packet59 pkt;

    if (stru_5CB3A8[index].field_0 != -1 && stru_5CB3A8[index].field_4 == 1) {
        stru_5CB3A8[index].field_4 = 2;

        if ((tig_net_flags & TIG_NET_CONNECTED) != 0) {
            pkt.type = 59;
            pkt.field_4 = index;
            tig_net_send_app_all(&pkt, sizeof(pkt));

            if ((tig_net_flags & TIG_NET_HOST) != 0) {
                sub_457110(stru_5CB3A8[index].field_0);
            }
        } else {
            sub_457110(stru_5CB3A8[index].field_0);
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
        && sub_4557C0(stru_5CB3A8[index].field_0, &lock)) {
        obj = lock->target_obj;
        if (obj == OBJ_HANDLE_NULL) {
            if (lock->summoned_obj != NULL) {
                obj = *lock->summoned_obj;
            }
            if (obj == OBJ_HANDLE_NULL) {
                obj = lock->parent_obj;
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
        stru_5CB3A8[index].field_0 = -1;
        stru_5CB3A8[index].field_4 = 0;
        sub_553620(index, -1);
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

    if ((tig_net_flags & TIG_NET_CONNECTED) == 0 || sub_4A2BA0()) {
        if (!sub_4B1790(obj, index, 0)) {
            if (spell_get_minimum_level(index) > stat_level(obj, STAT_LEVEL)) {
                sub_55F180();
                return;
            }

            if (sub_4B1750(index) > stat_level(obj, STAT_INTELLIGENCE)) {
                sub_55F1A0();
            } else {
                sub_55F200(6);
            }

            return;
        }
    } else {
        if ((tig_net_flags & TIG_NET_HOST) == 0) {
            sub_4B1790(obj, index, 0);
            return;
        }

        if (!sub_4B1790(obj, index, 0)) {
            if (spell_get_minimum_level(index) > stat_level(obj, STAT_LEVEL)) {
                sub_55F180();
                return;
            }

            if (sub_4B1750(index) > stat_level(obj, STAT_INTELLIGENCE)) {
                sub_55F1A0();
            } else {
                sub_55F200(6);
            }

            return;
        }
    }

    stat_set_base(obj, STAT_UNSPENT_POINTS, unspent_points - cost);
    if (sub_55A220()) {
        sub_55A230();
        sub_550720();
    }
}

// 0x57C670
void sub_57C670(int64_t obj, int index)
{
    int cost;
    int unspent_points;

    cost = sub_4B1650(index);
    unspent_points = stat_level(obj, STAT_UNSPENT_POINTS);
    if (sub_4B19B0(obj, index)) {
        sub_57F340(index);
        stat_set_base(obj, STAT_UNSPENT_POINTS, unspent_points + cost);

        if (sub_55A220()) {
            sub_55A230();
            sub_550720();
        }
    }
}
