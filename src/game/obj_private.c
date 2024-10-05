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

typedef struct S603710 {
    /* 0000 */ int field_0;
    /* 0004 */ int field_4;
} S603710;

typedef struct S6036B8 {
    /* 0000 */ ObjectID field_0;
    /* 0018 */ int field_18;
    /* 001C */ int field_1C;
} S6036B8;

static_assert(sizeof(S6036B8) == 0x20, "wrong size");

static void sub_4E3BE0();
static void sub_4E3C60();
static void obj_find_node_allocate(FindNode** obj_find_node);
static void obj_find_node_deallocate(FindNode* obj_find_node);
static void sub_4E3DD0();
static void sub_4E4C80(S4E4BD0* a1, int size);
static void sub_4E56A0(int index);
static int64_t sub_4E58C0(int a1, int a2);
static int64_t sub_4E5900(int64_t a1);
static uint8_t* sub_4E5920(int index);
static Object* object_ptr(int index);
static void sub_4E5980(int* a1, int a2);
static int sub_4E59A0(int* a1);
static int sub_4E61E0(int a1);
static int sub_4E61F0(int a1);
static bool objid_compare(ObjectID a, ObjectID b);
static bool sub_4E67A0(GUID* guid, char* str);
static bool sub_4E6970(ObjectID_P* p, const char* str);
static bool sub_4E6A60(int* value_ptr, const char* str);
static bool sub_4E6AA0(int* value_ptr, const char* str, size_t length);
static bool sub_4E6B00(char* dst, const char* src, int size);
static bool sub_4E7050(int64_t handle, char* path);
static TigFile* open_solitary_for_write(int64_t handle, const char* dir, const char* ext);
static bool handle_from_fname(int64_t* handle_ptr, const char* path);

// 0x5B9258
static int dword_5B9258 = 4;

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
static void* dword_6036D8;

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

// 0x603700
static int dword_603700;

// 0x603704
static int dword_603704;

// 0x603708
static int dword_603708;

// Capacity: 0x603704
// Size: 0x603714
//
// 0x60370C
static int* dword_60370C;

// Capacity: 0x603700
// Size: 0x603724
//
// 0x603710
static S603710* dword_603710;

// 0x603714
static int dword_603714;

// Capacity: 0x60371C
//
// 0x603718
static int* dword_603718;

// 0x60371C
static int dword_60371C;

// 0x603724
static int dword_603724;

// 0x603728
static bool dword_603728;

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

    location = obj_field_int64_get(obj, OBJ_F_LOCATION);
    v1 = sub_4CFC50(location);
    obj_find_node_allocate(&find_node);
    find_node->obj = obj;
    sub_4E3E90(v1, &v2);
    sub_4E3D30(v2, find_node);
    obj_field_int32_set(obj, OBJ_F_FIND_NODE, find_node);
}

// 0x4E39F0
void obj_find_remove(int64_t obj)
{
    FindNode* find_node;

    find_node = obj_field_int32_get(obj, OBJ_F_FIND_NODE);
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
    obj_field_int32_set(obj, OBJ_F_FIND_NODE, NULL);
}

