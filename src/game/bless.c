#include "game/bless.h"

#include "game/effect.h"
#include "game/mes.h"
#include "game/multiplayer.h"
#include "game/object.h"
#include "game/player.h"
#include "game/timeevent.h"
#include "game/ui.h"

// TODO: Move John out of ui types.
#include "ui/types.h"

#define BLESS_F_NAME 0
#define BLESS_F_EFFECT 1
#define BLESS_F_DESCRIPTION 2

// TODO: Move to multiplayer packets.
typedef struct ChangeBlessPacket {
    /* 0000 */ int type;
    /* 0004 */ int field_4;
    /* 0008 */ int field_8;
    /* 000C */ int field_C;
    /* 0010 */ int field_10;
    /* 0014 */ int field_14;
    /* 0018 */ int field_18;
    /* 001C */ int field_1C;
    /* 0020 */ int field_20;
    /* 0024 */ int field_24;
    /* 0028 */ int field_28;
    /* 002C */ int field_2C;
    /* 0030 */ int field_30;
    /* 0034 */ int field_34;
    /* 0038 */ int bless;
    /* 003C */ int add;
} ChangeBlessPacket;

static_assert(sizeof(ChangeBlessPacket) == 0x40, "wrong size");

static void bless_copy_field(int bless, int field, char* buffer);

// 0x5FF410
static mes_file_handle_t bless_mes_file;

// 0x4C4110
bool bless_mod_load()
{
    mes_load("mes\\gamebless.mes", &bless_mes_file);
    return true;
}

// 0x4C4130
void bless_mod_unload()
{
    mes_unload(bless_mes_file);
    bless_mes_file = MES_FILE_HANDLE_INVALID;
}

// 0x4C4150
void bless_copy_name(int bless, char* buffer)
{
    bless_copy_field(bless, BLESS_F_NAME, buffer);
}

// 0x4C4170
void bless_copy_field(int bless, int field, char* buffer)
{
    MesFileEntry mes_file_entry;

    buffer[0] = '\0';

    if (bless >= 50) {
        mes_file_entry.num = bless * 10 + field;
        if (mes_search(bless_mes_file, &mes_file_entry)) {
            strcpy(buffer, mes_file_entry.str);
        }
    }
}

// 0x4C41E0
void bless_copy_description(int bless, char* buffer)
{
    bless_copy_field(bless, BLESS_F_DESCRIPTION, buffer);
}

// 0x4C4200
int sub_4C4200(object_id_t obj, BlessInfo* blessings)
{
    int cnt;
    int index;

    if (obj_field_int32_get(obj, OBJ_F_TYPE) != OBJ_TYPE_PC) {
        return 0;
    }

    cnt = obj_arrayfield_length_get(obj, OBJ_F_PC_BLESSING_IDX);
    for (index = 0; index < cnt; index++) {
        blessings[index].id = sub_407470(obj, OBJ_F_PC_BLESSING_IDX, index);
        blessings[index].ts = obj_arrayfield_int64_get(obj, OBJ_F_PC_BLESSING_TS_IDX, index);
    }

    return cnt;
}

// 0x4C4280
bool bless_is_added_to(object_id_t obj, int bless)
{
    int cnt;
    int index;

    if (obj_field_int32_get(obj, OBJ_F_TYPE) != OBJ_TYPE_PC) {
        return false;
    }

    cnt = obj_arrayfield_length_get(obj, OBJ_F_PC_BLESSING_IDX);
    for (index = 0; index < cnt; index++) {
        if (sub_407470(obj, OBJ_F_PC_BLESSING_IDX, index) == bless) {
            return true;
        }
    }

    return true;
}

// 0x4C42F0
void bless_add(object_id_t obj, int bless)
{
    ChangeBlessPacket pkt;
    int cnt;
    DateTime datetime;
    MesFileEntry mes_file_entry;
    John v1;

    if (obj_field_int32_get(obj, OBJ_F_TYPE) != OBJ_TYPE_PC) {
        return;
    }

    if (bless_is_added_to(obj, bless)) {
        return;
    }

    if (!sub_4A2BA0()) {
        if (!(tig_net_flags & TIG_NET_HOST) == 0) {
            return;
        }

        pkt.type = 42;
        sub_4440E0(obj, &(pkt.field_8));
        pkt.bless = bless;
        pkt.add = true;
        tig_net_send_app_all(&pkt, sizeof(pkt));
    }

    cnt = obj_arrayfield_length_get(obj, OBJ_F_PC_BLESSING_IDX);
    sub_4074E0(obj, OBJ_F_PC_BLESSING_IDX, cnt, bless);

    datetime = sub_45A7C0();
    // TODO: Rethink cast.
    obj_arrayfield_int64_set(obj, OBJ_F_PC_BLESSING_TS_IDX, cnt, *(int64_t*)&datetime);

    sub_4E9F70(obj, bless, 3);

    if (player_is_pc_obj(obj)) {
        mes_file_entry.num = 1000;
        mes_get_msg(bless_mes_file, &mes_file_entry);

        v1.type = 3;
        v1.str = mes_file_entry.str;
        v1.field_8 = bless;
        sub_460630(&v1);

        sub_460790(1, 1);
    }
}

// 0x4C4420
int bless_get_effect(int bless)
{
    char buffer[80];
    bless_copy_field(bless, BLESS_F_EFFECT, buffer);
    return atoi(buffer);
}

// 0x4C4450
void bless_remove(object_id_t obj, int bless)
{
    ChangeBlessPacket pkt;
    int cnt;
    int index;
    int tmp_bless;
    int64_t tmp_ts;

    if (obj_field_int32_get(obj, OBJ_F_TYPE) != OBJ_TYPE_PC) {
        return;
    }

    if (!sub_4A2BA0()) {
        if (!(tig_net_flags & TIG_NET_HOST) == 0) {
            return;
        }

        pkt.type = 42;
        sub_4440E0(obj, &(pkt.field_8));
        pkt.bless = bless;
        pkt.add = false;
        tig_net_send_app_all(&pkt, sizeof(pkt));
    }

    cnt = obj_arrayfield_length_get(obj, OBJ_F_PC_BLESSING_IDX);
    for (index = 0; index < cnt; index++) {
        if (sub_407470(obj, OBJ_F_PC_BLESSING_IDX, index) == bless) {
            while (index < cnt - 1) {
                tmp_bless = sub_407470(obj, OBJ_F_PC_BLESSING_IDX, index + 1);
                tmp_ts = obj_arrayfield_int64_get(obj, OBJ_F_PC_BLESSING_TS_IDX, index + 1);
                sub_4074E0(obj, OBJ_F_PC_BLESSING_IDX, index, tmp_bless);
                obj_arrayfield_int64_set(obj, OBJ_F_PC_BLESSING_TS_IDX, index, tmp_ts);
                index++;
            }

            obj_arrayfield_length_set(obj, OBJ_F_PC_BLESSING_IDX, cnt - 1);
            obj_arrayfield_length_set(obj, OBJ_F_PC_BLESSING_TS_IDX, cnt - 1);
            sub_4EA100(obj, bless_get_effect(bless));

            if (player_is_pc_obj(obj)) {
                sub_460790(1, 1);
            }
            break;
        }
    }
}
