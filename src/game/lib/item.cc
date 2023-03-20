#include "game/lib/item.h"

#include "game/lib/message.h"
#include "game/lib/random.h"
#include "game/lib/stat.h"
#include "tig/art.h"
#include "tig/debug.h"
#include "tig/window.h"

#define FIRST_AMMUNITION_TYPE_ID 0
#define FIRST_ARMOR_COVERAGE_TYPE_ID (FIRST_AMMUNITION_TYPE_ID + AMMUNITION_TYPE_COUNT)

// 0x5B32A0
static int dword_5B32A0[AMMUNITION_TYPE_COUNT] = {
    OBJ_F_CRITTER_ARROWS,
    OBJ_F_CRITTER_BULLETS,
    OBJ_F_CRITTER_POWER_CELLS,
    OBJ_F_CRITTER_FUEL,
};

// 0x5B32B0
static int dword_5B32B0[AMMUNITION_TYPE_COUNT] = {
    7038,
    7039,
    7040,
    7041,
};

// 0x5E87E0
static char** item_ammunition_type_names;

// 0x5E87E4
static bool dword_5E87E4;

// 0x5E87F8
static char** item_armor_coverage_type_names;

// 0x5E87FC
static int item_msg_file;

// 0x5E8800
static int dword_5E8800;

// 0x5E8808
static TigRect stru_5E8808;

// 0x460E70
bool item_init(GameContext* ctx)
{
    TigWindowData window_data;
    if (tig_window_data(ctx->iso_window_handle, &window_data) != TIG_OK) {
        return false;
    }

    stru_5E8808.x = 0;
    stru_5E8808.y = 0;
    stru_5E8808.width = window_data.rect.width;
    stru_5E8808.height = window_data.rect.height;

    dword_5E87E4 = ctx->editor;

    if (!message_load("mes\\item.mes", &item_msg_file)) {
        return false;
    }

    item_ammunition_type_names = (char**)calloc(AMMUNITION_TYPE_COUNT, sizeof(*item_ammunition_type_names));
    item_armor_coverage_type_names = (char**)calloc(ARMOR_COVERAGE_TYPE_COUNT, sizeof(*item_armor_coverage_type_names));

    MessageListItem msg;

    for (int index = 0; index < AMMUNITION_TYPE_COUNT; index++) {
        msg.num = index + FIRST_AMMUNITION_TYPE_ID;
        sub_4D43A0(item_msg_file, &msg);
        item_ammunition_type_names[index] = msg.text;
    }

    for (int index = 0; index < ARMOR_COVERAGE_TYPE_COUNT; index++) {
        msg.num = index + FIRST_ARMOR_COVERAGE_TYPE_ID;
        sub_4D43A0(item_msg_file, &msg);
        item_armor_coverage_type_names[index] = msg.text;
    }

    dword_5E87E8 = 0;
    dword_5E8820 = 0;
    dword_5E8800 = 1;

    return true;
}

// 0x460F90
void item_exit()
{
    free(item_ammunition_type_names);
    free(item_armor_coverage_type_names);
    message_unload(item_msg_file);
}

// 0x460FC0
void item_resize(ResizeContext *ctx)
{
    stru_5E8808 = *ctx->field_14;
}

// 0x4612A0
bool item_parent(object_id_t object_id, object_id_t* parent_object_id)
{
    if (!item_is_item(object_id)) {
        tig_debug_printf("Item: item_parent: ERROR: Called on non-Item!\n");
        return false;
    }

    if (parent_object_id != NULL) {
        *parent_object_id = 0;
    }

    if ((object_field_get(object_id, OBJ_F_FLAGS) & OBJECT_FLAG_0x1000) == 0) {
        return false;
    }

    if (parent_object_id != NULL) {
        *parent_object_id = sub_406E80(object_id, OBJ_F_ITEM_PARENT);
    }

    return true;
}

// 0x461310
bool item_is_item(object_id_t object_id)
{
    if (object_id != 0) {
        int type = object_field_get(object_id, OBJ_F_TYPE);
        if (type >= OBJ_TYPE_WEAPON && type <= OBJ_TYPE_14) {
            return true;
        }
    }

    return false;
}

// 0x461340
int sub_461340(object_id_t object_id)
{
    // FIXME: Result is not used.
    item_parent(object_id, NULL);
    return object_field_get(object_id, OBJ_F_ITEM_INV_LOCATION);
}

// 0x461370
int sub_461370(object_id_t item_id, object_id_t owner_id)
{
    if (object_field_get(item_id, OBJ_F_TYPE) == OBJ_TYPE_8) {
        return 0;
    }

    int weight = object_field_get(item_id, OBJ_F_ITEM_WEIGHT);

    if (owner_id != 0) {
        int v1 = object_field_get(owner_id, OBJ_F_ITEM_MAGIC_WEIGHT_ADJ);
        weight += sub_461590(item_id, owner_id, v1);
    }

    int field;
    if (sub_462410(item_id, &field) != -1) {
        weight *= object_field_get(item_id, field) / 4;
    }

    return weight;
}

