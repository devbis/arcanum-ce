#include "game/dialog.h"

#include <stdio.h>

#include "game/critter.h"
#include "game/newspaper.h"
#include "game/magictech.h"
#include "game/mes.h"
#include "game/timeevent.h"
#include "game/stat.h"
#include "game/skill.h"
#include "game/spell.h"
#include "game/random.h"
#include "game/rumor.h"

#define THIRTY_TWO 32

typedef struct DialogEntry {
    /* 0000 */ int field_0;
    /* 0004 */ int field_4;
    /* 0008 */ int field_8;
    /* 000C */ int field_C;
    /* 0010 */ int field_10;
    /* 0014 */ int field_14;
    /* 0018 */ int field_18;
} DialogEntry;

static_assert(sizeof(DialogEntry) == 0x1C, "wrong size");

typedef struct Dialog {
    /* 0000 */ char path[TIG_MAX_PATH];
    /* 0104 */ int refcount;
    /* 0108 */ DateTime timestamp;
    /* 0110 */ int entries_length;
    /* 0114 */ int entries_capacity;
    /* 0118 */ DialogEntry* entries;
    /* 011C */ int field_11C;
} Dialog;

static_assert(sizeof(Dialog) == 0x120, "wrong size");

static void sub_414E60(DialogEntryNode* a1, bool randomize);
static void sub_41A150(int a1, int a2, int a3, DialogEntryNode* a4);
static void sub_41A230(int a1, int a2, int a3, DialogEntryNode* a4);
static void sub_41A290(int a1, int a2, int a3, DialogEntryNode* a4);
static void sub_41A700(int a1, DialogEntryNode* a2);

// 0x5A063C
static const char* off_5A063C[] = {
    "gd_pc2m",
    "gd_pc2f",
    "gd_cls_m2m",
    "gd_cls_m2f",
    "gd_cls_f2f",
    "gd_cls_f2m",
    "gd_cls_pc2m",
    "gd_cls_pc2f",
    "gd_rce_m2m",
    "gd_rce_m2f",
    "gd_rce_f2f",
    "gd_rce_f2m",
    "gd_npc_m2m",
    "gd_npc_m2f",
    "gd_npc_f2f",
    "gd_npc_f2m",
    "gd_dumb_pc2m",
    "gd_dumb_pc2f",
    "gd_cls_dumb_pc2m",
    "gd_cls_dumb_pc2f",
    "gd_cls_dumb_m2m",
    "gd_cls_dumb_m2f",
    "gd_cls_dumb_f2f",
    "gd_cls_dumb_f2m",
    "gd_rce_dumb_m2m",
    "gd_rce_dumb_m2f",
    "gd_rce_dumb_f2f",
    "gd_rce_dumb_f2m",
    "gd_sto_m2m",
    "gd_sto_m2f",
    "gd_sto_f2f",
    "gd_sto_f2m",
};

// 0x5A06BC
static const char* off_5A06BC[] = {
    "ps",
    "ch",
    "pe",
    "al",
    "ma",
    "ta",
    "gv",
    "gf",
    "qu",
    "re",
    "$$",
    "in",
    "ha",
    "lf",
    "lc",
    "tr",
    "sk",
    "ru",
    "rq",
    "fo",
    "le",
    "qb",
    "me",
    "ni",
    "qa",
    "ra",
    "pa",
    "ss",
    "wa",
    "wt",
    "pv",
    "pf",
    "na",
    "ar",
    "rp",
    "ia",
    "sc",
};

// 0x5A0750
static const char* off_5A0750[] = {
    "$$",
    "re",
    "qu",
    "fl",
    "co",
    "gv",
    "gf",
    "mm",
    "al",
    "in",
    "lf",
    "lc",
    "tr",
    "ru",
    "rq",
    "jo",
    "wa",
    "lv",
    "ss",
    "sc",
    "so",
    "uw",
    "pv",
    "pf",
    "xp",
    "nk",
    "rp",
    "np",
    "ce",
    "fp",
    "su",
    "or",
    "ii",
    "ri",
    "et",
};

// 0x5D19F4
static mes_file_handle_t* dword_5D19F4;

// 0x5D19F8
static int dword_5D19F8;

// 0x5D19FC
static int dword_5D19FC;

// 0x5D1A00
static int dword_5D1A00;

// 0x5D1A04
static int dword_5D1A04;

// 0x5D1A08
static Dialog* dword_5D1A08;

// 0x5D1A0C
static bool dword_5D1A0C;

// 0x412D40
bool dialog_init(GameInitInfo* init_info)
{
    int index;

    (void)init_info;

    dword_5D19F4 = (mes_file_handle_t*)CALLOC(THIRTY_TWO, sizeof(mes_file_handle_t));
    for (index = 0; index < THIRTY_TWO; index++) {
        dword_5D19F4[index] = MES_FILE_HANDLE_INVALID;
    }

    return true;
}

// 0x412D80
void dialog_exit()
{
    int index;

    for (index = 0; index < THIRTY_TWO; index++) {
        if (dword_5D19F4[index] != MES_FILE_HANDLE_INVALID) {
            mes_unload(dword_5D19F4[index]);
        }
    }

    for (index = 0; index < dword_5D1A04; index++) {
        if (dword_5D1A08[index].path[0] != '\0') {
            sub_412F60(index);
        }
    }

    if (dword_5D1A04 > 0) {
        FREE(dword_5D1A08);
        dword_5D1A08 = NULL;
        dword_5D1A04 = 0;
    }
}

// 0x412E10
bool sub_412E10(const char* path, int* a2)
{
    int index;
    Dialog dialog;

    if (sub_4175D0(path, &index)) {
        dword_5D1A08[index].refcount++;
        dword_5D1A08[index].timestamp = sub_45A7C0();
    } else {
        strcpy(dialog.path, path);
        dialog.refcount = 1;
        dialog.timestamp = sub_45A7C0();
        dialog.entries_length = 0;
        dialog.entries_capacity = 0;
        dialog.entries = NULL;
        sub_417720(&dialog);

        if (dialog.entries_length == 0) {
            return false;
        }

        dword_5D1A08[index] = dialog;
        dword_5D1A00++;
    }

    *a2 = index;

    return true;
}

// 0x412F40
void sub_412F40(int dlg)
{
    dword_5D1A08[dlg].refcount--;
}

// 0x412F60
void sub_412F60(int dlg)
{
    int index;

    for (index = 0; index < dword_5D1A08[dlg].entries_length; index++) {
        sub_417F40(&(dword_5D1A08[dlg].entries[index]));
    }

    FREE(dword_5D1A08[dlg].entries);
    dword_5D1A08[dlg].path[0] = '\0';

    dword_5D1A00--;
}

