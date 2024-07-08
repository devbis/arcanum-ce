#ifndef ARCANUM_GAMELIB_OBJECT_NODE_H_
#define ARCANUM_GAMELIB_OBJECT_NODE_H_

#include "game/context.h"

typedef struct ObjectNode {
    int field_0;
    int field_4;
    ObjectNode* next;
    int field_C;
};

// See 0x4E9990.
static_assert(sizeof(ObjectNode) == 0x10, "wrong size");

bool object_node_init(GameContext* ctx);
void object_node_exit();
ObjectNode* object_node_create();
void object_node_destroy(ObjectNode* node);

#endif /* ARCANUM_GAMELIB_OBJECT_NODE_H_ */
