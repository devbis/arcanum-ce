#include "game/obj_pool.h"

#include "game/object.h"
#include "game/map.h"

typedef struct S6036B8 {
    /* 0000 */ ObjectID field_0;
    /* 0018 */ int64_t field_18;
} S6036B8;

static_assert(sizeof(S6036B8) == 0x20, "wrong size");

static int sub_4E5640();
static void sub_4E56A0(int index);
static bool sub_4E56E0();
static void sub_4E5770(int index);
static bool sub_4E57E0(ObjectID a1, int* index_ptr);
static int64_t sub_4E58C0(int a1, int a2);
static int sub_4E5900(int64_t a1);
static uint8_t* sub_4E5920(int index);
static void* object_ptr(int index);
static void sub_4E5980(int* a1, int a2);
static int sub_4E59A0(int* a1);

// 0x5B9258
static int dword_5B9258 = 4;

// 0x6036B0
static int dword_6036B0;

// 0x6036B4
static bool obj_priv_editor;

// 0x6036B8
static S6036B8* dword_6036B8;

// 0x6036BC
static uint8_t** object_pool_buckets;

// 0x6036C0
static int dword_6036C0;

// 0x6036C4
static int dword_6036C4;

// 0x6036C8
static int object_pool_capacity;

// 0x6036CC
static int object_pool_num_buckets;

// 0x6036D0
static int dword_6036D0;

// 0x6036D4
static int object_pool_size_plus_padding;

// 0x6036D8
static int* dword_6036D8;

// 0x6036DC
static int dword_6036DC;

// 0x6036E0
static int dword_6036E0;

// 0x6036E4
static int object_size_plus_padding;

// 0x6036E8
static int dword_6036E8;

// 0x6036F0
static int64_t qword_6036F0;

// 0x6036F8
static bool obj_priv_initialized;

// 0x4E4CD0
void obj_pool_init(int size, bool editor)
{
    object_pool_buckets = (uint8_t**)CALLOC(256, sizeof(*object_pool_buckets));
    obj_priv_editor = editor;
    dword_6036E8 = 0x80000;
    object_pool_capacity = 0x2000;
    dword_6036B0 = 2 * ((int)time(0) & 0x3FFFFF) + 1;
    object_size_plus_padding = dword_5B9258 + size;
    object_pool_size_plus_padding = object_size_plus_padding * 8192;
    object_pool_buckets[0] = (uint8_t*)MALLOC(object_pool_size_plus_padding);
    object_pool_num_buckets = 1;
    dword_6036C4 = 0;
    dword_6036E0 = 4096;
    dword_6036D8 = (int*)MALLOC(sizeof(*dword_6036D8) * dword_6036E0);
    dword_6036D0 = 0;
    dword_6036C0 = 1024;
    dword_6036DC = 0;
    qword_6036F0 = OBJ_HANDLE_NULL;
    dword_6036B8 = (S6036B8 *)MALLOC(sizeof(*dword_6036B8) * dword_6036C0);
    obj_priv_initialized = true;
}

// 0x4E4DB0
void obj_pool_exit()
{
    int index;
    uint8_t* data;
    int v1;
    int64_t obj;

    if (dword_6036C4 != 0) {
        tig_debug_printf("Destroying %d leftover objects.\n", dword_6036C4);
        for (index = dword_6036D0 + dword_6036C4 - 1; index >= 0; index--) {
            data = sub_4E5920(index);
            if (data[0] == 'H') {
                // TODO: Review cast.
                v1 = sub_4E59A0((int*)data);
                obj = sub_4E58C0(index, v1);
                sub_405BF0(obj);
            }
        }
    }

    for (index = 0; index < object_pool_num_buckets; index++) {
        FREE(object_pool_buckets[index]);
    }

    FREE(dword_6036D8);
    FREE(dword_6036B8);
    FREE(object_pool_buckets);
    obj_priv_initialized = 0;
}