// 0x461410
int sub_461410(object_id_t object_id)
{
    int inventory_num_field;
    int inventory_list_idx_field;
    if (object_get_field(object_id, OBJ_F_TYPE) == OBJ_TYPE_CONTAINER) {
        inventory_num_field = OBJ_F_CONTAINER_INVENTORY_NUM;
        inventory_list_idx_field = OBJ_F_CONTAINER_INVENTORY_LIST_IDX;
    } else {
        inventory_num_field = OBJ_F_CRITTER_INVENTORY_NUM;
        inventory_list_idx_field = OBJ_F_CRITTER_INVENTORY_LIST_IDX;
    }

    int count = object_field_get(object_id, inventory_num_field);
    int weight = 0;
    for (int index = 0; index < count; index++) {
        object_id_t item_object_id = sub_407610(object_id, inventory_list_idx_field, 0);
        weight += sub_461370(item_object_id, object_id);
    }

    return weight;
}

// 0x4614A0
int sub_4614A0(object_id_t item_id, object_id_t owner_id)
{
    int complexity = item_magic_tech_complexity(item_id);

    int owner_type = object_field_get(owner_id, OBJ_F_TYPE);
    if (owner_type != OBJ_TYPE_15 && owner_type != OBJ_TYPE_CRITTER) {
        return complexity;
    }

    int adjusted_complexity = (complexity + sub_4B0490(owner_id, STAT_MAGICK_TECH_APTITUDE)) / 2;
    if (complexity < 0) {
        if (adjusted_complexity > 0) {
            return 0;
        } else if (adjusted_complexity < complexity) {
            return complexity;
        } else {
            return adjusted_complexity;
        }
    } else {
        if (adjusted_complexity < 0) {
            return 0;
        } else if (adjusted_complexity > complexity) {
            return complexity;
        } else {
            return adjusted_complexity;
        }
    }
}

// 0x461520
int item_magic_tech_complexity(object_id_t item_id)
{
    return object_field_get(item_id, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY);
}

// 0x461540
int sub_461540(object_id_t item_id, object_id_t owner_id)
{
    int complexity = item_magic_tech_complexity(item_id);
    if (complexity != 0) {
        return 100 * sub_4614A0(item_id, owner_id) / complexity;
    } else {
        return 100;
    }
}

// 0x461590
int sub_461590(object_id_t item_id, object_id_t owner_id, int a3)
{
    int complexity = item_magic_tech_complexity(item_id);
    if (complexity > 0) {
        // FIXME: Calculating complexity twice.
        return a3 * sub_4614A0(item_id, owner_id) / item_magic_tech_complexity(item_id);
    } else if (complexity < 0) {
        // FIXME: Calculating complexity twice.
        return a3 - a3 * sub_4614A0(item_id, owner_id) / item_magic_tech_complexity(item_id);
    } else {
        return a3;
    }
}

// 0x461780
void sub_461780(object_id_t item_id, int* width, int* height)
{
    unsigned int inventory_art_id = object_field_get(item_id, OBJ_F_ITEM_INV_AID);

    TigArtFrameData art_frame_data;
    if (tig_art_frame_data(inventory_art_id, &art_frame_data) == TIG_OK) {
        if (width != NULL) {
            *width = (art_frame_data.width - 1) / 32 + 1;
        }

        if (height != NULL) {
            *height = (art_frame_data - 1) / 32 + 1;
        }
    } else {
        if (width != NULL) {
            *width = 1;
        }

        if (height != NULL) {
            *height = 1;
        }
    }
}

// 0x461F20
int item_worth(object_id_t item_id)
{
    if (!sub_464370(item_id)) {
        return 300;
    }

    int worth = object_field_get(item_id, OBJ_F_ITEM_WORTH);
    if (worth < 2) {
        worth = 2;
    }
    return worth;
}

// 0x461F60
bool sub_461F60(object_id_t item_id)
{
    return object_field_get(item_id, OBJ_F_ITEM_WORTH) == 0;
}

// 0x462410
int sub_462410(object_id_t item_id, int* quantity_field_ptr)
{
    // TODO: Rename.
    int rc = -1;
    int quantity_field;
    int ammo_type;

    switch (object_field_get(item_id, OBJ_F_TYPE)) {
    case OBJ_TYPE_8:
        rc = OBJ_F_CRITTER_GOLD;
        quantity_field = OBJ_F_GOLD_QUANTITY;
        break;
    case OBJ_TYPE_AMMO:
        ammo_type = object_field_get(item_id, OBJ_F_AMMO_TYPE);
        rc = dword_5B32A0[ammo_type];
        quantity_field = OBJ_F_AMMO_QUANTITY;
        break;
    }

    // TODO: There is a strange default value at 0x462460.

    if (quantity_field_ptr != NULL) {
        *quantity_field_ptr = quantity_field;
    }

    return rc;
}

