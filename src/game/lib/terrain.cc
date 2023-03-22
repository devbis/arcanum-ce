#include "game/lib/terrain.h"

#include "game/lib/message.h"
#include "game/lib/random.h"
#include "tig/file.h"

#define FIRST_TERRAIN_COLOR_ID 100
#define FIRST_TERRAIN_TRANSITION_ID 200

#define TERRAIN_TYPE_MAX 32

// 0x603748
static int dword_603748[TERRAIN_TYPE_MAX];

// 0x6038E0
static bool terrain_is_editor;

// 0x6039E8
static int* dword_6039E8;

// 0x603A14
static int dword_603A14;

// 0x603A1C
static int terrain_msg_file;

// 0x603A20
static bool dword_603A20[TERRAIN_TYPE_MAX];

// 0x4E7B00
bool terrain_init(GameContext* ctx)
{
    if (!message_load("terrain\\terrain.mes", &terrain_msg_file)) {
        return false;
    }

    if (!terrain_init_types()) {
        return false;
    }

    if (!sub_4E8F40()) {
        return false;
    }

    if (!terrain_init_transitions()) {
        return false;
    }

    terrain_is_editor = ctx->editor;

    return true;
}

// 0x4E7B50
void terrain_reset()
{
}

// 0x4E7B60
void terrain_exit()
{
    if (dword_6039E8 != NULL) {
        free(dword_6039E8);
        dword_6039E8 = NULL;
    }

    message_unload(terrain_msg_file);
}

// 0x4E8B20
const char* terrain_base_name(int terrain_type)
{
    if (terrain_type >= 0 && terrain_type < dword_603A14) {
        MessageListItem msg;
        msg.num = terrain_type;

        if (message_find(terrain_msg_file, &msg)) {
            return msg.text;
        }
    }

    return NULL;
}

// 0x4E8B60
int terrain_match_base_name(const char* base_name)
{
    MessageListItem msg;
    msg.num = 0;
    if (message_find(terrain_msg_file, &msg)) {
        do {
            if (_strcmpi(base_name, msg.text) == 0) {
                return msg.num;
            }
        } while (sub_4D4460(terrain_msg_file, &msg));
    }

    return -1;
}

// 0x4E8BE0
void terrain_color(int terrain_type, int* red, int* green, int* blue)
{
    MessageListItem msg;
    char buffer[MESSAGE_LIST_ITEM_TEXT_LENGTH];

    *red = 255;
    *green = 0;
    *blue = 0;

    if (terrain_type >= 0 && terrain_type < dword_603A14) {
        msg.num = terrain_type + FIRST_TERRAIN_COLOR_ID;
        if (message_find(terrain_msg_file, &msg)) {
            strcpy(buffer, msg.text);

            char* tok = strtok(buffer, ",");
            if (tok != NULL) {
                *red = atoi(tok);

                tok = strtok(NULL, ",");
                if (tok != NULL) {
                    *green = atoi(tok);

                    tok = strtok(NULL, ",");
                    if (tok != NULL) {
                        *blue = atoi(tok);
                    }
                }
            }
        }
    }
}

// 0x4E8CE0
void sub_4E8CE0(int a1, int a2, int a3, int a4, uint16_t* value)
{
    a1 = __min(__max(a1, 0), 31);
    a2 = __min(__max(a2, 0), 31);
    a3 = __min(__max(a3, 0), 15);
    a4 = __min(__max(a4, 0), 3);

    // TODO: Check.
    *value = a4 | (a3 << 2) | (a2 << 4) | (a1 << 11);
}

// 0x4E8DC0
int sub_4E8DC0(uint16_t a1)
{
    return (a1 >> 11) & 0x1F;
}

// 0x4E8DD0
int sub_4E8DD0(uint16_t a1)
{
    return (a1 >> 6) & 0x1F;
}

// 0x4E8DE0
int sub_4E8DE0(uint16_t a1)
{
    return (a1 >> 2) & 0xF;
}

// 0x4E8DF0
int sub_4E8DF0(uint16_t a1)
{
    return a1 & 3;
}

