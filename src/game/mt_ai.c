#include "game/mt_ai.h"

#include "game/ai.h"
#include "game/combat.h"
#include "game/critter.h"
#include "game/item.h"
#include "game/magictech.h"
#include "game/spell.h"
#include "game/stat.h"

#define MT_AI_MAX_LISTS 40
#define MT_AI_MAX_ENTRIES 200

static bool sub_4CC240();
static void sub_4CC270();
static int sub_4CC2D0(const AiActionListEntry* a, const AiActionListEntry* b);
static int sub_4CC310(int spl);
static bool sub_4CC350(int index, S600A20* a2);
static bool sub_4CC380(S600A20* a2, const char* name);
static void sub_4CC470();
static void sub_4CC4C0();
static void mt_ai_action_list_init(AiActionList* ai_action_list, int64_t obj, int action);
static void sub_4CC520(int slot);
static void sub_4CC580(AiActionList* ai_action_list);
static bool sub_4CC6F0(AiActionList* ai_action_list);
static void copy_actions(AiActionList* src, AiActionList* dst);
static bool sub_4CC770(AiActionList** ai_action_list_ptr);
static void sub_4CC810(AiActionList* ai_action_list);
static void magictech_build_ai_action_list(AiActionList* ai_action_list);
static void sub_4CC930(AiActionList* ai_action_list, int64_t item_obj);
static bool sub_4CCA30(int64_t item_obj, AiActionList* a2);
static void sub_4CCAD0(int spell);
static void sub_4CCB10(int spell, int64_t item_obj);
static void sub_4CCB50(int64_t item_obj);
static void flush_actions(AiActionList* ai_action_list, int action);

// 0x5B7558
static int dword_5B7558 = -1;

// 0x5B755C
static const char* off_5B755C[AI_ACTION_COUNT] = {
    /*            AI_ACTION_FLEE */ "AIFlee",
    /*          AI_ACTION_SUMMON */ "AISummon",
    /*       AI_ACTION_DEFENSIVE */ "AIDefensive",
    /*       AI_ACTION_OFFENSIVE */ "AIOffensive",
    /*      AI_ACTION_HEAL_LIGHT */ "AIHealLight",
    /*     AI_ACTION_HEAL_MEDIUM */ "AIHealMedium",
    /*      AI_ACTION_HEAL_HEAVY */ "AIHealHeavy",
    /*     AI_ACTION_CURE_POISON */ "AICurePoison",
    /* AI_ACTION_FATIGUE_RECOVER */ "AIFatigueRecover",
    /*       AI_ACTION_RESURRECT */ "AIResurrect",
};

// 0x5FF620
static AiActionList mt_ai_action_lists[MT_AI_MAX_LISTS];

// 0x600A20
static S600A20 stru_600A20[AI_ACTION_COUNT];

// 0x600A70
static AiActionListEntry stru_600A70[MT_AI_MAX_ENTRIES];

// 0x6016F0
static bool mt_ai_initialized;

// 06016F4
static int dword_6016F4;

// 0x6016F8
static int dword_6016F8;

// 0x6016FC
static int dword_6016FC;

// 0x4CC1F0
bool mt_ai_init(GameInitInfo* init_info)
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
    int action;

    for (action = 0; action < AI_ACTION_COUNT; action++) {
        if (!sub_4CC350(action, &(stru_600A20[action]))) {
            return false;
        }
    }

    return true;
}

// 0x4CC270
void sub_4CC270()
{
    int action;

    for (action = 0; action < AI_ACTION_COUNT; action++) {
        if (stru_600A20[action].entries != NULL) {
            FREE(stru_600A20[action].entries);
            stru_600A20[action].entries = NULL;
        }
    }
}

// 0x4CC2A0
int sub_4CC2A0(int spl)
{
    if (mt_ai_initialized) {
        return magictech_spells[spl].ai.defensive2;
    } else {
        return 0;
    }
}

// 0x4CC2D0
int sub_4CC2D0(const AiActionListEntry* a, const AiActionListEntry* b)
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
    return dword_5B7558 >= 0 && dword_5B7558 < AI_ACTION_COUNT && mt_ai_initialized
        ? magictech_spells[spl].ai.values[dword_5B7558]
        : -1;
}

