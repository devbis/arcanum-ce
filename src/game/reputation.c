#include "game/reputation.h"

#include <stdio.h>

#include "game/critter.h"
#include "game/mes.h"
#include "game/mp_utils.h"
#include "game/multiplayer.h"
#include "game/obj.h"
#include "game/player.h"
#include "game/random.h"
#include "game/reaction.h"
#include "game/stat.h"

#define THOUSAND 1000
#define FOUR 4

typedef enum ReputationType {
    GAME_RP_NPC_M2M,
    GAME_RP_NPC_M2F,
    GAME_RP_NPC_F2F,
    GAME_RP_NPC_F2M,
} ReputationType;

typedef struct ReputationEffectInfo {
    /* 0000 */ int adj;
    /* 0004 */ int origin;
    /* 0008 */ int faction;
} ReputationEffectInfo;

typedef struct ReputationData {
    /* 0000 */ int num_effects;
    /* 0004 */ int factions[3];
    /* 0010 */ ReputationEffectInfo effects[5];
} ReputationData;

// See 0x4C15D0.
static_assert(sizeof(ReputationData) == 0x4C, "wrong size");

static bool sub_4C1730(const char* path, int start, int end);

// 0x5B69C0
static const char* off_5B69C0[FOUR] = {
    "game_rp_npc_m2m",
    "game_rp_npc_m2f",
    "game_rp_npc_f2f",
    "game_rp_npc_f2m",
};

// 0x5FC8AC
static mes_file_handle_t dword_5FC8AC;

// 0x5FC8B0
static ReputationData* dword_5FC8B0;

// 0x5FC8B4
static int* dword_5FC8B4;

// 0x5FC8B8
static int dword_5FC8B8[1000];

// 0x4C15D0
bool reputation_init(GameInitInfo* init_info)
{
    (void)init_info;

    dword_5FC8B0 = (ReputationData*)CALLOC(THOUSAND, sizeof(*dword_5FC8B0));
    dword_5FC8B4 = (int*)CALLOC(FOUR, sizeof(*dword_5FC8B4));

    return true;
}

// 0x4C1600
void reputation_exit()
{
    FREE(dword_5FC8B0);
    FREE(dword_5FC8B4);
}

// 0x4C1620
bool reputation_mod_load()
{
    int index;
    char path[TIG_MAX_PATH];

    for (index = 0; index < FOUR; index++) {
        dword_5FC8B4[index] = -1;
    }

    dword_5FC8AC = MES_FILE_HANDLE_INVALID;

    if (!sub_4C1730("rules\\gamerep.mes", 1000, 1999)) {
        return true;
    }

    if (!mes_load("mes\\gamereplog.mes", &dword_5FC8AC)) {
        return true;
    }

    for (index = 0; index < FOUR; index++) {
        sprintf(path, "mes\\%s.mes", off_5B69C0[index]);
        mes_load(path, &(dword_5FC8B4[index]));
    }

    return true;
}

// 0x4C16E0
void reputation_mod_unload()
{
    int index;

    for (index = 0; index < FOUR; index++) {
        mes_unload(dword_5FC8B4[index]);
        dword_5FC8B4[index] = MES_FILE_HANDLE_INVALID;
    }

    mes_unload(dword_5FC8AC);
    dword_5FC8AC = MES_FILE_HANDLE_INVALID;
}


