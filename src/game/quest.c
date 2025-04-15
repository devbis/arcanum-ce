#include "game/quest.h"

#include "game/critter.h"
#include "game/mes.h"
#include "game/mp_utils.h"
#include "game/multiplayer.h"
#include "game/object.h"
#include "game/party.h"
#include "game/player.h"
#include "game/reaction.h"
#include "game/stat.h"
#include "game/ui.h"

#define MAX_QUEST 1000

#define QUEST_BOTCHED_MODIFIER 0x100

typedef struct Quest {
    int experience_level;
    int alignment_adjustment;
    int normal_dialog[QUEST_STATE_COUNT];
    int bad_reaction_dialog[QUEST_STATE_COUNT];
    int dumb_dialog[QUEST_STATE_COUNT];
    char* normal_description;
    char* dumb_description;
} Quest;

static_assert(sizeof(Quest) == 0x64, "wrong size");

static bool quest_parse(const char* path, int start, int end);
static int quest_state_set_internal(int64_t pc_obj, int num, int state, int64_t npc_obj);
static int quest_logbook_entry_compare(const void* va, const void* vb);

// 0x5B6E34
static mes_file_handle_t quest_log_mes_file = MES_FILE_HANDLE_INVALID;

// 0x5B6E38
static mes_file_handle_t quest_log_dumb_mes_file = MES_FILE_HANDLE_INVALID;

// 0x5FF414
static mes_file_handle_t quest_xp_mes_file;

// 0x5FF418
static int* quest_gstate;

// 0x5FF41C
static Quest* quests;

// 0x4C45C0
bool quest_init(GameInitInfo* init_info)
{
    (void)init_info;

    quest_gstate = (int*)CALLOC(MAX_QUEST, sizeof(*quest_gstate));
    quests = (Quest*)CALLOC(MAX_QUEST, sizeof(*quests));

    if (!mes_load("Rules\\xp_quest.mes", &quest_xp_mes_file)) {
        FREE(quest_gstate);
        FREE(quests);
        return false;
    }

    return true;
}

// 0x4C4620
void quest_reset()
{
    int index;

    for (index = 0; index < MAX_QUEST; index++) {
        quest_gstate[index] = QUEST_STATE_ACCEPTED;
    }
}

// 0x4C4640
void quest_exit()
{
    mes_unload(quest_xp_mes_file);
    FREE(quest_gstate);
    FREE(quests);
}

// 0x4C4670
bool quest_mod_load()
{
    int index;
    int state;
    MesFileEntry mes_file_entry;
    int num;

    for (index = 0; index < MAX_QUEST; index++) {
        for (state = 0; state < QUEST_STATE_COUNT; state++) {
            quests[index].normal_dialog[state] = -1;
            quests[index].bad_reaction_dialog[state] = -1;
            quests[index].dumb_dialog[state] = -1;
        }

        quests[index].experience_level = 1;
        quests[index].normal_description = NULL;
        quests[index].dumb_description = NULL;
    }

    if (quest_parse("rules\\gamequest.mes", 1000, 1999)) {
        if (mes_load("mes\\gamequestlog.mes", &quest_log_mes_file)) {
            for (num = 1000; num < 2000; num++) {
                mes_file_entry.num = num;
                if (mes_search(quest_log_mes_file, &mes_file_entry)) {
                    quests[num - 1000].normal_description = mes_file_entry.str;
                }
            }
        }

        if (mes_load("mes\\gamequestlogdumb.mes", &quest_log_dumb_mes_file)) {
            for (num = 1000; num < 2000; num++) {
                mes_file_entry.num = num;
                if (mes_search(quest_log_mes_file, &mes_file_entry)) {
                    quests[num - 1000].dumb_description = mes_file_entry.str;
                }
            }
        }

        quest_reset();
    }

    return true;
}

// 0x4C47C0
void quest_mod_unload()
{
    if (quest_log_mes_file != -1) {
        mes_unload(quest_log_mes_file);
        quest_log_mes_file = -1;
    }

    if (quest_log_dumb_mes_file != -1) {
        mes_unload(quest_log_dumb_mes_file);
        quest_log_dumb_mes_file = -1;
    }
}

