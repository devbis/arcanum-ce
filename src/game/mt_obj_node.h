#ifndef ARCANUM_GAME_MT_OBJ_NODE_H_
#define ARCANUM_GAME_MT_OBJ_NODE_H_

#include "game/context.h"

typedef struct MagicTechObjectNode {
    /* 0000 */ int64_t obj;
    /* 0008 */ int field_8;
    /* 000C */ int field_C;
    /* 0010 */ int field_10;
    /* 0014 */ int field_14;
    /* 0018 */ int field_18;
    /* 001C */ int field_1C;
    /* 0020 */ int field_20;
    /* 0024 */ int field_24;
    /* 0028 */ int field_28;
    /* 002C */ int field_2C;
    /* 0030 */ int field_30;
    /* 0034 */ int field_34;
    /* 0038 */ int type;
    /* 003C */ int field_3C;
    /* 0040 */ struct MagicTechObjectNode* next;
    /* 0044 */ int field_44;
} MagicTechObjectNode;

// See 0x4BACB1
static_assert(sizeof(MagicTechObjectNode) == 0x48, "wrong size");

bool mt_obj_node_init(GameInitInfo* init_info);
void mt_obj_node_exit();
MagicTechObjectNode* mt_obj_node_create();
void mt_obj_node_destroy(MagicTechObjectNode* node);
bool mt_obj_node_save(MagicTechObjectNode** node_ptr, TigFile* stream);
bool mt_obj_node_load(MagicTechObjectNode** node_ptr, TigFile* stream);
bool sub_4BAEE0(MagicTechObjectNode* node, TigFile* stream);
bool sub_4BAF50(MagicTechObjectNode* node, TigFile* stream);

#endif /* ARCANUM_GAME_MT_OBJ_NODE_H_ */