// 0x4C1730
bool sub_4C1730(const char* path, int start, int end)
{
    mes_file_handle_t mes_file;
    MesFileEntry mes_file_entry;
    int reputation;
    char str[MAX_STRING];
    char* tok;
    int effect;
    bool err;

    if (!mes_load(path, &mes_file)) {
        return false;
    }

    for (reputation = start; reputation < end; reputation++) {
        mes_file_entry.num = reputation;
        if (mes_search(mes_file, &mes_file_entry)) {
            strcpy(str, mes_file_entry.str);

            tok = strtok(str, ",");

            err = true;
            do {
                if (tok == NULL) break;

                dword_5FC8B0[reputation - 1000].factions[0] = atoi(tok);
                while (isspace(*tok)) tok++;
                while (isdigit(*tok)) tok++;
                if (*tok == '\0') break;

                dword_5FC8B0[reputation - 1000].factions[1] = atoi(tok);
                while (isspace(*tok)) tok++;
                while (isdigit(*tok)) tok++;
                if (*tok == '\0') break;

                dword_5FC8B0[reputation - 1000].factions[2] = atoi(tok);

                err = false;
            } while (0);

            if (err) {
                tig_debug_printf("Warning! Reputation %d is missing required faction values!\n", reputation);
                // FIXME: Leaking mes_file.
                return false;
            }

            tok = strtok(NULL, ",");

            // NOTE: Original code is different, but the same thing.
            for (effect = 0; effect < 5; effect++) {
                if (tok == NULL) {
                    break;
                }

                err = true;
                do {
                    dword_5FC8B0[reputation - 1000].effects[effect].adj = atoi(tok);
                    while (isspace(*tok)) tok++;
                    while (isdigit(*tok)) tok++;
                    if (*tok == '\0') break;

                    dword_5FC8B0[reputation - 1000].effects[effect].origin = atoi(tok);
                    while (isspace(*tok)) tok++;
                    while (isdigit(*tok)) tok++;
                    if (*tok == '\0') break;

                    dword_5FC8B0[reputation - 1000].effects[effect].faction = atoi(tok);

                    err = false;
                } while (0);

                if (err) {
                    tig_debug_printf("Warning! Reputation %d has effect with less than 3 parameters\n", reputation);
                }

                tok = strtok(NULL, ",");
            }

            dword_5FC8B0[reputation - 1000].num_effects = effect;

            if (effect == 5 && tok != NULL) {
                tig_debug_printf("Warning! Too many reputation effects for reputation %d\n", reputation);
            }
        }
    }

    mes_unload(mes_file);
    return true;
}

// 0x4C1AC0
int reputation_reaction_adj(int64_t pc_obj, int64_t npc_obj)
{
    int adj = 0;
    int cnt;
    int rep_idx;
    int rep;
    int eff_idx;
    int origin;
    int faction;
    ReputationEffectInfo* effect;

    if (obj_field_int32_get(pc_obj, OBJ_F_TYPE) != OBJ_TYPE_PC) {
        return 0;
    }

    if (obj_field_int32_get(npc_obj, OBJ_F_TYPE) != OBJ_TYPE_NPC) {
        return 0;
    }

    cnt = obj_arrayfield_length_get(pc_obj, OBJ_F_PC_REPUTATION_IDX);
    if (cnt > 0) {
        origin = critter_origin_get(npc_obj);
        faction = critter_faction_get(npc_obj);

        for (rep_idx = 0; rep_idx < cnt; rep_idx++) {
            rep = obj_arrayfield_uint32_get(pc_obj, OBJ_F_PC_REPUTATION_IDX, rep_idx);
            for (eff_idx = 0; eff_idx < dword_5FC8B0[rep - 1000].num_effects; eff_idx++) {
                effect = &(dword_5FC8B0[rep - 1000].effects[eff_idx]);
                if ((effect->origin == 0 || effect->origin == origin)
                    && (effect->faction == 0 || effect->faction == faction)) {
                    adj += effect->adj;
                }
            }
        }
    }

    return adj;
}

// 0x4C1BD0
void reputation_name(int reputation, char* buffer)
{
    MesFileEntry mes_file_entry;

    buffer[0] = '\0';

    if (reputation >= 1000) {
        mes_file_entry.num = reputation;
        if (mes_search(dword_5FC8AC, &mes_file_entry)) {
            strcpy(buffer, mes_file_entry.str);
        }
    }
}

// 0x4C1C30
int reputation_copy_state(int64_t pc_obj, ReputationStateEntry* entries)
{
    int cnt;
    int index;

    if (obj_field_int32_get(pc_obj, OBJ_F_TYPE) != OBJ_TYPE_PC) {
        return 0;
    }

    cnt = obj_arrayfield_length_get(pc_obj, OBJ_F_PC_REPUTATION_IDX);
    for (index = 0; index < cnt; index++) {
        entries[index].reputation = obj_arrayfield_uint32_get(pc_obj, OBJ_F_PC_REPUTATION_IDX, index);
        entries[index].datetime.value = obj_arrayfield_int64_get(pc_obj, OBJ_F_PC_REPUTATION_TS_IDX, index);
    }

    return cnt;
}

