#include "game/lib/rumor.h"

#include <stdio.h>

#include "game/lib/critter.h"
#include "game/lib/message.h"
#include "game/lib/stat.h"
#include "tig/net.h"

#define TWO_FIVE_ONE 251

#define FIVE 5

typedef enum RumorInteractionType {
    RUMOR_INTERACTION_TYPE_MALE_TO_MALE,
    RUMOR_INTERACTION_TYPE_MALE_TO_FEMALE,
    RUMOR_INTERACTION_TYPE_FEMALE_TO_FEMALE,
    RUMOR_INTERACTION_TYPE_FEMALE_TO_MALE,
    RUMOR_INTERACTION_TYPE_DUMB,
    RUMOR_INTERACTION_TYPE_COUNT,
};

// 0x5B6E98
static const char* off_5B6E98[FIVE] = {
    "game_rd_npc_m2m",
    "game_rd_npc_m2f",
    "game_rd_npc_f2f",
    "game_rd_npc_f2m",
    "game_rd_npc_m2m_dumb",
};

// 0x5FF420
static int* dword_5FF420;

// 0x6876C8
static uint8_t* dword_6876C8;

// 0x4C5500
bool rumor_init(GameContext* ctx)
{
    dword_6876C8 = (uint8_t*)calloc(TWO_FIVE_ONE, sizeof(*dword_6876C8));
    dword_5FF420 = (int*)calloc(FIVE, sizeof(*dword_5FF420));
    return true;
}

// 0x4C5530
void rumor_reset()
{
    for (int index = 0; index < TWO_FIVE_ONE; index++) {
        dword_6876C8[index] = 0;
    }
}

// 0x4C5550
void rumor_exit()
{
    free(dword_6876C8);
    free(dword_5FF420);
}

// 0x4C5570
bool rumor_mod_load()
{
    for (int index = 0; index < FIVE; index++) {
        char path[MAX_PATH];
        sprintf(path, "mes\\%s.mes", off_5B6E98[index]);
        message_load(path, &(dword_5FF420[index]));
    }

    rumor_reset();

    return true;
}

// 0x4C55C0
void rumor_mod_unload()
{
    for (int index = 0; index < FIVE; index++) {
        message_unload(dword_5FF420[index]);
        dword_5FF420[index] = -1;
    }
}

// 0x4C55F0
bool rumor_load(LoadContext* ctx)
{
    if (tig_file_fread(dword_6876C8, TWO_FIVE_ONE, 1, ctx->stream) != 1) return false;

    return true;
}

// 0x4C5620
bool rumor_save(TigFile* stream)
{
    if (tig_file_fwrite(dword_6876C8, TWO_FIVE_ONE, 1, stream) != 1) return false;

    return true;
}

// 0x4C5650
bool rumor_is_known(int rumor)
{
    int v1 = (rumor - 1000) / 8;
    int v2 = (rumor - 1000) % 8;
    return (dword_6876C8[v1] >> v2) & 1 != 0;
}

// 0x4C5690
void rumor_set_known(int rumor)
{
    if (!sub_4A2BA0()) {
        // TODO: Incomplete.
    }

    int v1 = (rumor - 1000) / 8;
    int v2 = (rumor - 1000) % 8;
    dword_6876C8[v1] |= 1 << v2;
}

// 0x4C5700
void sub_4C5700(object_id_t pc_object_id, object_id_t npc_object_id, int rumor, char* buffer)
{
    int interaction_type;
    if (sub_4B0490(npc_object_id, STAT_GENDER) == GENDER_MALE) {
        if (sub_4B0490(pc_object_id, STAT_GENDER) == GENDER_MALE) {
            interaction_type = RUMOR_INTERACTION_TYPE_MALE_TO_MALE;
        } else {
            interaction_type = RUMOR_INTERACTION_TYPE_MALE_TO_FEMALE;
        }
    } else {
        if (sub_4B0490(pc_object_id, STAT_GENDER) == GENDER_MALE) {
            interaction_type = RUMOR_INTERACTION_TYPE_FEMALE_TO_MALE;
        } else {
            interaction_type = RUMOR_INTERACTION_TYPE_FEMALE_TO_FEMALE;
        }
    }

    MessageListItem msg;
    msg.num = critter_social_class_get(npc_object_id) + 20 * rumor + 1;
    if (message_find(dword_5FF420[interaction_type], &msg)) {
        // FIXME: Why not `strcpy`?
        memcpy(buffer, msg.text, strlen(msg.text) + 1);
    } else {
        msg.num = 20 * rumor + 1;
        if (message_find(dword_5FF420[interaction_type], &msg)) {
            // FIXME: Why not `strcpy`?
            memcpy(buffer, msg.text, strlen(msg.text) + 1);
        }
    }
}

// 0x4C58D0
bool sub_4C58D0(object_id_t object_id, int rumor)
{
    if (object_field_get(object_id, OBJ_F_TYPE) != OBJ_TYPE_15) {
        return false;
    }

    if (sub_407540(object_id, OBJ_F_PC_RUMOR_IDX, rumor - 1000) == 0) {
        return false;
    }

    return true;
}

// 0x4C5920
void sub_4C5920(object_id_t object_id, int rumor, char* buffer)
{
    if (sub_4B0490(object_id, STAT_INTELLIGENCE) > 4) {
        sub_4C5960(rumor, buffer);
    } else {
        sub_4C59D0(rumor, buffer);
    }
}

// 0x4C5960
void sub_4C5960(int rumor, char* buffer)
{
    MessageListItem msg;
    msg.num = 20 * rumor;
    if (message_find(dword_5FF420[RUMOR_INTERACTION_TYPE_MALE_TO_MALE], &msg)) {
        strcpy(buffer, msg.text);
    } else {
        buffer[0] = '\0';
    }
}

// 0x4C59D0
void sub_4C59D0(int rumor, char* buffer)
{
    MessageListItem msg;
    msg.num = 20 * rumor;
    if (message_find(dword_5FF420[RUMOR_INTERACTION_TYPE_DUMB], &msg)) {
        strcpy(buffer, msg.text);
    } else {
        buffer[0] = '\0';
    }
}
