#include "ui/dialog_ui.h"

#include <stdio.h>

#include "game/ai.h"
#include "game/anim.h"
#include "game/broadcast.h"
#include "game/combat.h"
#include "game/critter.h"
#include "game/dialog.h"
#include "game/gsound.h"
#include "game/mp_utils.h"
#include "game/mp_utils.h"
#include "game/multiplayer.h"
#include "game/obj_private.h"
#include "game/player.h"
#include "game/script_name.h"
#include "game/script.h"
#include "game/stat.h"
#include "game/tb.h"
#include "ui/charedit_ui.h"
#include "ui/intgame.h"
#include "ui/inven_ui.h"
#include "ui/schematic_ui.h"
#include "ui/tb_ui.h"
#include "ui/wmap_ui.h"

#define MAX_ENTRIES 8

typedef struct DialogUiEntry {
    /* 0000 */ int field_0;
    /* 0004 */ int field_4;
    /* 0008 */ DialogEntryNode field_8;
    /* 1850 */ int field_1850;
    /* 1854 */ int field_1854;
    /* 1858 */ int field_1858;
    /* 185C */ char field_185C;
    /* 185D */ char field_185D;
    /* 185E */ char field_185E;
    /* 185F */ char field_185F;
} DialogUiEntry;

static DialogUiEntry* sub_567420(long long obj);
static void sub_5679C0(DialogUiEntry* entry);
static void sub_567D60(DialogUiEntry* entry);
static bool sub_567E30(DialogUiEntry* entry, int a2);
static bool sub_5680A0(TigMessage* msg);
static bool sub_5681B0(DialogUiEntry* entry);
static bool sub_568280(DialogUiEntry *a1);
static void sub_568480(DialogUiEntry* entry, int a2);
static void sub_5684C0(DialogUiEntry* entry);
static void sub_568540(int64_t a1, int64_t a2, int a3, int a4, const char* str, int a6);
static void sub_5686C0(int64_t a1, int64_t a2, int a3, int a4, const char* str);
static void sub_5688D0(int64_t a1, long long obj, int a4);
static void sub_5689B0();

// 0x66DAB8
static DialogUiEntry stru_66DAB8[8];

// 0x679DB8
static unsigned char byte_679DB8[8]; // boolean

// 0x67B960
static tig_sound_handle_t dword_67B960;

// 0x67B964
static bool dword_67B964;

// 0x679DC0
static struct {
    Packet44 pkt;
    char buffer[6000];
} stru_679DC0;

// 0x567330
bool dialog_ui_init(GameInitInfo* init_info)
{
    int index;

    (void)init_info;

    memset(byte_679DB8, 0, sizeof(byte_679DB8));

    for (index = 0; index < MAX_ENTRIES; index++) {
        stru_66DAB8[index].field_1850 = false;
        stru_66DAB8[index].field_0 = index;
    }

    sub_444990(sub_567460, sub_568430);
    sub_4A84D0(sub_5678D0, sub_568430);
    sub_4C2EA0(sub_568430);
    dword_67B960 = TIG_SOUND_HANDLE_INVALID;

    return true;
}

// 0x5673A0
void dialog_ui_exit()
{
    sub_5689B0();
}

// 0x5673B0
void dialog_ui_reset()
{
    int index;

    if (dword_67B964) {
        sub_553350();
        dword_67B964 = false;
    }

    for (index = 0; index < MAX_ENTRIES; index++) {
        if (stru_66DAB8[index].field_1850) {
            sub_5679C0(&(stru_66DAB8[index]));
        }
    }
}

// 0x567400
bool sub_567400(long long obj)
{
    return sub_567420(obj)->field_1850;
}

// 0x567420
DialogUiEntry* sub_567420(long long obj)
{
    int index = 0;

    if ((tig_net_flags & 0x1) != 0) {
        index = sub_4A2B10(obj);
    }

    return &(stru_66DAB8[index]);
}

