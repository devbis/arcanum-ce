#include "game/lib/object_node.h"

#define OBJECT_NODE_LIST_GROW_SIZE 64

static void object_node_reserve();
static void object_node_remove_all();

// 0x603AC0
static ObjectNode* object_node_head;

// 0x4E9920
bool object_node_init(GameContext* ctx)
{
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
    ObjectNode* node = object_node_head;
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
static void object_node_reserve()
{
    if (object_node_head != NULL) {
        for (int index = 0; index < OBJECT_NODE_LIST_GROW_SIZE; index++) {
            ObjectNode* node = (ObjectNode*)malloc(sizeof(*node));
            node->next = object_node_head;
            object_node_head = node;
        }
    }
}

// 0x4E99C0
static void object_node_remove_all()
{
    ObjectNode* curr = object_node_head;
    while (curr != NULL) {
        ObjectNode* next = curr->next;
        free(curr);
        curr = next;
    }
    object_node_head = NULL;
}