// 0x4E3A70
void obj_find_move(int64_t obj)
{
    int64_t location;
    int64_t v1;
    FindNode* find_node;
    S60369C* v2;

    location = obj_field_int64_get(obj, OBJ_F_LOCATION);
    v1 = sub_4CFC50(location);
    find_node = (FindNode*)obj_field_int32_get(obj, OBJ_F_FIND_NODE);
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

// 0x4E4BD0
void sub_4E4BD0(S4E4BD0* a1)
{
    a1->field_0 = (uint8_t*)MALLOC(256);
    a1->field_4 = a1->field_0;
    a1->field_8 = 256;
    a1->field_C = a1->field_8;
}

// 0x4E4C00
void sub_4E4C00(const void* data, int size, S4E4BD0* a3)
{
    sub_4E4C80(a3, size);
    memcpy(a3->field_4, data, size);
    a3->field_4 += size;
    a3->field_C -= size;
}

// 0x4E4C50
void sub_4E4C50(void* data, int size, S4E4BD0* a3)
{
    memcpy(data, a3->field_0, size);
    a3->field_0 += size;
}

// 0x4E4C80
void sub_4E4C80(S4E4BD0* a1, int size)
{
    int extra_size;
    int new_size;

    extra_size = size - a1->field_C;
    if (extra_size > 0) {
        new_size = (extra_size / 256 + 1) * 256;
        a1->field_8 += new_size;
        a1->field_0 = (uint8_t*)REALLOC(a1->field_0, a1->field_8);
        a1->field_4 = a1->field_0 + a1->field_8 - a1->field_C - new_size;
        a1->field_C += new_size;
    }
}

// 0x4E4CD0
void sub_4E4CD0(int size, bool editor)
{
    object_pool_buckets = (uint8_t**)CALLOC(256, sizeof(*object_pool_buckets));
    obj_priv_editor = editor;
    dword_6036E8 = 0x80000;
    object_pool_capacity = 0x2000;
    dword_6036B0 = 2 * ((int)time(0) & 0x3FFFFF) + 1;
    object_size_plus_padding = dword_5B9258 + size;
    object_pool_size_plus_padding = (dword_5B9258 + size) * 8192;
    object_pool_buckets[0] = (Object*)MALLOC(object_pool_size_plus_padding);
    object_pool_num_buckets = 1;
    dword_6036C4 = 0;
    dword_6036E0 = 4096;
    dword_6036D8 = MALLOC(0x4000u);
    dword_6036D0 = 0;
    dword_6036C0 = 1024;
    dword_6036DC = 0;
    qword_6036F0 = OBJ_HANDLE_NULL;
    dword_6036B8 = (S6036B8 *)MALLOC(sizeof(*dword_6036B8) * dword_6036C0);
    obj_priv_initialized = true;
}

// 0x4E4DB0
void sub_4E4DB0()
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
Object* sub_4E4E60(int64_t* obj_ptr)
{
    int v1;
    uint8_t* data;
    Object* object;

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
Object* sub_4E4F80(int64_t obj)
{
    return object_ptr(sub_4E5900(obj));
}

// 0x4E4FA0
void sub_4E4FA0(int64_t obj)
{
}

// 0x4E4FB0
void sub_4E4FB0(int64_t obj)
{
    sub_4E56A0(sub_4E5900(obj));
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

// 0x4E58C0
int64_t sub_4E58C0(int a1, int a2)
{
    return 8 * (a2 + ((int64_t)a1 << 26)) + 2;
}

// 0x4E5900
int64_t sub_4E5900(int64_t a1)
{
    return a1 >> 29;
}

// 0x4E5920
uint8_t* sub_4E5920(int index)
{
    return object_pool_buckets[index / 8192] + object_size_plus_padding * (index % 8192);
}

// 0x4E5960
Object* object_ptr(int index)
{
    uint8_t* data;

    data = sub_4E5920(index);

    return (Object*)(data + dword_5B9258);
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

// 0x4E59B0
void sub_4E59B0()
{
    dword_603724 = 0;
    dword_603700 = 4096;
    dword_603714 = 0;
    dword_603704 = 4096;
    dword_603708 = 0;
    dword_60371C = 8192;
    dword_603710 = (S603710*)MALLOC(sizeof(*dword_603710) * dword_603700);
    dword_60370C = (int*)MALLOC(sizeof(*dword_60370C) * dword_603704);
    dword_603718 = (int*)MALLOC(sizeof(*dword_603718) * dword_60371C);
    dword_6036FC = MALLOC(0x10000);
    dword_603720 = MALLOC(0x84);
    sub_4E6210();
    sub_4E6240();
    dword_603728 = true;
}

// 0x4E5A50
void sub_4E5A50()
{
    FREE(dword_603720);
    FREE(dword_6036FC);
    FREE(dword_603718);
    FREE(dword_60370C);
    FREE(dword_603710);
    dword_603728 = false;
}

// 0x4E5AA0
int sub_4E5AA0()
{
    int index;

    if (dword_603714 != 0) {
        return dword_60370C[--dword_603714];
    }

    if (dword_603724 == dword_603700) {
        dword_603700 += 4096;
        dword_603710 = (S603710*)REALLOC(dword_603710, sizeof(S603710) * dword_603700);
    }

    index = dword_603724++;
    if (index == 0) {
        dword_603710[0].field_4 = index;
    } else {
        dword_603710[index].field_4 = dword_603710[index - 1].field_0 + dword_603710[index - 1].field_4;
    }

    dword_603710[index].field_0 = 0;
    sub_4E6040(index, 2);

    return index;
}

// 0x4E5B40
int sub_4E5B40(int a1)
{
    S603710* v1;
    int index;

    v1 = &(dword_603710[a1]);
    if (v1->field_0 != 2) {
        sub_4E6130(a1, v1->field_0 - 2);
    }

    for (index = v1->field_4; index < index + v1->field_0; index++) {
        dword_603718[index++] = 0;
    }

    if (dword_603714 == dword_603704) {
        dword_603704 += 4096;
        dword_60370C = (int *)REALLOC(dword_60370C, sizeof(int) * dword_603704);
    }

    dword_60370C[dword_603714] = a1;

    return ++dword_603714;
}

// 0x4E5BF0
int sub_4E5BF0(int a1)
{
    int v1;
    int v2;
    int index;

    v1 = sub_4E5AA0();
    v2 = dword_603710[a1].field_0 - dword_603710[v1].field_0;
    if (v2 != 0) {
        sub_4E6040(v1, v2);
    }

    for (index = 0; index < dword_603710[a1].field_0; index++) {
        dword_603718[dword_603710[v1].field_4 + index] = dword_603718[dword_603710[a1].field_4 + index];
    }

    return v1;
}

// 0x4E5C60
void sub_4E5C60(int a1, int a2, bool a3)
{
    int v1;
    int v2;

    v1 = sub_4E61E0(a2);
    if (v1 > dword_603710[a1].field_0) {
        if (!a3) {
            return;
        }

        sub_4E6040(a1, v1 - dword_603710[a1].field_0 + 1);
    }

    v2 = sub_4E61F0(a2);
    if (a3) {
        dword_603718[v1 + dword_603710[a1].field_4] |= v2;
    } else {
        dword_603718[v1 + dword_603710[a1].field_4] &= ~v2;
    }
}

// 0x4E5CE0
int sub_4E5CE0(int a1, int a2)
{
    int v1;
    int v2;

    v1 = sub_4E61E0(a2);
    if (v1 > dword_603710[a1].field_0 - 1) {
        return 0;
    }

    v2 = sub_4E61F0(a2);
    return v2 & dword_603718[v1 + dword_603710[a1].field_4];
}

// 0x4E5D30
int sub_4E5D30(int a1, int a2)
{
    int v1 = 0;
    int v2;
    int v3;
    int v4;

    v2 = dword_603710[a1].field_4;
    v3 = sub_4E61E0(a2);
    v4 = v2 + v3;
    if (v3 < dword_603710[a1].field_0) {
        v1 += sub_4E5FE0(dword_603718[v4], a2 % 32);
    } else {
        v4 = v2 + dword_603710[a1].field_0;
    }

    while (v2 < v4) {
        v1 += sub_4E5FE0(dword_603718[v2++], 32);
    }

    return v1;
}

// 0x4E5DB0
void sub_4E5DB0()
{
    // TODO: Incomplete.
}

// 0x4E5E20
bool sub_4E5E20(int a1, TigFile* stream)
{
    if (!tig_file_fwrite(&(dword_603710[a1].field_0), sizeof(int), 1, stream) != 1) {
        return false;
    }

    if (!tig_file_fwrite(&(dword_603718[dword_603710[a1].field_4]), sizeof(int) * dword_603710[a1].field_0, 1, stream) != 1) {
        return false;
    }

    return true;
}

// 0x4E5E80
bool sub_4E5E80(int* a1, TigFile* stream)
{
    int v1;
    int v2;

    v1 = sub_4E5AA0();

    if (tig_file_fread(&v2, sizeof(v2), 1, stream) != 1) {
        return 0;
    }

    if (v2 != dword_603710[v1].field_0 && v2 - dword_603710[v1].field_0 > 0) {
        sub_4E6040(v1, v2 - dword_603710[v1].field_0);
    }

    if (tig_file_fread(&(dword_603718[dword_603710[v1].field_4]), 4 * v2, 1, stream) != 1) {
        return 0;
    }

    *a1 = v1;

    return true;
}

// 0x4E5F10
void sub_4E5F10(int a1)
{
    return 4 * (dword_603710[a1].field_0 + 1);
}

// 0x4E5F30
void sub_4E5F30()
{
    // TODO: Incomplete.
}

// 0x4E5F70
void sub_4E5F70()
{
    // TODO: Incomplete.
}

// 0x4E5FE0
void sub_4E5FE0()
{
    // TODO: Incomplete.
}

// 0x4E6040
void sub_4E6040()
{
    // TODO: Incomplete.
}

// 0x4E6130
void sub_4E6130()
{
    // TODO: Incomplete.
}

// 0x4E61B0
void sub_4E61B0(int start, int end, int inc)
{
    int index;

    for (index = start; index <= end; index++) {
        dword_603710[index].field_4 += inc;
    }
}

// 0x4E61E0
int sub_4E61E0(int a1)
{
    return a1 / 32;
}

// 0x4E61F0
int sub_4E61F0(int a1)
{
    return 1 << (a1 % 32);
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
        object_id->p.location = obj_field_int64_get(obj, OBJ_F_LOCATION);
        object_id->p.temp_id = obj_field_int32_get(obj, OBJ_F_TEMP_ID);
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
    p->location = (x << 32) | y;

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

    if (!sub_4E6AA0(value, str + 2, 8)) {
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

// 0x4E6B60
bool objf_solitary_write(int64_t handle, const char* dir, const char* ext)
{
    TigFile* stream;

    stream = open_solitary_for_write(handle, dir, ext);
    if (stream == NULL) {
        return false;
    }

    if (!obj_write(stream, handle)) {
        tig_file_fclose(stream);
        return false;
    }

    tig_file_fclose(stream);
    return true;
}

// 0x4E6BC0
bool objf_solitary_read(int64_t* handle_ptr, const char* path)
{
    TigFile* stream;

    stream = tig_file_fopen(path, "rb");
    if (stream == NULL) {
        tig_debug_printf("Unable to open [%s] in ObjFile, objf_solitary_read\n", path);
        return false;
    }

    if (!obj_read(stream, handle_ptr)) {
        tig_file_fclose(stream);
        return false;
    }

    tig_file_fclose(stream);
    return true;
}

// 0x4E6D10
void objf_solitary_delete(int64_t handle, const char* dir, const char* ext)
{
    char path[MAX_PATH];
    int length;
    char* dot;
    TigFile* stream;

    strcpy(path, dir);

    if (!sub_4E7050(handle, path)) {
        tig_debug_println("Can't generate filename in ObjFile, objf_solitary_delete");
        return;
    }

    strcat(path, ext);

    length = (int)strlen(path);
    if (length > MAX_PATH) {
        tig_debug_println("Filename too long in ObjFile, objf_solitary_delete.");
        return;
    }

    if (tig_file_exists(path, NULL) && tig_file_remove(path) != 0) {
        tig_debug_printf("Unable to delete [%s] in ObjFile, objf_solitary_delete\n", path);
        return;
    }

    dot = strchr(path, '.');
    if (dot == NULL) {
        tig_debug_printf("Unable to find the dot in [%s] in ObjFile, objf_solitary_delete\n", path);
        return;
    }

    strcpy(dot, ".del");

    stream = tig_file_fopen(path, "wb");
    if (stream == NULL) {
        tig_debug_printf("Unable to open [%s] in ObjFile, objf_solitary_delete\n", path);
        return;
    }

    if (!obj_write_raw(&length, sizeof(length), stream)) {
        tig_debug_println("Write failed in ObjFile, objf_solitary_delete");
    }

    tig_file_fclose(stream);
}

// 0x4E7010
bool obj_write_raw(void* buffer, size_t size, TigFile* stream)
{
    return tig_file_fwrite(buffer, size, 1, stream) == 1;
}

// 0x4E7030
bool obj_read_raw(void* buffer, size_t size, TigFile* stream)
{
    return tig_file_fread(buffer, size, 1, stream) == 1;
}

// 0x4E7050
bool sub_4E7050(int64_t handle, char* path)
{
    size_t length;

    length = strlen(path);
    if (length != 0 && path[length - 1] != '\\') {
        path[length - 1] = '\\';
        path[length] = '\0';
    }

    objid_id_to_str(path, sub_407EF0(handle));

    return true;
}

// 0x4E70C0
TigFile* open_solitary_for_write(int64_t handle, const char* dir, const char* ext)
{
    char path[MAX_PATH];
    int length;
    TigFile* stream;

    strcpy(path, dir);

    if (!sub_4E7050(handle, path)) {
        return NULL;
    }

    strcat(path, ext);

    length = (int)strlen(path);
    if (length > MAX_PATH) {
        tig_debug_println("Filename too long in ObjFile, open_solitary_for_write.");
        return NULL;
    }

    stream = tig_file_fopen(path, "wb");
    if (stream == NULL) {
        tig_debug_printf("Unable to open [%s] in ObjFile, open_solitary_for_write\n", path);
        return NULL;
    }

    return stream;
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