// 0x412FD0
bool sub_412FD0(DialogEntryNode* a1)
{
    int64_t loc1;
    int64_t loc2;
    int64_t tmp;
    int64_t y1;
    int64_t y2;

    if (sub_4AD800(a1->field_38, a1->field_8, 0)) {
        return false;
    }

    sub_4C1020(a1->field_38, a1->field_8);

    if (sub_45D8D0(a1->field_38) || !sub_4AE120(a1->field_38, a1->field_8)) {
        if (player_is_pc_obj_(a1->field_8)) {
            loc1 = obj_field_int64_get(a1->field_8, OBJ_F_LOCATION);
            loc2 = obj_field_int64_get(a1->field_38, OBJ_F_LOCATION);
            sub_4B8680(loc1, &tmp, &y1);
            sub_4B8680(loc2, &tmp, &y2);
            if (y2 > y1) {
                sub_4B8CE0(loc2)
            } else {
                sub_4B8CE0(loc1);
            }
        }

        a1->field_17EC = a1->field_68;
        a1->field_17E8 = 0;
        sub_414E60(a1, 0);
    } else {
        sub_4185F0(a1->field_70, a1, 1000);
        a1->field_17E8 = 4;
    }

    return true;
}

// 0x413130
void sub_413130(DialogEntryNode* a1, int a2)
{
    int v1;
    int v2;
    int v3;

    if (a1->field_17E8 != 4
        && a1->field_17E8 != 5
        && a1->field_17E8 != 7
        && a1->field_17E8 != 6
        && a1->field_17E8 != 8
        && a1->field_17E8 != 9) {
        if (sub_4AD800(a1->field_38, a1->field_8, 0)) {
            a1->field_17E8 = 1;
            return;
        }

        if (a1->field_17E8 == 3) {
            sub_417590(a1->field_17EC, &v1, &v2);
            v3 = 0;
        } else if (sub_415BA0(a1, &(a1->field_182C[a2]), a2)) {
            v1 = a1->field_17F0[a2];
            v2 = a1->field_1804[a2];
            v3 = a1->field_1818[a2];
        } else {
            return;
        }

        if (sub_45D8D0(a1->field_38) || !sub_4AE120(a1->field_38, a1->field_8)) {
            sub_414810(v1, a2, v3, a2, a1);
        } else {
            sub_4185F0(a1->field_70, a1, 1000);
            a1->field_17E8 = 4;
        }
    }
}

// 0x413280
void sub_413280(DialogEntryNode* a1)
{
    sub_4C10A0(a1->field_38, a1->field_8);
}

// 0x4132A0
void sub_4132A0(int64_t a1, int64_t a2, char* buffer)
{
    DialogEntryNode v1;

    if (sub_4AD800(a1, a2, 0) == 0) {
        sub_413360(a1, a2, &v1);
        if (sub_45DDA0(a1) == a2) {
            sub_419260(&v1, "1 0, 2 0, 3 0, 4 0, 5 0, 6 0, 7 0, 8 0");
        } else {
            sub_419260(&v1, NULL);
        }
        strcpy(buffer, v1.field_70);
    } else {
        buffer[0] = '\0';
    }
}

// 0x413360
void sub_413360(int64_t a1, int64_t a2, DialogEntryNode* a3)
{
    a3->field_8 = a2;
    a3->field_38 = a1;
    sub_443EB0(a3->field_38, &(a3->field_40));
    sub_443EB0(a3->field_8, &(a3->field_10));
}

// 0x4133B0
void sub_4133B0(int64_t a1, int64_t a2, char* buffer, int* a4)
{
    DialogEntryNode v1;

    if (sub_4AD800(a1, a2, 0) == 0) {
        sub_413360(a1, a2, &v1);
        if (reaction_translate(sub_4C0CC0(a1, a2)) < 4) {
            sub_418780(buffer, &v1, 2000, 2099);
        } else {
            sub_418780(buffer, &v1, 1900, 1999);
        }
        *a4 = v1.field_458;
    } else {
        buffer[0] = '\0';
        *a4 = -1;
    }
}

// 0x413490
void sub_413490(int64_t a1, int64_t a2, char* a3)
{
    DialogEntryNode v1;

    if (sub_4AD800(a1, a2, 0) == 0) {
        sub_413360(a1, a2, &v1);
        sub_418780(a3, &v1, 800, 899);
    } else {
        a3[0] = '\0';
    }
}

// 0x413520
void sub_413520(int64_t a1, int64_t a2, char* a3)
{
    DialogEntryNode v1;

    if (sub_4AD800(a1, a2, 0) == 0) {
        sub_413360(a1, a2, &v1);
        sub_418780(a3, &v1, 900, 999);
    } else {
        a3[0] = '\0';
    }
}

// 0x4135B0
void sub_4135B0(int64_t a1, int64_t a2, char* a3)
{
    DialogEntryNode v1;

    if (sub_4AD800(a1, a2, 0) == 0) {
        sub_413360(a1, a2, &v1);
        sub_418780(a3, &v1, 1000, 1099);
    } else {
        a3[0] = '\0';
    }
}

// 0x413640
void sub_413640(int64_t a1, int64_t a2, char* a3)
{
    DialogEntryNode v1;

    if (sub_4AD800(a1, a2, 0) == 0) {
        sub_413360(a1, a2, &v1);
        sub_418780(a3, &v1, 1100, 1199);
    } else {
        a3[0] = '\0';
    }
}

// 0x4136D0
void sub_4136D0(int64_t a1, int64_t a2, char* a3)
{
    DialogEntryNode v1;

    if (sub_4AD800(a1, a2, 0) == 0) {
        sub_413360(a1, a2, &v1);
        sub_418780(a3, &v1, 1200, 1299);
    } else {
        a3[0] = '\0';
    }
}

// 0x413760
void sub_413760(int64_t a1, int64_t a2, char* a3)
{
    DialogEntryNode v1;

    if (sub_4AD800(a1, a2, 0) == 0) {
        sub_413360(a1, a2, &v1);
        sub_418780(a3, &v1, 4600, 4699);
    } else {
        a3[0] = '\0';
    }
}

// 0x4137F0
void sub_4137F0(int64_t a1, int64_t a2, char* a3)
{
    DialogEntryNode v1;

    if (sub_4AD800(a1, a2, 0) == 0) {
        sub_413360(a1, a2, &v1);
        sub_418780(a3, &v1, 1300, 1399);
    } else {
        a3[0] = '\0';
    }
}