// 0x4E4E60
void* obj_pool_allocate(int64_t* obj_ptr)
{
    int v1;
    uint8_t* data;
    void* object;

    if (dword_6036C4 == 0x200000) {
        tig_message_post_quit(0);
        return NULL;
    }

    if (qword_6036F0 != OBJ_HANDLE_NULL) {
        *obj_ptr = qword_6036F0;

        v1 = sub_4E5900(*obj_ptr);

        data = sub_4E5920(v1);
        data[0] = 'H';
        // TODO: Review cast.
        sub_4E5980((int*)data, (*obj_ptr >> 3) & 0x7FFFFF);

        object = object_ptr(v1);
        dword_6036C4++;
        dword_6036D0--;
        qword_6036F0 = OBJ_HANDLE_NULL;
    } else {
        v1 = sub_4E5640();
        object = object_ptr(v1);
        *obj_ptr = sub_4E58C0(v1, dword_6036B0);

        data = sub_4E5920(v1);
        // TODO: Review cast.
        sub_4E5980((int*)data, dword_6036B0);

        dword_6036B0++;
        if (dword_6036B0 > 0x7FFFFF) {
            dword_6036B0 = 1;
        }
    }

    return object;
}

// 0x4E4F80
void* obj_pool_lock(int64_t obj)
{
    return object_ptr(sub_4E5900(obj));
}

// 0x4E4FA0
void obj_pool_unlock(int64_t obj)
{
    (void)obj;
}

// 0x4E4FB0
void obj_pool_deallocate(int64_t obj)
{
    sub_4E56A0(sub_4E5900(obj));
}

// 0x4E4FD0
void sub_4E4FD0(ObjectID a1, int64_t obj)
{
    int index;

    if (sub_4E57E0(a1, &index)) {
        dword_6036B8[index].field_18 = obj;
        return;
    }

    if (dword_6036DC == dword_6036C0) {
        dword_6036C0 += 0x200;
        if (dword_6036C0 > 0x200000) {
            return;
        }

        dword_6036B8 = (S6036B8*)REALLOC(dword_6036B8, sizeof(*dword_6036B8) * dword_6036C0);
    }

    if (index != dword_6036DC) {
        memcpy(&(dword_6036B8[index + 1]),
            &(dword_6036B8[index]),
            sizeof(*dword_6036B8) * (dword_6036DC - index));
    }

    dword_6036B8[index].field_0 = a1;
    dword_6036B8[index].field_18 = obj;
    dword_6036DC++;
}

// 0x4E50E0
int64_t objp_perm_lookup(ObjectID oid)
{
    int idx;
    ObjectList objects;
    ObjectNode* node;

    if (sub_4E57E0(oid, &idx)) {
        if (sub_4E5470(dword_6036B8[idx].field_18)) {
            return dword_6036B8[idx].field_18;
        }
    }

    if (oid.type != OID_TYPE_P) {
        return OBJ_HANDLE_NULL;
    }

    if (obj_priv_editor) {
        return OBJ_HANDLE_NULL;
    }

    if (map_current_map() != oid.d.p.map) {
        return OBJ_HANDLE_NULL;
    }

    object_list_location(oid.d.p.location, OBJ_TM_TRAP | OBJ_TM_WALL | OBJ_TM_PORTAL | OBJ_TM_SCENERY, &objects);

    if (objects.num_sectors != 1) {
        tig_debug_println("Warning: objp_perm_lookup found sectors != 1");
        object_list_destroy(&objects);
        return OBJ_HANDLE_NULL;
    }

    node = objects.head;
    while (node != NULL) {
        if (obj_field_int32_get(node->obj, OBJ_F_TEMP_ID) == oid.d.p.temp_id) {
            break;
        }
        node = node->next;
    }

    if (node != NULL) {
        sub_4E4FD0(oid, node->obj);
        object_list_destroy(&objects);

        if (!sub_4E57E0(oid, &idx)) {
            tig_debug_println("Error: objp_perm_lookup can't find handle just added with positional id.");
            return OBJ_HANDLE_NULL;
        }

        return dword_6036B8[idx].field_18;
    }

    object_list_destroy(&objects);
    return OBJ_HANDLE_NULL;
}

