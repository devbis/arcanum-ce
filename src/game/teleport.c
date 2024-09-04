#include "game/teleport.h"

#include "game/combat.h"
#include "game/map.h"
#include "game/player.h"

typedef struct S6018B8 {
    int field_0;
    int field_4;
    int64_t obj;
    struct S6018B8* next;
} S6018B8;

// See 0x4D3F00.
static_assert(sizeof(S6018B8) == 0x18, "wrong size");

static bool sub_4D39A0(TeleportData* teleport_data);
static void sub_4D3D60(int64_t obj);
static void sub_4D3E20(int64_t obj);
static void sub_4D3E80();
static S6018B8* sub_4D3EB0();
static void sub_4D3EE0(S6018B8* node);
static void sub_4D3F00();
static void sub_4D3F30();

// 0x601840
static S6018B8* dword_601840;

// 0x601844
static bool dword_601844;

// 0x601848
static GameContextF8* dword_601848;

// 0x60184C
static bool dword_60184C;

// 0x601850
static GameContextFC* dword_601850;

// 0x601858
static TeleportData stru_601858;

// 0x6018B8
static S6018B8* off_6018B8;

// 0x4D32D0
bool teleport_init(GameInitInfo* init_info)
{
    dword_601848 = init_info->field_8;
    dword_601850 = init_info->field_C;
    dword_601844 = false;

    return true;
}

// 0x4D3300
bool teleport_reset()
{
    sub_4D3F30();
    dword_601844 = false;
    return true;
}

// 0x4D3320
void teleport_exit()
{
    teleport_reset();
}

// 0x4D3330
void teleport_ping()
{
    if (dword_601844) {
        dword_60184C = true;
        sub_4D3460(&stru_601858);
        dword_60184C = false;

        dword_601844 = false;

        if ((stru_601858.flags & TELEPORT_FLAG_0x80000000) != 0
            && (stru_601858.flags & TELEPORT_FLAG_0x00000020) != 0) {
            sub_402FA0();
        }
    }
}

// 0x4D3380
bool sub_4D3380(TeleportData* teleport_data)
{
    if (dword_601844) {
        if ((stru_601858.flags & TELEPORT_FLAG_0x80000000) != 0) {
            return false;
        }
    }

    stru_601858 = *teleport_data;
    dword_601844 = true;

    if (player_is_pc_obj(teleport_data->field_8)) {
        stru_601858.flags |= TELEPORT_FLAG_0x80000000;

        if ((stru_601858.flags & TELEPORT_FLAG_0x00000002) != 0) {
            sub_4BDFA0(&(stru_601858.fade));
        }

        if ((stru_601858.flags & TELEPORT_FLAG_0x00000020) != 0) {
            sub_402FC0();
            sub_402F90();
        }
    }

    return true;
}

// 0x4D3410
bool sub_4D3410()
{
    return dword_60184C;
}

// 0x4D3420
bool sub_4D3420(int64_t obj)
{
    S6018B8* node;

    node = dword_601840;
    while (node != NULL) {
        if (node->obj == obj) {
            return true;
        }
        node = node->next;
    }

    return false;
}

