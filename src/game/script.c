#include "game/script.h"

#include <tig/tig.h>

#include "game/lib/object.h"

#define MAX_CACHE_ENTRIES 100
#define MAX_GL_VARS 2000
#define MAX_GL_FLAGS 100

typedef struct ScriptCacheEntry {
    /* 0000 */ int script_id;
    /* 0008 */ long long datetime;
    /* 0010 */ int ref_count;
    /* 0014 */ void* file;
} ScriptCacheEntry;

static_assert(sizeof(ScriptCacheEntry) == 0x18, "wrong size");

// 0x5A56FC
static int script_story_state_mes_file = -1;

// 0x5E2FA0
static int* script_gl_vars;

// 0x5E2FA4
static int dword_5E2FA4;

// 0x5E2FA8
static int* script_gl_flags;

// 0x5E2FAC
static void* dword_5E2FAC;

// 0x5E2FDC
static int dword_5E2FDC;

// 0x5E2FE0
static int dword_5E2FE0;

// 0x5E2FE4
static int dword_5E2FE4;

// 0x5E2FE8
static void* dword_5E2FE8;

// 0x5E2FEC
static void* dword_5E2FEC;

// 0x5E2FF0
static ScriptCacheEntry* script_cache_entries;

// 0x4446E0
void script_init(GameInitInfo* init_info)
{
    int index;

    dword_5E2FDC = init_info->type;
    script_cache_entries = (ScriptCacheEntry*)CALLOC(MAX_CACHE_ENTRIES, sizeof(ScriptCacheEntry));
    script_gl_vars = (int*)CALLOC(MAX_GL_VARS, sizeof(int));
    script_gl_flags = (int*)CALLOC(MAX_GL_FLAGS, sizeof(int));

    for (index = 0; index < MAX_CACHE_ENTRIES; index++) {
        script_cache_entries[index].script_id = 0;
    }

    dword_5E2FEC = NULL;
    dword_5E2FE8 = NULL;
    dword_5E2FA4 = 0;

    for (index = 0; index < MAX_GL_VARS; index++) {
        script_gl_vars[index] = 0;
    }

    for (index = 0; index < MAX_GL_FLAGS; index++) {
        script_gl_flags[index] = 0;
    }

    dword_5E2FAC = init_info->field_8;
    dword_5E2FE0 = init_info->field_C;

    if (dword_5E2FDC != EDITOR) {
        if (!animfx_list_init(&stru_5E2FB0)) {
            return false;
        }

        stru_5E2FB0.path = "Rules\\ScriptEyeCandy.mes";
        stru_5E2FB0.field_18 = 10;

        if (!animfx_list_load(&stru_5E2FB0)) {
            return false;
        }

    }

    return true;
}

// 0x4447D0
void script_reset()
{
    int index;

    for (index = 0; index < MAX_CACHE_ENTRIES; index++) {
        cache_remove(index);
    }

    dword_5E2FA4 = 0;

    for (index = 0; index < MAX_GL_VARS; index++) {
        script_gl_vars[index] = 0;
    }

    for (index = 0; index < MAX_GL_FLAGS; index++) {
        script_gl_flags[index] = 0;
    }
}

// 0x444830
void script_exit()
{
    int index;

    for (index = 0; index < MAX_CACHE_ENTRIES; index++) {
        cache_remove(index);
    }

    dword_5E2FA4 = 0;

    dword_5E2FA4 = 0;
    FREE(script_cache_entries);
    FREE(script_gl_vars);
    FREE(script_gl_flags);

    if (dword_5E2FDC != EDITOR) {
        animfx_list_exit(&stru_5E2FB0);
    }
}

// 0x444890
bool script_mod_load()
{
    message_load("mes\\storystate.mes", &script_story_state_mes_file);
    return true;
}

// 0x4448B0
void script_mod_unload()
{
    if (script_story_state_mes_file != -1) {
        message_unload(script_story_state_mes_file);
        script_story_state_mes_file = -1;
    }
}