// 0x4E5280
ObjectID sub_4E5280(int64_t obj)
{
    ObjectID oid;
    int index;

    for (index = 0; index < dword_6036DC; index++) {
        if (dword_6036B8[index].field_18 == obj) {
            return dword_6036B8[index].field_0;
        }
    }

    oid.type = OID_TYPE_NULL;

    return oid;
}

// 0x4E52F0
void sub_4E52F0(ObjectID oid)
{
    (void)oid;
}

// 0x4E5300
void sub_4E5300()
{
    S6036B8* v1;
    int cnt;
    int v2;
    int v3;

    cnt = 0;
    v1 = MALLOC(sizeof(*v1) * dword_6036DC);

    v2 = dword_6036DC - 1;
    v3 = v2;
    while (v2 >= 0) {
        if (dword_6036B8[v2].field_0.type == OID_TYPE_A) {
            v1[v3] = dword_6036B8[v2];
            v3--;
            cnt++;
        }
        v2--;
    }

    memset(dword_6036B8, 0, sizeof(*dword_6036B8) * dword_6036DC);
    memcpy(dword_6036B8, &(v1[v3 + 1]), sizeof(*dword_6036B8) * cnt);
    FREE(v1);
    dword_6036DC = cnt;
}

// 0x4E53C0
bool obj_pool_walk_first(int64_t* obj_ptr, int* iter_ptr)
{
    int iter;
    uint8_t* data;

    if (dword_6036C4 == 0) {
        return false;
    }

    iter = dword_6036D0 + dword_6036C4 - 1;
    while (iter >= 0) {
        data = sub_4E5920(iter);
        if (*data == 'H') {
            *obj_ptr = sub_4E58C0(iter, sub_4E59A0(data));
            *iter_ptr = iter;
            return true;
        }
        iter--;
    }

    *iter_ptr = iter;
    return false;
}

// 0x4E5420
bool obj_pool_walk_next(int64_t* obj_ptr, int* iter_ptr)
{
    int iter;
    uint8_t* data;

    iter = *iter_ptr - 1;
    while (iter >= 0) {
        data = sub_4E5920(iter);
        if (*data == 'H') {
            *obj_ptr = sub_4E58C0(iter, sub_4E59A0(data));
            *iter_ptr = iter;
            return true;
        }
        iter--;
    }

    *iter_ptr = iter;
    return false;
}

// 0x4E5470
bool sub_4E5470(int64_t obj)
{
    int64_t v1;
    uint8_t* data;

    if (obj == OBJ_HANDLE_NULL) {
        return true;
    }

    if ((obj & 0x7) != 2) {
        return false;
    }

    v1 = obj >> 29;
    if (v1 >= dword_6036C4 + dword_6036D0) {
        return false;
    }

    data = sub_4E5920(v1);
    if (sub_4E59A0(data) != ((obj >> 3) & 0x7FFFFF)) {
        return false;
    }

    if (data[0] != 'H') {
        return false;
    }

    return true;
}

// 0x4E5530
int sub_4E5530(int64_t obj)
{
    int v1;
    int v2;
    int v3;
    uint8_t* data;

    if ((obj & 0x7) != 2) {
        tig_debug_println("Handle requested is not marked as a handle.");
        return false;
    }

    v1 = sub_4E5900(obj);
    if (v1 < dword_6036D0 + dword_6036C4) {
        data = sub_4E5920(v1);
        if (data[0] != 'P') {
            tig_debug_println("Index of handle requested is not released.");
            return false;
        }

        qword_6036F0 = obj;
        return true;
    }

    if (v1 >= 0x200000) {
        tig_debug_println("Index of handle requested is too large.");
        return false;
    }

    v2 = v1 - dword_6036D0 - dword_6036C4 + 1;
    while (v1 >= object_pool_capacity) {
        if (!sub_4E56E0()) {
            return false;
        }
    }

    v3 = dword_6036D0 + dword_6036C4;
    while (v2 != 0) {
        sub_4E5770(v3);
        data = sub_4E5920(v3);
        sub_4E5980(data, 0);
        data[0] = 'P';
        v3++;
        v2--;
    }

    qword_6036F0 = obj;
    return true;
}