// 0x4E8E60
int64_t sub_4E8E60(int a1)
{
    // TODO: Check.
    int64_t v1 = a1;
    return (v1 % 2) | ((v1 / 2) << 26);
}

// 0x4E8EA0
bool terrain_init_types()
{
    MessageListItem msg;

    for (int index = 0; index < TERRAIN_TYPE_MAX; index++) {
        msg.num = index;
        if (!message_find(terrain_msg_file, &msg)) {
            break;
        }

        char* pch = strstr(msg.text, "/b");
        if (pch != NULL) {
            dword_603A20[index] = true;

            while (isspace(*pch)) {
                pch--;
            }

            pch[1] = '\0';
        }
    }

    return true;
}

// 0x4E8F40
bool sub_4E8F40()
{
    MessageListItem msg;
    char path[MAX_PATH];

    int index;
    for (index = 0; index < TERRAIN_TYPE_MAX; index++) {
        msg.num = index;
        if (!message_find(terrain_msg_file, &msg)) {
            break;
        }

        int k;
        for (k = 0; k <= 3; k++) {
            strcpy(path, "terrain\\");
            strcat(path, msg.text);
            strcat(path, "\\");
            _i64toa(sub_4E8E60(k), path + strlen(path), 10);
            strcat(path, ".sec");

            if (!tig_file_info(path, NULL)) {
                break;
            }

            dword_603748[index]++;
        }

        if (k == 0 && index == 0) {
            return false;
        }
    }

    if (index == 0) {
        return false;
    }

    dword_603A14 = index;

    return true;
}

// 0x4E90B0
bool terrain_init_transitions()
{
    MessageListItem msg;

    msg.num = FIRST_TERRAIN_TRANSITION_ID;
    if (!message_find(terrain_msg_file, &msg)) {
        return false;
    }

    int transition_count = dword_603A14 * dword_603A14;
    bool* transitions = (bool*)malloc(sizeof(*transitions) * transition_count);
    for (int index = 0; index < transition_count; index++) {
        transitions[index] = false;
    }

    do {
        int from;
        int to;
        if (!terrain_parse_transition(msg.text, &from, &to)) {
            free(transitions);
            return false;
        }

        transitions[to + from * dword_603A14] = true;
        transitions[from + to * dword_603A14] = true;
    } while (sub_4D4460(terrain_msg_file, &msg));

    dword_6039E8 = (int*)malloc(sizeof(*dword_6039E8) * transition_count);

    for (int from = 0; from < dword_603A14; from++) {
        for (int to = 0; to < dword_603A14; to++) {
            int v1;
            if (!sub_4E92B0(from, to, transitions, &v1)) {
                free(transitions);
                return false;
            }

            dword_6039E8[to + from * dword_603A14] = v1;
        }
    }

    free(transitions);
    return true;
}

// 0x4E9240
bool terrain_parse_transition(char* str, int* from_terrain_type, int* to_terrain_type)
{
    char* pch = strstr(str, " to ");
    if (pch == NULL) {
        return false;
    }

    *pch = '\0';

    int from = terrain_match_base_name(str);
    if (from == -1) {
        // FIXME: `str` not restored to previous state.
        return false;
    }

    *pch = ' ';

    int to = terrain_match_base_name(pch + 4);
    if (to = -1) {
        return false;
    }

    *from_terrain_type = from;
    *to_terrain_type = to;

    return true;
}

// 0x4E92B0
bool sub_4E92B0(int from, int to, bool* transitions, int* a4)
{
    if (from == to) {
        *a4 = from;
        return true;
    }

    int* v1 = (int*)malloc(sizeof(*v1) * dword_603A14);
    v1[0] = from;

    bool result = sub_4E9310(v1, 0, to, transitions);

    *a4 = v1[1];
    free(v1);

    return result;
}

// 0x4E93A0
uint16_t sub_4E93A0(int from, int to)
{
    uint16_t value;

    int v1 = dword_6039E8[to + from * dword_603A14];
    sub_4E8CE0(v1, v1, 15, 0, &value);

    return value;
}

// 0x4E93E0
bool sub_4E93E0(int from, int to)
{
    int terrain_type = sub_4E8DC0(sub_4E93A0(from, to));
    return terrain_type == from || terrain_type == to;
}