// 0x4448D0
bool script_load(GameLoadInfo* load_info)
{
    if (tig_file_fread(script_gl_vars, sizeof(int) * MAX_GL_VARS, 1, load_info->stream) != 1) return false;
    if (tig_file_fread(script_gl_flags, sizeof(int) * MAX_GL_FLAGS, 1, load_info->stream) != 1) return false;
    if (tig_file_fread(&dword_5E2FA4, sizeof(int), 1, load_info->stream) != 1) return false;

    return true;
}

// 0x444930
void script_save(TigFile* stream)
{
    if (tig_file_fwrite(script_gl_vars, sizeof(int) * MAX_GL_VARS, 1, stream) != 1) return false;
    if (tig_file_fwrite(script_gl_flags, sizeof(int) * MAX_GL_FLAGS, 1, stream) != 1) return false;
    if (tig_file_fwrite(&dword_5E2FA4, sizeof(int), 1, stream) != 1) return false;

    return true;
}

// 0x444990
void sub_444990()
{
    // TODO: Incomplete.
}

// 0x4449B0
void sub_4449B0()
{
    // TODO: Incomplete.
}

// 0x444C80
int script_gl_var_get(int index)
{
    return script_gl_vars[index];
}

// 0x444C90
void script_gl_var_set(int index, int value)
{
    if (!sub_4A2BA0()) {
        unsigned char packet[16];

        if ((tig_net_flags & 0x2) == 0) {
            return;
        }

        *(int*)(&(packet[0])) = 124;
        *(int*)(&(packet[4])) = 1;
        *(int*)(&(packet[8])) = index;
        *(int*)(&(packet[12])) = value;
        sub_5295F0(packet, sizeof(packet));
    }

    script_gl_vars[index] = value;
}

// 0x444CF0
int script_gl_flag_get(int index)
{
    return (script_gl_flags[index / 32] >> (index % 32)) & 1;
}

// 0x444D20
void script_gl_flag_set(int index, int value)
{
    if (!sub_4A2BA0()) {
        unsigned char packet[16];

        if ((tig_net_flags & 0x2) == 0) {
            return;
        }

        *(int*)(&(packet[0])) = 124;
        *(int*)(&(packet[4])) = 2;
        *(int*)(&(packet[8])) = index;
        *(int*)(&(packet[12])) = value;
        sub_5295F0(packet, sizeof(packet));
    }

    script_gl_flags[index / 32] &= ~(1 << (index % 32));
    script_gl_flags[index / 32] |= value << (index % 32);
}

// 0x444DB0
int script_pc_gl_var_get(long long obj, int index)
{
    if (obj_field_int32_get(obj, OBJ_F_TYPE) == OBJ_TYPE_PC) {
        return sub_407470(obj, OBJ_F_PC_GLOBAL_VARIABLES, index);
    }

    return 0;
}

// 0x444DF0
void script_pc_gl_var_set(long long obj, int index, int value)
{
    if (obj_field_int32_get(obj, OBJ_F_TYPE) == OBJ_TYPE_PC) {
        obj_f_set_int32_with_network(obj, OBJ_F_PC_GLOBAL_VARIABLES, index, value);
    }
}

// 0x444E30
int script_pc_gl_flag_get(long long obj, int index)
{
    int flags;

    if (obj_field_int32_get(obj, OBJ_F_TYPE) == OBJ_TYPE_PC) {
        flags = sub_407470(obj, OBJ_F_PC_GLOBAL_FLAGS, index / 32);
        return (flags >> (index % 32)) & 1;
    }

    return 0;
}

// 0x444E90
void script_pc_gl_flag_set(long long obj, int index, int value)
{
    int flags;

    if (obj_field_int32_get(obj, OBJ_F_TYPE) == OBJ_TYPE_PC) {
        flags = sub_407470(obj, OBJ_F_PC_GLOBAL_FLAGS, index / 32);
        flags &= ~(1 << (index % 32));
        flags |= value << (index % 32);
        obj_f_set_int32_with_network(obj, OBJ_F_PC_GLOBAL_FLAGS, index / 32, flags);
    }
}

