#include "game/obj_private.h"

#include <stdio.h>

#include "game/map.h"

#define ONE_TWO_EIGHT 128

typedef struct FindNode {
    /* 0000 */ unsigned int flags;
    /* 0004 */ int field_4;
    /* 0008 */ int64_t obj;
    /* 0010 */ int field_10;
    /* 0014 */ FindNode* next;
    /* 0018 */ int field_18;
    /* 001C */ int field_1C;
} FindNode;

// See 0x4E3BE0.
static_assert(sizeof(FindNode) == 0x20, "wrong size");

typedef struct S60369C {
    /* 0000 */ int field_0;
    /* 0004 */ int field_4;
    /* 0008 */ int field_8;
    /* 000C */ int field_C;
} S60369C;

// See 0x4E3DD0.
static_assert(sizeof(S60369C) == 0x10, "wrong size");

static void sub_4E3BE0();
static void sub_4E3C60();
static void obj_find_node_allocate(FindNode** obj_find_node);
static void obj_find_node_deallocate(FindNode* obj_find_node);
static void sub_4E3DD0();
static bool objid_compare(ObjectID a, ObjectID b);
static bool sub_4E67A0(GUID* guid, char* str);
static bool sub_4E6AA0(int* value_ptr, char* str, size_t length);
static bool handle_from_fname(int64_t* handle_ptr, const char* path);

// 0x60368C
static FindNode* dword_60368C;

// 0x603690
static int dword_603690;

// 0x603694
static FindNode** dword_603694;

// 0x603698
static int dword_603698;

// 0x60369C
static S60369C* dword_60369C;

// 0x6036A0
static int dword_6036A0;

// 0x6036A4
static bool obj_find_initialized;

// 0x6036A8
static bool dword_6036A8;

// 0x4E3900
void obj_find_init()
{
    if (!obj_find_initialized) {
        dword_6036A0 = 0;
        dword_603694 = NULL;
        dword_60368C = NULL;
        sub_4E3BE0();

        dword_603690 = 0;
        dword_603698 = 0;
        dword_60369C = NULL;
        sub_4E3DD0();

        obj_find_initialized = true;
    }
}

// 0x4E3950
void obj_find_exit()
{
    if (obj_find_initialized) {
        sub_4E3C60();

        if (dword_60369C != NULL) {
            free(dword_60369C);
        }

        obj_find_initialized = false;
    }
}

// 0x4E3980
void obj_find_add(int64_t obj)
{
    int64_t location;
    int64_t v1;
    FindNode* find_node;
    S60369C* v2;

    location = obj_f_get_int64(obj, OBJ_F_LOCATION);
    v1 = sub_4CFC50(location);
    obj_find_node_allocate(&find_node);
    find_node->obj = obj;
    sub_4E3E90(v1, &v2);
    sub_4E3D30(v2, find_node);
    obj_f_set_int32(obj, OBJ_F_FIND_NODE, find_node);
}

// 0x4E39F0
void obj_find_remove(int64_t obj)
{
    FindNode* find_node;

    find_node = obj_f_get_int32(obj, OBJ_F_FIND_NODE);
    if ((find_node->flags & 0x02) == 0) {
        tig_debug_println("Error: Node already released in obj_find_remove.");
        return;
    }

    if (find_node->obj != obj) {
        tig_debug_println("Error: Node already reassigned to different handlein obj_find_remove.");
        return;
    }

    obj_find_node_detach(find_node);
    obj_find_node_deallocate(find_node);
    obj_f_set_int32(obj, OBJ_F_FIND_NODE, NULL);
}

// 0x4E3A70
void obj_find_move(int64_t obj)
{
    int64_t location;
    int64_t v1;
    FindNode* find_node;
    S60369C* v2;

    location = obj_f_get_int64(obj, OBJ_F_LOCATION);
    v1 = sub_4CFC50(location);
    find_node = (FindNode*)obj_f_get_int32(obj, OBJ_F_FIND_NODE);
    if (find_node == NULL) {
        return;
    }

    if ((find_node->flags & 0x02) == 0) {
        tig_debug_println("Error: Node already released in obj_find_move.");
        return;
    }

    if (find_node->obj != obj) {
        tig_debug_println("Error: Node already reassigned to different handlein obj_find_move");
        return;
    }

    if (find_node->field_18 != v1) {
        obj_find_node_detach(find_node);
        sub_4E3E90(v1, &v2);
        sub_4E3D30(v2, find_node);
    }
}

// 0x4E3BE0
void sub_4E3BE0()
{
    int index;

    dword_6036A0++;

    if (dword_603694 == NULL) {
        dword_603694 = (FindNode**)malloc(sizeof(*dword_603694) * dword_6036A0);
    } else {
        dword_603694 = (FindNode**)realloc(dword_603694, sizeof(*dword_603694) * dword_6036A0);
    }

    dword_603694[dword_6036A0 - 1] = (FindNode*)malloc(sizeof(FindNode) * ONE_TWO_EIGHT);

    for (index = 0; index < ONE_TWO_EIGHT; index++) {
        obj_find_node_deallocate(&(dword_603694[dword_6036A0 - 1][index]));
    }
}

