#include "game/mt_ai.h"

#include "game/magictech.h"
#include "game/stat.h"

typedef struct S600A20_Entry {
    int spell;
    int64_t obj;
} S600A20_Entry;

// See 0x4CC3C0.
static_assert(sizeof(S600A20_Entry) == 0x10, "wrong size");

typedef struct S600A20 {
    int cnt;
    S600A20_Entry* entries;
} S600A20;

static_assert(sizeof(S600A20) == 0x8, "wrong size");

typedef struct S5FF620 {
    /* 0000 */ int64_t obj;
    /* 0008 */ int64_t field_8;
    /* 0010 */ int field_10;
    /* 0014 */ int field_14;
    /* 0018 */ int field_18;
    /* 001C */ int field_1C;
    /* 0020 */ int field_20;
    /* 0024 */ int field_24;
    /* 0028 */ int field_28;
    /* 002C */ int field_2C;
    /* 0030 */ S600A20 field_30[10];
} S5FF620;

static_assert(sizeof(S5FF620) == 0x80, "wrong size");

static int sub_4CC2D0(const S600A20_Entry* a, const S600A20_Entry* b);

// 0x5B7558
static int dword_5B7558 = -1;

// 0x5FF620
static S5FF620 stru_5FF620[40];

// 0x600A20
static S600A20 stru_600A20[10];

// 0x600A70
static S600A20_Entry stru_600A70[200];

// 0x6016F0
static bool mt_ai_initialized;

// 06016F4
static int dword_6016F4;

// 0x6016F8
static int dword_6016F8;

// 0x6016FC
static int dword_6016FC;

// 0x4CC1F0
void mt_ai_init(GameInitInfo* init_info)
{
    (void)init_info;

    sub_4CC470();
    mt_ai_initialized = true;

    if (!sub_4CC240()) {
        return false;
    }

    return true;
}

// 0x4CC210
void mt_ai_reset()
{
    sub_4CC4C0();
}

// 0x4CC220
void mt_ai_exit()
{
    if (mt_ai_initialized) {
        sub_4CC270();
        sub_4CC4C0();
        mt_ai_initialized = false;
    }
}

// 0x4CC240
bool sub_4CC240()
{
    int index;

    for (index = 0; index < 10; index++) {
        if (!sub_4CC350(index, &(stru_600A20[index]))) {
            return false;
        }
    }

    return true;
}

// 0x4CC270
void sub_4CC270()
{
    int index;

    for (index = 0; index < 10; index++) {
        if (stru_600A20[index].entries != NULL) {
            FREE(stru_600A20[index].entries);
            stru_600A20[index].entries = NULL;
        }
    }
}

// 0x4CC2A0
int sub_4CC2A0(int spl)
{
    if (mt_ai_initialized) {
        return magictech_spells[spl].ai.defensive2;
    }
}

// 0x4CC2D0
int sub_4CC2D0(const S600A20_Entry* a, const S600A20_Entry* b)
{
    int v1;
    int v2;

    v1 = sub_4CC310(a->spell);
    v2 = sub_4CC310(b->spell);

    if (v1 < v2) {
        return 1;
    } else if (v1 > v2) {
        return -1;
    } else {
        return 0;
    }
}

// 0x4CC310
int sub_4CC310(int spl)
{
    return dword_5B7558 >= 0 && dword_5B7558 < 10 && mt_ai_initialized
        ? magictech_spells[spl].ai.flee + dword_5B7558
        : -1;
}

// 0x4CC350
bool sub_4CC350(int index, S600A20* a2)
{
    dword_5B7558 = index;
    if (index >= 0 && index < 10) {
        return sub_4CC380(a2);
    } else {
        return false;
    }
}

// 0x4CC380
bool sub_4CC380(S600A20* a2)
{
    int cnt;
    int index;

    cnt = 0;
    for (index = 0; index < MT_SPELL_COUNT; index++) {
        if (sub_4CC310(index) != -1) {
            cnt++;
        }
    }

    a2->cnt = cnt;

    if (cnt != 0) {
        if (a2->entries != NULL) {
            FREE(a2->entries);
        }

        a2->entries = MALLOC(sizeof(*a2->entries) * cnt);

        cnt = 0;
        for (index = 0; index < MT_SPELL_COUNT; index++) {
            if (sub_4CC310(index) != -1) {
                a2->entries[cnt].spell = index;
                a2->entries[cnt].obj = OBJ_HANDLE_NULL;
            }
        }

        qsort(a2->entries, a2->cnt, sizeof(*a2->entries), sub_4CC2D0);
    } else {
        a2->entries = NULL;
    }

    return true;
}

