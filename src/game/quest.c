#include "game/quest.h"

#include "game/mes.h"
#include "game/multiplayer.h"
#include "game/stat.h"

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

static bool quest_parse(const char* path, int start, int end);

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
bool quest_init(GameContext* init_info)
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
bool quest_load(LoadContext* ctx)
{
    if (tig_file_fread(quest_states, sizeof(*quest_states) * MAX_QUEST, 1, ctx->stream) != 1) {
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