// 0x413880
void sub_413880(int64_t a1, int64_t a2, char* a3)
{
    DialogEntryNode v1;

    if (sub_4AD800(a1, a2, 0) == 0) {
        sub_413360(a1, a2, &v1);
        sub_418780(a3, &v1, 1400, 1499);
    } else {
        a3[0] = '\0';
    }
}

// 0x413910
void sub_413910(int64_t a1, int64_t a2, char* a3)
{
    DialogEntryNode v1;

    if (sub_4AD800(a1, a2, 0) == 0) {
        sub_413360(a1, a2, &v1);
        sub_418480(a3, &v1, 2000);
    } else {
        a3[0] = '\0';
    }
}

// 0x4139A0
void sub_4139A0(int64_t a1, int64_t a2, char* a3)
{
    DialogEntryNode v1;

    if (sub_4AD800(a1, a2, 0) == 0) {
        sub_413360(a1, a2, &v1);
        sub_418780(a3, &v1, 4700, 4799);
    } else {
        a3[0] = '\0';
    }
}

// 0x413A30
void sub_413A30(DialogEntryNode* a1, bool a2)
{
    if (a2 || !sub_4AD800(a1->field_38, a1->field_8, 0)) {
        a1->field_17EC = a1->field_68;
        a1->field_17E8 = 0;
        sub_416840(a1, 0);
    } else {
        a1->field_70[0] = '\0';
        a1->field_458 = -1;
    }
}

// 0x413A90
void sub_413A90(int64_t a1, int64_t a2, int a3, char* a4, int* a5)
{
    DialogEntryNode v1;
    int start;
    int end;

    if (sub_4AD800(a1, a2, 1) == 0) {
        sub_413360(a1, a2, &v1);

        switch (a3) {
        case 1:
            start = 1600;
            end = 1699;
            break;
        case 2:
            start = 1700;
            end = 1799;
            break;
        case 3:
            start = 1800;
            end = 1899;
            break;
        case 4:
            start = 2600;
            end = 2699;
            break;
        case 5:
            start = 2700;
            end = 2799;
            break;
        case 6:
            start = 2800;
            end = 2899;
            break;
        case 7:
            start = 2900;
            end = 2999;
            break;
        default:
            a4[0] = '\0';
            *a5 = -1;
            return;
        }

        sub_418780(a4, &v1, start, end);
        *a5 = v1.field_458;
    } else {
        a4[0] = '\0';
        *a5 = -1;
    }
}

// 0x413BE0
void sub_413BE0(int64_t a1, int64_t a2, char* a3, int* a4)
{
    DialogEntryNode v1;

    if (sub_4AD800(a1, a2, 1) == 0) {
        sub_413360(a1, a2, &v1);
        sub_418780(a3, &v1, 2100, 2199);
        *a4 = v1.field_458;
    } else {
        a3[0] = '\0';
        *a4 = -1;
    }
}

// 0x413C90
void sub_413C90(int64_t a1, int64_t a2, char* a3, int* a4)
{
    DialogEntryNode v1;

    if (sub_4AD800(a1, a2, 0) == 0) {
        sub_413360(a1, a2, &v1);
        sub_418780(a3, &v1, 2200, 2299);
        *a4 = v1.field_458;
    } else {
        a3[0] = '\0';
        *a4 = -1;
    }
}

// 0x413D40
void sub_413D40(int64_t a1, int64_t a2, char* a3, int* a4)
{
    DialogEntryNode v1;

    if (sub_4AD800(a1, a2, 0) == 0) {
        sub_413360(a1, a2, &v1);
        sub_4185F0(a3, &v1, 1000);
        *a4 = v1.field_458;
    } else {
        a3[0] = '\0';
        *a4 = -1;
    }
}

// 0x413DF0
void sub_413DF0(int64_t a1, int64_t a2, char* a3, int* a4)
{
    DialogEntryNode v1;

    if (sub_4AD800(a1, a2, 1) == 0) {
        sub_413360(a1, a2, &v1);
        sub_418780(a3, &v1, 2300, 2399);
        *a4 = v1.field_458;
    } else {
        a3[0] = '\0';
        *a4 = -1;
    }
}

// 0x413EA0
void sub_413EA0(int64_t a1, int64_t a2, char* a3, int* a4)
{
    DialogEntryNode v1;

    if (sub_4AD800(a1, a2, 1) == 0) {
        sub_413360(a1, a2, &v1);
        sub_418780(a3, &v1, 3000, 3099);
        *a4 = v1.field_458;
    } else {
        a3[0] = '\0';
        *a4 = -1;
    }
}

// 0x413F50
void sub_413F50(int64_t a1, int64_t a2, int a3, char* a4, int* a5)
{
    DialogEntryNode v1;
    int start;
    int end;

    if (sub_4AD800(a1, a2, 1) == 0) {
        sub_413360(a1, a2, &v1);

        switch (a3) {
        case 0:
            start = 2400;
            end = 2499;
            break;
        case 1:
            start = 2500;
            end = 2599;
            break;
        default:
            a4[0] = '\0';
            *a5 = -1;
            return;
        }

        sub_418780(a4, &v1, start, end);
        *a5 = v1.field_458;
    } else {
        a4[0] = '\0';
        *a5 = -1;
    }
}

// 0x414020
void sub_414020(int64_t a1, int64_t a2, int a3, char* a4)
{
    DialogEntryNode v1;
    int start;
    int end;

    sub_413360(a1, a2, &v1);

    switch (a3) {
    case 0:
        start = 3100;
        end = 3199;
        break;
    case 1:
        start = 3200;
        end = 3299;
        break;
    case 2:
        start = 3300;
        end = 3399;
        break;
    case 3:
        start = 3400;
        end = 3499;
        break;
    case 4:
        start = 3500;
        end = 3599;
        break;
    case 5:
        start = 3600;
        end = 3699;
        break;
    case 6:
        start = 3700;
        end = 3799;
        break;
    case 7:
        start = 3800;
        end = 3899;
        break;
    case 8:
        start = 3900;
        end = 3999;
        break;
    case 9:
        start = 5600;
        end = 5699;
        break;
    default:
        return;
    }

    sub_418780(a4, &v1, start, end);
}

// 0x414130
void sub_414130(int64_t a1, int64_t a2, char* a3, int* a4)
{
    DialogEntryNode v1;

    if (sub_4AD800(a1, a2, 1) == 0) {
        sub_413360(a1, a2, &v1);
        sub_418780(a3, &v1, 4000, 4099);
        *a4 = v1.field_458;
    } else {
        a3[0] = '\0';
        *a4 = -1;
    }
}

