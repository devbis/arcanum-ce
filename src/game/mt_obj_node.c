#include "game/mt_obj_node.h"

#include "game/obj.h"

#define GROW 64

static void mt_obj_node_reserve();
static void mt_obj_node_clear();
static bool mt_obj_node_save_list_node(MagicTechObjectNode* node, TigFile* stream);
static bool mt_obj_node_load_list_node(MagicTechObjectNode* node, TigFile* stream);

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
    mt_obj_node_clear();
}

// 0x4BAC30
MagicTechObjectNode* mt_obj_node_create()
{
    MagicTechObjectNode* node;

    if (mt_obj_node_head == NULL) {
        mt_obj_node_reserve();
    }

    node = mt_obj_node_head;
    mt_obj_node_head = node->next;

    node->next = NULL;
    node->obj = OBJ_HANDLE_NULL;
    node->type = -1;
    node->aptitude = 0;
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
void mt_obj_node_reserve()
{
    int index;
    MagicTechObjectNode* node;

    if (mt_obj_node_head == NULL) {
        for (index = 0; index < GROW; index++) {
            node = (MagicTechObjectNode*)MALLOC(sizeof(*node));
            node->next = mt_obj_node_head;
            mt_obj_node_head = node;
        }
    }
}

// 0x4BACD0
void mt_obj_node_clear()
{
    MagicTechObjectNode* next;

    while (mt_obj_node_head != NULL) {
        next = mt_obj_node_head->next;
        FREE(mt_obj_node_head);
        mt_obj_node_head = next;
    }
}

// 0x4BAD00
bool mt_obj_node_save_list(MagicTechObjectNode** head_ptr, TigFile* stream)
{
    int cnt = 0;
    MagicTechObjectNode* node;

    if (head_ptr == NULL) {
        return false;
    }

    node = *head_ptr;
    while (node != NULL) {
        cnt++;
        node = node->next;
    }

    if (tig_file_fwrite(&cnt, sizeof(cnt), 1, stream) != 1) {
        return false;
    }

    node = *head_ptr;
    while (node != NULL) {
        if (!mt_obj_node_save_list_node(node, stream)) {
            return false;
        }
        node = node->next;
    }

    return true;
}

// 0x4BAD70
bool mt_obj_node_save_list_node(MagicTechObjectNode* node, TigFile* stream)
{
    if (node == NULL) {
        return false;
    }

    if (!object_save_obj_handle_safe(&(node->obj), &(node->field_8), stream)) {
        return false;
    }

    if (tig_file_fwrite(&(node->aptitude), sizeof(node->aptitude), 1, stream) != 1) {
        return false;
    }

    if (tig_file_fwrite(&(node->field_30), sizeof(node->field_30), 1, stream) != 1) {
        return false;
    }

    return true;
}

// 0x4BADD0
bool mt_obj_node_load_list(MagicTechObjectNode** head_ptr, TigFile* stream)
{
    int cnt = 0;
    int index;
    MagicTechObjectNode* node;

    if (head_ptr == NULL) {
        return false;
    }

    if (tig_file_fread(&cnt, sizeof(cnt), 1, stream) != 1) {
        return false;
    }

    for (index = 0; index < cnt; index++) {
        node = *head_ptr = mt_obj_node_create();
        if (!mt_obj_node_load_list_node(node, stream)) {
            return false;
        }
        head_ptr = &(node->next);
        node->next = NULL;
    }

    return true;
}

// 0x4BAE50
bool mt_obj_node_load_list_node(MagicTechObjectNode* node, TigFile* stream)
{
    if (node == NULL) {
        return false;
    }

    if (!object_load_obj_handle_safe(&(node->obj), &(node->field_8), stream)) {
        return false;
    }

    if (tig_file_fread(&(node->aptitude), sizeof(node->aptitude), 1, stream) != 1) {
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
bool mt_obj_node_save_detached(MagicTechObjectNode* node, TigFile* stream)
{
    if (node == NULL) {
        return false;
    }

    if (!object_save_obj_handle_safe(&(node->obj), &(node->field_8), stream)) {
        return false;
    }

    if (tig_file_fwrite(&(node->aptitude), sizeof(node->aptitude), 1, stream) != 1) {
        return false;
    }

    if (tig_file_fwrite(&(node->field_30), sizeof(node->field_30), 1, stream) != 1) {
        return false;
    }

    if (tig_file_fwrite(&(node->loc), sizeof(node->loc), 1, stream) != 1) {
        return false;
    }

    return true;
}

// 0x4BAF50
bool mt_obj_node_load_detached(MagicTechObjectNode* node, TigFile* stream)
{
    if (node == NULL) {
        return false;
    }

    if (!object_load_obj_handle_safe(&(node->obj), &(node->field_8), stream)) {
        return false;
    }

    if (tig_file_fread(&(node->aptitude), sizeof(node->aptitude), 1, stream) != 1) {
        return false;
    }

    if (tig_file_fread(&(node->field_30), sizeof(node->field_30), 1, stream) != 1) {
        return false;
    }

    if (tig_file_fread(&(node->loc), sizeof(node->loc), 1, stream) != 1) {
        return false;
    }

    if (node->obj != OBJ_HANDLE_NULL) {
        node->type = obj_field_int32_get(node->obj, OBJ_F_TYPE);
    } else {
        node->type = -1;
    }

    return true;
}
