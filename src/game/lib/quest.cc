#include "game/lib/quest.h"

#include "game/lib/message.h"
#include "game/lib/stat.h"

#define MAX_QUEST 1000

typedef struct Quest {
    int experience_level;
    int alignment_adjustment;
    int normal_dialog[QUEST_STATE_COUNT];
    int bad_reaction_dialog[QUEST_STATE_COUNT];
    int dumb_dialog[QUEST_STATE_COUNT];
    char* normal_description;
    char* dumb_description;
};

static_assert(sizeof(Quest) == 0x64, "wrong size");

static bool quest_parse(const char* path, int start, int end);

// 0x5B6E34
static int quest_log_msg_file = -1;

// 0x5B6E38
static int quest_log_dumb_msg_file = -1;

// 0x5FF414
static int quest_xp_msg_file;

// 0x5FF418
static int* quest_states;

// 0x5FF41C
static Quest* quests;

// 0x4C45C0
bool quest_init(GameContext* ctx)
{
    quest_states = (int*)calloc(MAX_QUEST, sizeof(*quest_states));
    quests = (Quest*)calloc(MAX_QUEST, sizeof(*quests));

    if (!message_load("Rules\\xp_quest.mes", &quest_xp_msg_file)) {
        free(quest_states);
        free(quests);
        return false;
    }

    return true;
}

// 0x4C4620
void quest_reset()
{
    for (int index = 0; index < MAX_QUEST; index++) {
        quest_states[index] = QUEST_STATE_ACCEPTED;
    }
}

// 0x4C4640
void quest_exit()
{
    message_unload(quest_xp_msg_file);
    free(quest_states);
    free(quests);
}

// 0x4C4670
bool quest_mod_load()
{
    for (int index = 0; index < MAX_QUEST; index++) {
        for (int k = 0; k < QUEST_STATE_COUNT; k++) {
            quests[index].normal_dialog[k] = -1;
            quests[index].bad_reaction_dialog[k] = -1;
            quests[index].dumb_dialog[k] = -1;
        }

        quests[index].experience_level = 1;
        quests[index].normal_description = NULL;
        quests[index].dumb_description = NULL;
    }

    if (quest_parse("rules\\gamequest.mes", 1000, 1999)) {
        MessageListItem message_list_item;

        if (message_load("mes\\gamequestlog.mes", &quest_log_msg_file)) {
            for (int id = 1000; id < 2000; id++) {
                message_list_item.num = id;
                if (message_find(quest_log_msg_file, &message_list_item)) {
                    quests[id - 1000].normal_description = message_list_item.text;
                }
            }
        }

        if (message_load("mes\\gamequestlogdumb.mes", &quest_log_dumb_msg_file)) {
            for (int id = 1000; id < 2000; id++) {
                message_list_item.num = id;
                if (message_find(quest_log_msg_file, &message_list_item)) {
                    quests[id - 1000].dumb_description = message_list_item.text;
                }
            }
        }
    }

    return true;
}

// 0x4C47C0
void quest_mod_unload()
{
    if (quest_log_msg_file != -1) {
        message_unload(quest_log_msg_file);
        quest_log_msg_file = -1;
    }

    if (quest_log_dumb_msg_file != -1) {
        message_unload(quest_log_dumb_msg_file);
        quest_log_dumb_msg_file = -1;
    }
}

// 0x4C4800
bool quest_load(LoadContext* ctx)
{
    if (tig_file_fread(quest_states, sizeof(*quest_states) * MAX_QUEST, 1, ctx->stream) != 1) return false;

    return true;
}

// 0x4C4830
bool quest_save(TigFile* stream)
{
    if (tig_file_fwrite(quest_states, sizeof(*quest_states) * MAX_QUEST, 1, stream) != 1) return false;

    return true;
}

