#include "game/anim_private.h"

#include "game/anim.h"
#include "game/critter.h"
#include "game/mp_utils.h"
#include "game/player.h"
#include "game/random.h"
#include "game/timeevent.h"
#include "game/ui.h"

static bool sub_44D240(int index);

// 0x5A5978
int dword_5A5978 = -1;

// 0x5B052C
static int dword_5B052C = -1;

// 0x5E3000
static AnimID stru_5E3000;

// 0x5E34F4
bool dword_5E34F4;

// 0x5E34F8
void(*dword_5E34F8)();

// 0x5E34FC
bool in_anim_load;

// 0x5E3500
int dword_5E3500;

// 0x5E3504
int animNumActiveGoals;

// 0x5E3508
static bool anim_private_editor;

// 0x6876E4
int dword_6876E4;

// 0x687700
AnimRunInfo anim_run_info[216];

// 0x739E40
int dword_739E40;

// 0x739E44
int dword_739E44;

// 0x44C840
void sub_44C840(AnimRunInfo* run_info, AnimGoalNode* goal_node)
{
    ASSERT(run_info != NULL); // pRunInfo != NULL
    ASSERT(goal_node != NULL); // pGoalNode != NULL

    if (goal_node->priority_level >= 2 && !goal_node->field_8 && !sub_44C9A0(run_info)) {
        ASSERT(animNumActiveGoals >= 0); // animNumActiveGoals >= 0

        animNumActiveGoals++;
    }
}

// 0x44C8F0
void sub_44C8F0(AnimRunInfo* run_info, AnimGoalNode* goal_node)
{
    ASSERT(run_info != NULL); // pRunInfo != NULL
    ASSERT(goal_node != NULL); // pGoalNode != NULL

    if (goal_node->priority_level >= 2 && !goal_node->field_8 && !sub_44C9A0(run_info)) {
        ASSERT(animNumActiveGoals >= 1); // animNumActiveGoals >= 1

        if (animNumActiveGoals >= 1) {
            animNumActiveGoals--;
        }
    }
}

// 0x44C9A0
bool sub_44C9A0(AnimRunInfo* run_info)
{
    ASSERT(run_info != NULL); // pRunInfo != NULL

    if ((run_info->field_C & 0x1) == 0) {
        return false;
    }

    ASSERT(run_info->cur_stack_data); // pRunInfo->pCurStackData != NULL

    if (run_info->cur_stack_data == NULL) {
        ASSERT(run_info->current_goal >= 0); // pRunInfo->current_goal >= 0

        if (run_info->current_goal < 0) {
            return false;
        }

        run_info->cur_stack_data = &(run_info->goals[run_info->current_goal]);
    }

    return (off_5B03D0[run_info->cur_stack_data->type]->field_10 & 0x1) != 0
        && (run_info->cur_stack_data->params[AGDATA_FLAGS_DATA].data & 0x80) != 0;
}

// 0x44CA70
bool anim_private_init(GameInitInfo* init_info)
{
    int index;

    anim_private_editor = init_info->editor;

    for (index = 0; index < 216; index++) {
        anim_run_info[index].id.slot_num = index;
        anim_run_info[index].field_C = 0;
        anim_run_info[index].path.flags = 1;
        anim_run_info[index].path.field_CC = 200;
        sub_44EBD0(&(anim_run_info[index].path));
    }

    dword_6876E4 = random_between(0, 10024);
    animNumActiveGoals = 0;
    dword_5E3500 = 0;

    return true;
}

// 0x44CAF0
void anim_private_exit()
{
    int index;

    for (index = 0; index < 216; index++) {
        anim_run_info[index].field_C = 0;
        anim_run_info[index].path.flags = 1;
        sub_44EBE0(&(anim_run_info[index].path));
    }

    animNumActiveGoals = 0;
}

// 0x44CB30
void anim_private_reset()
{
    int index;

    for (index = 0; index < 216; index++) {
        anim_run_info[index].field_C = 0;
        anim_run_info[index].path.flags = 1;
    }

    animNumActiveGoals = 0;
    dword_5E3500 = 0;
}

// 0x44CB60
bool sub_44CB60()
{
    return dword_5A5978 != -1;
}

// 0x44CB70
bool sub_44CB70(TimeEvent* timeevent)
{
    ASSERT(timeevent != NULL); // 2766, "pTimeEvent"

    return timeevent->params[0].integer_value == stru_5E3000.slot_num;
}

