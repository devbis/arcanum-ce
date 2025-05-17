#include "game/obj_private.h"

#include <stdio.h>

#include "game/map.h"
#include "game/obj_file.h"
#include "game/object.h"
#include "game/sector.h"

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

// 0x5B9258
static int dword_5B9258 = 4;

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
        if (!objf_read(a1->ptr, sizeof(int), stream)) {
            return false;
        }
        return true;
    case SA_TYPE_INT64:
        if (!objf_read(&presence, sizeof(presence), stream)) {
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
        if (!objf_read(*(int64_t**)a1->ptr, sizeof(int64_t), stream)) {
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
        if (!objf_read(&presence, sizeof(presence), stream)) {
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
        if (!objf_read(&presence, sizeof(presence), stream)) {
            return false;
        }
        if (*(char**)a1->ptr != NULL) {
            FREE(*(char**)a1->ptr);
        }
        if (!presence) {
            *(char**)a1->ptr = NULL;
            return true;
        }
        if (!objf_read(&size, sizeof(size), stream)) {
            return false;
        }
        *(char**)a1->ptr = (char*)MALLOC(size + 1);
        if (!objf_read(*(char**)a1->ptr, size + 1, stream)) {
            return false;
        }
        return true;
    case SA_TYPE_HANDLE:
        if (!objf_read(&presence, sizeof(presence), stream)) {
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
        if (!objf_read(*(ObjectID**)a1->ptr, sizeof(ObjectID), stream)) {
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
        if (!objf_read(a1->ptr, sizeof(int), stream)) {
            return false;
        }
        return true;
    case SA_TYPE_INT64:
        if (!objf_read(&presence, sizeof(presence), stream)) {
            return false;
        }
        if (!presence) {
                *(int64_t**)a1->ptr = NULL;
            return true;
        }
            *(int64_t**)a1->ptr = (int64_t*)MALLOC(sizeof(int64_t));
        if (!objf_read(*(int64_t**)a1->ptr, sizeof(int64_t), stream)) {
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
        if (!objf_read(&presence, sizeof(presence), stream)) {
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
        if (!objf_read(&presence, sizeof(presence), stream)) {
            return false;
        }
        if (!presence) {
            *(char**)a1->ptr = NULL;
            return true;
        }
        if (!objf_read(&size, sizeof(size), stream)) {
            return false;
        }
        *(char**)a1->ptr = (char*)MALLOC(size + 1);
        if (!objf_read(*(char**)a1->ptr, size + 1, stream)) {
            return false;
        }
        return true;
    case SA_TYPE_HANDLE:
        if (!objf_read(&presence, sizeof(presence), stream)) {
            return false;
        }
        if (!presence) {
                *(ObjectID**)a1->ptr = NULL;
            return true;
        }
            *(ObjectID**)a1->ptr = (ObjectID*)MALLOC(sizeof(ObjectID));
        if (!objf_read(*(ObjectID**)a1->ptr, sizeof(ObjectID), stream)) {
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
        if (!objf_write((int*)a1->ptr, sizeof(int), stream)) return false;
        return true;
    case SA_TYPE_INT64:
        if (*(int64_t**)a1->ptr != NULL) {
            presence = 1;
            if (!objf_write(&presence, sizeof(presence), stream)) return false;
            if (!objf_write(*(int64_t**)a1->ptr, sizeof(int64_t), stream)) return false;
        } else {
            presence = 0;
            if (!objf_write(&presence, sizeof(presence), stream)) return false;
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
            if (!objf_write(&presence, sizeof(presence), stream)) return false;
            if (!sa_write((SizeableArray**)a1->ptr, stream)) return false;
        } else {
            presence = 0;
            if (!objf_write(&presence, sizeof(presence), stream)) return false;
        }
        return true;
    case SA_TYPE_STRING:
        if (*(char**)a1->ptr != NULL) {
            presence = 1;
            if (!objf_write(&presence, sizeof(presence), stream)) return false;
            size = (int)strlen(*(char**)a1->ptr);
            if (!objf_write(&size, sizeof(size), stream)) return false;
            if (!objf_write(*(char**)a1->ptr, size + 1, stream)) return false;
        } else {
            presence = 0;
            if (!objf_write(&presence, sizeof(presence), stream)) return false;
        }
        return true;
    case SA_TYPE_HANDLE:
        if (*(ObjectID**)a1->ptr != NULL) {
            presence = 1;
            if (!objf_write(&presence, sizeof(presence), stream)) return false;
            if (!objf_write(*(ObjectID**)a1->ptr, sizeof(ObjectID), stream)) return false;
        } else {
            presence = 0;
            if (!objf_write(&presence, sizeof(presence), stream)) return false;
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
Object* obj_pool_allocate(int64_t* obj_ptr)
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
Object* obj_pool_lock(int64_t obj)
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
    if (v1 >= dword_603710[a1].field_0) {
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
