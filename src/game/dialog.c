#include "game/dialog.h"

#include <stdio.h>

#include "game/ai.h"
#include "game/anim.h"
#include "game/area.h"
#include "game/critter.h"
#include "game/item.h"
#include "game/magictech.h"
#include "game/mes.h"
#include "game/mp_utils.h"
#include "game/newspaper.h"
#include "game/player.h"
#include "game/quest.h"
#include "game/random.h"
#include "game/reaction.h"
#include "game/reputation.h"
#include "game/rumor.h"
#include "game/script_name.h"
#include "game/script.h"
#include "game/skill.h"
#include "game/spell.h"
#include "game/stat.h"
#include "game/tc.h"
#include "game/timeevent.h"
#include "game/ui.h"

#define THIRTY_TWO 32

typedef enum GeneratedDialog {
    GD_PC2M,
    GD_PC2F,
    GD_CLS_M2M,
    GD_CLS_M2F,
    GD_CLS_F2F,
    GD_CLS_F2M,
    GD_CLS_PC2M,
    GD_CLS_PC2F,
    GD_RCE_M2M,
    GD_RCE_M2F,
    GD_RCE_F2F,
    GD_RCE_F2M,
    GD_NPC_M2M,
    GD_NPC_M2F,
    GD_NPC_F2F,
    GD_NPC_F2M,
    GD_DUMB_PC2M,
    GD_DUMB_PC2F,
    GD_CLS_DUMB_PC2M,
    GD_CLS_DUMB_PC2F,
    GD_CLS_DUMB_M2M,
    GD_CLS_DUMB_M2F,
    GD_CLS_DUMB_F2F,
    GD_CLS_DUMB_F2M,
    GD_RCE_DUMB_M2M,
    GD_RCE_DUMB_M2F,
    GD_RCE_DUMB_F2F,
    GD_RCE_DUMB_F2M,
    GD_STO_M2M,
    GD_STO_M2F,
    GD_STO_F2F,
    GD_STO_F2M,
} GeneratedDialog;

typedef enum DialogCondition {
    DIALOG_COND_PS,
    DIALOG_COND_CH,
    DIALOG_COND_PE,
    DIALOG_COND_AL,
    DIALOG_COND_MA,
    DIALOG_COND_TA,
    DIALOG_COND_GV,
    DIALOG_COND_GF,
    DIALOG_COND_QU,
    DIALOG_COND_RE,
    DIALOG_COND_GOLD,
    DIALOG_COND_IN,
    DIALOG_COND_HA,
    DIALOG_COND_LF,
    DIALOG_COND_LC,
    DIALOG_COND_TR,
    DIALOG_COND_SK,
    DIALOG_COND_RU,
    DIALOG_COND_RQ,
    DIALOG_COND_FO,
    DIALOG_COND_LE,
    DIALOG_COND_QB,
    DIALOG_COND_ME,
    DIALOG_COND_NI,
    DIALOG_COND_QA,
    DIALOG_COND_RA,
    DIALOG_COND_PA,
    DIALOG_COND_SS,
    DIALOG_COND_WA,
    DIALOG_COND_WT,
    DIALOG_COND_PV,
    DIALOG_COND_PF,
    DIALOG_COND_NA,
    DIALOG_COND_AR,
    DIALOG_COND_RP,
    DIALOG_COND_IA,
    DIALOG_COND_SC,
    DIALOG_COND_COUNT,
} DialogCondition;

typedef enum DialogAction {
    DIALOG_ACTION_GOLD,
    DIALOG_ACTION_RE,
    DIALOG_ACTION_QU,
    DIALOG_ACTION_FL,
    DIALOG_ACTION_CO,
    DIALOG_ACTION_GV,
    DIALOG_ACTION_GF,
    DIALOG_ACTION_MM,
    DIALOG_ACTION_AL,
    DIALOG_ACTION_IN,
    DIALOG_ACTION_LF,
    DIALOG_ACTION_LC,
    DIALOG_ACTION_TR,
    DIALOG_ACTION_RU,
    DIALOG_ACTION_RQ,
    DIALOG_ACTION_JO,
    DIALOG_ACTION_WA,
    DIALOG_ACTION_LV,
    DIALOG_ACTION_SS,
    DIALOG_ACTION_SC,
    DIALOG_ACTION_SO,
    DIALOG_ACTION_UW,
    DIALOG_ACTION_PV,
    DIALOG_ACTION_PF,
    DIALOG_ACTION_XP,
    DIALOG_ACTION_NK,
    DIALOG_ACTION_RP,
    DIALOG_ACTION_NP,
    DIALOG_ACTION_CE,
    DIALOG_ACTION_FP,
    DIALOG_ACTION_SU,
    DIALOG_ACTION_OR,
    DIALOG_ACTION_II,
    DIALOG_ACTION_RI,
    DIALOG_ACTION_ET,
    DIALOG_ACTION_COUNT,
} DialogAction;