// 0x44CBB0
bool anim_goal_restart(AnimID* anim_id)
{
    AnimRunInfo* run_info;
    char str[36];
    TimeEvent timeevent;

    ASSERT(anim_id != NULL); // pAnimID != NULL

    if (!anim_id_to_run_info(anim_id, &run_info)) {
        sub_421E20(anim_id, str);
        tig_debug_printf("Anim: anim_goal_restart: could not turn animID into a AnimRunInfo %s.\n");
        return false;
    }

    if ((run_info->field_C & 0x1) == 0) {
        return false;
    }

    run_info->field_C &= ~0x2;
    if (run_info->current_goal == -1) {
        run_info->current_goal = 0;
        run_info->field_10 = 0;
        run_info->field_14 = -1;
    }

    stru_5E3000 = run_info->id;
    timeevent_clear_one_ex(TIMEEVENT_TYPE_ANIM, sub_44CB70);

    timeevent.type = TIMEEVENT_TYPE_ANIM;
    timeevent.params[0].integer_value = run_info->id.slot_num;
    timeevent.params[1].integer_value = run_info->id.field_4;
    timeevent.params[2].integer_value = 2222;
    return sub_45B860(&timeevent, &(run_info->field_18));
}

// 0x44CCB0
bool sub_44CCB0(AnimID* anim_id)
{
    int index;
    AnimRunInfo* run_info;
    int subindex;

    ASSERT(anim_id != NULL); // pAnimID != NULL

    for (index = 0; index < 216; index++) {
        if ((anim_run_info[index].field_C & 0x1) == 0) {
            break;
        }
    }

    if (index == 216) {
        tig_debug_printf("Anim: WARNING: Ran out of animation slots!\n");
        dword_5E34F4 = 1;
        return false;
    }

    run_info = &(anim_run_info[index]);
    run_info->id.field_4 = dword_6876E4++;
    run_info->id.field_8 = 0;
    run_info->field_C = 1;
    run_info->path.maxPathLength = 0;
    *anim_id = run_info->id;

    run_info->field_20 = 0;
    run_info->cur_stack_data = NULL;
    run_info->field_18.days = 0;
    run_info->field_18.milliseconds = 0;
    run_info->goals[0].params[AGDATA_SELF_OBJ].obj = OBJ_HANDLE_NULL;
    run_info->goals[0].params[AGDATA_TARGET_OBJ].obj = OBJ_HANDLE_NULL;
    run_info->goals[0].params[AGDATA_BLOCK_OBJ].obj = OBJ_HANDLE_NULL;
    run_info->goals[0].params[AGDATA_SCRATCH_OBJ].obj = OBJ_HANDLE_NULL;
    run_info->goals[0].params[AGDATA_PARENT_OBJ].obj = OBJ_HANDLE_NULL;

    for (subindex = 0; subindex < 5; subindex++) {
        run_info->goals[0].field_B0[subindex].objid.type = OID_TYPE_NULL;
    }

    dword_5E3500++;

    return true;
}

// 0x44CDA0
void sub_44CDA0()
{
    // TODO: Incomplete.
}

// 0x44CF20
bool mp_deallocate_run_index(AnimID* anim_id)
{
    AnimRunInfo* run_info;
    char str[36];
    int stack_index;

    ASSERT(anim_id != NULL); // pAnimID != NULL

    if (!anim_id_to_run_info(anim_id, &run_info)) {
        sub_421E20(anim_id, str);
        tig_debug_printf("Anim: mp_deallocate_run_index: could not turn animID into a AnimRunInfo %s.\n", str);
        return false;
    }

    if ((run_info->field_C & 0x1) != 0) {
        if (run_info->goals[0].type == AG_ATTACK
            || run_info->goals[0].type == AG_ATTEMPT_ATTACK) {
            if (player_is_pc_obj(run_info->field_20)) {
                sub_460280(OBJ_HANDLE_NULL);
            }
        }

        if (run_info->current_goal >= 0) {
            for (stack_index = run_info->current_goal; stack_index <= run_info->current_goal; stack_index++) {
                if (run_info->goals[stack_index].type >= 0 && run_info->goals[stack_index].type < ANIM_GOAL_MAX) {
                    sub_44C8F0(run_info, off_5B03D0[run_info->goals[stack_index].type]);
                }
            }
        }

        sub_421DE0(&(run_info->id));
        run_info->cur_stack_data = NULL;
        run_info->field_20 = OBJ_HANDLE_NULL;
        run_info->field_C = 0;
        run_info->current_goal = -1;
        run_info->path.flags = 1;

        dword_5E3500--;
        sub_423E60("Free Run Index");

        if (animNumActiveGoals == 0) {
            if (dword_5E34F8 != NULL) {
                dword_5E34F8();
            }
        }
    } else {
        sub_421DE0(&(run_info->id));
        run_info->cur_stack_data = NULL;
        run_info->field_20 = 0;
        run_info->field_C = 0;
        run_info->current_goal = -1;
        run_info->path.flags = 1;
    }

    return true;
}