// 0x444F10
void sub_444F10()
{
    // TODO: Incomplete.
}

// 0x444F60
void sub_444F60()
{
    // TODO: Incomplete.
}

// 0x444FD0
void sub_444FD0()
{
    // TODO: Incomplete.
}

// 0x445020
void sub_445020()
{
    // TODO: Incomplete.
}

// 0x445090
int sub_445090()
{
    return dword_5E2FA4;
}

// 0x4450A0
void sub_4450A0()
{
    // TODO: Incomplete.
}

// 0x4450F0
const char* sub_4450F0(int story_state_num)
{
    MesFileEntry mes_file_entry;

    if (script_story_state_mes_file == -1) {
        return "";
    }

    mes_file_entry.num = story_state_num;
    if (!message_find(script_story_state_mes_file, &mes_file_entry)) {
        return "";
    }

    sub_4D43A0(script_story_state_mes_file, &mes_file_entry);

    return mes_file_entry.text;
}

// 0x445140
void script_timeevent_process()
{
    // TODO: Incomplete.
}

// 0x4451B0
void script_execute_condition()
{
    // TODO: Incomplete.
}

// 0x447220
int script_execute_action(ScriptAction* action, int a2, ScriptState* state)
{
    int value1;
    int value2;
    long long obj;

    switch (action->type) {
    case SAT_RETURN_AND_SKIP_DEFAULT:
        return -2;
    case SAT_RETURN_AND_RUN_DEFAULT:
        return -3;
    case SAT_GOTO:
        return script_get_value(action->op_type[0], action->op_value[0], state);
    case SAT_DIALOG:
        if (dword_5E2FEC != NULL && sub_40DA20(state->field_0->triggerer_obj)) {
            int num;
            int scr[3];

            num = script_get_value(action->op_type[0], action->op_value[0], state);
            sub_407840(state->field_0->attachee_obj, OBJ_F_SCRIPTS_IDX, state->field_0->field_20, scr);

            if (scr[0] != state->field_0->field_0->field_0
                || scr[1] != state->field_0->field_0->field_4) {
                scr[0] = state->field_0->field_0->field_0;
                scr[1] = state->field_0->field_0->field_4;
                sub_4078A0(state->field_0->attachee_obj, OBJ_F_SCRIPTS_IDX, state->field_0->field_20, scr);
            }

            dword_5E2FEC(state->field_0->triggerer_obj,
                state->field_0->attachee_obj,
                state->field_0->field_0->script_num,
                a2,
                num);
        }
        return -2;
    case SAT_REMOVE_THIS_SCRIPT:
        state->script_num = 0;
        return -1;
    case SAT_CHANGE_THIS_SCRIPT_TO_SCRIPT:
        state->script_num = script_get_value(action->op_type[0], action->op_value[0], state);
        return -1;
    case SAT_CALL_SCRIPT:
        sub_44B030(action, state);
        return -1;
    case SAT_SET_LOCAL_FLAG:
        state->field_0->field_0->field_0 |= 1 << script_get_value(action->op_type[0], action->op_value[0], state);
        return -1;
    case SAT_CLEAR_LOCAL_FLAG:
        state->field_0->field_0->field_0 &= ~(1 << script_get_value(action->op_type[0], action->op_value[0], state));
        return -1;
    case SAT_ASSIGN_NUM:
        script_set_value(action->op_type[0], action->op_value[0], state,
            script_get_value(action->op_type[1], action->op_value[1], state));
        return -1;
    case SAT_ADD:
        value1 = script_get_value(action->op_type[1], action->op_value[1], state);
        value2 = script_get_value(action->op_type[2], action->op_value[2], state);
        script_set_value(action->op_type[0],
            action->op_value[0],
            state,
            value1 + value2);
        return -1;
    case SAT_SUBTRACT:
        value1 = script_get_value(action->op_type[1], action->op_value[1], state);
        value2 = script_get_value(action->op_type[2], action->op_value[2], state);
        script_set_value(action->op_type[0],
            action->op_value[0],
            state,
            value1 - value2);
        return -1;
    case SAT_MULTIPLY:
        value1 = script_get_value(action->op_type[1], action->op_value[1], state);
        value2 = script_get_value(action->op_type[2], action->op_value[2], state);
        script_set_value(action->op_type[0],
            action->op_value[0],
            state,
            value1 * value2);
        return -1;
    case SAT_DIVIDE:
        value1 = script_get_value(action->op_type[1], action->op_value[1], state);
        value2 = script_get_value(action->op_type[2], action->op_value[2], state);
        if (value2 != 0) {
            script_set_value(action->op_type[0],
                action->op_value[0],
                state,
                value1 + value2);
        }
        return -1;
    case SAT_ASSIGN_OBJ:
        obj = script_get_obj(action->op_type[1], action->op_value[1], state);
        script_set_obj(action->op_type[0], action->op_value[0], state, obj);
        return -1;
    case SAT_SET_PC_QUEST_STATE:
        if (1) {
            int cnt;
            int quest_num;
            int quest_state;
            int index;

            cnt = script_resolve_focus_obj(action->op_type[0], action->op_value[0], state, objs, &l);
            quest_num = script_get_value(action->op_type[1], action->op_value[1], state);
            quest_state = script_get_value(action->op_type[2], action->op_value[2], state);

            for (index = 0; index < cnt; index++) {
                sub_4C4D20(objs[index],
                    quest_num,
                    quest_state,
                    state->field_0->attachee_obj);
            }

            sub_44B8F0(action->op_type[0], &l);
        }
        return -1;
    case SAT_SET_QUEST_GLOBAL_STATE:
        if (1) {
            int quest_num;
            int quest_state;

            quest_num = script_get_value(action->op_type[1], action->op_value[1], state);
            quest_state = script_get_value(action->op_type[2], action->op_value[2], state);
            quest_set_state(quest_num, quest_state);
        }
        return -1;
    case SAT_LOOP_FOR:
        if (state->loop_cnt != 0) {
            tig_debug_printf("Script: script_execute_action: sat_loop_for: ERROR: Already in a loop!\n");
            return -1;
        }

        state->field_4 = 0;
        state->field_C = a2 + 1;
        state->field_38 = action->op_type[0];
        state->loop_cnt = script_resolve_focus_obj(action->op_type[0],
            action->op_value[0],
            state,
            state->field_20,
            &(state->l));
        if (state->loop_cnt != 0) {
            state->current_loop_obj = state->field_20[state->field_4];
            return -1;
        }

        sub_44B8F0(state->field_398, &(state->l));
        return sub_44BC60(state);
    case SAT_LOOP_END:
        if (state->loop_cnt <= 0) {
            tig_debug_printf("Script: script_execute_action: sat_loop_end: ERROR: Not in a loop!\n");
            return -1;
        }

        if (++state->field_4 < state->loop_cnt) {
            state->current_loop_obj = state->field_20[state->field_4];
            return state->field_C;
        }

        state->loop_cnt = 0;
        sub_44B8F0(state->field_398, &(state->l));
        return -1;
    case SAT_LOOP_BREAK:
        if (state->loop_cnt <= 0) {
            tig_debug_printf("Script: script_execute_action: sat_loop_break: ERROR: Not in a loop!\n");
            return -1;
        }

        state->loop_cnt = 0;
        sub_44B8F0(state->field_398, &(state->l));
        return sub_44BC60(state);
    case SAT_CRITTER_FOLLOW:
        if (1) {
            long long follower;
            long long leader;

            follower = script_get_obj(action->op_type[0], action->op_value[0], state);
            leader = script_get_obj(action->op_type[1], action->op_value[1], state);
            critter_follow(follower, leader, true);
        }
        return -1;
    case SAT_CRITTER_DISBAND:
        if (1) {
            long long obj;

            obj = script_get_obj(action->op_type[0], action->op_value[0], state);;

            critter_disband(obj, true);
        }
        return -1;
    case SAT_FLOAT_LINE:
        sub_44B1F0(action, state);
        return -1
    case SAT_PRINT_LINE:
        sub_44B390(action, state);
        return -1;
    }
}

