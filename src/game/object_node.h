#ifndef ARCANUM_GAME_OBJECT_NODE_H_
#define ARCANUM_GAME_OBJECT_NODE_H_

#include "game/context.h"

typedef struct ObjectNode {
    int64_t obj;
    ObjectNode* next;
    int field_C;
} ObjectNode;

// See 0x4E9990.
static_assert(sizeof(ObjectNode) == 0x10, "wrong size");

bool object_node_init(GameInitInfo* init_info);
void object_node_exit();
ObjectNode* object_node_create();
void object_node_destroy(ObjectNode* node);

#endif /* ARCANUM_GAME_OBJECT_NODE_H_ */
