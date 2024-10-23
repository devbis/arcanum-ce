#include "game/tile_script_list.h"

static void sub_4F6380();

// 0x603DD0
TileScriptListNode* tile_script_node_head;

// 0x4F6310
void sub_4F6310()
{
}

// 0x4F6330
TileScriptListNode* tile_script_node_create()
{
    TileScriptListNode* node;

    if (tile_script_node_head == NULL) {
        sub_4F6380();
    }

    node = tile_script_node_head;
    tile_script_node_head = node->next;

    node->next = NULL;

    return node;
}

// 0x4F6360
void tile_script_node_destroy(TileScriptListNode* node)
{
    node->next = tile_script_node_head;
    tile_script_node_head = node;
}

// 0x4F6380
void sub_4F6380()
{
    int index;
    TileScriptListNode* node;

    if (tile_script_node_head == NULL) {
        for (index = 0; index < 8; index++) {
            node = (TileScriptListNode*)MALLOC(sizeof(*node));
            node->next = tile_script_node_head;
            tile_script_node_head = node;
        }
    }
}

// 0x4F63E0
bool tile_script_list_init(TileScriptList* list)
{
    list->flags = 0;
    list->head = 0;

    return true;
}

// 0x4F6400
bool tile_script_list_reset(TileScriptList* list)
{
    TileScriptListNode* node;
    TileScriptListNode* next;

    node = list->head;
    while (node != NULL) {
        next = node->next;
        tile_script_node_destroy(node);
        node = next;
    }

    list->flags = 0;
    list->head = NULL;

    return true;
}

// 0x4F6460
bool tile_script_list_exit(TileScriptList* list)
{
    return tile_script_list_reset(list);
}

// 0x4F6470
bool sub_4F6470(TileScriptList* list, unsigned int id, Script* scr)
{
    TileScriptListNode* prev;
    TileScriptListNode* node;
    TileScriptListNode* new_node;

    prev = NULL;
    node = list->head;
    while (node != NULL) {
        if (node->id == id) {
            node->flags |= 0x1;
            node->scr = *scr;

            list->flags |= 0x1;

            return true;
        }

        if (node->id > id) {
            break;
        }

        prev = node;
        node = node->next;
    }

    new_node = tile_script_node_create();
    new_node->flags = 0x1;
    new_node->id = id;
    new_node->scr = *scr;
    new_node->next = node;

    if (prev != NULL) {
        prev->next = new_node;
    } else {
        list->head = new_node;
    }

    list->flags |= 0x1;

    return true;
}

// 0x4F6520
bool sub_4F6520(TileScriptList* list, unsigned int id)
{
    TileScriptListNode* prev;
    TileScriptListNode* node;

    prev = NULL;
    node = list->head;
    while (node != NULL) {
        if (node->id == id) {
            break;
        }
        prev = node;
        node = node->next;
    }

    if (node == NULL) {
        return false;
    }

    if (prev != NULL) {
        prev->next = node->next;
    } else {
        list->head = node->next;
    }

    tile_script_node_destroy(node);
    list->flags |= 0x1;

    return true;
}

// 0x4F6570
bool sub_4F6570(TileScriptList* list, unsigned int id, Script* scr)
{
    TileScriptListNode* node;

    node = list->head;
    while (node != NULL) {
        if (node->id == id) {
            *scr = node->scr;
            return true;
        }
    }

    return false;
}

// 0x4F65B0
bool tile_script_list_load(TileScriptList* list, TigFile* stream)
{
    int cnt;
    int index;
    TileScriptListNode* prev;
    TileScriptListNode* node;

    prev = NULL;
    cnt = 0;
    if (tig_file_fread(&cnt, sizeof(cnt), 1, stream) != 1) {
        return false;
    }

    for (index = 0; index < cnt; index++) {
        node = tile_script_node_create();
        if (tig_file_fread(node, sizeof(*node), 1, stream) != 1) {
            return false;
        }

        if (prev != NULL) {
            prev->next = node;
        } else {
            list->head = node;
        }
        prev = node;
    }

    return true;
}

// 0x4F6630
bool tile_script_list_save(TileScriptList* list, TigFile* stream)
{
    int cnt;
    TileScriptListNode* node;

    cnt = 0;
    node = list->head;
    while (node != NULL) {
        cnt++;
        node = node->next;
    }

    if (tig_file_fwrite(&cnt, sizeof(cnt), 1, stream) != 1) {
        return false;
    }

    node = list->head;
    while (node != NULL) {
        if (tig_file_fwrite(node, sizeof(*node), 1, stream) != 1) {
            return false;
        }
        node = node->next;
    }

    list->flags &= ~0x1;

    return true;
}

// 0x4F66C0
bool sub_4F66C0(TileScriptList* list)
{
    return (list->flags & 0x1) != 0;
}

// 0x4F66D0
bool tile_script_list_load_with_dif(TileScriptList* list, TigFile* stream)
{
    int cnt;
    int index;
    TileScriptListNode node;

    if (tig_file_fread(&cnt, sizeof(cnt), 1, stream) != 1) {
        return false;
    }

    for (index = 0; index < cnt; index++) {
        if (tig_file_fread(&node, sizeof(node), 1, stream) != 1) {
            return false;
        }

        sub_4F6470(list, node.id, &(node.scr));
    }

    return true;
}

// 0x4F6750
bool tile_script_list_save_with_dif(TileScriptList* list, TigFile* stream)
{
    int cnt;
    TileScriptListNode* node;

    cnt = 0;
    node = list->head;
    while (node != NULL) {
        if ((node->flags & 0x1) != 0) {
            cnt++;
        }
        node = node->next;
    }

    if (tig_file_fwrite(&cnt, sizeof(cnt), 1, stream) != 1) {
        return false;
    }

    node = list->head;
    while (node != NULL) {
        if ((node->flags & 0x1) != 0) {
            if (tig_file_fwrite(node, sizeof(*node), 1, stream) != 1) {
                return false;
            }
        }
        node = node->next;
    }

    return true;
}