// 0x4C4860
bool quest_parse(const char* path, int start, int end)
{
    int msg_file;
    if (!message_load(path, &msg_file)) {
        return false;
    }

    for (int id = start; id <= end; id++) {
        MessageListItem message_list_item;
        message_list_item.num = id;

        if (message_find(msg_file, &message_list_item)) {
            char temp[2000];
            strcpy(temp, message_list_item.text);

            quests[id - 1000].experience_level = atoi(strtok(temp, " "));
            quests[id - 1000].alignment_adjustment = atoi(strtok(NULL, " "));
            quests[id - 1000].normal_dialog[0] = atoi(strtok(NULL, " "));
            quests[id - 1000].normal_dialog[1] = atoi(strtok(NULL, " "));
            quests[id - 1000].normal_dialog[2] = atoi(strtok(NULL, " "));
            quests[id - 1000].normal_dialog[3] = atoi(strtok(NULL, " "));
            quests[id - 1000].normal_dialog[4] = atoi(strtok(NULL, " "));
            quests[id - 1000].normal_dialog[5] = atoi(strtok(NULL, " "));
            quests[id - 1000].normal_dialog[6] = atoi(strtok(NULL, " "));
            quests[id - 1000].bad_reaction_dialog[0] = atoi(strtok(NULL, " "));
            quests[id - 1000].bad_reaction_dialog[1] = atoi(strtok(NULL, " "));
            quests[id - 1000].bad_reaction_dialog[2] = atoi(strtok(NULL, " "));
            quests[id - 1000].bad_reaction_dialog[3] = atoi(strtok(NULL, " "));
            quests[id - 1000].bad_reaction_dialog[4] = atoi(strtok(NULL, " "));
            quests[id - 1000].bad_reaction_dialog[5] = atoi(strtok(NULL, " "));
            quests[id - 1000].bad_reaction_dialog[6] = atoi(strtok(NULL, " "));
            quests[id - 1000].dumb_dialog[0] = atoi(strtok(NULL, " "));
            quests[id - 1000].dumb_dialog[1] = atoi(strtok(NULL, " "));
            quests[id - 1000].dumb_dialog[2] = atoi(strtok(NULL, " "));
            quests[id - 1000].dumb_dialog[3] = atoi(strtok(NULL, " "));
            quests[id - 1000].dumb_dialog[4] = atoi(strtok(NULL, " "));
            quests[id - 1000].dumb_dialog[5] = atoi(strtok(NULL, " "));
            quests[id - 1000].dumb_dialog[6] = atoi(strtok(NULL, " "));
        }
    }

    message_unload(msg_file);

    return true;
}

// 0x4C51A0
int quest_get_state(int id)
{
    return quest_states[id - 1000];
}

// 0x4C51C0
int quest_set_state(int id, int state)
{
    int old_state = quest_states[id - 1000];
    if (old_state == QUEST_STATE_COMPLETED || old_state == QUEST_STATE_BOTCHED) {
        return old_state;
    }

    if (!sub_4A2BA0()) {
        // TODO: Incomplete.
    }

    if (state == QUEST_STATE_COMPLETED || state == QUEST_STATE_BOTCHED) {
        quest_states[id - 1000] = state;
        return state;
    } else {
        return QUEST_STATE_ACCEPTED;
    }
}

// 0x4C5250
void quest_copy_description(object_id_t object_id, int quest_id, char* buffer)
{
    if (quests[quest_id - 1000].dumb_description != NULL && sub_4B0490(object_id, STAT_INTELLIGENCE) <= LOW_INTELLIGENCE) {
        strcpy(buffer, quests[quest_id - 1000].dumb_description);
    } else if (quests[quest_id - 1000].normal_description != NULL) {
        strcpy(buffer, quests[quest_id - 1000].normal_description);
    }
}

// 0x4C53C0
int quest_get_xp(int xp_level)
{
    MessageListItem message_list_item;
    message_list_item.num = xp_level;
    if (message_find(quest_xp_msg_file, &message_list_item)) {
        return atoi(message_list_item.text);
    } else {
        return 0;
    }
}
