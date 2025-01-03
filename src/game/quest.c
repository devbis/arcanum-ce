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

typedef struct PcQuestStateInfo {
    /* 0000 */ DateTime timestamp;
    /* 0008 */ int state;
    /* 000C */ int field_C;
} PcQuestStateInfo;

static_assert(sizeof(PcQuestStateInfo) == 0x10, "wrong size");

static bool quest_parse(const char* path, int start, int end);
static int quest_compare(const QuestInfo* a, const QuestInfo* b);

// 0x5B6E34
static mes_file_handle_t quest_log_mes_file = MES_FILE_HANDLE_INVALID;

// 0x5B6E38
static mes_file_handle_t quest_log_dumb_mes_file = MES_FILE_HANDLE_INVALID;

// 0x5FF414
static mes_file_handle_t quest_xp_mes_file;

// 0x5FF418
static int* quest_states;

// 0x5FF41C
static Quest* quests;

// 0x4C45C0
bool quest_init(GameInitInfo* init_info)
{
    (void)init_info;

    quest_states = (int*)CALLOC(MAX_QUEST, sizeof(*quest_states));
    quests = (Quest*)CALLOC(MAX_QUEST, sizeof(*quests));

    if (!mes_load("Rules\\xp_quest.mes", &quest_xp_mes_file)) {
        FREE(quest_states);
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
        quest_states[index] = QUEST_STATE_ACCEPTED;
    }
}

// 0x4C4640
void quest_exit()
{
    mes_unload(quest_xp_mes_file);
    FREE(quest_states);
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
    if (tig_file_fread(quest_states, sizeof(*quest_states) * MAX_QUEST, 1, load_info->stream) != 1) {
        return false;
    }

    return true;
}