// 0x44D0C0
void sub_44D0C0(AnimRunInfo* run_info)
{
    (void)run_info;
}

// 0x44D0D0
void sub_44D0D0(AnimID* anim_id, int a2)
{
    (void)anim_id;
    (void)a2;
}

// 0x44D0E0
bool anim_mp_reap_run_index(AnimID* anim_id)
{
    AnimRunInfo* run_info;
    char str[36];

    ASSERT(tig_net_is_active()); // tig_net_is_active()
    ASSERT(anim_id != NULL); // pAnimID != NULL

    if (!anim_id_to_run_info(anim_id, &run_info)) {
        sub_421E20(anim_id, str);
        tig_debug_printf("Anim: anim_mp_reap_run_index: could not turn animID into a AnimRunInfo %s.\n", str);
        return false;
    }

    mp_deallocate_run_index(anim_id);

    return true;
}

// 0x44D180
bool anim_free_run_index(AnimID* anim_id)
{
    AnimRunInfo* run_info;
    char str[36];

    ASSERT(anim_id != NULL); // pAnimID != NULL

    if (!anim_id_to_run_info(anim_id, &run_info)) {
        sub_421E20(anim_id, str);
        tig_debug_printf("Anim: anim_free_run_index: could not turn animID into a AnimRunInfo %s.\n", str);
        return false;
    }

    if (tig_net_is_active()) {
        if (tig_net_is_host()) {
            sub_44D0D0(&(run_info->id), 0);
        }

        anim_mp_reap_run_index(&(run_info->id));

        return true;
    }

    return mp_deallocate_run_index(anim_id);
}

// 0x44D240
bool sub_44D240(int index)
{
    AnimRunInfo* run_info;

    run_info = &(anim_run_info[index]);
    run_info->field_20 = 0;
    run_info->cur_stack_data = NULL;
    run_info->field_C = 0;
    run_info->current_goal = -1;
    run_info->path.flags |= 0x1;

    stru_5E3000 = run_info->id;
    timeevent_clear_one_ex(TIMEEVENT_TYPE_ANIM, sub_44CB70);

    return true;
}

// 0x44D2F0
int sub_44D2F0(int64_t obj)
{
    // TODO: Incomplete.
}

// 0x44D340
int sub_44D340(int slot, int64_t obj)
{
    // TODO: Incomplete.
}

// 0x44D3B0
bool sub_44D3B0(AnimGoalData* goal_data, int64_t obj, int goal_type, bool a4)
{
    AnimGoalNode* goal_node;

    ASSERT(goal_data != NULL); // pAGoalData != NULL
    ASSERT(goal_type >= 0 && goal_type < ANIM_GOAL_MAX); // (goalType >= 0)&&(goalType < anim_goal_max)

    goal_data->type = goal_type;
    goal_data->params[AGDATA_SELF_OBJ].obj = obj;
    goal_data->params[AGDATA_TARGET_OBJ].obj = OBJ_HANDLE_NULL;
    goal_data->params[AGDATA_BLOCK_OBJ].obj = OBJ_HANDLE_NULL;
    goal_data->params[AGDATA_SCRATCH_OBJ].obj = OBJ_HANDLE_NULL;
    goal_data->params[AGDATA_PARENT_OBJ].obj = OBJ_HANDLE_NULL;
    goal_data->params[AGDATA_TARGET_TILE].loc = 0;
    goal_data->params[AGDATA_ORIGINAL_TILE].loc = 0;
    goal_data->params[AGDATA_RANGE_DATA].data = -1;
    goal_data->params[AGDATA_ANIM_ID].data = -1;
    goal_data->params[AGDATA_ANIM_ID_PREVIOUS].data = -1;
    goal_data->params[AGDATA_ANIM_DATA].data = -1;
    goal_data->params[AGDATA_SPELL_DATA].data = -1;
    goal_data->params[AGDATA_SKILL_DATA].data = -1;
    goal_data->params[AGDATA_FLAGS_DATA].data = 0;
    goal_data->params[AGDATA_SCRATCH_VAL1].data = 0;
    goal_data->params[AGDATA_SCRATCH_VAL2].data = 0;
    goal_data->params[AGDATA_SCRATCH_VAL3].data = 0;
    goal_data->params[AGDATA_SCRATCH_VAL4].data = 0;
    goal_data->params[AGDATA_SCRATCH_VAL5].data = 0;
    goal_data->params[AGDATA_SCRATCH_VAL6].data = 0;
    goal_data->params[AGDATA_SOUND_HANDLE].data = TIG_SOUND_HANDLE_INVALID;

    goal_node = off_5B03D0[goal_type];

    ASSERT(goal_node != NULL); // pGoalNode != NULL

    return sub_424070(obj, goal_node->priority_level, goal_node->field_8, 1);
}

