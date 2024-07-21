#include "game/object_node.h"

#define OBJECT_NODE_LIST_GROW_SIZE 64

static void object_node_reserve();
static void object_node_remove_all();

// 0x603AC0
static ObjectNode* object_node_head;

// 0x4E9920
bool object_node_init(GameContext* init_info)
{
    (void)init_info;

    return true;
}

// 0x4E9930
void object_node_exit()
{
    object_node_remove_all();
}

// 0x4E9940
ObjectNode* object_node_create()
{
    ObjectNode* node;

    node = object_node_head;
    if (node == NULL) {
        object_node_reserve();
        node = object_node_head;
    }

    object_node_head = node->next;
    node->next = NULL;

    return node;
}

// 0x4E9970
void object_node_destroy(ObjectNode* node)
{
    node->next = object_node_head;
    object_node_head = node;
}

// 0x4E9990
void object_node_reserve()
{
    int index;
    ObjectNode* node;

    if (object_node_head != NULL) {
        for (index = 0; index < OBJECT_NODE_LIST_GROW_SIZE; index++) {
            node = (ObjectNode*)MALLOC(sizeof(*node));
            node->next = object_node_head;
            object_node_head = node;
        }
    }
}

// 0x4E99C0
void object_node_remove_all()
{
    ObjectNode* next;

    while (object_node_head != NULL) {
        next = object_node_head->next;
        FREE(object_node_head);
        object_node_head = next;
    }
}