// 0x4C4800
bool quest_load(GameLoadInfo* load_info)
{
    if (tig_file_fread(quest_gstate, sizeof(*quest_gstate) * MAX_QUEST, 1, load_info->stream) != 1) {
        return false;
    }

    return true;
}

// 0x4C4830
bool quest_save(TigFile* stream)
{
    if (tig_file_fwrite(quest_gstate, sizeof(*quest_gstate) * MAX_QUEST, 1, stream) != 1) {
        return false;
    }

    return true;
}

// 0x4C4860
bool quest_parse(const char* path, int start, int end)
{
    mes_file_handle_t mes_file;
    MesFileEntry mes_file_entry;
    int num;
    char temp[2000];

    if (!mes_load(path, &mes_file)) {
        return false;
    }

    for (num = start; num <= end; num++) {
        mes_file_entry.num = num;

        if (mes_search(mes_file, &mes_file_entry)) {
            strcpy(temp, mes_file_entry.str);

            quests[num - 1000].experience_level = atoi(strtok(temp, " "));
            quests[num - 1000].alignment_adjustment = atoi(strtok(NULL, " "));
            quests[num - 1000].normal_dialog[QUEST_STATE_UNKNOWN] = atoi(strtok(NULL, " "));
            quests[num - 1000].normal_dialog[QUEST_STATE_MENTIONED] = atoi(strtok(NULL, " "));
            quests[num - 1000].normal_dialog[QUEST_STATE_ACCEPTED] = atoi(strtok(NULL, " "));
            quests[num - 1000].normal_dialog[QUEST_STATE_ACHIEVED] = atoi(strtok(NULL, " "));
            quests[num - 1000].normal_dialog[QUEST_STATE_COMPLETED] = atoi(strtok(NULL, " "));
            quests[num - 1000].normal_dialog[QUEST_STATE_OTHER_COMPLETED] = atoi(strtok(NULL, " "));
            quests[num - 1000].normal_dialog[QUEST_STATE_BOTCHED] = atoi(strtok(NULL, " "));
            quests[num - 1000].bad_reaction_dialog[QUEST_STATE_UNKNOWN] = atoi(strtok(NULL, " "));
            quests[num - 1000].bad_reaction_dialog[QUEST_STATE_MENTIONED] = atoi(strtok(NULL, " "));
            quests[num - 1000].bad_reaction_dialog[QUEST_STATE_ACCEPTED] = atoi(strtok(NULL, " "));
            quests[num - 1000].bad_reaction_dialog[QUEST_STATE_ACHIEVED] = atoi(strtok(NULL, " "));
            quests[num - 1000].bad_reaction_dialog[QUEST_STATE_COMPLETED] = atoi(strtok(NULL, " "));
            quests[num - 1000].bad_reaction_dialog[QUEST_STATE_OTHER_COMPLETED] = atoi(strtok(NULL, " "));
            quests[num - 1000].bad_reaction_dialog[QUEST_STATE_BOTCHED] = atoi(strtok(NULL, " "));
            quests[num - 1000].dumb_dialog[QUEST_STATE_UNKNOWN] = atoi(strtok(NULL, " "));
            quests[num - 1000].dumb_dialog[QUEST_STATE_MENTIONED] = atoi(strtok(NULL, " "));
            quests[num - 1000].dumb_dialog[QUEST_STATE_ACCEPTED] = atoi(strtok(NULL, " "));
            quests[num - 1000].dumb_dialog[QUEST_STATE_ACHIEVED] = atoi(strtok(NULL, " "));
            quests[num - 1000].dumb_dialog[QUEST_STATE_COMPLETED] = atoi(strtok(NULL, " "));
            quests[num - 1000].dumb_dialog[QUEST_STATE_OTHER_COMPLETED] = atoi(strtok(NULL, " "));
            quests[num - 1000].dumb_dialog[QUEST_STATE_BOTCHED] = atoi(strtok(NULL, " "));
        }
    }

    mes_unload(mes_file);

    return true;
}