// 0x4E3C60
void sub_4E3C60()
{
    while (dword_6036A0 != 0) {
        FREE(dword_603694[--dword_6036A0]);
    }

    FREE(dword_603694);
    dword_603694 = NULL;
}

// 0x4E3CB0
void obj_find_node_allocate(FindNode** obj_find_node)
{
    if (dword_60368C == NULL) {
        sub_4E3BE0();
    }

    *obj_find_node = dword_60368C;

    if (((*obj_find_node)->flags & 0x02) != 0) {
        tig_debug_println("Error: Allocating twice in obj_find_node_allocate.");
        tig_message_quit(0);
    }

    (*obj_find_node)->flags |= 0x02;

    dword_60368C = dword_60368C->next;
}

// 0x4E3D10
void obj_find_node_deallocate(FindNode* obj_find_node)
{
    obj_find_node->flags = 0;
    obj_find_node->next = dword_60368C;
    dword_60368C = obj_find_node;
}

// 0x4E3DD0
void sub_4E3DD0()
{
    dword_603698 += 32;

    if (dword_60369C != NULL) {
        dword_60369C = (S60369C*)realloc(dword_60369C, sizeof(*dword_60369C) * dword_603698);
    } else {
        dword_60369C = (S60369C*)malloc(sizeof(*dword_60369C) * dword_603698);
    }
}

// 0x4E3F80
void sub_4E3F80()
{
    dword_6036A8 = true;
}

// 0x4E3F90
void sub_4E3F90()
{
    dword_6036A8 = false;
}

// 0x4E62A0
void sub_4E62A0(ObjectID* object_id)
{
    if (CoCreateGuid(&(object_id->g)) != S_OK) {
        tig_debug_println("Error: Unable to generate permanent ID!");
        tig_message_quit(0);
    }

    object_id->field_0 = 2;
}

// 0x4E62D0
void objid_id_perm_by_load_order(ObjectID* object_id, object_id_t obj)
{
    if (sub_43D990(obj)) {
        object_id->p.location = object_field_get_64(obj, OBJ_F_LOCATION);
        object_id->p.temp_id = object_field_get(obj, OBJ_F_TEMP_ID);
        object_id->p.map = sub_40FF40();
        object_id->field_0 = 3;
    } else {
        tig_debug_println("Error: Resident object passed to objid_id_perm_by_load_order");
        object_id->field_0 = 0;
    }
}

// 0x4E6340
bool objid_is_valid(ObjectID a)
{
    return a.field_0 >= 0 && a.field_0 < 4;
}

// 0x4E6360
bool objid_compare(ObjectID a, ObjectID b)
{
    if (a.field_0 < b.field_0) return true;
    if (a.field_0 > b.field_0) return false;

    switch (a.field_0) {
    case 1:
        return a.a.field_8 < b.a.field_8;
    case 2:
        // TODO: Incomplete.
    case 3:
        if (a.p.location < b.p.location) return true;
        if (a.p.location > b.p.location) return false;
        if (a.p.temp_id < b.p.temp_id) return true;
        if (a.p.temp_id > b.p.temp_id) return false;
        if (a.p.map < b.p.map) return true;
        if (a.p.map > b.p.map) return false;
    }

    return false;
}

// 0x4E6490
bool objid_is_equal(ObjectID a, ObjectID b)
{
    if (a.field_0 == b.field_0) {
        switch (a.field_0) {
        case 0:
            return true;
        case 1:
            return a.a.field_8 == b.a.field_8;
        case 2:
            return InlineIsEqualGUID(&(a.g), &(b.g));
        case 3:
            return a.p.location == b.p.location
                && a.p.temp_id == b.p.temp_id
                && a.p.map == b.p.map;
        }
    }

    return false;
}

// 0x4E6540
ObjectID sub_4E6540(int a1)
{
    ObjectID object_id;
    object_id.field_0 = 1;
    object_id.a.field_8 = a1;
    return object_id;
}