// 0x4141E0
void sub_4141E0(int64_t a1, int64_t a2, char* a3, int* a4)
{
    DialogEntryNode v1;

    if (sub_4AD800(a1, a2, 1) == 0) {
        sub_413360(a1, a2, &v1);
        sub_418780(a3, &v1, 4100, 4199);
        *a4 = v1.field_458;
    } else {
        a3[0] = '\0';
        *a4 = -1;
    }
}

// 0x414290
void sub_414290(int64_t a1, int64_t a2, int a3, char* a4, int* a5)
{
    DialogEntryNode v1;
    int start;
    int end;

    if (sub_4AD800(a1, a2, 1) == 0) {
        sub_413360(a1, a2, &v1);

        switch (a3) {
        case 1:
            start = 5200;
            end = 5299;
            break;
        case 2:
            start = 5300;
            end = 5399;
            break;
        case 3:
            start = 5400;
            end = 5499;
            break;
        default:
            a4[0] = '\0';
            *a5 = -1;
            return;
        }

        sub_418780(a4, &v1, start, end);
        *a5 = v1.field_458;
    } else {
        a4[0] = '\0';
        *a5 = -1;
    }
}

// 0x414370
void sub_414370(int64_t a1, int64_t a2, int a3, char* a4, int* a5)
{
    DialogEntryNode v1;
    int start;
    int end;

    if (sub_4AD800(a1, a2, 1) == 0) {
        sub_413360(a1, a2, &v1);

        switch (a3) {
        case 1:
            start = 4900;
            end = 4999;
            break;
        case 2:
            start = 5000;
            end = 5099;
            break;
        case 3:
            start = 5100;
            end = 5199;
            break;
        case 4:
            start = 4800;
            end = 4899;
            break;
        default:
            a4[0] = '\0';
            *a5 = -1;
            return;
        }

        sub_418780(a4, &v1, start, end);
        *a5 = v1.field_458;
    } else {
        a4[0] = '\0';
        *a5 = -1;
    }
}

// 0x414490
void sub_414490(int64_t a1, int64_t a2, char* a3, int* a4)
{
    DialogEntryNode v1;

    if (sub_4AD800(a1, a2, 1) == 0) {
        sub_413360(a1, a2, &v1);
        sub_418780(a3, &v1, 5500, 5599);
        *a4 = v1.field_458;
    } else {
        a3[0] = '\0';
        *a4 = -1;
    }
}

// 0x414540
void sub_414540(int64_t a1, int64_t a2, char* a3)
{
    DialogEntryNode v1;

    if (sub_4AD800(a1, a2, 0) == 0) {
        sub_413360(a1, a2, &v1);
        sub_418780(a3, &v1, 5700, 5799);
    } else {
        a3[0] = '\0';
    }
}

// 0x4145D0
void sub_4145D0(int64_t a1, int64_t a2, char* a3)
{
    DialogEntryNode v1;

    if (sub_4AD800(a1, a2, 0) == 0) {
        sub_413360(a1, a2, &v1);
        sub_418780(a3, &v1, 5800, 5899);
    } else {
        a3[0] = '\0';
    }
}

// 0x414660
void sub_414660(int64_t a1, int64_t a2, char* a3)
{
    DialogEntryNode v1;

    if (sub_4AD800(a1, a2, 0) == 0) {
        sub_413360(a1, a2, &v1);
        sub_418780(a3, &v1, 5900, 5999);
    } else {
        a3[0] = '\0';
    }
}

// 0x4146F0
void sub_4146F0(int64_t a1, int64_t a2, char* a3)
{
    DialogEntryNode v1;

    if (sub_4AD800(a1, a2, 0) == 0) {
        sub_413360(a1, a2, &v1);
        sub_418780(a3, &v1, 6000, 6099);
    } else {
        a3[0] = '\0';
    }
}

// 0x414780
void sub_414780(int64_t a1, int64_t a2, char* a3)
{
    DialogEntryNode v1;

    if (sub_4AD800(a1, a2, 0) == 0) {
        sub_413360(a1, a2, &v1);
        sub_418780(a3, &v1, 6100, 6199);
    } else {
        a3[0] = '\0';
    }
}

// 0x414810
void sub_414810(int a1, int a2, int a3, int a4, DialogEntryNode* a5)
{
    int v1[100];
    int cnt;
    char *pch;
    int v2;

    switch (a1) {
    case 0:
        a5->field_17EC = a2;
        a5->field_17E8 = 0;
        sub_414E60(a5, 0);
        break;
    case 1:
        a5->field_17E8 = 1;
        break;
    case 2:
        a5->field_17EC = a2;
        a5->field_17E8 = 2;
        break;
    case 3:
        a5->field_17EC = a2;
        a5->field_17E8 = 3;
        break;
    case 4:
        sub_418B30(a2, a5);
        break;
    case 5:
        cnt = sub_417F90(v1, &(a5->field_460[a4][strlen(a5->field_460[a4]) + 1]));
        sub_418CA0(v1, cnt, a2, a5);
        break;
    case 6:
        sub_418DE0(a2, a5);
        break;
    case 7:
        sub_418F30(a2, a5);
        break;
    case 8:
        pch = strchr(&(a5->field_460[a4][strlen(a5->field_460[a4]) + 1]), '$');
        v2 = atoi(pch + 1);
        pch = strchr(pch, ',');
        cnt = sub_417F90(v1, pch + 1);
        sub_418FC0(v2, v1, cnt, a2, a5);
        break;
    case 9:
        sub_4190E0(a2, a5->field_17F0[1], a5->field_1804[1], a5);
        break;
    case 10:
        sub_4191E0(a2, a3, a5);
        break;
    case 11:
        sub_419830(0, a2, a5);
        break;
    case 12:
        sub_419830(1, a2, a5);
        break;
    case 13:
        sub_419830(2, a2, a5);
        break;
    case 14:
        sub_419B50(a2, a3, a5);
        break;
    case 15:
        sub_419C30(a2, a3, a5);
        break;
    case 16:
        sub_419CB0(a2, a5->field_17F0[1], a5->field_1804[1], a5);
        break;
    case 17:
        sub_419D50(a2, a5->field_17F0[1], a5->field_1804[1], a5);
        break;
    case 18:
        sub_419E70(&(a5->field_460[a4][strlen(a5->field_460[a4]) + 1]), a2, a3, 0, a5);
        break;
    case 19:
        sub_41A0F0(a5->field_17EC, a2, a3, a5);
        break;
    case 20:
        sub_41A150(a2, a5->field_17F0[1], a5->field_1804[1], a5);
        break;
    case 21:
        sub_419E70(&(a5->field_460[a4][strlen(a5->field_460[a4]) + 1]), a2, a3, 1, a5);
        break;
    case 22:
        sub_41A230(a5->field_17EC, a2, a3, a5);
        break;
    case 23:
        sub_41A290(a2, a5->field_17F0[1], a5->field_1804[1], a5);
        break;
    case 24:
        sub_41A3E0(a2, a5);
        break;
    case 25:
        sub_41A520(a2, a5);
        break;
    case 26:
        sub_41A620(a2, a5);
        break;
    case 27:
        sub_41A700(a2, a5);
        break;
    case 28:
        sub_41A880(a2, a3, a5);
        break;
    case 29:
        sub_41A8C0(a2, a5->field_17F0[1], a5->field_1804[1], a5);
        break;
    case 30:
        a5->field_17E8 = 6;
        break;
    }
}

