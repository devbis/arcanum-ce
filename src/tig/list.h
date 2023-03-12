#ifndef ARCANUM_TIG_LIST_H_
#define ARCANUM_TIG_LIST_H_

#include "tig/types.h"

typedef struct TigListNode {
    void* value;
    TigListNode* next;
    int index;
};

static_assert(sizeof(TigListNode) == 0xC, "wrong size");

typedef struct TigList {
    int buckets_count;
    TigListNode** buckets;
    size_t value_size;
    int count;
};

static_assert(sizeof(TigList) == 0x10, "wrong size");

typedef bool(TigListEnumerateCallback)(void* value, int index, void* context);

void tig_list_init(TigList* list, size_t value_size);
void tig_list_exit(TigList* list);
bool tig_list_enumerate(TigList* list, TigListEnumerateCallback* callback, void* context);
void tig_list_set_value(TigList* list, int index, void* value);
bool tig_list_get_value(TigList* list, int index, void* value);
bool tig_list_has_value(TigList* list, int index);
void tig_list_remove(TigList* list, int index);
int tig_list_get_count(TigList* list);

#endif /* ARCANUM_TIG_LIST_H_ */