// 0x4C4C00
int quest_dialog_line(int64_t pc_obj, int64_t npc_obj, int num)
{
    int state;

    if (obj_field_int32_get(pc_obj, OBJ_F_TYPE) != OBJ_TYPE_PC) {
        return 0;
    }

    state = quest_state_get(pc_obj, num);
    if (stat_level_get(pc_obj, STAT_INTELLIGENCE) <= LOW_INTELLIGENCE) {
        return quests[num].dumb_dialog[state];
    }

    if (reaction_get(npc_obj, pc_obj) < 20) {
        return quests[num].bad_reaction_dialog[state];
    }

    return quests[num].normal_dialog[state];
}

// 0x4C4CB0
int quest_state_get(int64_t pc_obj, int num)
{
    PcQuestState pc_quest_state;
    int state;

    if (obj_field_int32_get(pc_obj, OBJ_F_TYPE) != OBJ_TYPE_PC) {
        return quest_global_state_get(num);
    }

    obj_arrayfield_pc_quest_get(pc_obj, OBJ_F_PC_QUEST_IDX, num, &pc_quest_state);

    state = pc_quest_state.state;
    if ((state & QUEST_BOTCHED_MODIFIER) != 0) {
        state = QUEST_STATE_BOTCHED;
    }

    return state;
}

// 0x4C4D20
void quest_state_set(int64_t pc_obj, int num, int state, int64_t npc_obj)
{
    int old_state;
    int64_t party_member_obj;
    int party_member_index;
    int64_t player_obj;
    int player_index;

    old_state = quest_state_get(pc_obj, num);
    if (old_state == QUEST_STATE_COMPLETED
        || old_state == QUEST_STATE_OTHER_COMPLETED
        || old_state == QUEST_STATE_BOTCHED) {
        return;
    }

    if (old_state >= state) {
        return;
    }

    if (state == QUEST_STATE_COMPLETED) {
        critter_give_xp(pc_obj, quest_get_xp(quests[num].experience_level));
    }

    if (!tig_net_is_active()) {
        quest_state_set_internal(pc_obj, num, state, npc_obj);
        return;
    }

    if (tig_net_is_host()) {
        if (state == QUEST_STATE_ACCEPTED || state == QUEST_STATE_COMPLETED) {
            party_member_obj = party_find_first(pc_obj, &party_member_index);
            do {
                quest_state_set_internal(party_member_obj, num, state, npc_obj);
            } while ((party_member_obj = party_find_next(&party_member_index)) != OBJ_HANDLE_NULL);
        } else if (state == QUEST_STATE_BOTCHED) {
            for (player_index = 0; player_index < 8; player_index++) {
                player_obj = sub_4A2B60(player_index);
                if (player_obj != OBJ_HANDLE_NULL) {
                    quest_state_set_internal(player_obj, num, state, npc_obj);
                }
            }
        } else {
            quest_state_set_internal(pc_obj, num, state, npc_obj);
            return;
        }
    }

    quest_state_get(pc_obj, num);
}