// 0x414E60
void sub_414E60(DialogEntryNode* a1, bool randomize)
{
    int index;
    int v1[5];
    int v2;

    for (index = 0; index < 5; index++) {
        a1->field_182C[index] = 0;
        a1->field_460[index][0] = '\0';
    }

    if (randomize) {
        random_seed(a1->field_1844);
    } else {
        a1->field_1844 = random_seed_generate();
    }

    if (sub_416840(a1, a2)) {
        a1->field_45C = sub_414F50(a1, v1);
        v2 = 0;
        for (index = 0; index < a1->field_45C; index++) {
            if (!sub_416C10(v1[index], index - v2, a1)) {
                v2++;
            }
        }
        a1->field_45C -= v2;
        if (a1->field_45C == 0) {
            sub_4182D0(a1->field_460[0], a1, 400, 499);
            a1->field_17F0[0] = 1;
            a1->field_45C = 1;
        }
    }
}

// 0x414F50
void sub_414F50()
{
    // TODO: Incomplete.
}

// 0x4150D0
void sub_4150D0()
{
    // TODO: Incomplete.
}

// 0x415BA0
void sub_415BA0()
{
    // TODO: Incomplete.
}

// 0x4167C0
void sub_4167C0()
{
    // TODO: Incomplete.
}

// 0x416840
void sub_416840()
{
    // TODO: Incomplete.
}

// 0x416AB0
bool sub_416AB0(int dlg, DialogEntry* a2)
{
    DialogEntry* v1;

    v1 = bsearch(a2, dword_5D1A08[dlg].entries, dword_5D1A08[dlg].entries_length, sizeof(*a2), sub_417E00);
    if (v1 == NULL) {
        return false;
    }

    *a2 = *v1;

    return true;
}

// 0x416B00
void sub_416B00()
{
    // TODO: Incomplete.
}

// 0x416C10
void sub_416C10()
{
    // TODO: Incomplete.
}

// 0x417590
void sub_417590()
{
    // TODO: Incomplete.
}

// 0x4175D0
void sub_4175D0()
{
    // TODO: Incomplete.
}

// 0x417720
void sub_417720()
{
    // TODO: Incomplete.
}

// 0x417860
void sub_417860()
{
    // TODO: Incomplete.
}

// 0x417C20
void sub_417C20()
{
    // TODO: Incomplete.
}

// 0x417D60
TigFile* sub_417D60(const char* fname, const char* mode)
{
    dword_5D19FC = 0;
    dword_5D19F8 = 0;
    return tig_file_fopen(fname, mode);
}

// 0x417D80
int sub_417D80(TigFile* stream)
{
    return tig_file_fclose(stream);
}

// 0x417D90
void sub_417D90()
{
    // TODO: Incomplete.
}

// 0x417E00
void sub_417E00()
{
    // TODO: Incomplete.
}

// 0x417E20
void sub_417E20()
{
    // TODO: Incomplete.
}

// 0x417F40
void sub_417F40(DialogEntry* a1)
{
    if (!a1->field_C) {
        FREE(a1->field_8);
    }
    FREE(a1->field_4);

    if (a1->field_10 != NULL) {
        FREE(a1->field_10);
    }

    if (a1->field_18 != NULL) {
        FREE(a1->field_18);
    }
}

// 0x417F90
void sub_417F90()
{
    // TODO: Incomplete.
}

// 0x418030
void sub_418030()
{
    TigFileList file_list;
    unsigned int index;
    char path[80];
    int dlg;
    int entry_index;
    DialogEntry* v2;
    int v3;
    DialogEntry tmp;

    tig_file_list_create(&file_list, "dlg\\*.dlg");
    for (index = 0; index < file_list.count; index++) {
        sprintf(path, "dlg\\%s", file_list.entries[index].path);
        tig_debug_printf("Checking dialog file %s\n", path);
        if (sub_412E10(path, &dlg)) {
            for (entry_index = 0; entry_index < dword_5D1A08[dlg].entries_length; entry_index++) {
                v2 = &(dword_5D1A08[dlg].entries[entry_index]);
                if (v2->field_C) {
                    v3 = sub_4C9B90(v2->field_4);
                    if (v3 > 0) {
                        tig_debug_printf("PC response %d too long by %d pixels\n", v2->field_0, v3);
                    }

                    if (v2->field_14 > 0) {
                        tmp.field_0 = v2->field_14;
                        if (sub_416AB0(dlg, &tmp)) {
                            if (tmp.field_C) {
                                tig_debug_printf("PC response %d jumps to non-PC line %d\n", v2->field_0, v2->field_14);
                            }
                        } else {
                            tig_debug_printf("PC response %d jumps to non-existent dialog line %d\n", v2->field_0, v2->field_14);
                        }
                    }
                }
            }
            sub_412F40(dlg);
        }
    }
    tig_file_list_destroy(&file_list);

    sub_4181D0(0);
    sub_4181D0(1);
    sub_4181D0(6);
    sub_4181D0(7);
    sub_4181D0(16);
    sub_4181D0(17);
    sub_4181D0(18);
    sub_4181D0(19);
}

// 0x4181D0
void sub_4181D0(int a1)
{
    MesFileEntry mes_file_entry;
    int v1;

    tig_debug_printf("Checking generated dialog file mes\\%s.mes\n", off_5A063C[a1]);
    sub_418250(a1);

    if (mes_find_first(dword_5D19F4[a1], &mes_file_entry)) {
        do {
            v1 = sub_4C9B90(mes_file_entry.str);
            if (v1 > 0) {
                tig_debug_printf("Generated response %d too long by %d pixels\n", mes_file_entry.num, v1);
            }
        } while (mes_find_next(dword_5D19F4[a1], &mes_file_entry));
    }
}