// 0x44AFF0
void sub_44AFF0()
{
    // TODO: Incomplete.
}

// 0x44B030
void sub_44B030()
{
    // TODO: Incomplete.
}

// 0x44B170
void sub_44B170(ScriptAction* action, ScriptState* state)
{

    sub_441980(script_get_obj(action->op_type[3], action->op_action[3], state),
        script_get_obj(action->op_type[0], action->op_action[0], state),
        0,
        0,
        script_get_value(action->op_type[1], action->op_value[1], state),
        script_get_value(action->op_type[2], action->op_value[2], state));
}

// 0x44B1F0
void sub_44B1F0()
{
    // TODO: Incomplete.
}

// 0x44B390
void sub_44B390()
{
    // TODO: Incomplete.
}

// 0x44B4E0
int script_resolve_focus_obj(ScriptFocusObject type, int index, ScriptState* state, long long* objs, ObjectNodeList* l)
{
    ObjectListNode* node;
    int cnt = 0;

    switch (type) {
    case SFO_TRIGGERER:
        objs[cnt++] = state->field_0->triggerer_obj;
        break;
    case SFO_ATTACHEE:
        objs[cnt++] = state->field_0->attachee_obj;
        break;
    case SFO_EVERY_FOLLOWER:
    case SFO_ANY_FOLLOWER:
        sub_4411C0(state->field_0->triggerer_obj, l);

        node = l->head;
        while (node != NULL && cnt < 100) {
            objs[cnt++] = node->obj;
            node = node->next;
        }
        break;
    case SFO_EVERYONE_IN_PARTY:
    case SFO_ANYONE_IN_PARTY:
        sub_441310(state->field_0->triggerer_obj, l);

        node = l->head;
        while (node != NULL && cnt < 100) {
            objs[cnt++] = node->obj;
            node = node->next;
        }
        break;
    case SFO_EVERYONE_IN_TEAM:
    case SFO_ANYONE_IN_TEAM:
        sub_4413E0(state->field_0->triggerer_obj, l);

        node = l->head;
        while (node != NULL && cnt < 100) {
            objs[cnt++] = node->obj;
            node = node->next;
        }
        break;
    case SFO_EVERYONE_IN_VICINITY:
    case SFO_ANYONE_IN_VICINITY:
        if (state->field_0->attachee_obj != NULL) {
            sub_440FC0(state->field_0->attachee_obj, 0x18000, l);

            node = l->head;
            while (node != NULL && cnt < 100) {
                objs[cnt++] = node->obj;
                node = node->next;
            }
        } else {
            if (l != NULL) {
                l->head = NULL;
                l->num_sectors = 0;
            }
        }
        break;
    case SFO_CURRENT_LOOPED_OBJECT:
        objs[cnt++] = state->current_loop_obj;
        break;
    case SFO_LOCAL_OBJECT:
        objs[cnt++] = state->lc_objs[index];
        break;
    case SFO_EXTRA_OBJECT:
        objs[cnt++] = state->field_0->extra_obj;
        break;
    case SFO_EVERYONE_IN_GROUP:
    case SFO_ANYONE_IN_GROUP:
        sub_4411C0(state->field_0->triggerer_obj, l);

        node = l->head;
        while (node != NULL && cnt < 100) {
            objs[cnt++] = node->obj;
            node = node->next;
        }

        // FIXME: Possible overflow when cnt == 100.
        objs[cnt++] = state->field_0->triggerer_obj;
        break;
    case SFO_EVERY_SCENERY_IN_VICINITY:
    case SFO_ANY_SCENERY_IN_VICINITY:
        if (state->field_0->attachee_obj != NULL) {
            sub_440FC0(state->field_0->attachee_obj, 0x8, l);

            node = l->head;
            while (node != NULL && cnt < 100) {
                objs[cnt++] = node->obj;
                node = node->next;
            }
        } else {
            if (l != NULL) {
                l->head = NULL;
                l->num_sectors = 0;
            }
        }
        break;
    case SFO_EVERY_CONTAINER_IN_VICINITY:
    case SFO_ANY_CONTAINER_IN_VICINITY:
        if (state->field_0->attachee_obj != NULL) {
            sub_440FC0(state->field_0->attachee_obj, 0x4, l);

            node = l->head;
            while (node != NULL && cnt < 100) {
                objs[cnt++] = node->obj;
                node = node->next;
            }
        } else {
            if (l != NULL) {
                l->head = NULL;
                l->num_sectors = 0;
            }
        }
        break;
    case SFO_EVERY_PORTAL_IN_VICINITY:
    case SFO_ANY_PORTAL_IN_VICINITY:
        if (state->field_0->attachee_obj != NULL) {
            sub_440FC0(state->field_0->attachee_obj, 0x2, l);

            node = l->head;
            while (node != NULL && cnt < 100) {
                objs[cnt++] = node->obj;
                node = node->next;
            }
        } else {
            if (l != NULL) {
                l->head = NULL;
                l->num_sectors = 0;
            }
        }
        break;
    case SFO_PLAYER:
        objs[cnt++] = player_get_pc_obj();
        break;
    case SFO_EVERY_ITEM_IN_VICINITY:
    case SFO_ANY_ITEM_IN_VICINITY:
        if (state->field_0->attachee_obj != NULL) {
            sub_440FC0(state->field_0->attachee_obj, 0x7FE0, l);

            node = l->head;
            while (node != NULL && cnt < 100) {
                objs[cnt++] = node->obj;
                node = node->next;
            }
        } else {
            if (l != NULL) {
                l->head = NULL;
                l->num_sectors = 0;
            }
        }
        break;
    }

    return cnt;
}