// 0x4C4830
bool quest_save(TigFile* stream)
{
    if (tig_file_fwrite(quest_states, sizeof(*quest_states) * MAX_QUEST, 1, stream) != 1) {
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
int sub_4C4C00(int64_t a1, int64_t a2, int num)
{
    int state;

    if (obj_field_int32_get(a1, OBJ_F_TYPE) != OBJ_TYPE_PC) {
        return 0;
    }

    state = sub_4C4CB0(a1, num);
    if (stat_level(a1, STAT_INTELLIGENCE) <= LOW_INTELLIGENCE) {
        return quests[num].dumb_dialog[state];
    }

    if (sub_4C0CC0(a2, a1) < 20) {
        return quests[num].bad_reaction_dialog[state];
    }

    return quests[num].normal_dialog[state];
}

// 0x4C4CB0
int sub_4C4CB0(int64_t obj, int num)
{
    PcQuestStateInfo pc_quest_state;
    int state;

    if (obj_field_int32_get(obj, OBJ_F_TYPE) != OBJ_TYPE_PC) {
        return quest_get_state(num);
    }

    obj_arrayfield_pc_quest_get(obj, OBJ_F_PC_QUEST_IDX, num, &pc_quest_state);

    state = pc_quest_state.state;
    if ((state & 0x100) != 0) {
        state = QUEST_STATE_BOTCHED;
    }

    return state;
}

// 0x4C4D20
void sub_4C4D20(int64_t obj, int num, int state, int64_t a4)
{
    int old_state;
    int64_t party_member_obj;
    int party_member_index;
    int64_t player_obj;
    int player_index;

    old_state = sub_4C4CB0(obj, num);
    if (old_state == QUEST_STATE_COMPLETED
        || old_state == QUEST_STATE_OTHER_COMPLETED
        || old_state == QUEST_STATE_BOTCHED) {
        return;
    }

    if (old_state >= state) {
        return;
    }

    if (state == QUEST_STATE_COMPLETED) {
        sub_45F110(obj, quest_get_xp(quests[num].experience_level));
    }

    if ((tig_net_flags & TIG_NET_CONNECTED) == 0) {
        sub_4C4E60(obj, num, state, a4);
        return;
    }

    if ((tig_net_flags & TIG_NET_HOST) != 0) {
        if (state == QUEST_STATE_ACCEPTED || state == QUEST_STATE_COMPLETED) {
            party_member_obj = party_find_first(obj, &party_member_index);
            do {
                sub_4C4E60(party_member_obj, num, state, a4);
            } while ((party_member_obj = party_find_next(&party_member_index)) != OBJ_HANDLE_NULL);
        } else if (state == QUEST_STATE_BOTCHED) {
            for (player_index = 0; player_index < 8; player_index++) {
                player_obj = sub_4A2B60(player_index);
                if (player_obj != OBJ_HANDLE_NULL) {
                    sub_4C4E60(player_obj, num, state, a4);
                }
            }
        } else {
            sub_4C4E60(obj, num, state, a4);
            return;
        }
    }

    sub_4C4CB0(obj, num);
}

// 0x4C4E60
int sub_4C4E60(int64_t obj, int num, int state, int64_t a4)
{
    int old_state;
    Packet39 pkt;
    PcQuestStateInfo pc_quest_state;
    int reaction;

    if (obj_field_int32_get(obj, OBJ_F_TYPE) != OBJ_TYPE_PC) {
        return state;
    }

    old_state = sub_4C4CB0(obj, num);
    if (!sub_4A2BA0()) {
        if ((tig_net_flags & TIG_NET_HOST) == 0) {
            return old_state;
        }

        pkt.type = 39;
        sub_4440E0(obj, &(pkt.field_8));
        pkt.quest = num;
        pkt.state = state;
        sub_4440E0(a4, &(pkt.field_40));
        tig_net_send_app_all(&pkt, sizeof(pkt));
    }

    if (quest_states[num - 1000] == QUEST_STATE_ACCEPTED) {
        if (state == QUEST_STATE_COMPLETED || state == QUEST_STATE_OTHER_COMPLETED) {
            quest_states[num - 1000] = QUEST_STATE_COMPLETED;
        } else if (state == QUEST_STATE_BOTCHED) {
            quest_states[num - 1000] = QUEST_STATE_BOTCHED;
        }
    } else {
        if (quest_states[num - 1000] == QUEST_STATE_COMPLETED) {
            state = QUEST_STATE_OTHER_COMPLETED;
        } else {
            state = QUEST_STATE_BOTCHED;
        }
    }

    obj_arrayfield_pc_quest_get(obj, OBJ_F_PC_QUEST_IDX, num, &pc_quest_state);
    if (state == QUEST_STATE_BOTCHED) {
        pc_quest_state.state |= 0x100;
    } else {
        pc_quest_state.state = state;
    }

    pc_quest_state.timestamp = sub_45A7C0();
    obj_arrayfield_pc_quest_set(obj, OBJ_F_PC_QUEST_IDX, num, &pc_quest_state);

    if (state == QUEST_STATE_COMPLETED) {
        stat_set_base(obj,
            STAT_ALIGNMENT,
            stat_get_base(obj, STAT_ALIGNMENT) + quests[num].alignment_adjustment);
        tig_sound_quick_play(3028);
    }

    if (a4 != OBJ_HANDLE_NULL) {
        if (state == QUEST_STATE_ACCEPTED) {
            reaction = sub_4C0CC0(a4, obj);
            if (reaction < 41) {
                sub_4C0DE0(a4, obj, 41 - reaction);
            }
        } else if (state == QUEST_STATE_COMPLETED) {
            sub_4C0DE0(a4, obj, 10);
        }
    }

    if (player_is_pc_obj(obj)) {
        if ((pc_quest_state.state & ~0x100) != QUEST_STATE_UNKNOWN) {
            sub_460790(1, 1);
        }
    }

    return state;
}

// 0x4C5070
int sub_4C5070(int64_t obj, int num)
{
    int state;
    PcQuestStateInfo pc_quest_state;
    Packet40 pkt;

    state = quest_get_state(num);
    if (state != QUEST_STATE_BOTCHED) {
        return state;
    }

    if (obj_field_int32_get(obj, OBJ_F_TYPE) != OBJ_TYPE_PC) {
        return state;
    }

    if (!sub_4A2BA0()) {
        if ((tig_net_flags & TIG_NET_HOST) == 0) {
            obj_arrayfield_pc_quest_get(obj, OBJ_F_PC_QUEST_IDX, num, &pc_quest_state);
            return pc_quest_state.state;
        }

        pkt.type = 40;
        sub_4440E0(obj, &(pkt.field_8));
        pkt.field_38 = num;
        tig_net_send_app_all(&pkt, sizeof(pkt));
    }

    quest_states[num - 1000] = QUEST_STATE_ACCEPTED;

    obj_arrayfield_pc_quest_get(obj, OBJ_F_PC_QUEST_IDX, num, &pc_quest_state);
    pc_quest_state.state &= ~0x100;
    pc_quest_state.timestamp = sub_45A7C0();
    obj_arrayfield_pc_quest_set(obj, OBJ_F_PC_QUEST_IDX, num, &pc_quest_state);

    if (player_is_pc_obj(obj)) {
        if (pc_quest_state.state == QUEST_STATE_COMPLETED) {
            tig_sound_quick_play(3028);
        }

        if (pc_quest_state.state != QUEST_STATE_UNKNOWN) {
            sub_460790(1, 1);
        }
    }

    return pc_quest_state.state;
}

// 0x4C51A0
int quest_get_state(int num)
{
    return quest_states[num - 1000];
}

// 0x4C51C0
int quest_set_state(int num, int state)
{
    int old_state;
    struct {
        int type;
        int field_4;
        int field_8;
    } packet;

    static_assert(sizeof(packet) == 0xC, "wrong size");

    old_state = quest_states[num - 1000];
    if (old_state == QUEST_STATE_COMPLETED || old_state == QUEST_STATE_BOTCHED) {
        return old_state;
    }

    if (!sub_4A2BA0()) {
        if ((tig_net_flags & TIG_NET_HOST) == 0) {
            return state;
        }

        packet.type = 41;
        packet.field_4 = num;
        packet.field_8 = state;
        tig_net_send_app_all(&packet, sizeof(packet));
    }

    if (state == QUEST_STATE_COMPLETED || state == QUEST_STATE_BOTCHED) {
        quest_states[num - 1000] = state;
        return state;
    } else {
        return QUEST_STATE_ACCEPTED;
    }
}

// 0x4C5250
void quest_copy_description(object_id_t obj, int num, char* buffer)
{
    if (quests[num - 1000].dumb_description != NULL
        && stat_level(obj, STAT_INTELLIGENCE) <= LOW_INTELLIGENCE) {
        strcpy(buffer, quests[num - 1000].dumb_description);
    } else if (quests[num - 1000].normal_description != NULL) {
        strcpy(buffer, quests[num - 1000].normal_description);
    }
}

// FIXME: Rename `quests1`.
//
// 0x4C52E0
int quest_copy_state(int64_t obj, QuestInfo* quests1)
{
    int index;
    PcQuestStateInfo pc_quest_state[2000];
    int cnt;

    if (obj_field_int32_get(obj, OBJ_F_TYPE) != OBJ_TYPE_PC) {
        return 0;
    }

    sub_407C30(obj, OBJ_F_PC_QUEST_IDX, 1999, pc_quest_state);

    cnt = 0;
    for (index = 0; index < 2000; index++) {
        if ((pc_quest_state[index].state & ~0x100) != QUEST_STATE_UNKNOWN) {
            quests1[cnt].num = index;
            quests1[cnt].timestamp = datetime_to_uint64(pc_quest_state[index].timestamp);
            if ((pc_quest_state[index].state & 0x100) != 0) {
                quests1[cnt].state = QUEST_STATE_BOTCHED;
            } else {
                quests1[cnt].state = pc_quest_state[index].state;
            }
        }
    }

    qsort(quests1, cnt, sizeof(*quests1), quest_compare);

    return cnt;
}

// 0x4C53A0
int quest_compare(const QuestInfo* a, const QuestInfo* b)
{
    return datetime_compare(a->timestamp, b->timestamp);
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
bool sub_4C5400(int64_t a1, int64_t a2)
{
    PcQuestStateInfo pc_quests[2000];
    int index;
    int state;
    int other_state;

    if (obj_field_int32_get(a1, OBJ_F_TYPE) != OBJ_TYPE_PC) {
        return false;
    }

    if (obj_field_int32_get(a2, OBJ_F_TYPE) != OBJ_TYPE_PC) {
        return false;
    }

    sub_407C30(a1, OBJ_F_PC_QUEST_IDX, 1999, pc_quests);

    for (index = 0; index < 2000; index++) {
        state = sub_4C4CB0(a2, index);
        if (state != QUEST_STATE_COMPLETED
            && state != QUEST_STATE_OTHER_COMPLETED
            && state != QUEST_STATE_BOTCHED) {
            other_state = pc_quests[index].state & ~0x100;
            if (state < other_state && other_state == QUEST_STATE_ACCEPTED) {
                sub_4C4E60(a2, index, QUEST_STATE_ACCEPTED, OBJ_HANDLE_NULL);
                if ((pc_quests[index].state & 0x100) != 0) {
                    sub_4C4E60(a2, index, QUEST_STATE_BOTCHED, OBJ_HANDLE_NULL);
                }
            }
        }
    }

    return true;
}
