#include "game/obj_find.h"

#include "game/obj.h"
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