// 0x418250
void sub_418250(int a1)
{
    char path[TIG_MAX_PATH];

    if (dword_5D19F4[a1] == MES_FILE_HANDLE_INVALID) {
        sprintf(path, "mes\\%s.mes", off_5A063C[a1]);
        if (!mes_load(path, &(dword_5D19F4[a1]))) {
            tig_debug_printf("Cannot open generated dialog file %s\n", path);
            exit(EXIT_SUCCESS); // FIXME: Should be EXIT_FAILURE.
        }
    }
}

// 0x4182C0
void sub_4182C0()
{
    dword_5D1A0C = true;
}

// 0x4182D0
void sub_4182D0()
{
    // TODO: Incomplete.
}

// 0x418390
void sub_418390()
{
    // TODO: Incomplete.
}

// 0x418460
void sub_418460(int a1, int a2)
{
    int v1 = sub_445090();
    sub_4182D0(a1, a2, v1 + 1700, v1 + 1700);
}

// 0x418480
void sub_418480()
{
    // TODO: Incomplete.
}

// 0x4185F0
void sub_4185F0()
{
    // TODO: Incomplete.
}

// 0x418780
void sub_418780()
{
    // TODO: Incomplete.
}

// 0x418870
void sub_418870()
{
    // TODO: Incomplete.
}

// 0x4189C0
int sub_4189C0(const char* a1, int a2)
{
    if (a1 != NULL && a2 != 0) {
        return ((a2 & 0x7FFF) << 16) | (atoi(a1) & 0xFFFF);
    } else {
        return -1;
    }
}

// 0x418A00
int sub_418A00(int a1, int* a2, int* a3)
{
    if (a1 != -1) {
        *a2 = (a1 >> 16) & 0x7FFF;
        *a3 = a1 & 0xFFFF;
    } else {
        *a2 = 0;
        *a3 = 0;
    }
}

// 0x418A40
void sub_418A40(int a1, int a2, int a3, int a4, int a5, DialogEntryNode *a6)
{
    int v1;
    char buffer[1000];

    v1 = sub_4C1150(a6->field_38, a6->field_8, a1);
    if (v1 == 1) {
        v1 = 2;
    }

    sub_418480(buffer, a6, 1000);
    sprintf(a6->field_70, buffer, v1);
    a6->field_45C = 2;
    sub_4182D0(a6->field_460[0], a6, 1, 99);
    a6->field_1804[0] = v1;
    a6->field_17F0[0] = 4;
    a6->field_182C[0] = 0;
    sub_4182D0(a6->field_460[1], a6, 100, 199);
    a6->field_17F0[1] = a4;
    a6->field_182C[1] = 0;
    a6->field_1804[1] = a5;
    a6->field_17F0[2] = a2;
    a6->field_1804[2] = a3;
}

// 0x418B30
void sub_418B30()
{
    // TODO: Incomplete.
}

// 0x418C40
void sub_418C40(int a1, int a2, int a3, DialogEntryNode* a4)
{
    sub_418480(a4->field_70, a4, a1);
    a4->field_45C = 1;
    sub_4182D0(a4->field_460[0], a4, 600, 699);
    a4->field_17F0[0] = a2;
    a4->field_1804[0] = a3;
    a4->field_182C[0] = 0;
}

// 0x418CA0
void sub_418CA0()
{
    // TODO: Incomplete.
}

// 0x418DE0
void sub_418DE0(int a1, DialogEntryNode* a2)
{
    int v1;

    v1 = a2->field_45C - 1;
    if (IS_TECH_SKILL(a1)) {
        if (tech_skill_get_training(a2->field_8, GET_TECH_SKILL(a1)) != TRAINING_NONE) {
            sub_418C40(4000, a2->field_17F0[v1], a2->field_1804[v1], a2);
            return;
        }

        if (tech_skill_set_training(a2->field_8, GET_TECH_SKILL(a1), TRAINING_APPRENTICE) == TRAINING_NONE) {
            sub_418C40(5000, a2->field_17F0[v1], a2->field_1804[v1], a2);
            return;
        }

        tech_skill_set_training(a2->field_8, GET_TECH_SKILL(a1), TRAINING_NONE);
        sub_418A40(100, 7, a1, a2->field_17F0[v1], a2->field_1804[v1], a2);
    } else {
        if (basic_skill_get_training(a2->field_8, GET_BASIC_SKILL(a1)) != TRAINING_NONE) {
            sub_418C40(4000, a2->field_17F0[v1], a2->field_1804[v1], a2);
            return;
        }

        if (basic_skill_set_training(a2->field_8, GET_BASIC_SKILL(a1), TRAINING_APPRENTICE) == TRAINING_NONE) {
            sub_418C40(5000, a2->field_17F0[v1], a2->field_1804[v1], a2);
            return;
        }

        basic_skill_set_training(a2->field_8, GET_BASIC_SKILL(a1), TRAINING_NONE);
        sub_418A40(100, 7, a1, a2->field_17F0[v1], a2->field_1804[v1], a2);
    }
}

// 0x418F30
void sub_418F30(int a1, DialogEntryNode* a2)
{
    if (IS_TECH_SKILL(a1)) {
        tech_skill_set_training(a2->field_8, GET_TECH_SKILL(a1), TRAINING_APPRENTICE);
    } else {
        basic_skill_set_training(a2->field_8, GET_BASIC_SKILL(a1), TRAINING_APPRENTICE);
    }

    sub_418480(a2->field_70, a2, 6000);
    a2->field_45C = 1;
    sub_418390(a2->field_460[0], a2, 1000);
    a2->field_17F0[0] = a2->field_17F0[1];
    a2->field_1804[0] = a2->field_1804[1];
    a2->field_182C[0] = 0;
}

// 0x418FC0
void sub_418FC0()
{
    // TODO: Incomplete.
}

// 0x4190E0
void sub_4190E0(int a1, int a2, int a3, DialogEntryNode* a4)
{
    char buffer[1000];

    sub_4C5700(a4->field_8, a4->field_38, a1, buffer);
    sub_416B00(a4->field_70, buffer, a4);
    a4->field_458 = -1;
    sub_4C57E0(a4->field_8, a1);
    a4->field_45C = 1;
    sub_418390(a4->field_460[0], a4, 1000);
    a4->field_17F0[0] = a2;
    a4->field_1804[0] = a3;
    a4->field_182C[0] = 0;
}

// 0x419190
void sub_419190(int a1, int a2, int a3, DialogEntryNode* a4)
{
    sub_418390(a4->field_460[a1], a4, 3000);
    a4->field_17F0[a1] = 10;
    a4->field_1804[a1] = a2;
    a4->field_1818[a1] = a3;
}