// 0x4C4E60
int quest_state_set_internal(int64_t pc_obj, int num, int state, int64_t npc_obj)
{
    int old_state;
    PcQuestState pc_quest_state;
    int reaction;

    if (obj_field_int32_get(pc_obj, OBJ_F_TYPE) != OBJ_TYPE_PC) {
        return state;
    }

    old_state = quest_state_get(pc_obj, num);
    if (!multiplayer_is_locked()) {
        PacketQuestStateSet pkt;

        if (!tig_net_is_host()) {
            return old_state;
        }

        pkt.type = 39;
        sub_4440E0(pc_obj, &(pkt.field_8));
        pkt.quest = num;
        pkt.state = state;
        sub_4440E0(npc_obj, &(pkt.field_40));
        tig_net_send_app_all(&pkt, sizeof(pkt));
    }

    if (quest_gstate[num - 1000] == QUEST_STATE_ACCEPTED) {
        if (state == QUEST_STATE_COMPLETED || state == QUEST_STATE_OTHER_COMPLETED) {
            quest_gstate[num - 1000] = QUEST_STATE_COMPLETED;
        } else if (state == QUEST_STATE_BOTCHED) {
            quest_gstate[num - 1000] = QUEST_STATE_BOTCHED;
        }
    } else {
        if (quest_gstate[num - 1000] == QUEST_STATE_COMPLETED) {
            state = QUEST_STATE_OTHER_COMPLETED;
        } else {
            state = QUEST_STATE_BOTCHED;
        }
    }

    obj_arrayfield_pc_quest_get(pc_obj, OBJ_F_PC_QUEST_IDX, num, &pc_quest_state);
    if (state == QUEST_STATE_BOTCHED) {
        pc_quest_state.state |= QUEST_BOTCHED_MODIFIER;
    } else {
        pc_quest_state.state = state;
    }

    pc_quest_state.datetime = sub_45A7C0();
    obj_arrayfield_pc_quest_set(pc_obj, OBJ_F_PC_QUEST_IDX, num, &pc_quest_state);

    if (state == QUEST_STATE_COMPLETED) {
        stat_base_set(pc_obj,
            STAT_ALIGNMENT,
            stat_base_get(pc_obj, STAT_ALIGNMENT) + quests[num].alignment_adjustment);
        tig_sound_quick_play(3028);
    }

    if (npc_obj != OBJ_HANDLE_NULL) {
        if (state == QUEST_STATE_ACCEPTED) {
            reaction = reaction_get(npc_obj, pc_obj);
            if (reaction < 41) {
                reaction_adj(npc_obj, pc_obj, 41 - reaction);
            }
        } else if (state == QUEST_STATE_COMPLETED) {
            reaction_adj(npc_obj, pc_obj, 10);
        }
    }

    if (player_is_local_pc_obj(pc_obj)) {
        if ((pc_quest_state.state & ~QUEST_BOTCHED_MODIFIER) != QUEST_STATE_UNKNOWN) {
            ui_toggle_primary_button(UI_PRIMARY_BUTTON_LOGBOOK, true);
        }
    }

    return state;
}

// 0x4C5070
int quest_unbotch(int64_t obj, int num)
{
    int state;
    PcQuestState pc_quest_state;

    state = quest_global_state_get(num);
    if (state != QUEST_STATE_BOTCHED) {
        return state;
    }

    if (obj_field_int32_get(obj, OBJ_F_TYPE) != OBJ_TYPE_PC) {
        return state;
    }

    if (!multiplayer_is_locked()) {
        PacketQuestUnbotch pkt;

        if (!tig_net_is_host()) {
            obj_arrayfield_pc_quest_get(obj, OBJ_F_PC_QUEST_IDX, num, &pc_quest_state);
            return pc_quest_state.state;
        }

        pkt.type = 40;
        sub_4440E0(obj, &(pkt.field_8));
        pkt.quest = num;
        tig_net_send_app_all(&pkt, sizeof(pkt));
    }

    quest_gstate[num - 1000] = QUEST_STATE_ACCEPTED;

    obj_arrayfield_pc_quest_get(obj, OBJ_F_PC_QUEST_IDX, num, &pc_quest_state);
    pc_quest_state.state &= ~QUEST_BOTCHED_MODIFIER;
    pc_quest_state.datetime = sub_45A7C0();
    obj_arrayfield_pc_quest_set(obj, OBJ_F_PC_QUEST_IDX, num, &pc_quest_state);

    if (player_is_local_pc_obj(obj)) {
        if (pc_quest_state.state == QUEST_STATE_COMPLETED) {
            tig_sound_quick_play(3028);
        }

        if (pc_quest_state.state != QUEST_STATE_UNKNOWN) {
            ui_toggle_primary_button(UI_PRIMARY_BUTTON_LOGBOOK, true);
        }
    }

    return pc_quest_state.state;
}

// 0x4C51A0
int quest_global_state_get(int num)
{
    return quest_gstate[num - 1000];
}

// 0x4C51C0
int quest_global_state_set(int num, int state)
{
    int old_state;

    old_state = quest_gstate[num - 1000];
    if (old_state == QUEST_STATE_COMPLETED || old_state == QUEST_STATE_BOTCHED) {
        return old_state;
    }

    if (!multiplayer_is_locked()) {
        PacketQuestGlobalStateSet pkt;

        if (!tig_net_is_host()) {
            return state;
        }

        pkt.type = 41;
        pkt.quest = num;
        pkt.state = state;
        tig_net_send_app_all(&pkt, sizeof(pkt));
    }

    if (state == QUEST_STATE_COMPLETED || state == QUEST_STATE_BOTCHED) {
        quest_gstate[num - 1000] = state;
        return state;
    } else {
        return QUEST_STATE_ACCEPTED;
    }
}