// 0x567460
void sub_567460(int64_t a1, int64_t a2, int a3, int a4, int a5)
{
    DialogUiEntry* entry;
    char path[TIG_MAX_PATH];
    char str[2000];
    Packet44 pkt;

    if (sub_45D8D0(a1)) {
        return;
    }

    if (sub_45D800(a1)) {
        return;
    }

    if (sub_4AD800(a2, a1, 0) != 0) {
        return;
    }

    if (player_is_pc_obj(a1) && sub_551A00() == 3) {
        return;
    }

    if (combat_critter_is_combat_mode_active(a1)) {
        if (!sub_4B3D90(a1)) {
            return;
        }
        combat_critter_deactivate_combat_mode(a1);
    }

    entry = sub_567420(a1);
    if (sub_4A2BA0() || (tig_net_flags & TIG_NET_HOST) != 0) {
        if (a3 != 0 && script_name_build_dlg_name(a3, path)) {
            if (!sub_412E10(path, &(entry->field_4))) {
                return;
            }

            entry->field_8.field_0 = entry->field_4;
            entry->field_8.pc_obj = a1;
            entry->field_8.npc_obj = a2;
            entry->field_8.field_68 = a5;
            entry->field_8.field_6C = a3;
            if (!sub_412FD0(&(entry->field_8))) {
                sub_412F40(entry->field_4);
                return;
            }

            if (entry->field_8.field_17E8 == 4) {
                sub_568540(entry->field_8.npc_obj,
                    entry->field_8.pc_obj,
                    0,
                    -1,
                    entry->field_8.field_70,
                    entry->field_8.field_458);
                sub_413280(&(entry->field_8));
                sub_412F40(entry->field_4);
                return;
            }

            if (player_is_pc_obj(a1)) {
                if (!sub_553320(sub_5680A0)) {
                    sub_413280(&(entry->field_8));
                    sub_412F40(entry->field_4);
                    return;
                }

                dword_67B964 = true;

                if (!intgame_is_compact_interface()) {
                    sub_57CD60(a1, a2, str);
                    sub_553BE0(a1, a2, str);
                    sub_43C270(a2);
                }
            }

            sub_424070(a1, 3, 0, true);
            sub_433440(a1, sub_441B20(a1, a2));

            if (critter_is_concealed(a1)) {
                sub_45EE30(a1, false);
            }

            if (critter_is_concealed(a2)) {
                sub_45EE30(a2, false);
            }

            entry->field_1854 = a3;
            entry->field_1858 = a4;
            entry->field_1850 = 1;

            if ((tig_net_flags & TIG_NET_CONNECTED) != 0
                && (tig_net_flags & TIG_NET_HOST) != 0) {
                pkt.type = 44;
                pkt.subtype = 0;
                pkt.d.d.field_8 = sub_407EF0(a1);
                pkt.d.d.field_20 = sub_407EF0(a2);
                pkt.d.d.field_38 = a3;
                pkt.d.d.field_3C = a4;
                pkt.d.d.field_40 = a5;
                tig_net_send_app_all(&pkt, sizeof(pkt));
            }

            sub_5684C0(entry);
            sub_567D60(entry);
        } else {
            sub_5681C0(a1, a2);
        }
    } else {
        if (a3 != 0
            && script_name_build_dlg_name(a3, path)
            && player_is_pc_obj(a1)) {
            if (sub_553320(sub_5680A0)) {
                entry->field_8.field_68 = a5;
                entry->field_8.pc_obj = a1;
                entry->field_8.npc_obj = a2;
                entry->field_8.field_6C = a3;
                entry->field_1854 = a3;
                entry->field_1858 = a4;
                entry->field_1850 = 1;

                dword_67B964 = true;
            }
        }
    }
}

// 0x5678D0
void sub_5678D0(long long obj, int a2)
{
    DialogUiEntry* entry;
    Packet44 pkt;

    (void)a2;

    entry = sub_567420(obj);
    if (!entry->field_1850) {
        return;
    }

    entry->field_1850 = false;

    if (player_is_pc_obj(obj)) {
        sub_553350();
        dword_67B964 = 0;
    }

    if ((tig_net_flags & TIG_NET_CONNECTED) == 0
        || (tig_net_flags & TIG_NET_HOST) != 0) {
        sub_412F40(entry->field_4);
    }

    sub_4D6160(entry->field_8.npc_obj, -1);

    if ((tig_net_flags & TIG_NET_CONNECTED) == 0
        || (tig_net_flags & TIG_NET_HOST) != 0) {
        sub_413280(&(entry->field_8));
    }

    if ((tig_net_flags & TIG_NET_CONNECTED) != 0
        && (tig_net_flags & TIG_NET_HOST) != 0) {
        pkt.type = 44;
        pkt.subtype = 1;
        pkt.d.b.field_8 = sub_407EF0(obj);
        tig_net_send_app_all(&pkt, sizeof(pkt));
    }
}

