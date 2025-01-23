#include "game/rumor.h"

#include <stdio.h>

#include "game/critter.h"
#include "game/mes.h"
#include "game/mp_utils.h"
#include "game/multiplayer.h"
#include "game/stat.h"

#define RUMOR_QSTATE_SIZE ((MAX_RUMORS / 8) + 1)

#define rumor_idx_to_num(idx) ((idx) + 1000)
#define rumor_num_to_idx(num) ((num) - 1000)

typedef enum RumorInteractionType {
    RUMOR_INTERACTION_TYPE_MALE_TO_MALE,
    RUMOR_INTERACTION_TYPE_MALE_TO_FEMALE,
    RUMOR_INTERACTION_TYPE_FEMALE_TO_FEMALE,
    RUMOR_INTERACTION_TYPE_FEMALE_TO_MALE,
    RUMOR_INTERACTION_TYPE_DUMB,
    RUMOR_INTERACTION_TYPE_COUNT,
} RumorInteractionType;

static void rumor_set_known_internal(int64_t obj, int rumor, int64_t timestamp);
static int rumor_compare(const RumorInfo* a, const RumorInfo* b);

// 0x5B6E98
static const char* rumor_mes_file_names[RUMOR_INTERACTION_TYPE_COUNT] = {
    "game_rd_npc_m2m",
    "game_rd_npc_m2f",
    "game_rd_npc_f2f",
    "game_rd_npc_f2m",
    "game_rd_npc_m2m_dumb",
};

// 0x5FF420
static mes_file_handle_t* rumor_mes_files;

// 0x6876C8
static uint8_t* rumor_qstate;

// 0x4C5500
bool rumor_init(GameInitInfo* init_info)
{
    (void)init_info;

    rumor_qstate = (uint8_t*)CALLOC(RUMOR_QSTATE_SIZE, sizeof(*rumor_qstate));
    rumor_mes_files = (mes_file_handle_t*)CALLOC(RUMOR_INTERACTION_TYPE_COUNT, sizeof(*rumor_mes_files));

    return true;
}

// 0x4C5530
void rumor_reset()
{
    int index;

    for (index = 0; index < RUMOR_QSTATE_SIZE; index++) {
        rumor_qstate[index] = 0;
    }
}

// 0x4C5550
void rumor_exit()
{
    FREE(rumor_qstate);
    FREE(rumor_mes_files);
}

// 0x4C5570
bool rumor_mod_load()
{
    int index;
    char path[TIG_MAX_PATH];

    for (index = 0; index < RUMOR_INTERACTION_TYPE_COUNT; index++) {
        sprintf(path, "mes\\%s.mes", rumor_mes_file_names[index]);
        mes_load(path, &(rumor_mes_files[index]));
    }

    rumor_reset();

    return true;
}

// 0x4C55C0
void rumor_mod_unload()
{
    int index;

    for (index = 0; index < RUMOR_INTERACTION_TYPE_COUNT; index++) {
        mes_unload(rumor_mes_files[index]);
        rumor_mes_files[index] = MES_FILE_HANDLE_INVALID;
    }
}

// 0x4C55F0
bool rumor_load(GameLoadInfo* load_info)
{
    if (tig_file_fread(rumor_qstate, RUMOR_QSTATE_SIZE, 1, load_info->stream) != 1) return false;

    return true;
}

// 0x4C5620
bool rumor_save(TigFile* stream)
{
    if (tig_file_fwrite(rumor_qstate, RUMOR_QSTATE_SIZE, 1, stream) != 1) return false;

    return true;
}

// 0x4C5650
bool rumor_qstate_get(int rumor)
{
    int pos = rumor_num_to_idx(rumor) / 8;
    int bit = rumor_num_to_idx(rumor) % 8;
    return ((rumor_qstate[pos] >> bit) & 1) != 0;
}

// 0x4C5690
void rumor_qstate_set(int rumor)
{
    int pos;
    int bit;
    PacketRumorQStateSet pkt;

    if (!multiplayer_is_locked()) {
        if ((tig_net_flags & TIG_NET_HOST) == 0) {
            return;
        }

        pkt.type = 37;
        pkt.rumor = rumor;
        tig_net_send_app_all(&pkt, sizeof(pkt));
    }

    pos = rumor_num_to_idx(rumor) / 8;
    bit = rumor_num_to_idx(rumor) % 8;
    rumor_qstate[pos] |= 1 << bit;
}

// 0x4C5700
void rumor_copy_interaction_str(int64_t pc_obj, int64_t npc_obj, int rumor, char* buffer)
{
    int interaction_type;
    MesFileEntry mes_file_entry;

    if (stat_level(npc_obj, STAT_GENDER) == GENDER_MALE) {
        if (stat_level(pc_obj, STAT_GENDER) == GENDER_MALE) {
            interaction_type = RUMOR_INTERACTION_TYPE_MALE_TO_MALE;
        } else {
            interaction_type = RUMOR_INTERACTION_TYPE_MALE_TO_FEMALE;
        }
    } else {
        if (stat_level(pc_obj, STAT_GENDER) == GENDER_MALE) {
            interaction_type = RUMOR_INTERACTION_TYPE_FEMALE_TO_MALE;
        } else {
            interaction_type = RUMOR_INTERACTION_TYPE_FEMALE_TO_FEMALE;
        }
    }

    mes_file_entry.num = critter_social_class_get(npc_obj) + 20 * rumor + 1;
    if (!mes_search(rumor_mes_files[interaction_type], &mes_file_entry)) {
        mes_file_entry.num = 20 * rumor + 1;
        if (!mes_search(rumor_mes_files[interaction_type], &mes_file_entry)) {
            buffer[0] = '\0';
            return;
        }
    }

    strcpy(buffer, mes_file_entry.str);
}