// 0x4C1CB0
bool reputation_has(int64_t pc_obj, int reputation)
{
    int cnt;
    int index;

    if (obj_field_int32_get(pc_obj, OBJ_F_TYPE) != OBJ_TYPE_PC) {
        return false;
    }

    cnt = obj_arrayfield_length_get(pc_obj, OBJ_F_PC_REPUTATION_IDX);
    for (index = 0; index < cnt; index++) {
        if (obj_arrayfield_uint32_get(pc_obj, OBJ_F_PC_REPUTATION_IDX, index) == reputation) {
            return true;
        }
    }

    return false;
}

// 0x4C1D20
void reputation_add(int64_t pc_obj, int reputation)
{
    Packet105 pkt;
    int index;

    if (obj_field_int32_get(pc_obj, OBJ_F_TYPE) != OBJ_TYPE_PC) {
        return;
    }

    if (reputation_has(pc_obj, reputation)) {
        return;
    }

    if (!multiplayer_is_locked()) {
        if ((tig_net_flags & TIG_NET_HOST) == 0) {
            return;
        }

        pkt.type = 105;
        pkt.pc_oid = sub_407EF0(pc_obj);
        pkt.reputation = reputation;
        pkt.field_24 = 0;
        tig_net_send_app_all(&pkt, sizeof(pkt));
    }

    index = obj_arrayfield_length_get(pc_obj, OBJ_F_PC_REPUTATION_IDX);
    obj_arrayfield_uint32_set(pc_obj, OBJ_F_PC_REPUTATION_IDX, index, reputation);
    obj_arrayfield_int64_set(pc_obj, OBJ_F_PC_REPUTATION_TS_IDX, index, sub_45A7C0().value);

    if (player_is_pc_obj(pc_obj)) {
        sub_460790(1, 1);
    }
}

// 0x4C1E10
void reputation_remove(int64_t pc_obj, int reputation)
{
    Packet105 pkt;
    int cnt;
    int index;
    int next_rep;
    uint64_t next_ts;

    if (obj_field_int32_get(pc_obj, OBJ_F_TYPE) != OBJ_TYPE_PC) {
        return;
    }

    if (!multiplayer_is_locked()) {
        if ((tig_net_flags & TIG_NET_HOST) == 0) {
            return;
        }

        pkt.type = 105;
        pkt.pc_oid = sub_407EF0(pc_obj);
        pkt.reputation = reputation;
        pkt.field_24 = 0;
        tig_net_send_app_all(&pkt, sizeof(pkt));
    }

    // Find index of the reputation in the reputation array.
    cnt = obj_arrayfield_length_get(pc_obj, OBJ_F_PC_REPUTATION_IDX);
    for (index = 0; index < cnt; index++) {
        if (obj_arrayfield_uint32_get(pc_obj, OBJ_F_PC_REPUTATION_IDX, index) == reputation) {
            break;
        }
    }

    if (index >= cnt) {
        // Index not found, attempt to remove reputation which is not added.
        return;
    }

    // Move subsequent entries one slot down.
    for (; index < cnt - 1; index++) {
        // TODO: Does not look right.
        next_rep = obj_arrayfield_uint32_get(pc_obj, OBJ_F_PC_REPUTATION_IDX, 2 * index);
        next_ts = obj_arrayfield_uint32_get(pc_obj, OBJ_F_PC_REPUTATION_TS_IDX, 2 * index);
        obj_arrayfield_uint32_set(pc_obj, OBJ_F_PC_REPUTATION_IDX, index, next_rep);
        obj_arrayfield_int64_set(pc_obj, OBJ_F_PC_REPUTATION_TS_IDX, index, next_ts);
    }

    // Shrink array fields.
    obj_arrayfield_length_set(pc_obj, OBJ_F_PC_REPUTATION_IDX, cnt - 1);
    obj_arrayfield_length_set(pc_obj, OBJ_F_PC_REPUTATION_TS_IDX, cnt - 1);

    if (player_is_pc_obj(pc_obj)) {
        sub_460790(1, 1);
    }
}