// 0x5679C0
void sub_5679C0(DialogUiEntry* entry)
{
    if (!entry->field_1850) {
        return;
    }

    entry->field_1850 = false;

    if ((tig_net_flags & 0x1) == 0 || (tig_net_flags & 0x2) != 0) {
        sub_412F40(entry->field_4);
    }

    if ((tig_net_flags & 0x1) == 0 || (tig_net_flags & 0x2) != 0) {
        sub_413280(&(entry->field_8));
    }
}

// 0x567A10
int sub_567A10()
{
    return dword_67B964;
}

// 0x567A20
void sub_567A20(long long obj)
{
    sub_567420(obj)->field_1850 = false;
    if (player_is_pc_obj(obj)) {
        sub_553350();
        dword_67B964 = false;
    }
}

// 0x567A60
void sub_567A60(long long obj)
{
    sub_567420(obj)->field_1850 = true;
    if (player_is_pc_obj(obj)) {
        if (sub_553320(sub_5680A0)) {
            dword_67B964 = true;
        }
    }
}

// 0x567AB0
void sub_567AB0(DialogUiEntry* entry, DialogSerializedData* serialized_data, char* buffer)
{
    int index;
    int pos;

    if (entry->field_8.pc_obj != OBJ_HANDLE_NULL) {
        serialized_data->field_8 = sub_407EF0(entry->field_8.pc_obj);
    } else {
        serialized_data->field_8.type = OID_TYPE_NULL;
    }

    if (entry->field_8.npc_obj != OBJ_HANDLE_NULL) {
        serialized_data->field_20 = sub_407EF0(entry->field_8.npc_obj);
    } else {
        serialized_data->field_20.type = OID_TYPE_NULL;
    }

    serialized_data->field_3C = entry->field_8.field_6C;
    serialized_data->field_40 = 0;
    serialized_data->field_44 = (int)strlen(entry->field_8.field_70) + 1;
    strncpy(buffer, entry->field_8.field_70, serialized_data->field_44);
    serialized_data->field_78 = entry->field_8.field_17E8;
    serialized_data->field_7C = entry->field_8.field_17EC;

    pos = serialized_data->field_44;
    for (index = 0; index < 5; index++) {
        serialized_data->field_50[index] = pos;
        serialized_data->field_64[index] = (int)strlen(entry->field_8.field_460[index]) + 1;
        strncpy(&(buffer[pos]), entry->field_8.field_460[index], serialized_data->field_64[index]);
        serialized_data->field_80[index] = entry->field_8.field_17F0[index];
        serialized_data->field_94[index] = entry->field_8.field_1804[index];
        serialized_data->field_A8[index] = entry->field_8.field_1818[index];
    }

    serialized_data->field_BC = entry->field_8.field_1840;
    serialized_data->field_C0 = entry->field_8.field_1844;
}

// 0x567C30
void sub_567C30(DialogSerializedData* serialized_data, DialogUiEntry* entry, const char* buffer)
{
    int index;

    if (serialized_data->field_8.type != OID_TYPE_NULL) {
        entry->field_8.pc_obj = objp_perm_lookup(serialized_data->field_8);
    } else {
        entry->field_8.pc_obj = OBJ_HANDLE_NULL;
    }

    if (serialized_data->field_20.type != OID_TYPE_NULL) {
        entry->field_8.npc_obj = objp_perm_lookup(serialized_data->field_20);
    } else {
        entry->field_8.npc_obj = OBJ_HANDLE_NULL;
    }

    entry->field_8.field_6C = serialized_data->field_3C;
    entry->field_8.field_68 = serialized_data->field_38;
    strncpy(entry->field_8.field_70, &(buffer[serialized_data->field_40]), serialized_data->field_44);
    entry->field_8.field_45C = serialized_data->field_4C;
    entry->field_8.field_17E8 = serialized_data->field_78;
    entry->field_8.field_17EC = serialized_data->field_7C;

    for (index = 0; index < 5; index++) {
        strncpy(entry->field_8.field_460[index], &(buffer[serialized_data->field_50[index]]), serialized_data->field_64[index]);
        entry->field_8.field_17F0[index] = serialized_data->field_80[index];
        entry->field_8.field_1804[index] = serialized_data->field_94[index];
        entry->field_8.field_1818[index] = serialized_data->field_A8[index];
    }

    entry->field_8.field_1840 = serialized_data->field_BC;
    entry->field_8.field_1844 = serialized_data->field_C0;
}

