#include "game/obj_id.h"

#include <stdio.h>

#include "game/location.h"
#include "game/map.h"
#include "game/object.h"

static bool sub_4E67A0(GUID* guid, const char* str);
static bool sub_4E6970(ObjectID_P* p, const char* str);
static bool sub_4E6A60(int* value_ptr, const char* str);
static bool sub_4E6AA0(int* value_ptr, const char* str, size_t length);
static bool sub_4E6B00(char* dst, const char* src, int size);

// 0x4E62A0
void objid_create_guid(ObjectID* oid)
{
    if (CoCreateGuid(&(oid->d.g)) != S_OK) {
        tig_debug_println("Error: Unable to generate permanent ID!");
        tig_message_post_quit(0);
    }

    oid->type = OID_TYPE_GUID;
}

// 0x4E62D0
void objid_id_perm_by_load_order(ObjectID* oid, int64_t obj)
{
    if (object_is_static(obj)) {
        oid->d.p.location = obj_field_int64_get(obj, OBJ_F_LOCATION);
        oid->d.p.temp_id = obj_field_int32_get(obj, OBJ_F_TEMP_ID);
        oid->d.p.map = map_current_map();
        oid->type = OID_TYPE_P;
    } else {
        tig_debug_println("Error: Resident object passed to objid_id_perm_by_load_order");
        oid->type = OID_TYPE_NULL;
    }
}

// 0x4E6340
bool objid_is_valid(ObjectID a)
{
    return a.type >= 0 && a.type < 4;
}

// 0x4E6360
bool objid_compare(ObjectID a, ObjectID b)
{
    if (a.type < b.type) return true;
    if (a.type > b.type) return false;

    switch (a.type) {
    case 1:
        return a.d.a < b.d.a;
    case 2:
        if (a.d.g.Data1 < b.d.g.Data1) return true;
        if (a.d.g.Data1 > b.d.g.Data1) return false;
        if (a.d.g.Data2 < b.d.g.Data2) return true;
        if (a.d.g.Data2 > b.d.g.Data2) return false;
        if (a.d.g.Data3 < b.d.g.Data3) return true;
        if (a.d.g.Data3 > b.d.g.Data3) return false;
        if (a.d.g.Data4[0] < b.d.g.Data4[0]) return true;
        if (a.d.g.Data4[0] > b.d.g.Data4[0]) return false;
        if (a.d.g.Data4[1] < b.d.g.Data4[1]) return true;
        if (a.d.g.Data4[1] > b.d.g.Data4[1]) return false;
        if (a.d.g.Data4[2] < b.d.g.Data4[2]) return true;
        if (a.d.g.Data4[2] > b.d.g.Data4[2]) return false;
        if (a.d.g.Data4[3] < b.d.g.Data4[3]) return true;
        if (a.d.g.Data4[3] > b.d.g.Data4[3]) return false;
        if (a.d.g.Data4[4] < b.d.g.Data4[4]) return true;
        if (a.d.g.Data4[4] > b.d.g.Data4[4]) return false;
        if (a.d.g.Data4[5] < b.d.g.Data4[5]) return true;
        if (a.d.g.Data4[5] > b.d.g.Data4[5]) return false;
        if (a.d.g.Data4[6] < b.d.g.Data4[6]) return true;
        if (a.d.g.Data4[6] > b.d.g.Data4[6]) return false;
        if (a.d.g.Data4[7] < b.d.g.Data4[7]) return true;
        if (a.d.g.Data4[7] > b.d.g.Data4[7]) return false;
    case 3:
        if (a.d.p.location < b.d.p.location) return true;
        if (a.d.p.location > b.d.p.location) return false;
        if (a.d.p.temp_id < b.d.p.temp_id) return true;
        if (a.d.p.temp_id > b.d.p.temp_id) return false;
        if (a.d.p.map < b.d.p.map) return true;
        if (a.d.p.map > b.d.p.map) return false;
    }

    return false;
}

// 0x4E6490
bool objid_is_equal(ObjectID a, ObjectID b)
{
    if (a.type == b.type) {
        switch (a.type) {
        case OID_TYPE_NULL:
            return true;
        case OID_TYPE_A:
            return a.d.a == b.d.a;
        case OID_TYPE_GUID:
            return InlineIsEqualGUID(&(a.d.g), &(b.d.g));
        case OID_TYPE_P:
            return a.d.p.location == b.d.p.location
                && a.d.p.temp_id == b.d.p.temp_id
                && a.d.p.map == b.d.p.map;
        }
    }

    return false;
}

// 0x4E6540
ObjectID sub_4E6540(int a1)
{
    ObjectID oid;
    oid.type = OID_TYPE_A;
    oid.d.a = a1;
    return oid;
}