// 0x4CC470
void sub_4CC470()
{
    int index;
    int k;
    S5FF620* ptr;

    for (index = 0; index < 40; index++) {
        ptr = &(stru_5FF620[index]);
        ptr->obj = OBJ_HANDLE_NULL;
        ptr->field_28 = 0;
        ptr->field_8 = 0;
        ptr->field_18 = 0;
        ptr->field_2C = 0;
        ptr->field_1C = 0;

        for (k = 0; k < 10; k++) {
            ptr->field_30[k].entries = NULL;
            ptr->field_30[k].cnt = 0;
        }
    }

    dword_6016F8 = 0;
    dword_6016FC = 0;
}

// 0x4CC4C0
void sub_4CC4C0()
{
    int index;

    for (index = 0; index < 40; index++) {
        sub_4CC520(index);
    }

    dword_6016F8 = 0;
    dword_6016FC = 0;
}

// 0x4CC4F0
void sub_4CC4F0(S5FF620* a1, int64_t obj, int a3)
{
    a1->obj = obj;
    a1->field_10 = a3;
    a1->field_18 = 0;
    a1->field_28 = 0;
    a1->field_1C = 0;
    a1->field_20 = -1;
    a1->field_2C = 0;
}

// 0x4CC520
void sub_4CC520(int index)
{
    S5FF620* ptr;
    int index;

    ptr = &(stru_5FF620[index]);
    ptr->obj = OBJ_HANDLE_NULL;
    ptr->field_8 = OBJ_HANDLE_NULL;
    ptr->field_28 = 0;
    ptr->field_18 = 0;
    ptr->field_2C = 0;
    ptr->field_1C = 0;

    for (index = 0; index < 10; index++) {
        if (ptr->field_30[index].entries != NULL) {
            FREE(ptr->field_30[index].entries);
            ptr->field_30[index].entries = NULL;
        }
        ptr->field_30[index].cnt = 0;
    }

    dword_6016F8--;
}

// 0x4CC580
void sub_4CC580()
{
    // TODO: Incomplete.
}

// 0x4CC6F0
bool sub_4CC6F0(S5FF620* a1)
{
    int index = 0;

    for (index = 0; index < 40; index++) {
        if (stru_5FF620[index].obj == a1->obj) {
            sub_4CC740(&(stru_5FF620[index]), a1);
            return true;
        }
    }

    return false;
}

// 0x4CC740
void sub_4CC740(S5FF620* src, S5FF620* dst)
{
    int index;

    for (index = 0; index < 10; index++) {
        dst->field_30[index] = src->field_30[index];
    }
}

// 0x4CC770
bool sub_4CC770(S5FF620** a1)
{
    int index;

    for (index = 0; index < 40; index++) {
        if (stru_5FF620[index].obj == OBJ_HANDLE_NULL) {
            break;
        }
    }

    if (index < 40) {
        *a1 = &(stru_5FF620[index]);
        dword_6016F8++;
        return true;
    }

    *a1 = &(stru_5FF620[dword_6016FC]);
    sub_4CC520(dword_6016FC);

    if (++dword_6016FC >= 40) {
        dword_6016FC = 0;
    }

    dword_6016F8++;

    return true;
}

// 0x4CC810
void sub_4CC810(S5FF620* a1)
{
    int64_t item_obj;
    int cnt;
    int index;

    item_obj = item_wield_get(a1->obj, 1004);
    if (item_obj != OBJ_HANDLE_NULL
        && (obj_field_int32_get(item_obj, OBJ_F_ITEM_FLAGS) & OIF_HEXED) != 0) {
        return sub_4CC930(a1, item_obj);
    }

    cnt = obj_field_int32_get(a1->obj, OBJ_F_CRITTER_INVENTORY_LIST_IDX);
    for (index = 0; index < cnt; index++) {
        item_obj = obj_arrayfield_handle_get(a1->obj, OBJ_F_CRITTER_INVENTORY_LIST_IDX, index);
        sub_4CC930(a1, item_obj);
    }
}