// 0x4C1F80
int sub_4C1F80(int64_t pc_obj, int64_t npc_obj)
{
    int cnt;
    int index;
    int origin;
    int faction;
    int reaction_level;
    int reaction_type;
    int rep;
    int eff;
    ReputationEffectInfo* effect;
    int selected;

    // Make sure `pc_obj` is PC.
    if (obj_field_int32_get(pc_obj, OBJ_F_TYPE) != OBJ_TYPE_PC) {
        return 0;
    }

    // Make sure `npc_obj` is NPC.
    if (obj_field_int32_get(npc_obj, OBJ_F_TYPE) != OBJ_TYPE_NPC) {
        return 0;
    }

    cnt = obj_arrayfield_length_get(pc_obj, OBJ_F_PC_REPUTATION_IDX);
    if (cnt <= 0) {
        return 0;
    }

    origin = critter_origin_get(npc_obj);
    faction = critter_origin_get(pc_obj);
    reaction_level = reaction_get(npc_obj, pc_obj);
    reaction_type = reaction_translate(reaction_level);

    selected = 0;
    for (index = 0; index < cnt; index++) {
        rep = obj_arrayfield_uint32_get(pc_obj, OBJ_F_PC_REPUTATION_IDX, index) - 1000;
        for (eff = 0; eff < dword_5FC8B0[rep].num_effects; eff++) {
            effect = &(dword_5FC8B0[rep].effects[eff]);
            if ((effect->origin == 0 || effect->origin == origin)
                && (effect->faction == 0 || effect->faction == faction)) {
                if ((effect->adj < 0 && reaction_type > REACTION_NEUTRAL)
                    || (effect > 0 && reaction_type < REACTION_NEUTRAL)) {
                    dword_5FC8B8[selected++] = index;
                    break;
                }
            }
        }
    }

    if (selected <= 0) {
        return 0;
    }

    return dword_5FC8B8[random_between(0, selected - 1)] + 1000;
}

// 0x4C2100
void sub_4C2100(int64_t pc_obj, int64_t npc_obj, int reputation, char* buffer)
{
    int rp;
    MesFileEntry mes_file_entry;

    if (obj_field_int32_get(pc_obj, OBJ_F_TYPE) != OBJ_TYPE_PC) {
        buffer[0] = '\0';
        return;
    }

    if (obj_field_int32_get(npc_obj, OBJ_F_TYPE) != OBJ_TYPE_NPC) {
        buffer[0] = '\0';
        return;
    }

    if (stat_level_get(npc_obj, STAT_GENDER) == GENDER_MALE) {
        rp = stat_level_get(pc_obj, STAT_GENDER) == GENDER_MALE
            ? GAME_RP_NPC_M2M
            : GAME_RP_NPC_M2F;
    } else {
        rp = stat_level_get(pc_obj, STAT_GENDER) == GENDER_MALE
            ? GAME_RP_NPC_F2M
            : GAME_RP_NPC_F2F;
    }

    mes_file_entry.num = reputation;
    if (!mes_search(dword_5FC8B4[rp], &mes_file_entry)) {
        buffer[0] = '\0';
        return;
    }

    strcpy(buffer, mes_file_entry.str);
}

// 0x4C21E0
bool sub_4C21E0(int64_t pc_obj, int faction)
{
    int cnt;
    int rep_idx;
    int rep;
    int fac_idx;

    if (faction == 0) {
        return false;
    }

    if (obj_field_int32_get(pc_obj, OBJ_F_TYPE) != OBJ_TYPE_PC) {
        return false;
    }

    cnt = obj_arrayfield_length_get(pc_obj, OBJ_F_PC_REPUTATION_IDX);
    for (rep_idx = 0; rep_idx < cnt; rep_idx++) {
        rep = obj_arrayfield_uint32_get(pc_obj, OBJ_F_PC_REPUTATION_IDX, rep_idx) - 1000;
        for (fac_idx = 0; fac_idx < 3; fac_idx++) {
            if (dword_5FC8B0[rep].factions[fac_idx] == faction) {
                return true;
            }
        }
    }

    return false;
}