// 0x567D60
void sub_567D60(DialogUiEntry* entry)
{
    int size;
    int index;

    if ((tig_net_flags & TIG_NET_CONNECTED) != 0
        && (tig_net_flags & TIG_NET_HOST) != 0) {
        stru_679DC0.pkt.type = 44;
        stru_679DC0.pkt.subtype = 3;
        stru_679DC0.pkt.d.e.field_8 = entry->field_0;
        stru_679DC0.pkt.d.e.field_C = entry->field_1850;
        stru_679DC0.pkt.d.e.field_10 = entry->field_1854;
        stru_679DC0.pkt.d.e.field_14 = entry->field_1858;
        sub_567AB0(entry, &(stru_679DC0.pkt.d.e.serialized_data), stru_679DC0.buffer);

        size = sizeof(stru_679DC0) + stru_679DC0.pkt.d.e.serialized_data.field_44;
        for (index = 0; index < 5; index++) {
            size += stru_679DC0.pkt.d.e.serialized_data.field_64[index];
        }

        tig_net_send_app_all(&stru_679DC0, size);
    }
}

// 0x567E00
void sub_567E00(int index, int a2)
{
    sub_567E30(&stru_66DAB8[index], a2);
}

// 0x567E30
bool sub_567E30(DialogUiEntry* entry, int a2)
{
    bool is_pc;

    is_pc = player_is_pc_obj(entry->field_8.pc_obj);
    sub_5686C0(entry->field_8.pc_obj, entry->field_8.npc_obj, 2, -1, entry->field_8.field_460[a2]);
    sub_4EF630(entry->field_8.npc_obj);
    sub_5689B0();
    sub_413130(&(entry->field_8), a2);
    sub_567D60(entry);

    switch (entry->field_8.field_17E8) {
    case 0:
        sub_5684C0(entry);
        break;
    case 1:
        sub_5678D0(entry->field_8.pc_obj, 0);
        sub_568480(entry, 0);
        break;
    case 2:
        sub_5678D0(entry->field_8.pc_obj, 0);
        sub_568480(entry, entry->field_8.field_17EC);
        break;
    case 3:
        if (is_pc) {
            sub_553370();
            sub_572240(entry->field_8.pc_obj, entry->field_8.npc_obj, 1);
        }
        if ((tig_net_flags & 0x1) != 0) {
            sub_5678D0(entry->field_8.pc_obj, 0);
        }
        break;
    case 4:
        sub_568540(entry->field_8.npc_obj, entry->field_8.pc_obj, 0, -1, entry->field_8.field_70, entry->field_8.field_458);
        sub_5678D0(entry->field_8.pc_obj, 0);
        break;
    case 5:
        if (is_pc) {
            sub_553370();
            charedit_create(entry->field_8.npc_obj, 2);
        }
        break;
    case 6:
        if (is_pc) {
            sub_553370();
            sub_560760();
        }
        break;
    case 7:
        if (is_pc) {
            sub_553370();
            sub_56D130(entry->field_8.npc_obj, entry->field_8.pc_obj);
        }
        break;
    case 8:
        if (is_pc) {
            sub_553370();
            sub_4EE550(entry->field_8.pc_obj, entry->field_8.npc_obj);
        }
        if ((tig_net_flags & 0x1) != 0) {
            sub_5678D0(entry->field_8.pc_obj, 0);
        }
        break;
    case 9:
        if (is_pc) {
            sub_553370();
            sub_572240(entry->field_8.pc_obj, entry->field_8.npc_obj, 6);
        }
        if ((tig_net_flags & 0x1) != 0) {
            sub_5678D0(entry->field_8.pc_obj, 0);
        }
        break;
    }

    return true;
}

