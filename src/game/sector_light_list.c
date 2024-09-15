#include "game/sector_light_list.h"

#include "game/light.h"

static void sub_4F7B40(SectorLightList* list, SectorBlockListNode* node);
static bool sub_4F7B50(SectorLightList* list, int value, SectorBlockListNode** node_ptr);

// 0x4F7110
bool sector_light_list_init(SectorLightList* list)
{
    list->head = NULL;
    list->field_4 = 0;

    return true;
}

// 0x4F7130
bool sector_light_list_reset(SectorLightList* list)
{
    SectorBlockListNode* node;
    SectorBlockListNode* next;

    node = list->head;
    while (node != NULL) {
        next = node->next;

        if (sub_4DD110(node->field_0)) {
            sub_4D9570(node->field_0);
        } else {
            sub_4D8620(node->field_0);
        }
        sector_block_list_node_create(node);

        node = next;
    }

    list->head = NULL;
    list->field_4 = 0;

    return true;
}

// 0x4F71B0
bool sector_light_list_exit(SectorLightList* list)
{
    sector_light_list_reset(list);
}

// 0x4F71C0
bool sub_4F71C0(SectorLightList* list, int a2)
{
    SectorBlockListNode* node;

    node = sector_block_list_node_create();
    node->field_0 = a2;
    sub_4F7B40(list, node);

    if (sub_4DD110(a2)) {
        list->field_4 = 1;
    }

    sub_4D8590(a2);

    return true;
}

// 0x4F7200
void sub_4F7200(SectorLightList* list, int a2)
{
    SectorBlockListNode* node;

    if (!sub_4F7B50(list, a2, &node)) {
        return false;
    }

    sector_block_list_node_destroy(node);

    if (sub_4DD110(a2)) {
        list->field_4 = 1;
    }

    return true;
}

// 0x4F7250
bool sub_4F7250(SectorLightList* list, int a2, int a3, int a4, int a5, int a6)
{
    SectorBlockListNode* node;

    if (!sub_4F7B50(list, a2, &node)) {
        return false;
    }

    sub_4DDA40(a2, a3, a4);
    sub_4DDA70(a2, a5, a6);
    sub_4F7B40(list, node);

    if (sub_4DD110(a2)) {
        list->field_4 = 1;
    }

    return true;
}

// 0x4F72C0
void sector_light_list_load()
{
    // TODO: Incomplete.
}

// 0x4F7440
void sector_light_list_load_with_dif()
{
    // TODO: Incomplete.
}

// 0x4F7610
bool sector_list_list_save(SectorLightList* list, TigFile* stream)
{
    int cnt;
    int pos_cnt;
    int pos_curr;
    SectorBlockListNode* node;

    pos_cnt = tig_file_ftell(stream);
    if (pos_cnt == -1) {
        return false;
    }

    cnt = 0;
    if (tig_file_fwrite(&cnt, sizeof(cnt), 1, stream) != 1) {
        return false;
    }

    node = list->head;
    while (node != NULL) {
        if (sub_4DD110(node->field_0)) {
            sub_4D94C0(node->field_0);
            if (!sub_4DDD70(stream, node->field_0)) {
                return false;
            }
            cnt++;
        }
        node = node->next;
    }

    pos_curr = tig_file_ftell(stream);
    if (pos_curr == -1) {
        return false;
    }

    if (tig_file_fseek(stream, pos_cnt, SEEK_SET) != 0) {
        return false;
    }

    if (tig_file_fwrite(&cnt, sizeof(cnt), 1, stream) != 1) {
        return false;
    }

    if (tig_file_fseek(stream, pos_curr, SEEK_SET) != 0) {
        return false;
    }

    list->field_4 = 0;

    return true;
}

// 0x4F7710
bool sub_4F7710(SectorLightList* list)
{
    SectorBlockListNode* node;

    if (list->field_4) {
        return true;
    }

    node = list->head;
    while (node != NULL) {
        if (sub_4DD110(node->field_0)
            && sub_4D94B0(node->field_0)) {
            return true;
        }
        node = node->next;
    }

    return false;
}

// 0x4F7760
void sector_light_list_save_with_dif()
{
    // TODO: Incomplete.
}

// 0x4F7940
void sub_4F7940()
{
    // TODO: Incomplete.
}

// 0x4F7B40
void sub_4F7B40(SectorLightList* list, SectorBlockListNode* node)
{
    node->next = list->head;
    list->head = node;
}

// 0x4F7B50
bool sub_4F7B50(SectorLightList* list, int value, SectorBlockListNode** node_ptr)
{
    SectorBlockListNode* node;
    SectorBlockListNode* prev;

    prev = list->head;
    node = list->head;
    while (node != NULL) {
        if (node->field_0 == value) {
            if (prev == list->head) {
                list->head = node->next;
            } else {
                prev->next = node->next;
            }
            *node_ptr = node;
            node->next = NULL;
            return true;
        }
        prev = node;
        node = node->next;
    }

    return false;
}