// 0x4E5640
int sub_4E5640()
{
    int v1;
    uint8_t* data;

    if (dword_6036D0 > 0) {
        v1 = dword_6036D8[--dword_6036D0];
        dword_6036C4++;
        data = sub_4E5920(v1);
        data[0] = 'H';
        return v1;
    }

    if (dword_6036C4 == object_pool_capacity) {
        if (!sub_4E56E0()) {
            return 0;
        }
    }

    v1 = dword_6036C4++;
    data = sub_4E5920(v1);
    data[0] = 'H';
    return v1;
}

// 0x4E56A0
void sub_4E56A0(int index)
{
    uint8_t* data;

    data = sub_4E5920(index);
    sub_4E5770(index);
    dword_6036C4--;
    // TODO: Review cast.
    sub_4E5980((int*)data, 0);
    data[0] = 'P';
}

// 0x4E56E0
bool sub_4E56E0()
{
    if (object_pool_capacity < 0x200000) {
        object_pool_capacity += 0x2000;
        if (dword_6036E8 <= dword_6036C4 + 1) {
            tig_debug_printf("WARNING: storing %d objects --", dword_6036C4 + 1);
            tig_debug_printf(" near %d, game will crash!\n", 0x200000);
        }

        object_pool_buckets[object_pool_num_buckets++] = (uint8_t*)MALLOC(object_pool_size_plus_padding);

        return true;
    }

    if (object_pool_capacity > 0x200000) {
        tig_debug_println("Object pool element cap exceeded.  Capacity tracking out of sync");
    }

    return false;
}

// 0x4E5770
void sub_4E5770(int index)
{
    if (dword_6036D0 == dword_6036E0) {
        dword_6036E0 += 2048;
        dword_6036D8 = (int*)REALLOC(dword_6036D8, sizeof(*dword_6036D8) * dword_6036E0);
    }

    dword_6036D8[dword_6036D0] = index;
    dword_6036D0++;
}

// 0x4E57E0
bool sub_4E57E0(ObjectID a1, int* index_ptr)
{
    int l;
    int r;
    int m;

    l = 0;
    r = dword_6036DC - 1;
    while (l <= r) {
        m = (l + r) / 2;
        // FIXME: Unnecessary copying.
        if (objid_compare(dword_6036B8[m].field_0, a1)) {
            l = m + 1;
        } else if (objid_compare(a1, dword_6036B8[m].field_0)) {
            r = m - 1;
        } else {
            *index_ptr = m;
            return true;
        }
    }

    *index_ptr = l;
    return false;
}

// 0x4E58C0
int64_t sub_4E58C0(int a1, int a2)
{
    return 8 * (a2 + ((int64_t)a1 << 26)) + 2;
}

// 0x4E5900
int sub_4E5900(int64_t a1)
{
    return (int)(a1 >> 29);
}

// 0x4E5920
uint8_t* sub_4E5920(int index)
{
    return object_pool_buckets[index / 8192] + object_size_plus_padding * (index % 8192);
}

// 0x4E5960
void* object_ptr(int index)
{
    uint8_t* data;

    data = sub_4E5920(index);

    return data + dword_5B9258;
}

// 0x4E5980
void sub_4E5980(int* a1, int a2)
{
    *a1 = (a2 << 8) + (uint8_t)*a1;
}

// 0x4E59A0
int sub_4E59A0(int* a1)
{
    return *a1 >> 8;
}
