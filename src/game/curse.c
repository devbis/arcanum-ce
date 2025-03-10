#include "game/curse.h"

#include "game/effect.h"
#include "game/mes.h"
#include "game/mp_utils.h"
#include "game/multiplayer.h"
#include "game/object.h"
#include "game/player.h"
#include "game/timeevent.h"
#include "game/ui.h"

#define CURSE_F_NAME 0
#define CURSE_F_EFFECT 1
#define CURSE_F_DESCRIPTION 2

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
int sub_4C3D50(int64_t obj, CurseInfo* curses)
{
    int cnt;
    int index;

    if (obj_field_int32_get(obj, OBJ_F_TYPE) != OBJ_TYPE_PC) {
        return 0;
    }

    cnt = obj_arrayfield_length_get(obj, OBJ_F_PC_CURSE_IDX);
    for (index = 0; index < cnt; index++) {
        curses[index].id = obj_arrayfield_uint32_get(obj, OBJ_F_PC_CURSE_IDX, index);
        curses[index].datetime.value = obj_arrayfield_int64_get(obj, OBJ_F_PC_CURSE_TS_IDX, index);
    }

    return cnt;
}

// 0x4C3DD0
bool curse_is_added_to(int64_t obj, int curse)
{
    int cnt;
    int index;

    if (obj_field_int32_get(obj, OBJ_F_TYPE) != OBJ_TYPE_PC) {
        return false;
    }

    cnt = obj_arrayfield_length_get(obj, OBJ_F_PC_CURSE_IDX);
    for (index = 0; index < cnt; index++) {
        if (obj_arrayfield_uint32_get(obj, OBJ_F_PC_CURSE_IDX, index) == curse) {
            return true;
        }
    }

    return true;
}

// 0x4C3E40
void curse_add(int64_t obj, int curse)
{
    int cnt;
    MesFileEntry mes_file_entry;
    UiMessage ui_message;

    if (obj_field_int32_get(obj, OBJ_F_TYPE) != OBJ_TYPE_PC) {
        return;
    }

    if (curse_is_added_to(obj, curse)) {
        return;
    }

    if (!multiplayer_is_locked()) {
        ChangeCursePacket pkt;

        if (!tig_net_is_host()) {
            return;
        }

        pkt.type = 43;
        sub_4440E0(obj, &(pkt.field_8));
        pkt.curse = curse;
        pkt.add = true;
        tig_net_send_app_all(&pkt, sizeof(pkt));
    }

    cnt = obj_arrayfield_length_get(obj, OBJ_F_PC_CURSE_IDX);
    obj_arrayfield_uint32_set(obj, OBJ_F_PC_CURSE_IDX, cnt, curse);

    obj_arrayfield_int64_set(obj, OBJ_F_PC_CURSE_TS_IDX, cnt, sub_45A7C0().value);

    effect_add(obj, curse, EFFECT_CAUSE_CURSE);

    if (player_is_pc_obj(obj)) {
        mes_file_entry.num = 1000;
        mes_get_msg(curse_mes_file, &mes_file_entry);

        ui_message.type = UI_MSG_TYPE_CURSE;
        ui_message.str = mes_file_entry.str;
        ui_message.field_8 = curse;
        sub_460630(&ui_message);

        ui_toggle_primary_button(UI_PRIMARY_BUTTON_LOGBOOK, true);
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
void curse_remove(int64_t obj, int curse)
{
    int cnt;
    int index;
    int tmp_curse;
    int64_t tmp_ts;

    if (obj_field_int32_get(obj, OBJ_F_TYPE) != OBJ_TYPE_PC) {
        return;
    }

    if (!multiplayer_is_locked()) {
        ChangeCursePacket pkt;

        if (!tig_net_is_host()) {
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
        if (obj_arrayfield_uint32_get(obj, OBJ_F_PC_CURSE_IDX, index) == curse) {
            while (index < cnt - 1) {
                tmp_curse = obj_arrayfield_uint32_get(obj, OBJ_F_PC_CURSE_IDX, index + 1);
                tmp_ts = obj_arrayfield_int64_get(obj, OBJ_F_PC_CURSE_TS_IDX, index + 1);
                obj_arrayfield_uint32_set(obj, OBJ_F_PC_CURSE_IDX, index, tmp_curse);
                obj_arrayfield_int64_set(obj, OBJ_F_PC_CURSE_TS_IDX, index, tmp_ts);
                index++;
            }

            obj_arrayfield_length_set(obj, OBJ_F_PC_CURSE_IDX, cnt - 1);
            obj_arrayfield_length_set(obj, OBJ_F_PC_CURSE_TS_IDX, cnt - 1);
            effect_remove_one_typed(obj, curse_get_effect(curse));

            if (player_is_pc_obj(obj)) {
                ui_toggle_primary_button(UI_PRIMARY_BUTTON_LOGBOOK, true);
            }
            break;
        }
    }
}
