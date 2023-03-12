#include "tig/list.h"

#define TIG_LIST_BUCKET_COUNT 63

static void tig_list_node_destroy(TigListNode* entry);
static TigListNode* tig_list_node_create(size_t size);
static TigListNode* tig_list_node_copy(TigListNode* src, size_t size);

// 0x534320
void tig_list_init(TigList* list, size_t value_size)
{
    list->buckets_count = TIG_LIST_BUCKET_COUNT;
    list->buckets = (TigListNode**)malloc(sizeof(list->buckets) * TIG_LIST_BUCKET_COUNT);

    for (int index = 0; index < list->buckets_count; index++) {
        list->buckets[index] = NULL;
    }

    list->count = 0;
    list->value_size = value_size;
}

// 0x534370
void tig_list_exit(TigList* list)
{
    for (int index = 0; index < list->buckets_count; index++) {
        // NOTE: Original code is slightly different.
        TigListNode* node = list->buckets[index];
        while (node != NULL) {
            TigListNode* next = node->next;
            tig_list_node_destroy(node);
            node = next;
        }
    }
    free(list->buckets);
}

// 0x534650
bool tig_list_enumerate(TigList* list, TigListEnumerateCallback* callback, void* context)
{
    for (int index = 0; index < list->buckets_count; index++) {
        TigListNode* node = list->buckets[index];
        while (node != NULL) {
            if (!callback(node->value, node->index, context)) {
                return false;
            }
            node = node->next;
        }
    }

    return true;
}

// 0x5346A0
void tig_list_set_value(TigList* list, int index, void* value)
{
    if (index < 0) {
        index = -index;
    }

    int bucket = index % list->buckets_count;
    TigListNode* node = list->buckets[bucket];
    while (node != NULL) {
        if (node->index == index) {
            memcpy(node->value, value, list->value_size);
            return;
        }
        node = node->next;
    }

    TigListNode* new_node = tig_list_node_create(list->value_size);
    memcpy(new_node->value, value, list->value_size);
    new_node->index = index;
    new_node->next = list->buckets[bucket];

    list->count++;
}

// 0x534730
bool tig_list_get_value(TigList* list, int index, void* value)
{
    if (index < 0) {
        index = -index;
    }

    int bucket = index % list->buckets_count;
    TigListNode* node = list->buckets[bucket];
    while (node != NULL) {
        if (node->index == index) {
            memcpy(value, node->value, list->value_size);
            return true;
        }
        node = node->next;
    }

    return false;
}

// 0x534780
bool tig_list_has_value(TigList* list, int index)
{
    if (index < 0) {
        index = -index;
    }

    int bucket = index % list->buckets_count;
    TigListNode* node = list->buckets[bucket];
    while (node != NULL) {
        if (node->index == index) {
            return true;
        }
        node = node->next;
    }

    return false;
}

// 0x5347C0
void tig_list_remove(TigList* list, int index)
{
    if (index < 0) {
        index = -index;
    }

    int bucket = index % list->buckets_count;
    TigListNode* node = list->buckets[bucket];

    // TODO: Incomplete.
}

// 0x534840
void tig_list_node_destroy(TigListNode* node)
{
    free(node->value);
    free(node);
}

// 0x534860
TigListNode* tig_list_node_create(size_t size)
{
    TigListNode* node = (TigListNode*)malloc(sizeof(*node));
    node->value = malloc(size);
    return node;
}

// 0x534880
TigListNode* tig_list_node_copy(TigListNode* src, size_t size)
{
    if (src == NULL) {
        return NULL;
    }

    TigListNode* copy = tig_list_node_create(size);
    memcpy(copy->value, src->value, size);
    copy->next = tig_list_node_copy(src->next, size);
    return copy;
}

// 0x534990
int tig_list_get_count(TigList* list)
{
    return list->count;
}