// 0x4CC8A0
void magictech_build_ai_action_list(S5FF620* a1)
{
    S600A20* v1;
    int index;

    if (a1->field_10 < 0
        || a1->field_10 >= 10) {
        tig_debug_printf("MagicTech: magictech_build_ai_action_list: ERROR: AI action type out of range!\n");
        return;
    }

    v1 = &(stru_600A20[a1->field_10]);
    if (v1 == NULL) {
        // FIXME: Meaningless?
        tig_debug_printf("MagicTech: magictech_build_ai_action_list: ERROR: AI action type out of range!\n");
        return;
    }

    if (stat_level(a1->obj, STAT_MAGICK_POINTS) > 0) {
        for (index = 0; index < v1->cnt; index++) {
            if (sub_4B1950(a1->obj, v1->entries[index].spell)) {
                sub_4CCAD0(v1->entries[index].spell);
            }
        }
    }
}

// 0x4CC930
void sub_4CC930(S5FF620* a1, int64_t item_obj)
{
    int index;
    int spell;

    // FIXME: Unused.
    obj_field_int32_get(item_obj, OBJ_F_TYPE);

    if ((obj_field_int32_get(item_obj, OBJ_F_ITEM_SPELL_MANA_STORE)) != 0
        && sub_4CCA30(a1, item_obj)) {
        for (index = 0; index < 5; index++) {
            spell = obj_field_int32_get(item_obj, 100 + index);
            if (spell != 10000
                && (magictech_spells[spell].ai.flee + dword_5B7558) != -1
                && sub_456A10(a1->obj, a1->field_18, a1->field_1C, item_obj)) {
                sub_4CCB10(spell, item_obj);
            }
        }
    }

    if ((obj_field_int32_get(item_obj, OBJ_F_ITEM_FLAGS) & OIF_VALID_AI_ACTION) != 0
        && obj_field_int32_get(item_obj, OBJ_F_ITEM_AI_ACTION) == a1->field_10) {
        sub_4CCB50(item_obj);
    }
}

// 0x4CCA30
bool sub_4CCA30(int64_t item_obj, S5FF620* a2)
{
    bool ret = true;
    int type;

    if ((obj_field_int32_get(item_obj, OBJ_F_ITEM_FLAGS) & OIF_IDENTIFIED) == 0
        && a2->field_8 != OBJ_HANDLE_NULL) {
        type = tig_art_item_id_type_get(obj_field_int32_get(item_obj, OBJ_F_CURRENT_AID));
        if (type != TIG_ART_ITEM_TYPE_FOOD && type != TIG_ART_ITEM_TYPE_SCROLL) {
            return false;
        }
    }

    return ret;
}

// 0x4CCA90
void sub_4CCA90(S5FF620* a1, int64_t obj, int a3)
{
    if (obj_type_is_critter(obj_field_int32_get(obj, OBJ_F_TYPE))) {
        sub_4CC4F0(a1, obj, a3);
        sub_4CC580(a1);
    }
}

// 0x4CCAD0
void sub_4CCAD0(int spell)
{
    if (dword_6016F4 < 200) {
        stru_600A70[dword_6016F4].spell = spell;
        stru_600A70[dword_6016F4].obj = OBJ_HANDLE_NULL;
        dword_6016F4++;
    }
}

// 0x4CCB10
void sub_4CCB10(int spell, int64_t obj)
{
    if (dword_6016F4 < 200) {
        stru_600A70[dword_6016F4].spell = spell;
        stru_600A70[dword_6016F4].obj = obj;
        dword_6016F4++;
    }
}

// 0x4CCB50
void sub_4CCB50(int64_t obj)
{
    if (dword_6016F4 < 200) {
        stru_600A70[dword_6016F4].spell = -1;
        stru_600A70[dword_6016F4].obj = obj;
        dword_6016F4++;
    }
}

// 0x4CCB90
void sub_4CCB90(S5FF620* a1, int a2)
{
    a1->field_30[a2].cnt = dword_6016F4;
    if (a1->field_30[a2].entries != NULL) {
        FREE(a1->field_30[a2].entries);
    }

    a1->field_30[a2].entries = (S600A20_Entry*)MALLOC(sizeof(S600A20_Entry) * dword_6016F4);
    memcpy(a1->field_30[a2].entries, stru_600A70, sizeof(S600A20_Entry) * dword_6016F4);
}

// 0x4CCBF0
void sub_4CCBF0(int a1)
{
}

// 0x4CCC00
void sub_4CCC00(int64_t obj)
{
    int index;

    for (index = 0; index < 40; index++) {
        if (stru_5FF620[index].obj == obj) {
            sub_4CC520(index);
        }
    }
}

// 0x4CCC40
void sub_4CCC40()
{
    // TODO: Incomplete.
}
