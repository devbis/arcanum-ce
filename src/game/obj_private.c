#include "game/obj_private.h"

#include <stdio.h>

#include "game/map.h"
#include "game/object.h"
#include "game/sector.h"

#define ONE_TWO_EIGHT 128

typedef struct FindNode {
    /* 0000 */ unsigned int flags;
    /* 0004 */ int field_4;
    /* 0008 */ int64_t obj;
    /* 0010 */ struct FindNode* prev;
    /* 0014 */ struct FindNode* next;
    /* 0018 */ int64_t sector_id;
} FindNode;

// See 0x4E3BE0.
static_assert(sizeof(FindNode) == 0x20, "wrong size");

typedef struct FindSector {
    /* 0000 */ int64_t sector_id;
    /* 0008 */ FindNode* head;
    /* 000C */ int field_C;
} FindSector;

// See 0x4E3DD0.
static_assert(sizeof(FindSector) == 0x10, "wrong size");

typedef struct S603710 {
    /* 0000 */ int field_0;
    /* 0004 */ int field_4;
} S603710;

typedef struct S6036B8 {
    /* 0000 */ ObjectID field_0;
    /* 0018 */ int64_t field_18;
} S6036B8;

static_assert(sizeof(S6036B8) == 0x20, "wrong size");

typedef struct S603720 {
    uint16_t field_0;
    uint16_t field_2;
} S603720;

static_assert(sizeof(S603720) == 0x4, "wrong size");

static void sub_4E3BE0();
static void sub_4E3C60();
static void obj_find_node_allocate(FindNode** obj_find_node);
static void obj_find_node_deallocate(FindNode* obj_find_node);
static void obj_find_node_attach(FindSector *find_sector, FindNode *find_node);
static void obj_find_node_detach(FindNode *find_node);
static void sub_4E3DD0();
static bool sub_4E3E10(int64_t sector_id, int* index_ptr);
static void sub_4E3E90(int64_t sector_id, FindSector** find_sector_ptr);
static void sub_4E3F40(FindSector* find_sector);
static void sub_4E4C80(S4E4BD0* a1, int size);
static int sub_4E5640();
static void sub_4E56A0(int index);
static bool sub_4E56E0();
static void sub_4E5770(int index);
static bool sub_4E57E0(ObjectID a1, int* index_ptr);
static int64_t sub_4E58C0(int a1, int a2);
static int sub_4E5900(int64_t a1);
static uint8_t* sub_4E5920(int index);
static Object* object_ptr(int index);
static void sub_4E5980(int* a1, int a2);
static int sub_4E59A0(int* a1);
static void sub_4E6040(int a1, int a2);
static void sub_4E6130(int a1, int a2);
static void sub_4E61B0(int start, int end, int inc);
static int sub_4E61E0(int a1);
static int sub_4E61F0(int a1);
static void sub_4E6210();
static void sub_4E6240();
static bool objid_compare(ObjectID a, ObjectID b);
static bool sub_4E67A0(GUID* guid, const char* str);
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
static FindSector* dword_60369C;

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

// 0x6036FC
static uint8_t* dword_6036FC;

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

// 0x603720
static S603720* dword_603720;

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
            FREE(dword_60369C);
        }

        obj_find_initialized = false;
    }
}

// 0x4E3980
void obj_find_add(int64_t obj)
{
    int64_t loc;
    int64_t sector_id;
    FindNode* find_node;
    FindSector* find_sector;

    loc = obj_field_int64_get(obj, OBJ_F_LOCATION);
    sector_id = sector_id_from_loc(loc);
    obj_find_node_allocate(&find_node);
    find_node->obj = obj;
    sub_4E3E90(sector_id, &find_sector);
    obj_find_node_attach(find_sector, find_node);
    obj_field_int32_set(obj, OBJ_F_FIND_NODE, (int)find_node); // TODO: x64
}

// 0x4E39F0
void obj_find_remove(int64_t obj)
{
    FindNode* find_node;

    find_node = (FindNode*)obj_field_int32_get(obj, OBJ_F_FIND_NODE); // TODO: x64
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
    obj_field_int32_set(obj, OBJ_F_FIND_NODE, (int)NULL); // TODO: x64.
}

// 0x4E3A70
void obj_find_move(int64_t obj)
{
    int64_t location;
    int64_t sector_id;
    FindNode* find_node;
    FindSector* find_sector;

    location = obj_field_int64_get(obj, OBJ_F_LOCATION);
    sector_id = sector_id_from_loc(location);
    find_node = (FindNode*)obj_field_int32_get(obj, OBJ_F_FIND_NODE); // TODO: x64
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

    if (find_node->sector_id != sector_id) {
        obj_find_node_detach(find_node);
        sub_4E3E90(sector_id, &find_sector);
        obj_find_node_attach(find_sector, find_node);
    }
}

// 0x4E3B30
bool obj_find_walk_first(int64_t sector_id, int64_t* obj_ptr, FindNode** iter_ptr)
{
    int index;
    FindNode* node;

    if (!sub_4E3E10(sector_id, &index)) {
        *obj_ptr = OBJ_HANDLE_NULL;
        return false;
    }

    node = dword_60369C[index].head;
    if (node == NULL) {
        tig_debug_println("Found empty sector in obj_find_walk_first.");
        *obj_ptr = OBJ_HANDLE_NULL;
        return false;
    }

    *obj_ptr = node->obj;
    *iter_ptr = node->next;

    return true;
}