// 0x44D4E0
bool sub_44D4E0(AnimGoalData* anim_data, int64_t obj, int goal_type)
{
    return sub_44D3B0(anim_data, obj, goal_type, true);
}

// 0x44D500
bool sub_44D500(AnimGoalData* anim_data, int64_t obj, int goal_type)
{
    return sub_44D3B0(anim_data, obj, goal_type, false);
}

// 0x44D520
bool sub_44D520(AnimGoalData* anim_data, AnimID* anim_id)
{
    return sub_44D540(anim_data, anim_id, false);
}

// 0x44D540
bool sub_44D540(AnimGoalData* anim_data, AnimID* anim_id, bool a3)
{
    Packet5 pkt;
    int index;
    AnimID v1;

    if (anim_private_editor) {
        return false;
    }

    if ((tig_net_flags & TIG_NET_CONNECTED) == 0) {
        return sub_44D730(anim_data, anim_id, 1, a3);
    }

    if (sub_45B300()
        || ((tig_net_flags & TIG_NET_HOST) == 0 && !player_is_pc_obj(anim_data->params[AGDATA_SELF_OBJ].obj))
        || ((tig_net_flags & TIG_NET_HOST) == 0 && anim_data->type == AG_DYING)) {
        return true;
    }

    pkt.type = 5;
    pkt.loc = obj_field_int64_get(anim_data->params[AGDATA_SELF_OBJ].obj, OBJ_F_LOCATION);
    pkt.offset_x = obj_field_int32_get(anim_data->params[AGDATA_SELF_OBJ].obj, OBJ_F_OFFSET_X);
    pkt.offset_y = obj_field_int32_get(anim_data->params[AGDATA_SELF_OBJ].obj, OBJ_F_OFFSET_Y);
    pkt.field_8 = sub_45A7C0();
    pkt.field_1A4 = a3;
    sub_421DE0(&(pkt.field_198));
    for (index = 0; index < 5; index++) {
        sub_443EB0(anim_data->params[index].obj, &(anim_data->field_B0[index]));
    }

    pkt.field_10 = *anim_data;

    if ((tig_net_flags & TIG_NET_HOST) != 0) {
        if (!sub_44D730(anim_data, &v1, 1, a3)) {
            return false;
        }

        pkt.field_198 = v1;
        if (anim_id != NULL) {
            *anim_id = v1;
        }

        tig_net_send_app_all(&pkt, sizeof(pkt));
        return true;
    } else {
        tig_net_send_app_all(&pkt, sizeof(pkt));
        if (anim_id != NULL) {
            sub_421DE0(anim_id);
        }
        return true;
    }
}

// 0x44D730
bool sub_44D730(AnimGoalData *anim_data, AnimID *anim_id, bool a3, bool a4)
{
    // TODO: Incomplete.
}

// 0x44D9B0
void anim_subgoal_add_func()
{
    // TODO: Incomplete.
}

// 0x44DBE0
void sub_44DBE0(AnimID anim_id, AnimGoalData *goal_data)
{
    // TODO: Incomplete.
}

// 0x44DD80
bool sub_44DD80(AnimRunInfo* run_info, AnimGoalSubNode* goal_subnode)
{
    // TODO: Incomplete.
}

// 0x44E050
void sub_44E050(int64_t a1, int64_t a2)
{
    AnimID anim_id;
    AnimRunInfo* run_info;

    if (sub_423300(a1, &anim_id)) {
        run_info = &(anim_run_info[anim_id.slot_num]);
        if (run_info->goals[0].type == AG_ATTACK
            || run_info->goals[1].type == AG_ATTEMPT_ATTACK) {
            if (run_info->goals[0].params[AGDATA_TARGET_OBJ].obj == a2
                || sub_45DDA0(run_info->goals[0].params[AGDATA_TARGET_OBJ].obj) == a2) {
                sub_44E2C0(&anim_id, 5);
            }
        }
    }
}