// 0x4C57E0
void rumor_known_set(int64_t obj, int rumor)
{
    DateTime datetime;
    PacketRumorKnownSet pkt;

    datetime = sub_45A7C0();
    if (!multiplayer_is_locked()) {
        if ((tig_net_flags & TIG_NET_HOST) == 0) {
            return;
        }

        pkt.type = 38;
        pkt.rumor = rumor;
        pkt.oid = sub_407EF0(obj);
        pkt.datetime = datetime;
        tig_net_send_app_all(&pkt, sizeof(pkt));
    }

    if (obj_field_int32_get(obj, OBJ_F_TYPE) != OBJ_TYPE_PC) {
        return;
    }

    if (rumor_known_get(obj, rumor)) {
        return;
    }

    rumor_set_known_internal(obj, rumor, datetime.value);
}

// 0x4C58A0
void rumor_set_known_internal(int64_t obj, int rumor, int64_t timestamp)
{
    obj_arrayfield_int64_set(obj, OBJ_F_PC_RUMOR_IDX, rumor_num_to_idx(rumor), timestamp);
}

// 0x4C58D0
bool rumor_known_get(int64_t obj, int rumor)
{
    if (obj_field_int32_get(obj, OBJ_F_TYPE) != OBJ_TYPE_PC) {
        return false;
    }

    if (obj_arrayfield_int64_get(obj, OBJ_F_PC_RUMOR_IDX, rumor_num_to_idx(rumor)) == 0) {
        return false;
    }

    return true;
}

// 0x4C5920
void rumor_copy_logbook_str(int64_t obj, int rumor, char* buffer)
{
    if (stat_level(obj, STAT_INTELLIGENCE) > LOW_INTELLIGENCE) {
        rumor_copy_logbook_normal_str(rumor, buffer);
    } else {
        rumor_copy_logbook_dumb_str(rumor, buffer);
    }
}

// 0x4C5960
void rumor_copy_logbook_normal_str(int rumor, char* buffer)
{
    MesFileEntry mes_file_entry;

    mes_file_entry.num = 20 * rumor;
    if (!mes_search(rumor_mes_files[RUMOR_INTERACTION_TYPE_MALE_TO_MALE], &mes_file_entry)) {
        buffer[0] = '\0';
        return;
    }

    strcpy(buffer, mes_file_entry.str);
}

// 0x4C59D0
void rumor_copy_logbook_dumb_str(int rumor, char* buffer)
{
    MesFileEntry mes_file_entry;

    mes_file_entry.num = 20 * rumor;
    if (!mes_search(rumor_mes_files[RUMOR_INTERACTION_TYPE_DUMB], &mes_file_entry)) {
        buffer[0] = '\0';
        return;
    }

    strcpy(buffer, mes_file_entry.str);
}

// 0x4C5A40
int rumor_copy_state(int64_t obj, RumorInfo* rumors)
{
    int index;
    uint64_t timestamps[MAX_RUMORS];
    int cnt;

    obj_arrayfield_pc_rumor_copy_to_flat(obj, OBJ_F_PC_RUMOR_IDX, MAX_RUMORS - 1, timestamps);

    cnt = 0;
    for (index = 0; index < MAX_RUMORS; index++) {
        if (timestamps[index] != 0) {
            rumors[cnt].num = rumor_idx_to_num(index);
            rumors[cnt].datetime.value = timestamps[index];
            rumors[cnt].quelled = rumor_qstate_get(rumor_idx_to_num(index));
            cnt++;
        }
    }

    qsort(rumors, cnt, sizeof(*rumors), rumor_compare);

    return cnt;
}

// 0x4C5AF0
int rumor_compare(const RumorInfo* a, const RumorInfo* b)
{
    return datetime_compare(&(a->datetime), &(b->datetime));
}

// 0x4C5B10
bool rumor_copy_known(int64_t src_obj, int64_t dst_obj)
{
    int index;
    uint64_t timestamps[MAX_RUMORS];

    if (obj_field_int32_get(src_obj, OBJ_F_TYPE) != OBJ_TYPE_PC
        || obj_field_int32_get(dst_obj, OBJ_F_TYPE) != OBJ_TYPE_PC) {
        return false;
    }

    obj_arrayfield_pc_rumor_copy_to_flat(src_obj, OBJ_F_PC_RUMOR_IDX, MAX_RUMORS - 1, timestamps);

    for (index = 0; index < MAX_RUMORS; index++) {
        if (timestamps[index] != 0) {
            rumor_known_set(dst_obj, index);
        }
    }

    return true;
}