// 0x44B8F0
void sub_44B8F0(ScriptFocusObject type, ObjectNodeList* l)
{
    switch (type) {
    case SFO_EVERY_FOLLOWER:
    case SFO_ANY_FOLLOWER:
    case SFO_EVERYONE_IN_PARTY:
    case SFO_ANYONE_IN_PARTY:
    case SFO_EVERYONE_IN_TEAM:
    case SFO_ANYONE_IN_TEAM:
    case SFO_EVERYONE_IN_VICINITY:
    case SFO_ANYONE_IN_VICINITY:
    case SFO_CURRENT_LOOPED_OBJECT:
    case SFO_LOCAL_OBJECT:
    case SFO_EXTRA_OBJECT:
    case SFO_EVERYONE_IN_GROUP:
    case SFO_ANYONE_IN_GROUP:
    case SFO_EVERY_SCENERY_IN_VICINITY:
    case SFO_ANY_SCENERY_IN_VICINITY:
    case SFO_EVERY_CONTAINER_IN_VICINITY:
    case SFO_ANY_CONTAINER_IN_VICINITY:
    case SFO_EVERY_PORTAL_IN_VICINITY:
    case SFO_ANY_PORTAL_IN_VICINITY:
    case SFO_EVERY_ITEM_IN_VICINITY:
    case SFO_ANY_ITEM_IN_VICINITY:
        if (l->head != NULL) {
            sub_4410E0(l);
        }
        break;
    }
}