// 0x44E0E0
void sub_44E0E0(int64_t a1, int64_t a2)
{
    AnimID anim_id;
    AnimRunInfo* run_info;

    if (sub_423300(a1, &anim_id)) {
        run_info = &(anim_run_info[anim_id.slot_num]);
        if (run_info->goals[0].type == AG_ATTACK
            || run_info->goals[1].type == AG_ATTEMPT_ATTACK) {
            if (run_info->goals[0].params[AGDATA_TARGET_OBJ].obj == a2) {
                sub_44E2C0(&anim_id, 5);
            }
        }
    }
}

// 0x44E160
bool sub_44E160(AnimID* anim_id)
{
    // TODO: Incomplete.
}

// 0x44E2A0
bool sub_44E2A0(TimeEvent* timeevent)
{
    return timeevent->params[0].integer_value == dword_5B052C;
}

// 0x44E2C0
bool sub_44E2C0(AnimID* anim_id, int priority)
{
    // TODO: Incomplete.
}

// 0x44E4D0
void sub_44E4D0(int64_t a1, int a2, int a3)
{
    // TODO: Incomplete.
}

// 0x44E5F0
bool anim_find_first_of_type(int64_t obj, int type, AnimID* anim_id)
{
    AnimID tmp_anim_id;

    if (anim_id == NULL) {
        anim_id = &tmp_anim_id;
    }

    sub_421DE0(anim_id);
    return anim_find_next_of_type(obj, type, anim_id);
}

// 0x44E650
bool anim_find_next_of_type(int64_t obj, int type, AnimID* anim_id)
{
    int prev_slot = -1;
    int slot;
    AnimRunInfo* run_info;

    if (obj == OBJ_HANDLE_NULL) {
        return false;
    }

    slot = anim_id != NULL ? anim_id->slot_num + 1 : sub_44D2F0(obj);
    while (slot != -1 && slot != prev_slot) {
        prev_slot = slot;
        run_info = &(anim_run_info[slot]);
        if (run_info->goals[0].type == type && run_info->id.slot_num != -1) {
            if (anim_id != NULL) {
                *anim_id = run_info->id;
            }
            return true;
        }

        slot = sub_44D340(slot, obj);
    }

    return false;
}

// 0x44E6F0
bool sub_44E6F0(int64_t obj, AnimGoalData* goal_data)
{
    return sub_44E710(obj, goal_data, NULL);
}

// 0x44E710
bool sub_44E710(int64_t obj, AnimGoalData* goal_data, AnimID* anim_id)
{
    // TODO: Incomplete.
}

// 0x44E830
bool sub_44E830(int64_t obj, int a2, AnimID* anim_id)
{
    // TODO: Incomplete.
}

// 0x44E8C0
bool sub_44E8C0(int64_t obj, AnimID* anim_id)
{
    int slot;

    if (obj == OBJ_HANDLE_NULL) {
        return false;
    }

    slot = sub_44D2F0(obj);
    while (slot != -1) {
        if (!off_5B03D0[anim_run_info[slot].goals[0].type]->field_C) {
            if (anim_id != NULL) {
                *anim_id = anim_run_info[slot].id;
            }
            return true;
        }
        slot = sub_44D340(slot, obj);
    }

    return false;

}

// 0x44E940
void sub_44E940()
{
    // TODO: Incomplete.
}

// 0x44EAD0
bool sub_44EAD0(int index)
{
    return off_5B03D0[index]->field_8 == 1;
}

// 0x44EB40
void sub_44EB40()
{
    // TODO: Incomplete.
}

// 0x44EBD0
void sub_44EBD0(AnimPath* path)
{
    (void)path;
}

// 0x44EBE0
void sub_44EBE0(AnimPath* path)
{
    (void)path;
}

// 0x44EBF0
void sub_44EBF0(AnimRunInfo* run_info)
{
    ASSERT(run_info != NULL); // pRunInfo != NULL

    if ((run_info->path.flags & 0x1) != 0) {
        run_info->field_C |= 0x10000;
    }
}

// 0x44EEB0
void anim_stat(AnimRunInfo* run_info)
{
    (void)run_info;
}

// 0x44EEC0
void sub_44EEC0(int index)
{
    anim_stat(&(anim_run_info[index]));
}

// 0x44EEE0
void anim_stats()
{
    int index;

    tig_debug_printf("Currently Existing Animations\n");
    tig_debug_printf("------------------------------------------------\n");

    for (index = 0; index < 216; index++) {
        if (anim_run_info[index].field_C != 0) {
            tig_debug_printf("In Slot %d:\n", index);
            anim_stat(&(anim_run_info[index]));
            tig_debug_printf("------------------------------------------------\n");
        }
    }

    tig_debug_printf("Done.\n");
    tig_debug_printf("------------------------------------------------\n");
}