// 0x462820
int sub_462820(object_id_t item_id)
{
    int inventory_count_field;
    int inventory_list_field;
    if (object_field_get(item_id, OBJ_F_TYPE) == OBJ_TYPE_CONTAINER) {
        inventory_count_field = OBJ_F_CONTAINER_INVENTORY_NUM;
        inventory_list_field = OBJ_F_CONTAINER_INVENTORY_LIST_IDX;
    } else {
        inventory_count_field = OBJ_F_CRITTER_INVENTORY_NUM;
        inventory_list_field = OBJ_F_CRITTER_INVENTORY_LIST_IDX;
    }

    if (object_field_get(item_id, inventory_count_field) > 0) {
        return sub_407610(item_id, inventory_list_field);
    } else {
        return 0;
    }
}

// 0x462880
int sub_462880(object_id_t item_id, object_id_t** child_item_ids_ptr)
{
    *child_item_ids_ptr = NULL;

    int inventory_count_field;
    int inventory_list_field;
    if (object_field_get(item_id, OBJ_F_TYPE) == OBJ_TYPE_CONTAINER) {
        inventory_count_field = OBJ_F_CONTAINER_INVENTORY_NUM;
        inventory_list_field = OBJ_F_CONTAINER_INVENTORY_LIST_IDX;
    } else {
        inventory_count_field = OBJ_F_CRITTER_INVENTORY_NUM;
        inventory_list_field = OBJ_F_CRITTER_INVENTORY_LIST_IDX;
    }

    int count = object_field_get(item_id, inventory_count_field);
    if (count > 0) {
        *child_item_ids_ptr = (object_id_t*)malloc(sizeof(**child_item_ids_ptr));
        for (int index = 0; index < count; index++) {
            child_item_ids_ptr[index] = sub_407610(item_id, inventory_list_field, index);
        }
    }
    return count;
}

// 0x462920
void sub_462920(object_id_t* child_item_ids)
{
    if (child_item_ids != NULL) {
        free(child_item_ids);
    }
}

// 0x4641A0
int item_inventory_source(object_id_t item_id)
{
    if (item_id != 0) {
        switch (object_field_get(item_id, OBJ_F_TYPE)) {
        case OBJ_TYPE_CONTAINER:
            return object_field_get(item_id, OBJ_F_CONTAINER_INVENTORY_SOURCE);
        case OBJ_TYPE_CRITTER:
            return object_field_get(item_id, OBJ_F_CRITTER_INVENTORY_SOURCE);
        }
    }
    return 0;
}

// 0x4657D0
const char* ammunition_type_get_name(int ammo_type)
{
    return item_ammunition_type_names[ammo_type];
}

// 0x4657E0
size_t ammunition_type_get_name_length(int ammo_type)
{
    return strlen(item_ammunition_type_names[ammo_type]);
}

// 0x465820
int item_ammo_quantity(object_id_t item_id, int a2)
{
    object_id_t ammo_id;

    if (object_field_get(item_id, OBJ_F_TYPE) == OBJ_TYPE_AMMO) {
        ammo_id = item_id;
    } else {
        ammo_id = sub_465870(item_id, a2);
    }

    if (ammo_id != 0) {
        return object_field_get(ammo_id, OBJ_F_AMMO_QUANTITY);
    } else {
        return 0;
    }
}

// 0x465870
int sub_465870(object_id_t item_id, int ammo_type)
{
    switch (object_field_get(item_id, OBJ_F_TYPE)) {
    case OBJ_TYPE_15:
    case OBJ_TYPE_CRITTER:
        return sub_406E80(item_id, dword_5B32A0[ammo_type]);
    case OBJ_TYPE_CONTAINER:
        return sub_462540(item_id, dword_5B32B0[ammo_type], 0);
    default:
        return 0;
    }
}

// 0x465A90
const char* armor_coverage_type_get_name(int coverage_type)
{
    return item_armor_coverage_type_names[coverage_type];
}

// 0x465AA0
size_t armor_coverage_type_get_name_length(int coverage_type)
{
    return strlen(item_armor_coverage_type_names[coverage_type]);
}

// 0x465CA0
int item_weapon_ammo_type(object_id_t item_id)
{
    if (item_id != 0) {
        if (object_field_get(item_id, OBJ_F_TYPE) == OBJ_TYPE_WEAPON) {
            return object_field_get(item_id, OBJ_F_WEAPON_AMMO_TYPE);
        }
    }

    return 10000;
}

// 0x465E30
int item_weapon_range(object_id_t item_id, object_id_t critter_id)
{
    if (item_id != 0) {
        if (object_field_get(item_id, OBJ_F_TYPE) == OBJ_TYPE_WEAPON) {
            int magic_range_adj = object_field_get(item_id, OBJ_F_WEAPON_MAGIC_RANGE_ADJ);
            return object_field_get(item_id, OBJ_F_WEAPON_RANGE) + sub_461590(item_id, critter_id, magic_range_adj);
    }

    return 1;
}

// 0x468180
void sub_468180()
{
    dword_5E8800++;
}

// 0x468190
void sub_468190()
{
    dword_5E8800--;
}

// 0x4681A0
int sub_4681A0()
{
    return dword_5E8800;
}