// 0x4C5250
void quest_copy_description(int64_t obj, int num, char* buffer)
{
    if (quests[num - 1000].dumb_description != NULL
        && stat_level_get(obj, STAT_INTELLIGENCE) <= LOW_INTELLIGENCE) {
        strcpy(buffer, quests[num - 1000].dumb_description);
    } else if (quests[num - 1000].normal_description != NULL) {
        strcpy(buffer, quests[num - 1000].normal_description);
    }
}

// 0x4C52E0
int quest_get_logbook_data(int64_t obj, QuestLogbookEntry* logbook_entries)
{
    int index;
    PcQuestState pc_quests[2000];
    int cnt;

    if (obj_field_int32_get(obj, OBJ_F_TYPE) != OBJ_TYPE_PC) {
        return 0;
    }

    obj_arrayfield_pc_quest_copy_to_flat(obj, OBJ_F_PC_QUEST_IDX, 1999, pc_quests);

    cnt = 0;
    for (index = 0; index < 2000; index++) {
        if ((pc_quests[index].state & ~QUEST_BOTCHED_MODIFIER) != QUEST_STATE_UNKNOWN) {
            logbook_entries[cnt].num = index;
            logbook_entries[cnt].datetime = pc_quests[index].datetime;
            if ((pc_quests[index].state & QUEST_BOTCHED_MODIFIER) != 0) {
                logbook_entries[cnt].state = QUEST_STATE_BOTCHED;
            } else {
                logbook_entries[cnt].state = pc_quests[index].state;
            }
            cnt++;
        }
    }

    qsort(logbook_entries, cnt, sizeof(*logbook_entries), quest_logbook_entry_compare);

    return cnt;
}

// 0x4C53A0
int quest_logbook_entry_compare(const void* va, const void* vb)
{
    const QuestLogbookEntry* a = (const QuestLogbookEntry*)va;
    const QuestLogbookEntry* b = (const QuestLogbookEntry*)vb;

    return datetime_compare(&(a->datetime), &(b->datetime));
}

// 0x4C53C0
int quest_get_xp(int xp_level)
{
    MesFileEntry mes_file_entry;

    mes_file_entry.num = xp_level;
    if (mes_search(quest_xp_mes_file, &mes_file_entry)) {
        return atoi(mes_file_entry.str);
    } else {
        return 0;
    }
}

// 0x4C5400
bool quest_copy_accepted(int64_t src_obj, int64_t dst_obj)
{
    PcQuestState pc_quests[2000];
    int index;
    int state;
    int other_state;

    if (obj_field_int32_get(src_obj, OBJ_F_TYPE) != OBJ_TYPE_PC) {
        return false;
    }

    if (obj_field_int32_get(dst_obj, OBJ_F_TYPE) != OBJ_TYPE_PC) {
        return false;
    }

    obj_arrayfield_pc_quest_copy_to_flat(src_obj, OBJ_F_PC_QUEST_IDX, 1999, pc_quests);

    for (index = 0; index < 2000; index++) {
        state = quest_state_get(dst_obj, index);
        if (state != QUEST_STATE_COMPLETED
            && state != QUEST_STATE_OTHER_COMPLETED
            && state != QUEST_STATE_BOTCHED) {
            other_state = pc_quests[index].state & ~QUEST_BOTCHED_MODIFIER;
            if (state < other_state && other_state == QUEST_STATE_ACCEPTED) {
                quest_state_set_internal(dst_obj, index, QUEST_STATE_ACCEPTED, OBJ_HANDLE_NULL);
                if ((pc_quests[index].state & QUEST_BOTCHED_MODIFIER) != 0) {
                    quest_state_set_internal(dst_obj, index, QUEST_STATE_BOTCHED, OBJ_HANDLE_NULL);
                }
            }
        }
    }

    return true;
}