// 0x5680A0
bool sub_5680A0(TigMessage* msg)
{
    DialogUiEntry* entry;
    int v1;
    int player;
    Packet44 pkt;

    entry = sub_567420(player_get_pc_obj());
    if (sub_5681B0(entry)) {
        return false;
    }

    v1 = sub_5533A0(msg);
    if (v1 == -1) {
        sub_5517A0(msg);
        return true;
    }

    if (sub_4A2BA0() || (tig_net_flags & TIG_NET_HOST) != 0) {
        if (!sub_567E30(entry, v1)) {
            sub_5517A0(msg);
        }
        return true;
    }

    player = sub_4A2B10(player_get_pc_obj());
    if (byte_679DB8[player] != 1) {
        byte_679DB8[player] = 1;

        pkt.type = 44;
        pkt.subtype = 2;
        pkt.d.f.field_8 = sub_407EF0(player_get_pc_obj());
        pkt.d.f.field_20 = entry->field_0;
        pkt.d.f.field_24 = v1;
        tig_net_send_app_all(&pkt, sizeof(pkt));
    }

    return true;
}

// 0x5681B0
bool sub_5681B0(DialogUiEntry* entry)
{
    (void)entry;

    return false;
}

// 0x5681C0
void sub_5681C0(long long a1, long long a2)
{
    char text[1000];

    sub_4132A0(a2, a1, text);
    sub_568540(a2, a1, 0, -1, text, -1);
}

// 0x568220
void sub_568220(DialogSerializedData* serialized_data, int a2, int a3, int a4, int a5, char* buffer)
{
    DialogUiEntry* entry;

    entry = &(stru_66DAB8[a2]);
    entry->field_1850 = a3;
    entry->field_1854 = a4;
    entry->field_1858 = a5;
    sub_567C30(serialized_data, entry, buffer);
    entry->field_0 = a2;
    sub_568280(entry);
}

// 0x568280
bool sub_568280(DialogUiEntry *a1)
{
    bool is_pc;

    is_pc = player_is_pc_obj(a1->field_8.pc_obj);

    if ((tig_net_flags & TIG_NET_CONNECTED) != 0 && (tig_net_flags & TIG_NET_HOST) == 0) {
        byte_679DB8[sub_4A2B10(player_get_pc_obj())] = 0;
    }

    switch (a1->field_8.field_17E8) {
    case 0:
        sub_5684C0(a1);
        break;
    case 1:
    case 2:
        sub_5678D0(a1->field_8.pc_obj, 0);
        break;
    case 3:
        if (is_pc) {
            sub_553370();
            sub_572240(a1->field_8.pc_obj, a1->field_8.npc_obj, 1);
        }
        break;
    case 5:
        if (is_pc) {
            sub_553370();
            charedit_create(a1->field_8.npc_obj, 2);
        }
        break;
    case 6:
        if (is_pc) {
            sub_553370();
            sub_560760();
        }
        break;
    case 7:
        if (is_pc) {
            sub_553370();
            sub_56D130(a1->field_8.npc_obj, a1->field_8.pc_obj);
        }
        break;
    case 8:
        if (is_pc) {
            sub_553370();
            sub_4EE550(a1->field_8.pc_obj, a1->field_8.npc_obj);
        }
        break;
    case 9:
        if (is_pc) {
            sub_553370();
            sub_572240(a1->field_8.pc_obj, a1->field_8.npc_obj, 6);
        }
        break;
    }

    return true;
}

// 0x568430
void sub_568430(int64_t a1, int64_t a2, const char* a3, int a4)
{
    int font;

    // TODO: Unclear.
    font = obj_field_int32_get(a1, OBJ_F_TYPE) == OBJ_TYPE_PC ? 0 : 2;
    sub_568540(a1, a2, font, -1, a3, a4);
}

// 0x568480
void sub_568480(DialogUiEntry* entry, int a2)
{
    if (a2 == 0) {
        a2 = entry->field_1858 + 1;
    }

    sub_441980(entry->field_8.pc_obj, entry->field_8.npc_obj, OBJ_HANDLE_NULL, SAP_DIALOG, a2);
}

// 0x5684C0
void sub_5684C0(DialogUiEntry* entry)
{
    int index;

    sub_568540(entry->field_8.npc_obj, entry->field_8.pc_obj, 0, -2, entry->field_8.field_70, entry->field_8.field_458);

    if (player_is_pc_obj(entry->field_8.pc_obj)) {
        sub_553370();

        for (index = 0; index < entry->field_8.field_45C; index++) {
            sub_553380(index, entry->field_8.field_460[index]);
        }
    }
}