// 0x4191E0
void sub_4191E0(int a1, int a2, DialogEntryNode* a3)
{
    sub_4C0DE0(a3->field_38, a3->field_8, -10);
    if (sub_4C0CC0(a3->field_38, a3->field_8) > 20) {
        sub_414810(a1, a2, 0, 0, a3);
    } else {
        sub_4185F0(a3->field_70, a3, 1000);
        a3->field_45C = 0;
        a3->field_17E8 = 4;
    }
}

// 0x419260
void sub_419260()
{
    // TODO: Incomplete.
}

// 0x4197D0
bool sub_4197D0(unsigned int flags, int a2, DialogEntryNode* a3)
{
    if ((flags & 0x1) != 0) {
        a3->field_68 = a2;
        sub_413A30(a3, 0);
        a3->field_17E8 = 4;
        return true;
    }

    if ((flags & 0x4) != 0) {
        a3->field_17EC = a2;
        a3->field_17E8 = 0;
        sub_414E60(a3, 0);
        return true;
    }

    return false;
}

// 0x419830
void sub_419830()
{
    // TODO: Incomplete.
}

// 0x419A00
void sub_419A00(int a1, int a2, int a3, DialogEntryNode* a4)
{
    char buffer[1000];
    const char* name;

    if (IS_TECH_SKILL(a2)) {
        name = tech_skill_get_name(GET_TECH_SKILL(a2));
    } else {
        name = basic_skill_get_name(GET_BASIC_SKILL(a2));
    }

    sprintf(a4->field_460[a1], buffer, name);
    a4->field_17F0[a1] = 14;
    a4->field_1804[a1] = a2;
    a4->field_1818[a1] = a3;
}

// 0x419AC0
void sub_419AC0(int a1, int a2, int a3, DialogEntryNode* a4)
{
    char buffer[1000];

    sub_4182D0(buffer, a4, 1000, 1099);
    sprintf(a4->field_460[a1], buffer, spell_get_name(a2));
    a4->field_17F0[a1] = 15;
    a4->field_1804[a1] = a2;
    a4->field_1818[a1] = a3;
}

// 0x419B50
void sub_419B50(int a1, int a2, DialogEntryNode* a3)
{
    int v1;
    int v2;
    int skill_level;
    int training;
    int v3;

    sub_417590(a2, &v1, &v2);
    if (critter_leader_get(a3->field_38) == a3->field_8) {
        sub_419CB0(a1, v1, v2, a3);
    } else {
        if (IS_TECH_SKILL(a1)) {
            skill_level = tech_skill_level(a3->field_38, GET_TECH_SKILL(a1));
            training = tech_skill_get_training(a3->field_38, GET_TECH_SKILL(a1));
            v3 = sub_4C69E0(GET_TECH_SKILL(a1), skill_level, training);
        } else {
            skill_level = basic_skill_level(a3->field_38, GET_BASIC_SKILL(a1));
            training = basic_skill_get_training(a3->field_38, GET_BASIC_SKILL(a1));
            v3 = sub_4C62D0(GET_BASIC_SKILL(a1), skill_level, training);
        }
        sub_418A40(v3, 16, a1, v1, v2, a3);
    }
}

// 0x419C30
void sub_419C30(int a1, int a2, DialogEntryNode* a3)
{
    int v1;
    int v2;

    sub_417590(a2, &v1, &v2);
    if (critter_leader_get(a3->field_38) == a3->field_8) {
        sub_419D50(a1, v1, v2, a3);
    } else {
        sub_418A40(sub_4B1740(a1), 17, a1, v1, v2, a3);
    }
}

// 0x419CB0
void sub_419CB0(int a1, int a2, int a3, DialogEntryNode* a4)
{
    unsigned int flags;
    int v1;

    if (sub_45DDA0(a4->field_38)) {
        flags = 0;
    } else {
        flags = 0x2000;
    }

    v1 = sub_4C91F0(a4->field_38, a1);
    sub_4350F0(a4->field_38, a4->field_8, v1, a1, flags);
    sub_418480(a4->field_70, a4, 15000);
    a4->field_45C = 1;
    sub_418390(a4->field_460[0], a4, 1000);
    a4->field_17F0[0] = a2;
    a4->field_1804[0] = a3;
    a4->field_182C[0] = 0;
}

// 0x419D50
void sub_419D50(int a1, int a2, int a3, DialogEntryNode* a4)
{
    MagicTechSerializedData v1;

    sub_455A20(&v1, a4->field_38, a1);
    sub_4440E0(a4->field_8, &(v1.field_70));
    if (!sub_45DDA0(a4->field_38)) {
        v1.field_DC |= 0x2;
    }
    sub_455AC0(&v1);
    sub_418480(a4->field_70, a4, 15000);
    a4->field_45C = 1;
    sub_418390(a4->field_460[0], a4, 1000);
    a4->field_17F0[0] = a2;
    a4->field_1804[0] = a3;
    a4->field_182C[0] = 0;
}

// 0x419E20
void sub_419E20()
{
    // TODO: Incomplete.
}

// 0x419E70
void sub_419E70()
{
    // TODO: Incomplete.
}

// 0x41A0F0
void sub_41A0F0(int a1, int a2, int a3, DialogEntryNode* a4)
{
    int v1;
    int v2;

    sub_417590(a3, &v1, &v2);
    if (a1 > 0) {
        sub_418A40(a1, 20, a2, v1, v2, a4);
    } else {
        sub_41A150(a2, v1, v2, a4);
    }
}

// 0x41A150
void sub_41A150(int a1, int a2, int a3, DialogEntryNode* a4)
{
    int64_t loc;
    int64_t v1;
    int v2;
    int64_t v3;
    int v4;

    loc = obj_field_int64_get(a4->field_38, OBJ_F_LOCATION);
    v1 = sub_4CAED0(a1);
    v2 = sub_4B8D50(loc, v1);
    v3 = sub_4B96F0(loc, v1) / 3168;

    if (v3 < 2) {
        v4 = 200;
    } else if (v3 < 100) {
        v4 = 300;
    } else {
        v4 = 400;
    }

    sub_418780(a4->field_70, a4, v4 + 10 * v2, v4 + 10 * v2 + 9);

    sub_418390(a4->field_460[0], a4, 1000);
    a4->field_17F0[0] = a2;
    a4->field_1804[0] = a3;
    a4->field_182C[0] = 0;
    a4->field_45C = 1;
}

// 0x41A230
void sub_41A230(int a1, int a2, int a3, DialogEntryNode* a4)
{
    int v1;
    int v2;

    sub_417590(a3, &v1, &v2);
    if (a1 > 0) {
        sub_418A40(a1, 23, a2, v1, v2, a4);
    } else {
        sub_41A290(a2, v1, v2, a4);
    }
}