// 0x4E6570
void objid_id_to_str(char* buffer, ObjectID object_id)
{
    switch (object_id.field_0) {
    case -2:
        sprintf(buffer, "Handle_%I64X", object_id.h.field_8);
        break;
    case -1:
        strcpy(buffer, "Blocked");
        break;
    case 0:
        strcpy(buffer, "NULL");
        break;
    case 1:
        sprintf(buffer, "A_%08X", object_id.a.field_8);
        break;
    case 2:
        sprintf(buffer, "G_%08X_%04X_%04X_%02X%02X_%02X%02X%02X%02X%02X%02X",
            object_id.g.Data1,
            object_id.g.Data2,
            object_id.g.Data3,
            object_id.g.Data4[0],
            object_id.g.Data4[1],
            object_id.g.Data4[2],
            object_id.g.Data4[3],
            object_id.g.Data4[4],
            object_id.g.Data4[5],
            object_id.g.Data4[6],
            object_id.g.Data4[7]);
        break;
    case 3:
        sprintf(buffer, "P_%08I64X_%08I64X_%08X_%08X",
            object_id.p.location,
            object_id.p.temp_id,
            object_id.p.map);
        break;
    }
}

// 0x4E66B0
bool objid_id_from_str(ObjectID* object_id, const char* str)
{
    ObjectID temp_object_id;

    switch (str[0]) {
    case 'A':
        temp_object_id.field_0 = 1;
        if (!sub_4E6A60(&(temp_object_id.a), str)) {
            return false;
        }
        break;
    case 'B':
        temp_object_id.field_0 = -1;
        break;
    case 'G':
        temp_object_id.field_0 = 2;
        if (!sub_4E67A0(&(temp_object_id.g), str)) {
            return false;
        }
        break;
    case 'H':
        tig_debug_println("Handle not handled in objid_id_from_str");
        return false;
    case 'N':
        temp_object_id.field_0 = 0;
        break;
    case 'P':
        temp_object_id.field_0 = 3;
        if (!sub_4E6970(&(temp_object_id.p), str)) {
            return false;
        }
        break;
    default:
        return false;
    }

    *object_id = temp_object_id;

    return true;
}

// 0x4E67A0
bool sub_4E67A0(GUID* guid, char* str)
{
    int value;

    if (str[0] != 'G' || str[1] != '_') {
        return false;
    }

    str += 2;
    if (!sub_4E6AA0(&value, str, 8)) return false;
    guid->Data1 = value;

    str += 8;
    if (str[0] != '_') return false;
    if (!sub_4E6AA0(&value, str, 4)) return false;
    guid->Data2 = value;

    str += 4;
    if (str[0] != '_') return false;
    if (!sub_4E6AA0(&value, str, 4)) return false;
    guid->Data3 = value;

    str += 4;
    if (str[0] != '_') return false;
    if (!sub_4E6AA0(&value, str, 2)) return false;
    guid->Data4[0] = value;

    str += 2;
    if (str[0] != '_') return false;
    if (!sub_4E6AA0(&value, str, 2)) return false;
    guid->Data4[1] = value;

    str += 2;
    if (str[0] != '_') return false;
    if (!sub_4E6AA0(&value, str, 2)) return false;
    guid->Data4[2] = value;

    str += 2;
    if (str[0] != '_') return false;
    if (!sub_4E6AA0(&value, str, 2)) return false;
    guid->Data4[3] = value;

    str += 2;
    if (str[0] != '_') return false;
    if (!sub_4E6AA0(&value, str, 2)) return false;
    guid->Data4[4] = value;

    str += 2;
    if (str[0] != '_') return false;
    if (!sub_4E6AA0(&value, str, 2)) return false;
    guid->Data4[5] = value;

    str += 2;
    if (str[0] != '_') return false;
    if (!sub_4E6AA0(&value, str, 2)) return false;
    guid->Data4[6] = value;

    str += 2;
    if (str[0] != '_') return false;
    if (!sub_4E6AA0(&value, str, 2)) return false;
    guid->Data4[7] = value;

    return true;
}

// 0x4E6AA0
bool sub_4E6AA0(int* value_ptr, char* str, size_t length)
{
    char temp[12];
    char* end;
    int value;

    if (!sub_4E6B00(temp, str, length)) {
        return false;
    }

    value = (int)strtoul(temp, &end, 16);
    if (&(temp[length]) != end) {
        return false;
    }

    *value_ptr = value;
    return true;
}

// 0x4E71B0
bool handle_from_fname(int64_t* handle_ptr, const char* path)
{
    char fname[MAX_PATH];
    ObjectID object_id;

    _splitpath(path, NULL, NULL, fname, NULL);

    if (fname[0] == '\0') {
        tig_debug_printf("handle_from_fname found empty base_name infilename [%s]", path);
        return false;
    }

    if (!objid_id_from_str(&object_id, fname)) {
        tig_debug_printf("Unable to extract id from filename [%s]\n", path);
        return false;
    }

    if (object_id.field_0 <= 0 || object_id.field_0 > 3) {
        tig_debug_printf("Wrong id type in filename [%s]\n", path);
        return false;
    }

    *handle_ptr = objp_perm_lookup(object_id);
    if (*handle_ptr == OBJ_HANDLE_NULL) {
        tig_debug_printf("ID not loaded, can't convert to handle in ObjFile,handle_from_fname: [%s]", fname);
        return false;
    }

    return true;
}