typedef struct DialogEntry {
    /* 0000 */ int num;
    /* 0004 */ char* str;
    union {
        /* 0008 */ int gender;
        /* 0008 */ char* female_str;
    } data;
    /* 000C */ int iq;
    /* 0010 */ char* conditions;
    /* 0014 */ int response_val;
    /* 0018 */ char* actions;
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

static void sub_414810(int a1, int a2, int a3, int a4, DialogEntryNode* a5);
static void sub_414E60(DialogEntryNode* a1, bool randomize);
static int sub_414F50(DialogEntryNode* a1, int* a2);
static bool sub_4150D0(DialogEntryNode* a1, char* a2);
static bool sub_415BA0(DialogEntryNode* a1, char* a2, int a3);
static int sub_4167C0(const char* str);
static bool sub_416840(DialogEntryNode* a1, bool a2);
static bool sub_416AB0(int dlg, DialogEntry* a2);
static void sub_416B00(char* dst, char* src, DialogEntryNode* a3);
static bool sub_416C10(int a1, int a2, DialogEntryNode* a3);
static void sub_417590(int a1, int* a2, int* a3);
static bool sub_4175D0(const char* path, int* index_ptr);
static void sub_417720(Dialog* dialog);
static bool sub_417860(TigFile* stream, DialogEntry* entry, int* a3);
static bool sub_417C20(TigFile* stream, char* str, int* line_ptr);
static TigFile* dialog_file_fopen(const char* fname, const char* mode);
static int dialog_file_fclose(TigFile* stream);
static int dialog_file_fgetc(TigFile* stream);
static int dialog_entry_compare(const void* va, const void* vb);
static void sub_417E20(DialogEntry* a1, const DialogEntry* a2);
static void sub_417F40(DialogEntry* a1);
static int sub_417F90(int* values, char* str);
static void dialog_check_generated(int gd);
static void dialog_load_generated(int gd);
static void sub_4182D0(char* str, DialogEntryNode* a2, int start, int end);
static void sub_418390(char* str, DialogEntryNode* a2, int start);
static void sub_418460(char* str, DialogEntryNode* a2);
static void dialog_copy_class_specific_msg(char* buffer, DialogEntryNode* a2, int num);
static void dialog_copy_race_specific_msg(char* buffer, DialogEntryNode* a2, int num);
static void dialog_copy_generic_msg(char* buffer, DialogEntryNode* a2, int a3, int a4);
static bool dialog_copy_override_msg(char* buffer, DialogEntryNode* a2, int num);
static int sub_4189C0(const char* a1, int a2);
static void sub_418A40(int a1, int a2, int a3, int a4, int a5, DialogEntryNode *a6);
static void sub_418B30(int a1, DialogEntryNode* a2);
static void sub_418C40(int a1, int a2, int a3, DialogEntryNode* a4);
static void sub_418CA0(int* a1, int a2, int a3, DialogEntryNode* a4);
static void sub_418DE0(int a1, DialogEntryNode* a2);
static void sub_418F30(int a1, DialogEntryNode* a2);
static void sub_418FC0(int a1, int* a2, int a3, int a4, DialogEntryNode* a5);
static void sub_4190E0(int a1, int a2, int a3, DialogEntryNode* a4);
static void sub_419190(int a1, int a2, int a3, DialogEntryNode* a4);
static void sub_4191E0(int a1, int a2, DialogEntryNode* a3);
static void sub_419260(DialogEntryNode* a1, const char* a2);
static bool sub_4197D0(unsigned int flags, int a2, DialogEntryNode* a3);
static void sub_419830(int a1, int a2, DialogEntryNode* a3);
static void sub_419A00(int a1, int a2, int a3, DialogEntryNode* a4);
static void sub_419AC0(int a1, int a2, int a3, DialogEntryNode* a4);
static void sub_419B50(int a1, int a2, DialogEntryNode* a3);
static void sub_419C30(int a1, int a2, DialogEntryNode* a3);
static void sub_419CB0(int a1, int a2, int a3, DialogEntryNode* a4);
static void sub_419D50(int a1, int a2, int a3, DialogEntryNode* a4);
static int sub_419E20(int64_t obj, int* a2, int cnt);
static void sub_419E70(const char* str, int a2, int a3, int a4, DialogEntryNode* a5);
static void sub_41A0F0(int a1, int a2, int a3, DialogEntryNode* a4);
static void sub_41A150(int a1, int a2, int a3, DialogEntryNode* a4);
static void sub_41A230(int a1, int a2, int a3, DialogEntryNode* a4);
static void sub_41A290(int a1, int a2, int a3, DialogEntryNode* a4);
static void sub_41A3E0(int a1, DialogEntryNode* a2);
static void sub_41A440(char* buffer, DialogEntryNode* a2);
static void sub_41A520(int a1, DialogEntryNode* a2);
static void sub_41A620(int a1, DialogEntryNode* a2);
static void sub_41A700(int a1, DialogEntryNode* a2);
static void sub_41A880(int a1, int a2, DialogEntryNode* a3);
static void sub_41A8C0(int a1, int a2, int a3, DialogEntryNode* a4);

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
static const char* off_5A06BC[DIALOG_COND_COUNT] = {
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
static const char* off_5A0750[DIALOG_ACTION_COUNT] = {
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

// 0x5D1224
static char dialog_file_tmp_str[MAX_STRING];

// 0x5D19F4
static mes_file_handle_t* dword_5D19F4;

// 0x5D19F8
static size_t dialog_file_tmp_str_size;

// 0x5D19FC
static size_t dialog_file_tmp_str_pos;

// 0x5D1A00
static int dword_5D1A00;

// 0x5D1A04
static int dword_5D1A04;

// 0x5D1A08
static Dialog* dword_5D1A08;

// 0x5D1A0C
static bool dialog_numbers_enabled;

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
    FREE(dword_5D19F4);

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
    int64_t pc_loc;
    int64_t npc_loc;
    int64_t tmp;
    int64_t pc_loc_y;
    int64_t npc_loc_y;

    if (sub_4AD800(a1->npc_obj, a1->pc_obj, 0) != 0) {
        return false;
    }

    sub_4C1020(a1->npc_obj, a1->pc_obj);

    if (critter_is_dead(a1->npc_obj) || sub_4AE120(a1->npc_obj, a1->pc_obj) == 0) {
        if (player_is_pc_obj(a1->pc_obj)) {
            pc_loc = obj_field_int64_get(a1->pc_obj, OBJ_F_LOCATION);
            npc_loc = obj_field_int64_get(a1->npc_obj, OBJ_F_LOCATION);
            location_xy(pc_loc, &tmp, &pc_loc_y);
            location_xy(npc_loc, &tmp, &npc_loc_y);
            if (npc_loc_y > pc_loc_y) {
                location_origin_set(npc_loc);
            } else {
                location_origin_set(pc_loc);
            }
        }

        a1->field_17EC = a1->field_68;
        a1->field_17E8 = 0;
        sub_414E60(a1, 0);
    } else {
        dialog_copy_race_specific_msg(a1->field_70, a1, 1000);
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
        if (sub_4AD800(a1->npc_obj, a1->pc_obj, 0) != 0) {
            a1->field_17E8 = 1;
            return;
        }

        if (a1->field_17E8 == 3) {
            sub_417590(a1->field_17EC, &v1, &v2);
            v3 = 0;
        } else if (sub_415BA0(a1, a1->field_182C[a2], a2)) {
            v1 = a1->field_17F0[a2];
            v2 = a1->field_1804[a2];
            v3 = a1->field_1818[a2];
        } else {
            return;
        }

        if (critter_is_dead(a1->npc_obj) || sub_4AE120(a1->npc_obj, a1->pc_obj) == 0) {
            sub_414810(v1, v2, v3, a2, a1);
        } else {
            dialog_copy_race_specific_msg(a1->field_70, a1, 1000);
            a1->field_17E8 = 4;
        }
    }
}

// 0x413280
void sub_413280(DialogEntryNode* a1)
{
    sub_4C10A0(a1->npc_obj, a1->pc_obj);
}

// 0x4132A0
void sub_4132A0(int64_t a1, int64_t a2, char* buffer)
{
    DialogEntryNode v1;

    if (sub_4AD800(a1, a2, 0) == 0) {
        sub_413360(a1, a2, &v1);
        if (critter_pc_leader_get(a1) == a2) {
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
    a3->pc_obj = a2;
    a3->npc_obj = a1;
    sub_443EB0(a3->npc_obj, &(a3->field_40));
    sub_443EB0(a3->pc_obj, &(a3->field_10));
}

// 0x4133B0
void sub_4133B0(int64_t a1, int64_t a2, char* buffer, int* a4)
{
    DialogEntryNode v1;
    int reaction_level;
    int reaction_type;

    if (sub_4AD800(a1, a2, 0) == 0) {
        sub_413360(a1, a2, &v1);

        reaction_level = reaction_get(a1, a2);
        reaction_type = reaction_translate(reaction_level);
        if (reaction_type < REACTION_SUSPICIOUS) {
            dialog_copy_generic_msg(buffer, &v1, 2000, 2099);
        } else {
            dialog_copy_generic_msg(buffer, &v1, 1900, 1999);
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
        dialog_copy_generic_msg(a3, &v1, 800, 899);
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
        dialog_copy_generic_msg(a3, &v1, 900, 999);
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
        dialog_copy_generic_msg(a3, &v1, 1000, 1099);
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
        dialog_copy_generic_msg(a3, &v1, 1100, 1199);
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
        dialog_copy_generic_msg(a3, &v1, 1200, 1299);
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
        dialog_copy_generic_msg(a3, &v1, 4600, 4699);
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
        dialog_copy_generic_msg(a3, &v1, 1300, 1399);
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
        dialog_copy_generic_msg(a3, &v1, 1400, 1499);
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
        dialog_copy_class_specific_msg(a3, &v1, 2000);
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
        dialog_copy_generic_msg(a3, &v1, 4700, 4799);
    } else {
        a3[0] = '\0';
    }
}

// 0x413A30
void sub_413A30(DialogEntryNode* a1, bool a2)
{
    if (a2 || sub_4AD800(a1->npc_obj, a1->pc_obj, 0) == 0) {
        a1->field_17EC = a1->field_68;
        a1->field_17E8 = 0;
        sub_416840(a1, 0);
    } else {
        a1->field_70[0] = '\0';
        a1->field_458 = -1;
    }
}

// 0x413A90
void sub_413A90(int64_t a1, int64_t a2, int rc, char* a4, int* a5)
{
    DialogEntryNode v1;
    int start;
    int end;

    if (sub_4AD800(a1, a2, 1) == 0) {
        sub_413360(a1, a2, &v1);

        switch (rc) {
        case AI_FOLLOW_TOO_GOOD:
            start = 1600;
            end = 1699;
            break;
        case AI_FOLLOW_TOO_BAD:
            start = 1700;
            end = 1799;
            break;
        case AI_FOLLOW_DISLIKE:
            start = 1800;
            end = 1899;
            break;
        case AI_FOLLOW_ALREADY_IN_GROUP:
            start = 2600;
            end = 2699;
            break;
        case AI_FOLLOW_LIMIT_REACHED:
            start = 2700;
            end = 2799;
            break;
        case AI_FOLLOW_NOT_ALLOWED:
            start = 2800;
            end = 2899;
            break;
        case AI_FOLLOW_LOW_LEVEL:
            start = 2900;
            end = 2999;
            break;
        default:
            a4[0] = '\0';
            *a5 = -1;
            return;
        }

        dialog_copy_generic_msg(a4, &v1, start, end);
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
        dialog_copy_generic_msg(a3, &v1, 2100, 2199);
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
        dialog_copy_generic_msg(a3, &v1, 2200, 2299);
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
        dialog_copy_race_specific_msg(a3, &v1, 1000);
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
        dialog_copy_generic_msg(a3, &v1, 2300, 2399);
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
        dialog_copy_generic_msg(a3, &v1, 3000, 3099);
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

        dialog_copy_generic_msg(a4, &v1, start, end);
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

    dialog_copy_generic_msg(a4, &v1, start, end);
}

// 0x414130
void sub_414130(int64_t a1, int64_t a2, char* a3, int* a4)
{
    DialogEntryNode v1;

    if (sub_4AD800(a1, a2, 1) == 0) {
        sub_413360(a1, a2, &v1);
        dialog_copy_generic_msg(a3, &v1, 4000, 4099);
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
        dialog_copy_generic_msg(a3, &v1, 4100, 4199);
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

        dialog_copy_generic_msg(a4, &v1, start, end);
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

        dialog_copy_generic_msg(a4, &v1, start, end);
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
        dialog_copy_generic_msg(a3, &v1, 5500, 5599);
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
        dialog_copy_generic_msg(a3, &v1, 5700, 5799);
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
        dialog_copy_generic_msg(a3, &v1, 5800, 5899);
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
        dialog_copy_generic_msg(a3, &v1, 5900, 5999);
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
        dialog_copy_generic_msg(a3, &v1, 6000, 6099);
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
        dialog_copy_generic_msg(a3, &v1, 6100, 6199);
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
        a1->field_182C[index] = NULL;
        a1->field_460[index][0] = '\0';
    }

    if (randomize) {
        random_seed(a1->field_1844);
    } else {
        a1->field_1844 = random_seed_generate();
    }

    if (sub_416840(a1, randomize)) {
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
int sub_414F50(DialogEntryNode* a1, int* a2)
{
    DialogEntry key;
    int gender;
    int intelligence;
    Dialog* dialog;
    DialogEntry *entry;
    int idx;
    int cnt;

    key.num = a1->field_17EC;
    gender = stat_level_get(a1->pc_obj, STAT_GENDER);
    intelligence = stat_level_get(a1->pc_obj, STAT_INTELLIGENCE);

    if (intelligence > LOW_INTELLIGENCE
        && critter_is_dumb(a1->pc_obj)) {
        intelligence = 1;
    }

    dialog = &(dword_5D1A08[a1->field_0]);
    entry = bsearch(&key,
        dialog->entries,
        dialog->entries_length,
        sizeof(key),
        dialog_entry_compare);

    if (entry == NULL) {
        return 0;
    }

    cnt = 0;

    for (idx = (entry - dialog->entries) + 1; idx < dialog->entries_length && cnt < 5; idx++) {
        entry = &(dialog->entries[idx]);
        if (entry->iq == 0) {
            return cnt;
        }

        if (entry->data.gender == -1 || entry->data.gender == gender) {
            if ((entry->iq < 0 && intelligence <= -entry->iq)
                || (entry->iq >= 0 && intelligence >= entry->iq)) {
                if (entry->conditions == NULL || sub_4150D0(a1, entry->conditions)) {
                    a2[cnt++] = entry->num;
                }
            }
        }
    }

    return cnt;
}

// 0x4150D0
bool sub_4150D0(DialogEntryNode* a1, char* a2)
{
    char* pch;
    int cond;
    int value;
    ObjectList followers;
    ObjectNode* node;
    int gold;
    int training;
    int level;
    int v39;
    int v40;
    bool inverse;
    int64_t substitute_inventory_obj;
    char code[3];

    if (a2 == NULL || a2[0] == '\0') {
        return true;
    }

    code[2] = '\0';

    pch = a2;
    while (*pch != '\0') {
        while (*pch != '\0' && !isalpha(*pch) && *pch != '$') {
            pch++;
        }

        if (*pch == '\0') {
            break;
        }

        code[0] = *pch++;
        if (*pch == '\0') {
            break;
        }

        code[1] = *pch++;

        value = atoi(pch);
        for (cond = 0; cond < DIALOG_COND_COUNT; cond++) {
            if (strcmpi(off_5A06BC[cond], code) == 0) {
                break;
            }
        }

        switch (cond) {
        case DIALOG_COND_PS:
            if (value < 0) {
                if (basic_skill_level(a1->pc_obj, BASIC_SKILL_PERSUATION) > -value) {
                    return false;
                }
            } else {
                if (basic_skill_level(a1->pc_obj, BASIC_SKILL_PERSUATION) < value) {
                    return false;
                }
            }
            break;
        case DIALOG_COND_CH:
            if (value < 0) {
                if (stat_level_get(a1->pc_obj, STAT_CHARISMA) > -value) {
                    return false;
                }
            } else {
                if (stat_level_get(a1->pc_obj, STAT_CHARISMA) < value) {
                    return false;
                }
            }
            break;
        case DIALOG_COND_PE:
            if (value < 0) {
                if (stat_level_get(a1->pc_obj, STAT_PERCEPTION) > -value) {
                    return false;
                }
            } else {
                if (stat_level_get(a1->pc_obj, STAT_PERCEPTION) < value) {
                    return false;
                }
            }
            break;
        case DIALOG_COND_AL:
            if (value < 0) {
                if (stat_level_get(a1->pc_obj, STAT_ALIGNMENT) > -value) {
                    return false;
                }
            } else {
                if (stat_level_get(a1->pc_obj, STAT_ALIGNMENT) < value) {
                    return false;
                }
            }
            break;
        case DIALOG_COND_MA:
            if (value < 0) {
                if (stat_level_get(a1->pc_obj, STAT_MAGICK_TECH_APTITUDE) > -value) {
                    return false;
                }
            } else {
                if (stat_level_get(a1->pc_obj, STAT_MAGICK_TECH_APTITUDE) < value) {
                    return false;
                }
            }
            break;
        case DIALOG_COND_TA:
            if (value < 0) {
                if (-stat_level_get(a1->pc_obj, STAT_MAGICK_TECH_APTITUDE) > -value) {
                    return false;
                }
            } else {
                if (-stat_level_get(a1->pc_obj, STAT_MAGICK_TECH_APTITUDE) < value) {
                    return false;
                }
            }
            break;
        case DIALOG_COND_GV:
            if (script_gl_var_get(value) != sub_4167C0(pch)) {
                return false;
            }
            break;
        case DIALOG_COND_GF:
            if (script_gl_flag_get(value) != sub_4167C0(pch)) {
                return false;
            }
            break;
        case DIALOG_COND_QU:
            if (sub_4C4CB0(a1->pc_obj, value) != sub_4167C0(pch)) {
                return false;
            }
            break;
        case DIALOG_COND_RE:
            if (value < 0) {
                if (reaction_get(a1->npc_obj, a1->pc_obj) > -value) {
                    return false;
                }
            } else {
                if (reaction_get(a1->npc_obj, a1->pc_obj) < value) {
                    return false;
                }
            }
            break;
        case DIALOG_COND_GOLD:
            gold = item_gold_get(a1->pc_obj);
            object_list_followers(a1->pc_obj, &followers);
            node = followers.head;
            while (node != NULL) {
                gold += item_gold_get(node->obj);
                node = node->next;
            }
            object_list_destroy(&followers);
            if (value < 0) {
                if (gold > value) {
                    return false;
                }
            } else {
                if (gold < value) {
                    return false;
                }
            }
            break;
        case DIALOG_COND_IN:
            if (value < 0) {
                value = -value;
                if (item_find_by_name(a1->npc_obj, value) == OBJ_HANDLE_NULL) {
                    substitute_inventory_obj = critter_substitute_inventory_get(a1->npc_obj);
                    if (substitute_inventory_obj == OBJ_HANDLE_NULL) {
                        return false;
                    }
                    if (item_find_by_name(substitute_inventory_obj, value) == OBJ_HANDLE_NULL) {
                        return false;
                    }
                }
            } else {
                if (!item_find_by_name(a1->pc_obj, value)) {
                    object_list_followers(a1->pc_obj, &followers);
                    node = followers.head;
                    while (node != NULL) {
                        if (item_find_by_name(node->obj, value) == OBJ_HANDLE_NULL) {
                            break;
                        }
                        node = node->next;
                    }
                    object_list_destroy(&followers);
                    if (node == NULL) {
                        return false;
                    }
                }
            }
            break;
        case DIALOG_COND_HA:
            if (value < 0) {
                if (basic_skill_level(a1->pc_obj, BASIC_SKILL_HAGGLE) > -value) {
                    return false;
                }
            } else {
                if (basic_skill_level(a1->pc_obj, BASIC_SKILL_HAGGLE) < value) {
                    return false;
                }
            }
            break;
        case DIALOG_COND_LF:
            if (script_local_flag_get(a1->npc_obj, SAP_DIALOG, value) != sub_4167C0(pch)) {
                return false;
            }
            break;
        case DIALOG_COND_LC:
            if (script_local_counter_get(a1->npc_obj, SAP_DIALOG, value) != sub_4167C0(pch)) {
                return false;
            }
            break;
        case DIALOG_COND_TR:
            training = sub_4167C0(pch);
            if (IS_TECH_SKILL(value)) {
                if (training < 0) {
                    if (tech_skill_get_training(a1->pc_obj, GET_TECH_SKILL(value)) > -training) {
                        return false;
                    }
                } else {
                    if (tech_skill_get_training(a1->pc_obj, GET_TECH_SKILL(value)) < training) {
                        return false;
                    }
                }
            } else {
                if (training < 0) {
                    if (basic_skill_get_training(a1->pc_obj, GET_BASIC_SKILL(value)) > -training) {
                        return false;
                    }
                } else {
                    if (basic_skill_get_training(a1->pc_obj, GET_BASIC_SKILL(value)) < training) {
                        return false;
                    }
                }
            }
            break;
        case DIALOG_COND_SK:
            level = sub_4167C0(pch);
            if (IS_TECH_SKILL(value)) {
                if (level < 0) {
                    if (tech_skill_level(a1->pc_obj, GET_TECH_SKILL(value)) > -level) {
                        return false;
                    }
                } else {
                    if (tech_skill_level(a1->pc_obj, GET_TECH_SKILL(value)) < level) {
                        return false;
                    }
                }
            } else {
                if (level < 0) {
                    if (basic_skill_level(a1->pc_obj, GET_BASIC_SKILL(value)) > -level) {
                        return false;
                    }
                } else {
                    if (basic_skill_level(a1->pc_obj, GET_BASIC_SKILL(value)) < level) {
                        return false;
                    }
                }
            }
            break;
        case DIALOG_COND_RU:
            if (value < 0) {
                if (rumor_known_get(a1->pc_obj, -value)) {
                    return false;
                }
            } else {
                if (!rumor_known_get(a1->pc_obj, value)) {
                    return false;
                }
            }
            break;
        case DIALOG_COND_RQ:
            if (value < 0) {
                if (rumor_qstate_get(-value)) {
                    return false;
                }
            } else {
                if (!rumor_qstate_get(value)) {
                    return false;
                }
            }
            break;
        case DIALOG_COND_FO:
            if (value == 0) {
                if (critter_leader_get(a1->npc_obj) != a1->pc_obj) {
                    return false;
                }
            } else if (value == 1) {
                if (critter_leader_get(a1->npc_obj) == a1->pc_obj) {
                    return false;
                }
            }
            break;
        case DIALOG_COND_LE:
            if (value < 0) {
                if (-stat_level_get(a1->pc_obj, STAT_LEVEL) > -value) {
                    return false;
                }
            } else {
                if (-stat_level_get(a1->pc_obj, STAT_LEVEL) < value) {
                    return false;
                }
            }
            break;
        case DIALOG_COND_QB:
            if (sub_4C4CB0(a1->pc_obj, value) > sub_4167C0(pch)) {
                return false;
            }
            break;
        case DIALOG_COND_ME:
            if (value == 0) {
                if (sub_4C0C40(a1->npc_obj, a1->pc_obj)) {
                    return false;
                }
            } else if (value == 1) {
                if (!sub_4C0C40(a1->npc_obj, a1->pc_obj)) {
                    return false;
                }
            }
            break;
        case DIALOG_COND_NI:
            if (value < 0) {
                value = -value;
                if (item_find_by_name(a1->npc_obj, value) != OBJ_HANDLE_NULL) {
                    return false;
                }

                substitute_inventory_obj = critter_substitute_inventory_get(a1->npc_obj);
                if (substitute_inventory_obj != OBJ_HANDLE_NULL) {
                    if (item_find_by_name(substitute_inventory_obj, value) != OBJ_HANDLE_NULL) {
                        return false;
                    }
                }
            } else {
                if (item_find_by_name(a1->pc_obj, value)) {
                    return false;
                }

                object_list_followers(a1->pc_obj, &followers);
                node = followers.head;
                while (node != NULL) {
                    if (item_find_by_name(node->obj, value)) {
                        break;
                    }
                    node = node->next;
                }
                object_list_destroy(&followers);
                if (node != NULL) {
                    return false;
                }
            }
            break;
        case DIALOG_COND_QA:
            if (sub_4C4CB0(a1->pc_obj, value) < sub_4167C0(pch)) {
                return false;
            }
            break;
        case DIALOG_COND_RA:
            if (value > 0) {
                if (stat_level_get(a1->pc_obj, STAT_RACE) + 1 != value) {
                    return false;
                }
            } else {
                if (stat_level_get(a1->pc_obj, STAT_RACE) + 1 == -value) {
                    return false;
                }
            }
            break;
        case DIALOG_COND_PA:
            if (value < 0) {
                value = -value;
                inverse = true;
            } else {
                inverse = false;
            }
            object_list_followers(a1->pc_obj, &followers);
            node = followers.head;
            while (node != NULL) {
                if ((obj_field_int32_get(node->obj, OBJ_F_SPELL_FLAGS) & OSF_MIND_CONTROLLED) == 0
                    && obj_field_int32_get(node->obj, OBJ_F_NAME) == value) {
                    break;
                }
                node = node->next;
            }
            object_list_destroy(&followers);
            if (!inverse) {
                if (node == NULL) {
                    return false;
                }
            } else {
                if (node != NULL) {
                    return false;
                }
            }
            break;
        case DIALOG_COND_SS:
            if (value < 0) {
                if (sub_445090() > -value) {
                    return false;
                }
            } else {
                if (sub_445090() < value) {
                    return false;
                }
            }
            break;
        case DIALOG_COND_WA:
            if ((obj_field_int32_get(a1->npc_obj, OBJ_F_NPC_FLAGS) & ONF_AI_WAIT_HERE) != 0) {
                if (value == 0) {
                    return false;
                }
            } else {
                if (value == 1) {
                    return false;
                }
            }
            break;
        case DIALOG_COND_WT:
            if ((obj_field_int32_get(a1->npc_obj, OBJ_F_NPC_FLAGS) & ONF_JILTED) != 0) {
                if (value == 0) {
                    return false;
                }
            } else {
                if (value == 1) {
                    return false;
                }
            }
            break;
        case DIALOG_COND_PV:
            if (script_pc_gl_var_get(a1->pc_obj, value) != sub_4167C0(pch)) {
                return false;
            }
            break;
        case DIALOG_COND_PF:
            if (script_pc_gl_flag_get(a1->pc_obj, value) != sub_4167C0(pch)) {
                return false;
            }
            break;
        case DIALOG_COND_NA:
            if (value < 0) {
                if (stat_level_get(a1->pc_obj, STAT_ALIGNMENT) > value) {
                    return false;
                }
            } else {
                if (stat_level_get(a1->pc_obj, STAT_ALIGNMENT) < -value) {
                    return false;
                }
            }
            break;
        case DIALOG_COND_AR:
            if (value > 0) {
                if (!area_is_known(a1->pc_obj, value)) {
                    return false;
                }
            } else {
                if (area_is_known(a1->pc_obj, -value)) {
                    return false;
                }
            }
            break;
        case DIALOG_COND_RP:
            if (value > 0) {
                if (!reputation_has(a1->pc_obj, value)) {
                    return false;
                }
            } else {
                if (reputation_has(a1->pc_obj, -value)) {
                    return false;
                }
            }
            break;
        case DIALOG_COND_IA:
            if (value > 0) {
                if (sub_4CB6A0(a1->pc_obj) != value) {
                    return false;
                }
            } else {
                if (sub_4CB6A0(a1->pc_obj) == -value) {
                    return false;
                }
            }
            break;
        case DIALOG_COND_SC:
            v39 = sub_4167C0(pch);
            v40 = spell_college_level_get(a1->pc_obj, value);
            if (v39 > 0) {
                if (v40 < v39) {
                    return false;
                }
            } else {
                if (v40 > -v39) {
                    return false;
                }
            }
            break;
        default:
            // Unknown condition.
            return false;
        }
    }

    return true;
}

// 0x415BA0
bool sub_415BA0(DialogEntryNode* a1, char* a2, int a3)
{
    char* pch;
    int act;
    int value;
    ObjectList followers;
    ObjectNode* node;
    bool v57 = true;
    bool v59 = false;
    char code[3];

    if (a2 == NULL || a2[0] == '\0') {
        return true;
    }

    code[2] = '\0';

    pch = a2;
    while (*pch != '\0') {
        while (*pch != '\0' && !isalpha(*pch) && *pch != '$') {
            pch++;
        }

        if (*pch == '\0') {
            break;
        }

        code[0] = *pch++;
        if (*pch == '\0') {
            break;
        }

        code[1] = *pch++;

        value = atoi(pch);
        for (act = 0; act < DIALOG_ACTION_COUNT; act++) {
            if (strcmpi(off_5A0750[act], code) == 0) {
                break;
            }
        }

        switch (act) {
        case DIALOG_ACTION_GOLD:
            if (value > 0) {
                item_gold_transfer(OBJ_HANDLE_NULL, a1->pc_obj, value, OBJ_HANDLE_NULL);
            } else if (value < 0) {
                int total_gold;

                value = -value;

                total_gold = item_gold_get(a1->pc_obj);
                item_gold_transfer(a1->pc_obj, a1->npc_obj, value, OBJ_HANDLE_NULL);

                value -= total_gold;
                if (value > 0) {
                    object_list_followers(a1->pc_obj, &followers);
                    node = followers.head;
                    while (node != NULL) {
                        if (value <= 0) {
                            break;
                        }

                        total_gold = item_gold_get(node->obj);
                        item_gold_transfer(node->obj, a1->npc_obj, value, OBJ_HANDLE_NULL);

                        node = node->next;
                    }
                    object_list_destroy(&followers);
                }
            }
            break;
        case DIALOG_ACTION_RE: {
            int reaction;

            reaction = reaction_get(a1->npc_obj, a1->pc_obj);

            while (isspace(*pch)) {
                pch++;
            }

            if (*pch == '+' || *pch == '-') {
                reaction_adj(a1->npc_obj, a1->pc_obj, value);
            } else if (*pch == '>') {
                if (reaction < value) {
                    reaction_adj(a1->npc_obj, a1->pc_obj, value - reaction);
                }
            } else if (*pch == '<') {
                if (reaction > value) {
                    reaction_adj(a1->npc_obj, a1->pc_obj, value - reaction);
                }
            } else {
                reaction_adj(a1->npc_obj, a1->pc_obj, value - reaction);
            }

            break;
        }
        case DIALOG_ACTION_QU:
            sub_4C4D20(a1->pc_obj, value, sub_4167C0(pch), a1->npc_obj);
            break;
        case DIALOG_ACTION_FL:
            a1->field_68 = value;
            sub_413A30(a1, false);
            a1->field_17E8 = 4;
            v57 = false;
            break;
        case DIALOG_ACTION_CO:
            v59 = true;
            break;
        case DIALOG_ACTION_GV:
            script_gl_var_set(value, sub_4167C0(pch));
            break;
        case DIALOG_ACTION_GF:
            script_gl_flag_set(value, sub_4167C0(pch));
            break;
        case DIALOG_ACTION_MM:
            area_set_known(a1->pc_obj, value);
            break;
        case DIALOG_ACTION_AL: {
            int alignment;

            alignment = stat_base_get(a1->pc_obj, STAT_ALIGNMENT);

            while (isspace(*pch)) {
                pch++;
            }

            if (*pch == '+' || *pch == '-') {
                stat_base_set(a1->pc_obj, STAT_ALIGNMENT, alignment + value);
            } else if (*pch == '>') {
                if (alignment < value) {
                    stat_base_set(a1->pc_obj, STAT_ALIGNMENT, value);
                }
            } else if (*pch == '<') {
                if (alignment > value) {
                    stat_base_set(a1->pc_obj, STAT_ALIGNMENT, value);
                }
            } else {
                stat_base_set(a1->pc_obj, STAT_ALIGNMENT, value);
            }

            break;
        }
        case DIALOG_ACTION_IN: {
            int64_t item_obj;
            int64_t substitute_inventory_obj;
            int64_t parent_obj;

            if (value < 0) {
                value = -value;

                item_obj = item_find_by_name(a1->npc_obj, value);
                if (item_obj == OBJ_HANDLE_NULL) {
                    substitute_inventory_obj = critter_substitute_inventory_get(a1->npc_obj);
                    if (substitute_inventory_obj != OBJ_HANDLE_NULL) {
                        item_obj = item_find_by_name(substitute_inventory_obj, value);
                    }
                }

                if (item_obj != OBJ_HANDLE_NULL) {
                    item_transfer(item_obj, a1->pc_obj);
                    item_parent(item_obj, &parent_obj);
                    if (parent_obj != a1->pc_obj) {
                        item_remove(item_obj);
                        sub_466E50(item_obj, obj_field_int64_get(a1->pc_obj, OBJ_F_LOCATION));
                    }
                }
            } else {
                item_obj = item_find_by_name(a1->pc_obj, value);
                if (item_obj == OBJ_HANDLE_NULL) {
                    object_list_followers(a1->pc_obj, &followers);
                    node = followers.head;
                    while (node != NULL) {
                        item_obj = item_find_by_name(node->obj, value);
                        if (item_obj != OBJ_HANDLE_NULL) {
                            break;
                        }
                        node = node->next;
                    }
                    object_list_destroy(&followers);
                }

                if (item_obj != OBJ_HANDLE_NULL) {
                    item_transfer(item_obj, a1->npc_obj);
                    item_parent(item_obj, &parent_obj);
                    if (parent_obj != a1->npc_obj) {
                        if (critter_pc_leader_get(a1->npc_obj) != OBJ_HANDLE_NULL) {
                            item_remove(item_obj);
                            sub_466E50(item_obj, obj_field_int64_get(a1->npc_obj, OBJ_F_LOCATION));
                        } else {
                            object_destroy(item_obj);
                        }
                    }
                }
            }
            break;
        }
        case DIALOG_ACTION_LF:
            script_local_flag_set(a1->npc_obj, SAP_DIALOG, value, sub_4167C0(pch));
            break;
        case DIALOG_ACTION_LC:
            script_local_counter_set(a1->npc_obj, SAP_DIALOG, value, sub_4167C0(pch));
            break;
        case DIALOG_ACTION_TR: {
            int training;

            training = sub_4167C0(pch);
            if (IS_TECH_SKILL(value)) {
                tech_skill_set_training(a1->pc_obj, GET_TECH_SKILL(value), training);
            } else {
                basic_skill_set_training(a1->pc_obj, GET_BASIC_SKILL(value), training);
            }
            break;
        }
        case DIALOG_ACTION_RU:
            rumor_known_set(a1->pc_obj, value);
            break;
        case DIALOG_ACTION_RQ:
            rumor_qstate_set(value);
            break;
        case DIALOG_ACTION_JO: {
            int v41;
            int rc;

            v41 = sub_4167C0(pch);
            rc = ai_check_follow(a1->npc_obj, a1->pc_obj, value);
            if (rc == AI_FOLLOW_OK) {
                critter_follow(a1->npc_obj, a1->pc_obj, value);
                a1->field_17EC = v41;
                a1->field_17E8 = 0;
                sub_414E60(a1, false);
                v57 = false;
            } else {
                sub_413A90(a1->npc_obj, a1->pc_obj, rc, a1->field_70, &(a1->field_458));
                a1->field_45C = 1;
                sub_4182D0(a1->field_460[0], a1, 600, 699);
                a1->field_17F0[0] = a1->field_17F0[a3];
                a1->field_1804[0] = a1->field_1804[a3];
                a1->field_182C[0] = 0;
                v57 = false;
            }
            break;
        }
        case DIALOG_ACTION_WA:
            sub_4AA7A0(a1->npc_obj);
            break;
        case DIALOG_ACTION_LV:
            critter_disband(a1->npc_obj, true);
            break;
        case DIALOG_ACTION_SS:
            sub_4450A0(value);
            break;
        case DIALOG_ACTION_SC:
            critter_spread_out_disable(a1->npc_obj);
            break;
        case DIALOG_ACTION_SO:
            critter_spread_out_enable(a1->npc_obj);
            break;
        case DIALOG_ACTION_UW: {
            int v43;
            int rc;

            v43 = sub_4167C0(pch);
            rc = ai_check_follow(a1->npc_obj, a1->pc_obj, value);
            if (rc == AI_FOLLOW_OK) {
                sub_4AA8C0(a1->npc_obj, value);
                a1->field_17EC = v43;
                a1->field_17E8 = 0;
                sub_414E60(a1, false);
                v57 = false;
            } else {
                sub_413A90(a1->npc_obj, a1->pc_obj, rc, a1->field_70, &(a1->field_458));
                a1->field_45C = 1;
                sub_4182D0(a1->field_460[0], a1, 600, 699);
                a1->field_17F0[0] = a1->field_17F0[a3];
                a1->field_1804[0] = a1->field_1804[a3];
                a1->field_182C[0] = NULL;
                v57 = false;
            }
            break;
        }
        case DIALOG_ACTION_PV:
            script_pc_gl_var_set(a1->pc_obj, value, sub_4167C0(pch));
            break;
        case DIALOG_ACTION_PF:
            script_pc_gl_flag_set(a1->pc_obj, value, sub_4167C0(pch));
            break;
        case DIALOG_ACTION_XP:
            critter_give_xp(a1->pc_obj, quest_get_xp(value));
            break;
        case DIALOG_ACTION_NK:
            critter_kill(a1->npc_obj);
            break;
        case DIALOG_ACTION_RP:
            if (value > 0) {
                reputation_add(a1->pc_obj, value);
            } else {
                reputation_remove(a1->pc_obj, -value);
            }
            break;
        case DIALOG_ACTION_NP:
            newspaper_queue(value, sub_4167C0(pch));
            break;
        case DIALOG_ACTION_CE:
            sub_413BE0(a1->npc_obj, a1->pc_obj, a1->field_70, &(a1->field_458));
            a1->field_17E8 = 5;
            v57 = false;
            break;
        case DIALOG_ACTION_FP:
            stat_base_set(a1->pc_obj,
                STAT_FATE_POINTS,
                stat_base_get(a1->pc_obj, STAT_FATE_POINTS) + 1);
            break;
        case DIALOG_ACTION_SU:
            sub_413BE0(a1->npc_obj, a1->pc_obj, a1->field_70, &(a1->field_458));
            a1->field_17E8 = 7;
            v57 = false;
            break;
        case DIALOG_ACTION_OR:
            critter_origin_set(a1->npc_obj, value);
            break;
        case DIALOG_ACTION_II:
            sub_413BE0(a1->npc_obj, a1->pc_obj, a1->field_70, &(a1->field_458));
            a1->field_17E8 = 8;
            v57 = false;
            break;
        case DIALOG_ACTION_RI:
            sub_413BE0(a1->npc_obj, a1->pc_obj, a1->field_70, &(a1->field_458));
            a1->field_17E8 = 9;
            v57 = false;
            break;
        case DIALOG_ACTION_ET: {
            int v50;
            int training;
            int level;

            v50 = sub_4167C0(pch);
            if (IS_TECH_SKILL(value)) {
                level = tech_skill_level(a1->pc_obj, GET_TECH_SKILL(value));
                training = tech_skill_level(a1->pc_obj, GET_TECH_SKILL(value));
            } else {
                level = basic_skill_level(a1->pc_obj, GET_BASIC_SKILL(value));
                training = basic_skill_level(a1->pc_obj, GET_BASIC_SKILL(value));
            }

            if (level < sub_4C69C0(TECH_SKILL_PICK_LOCKS)) {
                sub_413A90(a1->npc_obj, a1->pc_obj, 34, a1->field_70, &(a1->field_458));
                dialog_copy_generic_msg(a1->field_70, a1, 6200, 6299);
                a1->field_45C = 1;
                sub_4182D0(a1->field_460[0], a1, 600, 699);
                a1->field_17F0[0] = a1->field_17F0[a3];
                a1->field_1804[0] = a1->field_1804[a3];
                a1->field_182C[0] = NULL;
                v57 = false;
            } else if (training < TRAINING_APPRENTICE) {
                sub_413A90(a1->npc_obj, a1->pc_obj, 34, a1->field_70, &(a1->field_458));
                dialog_copy_generic_msg(a1->field_70, a1, 6300, 6399);
                a1->field_45C = 1;
                sub_4182D0(a1->field_460[0], a1, 600, 699);
                a1->field_17F0[0] = a1->field_17F0[a3];
                a1->field_1804[0] = a1->field_1804[a3];
                a1->field_182C[0] = NULL;
                v57 = false;
            } else {
                a1->field_17EC = v50;
                a1->field_17E8 = 0;
                sub_414E60(a1, false);
                v57 = false;
            }
            break;
        }
        default:
            // Unknown action.
            return v57;
        }
    }

    if (v59) {
        sub_4A9650(a1->pc_obj, a1->npc_obj, COMBAT_WEAPON_LOUDNESS_NORMAL, 0);
    }

    return v57;
}

// 0x4167C0
int sub_4167C0(const char* str)
{
    while (isspace(*str)) {
        str++;
    }

    while (isdigit(*str)) {
        str++;
    }

    return atoi(str);
}

// 0x416840
bool sub_416840(DialogEntryNode* a1, bool a2)
{
    DialogEntry v1;

    v1.num = a1->field_17EC;
    if (!sub_416AB0(a1->field_0, &v1)) {
        a1->field_458 = -1;
        a1->field_70[0] = ' ';
        a1->field_70[1] = '\0';
        return false;
    }

    a1->field_1840 = v1.response_val;
    a1->field_458 = sub_4189C0(v1.conditions, a1->field_6C);

    if (!a2) {
        sub_415BA0(a1, v1.actions, 0);
    }

    if (strnicmp(v1.str, "g:", 2) == 0) {
        sub_419260(a1, &(v1.str[2]));

        if (a1->field_17E8) {
            a1->field_45C = 0;
            return false;
        }
        return true;
    }

    if (strcmpi(v1.str, "i:") == 0) {
        dialog_copy_race_specific_msg(a1->field_70, a1, 1000);
        return true;
    }

    if (strnicmp(v1.str, "m:", 2) == 0) {
        char* pch;
        int v2;
        int v3;
        int v4;
        int v5;
        int v6;
        int v7;

        pch = strchr(v1.str, '$') + 1;
        v2 = atoi(pch);

        pch = strchr(pch, ',') + 1;
        v3 = atoi(pch);

        sub_417590(v3, &v4, &v5);
        sub_417590(v1.response_val, &v6, &v7);
        sub_418A40(v2, v4, v5, v6, v7, a1);

        return false;
    }

    if (strnicmp(v1.str, "r:", 2) == 0) {
        char* pch;
        int v8;
        int v9;
        int v10[100];

        pch = strchr(v1.str, '$') + 1;
        v8 = atoi(pch);

        pch = strchr(pch, ',');
        v9 = sub_417F90(v10, pch + 1);

        sub_418FC0(v8, v10, v9, v1.response_val, a1);
        return false;
    }

    if (obj_type_is_critter(obj_field_int32_get(a1->pc_obj, OBJ_F_TYPE))
        && stat_level_get(a1->pc_obj, STAT_GENDER) != GENDER_MALE) {
        sub_416B00(a1->field_70, v1.data.female_str, a1);
    } else {
        sub_416B00(a1->field_70, v1.str, a1);
    }

    return true;
}

// 0x416AB0
bool sub_416AB0(int dlg, DialogEntry* a2)
{
    DialogEntry* v1;

    v1 = bsearch(a2,
        dword_5D1A08[dlg].entries,
        dword_5D1A08[dlg].entries_length,
        sizeof(*a2),
        dialog_entry_compare);
    if (v1 == NULL) {
        return false;
    }

    *a2 = *v1;

    return true;
}

// 0x416B00
void sub_416B00(char* dst, char* src, DialogEntryNode* a3)
{
    char* remainder;
    char* start;
    char* end;

    remainder = src;
    *dst = '\0';

    start = strchr(src, '@');
    while (start != NULL) {
        *start = '\0';
        strcat(dst, remainder);
        end = strchr(start + 1, '@');
        *end = '\0';
        if (strcmpi(start + 1, "pcname") == 0) {
            sub_441B60(a3->pc_obj, OBJ_HANDLE_NULL, dst + strlen(dst));
        } else if (strcmpi(start + 1, "npcname") == 0) {
            sub_441B60(a3->pc_obj, a3->npc_obj, dst + strlen(dst));
        }

        *start = '@';
        *end = '@';
        start = strchr(end + 1, '@');
    }

    strcat(dst, remainder);
}

// 0x416C10
bool sub_416C10(int a1, int a2, DialogEntryNode* a3)
{
    DialogEntry v1;
    char* pch;
    int values[100];
    int cnt;
    int v2;
    int v3;
    int v4;

    v1.num = a1;
    sub_416AB0(a3->field_0, &v1);

    if (strcmpi(v1.str, "a:") == 0) {
        sub_418390(a3->field_460[a2], a3, 1000);
        sub_417590(v1.response_val, &(a3->field_17F0[a2]), &(a3->field_1804[a2]));
    } else if (strcmpi(v1.str, "b:") == 0) {
        if (critter_leader_get(a3->npc_obj) == a3->pc_obj) {
            sub_4182D0(a3->field_460[a2], a3, 1600, 1699);
        } else {
            sub_4182D0(a3->field_460[a2], a3, 300, 399);
        }
        a3->field_17F0[a2] = 3;
        a3->field_1804[a2] = v1.response_val;
    } else if (strnicmp(v1.str, "c:", 2) == 0) {
        sub_418460(a3->field_460[a2], a3);
        a3->field_17F0[a2] = 24;
        a3->field_1804[a2] = v1.response_val;
    } else if (strnicmp(v1.str, "d:", 2) == 0) {
        pch = strchr(v1.str, ',');
        cnt = sub_417F90(values, pch + 1);
        if (!sub_419E20(a3->pc_obj, values, cnt)) {
            return false;
        }
        sub_4182D0(a3->field_460[a2], a3, 1300, 1399);

        pch = a3->field_460[a2];
        strcpy(&(pch[strlen(pch) + 1]), v1.str + 2);
        a3->field_17F0[a2] = 18;
        a3->field_1804[a2] = v1.response_val;
        a3->field_1818[a2] = 0;
    } else if (strcmpi(v1.str, "e:") == 0) {
        sub_4182D0(a3->field_460[a2], a3, 400, 499);
        sub_417590(v1.response_val, &(a3->field_17F0[a2]), &(a3->field_1804[a2]));
    } else if (strcmpi(v1.str, "f:") == 0) {
        sub_4182D0(a3->field_460[a2], a3, 800, 899);
        sub_417590(v1.response_val, &(a3->field_17F0[a2]), &(a3->field_1804[a2]));
    } else if (strcmpi(v1.str, "h:") == 0) {
        sub_4182D0(a3->field_460[a2], a3, 700, 799);
        a3->field_17F0[a2] = 11;
        a3->field_1804[a2] = v1.response_val;
    } else if (strcmpi(v1.str, "i:") == 0) {
        sub_417590(v1.response_val, &v2, &v3);
        sub_419190(a2, v2, v3, a3);
    } else if (strcmpi(v1.str, "k:") == 0) {
        sub_4182D0(a3->field_460[a2], a3, 1500, 1599);
        sub_417590(v1.response_val, &(a3->field_17F0[a2]), &(a3->field_1804[a2]));
    } else if (strcmpi(v1.str, "l:") == 0) {
        sub_4182D0(a3->field_460[a2], a3, 2300, 2399);
        a3->field_17F0[a2] = 30;
        a3->field_1804[a2] = v1.response_val;
    } else if (strcmpi(v1.str, "n:") == 0) {
        sub_4182D0(a3->field_460[a2], a3, 100, 199);
        sub_417590(v1.response_val, &(a3->field_17F0[a2]), &(a3->field_1804[a2]));
    } else if (strcmpi(v1.str, "p:") == 0) {
        sub_4182D0(a3->field_460[a2], a3, 1900, 1999);
        a3->field_17F0[a2] = 25;
        a3->field_1804[a2] = v1.response_val;
    } else if (strnicmp(v1.str, "q:", 2) == 0) {
        v4 = sub_4C4C00(a3->pc_obj, a3->npc_obj, atoi(v1.str + 2));
        if (v4 != -1) {
            return sub_416C10(v4, a2, a3);
        }
        return false;
    } else if (strnicmp(v1.str, "r:", 2) == 0) {
        sub_418390(a3->field_460[a2], a3, 2000);

        pch = a3->field_460[a2];
        strcpy(&(pch[strlen(pch) + 1]), v1.str + 2);
        a3->field_17F0[a2] = 8;
        a3->field_1804[a2] = v1.response_val;
    } else if (strcmpi(v1.str, "s:") == 0) {
        sub_4182D0(a3->field_460[a2], a3, 200, 299);
        sub_417590(v1.response_val, &(a3->field_17F0[a2]), &(a3->field_1804[a2]));
    } else if (strnicmp(v1.str, "r:", 2) == 0) {
        sub_4182D0(a3->field_460[a2], a3, 500, 599);

        pch = a3->field_460[a2];
        strcpy(&(pch[strlen(pch) + 1]), v1.str + 2);
        a3->field_17F0[a2] = 5;
        a3->field_1804[a2] = v1.response_val;
    } else if (strnicmp(v1.str, "u:", 2) == 0) {
        v4 = atoi(v1.str + 2);
        if (sub_4AE570(a3->npc_obj, a3->pc_obj, sub_4C91F0(a3->npc_obj, v4), v4)) {
            return false;
        }

        sub_419A00(a2, v4, v1.response_val, a3);
    } else if (strcmpi(v1.str, "w:") == 0) {
        sub_4182D0(a3->field_460[a2], a3, 1800, 1899);
        sub_417590(v1.response_val, &(a3->field_17F0[a2]), &(a3->field_1804[a2]));
    } else if (strnicmp(v1.str, "x:", 2) == 0) {
        pch = strchr(v1.str, ',');
        cnt = sub_417F90(values, pch + 1);
        if (!sub_419E20(a3->pc_obj, values, cnt)) {
            return false;
        }

        sub_4182D0(a3->field_460[a2], a3, 1400, 1499);
        pch = a3->field_460[a2];
        strcpy(&(pch[strlen(pch) + 1]), v1.str + 2);
        a3->field_17F0[a2] = 21;
        a3->field_1804[a2] = v1.response_val;
        a3->field_1818[a2] = 0;
    } else if (strcmpi(v1.str, "y:") == 0) {
        sub_4182D0(a3->field_460[a2], a3, 1, 99);
        sub_417590(v1.response_val, &(a3->field_17F0[a2]), &(a3->field_1804[a2]));
    } else if (strnicmp(v1.str, "z:", 2) == 0) {
        sub_419AC0(a2, atoi(v1.str + 2), v1.response_val, a3);
    } else {
        sub_416B00(a3->field_460[a2], v1.str, a3);
        sub_417590(v1.response_val, &(a3->field_17F0[a2]), &(a3->field_1804[a2]));
    }

    a3->field_182C[a2] = v1.actions;

    if (dialog_numbers_enabled) {
        char str[20];
        size_t len;
        size_t pos;

        sprintf(str, "[%d]", v1.num);
        len = strlen(str);
        for (pos = 999; pos >= len; pos--) {
            a3->field_460[a2][pos] = a3->field_460[a2][pos - len];
        }
        if (len > 0) {
            strncpy(a3->field_460[a2], str, len);
        }
    }

    return true;
}

// 0x417590
void sub_417590(int a1, int* a2, int* a3)
{
    if (a1 > 0) {
        *a2 = 0;
        *a3 = a1;
    } else if (a1 < 0) {
        *a2 = 2;
        *a3 = -1;
    } else {
        *a2 = 1;
    }
}

// 0x4175D0
bool sub_4175D0(const char* path, int* index_ptr)
{
    int candidate = -1;
    int index;

    for (index = 0; index < dword_5D1A04; index++) {
        if (strcmpi(path, dword_5D1A08[index].path) == 0) {
            *index_ptr = index;
            return true;
        }

        if (dword_5D1A08[index].refcount == 0) {
            if (candidate == -1) {
                candidate = index;
            } else if (dword_5D1A08[candidate].path[0] != '\0') {
                if (dword_5D1A08[index].path[0] == '\0'
                    || datetime_compare(&(dword_5D1A08[candidate].timestamp), &(dword_5D1A08[index].timestamp)) > 0) {
                    candidate = index;
                }
            }
        }
    }

    if (candidate != -1) {
        *index_ptr = candidate;
        if (dword_5D1A08[candidate].path[0] != '\0') {
            sub_412F60(index);
        }
        return false;
    }

    *index_ptr = index;

    dword_5D1A04 += 10;
    dword_5D1A08 = (Dialog*)REALLOC(dword_5D1A08, sizeof(*dword_5D1A08) * dword_5D1A04);
    while (index < dword_5D1A04) {
        dword_5D1A08[index].refcount = 0;
        dword_5D1A08[index].path[0] = '\0';
        index++;
    }

    return false;
}

// 0x417720
void sub_417720(Dialog* dialog)
{
    TigFile* stream;
    DialogEntry v1;
    char v2[1000];
    char v3[1000];
    char v4[1000];
    char v5[1000];
    int line;

    stream = dialog_file_fopen(dialog->path, "rt");
    if (stream == NULL) {
        return;
    }

    v1.data.female_str = v2;
    v1.conditions = v3;
    v1.str = v4;
    v1.actions = v5;

    line = 1;
    while (sub_417860(stream, &v1, &line)) {
        if (dialog->entries_length == dialog->entries_capacity) {
            dialog->entries_capacity += 10;
            dialog->entries = (DialogEntry*)REALLOC(dialog->entries, sizeof(*dialog->entries) * dialog->entries_capacity);
        }

        sub_417E20(&(dialog->entries[dialog->entries_length]), &v1);
        dialog->entries_length++;

        v1.data.female_str = v2;
    }

    dialog_file_fclose(stream);

    if (dialog->entries_length != 0) {
        qsort(dialog->entries,
            dialog->entries_length,
            sizeof(*dialog->entries),
            dialog_entry_compare);
    }
}

// 0x417860
bool sub_417860(TigFile* stream, DialogEntry* entry, int* line_ptr)
{
    char str1[1000];
    char str2[1000];

    if (!sub_417C20(stream, str1, line_ptr)) {
        return false;
    }
    entry->num = atoi(str1);

    if (!sub_417C20(stream, str1, line_ptr)) {
        tig_debug_printf("Missing text on line: %d (dialog line %d)\n", *line_ptr, entry->num);
        return false;
    }
    strcpy(entry->str, str1);

    if (!sub_417C20(stream, str2, line_ptr)) {
        tig_debug_printf("Missing gender field on line: %d (dialog line %d)\n", *line_ptr, entry->num);
        return false;
    }

    if (!sub_417C20(stream, str1, line_ptr)) {
        tig_debug_printf("Missing minimum IQ value on line: %d (dialog line %d)\n", *line_ptr, entry->num);
        return false;
    }

    entry->iq = atoi(str1);
    if (entry->iq == 0 && str1[0] != '\0') {
        tig_debug_printf("Invalid minimum IQ value on line: %d (dialog line %d). Must be blank (for an NPC) or non-zero (for a PC).\n", *line_ptr, entry->num);
        return false;
    }

    if (!sub_417C20(stream, str1, line_ptr)) {
        tig_debug_printf("Missing test field on line: %d (dialog line %d)\n", *line_ptr, entry->num);
        return false;
    }
    strcpy(entry->conditions, str1);

    if (!sub_417C20(stream, str1, line_ptr)) {
        tig_debug_printf("Missing response value on line: %d (dialog line %d)\n", *line_ptr, entry->num);
        return false;
    }

    if (str1[0] == '#') {
        tig_debug_printf("Saw a # in a response value on line: %d (dialog line %d)\n", *line_ptr, entry->num);
        return false;
    }
    entry->response_val = atoi(str1);

    if (!sub_417C20(stream, str1, line_ptr)) {
        tig_debug_printf("Missing effect field on line: %d (dialog line %d)\n", *line_ptr, entry->num);
        return false;
    }
    strcpy(entry->actions, str1);

    if (entry->iq) {
        if (str2[0] != '\0') {
            entry->data.gender = atoi(str2);
        } else {
            entry->data.gender = -1;
        }
    } else {
        size_t pos;
        size_t end;

        strcpy(entry->data.female_str, str2);

        pos = 0;
        end = strlen(str2);
        while (pos < end) {
            if (!isspace(str2[pos])) {
                break;
            }
            pos++;
        }

        if (pos == end) {
            pos = 0;
            end = strlen(entry->str);
            while (pos < end) {
                if (!isspace(entry->str[pos])) {
                    break;
                }
                pos++;
            }

            if (pos != end) {
                tig_debug_printf("Missing NPC response line for females: %d (dialog line %d)\n", *line_ptr, entry->num);
            }
        }
    }

    return true;
}

// 0x417C20
bool sub_417C20(TigFile* stream, char* str, int* line_ptr)
{
    int prev = 0;
    int ch;
    int len;
    bool overflow;

    ch = dialog_file_fgetc(stream);
    while (ch != '{') {
        if (ch == EOF) {
            return false;
        }

        if (ch == '\n') {
            (*line_ptr)++;
        } else if (ch == '}') {
            tig_debug_printf("Warning! Possible missing left brace { on or before line %d\n", *line_ptr);
        } else if (ch == '/' && prev == '/') {
            do {
                ch = dialog_file_fgetc(stream);
                if (ch == EOF) {
                    return false;
                }
            } while (ch != '\n');
            prev = 0;
            (*line_ptr)++;
        } else {
            prev = ch;
        }

        ch = dialog_file_fgetc(stream);
    }

    len = 0;
    overflow = false;

    ch = dialog_file_fgetc(stream);
    while (ch != '}') {
        if (ch == EOF) {
            tig_debug_printf("Expected right bracket }, reached end of file.\n");
            return false;
        }

        if (ch == '\n') {
            (*line_ptr)++;
        } else if (ch == '{') {
            tig_debug_printf("Warning! Possible missing right brace } on or before line %d\n", *line_ptr);
        }

        if (len < 999) {
            str[len++] = (char)(unsigned char)ch;
        } else {
            overflow = true;
        }

        ch = dialog_file_fgetc(stream);
    }

    str[len] = '\0';

    if (overflow) {
        tig_debug_printf("String too long on line: %d\n", *line_ptr);
    }

    return true;
}

// 0x417D60
TigFile* dialog_file_fopen(const char* fname, const char* mode)
{
    dialog_file_tmp_str_pos = 0;
    dialog_file_tmp_str_size = 0;
    return tig_file_fopen(fname, mode);
}

// 0x417D80
int dialog_file_fclose(TigFile* stream)
{
    return tig_file_fclose(stream);
}

// 0x417D90
int dialog_file_fgetc(TigFile* stream)
{
    if (dialog_file_tmp_str_pos != dialog_file_tmp_str_size) {
        return dialog_file_tmp_str[dialog_file_tmp_str_pos++];
    }

    dialog_file_tmp_str_pos = 0;
    dialog_file_tmp_str_size = 0;

    if (tig_file_fgets(dialog_file_tmp_str, sizeof(dialog_file_tmp_str), stream) == NULL) {
        return EOF;
    }

    dialog_file_tmp_str_size = strlen(dialog_file_tmp_str);
    if (dialog_file_tmp_str_size == 0) {
        return EOF;
    }

    return dialog_file_tmp_str[dialog_file_tmp_str_pos++];
}

// 0x417E00
int dialog_entry_compare(const void* va, const void* vb)
{
    const DialogEntry* a = (const DialogEntry*)va;
    const DialogEntry* b = (const DialogEntry*)vb;

    if (a->num < b->num) {
        return -1;
    } else if (a->num > b->num) {
        return 1;
    } else {
        return 0;
    }
}

// 0x417E20
void sub_417E20(DialogEntry* a1, const DialogEntry* a2)
{
    size_t pos;
    size_t end;

    *a1 = *a2;
    a1->str = STRDUP(a2->str);

    pos = 0;
    end = strlen(a2->conditions);
    while (pos < end) {
        if (!isspace(a2->conditions[pos])) {
            break;
        }
        pos++;
    }

    if (pos != end) {
        a1->conditions = STRDUP(a2->conditions);
    } else {
        a1->conditions = NULL;
    }

    pos = 0;
    end = strlen(a2->actions);
    while (pos < end) {
        if (!isspace(a2->actions[pos])) {
            break;
        }
        pos++;
    }

    if (pos != end) {
        a1->actions = STRDUP(a2->actions);
    } else {
        a1->actions = NULL;
    }

    if (!a2->iq) {
        a1->data.female_str = STRDUP(a2->data.female_str);
    }
}

// 0x417F40
void sub_417F40(DialogEntry* a1)
{
    if (!a1->iq) {
        FREE(a1->data.female_str);
    }
    FREE(a1->str);

    if (a1->conditions != NULL) {
        FREE(a1->conditions);
    }

    if (a1->actions != NULL) {
        FREE(a1->actions);
    }
}

// 0x417F90
int sub_417F90(int* values, char* str)
{
    int cnt;
    char* comma;
    char* dash;
    int start;
    int end;

    for (cnt = 0; cnt < 100; cnt++) {
        comma = strchr(str, ',');
        if (comma != NULL) {
            *comma = '\0';
        }

        dash = strchr(str, '-');
        if (dash != NULL) {
            *dash = '\0';
            start = atoi(str);
            end = atoi(dash + 1);
            while (start <= end && cnt < 100) {
                values[cnt++] = start;
                start++;
            }
        } else {
            values[cnt++] = atoi(str);
        }

        if (comma == NULL) {
            break;
        }

        *comma = ',';
        str = comma + 1;
    }

    return cnt;
}

// 0x418030
void dialog_check()
{
    TigFileList file_list;
    unsigned int index;
    char path[80];
    int dlg;
    int entry_index;
    DialogEntry* v2;
    int overflow;
    DialogEntry tmp;

    tig_file_list_create(&file_list, "dlg\\*.dlg");
    for (index = 0; index < file_list.count; index++) {
        sprintf(path, "dlg\\%s", file_list.entries[index].path);
        tig_debug_printf("Checking dialog file %s\n", path);
        if (sub_412E10(path, &dlg)) {
            for (entry_index = 0; entry_index < dword_5D1A08[dlg].entries_length; entry_index++) {
                v2 = &(dword_5D1A08[dlg].entries[entry_index]);
                if (v2->iq) {
                    overflow = tc_check_size(v2->str);
                    if (overflow > 0) {
                        tig_debug_printf("PC response %d too long by %d pixels\n", v2->num, overflow);
                    }

                    if (v2->response_val > 0) {
                        tmp.num = v2->response_val;
                        if (sub_416AB0(dlg, &tmp)) {
                            if (tmp.iq) {
                                tig_debug_printf("PC response %d jumps to non-PC line %d\n", v2->num, v2->response_val);
                            }
                        } else {
                            tig_debug_printf("PC response %d jumps to non-existent dialog line %d\n", v2->num, v2->response_val);
                        }
                    }
                }
            }
            sub_412F40(dlg);
        }
    }
    tig_file_list_destroy(&file_list);

    dialog_check_generated(GD_PC2M);
    dialog_check_generated(GD_PC2F);
    dialog_check_generated(GD_CLS_PC2M);
    dialog_check_generated(GD_CLS_PC2F);
    dialog_check_generated(GD_DUMB_PC2M);
    dialog_check_generated(GD_DUMB_PC2F);
    dialog_check_generated(GD_CLS_DUMB_PC2M);
    dialog_check_generated(GD_CLS_DUMB_PC2F);
}

// 0x4181D0
void dialog_check_generated(int gd)
{
    MesFileEntry mes_file_entry;
    int overflow;

    tig_debug_printf("Checking generated dialog file mes\\%s.mes\n", off_5A063C[gd]);
    dialog_load_generated(gd);

    if (mes_find_first(dword_5D19F4[gd], &mes_file_entry)) {
        do {
            overflow = tc_check_size(mes_file_entry.str);
            if (overflow > 0) {
                tig_debug_printf("Generated response %d too long by %d pixels\n", mes_file_entry.num, overflow);
            }
        } while (mes_find_next(dword_5D19F4[gd], &mes_file_entry));
    }
}

// 0x418250
void dialog_load_generated(int gd)
{
    char path[TIG_MAX_PATH];

    if (dword_5D19F4[gd] == MES_FILE_HANDLE_INVALID) {
        sprintf(path, "mes\\%s.mes", off_5A063C[gd]);
        if (!mes_load(path, &(dword_5D19F4[gd]))) {
            tig_debug_printf("Cannot open generated dialog file %s\n", path);
            exit(EXIT_SUCCESS); // FIXME: Should be EXIT_FAILURE.
        }
    }
}

// 0x4182C0
void dialog_enable_numbers()
{
    dialog_numbers_enabled = true;
}

// 0x4182D0
void sub_4182D0(char* str, DialogEntryNode* a2, int start, int end)
{
    int gd;
    int cnt;
    MesFileEntry mes_file_entry;

    if (critter_is_dumb(a2->pc_obj)) {
        gd = stat_level_get(a2->pc_obj, STAT_GENDER) == GENDER_MALE
            ? GD_DUMB_PC2M
            : GD_DUMB_PC2F;
    } else {
        gd = stat_level_get(a2->pc_obj, STAT_GENDER) == GENDER_MALE
            ? GD_PC2M
            : GD_PC2F;
    }

    dialog_load_generated(gd);

    cnt = mes_entries_count_in_range(dword_5D19F4[gd], start, end);
    mes_file_entry.num = start + random_between(0, cnt - 1);
    mes_get_msg(dword_5D19F4[gd], &mes_file_entry);

    sub_416B00(str, mes_file_entry.str, a2);
}

// 0x418390
void sub_418390(char* str, DialogEntryNode* a2, int start)
{
    int gd;
    int cnt;
    MesFileEntry mes_file_entry;

    if (critter_is_dumb(a2->pc_obj)) {
        gd = stat_level_get(a2->pc_obj, STAT_GENDER) == GENDER_MALE
            ? GD_CLS_DUMB_PC2M
            : GD_CLS_DUMB_PC2F;
    } else {
        gd = stat_level_get(a2->pc_obj, STAT_GENDER) == GENDER_MALE
            ? GD_CLS_PC2M
            : GD_CLS_PC2F;
    }

    dialog_load_generated(gd);

    cnt = mes_entries_count_in_range(dword_5D19F4[gd], start, start + 49);
    mes_file_entry.num = start + random_between(0, cnt - 1);
    mes_get_msg(dword_5D19F4[gd], &mes_file_entry);

    sub_416B00(str, mes_file_entry.str, a2);
}

// 0x418460
void sub_418460(char* str, DialogEntryNode* a2)
{
    int v1 = sub_445090();
    sub_4182D0(str, a2, v1 + 1700, v1 + 1700);
}

// 0x418480
void dialog_copy_class_specific_msg(char* buffer, DialogEntryNode* a2, int num)
{
    int gd;
    int cnt;
    MesFileEntry mes_file_entry;

    if (dialog_copy_override_msg(buffer, a2, num / 1000 + 9999)) {
        return;
    }

    if (critter_is_dumb(a2->npc_obj)) {
        if (stat_level_get(a2->npc_obj, STAT_GENDER) == GENDER_MALE) {
            gd = stat_level_get(a2->pc_obj, STAT_GENDER) == GENDER_MALE
                ? GD_CLS_DUMB_M2M
                : GD_CLS_DUMB_M2F;
        } else {
            gd = stat_level_get(a2->pc_obj, STAT_GENDER) == GENDER_MALE
                ? GD_CLS_DUMB_F2M
                : GD_CLS_DUMB_F2F;
        }
    } else {
        if (stat_level_get(a2->npc_obj, STAT_GENDER) == GENDER_MALE) {
            gd = stat_level_get(a2->pc_obj, STAT_GENDER) == GENDER_MALE
                ? GD_CLS_M2M
                : GD_CLS_M2F;
        } else {
            gd = stat_level_get(a2->pc_obj, STAT_GENDER) == GENDER_MALE
                ? GD_CLS_F2M
                : GD_CLS_F2F;
        }

        num += 50 * critter_social_class_get(a2->npc_obj);
    }

    dialog_load_generated(gd);

    cnt = mes_entries_count_in_range(dword_5D19F4[gd], num, num + 49);
    mes_file_entry.num = num + random_between(0, cnt - 1);
    mes_get_msg(dword_5D19F4[gd], &mes_file_entry);

    sub_416B00(buffer, mes_file_entry.str, a2);
    a2->field_458 = -1;
}

// 0x4185F0
void dialog_copy_race_specific_msg(char* buffer, DialogEntryNode* a2, int num)
{
    int gd;
    int cnt;
    MesFileEntry mes_file_entry;
    int race;

    if (dialog_copy_override_msg(buffer, a2, num / 1000 + 10999)) {
        return;
    }

    if (critter_is_dumb(a2->npc_obj)) {
        if (stat_level_get(a2->npc_obj, STAT_GENDER) == GENDER_MALE) {
            gd = stat_level_get(a2->pc_obj, STAT_GENDER) == GENDER_MALE
                ? GD_RCE_DUMB_M2M
                : GD_RCE_DUMB_M2F;
        } else {
            gd = stat_level_get(a2->pc_obj, STAT_GENDER) == GENDER_MALE
                ? GD_RCE_DUMB_F2M
                : GD_RCE_DUMB_F2F;
        }
    } else {
        if (stat_level_get(a2->npc_obj, STAT_GENDER) == GENDER_MALE) {
            gd = stat_level_get(a2->pc_obj, STAT_GENDER) == GENDER_MALE
                ? GD_RCE_M2M
                : GD_RCE_M2F;
        } else {
            gd = stat_level_get(a2->pc_obj, STAT_GENDER) == GENDER_MALE
                ? GD_RCE_F2M
                : GD_RCE_F2F;
        }

        race = stat_level_get(a2->pc_obj, STAT_RACE);
        if (race != stat_level_get(a2->npc_obj, STAT_RACE)) {
            num += 50 * (race + 1);
        }
    }

    dialog_load_generated(gd);

    cnt = mes_entries_count_in_range(dword_5D19F4[gd], num, num + 49);
    mes_file_entry.num = num + random_between(0, cnt - 1);
    mes_get_msg(dword_5D19F4[gd], &mes_file_entry);

    sub_416B00(buffer, mes_file_entry.str, a2);
    a2->field_458 = -1;
}

// 0x418780
void dialog_copy_generic_msg(char* buffer, DialogEntryNode* a2, int start, int end)
{
    int gd;
    int cnt;
    MesFileEntry mes_file_entry;

    if (dialog_copy_override_msg(buffer, a2, start / 100 + 11999)) {
        return;
    }

    if (stat_level_get(a2->npc_obj, STAT_GENDER) == GENDER_MALE) {
        gd = stat_level_get(a2->pc_obj, STAT_GENDER) == GENDER_MALE
            ? GD_NPC_M2M
            : GD_NPC_M2F;
    } else {
        gd = stat_level_get(a2->pc_obj, STAT_GENDER) == GENDER_MALE
            ? GD_NPC_F2M
            : GD_NPC_F2F;
    }

    dialog_load_generated(gd);

    cnt = mes_entries_count_in_range(dword_5D19F4[gd], start, end);
    mes_file_entry.num = start + random_between(0, cnt - 1);
    mes_get_msg(dword_5D19F4[gd], &mes_file_entry);

    sub_416B00(buffer, mes_file_entry.str, a2);
    a2->field_458 = -1;
}

// 0x418870
bool dialog_copy_override_msg(char* buffer, DialogEntryNode* a2, int num)
{
    Script scr;
    char path[TIG_MAX_PATH];
    int dlg;
    DialogEntry entry;
    bool exists;

    obj_arrayfield_script_get(a2->npc_obj, OBJ_F_SCRIPTS_IDX, SAP_DIALOG_OVERRIDE, &scr);

    if (scr.num == 0) {
        return false;
    }

    if (!script_name_build_dlg_name(scr.num, path)) {
        return false;
    }

    if (!sub_412E10(path, &dlg)) {
        return false;
    }

    entry.num = num;
    exists = sub_416AB0(dlg, &entry);
    sub_412F40(dlg);

    if (!exists) {
        return false;
    }

    if (a2->pc_obj != OBJ_HANDLE_NULL
        && obj_type_is_critter(obj_field_int32_get(a2->pc_obj, OBJ_F_TYPE))
        && stat_level_get(a2->pc_obj, STAT_GENDER) == GENDER_MALE) {
        strcpy(buffer, entry.str);
    } else {
        strcpy(buffer, entry.data.female_str);
    }

    a2->field_458 = sub_4189C0(entry.conditions, scr.num);

    return true;
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
void sub_418A00(int a1, int* a2, int* a3)
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

    v1 = sub_4C1150(a6->npc_obj, a6->pc_obj, a1);
    if (v1 == 1) {
        v1 = 2;
    }

    dialog_copy_class_specific_msg(buffer, a6, 1000);
    sprintf(a6->field_70, buffer, v1);
    a6->field_45C = 2;
    sub_4182D0(a6->field_460[0], a6, 1, 99);
    a6->field_1804[0] = v1;
    a6->field_17F0[0] = 4;
    a6->field_182C[0] = NULL;
    sub_4182D0(a6->field_460[1], a6, 100, 199);
    a6->field_17F0[1] = a4;
    a6->field_182C[1] = NULL;
    a6->field_1804[1] = a5;
    a6->field_17F0[2] = a2;
    a6->field_1804[2] = a3;
}

// 0x418B30
void sub_418B30(int a1, DialogEntryNode* a2)
{
    Packet82 pkt;

    if (item_gold_get(a2->pc_obj) < a1) {
        sub_418C40(2000, a2->field_17F0[1], a2->field_1804[1], a2);
    } else {
        if (tig_net_is_active()
            && tig_net_is_host()) {
            pkt.type = 82;
            pkt.field_8 = sub_407EF0(a2->pc_obj);
            pkt.field_20 = sub_407EF0(a2->npc_obj);
            pkt.field_38 = a1;
            pkt.field_40.type = OID_TYPE_NULL;
        }

        item_gold_transfer(a2->pc_obj, a2->npc_obj, a1, OBJ_HANDLE_NULL);
        sub_414810(a2->field_17F0[2], a2->field_1804[2], a2->field_1818[2], 2, a2);
    }
}

// 0x418C40
void sub_418C40(int a1, int a2, int a3, DialogEntryNode* a4)
{
    dialog_copy_class_specific_msg(a4->field_70, a4, a1);
    a4->field_45C = 1;
    sub_4182D0(a4->field_460[0], a4, 600, 699);
    a4->field_17F0[0] = a2;
    a4->field_1804[0] = a3;
    a4->field_182C[0] = NULL;
}

// 0x418CA0
void sub_418CA0(int* a1, int a2, int a3, DialogEntryNode* a4)
{
    int index;

    dialog_copy_class_specific_msg(a4->field_70, a4, 3000);
    a4->field_45C = a2 + 1;

    for (index = 0; index < a2; index++) {
        if (IS_TECH_SKILL(a1[index])) {
            strcpy(a4->field_460[index], tech_skill_get_name(GET_TECH_SKILL(a1[index])));
        } else {
            strcpy(a4->field_460[index], tech_skill_get_name(GET_BASIC_SKILL(a1[index])));
        }

        a4->field_17F0[index] = 6;
        a4->field_1804[index] = a1[index];
        a4->field_182C[index] = NULL;
    }

    a4->field_182C[a2] = NULL;
    sub_4182D0(a4->field_460[a2], a4, 800, 899);
    sub_417590(a3, &(a4->field_17F0[a2]), &(a4->field_1804[a2]));
}

// 0x418DE0
void sub_418DE0(int a1, DialogEntryNode* a2)
{
    int v1;

    v1 = a2->field_45C - 1;
    if (IS_TECH_SKILL(a1)) {
        if (tech_skill_get_training(a2->pc_obj, GET_TECH_SKILL(a1)) != TRAINING_NONE) {
            sub_418C40(4000, a2->field_17F0[v1], a2->field_1804[v1], a2);
            return;
        }

        if (tech_skill_set_training(a2->pc_obj, GET_TECH_SKILL(a1), TRAINING_APPRENTICE) == TRAINING_NONE) {
            sub_418C40(5000, a2->field_17F0[v1], a2->field_1804[v1], a2);
            return;
        }

        tech_skill_set_training(a2->pc_obj, GET_TECH_SKILL(a1), TRAINING_NONE);
        sub_418A40(100, 7, a1, a2->field_17F0[v1], a2->field_1804[v1], a2);
    } else {
        if (basic_skill_get_training(a2->pc_obj, GET_BASIC_SKILL(a1)) != TRAINING_NONE) {
            sub_418C40(4000, a2->field_17F0[v1], a2->field_1804[v1], a2);
            return;
        }

        if (basic_skill_set_training(a2->pc_obj, GET_BASIC_SKILL(a1), TRAINING_APPRENTICE) == TRAINING_NONE) {
            sub_418C40(5000, a2->field_17F0[v1], a2->field_1804[v1], a2);
            return;
        }

        basic_skill_set_training(a2->pc_obj, GET_BASIC_SKILL(a1), TRAINING_NONE);
        sub_418A40(100, 7, a1, a2->field_17F0[v1], a2->field_1804[v1], a2);
    }
}

// 0x418F30
void sub_418F30(int a1, DialogEntryNode* a2)
{
    if (IS_TECH_SKILL(a1)) {
        tech_skill_set_training(a2->pc_obj, GET_TECH_SKILL(a1), TRAINING_APPRENTICE);
    } else {
        basic_skill_set_training(a2->pc_obj, GET_BASIC_SKILL(a1), TRAINING_APPRENTICE);
    }

    dialog_copy_class_specific_msg(a2->field_70, a2, 6000);
    a2->field_45C = 1;
    sub_418390(a2->field_460[0], a2, 1000);
    a2->field_17F0[0] = a2->field_17F0[1];
    a2->field_1804[0] = a2->field_1804[1];
    a2->field_182C[0] = NULL;
}

// 0x418FC0
void sub_418FC0(int a1, int* rumors, int num_rumors, int a4, DialogEntryNode* a5)
{
    int index;
    int num_known_rumors = 0;
    int v1;
    int v2;

    for (index = 0; index < num_rumors; index++) {
        if (!rumor_qstate_get(rumors[index])
            && !rumor_known_get(a5->pc_obj, rumors[index])) {
            rumors[num_known_rumors++] = rumors[index];
        }
    }

    sub_417590(a4, &v1, &v2);

    if (num_known_rumors != 0) {
        index = random_between(0, num_known_rumors - 1);
        if (a1 > 0) {
            sub_418A40(a1, 9, rumors[index], v1, v2, a5);
        } else {
            sub_4190E0(rumors[index], v1, v2, a5);
        }
    } else {
        dialog_copy_class_specific_msg(a5->field_70, a5, 7000);
        a5->field_45C = 1;
        sub_4182D0(a5->field_460[0], a5, 600, 699);
        a5->field_17F0[0] = v1;
        a5->field_1804[0] = v2;
        a5->field_182C[0] = NULL;
    }
}

// 0x4190E0
void sub_4190E0(int a1, int a2, int a3, DialogEntryNode* a4)
{
    char buffer[1000];

    rumor_copy_interaction_str(a4->pc_obj, a4->npc_obj, a1, buffer);
    sub_416B00(a4->field_70, buffer, a4);
    a4->field_458 = -1;
    rumor_known_set(a4->pc_obj, a1);
    a4->field_45C = 1;
    sub_418390(a4->field_460[0], a4, 1000);
    a4->field_17F0[0] = a2;
    a4->field_1804[0] = a3;
    a4->field_182C[0] = NULL;
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
    reaction_adj(a3->npc_obj, a3->pc_obj, -10);
    if (reaction_get(a3->npc_obj, a3->pc_obj) > 20) {
        sub_414810(a1, a2, 0, 0, a3);
    } else {
        dialog_copy_race_specific_msg(a3->field_70, a3, 1000);
        a3->field_45C = 0;
        a3->field_17E8 = 4;
    }
}

// 0x419260
void sub_419260(DialogEntryNode* a1, const char* str)
{
    unsigned int flags[10];
    int values[10];
    unsigned int spell_flags;
    tig_art_id_t aid;
    int armor_type;

    memset(flags, 0, sizeof(flags));

    if (str != NULL) {
        int index;
        int value;

        while (*str != '\0') {
            while (!isdigit(*str)) {
                str++;
            }

            if (*str == '\0') {
                break;
            }

            index = atoi(str);

            while (*str != '\0' && *str != ' ') {
                str++;
            }

            if (*str == '\0') {
                break;
            }

            // TODO: Refactor.
            if (*str != 'f') {
                value = atoi(str);
                flags[index] = value != 0 ? 0x4 : 0x2;
                values[index] = value;
            } else if (str[1] == 'l') {
                value = atoi(str + 2);
                flags[index] = 0x1;
                values[index] = value;
            } else {
                break;
            }

            while (*str != '\0' && *str != ',') {
                str++;
            }
        }
    }

    if ((flags[0] & 0x2) == 0 && critter_is_dead(a1->npc_obj)) {
        if (!sub_4197D0(flags[0], values[0], a1)) {
            dialog_copy_generic_msg(a1->field_70, a1, 1500, 1599);
        }
        return;
    }

    if (sub_4AF210(a1->npc_obj, NULL)) {
        sub_413EA0(a1->npc_obj, a1->pc_obj, a1->field_70, &(a1->field_458));
        return;
    }

    if ((flags[1] & 0x2) == 0 && critter_has_bad_associates(a1->pc_obj)) {
        if (!sub_4197D0(flags[1], values[1], a1)) {
            dialog_copy_class_specific_msg(a1->field_70, a1, 18000);

            if (critter_social_class_get(a1->npc_obj) != SOCIAL_CLASS_WIZARD) {
                a1->field_17E8 = 4;
            }
        }
        return;
    }

    spell_flags = obj_field_int32_get(a1->pc_obj, OBJ_F_SPELL_FLAGS);

    if ((flags[2] & 0x2) == 0 && (spell_flags & OSF_INVISIBLE) != 0) {
        if (!sub_4197D0(flags[2], values[2], a1)) {
            dialog_copy_class_specific_msg(a1->field_70, a1, 22000);

            if (critter_social_class_get(a1->npc_obj) != SOCIAL_CLASS_WIZARD) {
                a1->field_17E8 = 4;
            }
        }
        return;
    }

    if ((flags[3] & 0x2) == 0 && (spell_flags & (OSF_BODY_OF_AIR | OSF_BODY_OF_EARTH | OSF_BODY_OF_FIRE | OSF_BODY_OF_WATER)) != 0) {
        if (!sub_4197D0(flags[3], values[3], a1)) {
            dialog_copy_class_specific_msg(a1->field_70, a1, 19000);

            if (critter_social_class_get(a1->npc_obj) != SOCIAL_CLASS_WIZARD) {
                a1->field_17E8 = 4;
            }
        }
        return;
    }

    if ((flags[4] & 0x2) == 0 && (spell_flags & OSF_POLYMORPHED) != 0) {
        if (!sub_4197D0(flags[4], values[4], a1)) {
            dialog_copy_class_specific_msg(a1->field_70, a1, 20000);

            if (critter_social_class_get(a1->npc_obj) != SOCIAL_CLASS_WIZARD) {
                a1->field_17E8 = 4;
            }
        }
        return;
    }

    if ((flags[6] & 0x2) == 0 && (spell_flags & OSF_SHRUNK) != 0) {
        if (!sub_4197D0(flags[6], values[6], a1)) {
            dialog_copy_race_specific_msg(a1->field_70, a1, 6000);
        }
        return;
    }

    aid = obj_field_int32_get(a1->pc_obj, OBJ_F_CURRENT_AID);
    armor_type = tig_art_critter_id_armor_get(aid);

    if ((flags[7] & 0x2) == 0
        && armor_type == TIG_ART_ARMOR_TYPE_UNDERWEAR) {
        if (!sub_4197D0(flags[7], values[7], a1)) {
            dialog_copy_class_specific_msg(a1->field_70, a1, 16000);
        }
        return;
    }

    if ((flags[8] & 0x2) == 0
        && armor_type == TIG_ART_ARMOR_TYPE_BARBARIAN) {
        if (!sub_4197D0(flags[8], values[8], a1)) {
            dialog_copy_class_specific_msg(a1->field_70, a1, 17000);
        }
        return;
    }

    if ((flags[9] & 0x2) == 0) {
        if (!sub_4197D0(flags[9], values[9], a1)) {
            int reaction_level;
            int v4;

            if (random_between(1, 100) <= 20) {
                v4 = sub_4C1F80(a1->pc_obj, a1->npc_obj);
                if (v4 != 0) {
                    sub_4C2100(a1->pc_obj, a1->npc_obj, v4, a1->field_70);
                    a1->field_458 = -1;
                    if (a1->field_70[0] != '\0') {
                        return;
                    }
                }
            }

            reaction_level = reaction_translate(reaction_get(a1->npc_obj, a1->pc_obj));
            if (sub_4C0C40(a1->npc_obj, a1->pc_obj)) {
                switch (reaction_level) {
                case REACTION_LOVE:
                    dialog_copy_class_specific_msg(a1->field_70, a1, 11000);
                    break;
                case REACTION_AMIABLE:
                case REACTION_COURTEOUS:
                    dialog_copy_class_specific_msg(a1->field_70, a1, 12000);
                    break;
                case REACTION_NEUTRAL:
                    dialog_copy_class_specific_msg(a1->field_70, a1, 13000);
                    break;
                case REACTION_SUSPICIOUS:
                case REACTION_DISLIKE:
                    dialog_copy_race_specific_msg(a1->field_70, a1, 4000);
                    break;
                case REACTION_HATRED:
                    dialog_copy_race_specific_msg(a1->field_70, a1, 5000);
                    break;
                }
            } else {
                switch (reaction_level) {
                case REACTION_LOVE:
                    dialog_copy_class_specific_msg(a1->field_70, a1, 8000);
                    break;
                case REACTION_AMIABLE:
                case REACTION_COURTEOUS:
                    dialog_copy_class_specific_msg(a1->field_70, a1, 9000);
                    break;
                case REACTION_NEUTRAL:
                    dialog_copy_class_specific_msg(a1->field_70, a1, 10000);
                    break;
                case REACTION_SUSPICIOUS:
                case REACTION_DISLIKE:
                    dialog_copy_race_specific_msg(a1->field_70, a1, 2000);
                    break;
                case REACTION_HATRED:
                    dialog_copy_race_specific_msg(a1->field_70, a1, 3000);
                    break;
                }
            }
        }
        return;
    }
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
void sub_419830(int a1, int a2, DialogEntryNode* a3)
{
    int v1;
    int64_t item_obj;
    int v3;
    int index;

    dialog_copy_class_specific_msg(a3->field_70, a3, 14000);
    v1 = spell_college_level_get(a3->npc_obj, 12);
    v3 = 0;
    switch (a1) {
    case 0:
        item_obj = sub_4C91F0(a3->npc_obj, SKILL_HEAL);
        if (sub_4AE570(a3->npc_obj, a3->pc_obj, item_obj, SKILL_HEAL) == 0) {
            sub_419A00(0, SKILL_HEAL, a2, a3);
            v3 = 1;
        }
        if (v1 >= 1) {
            sub_4182D0(a3->field_460[v3], a3, 1100, 1199);
            a3->field_17F0[v3] = 12;
            a3->field_1804[v3] = a2;
            v3++;
        }
        if (v1 >= 2) {
            sub_4182D0(a3->field_460[v3], a3, 1200, 1299);
            a3->field_17F0[v3] = 13;
            a3->field_1804[v3] = a2;
            v3++;
        }
        if (v1 == 5) {
            sub_419AC0(v3++, 64, a2, a3);
        }
        break;
    case 1:
        if (v1 >= 1) {
            sub_419AC0(0, 60, a2, a3);
            v3 = 1;
        }
        if (v1 >= 3) {
            sub_419AC0(v3++, 62, a2, a3);
        }
        break;
    case 2:
        if (v1 >= 2) {
            sub_419AC0(0, 61, a2, a3);
            v3 = 1;
        }
        break;
    }

    sub_4182D0(a3->field_460[v3], a3, 800, 899);
    sub_417590(a2, &(a3->field_17F0[v3]), &(a3->field_1804[v3]));
    a3->field_45C = v3 + 1;

    for (index = 0; index < a3->field_45C; index++) {
        a3->field_182C[index] = NULL;
    }
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
    if (critter_leader_get(a3->npc_obj) == a3->pc_obj) {
        sub_419CB0(a1, v1, v2, a3);
    } else {
        if (IS_TECH_SKILL(a1)) {
            skill_level = tech_skill_level(a3->npc_obj, GET_TECH_SKILL(a1));
            training = tech_skill_get_training(a3->npc_obj, GET_TECH_SKILL(a1));
            v3 = sub_4C69E0(GET_TECH_SKILL(a1), skill_level, training);
        } else {
            skill_level = basic_skill_level(a3->npc_obj, GET_BASIC_SKILL(a1));
            training = basic_skill_get_training(a3->npc_obj, GET_BASIC_SKILL(a1));
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
    if (critter_leader_get(a3->npc_obj) == a3->pc_obj) {
        sub_419D50(a1, v1, v2, a3);
    } else {
        sub_418A40(sub_4B1740(a1), 17, a1, v1, v2, a3);
    }
}

// 0x419CB0
void sub_419CB0(int a1, int a2, int a3, DialogEntryNode* a4)
{
    unsigned int flags;
    int64_t item_obj;

    if (critter_pc_leader_get(a4->npc_obj) != OBJ_HANDLE_NULL) {
        flags = 0;
    } else {
        flags = 0x2000;
    }

    item_obj = sub_4C91F0(a4->npc_obj, a1);
    anim_goal_use_skill_on(a4->npc_obj, a4->pc_obj, item_obj, a1, flags);
    dialog_copy_class_specific_msg(a4->field_70, a4, 15000);
    a4->field_45C = 1;
    sub_418390(a4->field_460[0], a4, 1000);
    a4->field_17F0[0] = a2;
    a4->field_1804[0] = a3;
    a4->field_182C[0] = NULL;
}

// 0x419D50
void sub_419D50(int a1, int a2, int a3, DialogEntryNode* a4)
{
    MagicTechSerializedData v1;

    sub_455A20(&v1, a4->npc_obj, a1);
    sub_4440E0(a4->pc_obj, &(v1.target_obj));
    if (critter_pc_leader_get(a4->npc_obj) == OBJ_HANDLE_NULL) {
        v1.flags |= 0x2;
    }
    sub_455AC0(&v1);
    dialog_copy_class_specific_msg(a4->field_70, a4, 15000);
    a4->field_45C = 1;
    sub_418390(a4->field_460[0], a4, 1000);
    a4->field_17F0[0] = a2;
    a4->field_1804[0] = a3;
    a4->field_182C[0] = NULL;
}

// 0x419E20
int sub_419E20(int64_t obj, int* a2, int cnt)
{
    int v1 = 0;
    int index;

    for (index = 0; index < cnt; index++) {
        if (!area_is_known(obj, a2[index])) {
            a2[v1++] = a2[index];
        }
    }

    return v1;
}

// 0x419E70
void sub_419E70(const char* str, int a2, int a3, int a4, DialogEntryNode* a5)
{
    char buffer[1000];
    char* pch;
    int v1;
    int v2;
    int v3[100];
    int v4;
    int v5;
    int index;

    strcpy(buffer, str);

    pch = strchr(buffer, '$');
    v1 = atoi(pch + 1);

    pch = strchr(pch, ',');
    v2 = sub_417F90(v3, pch + 1);

    v4 = 0;
    v5 = sub_419E20(a5->pc_obj, v3, v2);

    if (a4) {
        dialog_copy_generic_msg(a5->field_70, a5, 500, 599);
    } else {
        dialog_copy_generic_msg(a5->field_70, a5, 100, 199);
    }

    if (critter_leader_get(a5->npc_obj) == a5->pc_obj) {
        a5->field_17EC = 0;
    } else {
        a5->field_17EC = v1;
    }

    for (v4 = 0; v4 < 4; v4++) {
        if (a3 + v4 >= v5) {
            break;
        }

        strcpy(a5->field_460[v4], area_get_name(v3[a3 + v4]));
        a5->field_17F0[v4] = a4 ? 22 : 19;
        a5->field_1804[v4] = v3[a3 + v4];
        a5->field_1818[v4] = a2;
    }

    if (a3 + v4 < v5) {
        sub_4182D0(a5->field_460[v4], a5, 600, 699);
        a5->field_17F0[v4] = a4 ? 21 : 18;
        a5->field_1804[v4] = a2;
        a5->field_1818[v4] = a3 + v4;
    } else {
        sub_4182D0(a5->field_460[v4], a5, 800, 899);
        sub_417590(a2, &(a5->field_17F0[v4]), &(a5->field_1804[v4]));
    }

    a5->field_45C = v4 + 1;
    for (index = 0; index < a5->field_45C; index++) {
        a5->field_182C[index] = NULL;
    }
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
void sub_41A150(int area, int a2, int a3, DialogEntryNode* a4)
{
    int64_t loc;
    int64_t area_loc;
    int rot;
    int64_t v3;
    int v4;

    loc = obj_field_int64_get(a4->npc_obj, OBJ_F_LOCATION);
    area_loc = area_get_location(area);
    rot = location_rot(loc, area_loc);
    v3 = location_dist(loc, area_loc) / 3168;

    if (v3 < 2) {
        v4 = 200;
    } else if (v3 < 100) {
        v4 = 300;
    } else {
        v4 = 400;
    }

    dialog_copy_generic_msg(a4->field_70, a4, v4 + 10 * rot, v4 + 10 * rot + 9);

    sub_418390(a4->field_460[0], a4, 1000);
    a4->field_17F0[0] = a2;
    a4->field_1804[0] = a3;
    a4->field_182C[0] = NULL;
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
void sub_41A290(int area, int a2, int a3, DialogEntryNode* a4)
{
    int64_t loc;
    int64_t area_loc;
    int rot;
    int64_t v3;
    int v4;
    char buffer[1000];

    loc = obj_field_int64_get(a4->npc_obj, OBJ_F_LOCATION);
    area_loc = area_get_location(area);
    rot = location_rot(loc, area_loc);
    v3 = location_dist(loc, area_loc) / 3168;
    v4 = v3 < 2 ? 600 : 700;

    dialog_copy_generic_msg(buffer, a4, v4 + 10 * rot, v4 + 10 * rot + 9);

    if (v3 < 2) {
        strcpy(a4->field_70, buffer);
    } else {
        sprintf(a4->field_70, buffer, v3);
    }

    sub_418390(a4->field_460[0], a4, 1000);
    a4->field_17F0[0] = a2;
    a4->field_1804[0] = a3;
    a4->field_182C[0] = NULL;
    a4->field_45C = 1;
    area_set_known(a4->pc_obj, area);
}

// 0x41A3E0
void sub_41A3E0(int a1, DialogEntryNode* a2)
{
    sub_41A440(a2->field_70, a2);
    sub_418390(a2->field_460[0], a2, 1000);
    sub_417590(a1, a2->field_17F0, a2->field_1804);
    a2->field_182C[0] = NULL;
    a2->field_45C = 1;
}

// 0x41A440
void sub_41A440(char* buffer, DialogEntryNode* a2)
{
    int v1;
    int gd;
    MesFileEntry mes_file_entry;

    v1 = sub_445090();
    if (!dialog_copy_override_msg(buffer, a2, v1 + 10000)) {
        if (stat_level_get(a2->npc_obj, STAT_GENDER) == GENDER_MALE) {
            gd = stat_level_get(a2->pc_obj, STAT_GENDER) == GENDER_MALE
                ? GD_STO_M2F
                : GD_STO_M2M;
        } else {
            gd = stat_level_get(a2->pc_obj, STAT_GENDER) == GENDER_MALE
                ? GD_STO_F2M
                : GD_STO_F2F;
        }

        dialog_load_generated(gd);

        mes_file_entry.num = stat_level_get(a2->npc_obj, STAT_RACE) + 100 * v1;
        mes_get_msg(dword_5D19F4[gd], &mes_file_entry);
        sub_416B00(buffer, mes_file_entry.str, a2);
        a2->field_458 = -1;
    }
}

// 0x41A520
void sub_41A520(int a1, DialogEntryNode* a2)
{
    dialog_copy_generic_msg(a2->field_70, a2, 4200, 4299);
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
    a2->field_182C[0] = NULL;
    a2->field_182C[1] = NULL;
    a2->field_182C[2] = NULL;
    a2->field_182C[3] = NULL;
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

    dialog_copy_generic_msg(&(a2->field_70[pos]), a2, 4300, 4399);
    sub_4182D0(a2->field_460[0], a2, 1, 99);
    a2->field_17F0[0] = 28;
    a2->field_1804[0] = v1;
    a2->field_1818[0] = a1;
    sub_4182D0(a2->field_460[1], a2, 100, 199);
    sub_417590(a1, &a2->field_17F0[1], &a2->field_1804[1]);
    a2->field_182C[0] = NULL;
    a2->field_182C[1] = NULL;
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
        dialog_copy_generic_msg(a2->field_70, a2, 4500, 4599);
        for (index = 0; index < cnt; index++) {
            sub_460800(v1[index], a2->field_460[index]);
            a2->field_17F0[index] = 28;
            a2->field_1804[index] = v1[index];
        }
        sub_4182D0(a2->field_460[cnt], a2, 800, 899);
        sub_417590(a1, &a2->field_17F0[cnt], &a2->field_1804[cnt]);
        a2->field_45C = cnt + 1;
    } else {
        dialog_copy_generic_msg(a2->field_70, a2, 4400, 4499);
        sub_4182D0(a2->field_460[0], a2, 600, 699);
        sub_417590(a1, a2->field_17F0, a2->field_1804);
        a2->field_45C = 1;
    }

    for (index = 0; index < a2->field_45C; index++) {
        a2->field_182C[index] = NULL;
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

    loc = obj_field_int64_get(a4->pc_obj, OBJ_F_LOCATION);
    obj = newspaper_create(a1, loc);
    item_transfer(obj, a4->pc_obj);
    dialog_copy_class_specific_msg(a4->field_70, a4, 15000);
    sub_418390(a4->field_460[0], a4, 1000);
    a4->field_17F0[0] = a2;
    a4->field_1804[0] = a3;
    a4->field_182C[0] = NULL;
}
