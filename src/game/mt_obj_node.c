#include "game/mt_obj_node.h"

#include "game/obj.h"

static void sub_4BACA0();
static void sub_4BACD0();
static bool sub_4BAD70(MagicTechObjectNode* node, TigFile* stream);

// 0x5FC368
static MagicTechObjectNode* mt_obj_node_head;

// 0x4BAC10
bool mt_obj_node_init(GameInitInfo* init_info)
{
    (void)init_info;

    return true;
}

// 0x4BAC20
void mt_obj_node_exit()
{
    sub_4BACD0();
}

// 0x4BAC30
MagicTechObjectNode* mt_obj_node_create()
{
    MagicTechObjectNode* node;

    if (mt_obj_node_head == NULL) {
        sub_4BACA0();
    }

    node = mt_obj_node_head;
    mt_obj_node_head = node->next;

    node->next = NULL;
    node->obj = OBJ_HANDLE_NULL;
    node->type = -1;
    node->field_34 = 0;
    node->field_30 = 0;

    return node;
}

// 0x4BAC80
void mt_obj_node_destroy(MagicTechObjectNode* node)
{
    node->next = mt_obj_node_head;
    mt_obj_node_head = node;
}

// 0x4BACA0
void sub_4BACA0()
{
    int index;
    MagicTechObjectNode* node;

    if (mt_obj_node_head == NULL) {
        for (index = 0; index < 64; index++) {
            node = (MagicTechObjectNode*)MALLOC(sizeof(*node));
            node->next = mt_obj_node_head;
            mt_obj_node_head = node;
        }
    }
}

// 0x4BACD0
void sub_4BACD0()
{
    MagicTechObjectNode* next;

    while (mt_obj_node_head != NULL) {
        next = mt_obj_node_head->next;
        FREE(mt_obj_node_head);
        mt_obj_node_head = next;
    }
}

// 0x4BAD00
bool mt_obj_node_save(MagicTechObjectNode** node_ptr, TigFile* stream)
{
    int cnt = 0;
    MagicTechObjectNode* node;

    if (node_ptr == NULL) {
        return false;
    }

    node = *node_ptr;
    while (node != NULL) {
        cnt++;
        node = node->next;
    }

    if (tig_file_fwrite(&cnt, sizeof(cnt), 1, stream) != 1) {
        return false;
    }

    node = *node_ptr;
    while (node != NULL) {
        if (!sub_4BAD70(node, stream)) {
            return false;
        }
        node = node->next;
    }

    return true;
}

// 0x4BAD70
bool sub_4BAD70(MagicTechObjectNode* node, TigFile* stream)
{
    if (node == NULL) {
        return false;
    }

    if (!sub_4439D0(&(node->obj), &(node->field_8), stream)) {
        return false;
    }

    if (tig_file_fwrite(&(node->field_34), sizeof(node->field_34), 1, stream) != 1) {
        return false;
    }

    if (tig_file_fwrite(&(node->field_30), sizeof(node->field_30), 1, stream) != 1) {
        return false;
    }

    return true;
}

// 0x4BADD0
bool mt_obj_node_load(MagicTechObjectNode** node_ptr, TigFile* stream)
{
    int cnt = 0;
    int index;
    MagicTechObjectNode* node;

    if (node_ptr == NULL) {
        return false;
    }

    if (tig_file_fread(&cnt, sizeof(cnt), 1, stream) != 1) {
        return false;
    }

    for (index = 0; index < cnt; index++) {
        node = *node_ptr = mt_obj_node_create();
        if (!sub_4BAE50(node, stream)) {
            return false;
        }
        node_ptr = &(node->next);
        node->next = NULL;
    }

    return true;
}

// 0x4BAE50
bool sub_4BAE50(MagicTechObjectNode* node, TigFile* stream)
{
    if (node == NULL) {
        return false;
    }

    if (!sub_443AD0(&(node->obj), &(node->field_8), stream)) {
        return false;
    }

    if (tig_file_fread(&(node->field_34), sizeof(node->field_34), 1, stream) != 1) {
        return false;
    }

    if (tig_file_fread(&(node->field_30), sizeof(node->field_30), 1, stream) != 1) {
        return false;
    }

    if (node->obj != OBJ_HANDLE_NULL) {
        node->type = obj_field_int32_get(node->obj, OBJ_F_TYPE);
    } else {
        node->type = -1;
    }

    return true;
}

// 0x4BAEE0
bool sub_4BAEE0(MagicTechObjectNode* node, TigFile* stream)
{
    if (node == NULL) {
        return false;
    }

    if (!sub_4439D0(&(node->obj), &(node->field_8), stream)) {
        return false;
    }

    if (tig_file_fwrite(&(node->field_34), sizeof(node->field_34), 1, stream) != 1) {
        return false;
    }

    if (tig_file_fwrite(&(node->field_30), sizeof(node->field_30), 1, stream) != 1) {
        return false;
    }

    // TODO: Probably wrong.
    if (tig_file_fwrite(&(node->next), 8, 1, stream) != 1) {
        return false;
    }

    return true;
}

// 0x4BAF50
bool sub_4BAF50(MagicTechObjectNode* node, TigFile* stream)
{
    if (node == NULL) {
        return false;
    }

    if (!sub_443AD0(&(node->obj), &(node->field_8), stream)) {
        return false;
    }

    if (tig_file_fread(&(node->field_34), sizeof(node->field_34), 1, stream) != 1) {
        return false;
    }

    if (tig_file_fread(&(node->field_30), sizeof(node->field_30), 1, stream) != 1) {
        return false;
    }

    // TODO: Probably wrong.
    if (tig_file_fread(&(node->next), 8, 1, stream) != 1) {
        return false;
    }

    if (node->obj != OBJ_HANDLE_NULL) {
        node->type = obj_field_int32_get(node->obj, OBJ_F_TYPE);
    } else {
        node->type = -1;
    }

    return true;
}