// 0x568540
void sub_568540(int64_t obj, int64_t a2, int type, int a4, const char* str, int a6)
{
    Packet44 pkt;

    if (!sub_4A2BA0()) {
        if ((tig_net_flags & TIG_NET_HOST) == 0) {
            return;
        }

        pkt.type = 44;
        pkt.subtype = 4;
        pkt.d.d.field_8 = sub_407EF0(obj);
        pkt.d.d.field_20 = sub_407EF0(a2);
        pkt.d.d.field_38 = type;
        pkt.d.d.field_3C = a4;
        pkt.d.d.field_40 = 0;
        strncpy(pkt.d.d.field_44, str, 1000);
        tig_net_send_app_all(&pkt, sizeof(pkt));
    }

    if (a2 != OBJ_HANDLE_NULL
        && !sub_45D8D0(obj)
        && !sub_423300(obj, 0)) {
        sub_424070(obj, 3, 0, 1);
        sub_433440(obj, sub_441B20(obj, a2));
    }

    sub_4D62B0(obj);
    tb_add(obj, type, str);
    sub_4D6160(obj, a4);
    sub_5688D0(obj, a2, a6);
}

// 0x5686C0
void sub_5686C0(int64_t obj, int64_t a2, int type, int a4, const char* str)
{
    Packet44 pkt;

    if (!sub_4A2BA0()) {
        if ((tig_net_flags & TIG_NET_HOST) == 0) {
            return;
        }

        pkt.type = 44;
        pkt.subtype = 4;
        pkt.d.d.field_8 = sub_407EF0(obj);
        pkt.d.d.field_20 = sub_407EF0(a2);
        pkt.d.d.field_38 = type;
        pkt.d.d.field_3C = a4;
        pkt.d.d.field_40 = 1;
        strncpy(pkt.d.d.field_44, str, 1000);
        tig_net_send_app_all(&pkt, sizeof(pkt));
    }

    if (a2 != OBJ_HANDLE_NULL
        && !sub_45D8D0(obj)
        && !sub_423300(obj, 0)) {
        sub_424070(obj, 3, 0, 1);
        sub_433440(obj, sub_441B20(obj, a2));
    }

    if (!player_is_pc_obj(obj)) {
        sub_4D62B0(obj);
        tb_add(obj, type, str);
        sub_4D6160(obj, a4);
    }
}

// 0x568830
void sub_568830(int64_t obj)
{
    int index;

    for (index = 0; index < 8; index++) {
        if (stru_66DAB8[index].field_8.pc_obj == obj
            || stru_66DAB8[index].field_8.npc_obj == obj) {
            sub_5678D0(obj, 0);
        }
    }
}

// 0x568880
void sub_568880(long long obj, int a2, int a3, int type, int a5, int a6, const char* str)
{
    (void)a2;
    (void)a3;

    if ((a6 & 1) != 0) {
        if (player_is_pc_obj(obj)) {
            return;
        }
    }

    sub_4D62B0(obj);
    tb_add(obj, type, str);
    sub_4D6160(obj, a5);
}

// 0x5688D0
void sub_5688D0(int64_t a1, long long obj, int a4)
{
    int v1;
    int v2;
    char gender;
    char path[TIG_MAX_PATH];

    (void)a1;

    if (a4 == -1) {
        return;
    }

    sub_5689B0();
    sub_418A00(a4, &v1, &v2);

    gender = stat_level(obj, STAT_GENDER) != 0 ? 'm' : 'f';
    sprintf(path, "sound\\speech\\%.5d\\v%d_%c.mp3", v1, v2, gender);
    if (gender == 'f' && !tig_file_exists(path, NULL)) {
        sprintf(path, "sound\\speech\\%.5d\\v%d_%c.mp3", v1, v2, 'm');
    }
    if (tig_file_exists(path, NULL)) {
        dword_67B960 = gsound_play_voice(path, 0);
    }
}

// 0x5689B0
void sub_5689B0()
{
    if (dword_67B960 != TIG_SOUND_HANDLE_INVALID) {
        tig_sound_destroy(dword_67B960);
        dword_67B960 = TIG_SOUND_HANDLE_INVALID;
    }
}