// 0x44B960
long long script_get_obj(ScriptFocusObject type, int index, ScriptState* state)
{
    ObjectNodeList l;
    long long objs[100];

    script_resolve_focus_obj(type, index, state, objs, &l);
    sub_44B8F0(type, &l);

    return objs[0];
}

// 0x44B9B0
void script_set_obj(ScriptFocusObject type, int index, ScriptState* state, long long obj)
{
    switch (type) {
    case SFO_TRIGGERER:
        state->field_0->triggerer_obj = obj;
        break;
    case SFO_ATTACHEE:
        state->field_0->attachee_obj = obj;
        break;
    case SFO_CURRENT_LOOPED_OBJECT:
        state->current_loop_obj = obj;
        break;
    case SFO_LOCAL_OBJECT:
        state->lc_objs[index] = obj;
        break;
    case SFO_EXTRA_OBJECT:
        state->field_0->extra_obj = obj;
        break;
    }
}

// 0x44BA70
int script_get_value(ScriptValueType type, int index, ScriptState* state)
{
    long long obj;

    switch (type) {
    case SVT_COUNTER:
        return (state->field_0->field_0->field_4 >> (8 * index)) & 0xFF;
    case SVT_GL_VAR:
        return script_get_gl_var(index);
    case SVT_LC_VAR:
        return state->lc_vars[index];
    case SVT_NUMBER:
        return index;
    case SVT_GL_FLAG:
        return script_get_gl_flag(index);
    case SVT_PC_VAR:
        obj = script_get_obj(index >> 16, 0, state);
        return script_get_pc_var(obj, index & 0xFFFF);
    case SVT_PC_FLAG:
        obj = script_get_obj(index >> 16, 0, state);
        return script_get_pc_flag(obj, index & 0xFFFF);
    }

    return index;
}