// 0x4E3BA0
bool obj_find_walk_next(int64_t* obj_ptr, FindNode** iter_ptr)
{
    if (*iter_ptr != NULL) {
        *obj_ptr = (*iter_ptr)->obj;
        *iter_ptr = (*iter_ptr)->next;
        return true;
    } else {
        *obj_ptr = OBJ_HANDLE_NULL;
        return false;
    }
}

// 0x4E3BE0
void sub_4E3BE0()
{
    int index;

    dword_6036A0++;

    if (dword_603694 == NULL) {
        dword_603694 = (FindNode**)MALLOC(sizeof(*dword_603694) * dword_6036A0);
    } else {
        dword_603694 = (FindNode**)REALLOC(dword_603694, sizeof(*dword_603694) * dword_6036A0);
    }

    dword_603694[dword_6036A0 - 1] = (FindNode*)MALLOC(sizeof(FindNode) * ONE_TWO_EIGHT);

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
        tig_message_post_quit(0);
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

// 0x4E3D30
void obj_find_node_attach(FindSector *find_sector, FindNode *find_node)
{
    find_node->prev = NULL;
    find_node->next = find_sector->head;
    if (find_sector->head != NULL) {
        find_sector->head->prev = find_node;
    }
    find_sector->head = find_node;
    find_node->sector_id = find_sector->sector_id;
}

// 0x4E3D60
void obj_find_node_detach(FindNode *find_node)
{
    int index;
    FindSector* find_sector;

    if (!sub_4E3E10(find_node->sector_id, &index)) {
        tig_debug_println("Error: Can't find sector in obj_find_node_detach.");
        return;
    }

    find_sector = &(dword_60369C[index]);

    if (find_node->next != NULL) {
        find_node->next->prev = find_node->prev;
    }

    if (find_node->prev != NULL) {
        find_node->prev->next = find_node->next;
    } else {
        find_sector->head = find_node->next;
        if (find_sector->head == NULL) {
            sub_4E3F40(find_sector);
        }
    }
}

// 0x4E3DD0
void sub_4E3DD0()
{
    dword_603698 += 32;

    if (dword_60369C != NULL) {
        dword_60369C = (FindSector*)REALLOC(dword_60369C, sizeof(*dword_60369C) * dword_603698);
    } else {
        dword_60369C = (FindSector*)MALLOC(sizeof(*dword_60369C) * dword_603698);
    }
}

// 0x4E3E10
bool sub_4E3E10(int64_t sector_id, int* index_ptr)
{
    int l;
    int r;
    int m;

    l = 0;
    r = dword_603690 - 1;
    while (l <= r) {
        m = (l + r) / 2;
        if (dword_60369C[m].sector_id == sector_id) {
            *index_ptr = m;
            return true;
        } else if (dword_60369C[m].sector_id < sector_id) {
            l = m + 1;
        } else {
            r = m - 1;
        }
    }

    *index_ptr = l;
    return false;
}

// 0x4E3E90
void sub_4E3E90(int64_t sector_id, FindSector** find_sector_ptr)
{
    int index;

    if (sub_4E3E10(sector_id, &index)) {
        *find_sector_ptr = &(dword_60369C[index]);
        return;
    }

    if (dword_603690 == dword_603698) {
        sub_4E3DD0();
    }

    if (dword_603690 - index != 0) {
        memcpy(&(dword_60369C[index + 1]),
            &(dword_60369C[index]),
            sizeof(*dword_60369C) * (dword_603690 - index));
    }

    *find_sector_ptr = &(dword_60369C[index]);
    (*find_sector_ptr)->sector_id = sector_id;
    (*find_sector_ptr)->head = NULL;
    dword_603690++;
}

// 0x4E3F40
void sub_4E3F40(FindSector* find_sector)
{
    int index;

    index = find_sector - dword_60369C;

    if (dword_603690 - index != 1) {
        memcpy(&(dword_60369C[index]),
            &(dword_60369C[index + 1]),
            sizeof(*dword_60369C) * (dword_603690 - index - 1));
    }

    dword_603690--;
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

// 0x4E3FA0
void sub_4E3FA0(ObjSa* a1)
{
    switch (a1->type) {
    case SA_TYPE_INT32:
        *(int*)a1->ptr = 0;
        break;
    case SA_TYPE_INT64:
    case SA_TYPE_STRING:
    case SA_TYPE_HANDLE:
        if (*(void**)a1->ptr != NULL) {
            FREE(*(void**)a1->ptr);
            *(void**)a1->ptr = NULL;
        }
        break;
    case SA_TYPE_INT32_ARRAY:
    case SA_TYPE_INT64_ARRAY:
    case SA_TYPE_UINT32_ARRAY:
    case SA_TYPE_UINT64_ARRAY:
    case SA_TYPE_SCRIPT:
    case SA_TYPE_QUEST:
    case SA_TYPE_HANDLE_ARRAY:
        if (*(SizeableArray**)a1->ptr != NULL) {
            sa_deallocate((SizeableArray**)a1->ptr);
        }
        break;
    }
}

// 0x4E4000
void sub_4E4000(ObjSa* a1)
{
    switch (a1->type) {
    case SA_TYPE_INT32:
        *(int*)a1->ptr = a1->storage.value;
        break;
    case SA_TYPE_INT64:
        if (a1->storage.value64 != 0) {
            if (*(int64_t**)a1->ptr == NULL) {
                *(int64_t**)a1->ptr = MALLOC(sizeof(int64_t));
            }
            **(int64_t**)a1->ptr = a1->storage.value64;
        } else {
            if (*(int64_t**)a1->ptr != NULL) {
                FREE(*(int64_t**)a1->ptr);
                *(int64_t**)a1->ptr = NULL;
            }
        }
        break;
    case SA_TYPE_INT32_ARRAY:
    case SA_TYPE_UINT32_ARRAY:
        if (*(SizeableArray**)a1->ptr == NULL) {
            sa_allocate((SizeableArray**)a1->ptr, sizeof(int));
        }
        sa_set((SizeableArray**)a1->ptr, a1->idx, &(a1->storage));
        break;
    case SA_TYPE_INT64_ARRAY:
    case SA_TYPE_UINT64_ARRAY:
        if (*(SizeableArray**)a1->ptr == NULL) {
            sa_allocate((SizeableArray**)a1->ptr, sizeof(int64_t));
        }
        sa_set((SizeableArray**)a1->ptr, a1->idx, &(a1->storage));
        break;
    case SA_TYPE_SCRIPT:
        if (*(SizeableArray**)a1->ptr == NULL) {
            sa_allocate((SizeableArray**)a1->ptr, sizeof(Script));
        }
        sa_set((SizeableArray**)a1->ptr, a1->idx, &(a1->storage));
        break;
    case SA_TYPE_QUEST:
        if (*(SizeableArray**)a1->ptr == NULL) {
            sa_allocate((SizeableArray**)a1->ptr, sizeof(PcQuestState));
        }
        sa_set((SizeableArray**)a1->ptr, a1->idx, &(a1->storage));
        break;
    case SA_TYPE_STRING:
        if (*(char**)a1->ptr != NULL) {
            FREE(*(char**)a1->ptr);
        }
        *(char**)a1->ptr = STRDUP(a1->storage.str);
        break;
    case SA_TYPE_HANDLE:
        if (a1->storage.oid.type != OID_TYPE_NULL) {
            if (*(ObjectID**)a1->ptr == NULL) {
                *(ObjectID**)a1->ptr = MALLOC(sizeof(ObjectID));
            }
            **(ObjectID**)a1->ptr = a1->storage.oid;
            break;
        } else {
            if (*(ObjectID**)a1->ptr != NULL) {
                FREE(*(ObjectID**)a1->ptr);
                *(ObjectID**)a1->ptr = NULL;
            }
        }
        break;
    case SA_TYPE_HANDLE_ARRAY:
        if (*(SizeableArray**)a1->ptr == NULL) {
            sa_allocate((SizeableArray**)a1->ptr, sizeof(ObjectID));
        }
        sa_set((SizeableArray**)a1->ptr, a1->idx, &(a1->storage));
        break;
    }
}

// 0x4E4180
void sub_4E4180(ObjSa* a1)
{
    switch (a1->type) {
    case SA_TYPE_INT32:
        a1->storage.value = *(int*)a1->ptr;
        break;
    case SA_TYPE_INT64:
        if (*(int64_t**)a1->ptr != NULL) {
            a1->storage.value64 = **(int64_t**)a1->ptr;
        } else {
            a1->storage.value64 = 0;
        }
        break;
    case SA_TYPE_INT32_ARRAY:
    case SA_TYPE_UINT32_ARRAY:
        if (*(SizeableArray**)a1->ptr != NULL) {
            sa_get((SizeableArray**)a1->ptr, a1->idx, &(a1->storage));
        } else {
            a1->storage.value = 0;
        }
        break;
    case SA_TYPE_INT64_ARRAY:
    case SA_TYPE_UINT64_ARRAY:
        if (*(SizeableArray**)a1->ptr != NULL) {
            sa_get((SizeableArray**)a1->ptr, a1->idx, &(a1->storage));
        } else {
            a1->storage.value64 = 0;
        }
        break;
    case SA_TYPE_SCRIPT:
        if (*(SizeableArray**)a1->ptr != NULL) {
            sa_get((SizeableArray**)a1->ptr, a1->idx, &(a1->storage));
        } else {
            memset(&(a1->storage.scr), 0, sizeof(a1->storage.scr));
        }
        break;
    case SA_TYPE_QUEST:
        if (*(SizeableArray**)a1->ptr != NULL) {
            sa_get((SizeableArray**)a1->ptr, a1->idx, &(a1->storage));
        } else {
            memset(&(a1->storage.quest), 0, sizeof(a1->storage.quest));
        }
        break;
    case SA_TYPE_STRING:
        if (*(char**)a1->ptr != NULL) {
            a1->storage.str = STRDUP(*(char**)a1->ptr);
        } else {
            a1->storage.str = NULL;
        }
        break;
    case SA_TYPE_HANDLE:
        if (*(ObjectID**)a1->ptr != NULL) {
            a1->storage.oid = **(ObjectID**)a1->ptr;
        } else {
            a1->storage.oid.type = OID_TYPE_NULL;
        }
        break;
    case SA_TYPE_HANDLE_ARRAY:
        if (*(SizeableArray**)a1->ptr != NULL) {
            sa_get((SizeableArray**)a1->ptr, a1->idx, &(a1->storage));
        } else {
            a1->storage.oid.type = OID_TYPE_NULL;
        }
        break;
    }
}

// 0x4E4280
void sub_4E4280(ObjSa* a1, void* value)
{
    switch (a1->type) {
    case SA_TYPE_INT32:
        *(int*)value = *(int*)a1->ptr;
        break;
    case SA_TYPE_INT64:
        if (*(int64_t**)a1->ptr != NULL) {
            *(int64_t**)value = (int64_t*)MALLOC(sizeof(int64_t));
            **(int64_t**)value = **(int64_t**)a1->ptr;
        } else {
            *(int64_t**)value = NULL;
        }
        break;
    case SA_TYPE_INT32_ARRAY:
    case SA_TYPE_INT64_ARRAY:
    case SA_TYPE_UINT32_ARRAY:
    case SA_TYPE_UINT64_ARRAY:
    case SA_TYPE_SCRIPT:
    case SA_TYPE_QUEST:
    case SA_TYPE_HANDLE_ARRAY:
        if (*(SizeableArray**)a1->ptr != NULL) {
            sub_4E74A0((SizeableArray**)value, (SizeableArray**)a1->ptr);
        } else {
            *(SizeableArray**)value = NULL;
        }
        break;
    case SA_TYPE_STRING:
        if (*(char**)a1->ptr != NULL) {
            *(char**)value = STRDUP(*(char**)a1->ptr);
        } else {
            *(char**)value = NULL;
        }
        break;
    case SA_TYPE_HANDLE:
        if (*(ObjectID**)a1->ptr != NULL) {
            *(ObjectID**)value = (ObjectID*)MALLOC(sizeof(ObjectID));
            **(ObjectID**)value = **(ObjectID**)a1->ptr;
        } else {
            *(ObjectID**)value = NULL;
        }
        break;
    }
}

// 0x4E4360
bool sub_4E4360(ObjSa* a1, TigFile* stream)
{
    uint8_t presence;
    int size;

    switch (a1->type) {
    case SA_TYPE_INT32:
        if (!obj_read_raw(a1->ptr, sizeof(int), stream)) {
            return false;
        }
        return true;
    case SA_TYPE_INT64:
        if (!obj_read_raw(&presence, sizeof(presence), stream)) {
            return false;
        }
        if (!presence) {
            if (*(int64_t**)a1->ptr != NULL) {
                FREE(*(int64_t**)a1->ptr);
                *(int64_t**)a1->ptr = NULL;
            }
            return true;
        }
        if (*(int64_t**)a1->ptr == NULL) {
            *(int64_t**)a1->ptr = (int64_t*)MALLOC(sizeof(int64_t));
        }
        if (!obj_read_raw(*(int64_t**)a1->ptr, sizeof(int64_t), stream)) {
            return false;
        }
        return true;
    case SA_TYPE_INT32_ARRAY:
    case SA_TYPE_INT64_ARRAY:
    case SA_TYPE_UINT32_ARRAY:
    case SA_TYPE_UINT64_ARRAY:
    case SA_TYPE_SCRIPT:
    case SA_TYPE_QUEST:
    case SA_TYPE_HANDLE_ARRAY:
        if (!obj_read_raw(&presence, sizeof(presence), stream)) {
            return false;
        }
        if (!presence) {
            *(SizeableArray**)a1->ptr = NULL;
            return true;
        }
        if (!sa_read((SizeableArray**)a1->ptr, stream)) {
            return false;
        }
        return true;
    case SA_TYPE_STRING:
        if (!obj_read_raw(&presence, sizeof(presence), stream)) {
            return false;
        }
        if (*(char**)a1->ptr != NULL) {
            FREE(*(char**)a1->ptr);
        }
        if (!presence) {
            *(char**)a1->ptr = NULL;
            return true;
        }
        if (!obj_read_raw(&size, sizeof(size), stream)) {
            return false;
        }
        *(char**)a1->ptr = (char*)MALLOC(size + 1);
        if (!obj_read_raw(*(char**)a1->ptr, size + 1, stream)) {
            return false;
        }
        return true;
    case SA_TYPE_HANDLE:
        if (!obj_read_raw(&presence, sizeof(presence), stream)) {
            return false;
        }
        if (!presence) {
            if (*(ObjectID**)a1->ptr != NULL) {
                FREE(*(ObjectID**)a1->ptr);
                *(ObjectID**)a1->ptr = NULL;
            }
            return true;
        }
        if (*(ObjectID**)a1->ptr == NULL) {
            *(ObjectID**)a1->ptr = (ObjectID*)MALLOC(sizeof(ObjectID));
        }
        if (!obj_read_raw(*(ObjectID**)a1->ptr, sizeof(ObjectID), stream)) {
            return false;
        }
        return true;
    default:
        return false;
    }
}

// 0x4E44F0
bool sub_4E44F0(ObjSa* a1, TigFile* stream)
{
    uint8_t presence;
    int size;

    switch (a1->type) {
    case SA_TYPE_INT32:
        if (!obj_read_raw(a1->ptr, sizeof(int), stream)) {
            return false;
        }
        return true;
    case SA_TYPE_INT64:
        if (!obj_read_raw(&presence, sizeof(presence), stream)) {
            return false;
        }
        if (!presence) {
            *(int64_t**)a1->ptr = NULL;
            return true;
        }
        *(int64_t**)a1->ptr = (int64_t*)MALLOC(sizeof(int64_t));
        if (!obj_read_raw(*(int64_t**)a1->ptr, sizeof(int64_t), stream)) {
            return false;
        }
        return true;
    case SA_TYPE_INT32_ARRAY:
    case SA_TYPE_INT64_ARRAY:
    case SA_TYPE_UINT32_ARRAY:
    case SA_TYPE_UINT64_ARRAY:
    case SA_TYPE_SCRIPT:
    case SA_TYPE_QUEST:
    case SA_TYPE_HANDLE_ARRAY:
        if (!obj_read_raw(&presence, sizeof(presence), stream)) {
            return false;
        }
        if (!presence) {
            *(SizeableArray**)a1->ptr = NULL;
            return true;
        }
        if (!sa_read_no_dealloc((SizeableArray**)a1->ptr, stream)) {
            return false;
        }
        return true;
    case SA_TYPE_STRING:
        if (!obj_read_raw(&presence, sizeof(presence), stream)) {
            return false;
        }
        if (!presence) {
            *(char**)a1->ptr = NULL;
            return true;
        }
        if (!obj_read_raw(&size, sizeof(size), stream)) {
            return false;
        }
        *(char**)a1->ptr = (char*)MALLOC(size + 1);
        if (!obj_read_raw(*(char**)a1->ptr, size + 1, stream)) {
            return false;
        }
        return true;
    case SA_TYPE_HANDLE:
        if (!obj_read_raw(&presence, sizeof(presence), stream)) {
            return false;
        }
        if (!presence) {
            *(ObjectID**)a1->ptr = NULL;
            return true;
        }
        *(ObjectID**)a1->ptr = (ObjectID*)MALLOC(sizeof(ObjectID));
        if (!obj_read_raw(*(ObjectID**)a1->ptr, sizeof(ObjectID), stream)) {
            return false;
        }
        return true;
    default:
        return false;
    }
}

// 0x4E4660
void sub_4E4660(ObjSa* a1, uint8_t** data)
{
    uint8_t presence;
    int size;

    switch (a1->type) {
    case SA_TYPE_INT32:
        sub_4E4C50(a1->ptr, sizeof(int), data);
        return;
    case SA_TYPE_INT64:
        sub_4E4C50(&presence, sizeof(presence), data);
        if (!presence) {
            if (*(int64_t**)a1->ptr != NULL) {
                FREE(*(int64_t**)a1->ptr);
                *(int64_t**)a1->ptr = NULL;
            }
            return;

        }
        if (*(int64_t**)a1->ptr == NULL) {
            *(int64_t**)a1->ptr = (int64_t*)MALLOC(sizeof(int64_t));
        }
        sub_4E4C50(*(int64_t**)a1->ptr, sizeof(int64_t), data);
        return;
    case SA_TYPE_INT32_ARRAY:
    case SA_TYPE_INT64_ARRAY:
    case SA_TYPE_UINT32_ARRAY:
    case SA_TYPE_UINT64_ARRAY:
    case SA_TYPE_SCRIPT:
    case SA_TYPE_QUEST:
    case SA_TYPE_HANDLE_ARRAY:
        sub_4E4C50(&presence, sizeof(presence), data);
        if (!presence) {
            if (*(SizeableArray**)a1->ptr != NULL) {
                FREE(*(SizeableArray**)a1->ptr);
                *(SizeableArray**)a1->ptr = NULL;
            }
            return;
        }
        sub_4E7820((SizeableArray**)a1->ptr, data);
        return;
    case SA_TYPE_STRING:
        sub_4E4C50(&presence, sizeof(presence), data);
        if (*(char**)a1->ptr != NULL) {
            FREE(*(char**)a1->ptr);
        }
        if (!presence) {
            *(char**)a1->ptr = NULL;
            return;
        }
        sub_4E4C50(&size, sizeof(size), data);
        *(char**)a1->ptr = (char*)MALLOC(size + 1);
        sub_4E4C50(*(char**)a1->ptr, size + 1, data);
        return;
    case SA_TYPE_HANDLE:
        sub_4E4C50(&presence, sizeof(presence), data);
        if (!presence) {
            if (*(ObjectID**)a1->ptr != NULL) {
                FREE(*(ObjectID**)a1->ptr);
                *(ObjectID**)a1->ptr = NULL;
            }
            return;
        }
        if (*(ObjectID**)a1->ptr == NULL) {
            *(ObjectID**)a1->ptr = (ObjectID*)MALLOC(sizeof(ObjectID));
        }
        sub_4E4C50(*(ObjectID**)a1->ptr, sizeof(ObjectID), data);
        return;
    }
}

// 0x4E47E0
bool sub_4E47E0(ObjSa* a1, TigFile* stream)
{
    uint8_t presence;
    int size;

    switch (a1->type) {
    case SA_TYPE_INT32:
        if (!obj_write_raw((int*)a1->ptr, sizeof(int), stream)) return false;
        return true;
    case SA_TYPE_INT64:
        if (*(int64_t**)a1->ptr != NULL) {
            presence = 1;
            if (!obj_write_raw(&presence, sizeof(presence), stream)) return false;
            if (!obj_write_raw(*(int64_t**)a1->ptr, sizeof(int64_t), stream)) return false;
        } else {
            presence = 0;
            if (!obj_write_raw(&presence, sizeof(presence), stream)) return false;
        }
        return true;
    case SA_TYPE_INT32_ARRAY:
    case SA_TYPE_INT64_ARRAY:
    case SA_TYPE_UINT32_ARRAY:
    case SA_TYPE_UINT64_ARRAY:
    case SA_TYPE_SCRIPT:
    case SA_TYPE_QUEST:
    case SA_TYPE_HANDLE_ARRAY:
        if (*(SizeableArray**)a1->ptr != NULL) {
            presence = 1;
            if (!obj_write_raw(&presence, sizeof(presence), stream)) return false;
            if (!sa_write((SizeableArray**)a1->ptr, stream)) return false;
        } else {
            presence = 0;
            if (!obj_write_raw(&presence, sizeof(presence), stream)) return false;
        }
        return true;
    case SA_TYPE_STRING:
        if (*(char**)a1->ptr != NULL) {
            presence = 1;
            if (!obj_write_raw(&presence, sizeof(presence), stream)) return false;
            size = (int)strlen(*(char**)a1->ptr);
            if (!obj_write_raw(&size, sizeof(size), stream)) return false;
            if (!obj_write_raw(*(char**)a1->ptr, size + 1, stream)) return false;
        } else {
            presence = 0;
            if (!obj_write_raw(&presence, sizeof(presence), stream)) return false;
        }
        return true;
    case SA_TYPE_HANDLE:
        if (*(ObjectID**)a1->ptr != NULL) {
            presence = 1;
            if (!obj_write_raw(&presence, sizeof(presence), stream)) return false;
            if (!obj_write_raw(*(ObjectID**)a1->ptr, sizeof(ObjectID), stream)) return false;
        } else {
            presence = 0;
            if (!obj_write_raw(&presence, sizeof(presence), stream)) return false;
        }
        return true;
    default:
        return false;
    }
}

// 0x4E4990
void sub_4E4990(ObjSa* a1, S4E4BD0* a2)
{
    // uint8_t presence;
    // int size;

    // switch (a1->type) {
    // case 3:
    //     sub_4E4C00(a1->d.int_value, sizeof(a1->d.int_value), a2);
    //     break;
    // case 4:
    //     if (*a1->d.b != NULL) {
    //         presence = 1;
    //         sub_4E4C00(&presence, sizeof(presence), a2);
    //         sub_4E4C00(*a1->d.b, sizeof(*a1->d.b), a2);
    //     } else {
    //         presence = 0;
    //         sub_4E4C00(&presence, sizeof(presence), a2);
    //     }
    //     break;
    // case 5:
    // case 6:
    // case 7:
    // case 8:
    // case 9:
    // case 10:
    // case 13:
    //     if (*a1->d.a.sa_ptr != NULL) {
    //         presence = 1;
    //         sub_4E4C00(&presence, sizeof(presence), a2);
    //         size = sub_4E77B0(a1->d.a.sa_ptr);
    //         sub_4E4C80(a2, size);
    //         sub_4E77E0(a1->d.a.sa_ptr, a2->field_4);
    //         a2->field_4 += size;
    //         a2->field_C -= size;
    //     } else {
    //         presence = 0;
    //         sub_4E4C00(&presence, sizeof(presence), a2);
    //     }
    //     break;
    // case 11:
    //     if (*a1->d.str != NULL) {
    //         presence = 1;
    //         sub_4E4C00(&presence, sizeof(presence), a2);
    //         size = strlen(*a1->d.str);
    //         sub_4E4C00(&size, sizeof(size), a2);
    //         sub_4E4C00(*a1->d.str, size + 1, a2);
    //     } else {
    //         presence = 0;
    //         sub_4E4C00(&presence, sizeof(presence), a2);
    //     }
    //     break;
    // case 12:
    //     if (*a1->d.oid != NULL) {
    //         presence = 1;
    //         sub_4E4C00(&presence, sizeof(presence), a2);
    //         sub_4E4C00(*a1->d.oid, sizeof(**a1->d.oid), a2);
    //     } else {
    //         presence = 0;
    //         sub_4E4C00(&presence, sizeof(presence), a2);
    //     }
    //     break;
    // }
}

// 0x4E4B70
void sub_4E4B70(ObjSa* a1)
{
    switch (a1->type) {
    case SA_TYPE_INT32_ARRAY:
    case SA_TYPE_INT64_ARRAY:
    case SA_TYPE_UINT32_ARRAY:
    case SA_TYPE_UINT64_ARRAY:
    case SA_TYPE_SCRIPT:
    case SA_TYPE_QUEST:
    case SA_TYPE_HANDLE_ARRAY:
        if (*(SizeableArray**)a1->ptr != NULL) {
            sub_4E7760((SizeableArray**)a1->ptr, a1->idx);
        }
        break;
    }
}

// 0x4E4BA0
int sub_4E4BA0(ObjSa* a1)
{
    switch (a1->type) {
    case SA_TYPE_INT32_ARRAY:
    case SA_TYPE_INT64_ARRAY:
    case SA_TYPE_UINT32_ARRAY:
    case SA_TYPE_UINT64_ARRAY:
    case SA_TYPE_SCRIPT:
    case SA_TYPE_QUEST:
    case SA_TYPE_HANDLE_ARRAY:
        if (*(SizeableArray**)a1->ptr != NULL) {
            return sa_count((SizeableArray**)a1->ptr);
        }
        break;
    }

    return 0;
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
void sub_4E4C50(void* buffer, int size, uint8_t** data)
{
    memcpy(buffer, *data, size);
    (*data) += size;
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
    (void)obj;
}

// 0x4E4FB0
void sub_4E4FB0(int64_t obj)
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

    if (sub_40FF40() != oid.d.p.map) {
        return OBJ_HANDLE_NULL;
    }

    sub_4407C0(oid.d.p.location, OBJ_TM_TRAP | OBJ_TM_WALL | OBJ_TM_PORTAL | OBJ_TM_SCENERY, &objects);

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
    return false;
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
bool sub_4E53C0(int64_t* obj_ptr, int* iter_ptr)
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
bool sub_4E5420(int64_t* obj_ptr, int* iter_ptr)
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
    dword_6036FC = (uint8_t*)MALLOC(65536);
    dword_603720 = (S603720*)MALLOC(sizeof(*dword_603720) * 33);
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

    for (index = v1->field_4; index < v1->field_0 + v1->field_4; index++) {
        dword_603718[index] = 0;
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
bool sub_4E5DB0(int a1, bool(*callback)(int))
{
    int v1;
    int v2;
    int pos;
    int idx;
    int bit;
    unsigned int flags;

    v1 = dword_603710[a1].field_4;
    v2 = dword_603710[a1].field_0 + v1;

    pos = 0;
    for (idx = v1; idx < v2; idx++) {
        flags = 1;
        for (bit = 0; bit < 32; bit++) {
            if ((flags & dword_603718[idx]) != 0) {
                if (!callback(pos)) {
                    return false;
                }
            }
            flags *= 2;
            pos++;
        }
    }

    return true;
}

// 0x4E5E20
bool sub_4E5E20(int a1, TigFile* stream)
{
    if (tig_file_fwrite(&(dword_603710[a1].field_0), sizeof(int), 1, stream) != 1) {
        return false;
    }

    if (tig_file_fwrite(&(dword_603718[dword_603710[a1].field_4]), sizeof(int) * dword_603710[a1].field_0, 1, stream) != 1) {
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
        return false;
    }

    if (v2 != dword_603710[v1].field_0 && v2 - dword_603710[v1].field_0 > 0) {
        sub_4E6040(v1, v2 - dword_603710[v1].field_0);
    }

    if (tig_file_fread(&(dword_603718[dword_603710[v1].field_4]), 4 * v2, 1, stream) != 1) {
        return false;
    }

    *a1 = v1;

    return true;
}

// 0x4E5F10
int sub_4E5F10(int a1)
{
    return 4 * (dword_603710[a1].field_0 + 1);
}

// 0x4E5F30
void sub_4E5F30(int a1, void* a2)
{
    int v1;
    int v2;

    v1 = dword_603710[a1].field_4;
    v2 = dword_603710[a1].field_0;

    *(int*)a2 = dword_603710[a1].field_0;
    memcpy((int*)a2 + 1, &(dword_603718[v1]), sizeof(int) * v2);
}

// 0x4E5F70
void sub_4E5F70(int* a1, uint8_t** data)
{
    int v1;
    int v2;

    v1 = sub_4E5AA0();

    sub_4E4C50(&v2, sizeof(v2), data);

    if (v2 != dword_603710[v1].field_0) {
        sub_4E6040(v1, v2 - dword_603710[v1].field_0);
    }

    sub_4E4C50(&(dword_603718[dword_603710[v1].field_4]), 4 * v2, data);

    *a1 = v1;
}

// 0x4E5FE0
int sub_4E5FE0(int a1, int a2)
{
    return dword_6036FC[dword_603720[a2].field_0 & (a1 & 0xFFFF)]
        + dword_6036FC[dword_603720[a2].field_2 & ((a1 >> 16) & 0xFFFF)];
}

// 0x4E6040
void sub_4E6040(int a1, int a2)
{
    int* v1;
    int idx;

    if (dword_603708 + a2 > dword_60371C) {
        dword_60371C += ((dword_603708 + a2 - dword_60371C - 1) / 8192 + 1) * 8192;
        dword_603718 = REALLOC(dword_603718, sizeof(*dword_603718) * dword_60371C);
    }

    if (a1 != dword_603724 - 1) {
        v1 = &(dword_603718[dword_603710[a1 + 1].field_4]);
        memcpy(&(v1[a2]), v1, sizeof(*v1) * (dword_603708 - dword_603710[a1 + 1].field_4));
        sub_4E61B0(a1 + 1, dword_603724 - 1, a2);
    }

    dword_603710[a1].field_0 += a2;
    dword_603708 += a2;

    for (idx = dword_603710[a1].field_0 + dword_603710[a1].field_4 - a2; idx < dword_603710[a1].field_0 + dword_603710[a1].field_4; idx++) {
        dword_603718[idx] = 0;
    }
}

// 0x4E6130
void sub_4E6130(int a1, int a2)
{
    int* v1;

    if (a1 != dword_603724 - 1) {
        v1 = &(dword_603718[dword_603710[a1 + 1].field_4]);
        memcpy(&(v1[-a2]), v1, sizeof(*v1) * (dword_603708 - dword_603710[a1 + 1].field_4));
        sub_4E61B0(a1 + 1, dword_603724 - 1, -a2);
    }

    dword_603710[a1].field_0 -= a2;
    dword_603708 -= a2;
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

// 0x4E6210
void sub_4E6210()
{
    int idx;
    uint8_t v1;
    int v2;
    int v3;

    for (idx = 0; idx <= 65535; idx++) {
        v1 = 0;
        v2 = 1;
        for (v3 = 16; v3 != 0; v3--) {
            if ((v2 & idx) != 0) {
                v1++;
            }
            v2 *= 2;
        }
        dword_6036FC[idx] = v1;
    }
}

// 0x4E6240
void sub_4E6240()
{
    int idx;
    int v1 = 1;
    int v2 = 0;

    dword_603720[0].field_0 = 0;
    dword_603720[0].field_2 = 0;

    for (idx = 1; idx <= 16; idx++) {
        v2 += v1;
        v1 *= 2;

        dword_603720[idx].field_0 = v2;
        dword_603720[idx].field_2 = 0;
        dword_603720[idx + 16].field_0 = -1;
        dword_603720[idx + 16].field_2 = v2;
    }
}

// 0x4E62A0
void sub_4E62A0(ObjectID* object_id)
{
    if (CoCreateGuid(&(object_id->d.g)) != S_OK) {
        tig_debug_println("Error: Unable to generate permanent ID!");
        tig_message_post_quit(0);
    }

    object_id->type = OID_TYPE_GUID;
}

// 0x4E62D0
void objid_id_perm_by_load_order(ObjectID* object_id, object_id_t obj)
{
    if (sub_43D990(obj)) {
        object_id->d.p.location = obj_field_int64_get(obj, OBJ_F_LOCATION);
        object_id->d.p.temp_id = obj_field_int32_get(obj, OBJ_F_TEMP_ID);
        object_id->d.p.map = sub_40FF40();
        object_id->type = OID_TYPE_P;
    } else {
        tig_debug_println("Error: Resident object passed to objid_id_perm_by_load_order");
        object_id->type = OID_TYPE_NULL;
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
void objid_id_to_str(char* buffer, ObjectID object_id)
{
    switch (object_id.type) {
    case OID_TYPE_HANDLE:
        sprintf(buffer, "Handle_%I64X", object_id.d.h);
        break;
    case OID_TYPE_BLOCKED:
        strcpy(buffer, "Blocked");
        break;
    case OID_TYPE_NULL:
        strcpy(buffer, "NULL");
        break;
    case OID_TYPE_A:
        sprintf(buffer, "A_%08X", object_id.d.a);
        break;
    case OID_TYPE_GUID:
        sprintf(buffer, "G_%08X_%04X_%04X_%02X%02X_%02X%02X%02X%02X%02X%02X",
            object_id.d.g.Data1,
            object_id.d.g.Data2,
            object_id.d.g.Data3,
            object_id.d.g.Data4[0],
            object_id.d.g.Data4[1],
            object_id.d.g.Data4[2],
            object_id.d.g.Data4[3],
            object_id.d.g.Data4[4],
            object_id.d.g.Data4[5],
            object_id.d.g.Data4[6],
            object_id.d.g.Data4[7]);
        break;
    case OID_TYPE_P:
        sprintf(buffer, "P_%08I64X_%08I64X_%08X_%08X",
            LOCATION_GET_X(object_id.d.p.location),
            LOCATION_GET_Y(object_id.d.p.location),
            object_id.d.p.temp_id,
            object_id.d.p.map);
        break;
    }
}

// 0x4E66B0
bool objid_id_from_str(ObjectID* object_id, const char* str)
{
    ObjectID temp_object_id;

    switch (str[0]) {
    case 'A':
        temp_object_id.type = OID_TYPE_A;
        if (!sub_4E6A60(&(temp_object_id.d.a), str)) {
            return false;
        }
        break;
    case 'B':
        temp_object_id.type = OID_TYPE_BLOCKED;
        break;
    case 'G':
        temp_object_id.type = OID_TYPE_GUID;
        if (!sub_4E67A0(&(temp_object_id.d.g), str)) {
            return false;
        }
        break;
    case 'H':
        tig_debug_println("Handle not handled in objid_id_from_str");
        return false;
    case 'N':
        temp_object_id.type = OID_TYPE_NULL;
        break;
    case 'P':
        temp_object_id.type = OID_TYPE_P;
        if (!sub_4E6970(&(temp_object_id.d.p), str)) {
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

    if (object_id.type <= 0 || object_id.type > 3) {
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
