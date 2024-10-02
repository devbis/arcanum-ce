#include "game/rumor.h"

#include <stdio.h>

#include "game/critter.h"
#include "game/mes.h"
#include "game/mp_utils.h"
#include "game/multiplayer.h"
#include "game/stat.h"

#define TWO_FIVE_ONE 251

#define FIVE 5

typedef enum RumorInteractionType {
    RUMOR_INTERACTION_TYPE_MALE_TO_MALE,
    RUMOR_INTERACTION_TYPE_MALE_TO_FEMALE,
    RUMOR_INTERACTION_TYPE_FEMALE_TO_FEMALE,
    RUMOR_INTERACTION_TYPE_FEMALE_TO_MALE,
    RUMOR_INTERACTION_TYPE_DUMB,
    RUMOR_INTERACTION_TYPE_COUNT,
} RumorInteractionType;

static int rumor_compare(const RumorInfo* a, const RumorInfo* b);

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
bool rumor_init(GameContext* init_info)
{
    (void)init_info;

    dword_6876C8 = (uint8_t*)CALLOC(TWO_FIVE_ONE, sizeof(*dword_6876C8));
    dword_5FF420 = (int*)CALLOC(FIVE, sizeof(*dword_5FF420));

    return true;
}

// 0x4C5530
void rumor_reset()
{
    int index;

    for (index = 0; index < TWO_FIVE_ONE; index++) {
        dword_6876C8[index] = 0;
    }
}

// 0x4C5550
void rumor_exit()
{
    FREE(dword_6876C8);
    FREE(dword_5FF420);
}

// 0x4C5570
bool rumor_mod_load()
{
    int index;
    char path[MAX_PATH];

    for (index = 0; index < FIVE; index++) {
        sprintf(path, "mes\\%s.mes", off_5B6E98[index]);
        mes_load(path, &(dword_5FF420[index]));
    }

    rumor_reset();

    return true;
}

// 0x4C55C0
void rumor_mod_unload()
{
    int index;

    for (index = 0; index < FIVE; index++) {
        mes_unload(dword_5FF420[index]);
        dword_5FF420[index] = -1;
    }
}

// 0x4C55F0
bool rumor_load(LoadContext* load_info)
{
    if (tig_file_fread(dword_6876C8, TWO_FIVE_ONE, 1, load_info->stream) != 1) return false;

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
    return ((dword_6876C8[v1] >> v2) & 1) != 0;
}

// 0x4C5690
void rumor_set_known(int rumor)
{
    int v1;
    int v2;
    Packet37 pkt;

    if (!sub_4A2BA0()) {
        if ((tig_net_flags & TIG_NET_HOST) == 0) {
            return;
        }

        pkt.type = 37;
        pkt.rumor = rumor;
        tig_net_send_app_all(&pkt, sizeof(pkt));
    }

    v1 = (rumor - 1000) / 8;
    v2 = (rumor - 1000) % 8;
    dword_6876C8[v1] |= 1 << v2;
}

// 0x4C5700
void sub_4C5700(object_id_t pc_object_id, object_id_t npc_object_id, int rumor, char* buffer)
{
    int interaction_type;
    MesFileEntry mes_file_entry;

    if (stat_level(npc_object_id, STAT_GENDER) == GENDER_MALE) {
        if (stat_level(pc_object_id, STAT_GENDER) == GENDER_MALE) {
            interaction_type = RUMOR_INTERACTION_TYPE_MALE_TO_MALE;
        } else {
            interaction_type = RUMOR_INTERACTION_TYPE_MALE_TO_FEMALE;
        }
    } else {
        if (stat_level(pc_object_id, STAT_GENDER) == GENDER_MALE) {
            interaction_type = RUMOR_INTERACTION_TYPE_FEMALE_TO_MALE;
        } else {
            interaction_type = RUMOR_INTERACTION_TYPE_FEMALE_TO_FEMALE;
        }
    }

    mes_file_entry.num = critter_social_class_get(npc_object_id) + 20 * rumor + 1;
    if (mes_search(dword_5FF420[interaction_type], &mes_file_entry)) {
        strcpy(buffer, mes_file_entry.str);
    } else {
        mes_file_entry.num = 20 * rumor + 1;
        if (mes_search(dword_5FF420[interaction_type], &mes_file_entry)) {
            strcpy(buffer, mes_file_entry.str);
        }
    }
}

// 0x4C58A0
void sub_4C58A0(object_id_t obj, int rumor, int64_t timestamp)
{
    obj_arrayfield_int64_set(obj, OBJ_F_PC_RUMOR_IDX, rumor - 1000, timestamp);
}

// 0x4C58D0
bool sub_4C58D0(object_id_t object_id, int rumor)
{
    if (obj_field_int32_get(object_id, OBJ_F_TYPE) != OBJ_TYPE_PC) {
        return false;
    }

    if (obj_arrayfield_int64_get(object_id, OBJ_F_PC_RUMOR_IDX, rumor - 1000) == 0) {
        return false;
    }

    return true;
}

// 0x4C5920
void sub_4C5920(object_id_t object_id, int rumor, char* buffer)
{
    if (stat_level(object_id, STAT_INTELLIGENCE) > 4) {
        sub_4C5960(rumor, buffer);
    } else {
        sub_4C59D0(rumor, buffer);
    }
}

// 0x4C5960
void sub_4C5960(int rumor, char* buffer)
{
    MesFileEntry mes_file_entry;

    mes_file_entry.num = 20 * rumor;
    if (mes_search(dword_5FF420[RUMOR_INTERACTION_TYPE_MALE_TO_MALE], &mes_file_entry)) {
        strcpy(buffer, mes_file_entry.str);
    } else {
        buffer[0] = '\0';
    }
}

// 0x4C59D0
void sub_4C59D0(int rumor, char* buffer)
{
    MesFileEntry mes_file_entry;

    mes_file_entry.num = 20 * rumor;
    if (mes_search(dword_5FF420[RUMOR_INTERACTION_TYPE_DUMB], &mes_file_entry)) {
        strcpy(buffer, mes_file_entry.str);
    } else {
        buffer[0] = '\0';
    }
}

// 0x4C5A40
int rumor_copy_state(int64_t obj, RumorInfo* rumors)
{
    int index;
    int64_t timestamps[2000];
    int cnt;

    sub_407BA0(obj, OBJ_F_PC_RUMOR_IDX, 1999, timestamps);

    cnt = 0;
    for (index = 0; index < 2000; index++) {
        if (timestamps[index] != 0) {
            rumors[cnt].num = 1000 + index;
            rumors[cnt].timestamp = timestamps[index];
            rumors[cnt].known = rumor_is_known(1000 + index);
            cnt++;
        }
    }

    qsort(rumors, cnt, sizeof(*rumors), rumor_compare);

    return cnt;
}

// 0x4C5AF0
int rumor_compare(const RumorInfo* a, const RumorInfo* b)
{
    return datetime_compare(&(a->timestamp), &(b->timestamp));
}

// 0x4C5B10
bool sub_4C5B10(int64_t a1, int64_t a2)
{
    int index;
    int64_t timestamps[2000];

    if (obj_field_int32_get(a1, OBJ_F_TYPE) != OBJ_TYPE_PC
        || obj_field_int32_get(a2, OBJ_F_TYPE) != OBJ_TYPE_PC) {
        return false;
    }

    sub_407BA0(a1, OBJ_F_PC_RUMOR_IDX, 1999, timestamps);

    for (index = 0; index < 2000; index++) {
        if (timestamps[index] != 0) {
            sub_4C57E0(a2, index);
        }
    }

    return true;
}