// 0x44BB50
void script_set_value(ScriptValueType type, int index, ScriptState* state, int value)
{
    long long obj;

    switch (type) {
    case SVT_COUNTER:
        state->field_0->field_0->field_4 &= ~(0xFF << (8 * index));
        state->field_0->field_0->field_4 |= value << (8 * index);
        break;
    case SVT_GL_VAR:
        script_set_gl_var(index, value);
        break;
    case SVT_LC_VAR:
        state->lc_vars[index] = value;
        break;
    case SVT_GL_FLAG:
        script_set_gl_flag(index, value);
        break;
    case SVT_PC_VAR:
        obj = script_get_obj(index >> 16, 0, state);
        script_set_pc_var(obj, index & 0xFFFF, value);
        break;
    case SVT_PC_FLAG:
        obj = script_get_obj(index >> 16, 0, state);
        script_set_pc_flag(obj, index & 0xFFFF, value);
        break;
    }
}

// 0x44BC60
void sub_44BC60()
{
    // TODO: Incomplete.
}

// 0x44BCC0
void sub_44BCC0()
{
    // TODO: Incomplete.
}

// 0x44BD30
void sub_44BD30()
{
    // TODO: Incomplete.
}

// 0x44BD60
void sub_44BD60()
{
    // TODO: Incomplete.
}

// 0x44C0A0
void sub_44C0A0()
{
    // TODO: Incomplete.
}

// 0x44C100
void sub_44C100()
{
    // TODO: Incomplete.
}

// 0x44C130
void sub_44C130()
{
    // TODO: Incomplete.
}

// 0x44C140
bool sub_44C140(void* a1, unsigned int index, ScriptFileEntry* entry)
{
    ScriptFile* script_file;
    bool success;

    script_file = script_lock(a1->script_num);
    if (script_file != NULL) {
        success = sub_44C1B0(script_file, index, entry);
        script_unlock(a1->script_num);
        return success;
    }

    return false;
}

// 0x44C1B0
bool sub_44C1B0(ScriptFile* script_file, unsigned int index, ScriptFileEntry* entry)
{
    if (index < script_file->num_entries) {
        memcpy(entry, &(script_file->entries[index]), sizeof(ScriptFileEntry));
        return true;
    }

    return false;
}

// 0x44C310
bool sub_44C310(void* a1, int* a2)
{
    ScriptFile* script_file;

    script_file = script_lock(a1->script_num);
    if (script_file != NULL) {
        *a2 = script_file->field_28;
        script_unlock(a1->script_num);
        return true;
    }

    return false;
}