// 0x4E6570
void objid_id_to_str(char* buffer, ObjectID oid)
{
    switch (oid.type) {
    case OID_TYPE_HANDLE:
        sprintf(buffer, "Handle_%I64X", oid.d.h);
        break;
    case OID_TYPE_BLOCKED:
        strcpy(buffer, "Blocked");
        break;
    case OID_TYPE_NULL:
        strcpy(buffer, "NULL");
        break;
    case OID_TYPE_A:
        sprintf(buffer, "A_%08X", oid.d.a);
        break;
    case OID_TYPE_GUID:
        sprintf(buffer, "G_%08X_%04X_%04X_%02X%02X_%02X%02X%02X%02X%02X%02X",
            oid.d.g.Data1,
            oid.d.g.Data2,
            oid.d.g.Data3,
            oid.d.g.Data4[0],
            oid.d.g.Data4[1],
            oid.d.g.Data4[2],
            oid.d.g.Data4[3],
            oid.d.g.Data4[4],
            oid.d.g.Data4[5],
            oid.d.g.Data4[6],
            oid.d.g.Data4[7]);
        break;
    case OID_TYPE_P:
        sprintf(buffer, "P_%08I64X_%08I64X_%08X_%08X",
            LOCATION_GET_X(oid.d.p.location),
            LOCATION_GET_Y(oid.d.p.location),
            oid.d.p.temp_id,
            oid.d.p.map);
        break;
    }
}

// 0x4E66B0
bool objid_id_from_str(ObjectID* oid, const char* str)
{
    ObjectID temp_oid;

    switch (str[0]) {
    case 'A':
        temp_oid.type = OID_TYPE_A;
        if (!sub_4E6A60(&(temp_oid.d.a), str)) {
            return false;
        }
        break;
    case 'B':
        temp_oid.type = OID_TYPE_BLOCKED;
        break;
    case 'G':
        temp_oid.type = OID_TYPE_GUID;
        if (!sub_4E67A0(&(temp_oid.d.g), str)) {
            return false;
        }
        break;
    case 'H':
        tig_debug_println("Handle not handled in objid_id_from_str");
        return false;
    case 'N':
        temp_oid.type = OID_TYPE_NULL;
        break;
    case 'P':
        temp_oid.type = OID_TYPE_P;
        if (!sub_4E6970(&(temp_oid.d.p), str)) {
            return false;
        }
        break;
    default:
        return false;
    }

    *oid = temp_oid;

    return true;
}

// 0x4E67A0
bool sub_4E67A0(GUID* guid, const char* str)
{
    int value;

    if (str[0] != 'G' || str[1] != '_') return false;
    str += 2;

    if (!sub_4E6AA0(&value, str, 8)) return false;
    guid->Data1 = value;
    str += 8;

    if (str[0] != '_') return false;
    str++;

    if (!sub_4E6AA0(&value, str, 4)) return false;
    guid->Data2 = value;
    str += 4;

    if (str[0] != '_') return false;
    str++;

    if (!sub_4E6AA0(&value, str, 4)) return false;
    guid->Data3 = value;
    str += 4;

    if (str[0] != '_') return false;
    str++;

    if (!sub_4E6AA0(&value, str, 2)) return false;
    guid->Data4[0] = value;
    str += 2;

    if (!sub_4E6AA0(&value, str, 2)) return false;
    guid->Data4[1] = value;
    str += 2;

    if (str[0] != '_') return false;
    str++;

    if (!sub_4E6AA0(&value, str, 2)) return false;
    guid->Data4[2] = value;
    str += 2;

    if (!sub_4E6AA0(&value, str, 2)) return false;
    guid->Data4[3] = value;
    str += 2;

    if (!sub_4E6AA0(&value, str, 2)) return false;
    guid->Data4[4] = value;
    str += 2;

    if (!sub_4E6AA0(&value, str, 2)) return false;
    guid->Data4[5] = value;
    str += 2;

    if (!sub_4E6AA0(&value, str, 2)) return false;
    guid->Data4[6] = value;
    str += 2;

    if (!sub_4E6AA0(&value, str, 2)) return false;
    guid->Data4[7] = value;

    return true;
}

// 0x4E6970
bool sub_4E6970(ObjectID_P* p, const char* str)
{
    int x;
    int y;
    int temp_id;
    int map;

    if (str[0] != 'P' || str[1] != '_') {
        return false;
    }

    str += 2;
    if (!sub_4E6AA0(&x, str, 8)) {
        return false;
    }

    str += 8;
    if (str[0] != '_') return false;
    if (!sub_4E6AA0(&y, str, 8)) return false;
    p->location = location_make(x, y);

    str += 8;
    if (str[0] != '_') return false;
    if (!sub_4E6AA0(&temp_id, str, 8)) return false;
    p->temp_id = temp_id;

    str += 8;
    if (str[0] != '_') return false;
    if (!sub_4E6AA0(&map, str, 8)) return false;
    p->map = map;

    return true;
}

// 0x4E6A60
bool sub_4E6A60(int* value_ptr, const char* str)
{
    int value;

    if (str[0] != 'A' || str[1] != '_') {
        return false;
    }

    if (!sub_4E6AA0(&value, str + 2, 8)) {
        return false;
    }

    *value_ptr = value;
    return true;
}

// 0x4E6AA0
bool sub_4E6AA0(int* value_ptr, const char* str, size_t length)
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

// 0x4E6B00
bool sub_4E6B00(char* dst, const char* src, int size)
{
    int index;

    for (index = 0; index < size; index++) {
        if (!((src[index] >= '0' && src[index] <= '9')
            || (src[index] >= 'A' && src[index] <= 'F')
            || (src[index] >= 'a' && src[index] <= 'f'))) {
            return false;
        }

        dst[index] = src[index];
    }

    dst[index] = '\0';
    return true;
}
