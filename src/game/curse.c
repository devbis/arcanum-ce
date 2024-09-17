#include "game/curse.h"

#include "game/effect.h"
#include "game/mes.h"
#include "game/multiplayer.h"
#include "game/object.h"
#include "game/player.h"
#include "game/timeevent.h"
#include "game/ui.h"

// TODO: Move John out of ui types.
#include "ui/types.h"

#define CURSE_F_NAME 0
#define CURSE_F_EFFECT 1
#define CURSE_F_DESCRIPTION 2

// TODO: Move to multiplayer packets.
typedef struct ChangeCursePacket {
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
    /* 0038 */ int curse;
    /* 003C */ int add;
} ChangeCursePacket;

static_assert(sizeof(ChangeCursePacket) == 0x40, "wrong size");

static void curse_copy_field(int curse, int field, char* buffer);

// 0x5FF40C
static int curse_mes_file;

// 0x4C3C60
bool curse_mod_load()
{
    mes_load("mes\\gamecurse.mes", &curse_mes_file);
    return true;
}

// 0x4C3C80
void curse_mod_unload()
{
    mes_unload(curse_mes_file);
    curse_mes_file = MES_FILE_HANDLE_INVALID;
}

// 0x4C3CA0
void curse_copy_name(int curse, char* buffer)
{
    curse_copy_field(curse, CURSE_F_NAME, buffer);
}

// 0x4C3CC0
void curse_copy_field(int curse, int field, char* buffer)
{
    MesFileEntry mes_file_entry;

    buffer[0] = '\0';

    if (curse >= 50) {
        mes_file_entry.num = curse * 10 + field;
        if (mes_search(curse_mes_file, &mes_file_entry)) {
            strcpy(buffer, mes_file_entry.str);
        }
    }
}

// 0x4C3D30
void curse_copy_description(int curse, char* buffer)
{
    curse_copy_field(curse, CURSE_F_DESCRIPTION, buffer);
}

// 0x4C3D50
int sub_4C3D50(object_id_t obj, CurseInfo* curses)
{
    int cnt;
    int index;

    if (obj_field_int32_get(obj, OBJ_F_TYPE) != OBJ_TYPE_PC) {
        return 0;
    }

    cnt = obj_arrayfield_length_get(obj, OBJ_F_PC_CURSE_IDX);
    for (index = 0; index < cnt; index++) {
        curses[index].id = sub_407470(obj, OBJ_F_PC_CURSE_IDX, index);
        curses[index].ts = obj_arrayfield_int64_get(obj, OBJ_F_PC_CURSE_TS_IDX, index);
    }

    return cnt;
}

// 0x4C3DD0
bool curse_is_added_to(object_id_t obj, int curse)
{
    int cnt;
    int index;

    if (obj_field_int32_get(obj, OBJ_F_TYPE) != OBJ_TYPE_PC) {
        return false;
    }

    cnt = obj_arrayfield_length_get(obj, OBJ_F_PC_CURSE_IDX);
    for (index = 0; index < cnt; index++) {
        if (sub_407470(obj, OBJ_F_PC_CURSE_IDX, index) == curse) {
            return true;
        }
    }

    return true;
}

// 0x4C3E40
void curse_add(object_id_t obj, int curse)
{
    ChangeCursePacket pkt;
    int cnt;
    DateTime datetime;
    MesFileEntry mes_file_entry;
    John v1;

    if (obj_field_int32_get(obj, OBJ_F_TYPE) != OBJ_TYPE_PC) {
        return;
    }

    if (curse_is_added_to(obj, curse)) {
        return;
    }

    if (!sub_4A2BA0()) {
        if (!(tig_net_flags & TIG_NET_HOST) == 0) {
            return;
        }

        pkt.type = 43;
        sub_4440E0(obj, &(pkt.field_8));
        pkt.curse = curse;
        pkt.add = true;
        tig_net_send_app_all(&pkt, sizeof(pkt));
    }

    cnt = obj_arrayfield_length_get(obj, OBJ_F_PC_CURSE_IDX);
    sub_4074E0(obj, OBJ_F_PC_CURSE_IDX, cnt, curse);

    datetime = sub_45A7C0();
    // TODO: Rethink cast.
    obj_arrayfield_int64_set(obj, OBJ_F_PC_CURSE_TS_IDX, cnt, *(int64_t*)&datetime);

    sub_4E9F70(obj, curse, 4);

    if (player_is_pc_obj(obj)) {
        mes_file_entry.num = 1000;
        mes_get_msg(curse_mes_file, &mes_file_entry);

        v1.type = 3;
        v1.str = mes_file_entry.str;
        v1.field_8 = curse;
        sub_460630(&v1);

        sub_460790(1, 1);
    }
}

// 0x4C3F70
int curse_get_effect(int curse)
{
    char buffer[80];
    curse_copy_field(curse, CURSE_F_EFFECT, buffer);
    return atoi(buffer);
}

// 0x4C3FA0
void curse_remove(object_id_t obj, int curse)
{
    ChangeCursePacket pkt;
    int cnt;
    int index;
    int tmp_curse;
    int64_t tmp_ts;

    if (obj_field_int32_get(obj, OBJ_F_TYPE) != OBJ_TYPE_PC) {
        return;
    }

    if (!sub_4A2BA0()) {
        if (!(tig_net_flags & TIG_NET_HOST) == 0) {
            return;
        }

        pkt.type = 43;
        sub_4440E0(obj, &(pkt.field_8));
        pkt.curse = curse;
        pkt.add = false;
        tig_net_send_app_all(&pkt, sizeof(pkt));
    }

    cnt = obj_arrayfield_length_get(obj, OBJ_F_PC_CURSE_IDX);
    for (index = 0; index < cnt; index++) {
        if (sub_407470(obj, OBJ_F_PC_CURSE_IDX, index) == curse) {
            while (index < cnt - 1) {
                tmp_curse = sub_407470(obj, OBJ_F_PC_CURSE_IDX, index + 1);
                tmp_ts = obj_arrayfield_int64_get(obj, OBJ_F_PC_CURSE_TS_IDX, index + 1);
                sub_4074E0(obj, OBJ_F_PC_CURSE_IDX, index, tmp_curse);
                obj_arrayfield_int64_set(obj, OBJ_F_PC_CURSE_TS_IDX, index, tmp_ts);
                index++;
            }

            obj_arrayfield_length_set(obj, OBJ_F_PC_CURSE_IDX, cnt - 1);
            obj_arrayfield_length_set(obj, OBJ_F_PC_CURSE_TS_IDX, cnt - 1);
            sub_4EA100(obj, curse_get_effect(curse));

            if (player_is_pc_obj(obj)) {
                sub_460790(1, 1);
            }
            break;
        }
    }
}