// 0x44C390
ScriptFile* script_lock(int script_id)
{
    int cache_entry_id;

    if (dword_5E2FDC == 1) {
        // NOTE: Looks odd.
        return script_id;
    }

    cache_entry_id = cache_find(script_id);
    if (script_cache_entries[cache_entry_id].script_id == script_id) {
        script_cache_entries[cache_entry_id].ref_count++;
        script_cache_entries[cache_entry_id].datetime = sub_45A7C0();
        return script_cache_entries[cache_entry_id].file;
    }

    if (script_cache_entries[cache_entry_id].script_id != 0) {
        cache_remove(cache_entry_id);
    }

    if (cache_add(cache_entry_id, script_id)) {
        script_cache_entries[cache_entry_id].ref_count++;
        script_cache_entries[cache_entry_id].datetime = sub_45A7C0();
        return script_cache_entries[cache_entry_id].file;
    }

    return NULL;
}

// 0x44C450
void script_unlock(int script_id)
{
    int cache_entry_id;

    if (dword_5E2FDC != 1) {
        cache_entry_id = cache_find(script_id);
        script_cache_entries[cache_entry_id].ref_count--;
    }
}

// 0x44C480
bool script_file_create(ScriptFile** script_file_ptr)
{
    ScriptFile* script_file;

    script_file = *script_file_ptr = (ScriptFile*)MALLOC(sizeof(ScriptFile));
    script_file->field_0 = 0;
    script_file->field_28 = 0;
    script_file->num_entries = 0;
    script_file->cap_entries = 0;
    script_file->entries = NULL;

    return true;
}

// 0x44C4B0
bool script_file_destroy(ScriptFile* script_file)
{
    if (script_file->cap_entries > 0) {
        FREE(script_file->entries);
    }

    FREE(script_file);

    return true;
}

// 0x44C4E0
bool cache_add(int cache_entry_id, int script_id)
{
    char path[TIG_MAX_PATH];
    TigFile* stream;

    if (!script_name_build_scr_name(script_id, path)) {
        tig_debug_printf("Script: cache_add: ERROR: Failed to build script name: %d!\n", script_id);
        return false;
    }

    stream = tig_file_fopen(path, "rb");
    if (stream == NULL) {
        tig_debug_printf("Script: cache_add: ERROR: Failed to load script file: %d!\n", script_id);
        return false;
    }

    if (!sub_44C710(script, vars)) {
        tig_debug_printf("Script: cache_add: ERROR: Failed to load script variables: %d!\n", script_id);
        // FIXME: Leaking stream.
        return false;
    }

    if (!script_file_create(&(script_cache_entries[cache_entry_id].file))) {
        tig_debug_printf("Script: cache_add: ERROR: Failed to build script code: %d!\n", script_id);
        // FIXME: Leaking stream.
        return false;
    }

    if (sub_44C730(stream, script_cache_entries[cache_entry_id].file)) {
        tig_debug_printf("Script: cache_add: ERROR: Failed to load script code: %d!\n", script_id);
        // FIXME: Leaking stream.
        return false;
    }

    tig_file_fclose(stream);

    script_cache_entries[cache_entry_id].script_id = script_id;
    script_cache_entries[cache_entry_id].ref_count = 0;

    return true;
}

// 0x44C630
void cache_remove(int cache_entry_id)
{
    if (script_cache_entries[cache_entry_id].script_id) {
        script_file_destroy(script_cache_entries[cache_entry_id].file)
        script_cache_entries[cache_entry_id].script_id = 0;
    }
}

// 0x44C670
void cache_find()
{
    // TODO: Incomplete.
}

// 0x44C710
void sub_44C710()
{
    // TODO: Incomplete.
}

// 0x44C730
bool sub_44C730(TigFile* stream, ScriptFile* script_file)
{
    if (tig_file_fread(script_file, sizeof(ScriptFile), 1, stream) != 1) {
        return false;
    }

    if (script_file->num_entries > 0) {
        script_file->entries = (ScriptFileEntry*)CALLOC(script_file->cap_entries, sizeof(ScriptFileEntry));
        if (tig_file_fread(script_file->entries, sizeof(ScriptFileEntry), script_file->num_entries, stream) != script_file->num_entries) {
            // FIXME: Leaks entries.
            return false;
        }
    } else {
        script_file->entries = NULL;
    }

    return true;
}