// 0x4D39A0
bool sub_4D39A0(TeleportData* teleport_data)
{
    int curr_map;
    char* curr_map_name;
    char curr_map_path[TIG_MAX_PATH];
    char* dst_map_name;
    char dst_map_path[TIG_MAX_PATH];
    S6018B8* node;
    unsigned int flags;
    char path[TIG_MAX_PATH];
    TigFile* stream;

    curr_map = sub_40FF40();
    if (curr_map != teleport_data->map) {
        if (!map_get_name(curr_map, &curr_map_name)) {
            return false;
        }

        strcpy(curr_map_path, "Save\\Current");
        strcat(curr_map_path, "\\maps\\");
        strcat(curr_map_path, curr_map_name);

        if (!map_get_name(teleport_data->map, &dst_map_name)) {
            return false;
        }

        strcpy(dst_map_path, "Save\\Current");
        strcat(dst_map_path, "\\maps\\");
        strcat(dst_map_path, dst_map_name);
        tig_file_mkdir_ex(dst_map_path);

        node = dword_601840;
        while (node != NULL) {
            sub_4D3E20(node->obj);
            node = node->next;
        }

        node = dword_601840;
        while (node != NULL) {
            sub_4064B0(node->obj);
            node = node->next;
        }

        node = dword_601840;
        while (node != NULL) {
            flags = obj_field_int32_get(node->obj, OBJ_F_FLAGS);
            flags |= OF_DYNAMIC;
            flags |= OF_TELEPORTED;
            obj_field_int32_set(node->obj, OBJ_F_FLAGS, flags);
            obj_field_int64_set(node->obj, OBJ_F_LOCATION, teleport_data->field_10);
            obj_field_int32_set(node->obj, OBJ_F_OFFSET_X, 0);
            obj_field_int32_set(node->obj, OBJ_F_OFFSET_Y, 0);

            sprintf(path, "%s\\mobile.mdy", dst_map_path);
            stream = tig_file_fopen(path, "ab");
            if (stream == NULL) {
                return false;
            }

            if (!obj_write(stream, node->obj)) {
                // FIXME: Leaking file.
                return false;
            }

            tig_file_fclose(stream);
            flags &= ~OF_DYNAMIC;
            flags &= ~OF_TELEPORTED;
            flags |= OF_DESTROYED;
            flags |= OF_EXTINCT;
            obj_field_int32_set(node->obj, OBJ_F_FLAGS, flags);

            node = node->next;
        }

        node = dword_601840;
        while (node != NULL) {
            sub_406520(node->obj);
            node = node->next;
        }

        sub_4D3E80();
        return true;
    }
}

// 0x4D3D60
void sub_4D3D60(int64_t obj)
{
    int type;

    type = obj_field_int32_get(obj, OBJ_F_TYPE);
    sub_424070(obj, 5, 0, 1);

    // NOTE: Conditions looks odd, check (note fallthrough from npc block).
    switch (type) {
    case OBJ_TYPE_NPC:
        sub_45F710(obj);
        mp_obj_field_obj_set(obj, OBJ_F_NPC_COMBAT_FOCUS, OBJ_HANDLE_NULL);
        mp_obj_field_obj_set(obj, OBJ_F_NPC_WHO_HIT_ME_LAST, OBJ_HANDLE_NULL);
        sub_4F0500(obj, 0x130);
        mp_obj_field_obj_set(obj, OBJ_F_NPC_SUBSTITUTE_INVENTORY, OBJ_HANDLE_NULL);
    case OBJ_TYPE_PC:
        mp_obj_field_obj_set(obj, OBJ_F_CRITTER_FLEEING_FROM, OBJ_HANDLE_NULL);
        if (player_is_pc_obj(obj)) {
            sub_460B20();
        }
    }

    sub_4D9990(obj);
    sub_4D9A90(obj);
}

// 0x4D3E20
void sub_4D3E20(int64_t obj)
{
    sub_4D3D60(obj);
    combat_critter_deactivate_combat_mode(obj);
    if (obj_field_int32_get(obj, OBJ_F_TYPE) == OBJ_TYPE_NPC) {
        sub_45F710(obj);
    }
    sub_4AD7D0(obj);
    sub_4601D0(obj);
    sub_43CF70(obj);
}

// 0x4D3E80
void sub_4D3E80()
{
    S6018B8* next;

    while (dword_601840 != NULL) {
        next = dword_601840->next;
        sub_4D3EE0(dword_601840);
        dword_601840 = next;
    }
}

// 0x4D3EB0
S6018B8* sub_4D3EB0()
{
    S6018B8* node;

    node = off_6018B8;
    if (node == NULL) {
        sub_4D3F00();
        node = off_6018B8;
    }

    off_6018B8 = node->next;
    node->next = NULL;

    return node;
}

// 0x4D3EE0
void sub_4D3EE0(S6018B8* node)
{
    node->next = off_6018B8;
    off_6018B8 = node;
}

// 0x4D3F00
void sub_4D3F00()
{
    int index;
    S6018B8* node;

    if (off_6018B8 == NULL) {
        for (index = 0; index < 32; index++) {
            node = (S6018B8*)MALLOC(sizeof(*node));
            node->next = off_6018B8;
            off_6018B8 = node;
        }
    }
}

// 0x4D3F30
void sub_4D3F30()
{
    S6018B8* curr;
    S6018B8* next;

    curr = off_6018B8;
    while (curr != NULL) {
        next = curr->next;
        FREE(curr);
        curr = next;
    }
    off_6018B8 = NULL;
}
