#include "game/lib/quest.h"

#include "game/lib/message.h"

#define THOUSAND 1000
#define SEVEN 7

typedef struct Quest {
    int field_0;
    int field_4;
    int field_8[SEVEN];
    int field_24[SEVEN];
    int field_40[SEVEN];
    char* field_5C;
    char* field_60;
};

static_assert(sizeof(Quest) == 0x64, "wrong size");

static bool sub_4C4860(const char* path, int start, int end);

// 0x5B6E34
static int quest_log_msg_file = -1;

// 0x5B6E38
static int quest_log_dumb_msg_file = -1;

// 0x5FF414
static int quest_msg_file;

// 0x5FF418
static int* off_5FF418;

// 0x5FF41C
static Quest* dword_5FF41C;

// 0x4C45C0
bool quest_init(GameContext* ctx)
{
    off_5FF418 = (int*)calloc(THOUSAND, sizeof(*off_5FF418));
    dword_5FF41C = (Quest*)calloc(THOUSAND, sizeof(*dword_5FF41C));

    if (!message_load("Rules\\xp_quest.mes", &quest_msg_file)) {
        free(off_5FF418);
        free(dword_5FF41C);
        return false;
    }

    return true;
}

// 0x4C4620
void quest_reset()
{
    for (int index = 0; index < THOUSAND; index++) {
        off_5FF418[index] = 2;
    }
}

// 0x4C4640
void quest_exit()
{
    message_unload(quest_msg_file);
    free(off_5FF418);
    free(dword_5FF41C);
}

// 0x4C4670
bool quest_mod_load()
{
    for (int index = 0; index < THOUSAND; index++) {
        for (int k = 0; k < SEVEN; k++) {
            dword_5FF41C[index].field_8[k] = -1;
            dword_5FF41C[index].field_24[k] = -1;
            dword_5FF41C[index].field_40[k] = -1;
        }

        dword_5FF41C[index].field_0 = 1;
        dword_5FF41C[index].field_5C = NULL;
        dword_5FF41C[index].field_60 = NULL;
    }

    if (sub_4C4860("rules\\gamequest.mes", 1000, 1999)) {
        MessageListItem message_list_item;

        if (message_load("mes\\gamequestlog.mes", &quest_log_msg_file)) {
            for (int id = 1000; id < 2000; id++) {
                message_list_item.num = id;
                if (message_find(quest_log_msg_file, &message_list_item)) {
                    dword_5FF41C[id - 1000].field_5C = message_list_item.text;
                }
            }
        }

        if (message_load("mes\\gamequestlogdumb.mes", &quest_log_dumb_msg_file)) {
            for (int id = 1000; id < 2000; id++) {
                message_list_item.num = id;
                if (message_find(quest_log_msg_file, &message_list_item)) {
                    dword_5FF41C[id - 1000].field_60 = message_list_item.text;
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
    if (tig_file_fread(off_5FF418, sizeof(*off_5FF418) * THOUSAND, 1, ctx->stream) != 1) return false;

    return true;
}

// 0x4C4830
bool quest_save(TigFile* stream)
{
    if (tig_file_fwrite(off_5FF418, sizeof(*off_5FF418) * THOUSAND, 1, stream) != 1) return false;

    return true;
}

// 0x4C4860
bool sub_4C4860(const char* path, int start, int end)
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

            dword_5FF41C[id - 1000].field_0 = atoi(strtok(temp, " "));
            dword_5FF41C[id - 1000].field_4 = atoi(strtok(NULL, " "));
            dword_5FF41C[id - 1000].field_8[0] = atoi(strtok(NULL, " "));
            dword_5FF41C[id - 1000].field_8[1] = atoi(strtok(NULL, " "));
            dword_5FF41C[id - 1000].field_8[2] = atoi(strtok(NULL, " "));
            dword_5FF41C[id - 1000].field_8[3] = atoi(strtok(NULL, " "));
            dword_5FF41C[id - 1000].field_8[4] = atoi(strtok(NULL, " "));
            dword_5FF41C[id - 1000].field_8[5] = atoi(strtok(NULL, " "));
            dword_5FF41C[id - 1000].field_8[6] = atoi(strtok(NULL, " "));
            dword_5FF41C[id - 1000].field_24[0] = atoi(strtok(NULL, " "));
            dword_5FF41C[id - 1000].field_24[1] = atoi(strtok(NULL, " "));
            dword_5FF41C[id - 1000].field_24[2] = atoi(strtok(NULL, " "));
            dword_5FF41C[id - 1000].field_24[3] = atoi(strtok(NULL, " "));
            dword_5FF41C[id - 1000].field_24[4] = atoi(strtok(NULL, " "));
            dword_5FF41C[id - 1000].field_24[5] = atoi(strtok(NULL, " "));
            dword_5FF41C[id - 1000].field_24[6] = atoi(strtok(NULL, " "));
            dword_5FF41C[id - 1000].field_40[0] = atoi(strtok(NULL, " "));
            dword_5FF41C[id - 1000].field_40[1] = atoi(strtok(NULL, " "));
            dword_5FF41C[id - 1000].field_40[2] = atoi(strtok(NULL, " "));
            dword_5FF41C[id - 1000].field_40[3] = atoi(strtok(NULL, " "));
            dword_5FF41C[id - 1000].field_40[4] = atoi(strtok(NULL, " "));
            dword_5FF41C[id - 1000].field_40[5] = atoi(strtok(NULL, " "));
            dword_5FF41C[id - 1000].field_40[6] = atoi(strtok(NULL, " "));
        }
    }

    message_unload(msg_file);

    return true;
}

// 0x4C51A0
int sub_4C51A0(int a1)
{
    return off_5FF418[a1 - 1000];
}

// 0x4C53C0
int sub_4C53C0(int id)
{
    MessageListItem message_list_item;
    message_list_item.num = id;
    if (message_find(quest_msg_file, &message_list_item)) {
        return atoi(message_list_item.text);
    } else {
        return 0;
    }
}