// 0x4CC350
bool sub_4CC350(int action, S600A20* a2)
{
    dword_5B7558 = action;
    if (action >= 0 && action < AI_ACTION_COUNT) {
        return sub_4CC380(a2, off_5B755C[action]);
    } else {
        return false;
    }
}

// 0x4CC380
bool sub_4CC380(S600A20* a2, const char* name)
{
    int cnt;
    int spl;

    (void)name;

    cnt = 0;
    for (spl = 0; spl < MT_SPELL_COUNT; spl++) {
        if (sub_4CC310(spl) != -1) {
            cnt++;
        }
    }

    a2->cnt = cnt;

    if (cnt != 0) {
        if (a2->entries != NULL) {
            FREE(a2->entries);
        }

        a2->entries = (AiActionListEntry*)MALLOC(sizeof(*a2->entries) * cnt);

        cnt = 0;
        for (spl = 0; spl < MT_SPELL_COUNT; spl++) {
            if (sub_4CC310(spl) != -1) {
                a2->entries[cnt].spell = spl;
                a2->entries[cnt].item_obj = OBJ_HANDLE_NULL;
                cnt++;
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
    int slot;
    int action;
    AiActionList* ai_action_list;

    for (slot = 0; slot < MT_AI_MAX_LISTS; slot++) {
        ai_action_list = &(mt_ai_action_lists[slot]);
        ai_action_list->obj = OBJ_HANDLE_NULL;
        ai_action_list->field_28 = 0;
        ai_action_list->field_8 = 0;
        ai_action_list->field_18 = OBJ_HANDLE_NULL;
        ai_action_list->field_2C = 0;

        for (action = 0; action < AI_ACTION_COUNT; action++) {
            ai_action_list->actions[action].entries = NULL;
            ai_action_list->actions[action].cnt = 0;
        }
    }

    dword_6016F8 = 0;
    dword_6016FC = 0;
}

// 0x4CC4C0
void sub_4CC4C0()
{
    int slot;

    for (slot = 0; slot < MT_AI_MAX_LISTS; slot++) {
        sub_4CC520(slot);
    }

    dword_6016F8 = 0;
    dword_6016FC = 0;
}

// 0x4CC4F0
void mt_ai_action_list_init(AiActionList* ai_action_list, int64_t obj, int action)
{
    ai_action_list->obj = obj;
    ai_action_list->action = action;
    ai_action_list->field_18 = OBJ_HANDLE_NULL;
    ai_action_list->field_28 = 0;
    ai_action_list->field_20 = -1;
    ai_action_list->field_2C = 0;
}

// 0x4CC520
void sub_4CC520(int slot)
{
    AiActionList* ai_action_list;
    int action;

    ai_action_list = &(mt_ai_action_lists[slot]);
    ai_action_list->obj = OBJ_HANDLE_NULL;
    ai_action_list->field_8 = OBJ_HANDLE_NULL;
    ai_action_list->field_28 = 0;
    ai_action_list->field_18 = OBJ_HANDLE_NULL;
    ai_action_list->field_2C = 0;

    for (action = 0; action < AI_ACTION_COUNT; action++) {
        if (ai_action_list->actions[action].entries != NULL) {
            FREE(ai_action_list->actions[action].entries);
            ai_action_list->actions[action].entries = NULL;
        }
        ai_action_list->actions[action].cnt = 0;
    }

    dword_6016F8--;
}

// 0x4CC580
void sub_4CC580(AiActionList* ai_action_list)
{
    AiActionList* tmp;

    if (ai_action_list->obj == OBJ_HANDLE_NULL) {
        return;
    }

    if (sub_4CC6F0(ai_action_list)) {
        tmp = ai_action_list;
        if (ai_action_list->actions[ai_action_list->action].cnt > 0) {
            return;
        }
    } else {
        if (!sub_4CC770(&tmp)) {
            return;
        }

        tmp->obj = ai_action_list->obj;
        tmp->action = ai_action_list->action;
    }

    tmp->field_8 = critter_pc_leader_get(ai_action_list->obj);
    tmp->field_18 = ai_action_list->field_18;

    dword_6016F4 = 0;
    dword_5B7558 = tmp->action;

    if (combat_critter_is_combat_mode_active(tmp->obj)
        && ai_critter_fatigue_ratio(tmp->obj) <= 50) {
        sub_4CC810(tmp);
        if (dword_6016F4 == 0) {
            magictech_build_ai_action_list(tmp);
        }
    } else {
        magictech_build_ai_action_list(tmp);
        if (dword_6016F4 == 0) {
            sub_4CC810(tmp);
        }
    }

    if (dword_6016F4 > 0) {
        flush_actions(tmp, dword_5B7558);
        tmp->field_20 = stru_600A70[0].spell;
        tmp->field_28 = stru_600A70[0].spell;
    }

    copy_actions(tmp, ai_action_list);
}

// 0x4CC6F0
bool sub_4CC6F0(AiActionList* ai_action_list)
{
    int slot = 0;

    for (slot = 0; slot < MT_AI_MAX_LISTS; slot++) {
        if (mt_ai_action_lists[slot].obj == ai_action_list->obj) {
            copy_actions(&(mt_ai_action_lists[slot]), ai_action_list);
            return true;
        }
    }

    return false;
}

// 0x4CC740
void copy_actions(AiActionList* src, AiActionList* dst)
{
    int action;

    for (action = 0; action < AI_ACTION_COUNT; action++) {
        dst->actions[action] = src->actions[action];
    }
}

// 0x4CC770
bool sub_4CC770(AiActionList** ai_action_list_ptr)
{
    int index;

    for (index = 0; index < MT_AI_MAX_LISTS; index++) {
        if (mt_ai_action_lists[index].obj == OBJ_HANDLE_NULL) {
            break;
        }
    }

    if (index < MT_AI_MAX_LISTS) {
        *ai_action_list_ptr = &(mt_ai_action_lists[index]);
        dword_6016F8++;
        return true;
    }

    *ai_action_list_ptr = &(mt_ai_action_lists[dword_6016FC]);
    sub_4CC520(dword_6016FC);

    if (++dword_6016FC >= MT_AI_MAX_LISTS) {
        dword_6016FC = 0;
    }

    dword_6016F8++;

    return true;
}

// 0x4CC810
void sub_4CC810(AiActionList* ai_action_list)
{
    int64_t item_obj;
    int cnt;
    int index;

    item_obj = item_wield_get(ai_action_list->obj, ITEM_INV_LOC_WEAPON);
    if (item_obj != OBJ_HANDLE_NULL
        && (obj_field_int32_get(item_obj, OBJ_F_ITEM_FLAGS) & OIF_HEXED) != 0) {
        sub_4CC930(ai_action_list, item_obj);
        return;
    }

    cnt = obj_field_int32_get(ai_action_list->obj, OBJ_F_CRITTER_INVENTORY_NUM);
    for (index = 0; index < cnt; index++) {
        item_obj = obj_arrayfield_handle_get(ai_action_list->obj, OBJ_F_CRITTER_INVENTORY_LIST_IDX, index);
        sub_4CC930(ai_action_list, item_obj);
    }
}

// 0x4CC8A0
void magictech_build_ai_action_list(AiActionList* ai_action_list)
{
    S600A20* v1;
    int index;

    if (ai_action_list->action < 0
        || ai_action_list->action >= AI_ACTION_COUNT) {
        tig_debug_printf("MagicTech: magictech_build_ai_action_list: ERROR: AI action type out of range!\n");
        return;
    }

    v1 = &(stru_600A20[ai_action_list->action]);
    if (v1 == NULL) {
        // FIXME: Meaningless?
        tig_debug_printf("MagicTech: magictech_build_ai_action_list: ERROR: AI action type out of range!\n");
        return;
    }

    if (stat_level_get(ai_action_list->obj, STAT_MAGICK_POINTS) > 0) {
        for (index = 0; index < v1->cnt; index++) {
            if (spell_is_known(ai_action_list->obj, v1->entries[index].spell)) {
                sub_4CCAD0(v1->entries[index].spell);
            }
        }
    }
}

// 0x4CC930
void sub_4CC930(AiActionList* ai_action_list, int64_t item_obj)
{
    int index;
    int spell;

    // FIXME: Unused.
    obj_field_int32_get(item_obj, OBJ_F_TYPE);

    if ((obj_field_int32_get(item_obj, OBJ_F_ITEM_SPELL_MANA_STORE)) != 0
        && sub_4CCA30(item_obj, ai_action_list)) {
        for (index = 0; index < 5; index++) {
            spell = obj_field_int32_get(item_obj, 100 + index);
            if (spell != 10000
                && magictech_spells[spell].ai.values[dword_5B7558] != -1
                && sub_456A10(ai_action_list->obj, ai_action_list->field_18, item_obj)) {
                sub_4CCB10(spell, item_obj);
            }
        }
    }

    if ((obj_field_int32_get(item_obj, OBJ_F_ITEM_FLAGS) & OIF_VALID_AI_ACTION) != 0
        && obj_field_int32_get(item_obj, OBJ_F_ITEM_AI_ACTION) == ai_action_list->action) {
        sub_4CCB50(item_obj);
    }
}

// 0x4CCA30
bool sub_4CCA30(int64_t item_obj, AiActionList* ai_action_list)
{
    bool ret = true;
    int type;

    if ((obj_field_int32_get(item_obj, OBJ_F_ITEM_FLAGS) & OIF_IDENTIFIED) == 0
        && ai_action_list->field_8 != OBJ_HANDLE_NULL) {
        type = tig_art_item_id_type_get(obj_field_int32_get(item_obj, OBJ_F_CURRENT_AID));
        if (type != TIG_ART_ITEM_TYPE_FOOD && type != TIG_ART_ITEM_TYPE_SCROLL) {
            return false;
        }
    }

    return ret;
}

// 0x4CCA90
void mt_ai_action_list_create(AiActionList* ai_action_list, int64_t obj, int action)
{
    if (obj_type_is_critter(obj_field_int32_get(obj, OBJ_F_TYPE))) {
        mt_ai_action_list_init(ai_action_list, obj, action);
        sub_4CC580(ai_action_list);
    }
}

// 0x4CCAD0
void sub_4CCAD0(int spell)
{
    if (dword_6016F4 < MT_AI_MAX_ENTRIES) {
        stru_600A70[dword_6016F4].spell = spell;
        stru_600A70[dword_6016F4].item_obj = OBJ_HANDLE_NULL;
        dword_6016F4++;
    }
}

// 0x4CCB10
void sub_4CCB10(int spell, int64_t item_obj)
{
    if (dword_6016F4 < MT_AI_MAX_ENTRIES) {
        stru_600A70[dword_6016F4].spell = spell;
        stru_600A70[dword_6016F4].item_obj = item_obj;
        dword_6016F4++;
    }
}

// 0x4CCB50
void sub_4CCB50(int64_t item_obj)
{
    if (dword_6016F4 < MT_AI_MAX_ENTRIES) {
        stru_600A70[dword_6016F4].spell = -1;
        stru_600A70[dword_6016F4].item_obj = item_obj;
        dword_6016F4++;
    }
}

// 0x4CCB90
void flush_actions(AiActionList* ai_action_list, int action)
{
    ai_action_list->actions[action].cnt = dword_6016F4;
    if (ai_action_list->actions[action].entries != NULL) {
        FREE(ai_action_list->actions[action].entries);
    }

    ai_action_list->actions[action].entries = (AiActionListEntry*)MALLOC(sizeof(AiActionListEntry) * dword_6016F4);
    memcpy(ai_action_list->actions[action].entries, stru_600A70, sizeof(AiActionListEntry) * dword_6016F4);
}

// 0x4CCBF0
void mt_ai_action_list_destroy(AiActionList* a1)
{
    (void)a1;
}

// 0x4CCC00
void mt_ai_notify_inventory_changed(int64_t obj)
{
    int slot;

    for (slot = 0; slot < MT_AI_MAX_LISTS; slot++) {
        if (mt_ai_action_lists[slot].obj == obj) {
            sub_4CC520(slot);
        }
    }
}

// 0x4CCC40
void mt_ai_notify_item_exhausted(int64_t obj, int64_t item_obj)
{
    int slot;
    int action;
    int idx;

    if (obj == OBJ_HANDLE_NULL) {
        return;
    }

    for (slot = 0; slot < MT_AI_MAX_LISTS; slot++) {
        if (mt_ai_action_lists[slot].obj == obj) {
            break;
        }
    }

    if (slot >= MT_AI_MAX_LISTS) {
        return;
    }

    for (action = 0; action < 10; action++) {
        for (idx = 0; idx < mt_ai_action_lists[slot].actions[action].cnt; idx++) {
            if (mt_ai_action_lists[slot].actions[action].entries[idx].item_obj == item_obj) {
                sub_4CC520(slot);
                return;
            }
        }
    }
}