// 0x41A290
void sub_41A290(int a1, int a2, int a3, DialogEntryNode* a4)
{
    int64_t loc;
    int64_t v1;
    int v2;
    int64_t v3;
    int v4;
    char buffer[1000];

    loc = obj_field_int64_get(a4->field_38, OBJ_F_LOCATION);
    v1 = sub_4CAED0(a1);
    v2 = sub_4B8D50(loc, v1);
    v3 = sub_4B96F0(loc, v1) / 3168;
    v4 = v3 < 2 ? 600 : 700;

    sub_418780(buffer, a4, v4 + 10 * v2, v4 + 10 * v2 + 9);

    if (v3 < 2) {
        strcpy(a4->field_70, buffer);
    } else {
        sprintf(a4->field_70, buffer, v3);
    }

    sub_418390(a4->field_460[0], a4, 1000);
    a4->field_17F0[0] = a2;
    a4->field_1804[0] = a3;
    a4->field_182C[0] = 0;
    a4->field_45C = 1;
    sub_4CAFD0(a4->field_8, a1);
}

// 0x41A3E0
void sub_41A3E0(int a1, DialogEntryNode* a2)
{
    sub_41A440(a2->field_70, a2);
    sub_418390(a2->field_460[0], a2, 1000);
    sub_417590(a1, a2->field_17F0, a2->field_1804);
    a2->field_182C[0] = 0;
    a2->field_45C = 1;
}

// 0x41A440
void sub_41A440(char* buffer, DialogEntryNode* a2)
{
    int v1;
    int v2;
    MesFileEntry mes_file_entry;

    v1 = sub_445090();
    if (!sub_418870(buffer, a2, v2 + 10000)) {
        if (stat_level(a2->field_38, STAT_GENDER) == GENDER_MALE) {
            v2 = stat_level(a2->field_8, STAT_GENDER) == GENDER_MALE ? 29 : 28;
        } else {
            v2 = stat_level(a2->field_8, STAT_GENDER) == GENDER_MALE ? 31 : 30;
        }

        mes_file_entry.num = stat_level(a2->field_38, STAT_RACE) + 100 * v1;
        sub_418250(v2);
        mes_get_msg(dword_5D19F4[v2], &mes_file_entry);
        sub_416B00(buffer, mes_file_entry.str, a2);
        a2->field_458 = -1;
    }
}

// 0x41A520
void sub_41A520(int a1, DialogEntryNode* a2)
{
    sub_418780(a2->field_70, a2, 4200, 4299);
    sub_4182D0(a2->field_460[0], a2, 2200, 2299);
    a2->field_17F0[0] = 28;
    a2->field_1804[0] = sub_4BF200(4);
    a2->field_1818[0] = a1;
    sub_4182D0(a2->field_460[1], a2, 2000, 2099);
    a2->field_17F0[1] = 26;
    a2->field_1804[1] = a1;
    sub_4182D0(a2->field_460[2], a2, 2100, 2199);
    a2->field_17F0[2] = 27;
    a2->field_1804[2] = a1;
    sub_4182D0(a2->field_460[3], a2, 800, 899);
    sub_417590(a1, &a2->field_17F0[3], &a2->field_1804[3]);
    a2->field_182C[0] = 0;
    a2->field_182C[1] = 0;
    a2->field_182C[2] = 0;
    a2->field_182C[3] = 0;
    a2->field_45C = 4;
}

// 0x41A620
void sub_41A620(int a1, DialogEntryNode* a2)
{
    int v1;
    int pos;

    v1 = sub_4BF200(4);
    sub_460800(v1, a2->field_70);

    pos = (int)strlen(a2->field_70);
    if (pos > 0) {
        if (a2->field_70[pos - 1] != '.'
            && a2->field_70[pos - 1] != '?'
            && a2->field_70[pos - 1] != '!') {
            a2->field_70[pos++] = '.';
        }

        a2->field_70[pos++] = ' ';
        a2->field_70[pos++] = '\0';
    }

    sub_418780(&(a2->field_70[pos]), a2, 4300, 4399);
    sub_4182D0(a2->field_460[0], a2, 1, 99);
    a2->field_17F0[0] = 28;
    a2->field_1804[0] = v1;
    a2->field_1818[0] = a1;
    sub_4182D0(a2->field_460[1], a2, 100, 199);
    sub_417590(a1, &a2->field_17F0[1], &a2->field_1804[1]);
    a2->field_182C[0] = 0;
    a2->field_182C[1] = 0;
    a2->field_45C = 2;
}

// 0x41A700
void sub_41A700(int a1, DialogEntryNode* a2)
{
    int cnt;
    int index;
    int v1[4];

    cnt = 0;
    for (index = 0; index < 4; index++) {
        v1[index] = sub_4BF200(index);
        if (v1[index] != -1) {
            cnt++;
        }
    }

    if (cnt != 0) {
        sub_418780(a2->field_70, a2, 4500, 4599);
        for (index = 0; index < cnt; index++) {
            sub_460800(v1[index], a2->field_460[index]);
            a2->field_17F0[index] = 28;
            a2->field_1804[index] = v1[index];
        }
        sub_4182D0(a2->field_460[cnt], a2, 800, 899);
        sub_417590(a1, &a2->field_17F0[cnt], &a2->field_1804[cnt]);
        a2->field_45C = cnt + 1;
    } else {
        sub_418780(a2->field_70, a2, 4400, 4499);
        sub_4182D0(a2->field_460[0], a2, 600, 699);
        sub_417590(a1, a2->field_17F0, a2->field_1804);
        a2->field_45C = 1;
    }

    for (index = 0; index < a2->field_45C; index++) {
        a2->field_182C[index] = 0;
    }
}

// 0x41A880
void sub_41A880(int a1, int a2, DialogEntryNode* a3)
{
    int v1;
    int v2;

    sub_417590(a2, &v1, &v2);
    sub_418A40(2, 29, a1, v1, v2, a3);
}

// 0x41A8C0
void sub_41A8C0(int a1, int a2, int a3, DialogEntryNode* a4)
{
    int64_t loc;
    int64_t obj;

    loc = obj_field_int64_get(a4->field_8, OBJ_F_LOCATION);
    obj = sub_4BF210(a1, loc);
    sub_4617F0(obj, a4->field_8);
    sub_418480(a4->field_70, a4, 15000);
    sub_418390(a4->field_460, a4, 1000);
    a4->field_17F0[0] = a2;
    a4->field_1804[0] = a3;
    a4->field_182C[0] = 0;
}
