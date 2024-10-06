#include "game/item.h"

#include "game/description.h"
#include "game/magictech.h"
#include "game/mes.h"
#include "game/mp_utils.h"
#include "game/mt_item.h"
#include "game/object.h"
#include "game/player.h"
#include "game/proto.h"
#include "game/random.h"
#include "game/skill.h"
#include "game/stat.h"
#include "game/timeevent.h"
#include "game/ui.h"

#define FIRST_AMMUNITION_TYPE_ID 0
#define FIRST_ARMOR_COVERAGE_TYPE_ID (FIRST_AMMUNITION_TYPE_ID + AMMUNITION_TYPE_COUNT)

typedef struct ItemInsertInfo {
    /* 0000 */ int64_t field_0;
    /* 0008 */ int64_t field_8;
    /* 0010 */ int64_t field_10;
} ItemInsertInfo;

static_assert(sizeof(ItemInsertInfo) == 0x18, "wrong size");

typedef struct ItemRemoveInfo {
    /* 0000 */ int64_t field_0;
    /* 0008 */ int64_t field_8;
} ItemRemoveInfo;

static_assert(sizeof(ItemRemoveInfo) == 0x10, "wrong size");

static bool sub_464150(TimeEvent* timeevent);
static int64_t item_gold_obj(int64_t obj);
static int sub_465010(int64_t obj);
static int64_t item_ammo_obj(object_id_t obj, int ammo_type);
static bool sub_466A00(int64_t a1, int64_t key_obj);
static void sub_466A50(int64_t key_obj, int64_t key_ring_obj);
static void sub_466BD0(int64_t key_ring_obj);
static bool item_insert_failure(ItemInsertInfo* item_insert_info);
static const char* item_cannot_msg(int reason);
static void sub_4677B0(int64_t a1, int64_t a2, int a3);
static void sub_467CB0(int64_t a1, int64_t a2, int a3);
static bool item_force_remove_failure(ItemRemoveInfo* item_remove_info);
static bool sub_467E70();
static bool sub_468110(int64_t obj);
static bool sub_468150(TimeEvent* timeevent);

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

// 0x5B32C0
int dword_5B32C0[RACE_COUNT] = {
    2,
    1,
    2,
    2,
    1,
    1,
    2,
    4,
    2,
    4,
    2,
};

// 0x5E87E0
static char** item_ammunition_type_names;

// 0x5E87E4
static bool dword_5E87E4;

// 0x5E87E8
static bool dword_5E87E8;

// 0x5E87F0
static int64_t qword_5E87F0;

// 0x5E87F8
static char** item_armor_coverage_type_names;

// 0x5E87FC
static mes_file_handle_t item_mes_file;

// 0x5E8800
static int dword_5E8800;

// 0x5E8808
static TigRect stru_5E8808;

// 0x5E8818
static int64_t qword_5E8818;

// 0x5E8820
static bool dword_5E8820;

// 0x460E70
bool item_init(GameInitInfo* init_info)
{
    TigWindowData window_data;
    MesFileEntry msg;
    int index;

    if (tig_window_data(init_info->iso_window_handle, &window_data) != TIG_OK) {
        return false;
    }

    stru_5E8808.x = 0;
    stru_5E8808.y = 0;
    stru_5E8808.width = window_data.rect.width;
    stru_5E8808.height = window_data.rect.height;

    dword_5E87E4 = init_info->editor;

    if (!mes_load("mes\\item.mes", &item_mes_file)) {
        return false;
    }

    item_ammunition_type_names = (char**)CALLOC(AMMUNITION_TYPE_COUNT, sizeof(*item_ammunition_type_names));
    item_armor_coverage_type_names = (char**)CALLOC(ARMOR_COVERAGE_TYPE_COUNT, sizeof(*item_armor_coverage_type_names));

    for (index = 0; index < AMMUNITION_TYPE_COUNT; index++) {
        msg.num = index + FIRST_AMMUNITION_TYPE_ID;
        mes_get_msg(item_mes_file, &msg);
        item_ammunition_type_names[index] = msg.str;
    }

    for (index = 0; index < ARMOR_COVERAGE_TYPE_COUNT; index++) {
        msg.num = index + FIRST_ARMOR_COVERAGE_TYPE_ID;
        mes_get_msg(item_mes_file, &msg);
        item_armor_coverage_type_names[index] = msg.str;
    }

    dword_5E87E8 = 0;
    dword_5E8820 = 0;
    dword_5E8800 = 1;

    return true;
}

// 0x460F90
void item_exit()
{
    FREE(item_ammunition_type_names);
    FREE(item_armor_coverage_type_names);
    mes_unload(item_mes_file);
}

// 0x460FC0
void item_resize(ResizeInfo *resize_info)
{
    stru_5E8808 = resize_info->field_14;
}

// 0x4612A0
bool item_parent(object_id_t object_id, object_id_t* parent_object_id)
{
    if (!item_is_item(object_id)) {
        tig_debug_printf("Item: item_parent: ERROR: Called on non-Item!\n");
        return false;
    }

    if (parent_object_id != NULL) {
        *parent_object_id = OBJ_HANDLE_NULL;
    }

    if ((obj_field_int32_get(object_id, OBJ_F_FLAGS) & OF_INVENTORY) == 0) {
        return false;
    }

    if (parent_object_id != NULL) {
        *parent_object_id = obj_field_handle_get(object_id, OBJ_F_ITEM_PARENT);
    }

    return true;
}

// 0x461310
bool item_is_item(object_id_t object_id)
{
    int type;

    if (object_id != OBJ_HANDLE_NULL) {
        type = obj_field_int32_get(object_id, OBJ_F_TYPE);
        if (type >= OBJ_TYPE_WEAPON && type <= OBJ_TYPE_ITEM_GENERIC) {
            return true;
        }
    }

    return false;
}

// 0x461340
int item_inventory_location_get(object_id_t object_id)
{
    // FIXME: Result is not used.
    item_parent(object_id, NULL);
    return obj_field_int32_get(object_id, OBJ_F_ITEM_INV_LOCATION);
}

// 0x461370
int sub_461370(object_id_t item_obj, object_id_t owner_obj)
{
    int weight;
    int weight_adj;
    int quantity_fld;

    if (obj_field_int32_get(item_obj, OBJ_F_TYPE) == OBJ_TYPE_ITEM_GOLD) {
        return 0;
    }

    weight = obj_field_int32_get(item_obj, OBJ_F_ITEM_WEIGHT);

    if (owner_obj != OBJ_HANDLE_NULL) {
        weight_adj = obj_field_int32_get(owner_obj, OBJ_F_ITEM_MAGIC_WEIGHT_ADJ);
        weight += sub_461590(item_obj, owner_obj, weight_adj);
    }

    if (sub_462410(item_obj, &quantity_fld) != -1) {
        weight *= obj_field_int32_get(item_obj, quantity_fld) / 4;
    }

    return weight;
}

// 0x461410
int item_total_weight(object_id_t obj)
{
    int inventory_num_field;
    int inventory_list_idx_field;
    int index;
    int count;
    int weight;
    int64_t item_obj;

    if (object_get_field(obj, OBJ_F_TYPE) == OBJ_TYPE_CONTAINER) {
        inventory_num_field = OBJ_F_CONTAINER_INVENTORY_NUM;
        inventory_list_idx_field = OBJ_F_CONTAINER_INVENTORY_LIST_IDX;
    } else {
        inventory_num_field = OBJ_F_CRITTER_INVENTORY_NUM;
        inventory_list_idx_field = OBJ_F_CRITTER_INVENTORY_LIST_IDX;
    }

    count = obj_field_int32_get(obj, inventory_num_field);
    weight = 0;
    for (index = 0; index < count; index++) {
        item_obj = obj_arrayfield_handle_get(obj, inventory_list_idx_field, index);
        weight += sub_461370(item_obj, obj);
    }

    return weight;
}

// 0x4614A0
int sub_4614A0(object_id_t item_id, object_id_t owner_id)
{
    int complexity;
    int owner_type;
    int adjusted_complexity;

    complexity = item_magic_tech_complexity(item_id);

    owner_type = obj_field_int32_get(owner_id, OBJ_F_TYPE);
    if (owner_type != OBJ_TYPE_PC && owner_type != OBJ_TYPE_NPC) {
        return complexity;
    }

    adjusted_complexity = (complexity + stat_level(owner_id, STAT_MAGICK_TECH_APTITUDE)) / 2;
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
    return obj_field_int32_get(item_id, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY);
}

// 0x461540
int sub_461540(object_id_t item_id, object_id_t owner_id)
{
    int complexity;

    complexity = item_magic_tech_complexity(item_id);
    if (complexity != 0) {
        return 100 * sub_4614A0(item_id, owner_id) / complexity;
    } else {
        return 100;
    }
}

// 0x461590
int sub_461590(object_id_t item_id, object_id_t owner_id, int a3)
{
    int complexity;

    complexity = item_magic_tech_complexity(item_id);
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

// 0x461620
int sub_461620(int64_t item_obj, int64_t owner_obj, int64_t a3)
{
    int complexity;
    int aptitude1;
    int aptitude2;

    complexity = item_magic_tech_complexity;

    if (!obj_type_is_critter(obj_field_int32_get(owner_obj, OBJ_F_TYPE))) {
        return 0;
    }
    aptitude1 = stat_level(owner_obj, STAT_MAGICK_TECH_APTITUDE);

    if (!obj_type_is_critter(obj_field_int32_get(a3, OBJ_F_TYPE))) {
        return 0;
    }
    aptitude2 = stat_level(a3, STAT_MAGICK_TECH_APTITUDE);

    if (complexity > 0) {
        if (aptitude2 < 0) {
            if (aptitude1 < 0) {
                return -aptitude2;
            } else {
                return -aptitude2 * (100 - aptitude1) / 100;
            }
        }
    } else if (complexity < 0) {
        if (aptitude2 > 0) {
            if (aptitude1 > 0) {
                return aptitude2;
            } else {
                return aptitude2 * (100 + aptitude1) / 100;
            }
        }
    }

    return 0;
}

// 0x461700
int sub_461700(int64_t item_obj, int64_t owner_obj)
{
    int complexity;
    int aptitude;

    complexity = item_magic_tech_complexity(item_obj);
    if (complexity >= 0) {
        return 0;
    }

    if (!obj_type_is_critter(obj_field_int32_get(owner_obj, OBJ_F_TYPE))) {
        return 0;
    }

    aptitude = stat_level(owner_obj, STAT_MAGICK_TECH_APTITUDE);
    if (aptitude <= 0) {
        return 0;
    }

    // TODO: Unclear math.
    return 0;
}

// 0x461780
void sub_461780(object_id_t item_id, int* width, int* height)
{
    tig_art_id_t aid;
    TigArtFrameData art_frame_data;

    aid = obj_field_int32_get(item_id, OBJ_F_ITEM_INV_AID);

    if (tig_art_frame_data(aid, &art_frame_data) == TIG_OK) {
        if (width != NULL) {
            *width = (art_frame_data.width - 1) / 32 + 1;
        }

        if (height != NULL) {
            *height = (art_frame_data.height - 1) / 32 + 1;
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

// 0x4617F0
void sub_4617F0(int64_t a1, int64_t a2)
{
    sub_461810(a1, a2, -1);
}

// 0x461810
void sub_461810()
{
    // TODO: Incomplete.
}

// 0x461A70
void sub_461A70(int64_t a1)
{
    sub_461AB0(a1, 0);
}

// 0x461A90
void sub_461A90(int64_t a1)
{
    sub_461AB0(a1, random_between(2, 4));
}

// 0x461AB0
void sub_461AB0()
{
    // TODO: Incomplete.
}

// 0x461CA0
void sub_461CA0()
{
    // TODO: Incomplete.
}

// 0x461F20
int item_worth(object_id_t item_id)
{
    int worth;

    if (!item_is_identified(item_id)) {
        return 300;
    }

    worth = obj_field_int32_get(item_id, OBJ_F_ITEM_WORTH);
    if (worth < 2) {
        worth = 2;
    }

    return worth;
}

// 0x461F60
bool sub_461F60(object_id_t item_id)
{
    return obj_field_int32_get(item_id, OBJ_F_ITEM_WORTH) == 0;
}

// 0x461F80
void sub_461F80()
{
    // TODO: Incomplete.
}

// 0x462170
bool sub_462170(int64_t a1, int64_t a2, int64_t a3)
{
    // TODO: Review.
    return !sub_461F60(a1)
        && (!tig_art_item_id_destroyed_get(obj_field_int32_get(a1, OBJ_F_CURRENT_AID)))
        && (basic_skill_get_training(a2, BASIC_SKILL_HAGGLE) >= TRAINING_EXPERT
            || (sub_441980(a1, a3, a2, 17, 0) == 1
                && sub_464200(a1, a3)))
        && ((obj_field_int32_get(a3, OBJ_F_NPC_FLAGS) & ONF_FENCE) != 0
            || (obj_field_int32_get(a1, OBJ_F_ITEM_FLAGS) & OIF_STOLEN) == 0);
}

// 0x462230
bool sub_462230(int64_t a1, int64_t a2, int64_t a3)
{
    (void)a2;

    // TODO: Review.
    return basic_skill_get_training(a3, BASIC_SKILL_HAGGLE) >= TRAINING_MASTER
        || ((obj_field_int32_get(a1, OBJ_F_ITEM_FLAGS) & OIF_WONT_SELL) == 0
            && (item_inventory_location_get(a1) < 1000 || item_inventory_location_get(a1) > 1008));
}

// 0x4622A0
int item_throwing_distance(int64_t item_obj, int64_t critter_obj)
{
    int distance;
    int weight;

    distance = 50 * stat_level(critter_obj, STAT_STRENGTH);
    if (basic_skill_get_training(critter_obj, BASIC_SKILL_THROWING) >= TRAINING_EXPERT) {
        distance += distance / 2;
    }

    weight = item_weight(item_obj, critter_obj) / 10;
    if (weight < 1) {
        weight = 1;
    }

    distance /= weight;
    distance /= 5;
    if (distance < 1) {
        distance = 1;
    }

    return distance;
}

// 0x462330
void sub_462330(int64_t item_obj, int index, int* min_damage, int* max_damage)
{
    int64_t owner_obj;
    int weight;

    if (obj_field_int32_get(item_obj, OBJ_F_TYPE) == OBJ_TYPE_WEAPON
        && (obj_field_int32_get(item_obj, OBJ_F_WEAPON_FLAGS) & OWF_THROWABLE) != 0) {
        *min_damage = obj_arrayfield_int32_get(item_obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, index);
        *max_damage = obj_arrayfield_int32_get(item_obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, index);
        return;
    }

    if (index == 0) {
        owner_obj = OBJ_HANDLE_NULL;
        item_parent(item_obj, &owner_obj);
        weight = item_weight(item_obj, owner_obj);

        *min_damage = (weight + 99) / 100;
        // TODO: Unclear math.
        *max_damage = *min_damage;
        return;
    }

    *min_damage = 0;
    *max_damage = 0;
}

// 0x462410
int sub_462410(object_id_t item_id, int* quantity_field_ptr)
{
    // TODO: Rename.
    int rc = -1;
    int quantity_field;
    int ammo_type;

    switch (obj_field_int32_get(item_id, OBJ_F_TYPE)) {
    case OBJ_TYPE_ITEM_GOLD:
        rc = OBJ_F_CRITTER_GOLD;
        quantity_field = OBJ_F_GOLD_QUANTITY;
        break;
    case OBJ_TYPE_AMMO:
        ammo_type = obj_field_int32_get(item_id, OBJ_F_AMMO_TYPE);
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

// 0x462480
int64_t item_find_by_name(int64_t obj, int name)
{
    int obj_type;
    int inventory_num_fld;
    int inventory_list_fld;
    int cnt;
    int index;
    int64_t item_obj;

    if (obj == OBJ_HANDLE_NULL) {
        return OBJ_HANDLE_NULL;
    }

    obj_type = obj_field_int32_get(obj, OBJ_F_TYPE);
    if (obj_type == OBJ_TYPE_CONTAINER) {
        inventory_num_fld = OBJ_F_CONTAINER_INVENTORY_NUM;
        inventory_list_fld = OBJ_F_CONTAINER_INVENTORY_LIST_IDX;
    } else if (obj_type_is_critter(obj_type)) {
        inventory_num_fld = OBJ_F_CRITTER_INVENTORY_NUM;
        inventory_list_fld = OBJ_F_CRITTER_INVENTORY_LIST_IDX;
    } else {
        return OBJ_HANDLE_NULL;
    }

    cnt = obj_field_int32_get(obj, inventory_num_fld);
    for (index = 0; index < cnt; index++) {
        item_obj = obj_arrayfield_handle_get(obj, inventory_list_fld, index);
        if (obj_field_int32_get(item_obj, OBJ_F_NAME) == name) {
            return item_obj;
        }
    }

    return OBJ_HANDLE_NULL;
}

// 0x4626B0
int64_t item_find_first_of_type(int64_t obj, int type)
{
    int obj_type;
    int inventory_num_fld;
    int inventory_list_fld;
    int cnt;
    int index;
    int64_t item_obj;

    if (obj == OBJ_HANDLE_NULL) {
        return OBJ_HANDLE_NULL;
    }

    obj_type = obj_field_int32_get(obj, OBJ_F_TYPE);
    if (obj_type == OBJ_TYPE_CONTAINER) {
        inventory_num_fld = OBJ_F_CONTAINER_INVENTORY_NUM;
        inventory_list_fld = OBJ_F_CONTAINER_INVENTORY_LIST_IDX;
    } else if (obj_type_is_critter(obj_type)) {
        inventory_num_fld = OBJ_F_CRITTER_INVENTORY_NUM;
        inventory_list_fld = OBJ_F_CRITTER_INVENTORY_LIST_IDX;
    } else {
        return OBJ_HANDLE_NULL;
    }

    cnt = obj_field_int32_get(obj, inventory_num_fld);
    for (index = 0; index < cnt; index++) {
        item_obj = obj_arrayfield_handle_get(obj, inventory_list_fld, index);
        if (obj_field_int32_get(item_obj, OBJ_F_TYPE) == type) {
            return item_obj;
        }
    }

    return OBJ_HANDLE_NULL;
}

// 0x462760
int64_t item_find_first_generic(int64_t obj, unsigned int flags)
{
    int obj_type;
    int inventory_num_fld;
    int inventory_list_fld;
    int cnt;
    int index;
    int64_t item_obj;

    if (obj == OBJ_HANDLE_NULL) {
        return OBJ_HANDLE_NULL;
    }

    obj_type = obj_field_int32_get(obj, OBJ_F_TYPE);
    if (obj_type == OBJ_TYPE_CONTAINER) {
        inventory_num_fld = OBJ_F_CONTAINER_INVENTORY_NUM;
        inventory_list_fld = OBJ_F_CONTAINER_INVENTORY_LIST_IDX;
    } else if (obj_type_is_critter(obj_type)) {
        inventory_num_fld = OBJ_F_CRITTER_INVENTORY_NUM;
        inventory_list_fld = OBJ_F_CRITTER_INVENTORY_LIST_IDX;
    } else {
        return OBJ_HANDLE_NULL;
    }

    cnt = obj_field_int32_get(obj, inventory_num_fld);
    for (index = 0; index < cnt; index++) {
        item_obj = obj_arrayfield_handle_get(obj, inventory_list_fld, index);
        if (obj_field_int32_get(item_obj, OBJ_F_TYPE) == OBJ_TYPE_ITEM_GENERIC
            && (obj_field_int32_get(item_obj, OBJ_F_GENERIC_FLAGS) & flags) != 0) {
            return item_obj;
        }
    }

    return OBJ_HANDLE_NULL;
}

// 0x462820
int64_t item_find_first(object_id_t obj)
{
    int inventory_num_fld;
    int inventory_list_fld;

    if (obj_field_int32_get(obj, OBJ_F_TYPE) == OBJ_TYPE_CONTAINER) {
        inventory_num_fld = OBJ_F_CONTAINER_INVENTORY_NUM;
        inventory_list_fld = OBJ_F_CONTAINER_INVENTORY_LIST_IDX;
    } else {
        inventory_num_fld = OBJ_F_CRITTER_INVENTORY_NUM;
        inventory_list_fld = OBJ_F_CRITTER_INVENTORY_LIST_IDX;
    }

    if (obj_field_int32_get(obj, inventory_num_fld) > 0) {
        return obj_arrayfield_handle_get(obj, inventory_list_fld, 0);
    } else {
        return 0;
    }
}

// 0x462880
int item_get_all(object_id_t obj, object_id_t** list_ptr)
{
    int inventory_num_fld;
    int inventory_list_fld;
    int index;
    int cnt;

    *list_ptr = NULL;

    if (obj_field_int32_get(obj, OBJ_F_TYPE) == OBJ_TYPE_CONTAINER) {
        inventory_num_fld = OBJ_F_CONTAINER_INVENTORY_NUM;
        inventory_list_fld = OBJ_F_CONTAINER_INVENTORY_LIST_IDX;
    } else {
        inventory_num_fld = OBJ_F_CRITTER_INVENTORY_NUM;
        inventory_list_fld = OBJ_F_CRITTER_INVENTORY_LIST_IDX;
    }

    cnt = obj_field_int32_get(obj, inventory_num_fld);
    if (cnt > 0) {
        *list_ptr = (object_id_t*)MALLOC(sizeof(**list_ptr));
        for (index = 0; index < cnt; index++) {
            list_ptr[index] = obj_arrayfield_handle_get(obj, inventory_list_fld, index);
        }
    }

    return cnt;
}

// 0x462920
void sub_462920(object_id_t* list)
{
    if (list != NULL) {
        FREE(list);
    }
}

// 0x462930
int64_t item_find_first_matching_prototype(int64_t obj, int64_t a2)
{
    int inventory_num_fld;
    int inventory_list_fld;
    int index;
    int cnt;
    int64_t prototype_obj;
    int64_t item_obj;

    if (sub_49B290(a2) == 15065) {
        return OBJ_HANDLE_NULL;
    }

    if (obj_field_int32_get(obj, OBJ_F_TYPE) == OBJ_TYPE_CONTAINER) {
        inventory_num_fld = OBJ_F_CONTAINER_INVENTORY_NUM;
        inventory_list_fld = OBJ_F_CONTAINER_INVENTORY_LIST_IDX;
    } else {
        inventory_num_fld = OBJ_F_CRITTER_INVENTORY_NUM;
        inventory_list_fld = OBJ_F_CRITTER_INVENTORY_LIST_IDX;
    }

    cnt = obj_field_int32_get(obj, inventory_num_fld);
    prototype_obj = obj_field_handle_get(a2, OBJ_F_PROTOTYPE_HANDLE);

    for (index = 0; index < cnt; index++) {
        item_obj = obj_arrayfield_handle_get(obj, inventory_list_fld, index);
        if (item_obj != a2
            && obj_field_int32_get(item_obj, OBJ_F_PROTOTYPE_HANDLE) == prototype_obj) {
            return item_obj;
        }
    }

    return OBJ_HANDLE_NULL;
}

// 0x462A30
int64_t sub_462A30(int64_t obj, int64_t a2)
{
    int inventory_num_fld;
    int inventory_list_fld;
    int index;
    int cnt;
    int64_t prototype_obj;
    int64_t item_obj;
    int inventory_location;

    if (sub_49B290(a2) == 15065) {
        return OBJ_HANDLE_NULL;
    }

    if (obj_field_int32_get(obj, OBJ_F_TYPE) == OBJ_TYPE_CONTAINER) {
        inventory_num_fld = OBJ_F_CONTAINER_INVENTORY_NUM;
        inventory_list_fld = OBJ_F_CONTAINER_INVENTORY_LIST_IDX;
    } else {
        inventory_num_fld = OBJ_F_CRITTER_INVENTORY_NUM;
        inventory_list_fld = OBJ_F_CRITTER_INVENTORY_LIST_IDX;
    }

    cnt = obj_field_int32_get(obj, inventory_num_fld);
    prototype_obj = obj_field_handle_get(a2, OBJ_F_PROTOTYPE_HANDLE);

    for (index = 0; index < cnt; index++) {
        item_obj = obj_arrayfield_handle_get(obj, inventory_list_fld, index);
        if (item_obj != a2
            && obj_field_int32_get(item_obj, OBJ_F_PROTOTYPE_HANDLE) == prototype_obj) {
            inventory_location = obj_field_int32_get(item_obj, OBJ_F_ITEM_INV_LOCATION);
            if (inventory_location >= 2000 && inventory_location <= 2009) {
                return item_obj;
            }
        }
    }

    return OBJ_HANDLE_NULL;
}

// 0x462B40
int item_count_items_matching_prototype(int64_t obj, int64_t a2)
{
    int inventory_num_fld;
    int inventory_list_fld;
    int index;
    int cnt;
    int64_t prototype_obj;
    int64_t item_obj;
    int ret = 1;

    if (sub_49B290(a2) == 15065) {
        return ret;
    }

    if (obj_field_int32_get(obj, OBJ_F_TYPE) == OBJ_TYPE_CONTAINER) {
        inventory_num_fld = OBJ_F_CONTAINER_INVENTORY_NUM;
        inventory_list_fld = OBJ_F_CONTAINER_INVENTORY_LIST_IDX;
    } else {
        inventory_num_fld = OBJ_F_CRITTER_INVENTORY_NUM;
        inventory_list_fld = OBJ_F_CRITTER_INVENTORY_LIST_IDX;
    }

    cnt = obj_field_int32_get(obj, inventory_num_fld);
    prototype_obj = obj_field_handle_get(a2, OBJ_F_PROTOTYPE_HANDLE);

    for (index = 0; index < cnt; index++) {
        item_obj = obj_arrayfield_handle_get(obj, inventory_list_fld, index);
        if (item_obj != a2
            && obj_field_int32_get(item_obj, OBJ_F_PROTOTYPE_HANDLE) == prototype_obj) {
            ret++;
        }
    }

    return ret;
}

// 0x462C30
int sub_462C30(int64_t a1, int64_t a2)
{
    if (a2 == OBJ_HANDLE_NULL
        || (obj_field_int32_get(a2, OBJ_F_ITEM_FLAGS) & OIF_CAN_USE_BOX) == 0) {
        return 6;
    }

    return 0;
}

// 0x462CC0
void sub_462CC0()
{
    // TODO: Incomplete.
}

// 0x462FC0
void sub_462FC0()
{
    // TODO: Incomplete.
}

// 0x463110
void sub_463110()
{
    // TODO: Incomplete.
}

// 0x4631A0
void sub_4631A0()
{
    // TODO: Incomplete.
}

// 0x463240
void sub_463240()
{
    // TODO: Incomplete.
}

// 0x463340
void sub_463340()
{
    // TODO: Incomplete.
}

// 0x463370
void sub_463370()
{
    // TODO: Incomplete.
}

// 0x463540
void sub_463540()
{
    // TODO: Incomplete.
}

// 0x463630
void sub_463630()
{
    // TODO: Incomplete.
}

// 0x463730
void sub_463730()
{
    // TODO: Incomplete.
}

// 0x463860
void sub_463860()
{
    // TODO: Incomplete.
}

// 0x4639E0
void sub_4639E0()
{
    // TODO: Incomplete.
}

// 0x463B30
void sub_463B30()
{
    // TODO: Incomplete.
}

// 0x463C60
void sub_463C60()
{
    // TODO: Incomplete.
}

// 0x463E20
void sub_463E20()
{
    // TODO: Incomplete.
}

// 0x4640C0
void sub_4640C0(int64_t obj)
{
    int64_t inven_source_obj;
    TimeEvent timeevent;
    DateTime datetime;
    int ms;

    inven_source_obj = item_inventory_source(obj);
    if (inven_source_obj != NULL) {
        qword_5E8818 = obj;
        timeevent_clear_all_ex(TIMEEVENT_TYPE_NPC_RESPAWN, sub_464150);
        timeevent.type = TIMEEVENT_TYPE_NPC_RESPAWN;
        timeevent.params[0].object_value = obj;

        ms = random_between(43200000, 86400000);
        if ((tig_net_flags & TIG_NET_CONNECTED) != 0) {
            ms /= 4;
        }

        sub_45A950(&datetime, ms);
        sub_45B800(&timeevent, &datetime);
    }
}

// 0x464150
bool sub_464150(TimeEvent* timeevent)
{
    return timeevent != NULL && timeevent->params[0].object_value == qword_5E8818;
}

// 0x464180
bool npc_respawn_timevent_process(TimeEvent* timeevent)
{
    sub_463C60(timeevent->params[0].object_value);
    return true;
}

// 0x4641A0
int item_inventory_source(object_id_t obj)
{
    if (obj != OBJ_HANDLE_NULL) {
        switch (obj_field_int32_get(obj, OBJ_F_TYPE)) {
        case OBJ_TYPE_CONTAINER:
            return obj_field_int32_get(obj, OBJ_F_CONTAINER_INVENTORY_SOURCE);
        case OBJ_TYPE_NPC:
            return obj_field_int32_get(obj, OBJ_F_CRITTER_INVENTORY_SOURCE);
        }
    }
    return 0;
}

// 0x4642C0
bool sub_4642C0(int64_t obj, int64_t item_obj)
{
    int obj_type;
    int inventory_num_fld;
    int inventory_list_fld;
    int index;
    int cnt;

    if (!sub_4E5470(obj)) {
        return false;
    }

    obj_type = obj_field_int32_get(obj, OBJ_F_TYPE);
    if (obj_type == OBJ_TYPE_CONTAINER) {
        inventory_num_fld = OBJ_F_CONTAINER_INVENTORY_NUM;
        inventory_list_fld = OBJ_F_CONTAINER_INVENTORY_LIST_IDX;
    } else if (obj_type_is_critter(obj_type)) {
        inventory_num_fld = OBJ_F_CRITTER_INVENTORY_NUM;
        inventory_list_fld = OBJ_F_CRITTER_INVENTORY_LIST_IDX;
    } else {
        return false;
    }

    cnt = obj_field_int32_get(obj, inventory_num_fld);
    for (index = 0; index < cnt; index++) {
        if (obj_arrayfield_handle_get(obj, inventory_list_fld, index) == item_obj) {
            return true;
        }
    }

    return false;
}

// 0x464370
bool item_is_identified(int64_t obj)
{
    if (obj == OBJ_HANDLE_NULL
        || !obj_type_is_item(obj_field_int32_get(obj, OBJ_F_TYPE))) {
        return false;
    }

    return obj_field_int32_get(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY) <= 0
        || (obj_field_int32_get(obj, OBJ_F_ITEM_FLAGS) & OIF_IDENTIFIED) != 0;
}

// 0x4643D0
void item_identify_all(int64_t obj)
{
    int obj_type;
    int inventory_num_fld;
    int inventory_list_fld;
    int index;
    int cnt;
    int64_t item_obj;
    unsigned int flags;

    obj_type = obj_field_int32_get(obj, OBJ_F_TYPE);
    if (obj_type == OBJ_TYPE_CONTAINER) {
        inventory_num_fld = OBJ_F_CONTAINER_INVENTORY_NUM;
        inventory_list_fld = OBJ_F_CONTAINER_INVENTORY_LIST_IDX;
    } else if (obj_type_is_critter(obj_type)) {
        inventory_num_fld = OBJ_F_CRITTER_INVENTORY_NUM;
        inventory_list_fld = OBJ_F_CRITTER_INVENTORY_LIST_IDX;
    } else {
        return;
    }

    cnt = obj_field_int32_get(obj, inventory_num_fld);
    for (index = 0; index < cnt; index++) {
        item_obj = obj_arrayfield_handle_get(obj, inventory_list_fld, index);
        flags = obj_field_int32_get(item_obj, OBJ_F_ITEM_FLAGS);
        flags |= OIF_IDENTIFIED;
        obj_field_int32_set(item_obj, OBJ_F_ITEM_FLAGS, flags);
    }

    return false;
}

// 0x464780
int item_gold_get(int64_t obj)
{
    if (obj_field_int32_get(obj, OBJ_F_TYPE) != OBJ_TYPE_ITEM_GOLD) {
        obj = item_gold_obj(obj);
    }

    if (obj != OBJ_HANDLE_NULL) {
        return obj_field_int32_get(obj, OBJ_F_GOLD_QUANTITY);
    }

    return 0;
}

// 0x4647D0
int64_t item_gold_obj(int64_t obj)
{
    int type;

    type = obj_field_int32_get(obj, OBJ_F_TYPE);
    if (obj_type_is_critter(type)) {
        return obj_field_handle_get(obj, OBJ_F_CRITTER_GOLD);
    }
    if (type == OBJ_TYPE_CONTAINER) {
        return sub_462540(obj, 9056, 0);
    }

    return OBJ_HANDLE_NULL;
}

// 0x464970
int64_t item_gold_set(int amount, int64_t obj)
{
    int64_t gold_obj;

    if (sub_4ED8B0(9056, obj, &gold_obj)) {
        sub_4EFDD0(obj, OBJ_F_GOLD_QUANTITY, amount);
    }

    return gold_obj;
}

// 0x4649C0
int64_t item_wield_get(int64_t obj, int inventory_location)
{
    int cnt;
    int index;
    int64_t item_obj;
    bool validated = false;

    cnt = obj_field_int32_get(obj, OBJ_F_CRITTER_INVENTORY_NUM);
    for (index = 0; index < cnt; index++) {
        item_obj = obj_arrayfield_handle_get(obj, OBJ_F_CRITTER_INVENTORY_LIST_IDX, index);
        if (item_obj == OBJ_HANDLE_NULL) {
            tig_debug_printf("item_wield_get: ERROR: inv_obj in slot #%d is NULL!\n", index);

            if (!validated) {
                if (!obj_validate_system(1)) {
                    tig_debug_printf("item_wield_get: obj_validate_system: ERROR: Failed to validate!\n");
                }
                validated = true;
                index = -1;
            }
        }

        if (!item_is_item(item_obj)) {
            tig_debug_printf("obj with d %d n %d contains object d %d n %d\n",
                obj_field_int32_get(obj, OBJ_F_DESCRIPTION),
                obj_field_int32_get(obj, OBJ_F_NAME),
                obj_field_int32_get(item_obj, OBJ_F_DESCRIPTION),
                obj_field_int32_get(item_obj, OBJ_F_NAME));
        }

        if (item_inventory_location_get(item_obj) == inventory_location) {
            return item_obj;
        }
    }

    return OBJ_HANDLE_NULL;
}

// 0x464AE0
bool item_wield_set(int64_t item_obj, int inventory_location)
{
    int64_t owner_obj;
    int64_t cur_item_obj;
    int saved_inventory_location;
    int v1;

    item_parent(item_obj, &owner_obj);

    cur_item_obj = item_wield_get(owner_obj, inventory_location);
    if (cur_item_obj != item_obj) {
        if (sub_464D20(item_obj, inventory_location, owner_obj) != ITEM_CANNOT_OK) {
            return false;
        }

        if (sub_466DA0(item_obj) != ITEM_CANNOT_OK) {
            return false;
        }

        if (cur_item_obj != OBJ_HANDLE_NULL) {
            if (sub_466DA0(cur_item_obj) != ITEM_CANNOT_OK) {
                return false;
            }

            saved_inventory_location = item_inventory_location_get(item_obj);
            item_remove(item_obj);
            item_remove(cur_item_obj);

            if (sub_466510(cur_item_obj, owner_obj, &v1) != ITEM_CANNOT_OK) {
                item_insert(item_obj, owner_obj, saved_inventory_location);
                item_insert(cur_item_obj, owner_obj, inventory_location);
                return false;
            }

            item_insert(cur_item_obj, owner_obj, v1);
        } else {
            item_remove(item_obj);
        }

        item_insert(item_obj, owner_obj, inventory_location);

        if (inventory_location == 1003) {
            sub_436FA0(item_obj);
        }
    }

    return true;
}

// 0x464C50
bool sub_464C50(int64_t obj, int inventory_location)
{
    int64_t item_obj;

    item_obj = item_wield_get(obj, inventory_location);
    if (item_obj != OBJ_HANDLE_NULL) {
        return sub_464C80(item_obj);
    } else {
        return true;
    }
}

// 0x464C80
bool sub_464C80(int64_t item_obj)
{
    int inventory_location;
    int64_t owner_obj;
    int v1;

    inventory_location = item_inventory_location_get(item_obj);
    if (inventory_location >= 1000 && inventory_location <= 1008) {
        if (sub_466DA0(item_obj) != ITEM_CANNOT_OK) {
            return false;
        }

        item_parent(item_obj, owner_obj);
        if (sub_466510(item_obj, owner_obj, &v1) != ITEM_CANNOT_OK) {
            return false;
        }

        item_remove(item_obj);
        item_insert(item_obj, owner_obj, v1);
    }

    return true;
}

// 0x465010
int sub_465010(int64_t obj)
{
    return ITEM_CANNOT_OK;
}

// 0x4654F0
void sub_4654F0(int64_t a, int64_t b)
{
    int index;

    for (index = 1000; index <= 1008; index++) {
        sub_465170(a, index, b);
    }
}

// 0x465530
void sub_465530(int64_t obj)
{
    int index;
    int64_t item_obj;

    for (index = 1000; index <= 1008; index++) {
        item_obj = item_wield_get(obj, index);
        if (item_obj != OBJ_HANDLE_NULL) {
            dword_5E87E8 = true;
            sub_467E80(item_obj, obj);
            sub_441980(obj, item_obj, OBJ_HANDLE_NULL, 25, 0);
            sub_441980(obj, item_obj, OBJ_HANDLE_NULL, 24, 0);
            dword_5E87E8 = false;
        }
    }
}

// 0x4655C0
bool sub_4655C0()
{
    return dword_5E87E8;
}

// 0x4655D0
int item_location_get(int64_t obj)
{
    int type;

    type = obj_field_int32_get(obj, OBJ_F_TYPE);
    if (type == OBJ_TYPE_ITEM_ARMOR) {
        switch (item_armor_coverage(obj)) {
        case TIG_ART_ARMOR_COVERAGE_HELMET:
            return 1000;
        case TIG_ART_ARMOR_COVERAGE_RING:
            return 1001;
        case TIG_ART_ARMOR_COVERAGE_MEDALLION:
            return 1003;
        case TIG_ART_ARMOR_COVERAGE_SHIELD:
            return 1005;
        case TIG_ART_ARMOR_COVERAGE_TORSO:
            return 1006;
        case TIG_ART_ARMOR_COVERAGE_GAUNTLETS:
            return 1007;
        case TIG_ART_ARMOR_COVERAGE_BOOTS:
            return 1008;
        }
    } else {
        if (type == OBJ_TYPE_WEAPON) {
            return 1004;
        }
        if (type == OBJ_TYPE_ITEM_GENERIC
            && (obj_field_int32_get(obj, OBJ_F_GENERIC_FLAGS) & OGF_USES_TORCH_SHIELD_LOCATION) != 0) {
            return 1005;
        }
    }
    return -1;
}

// 0x465690
int64_t sub_465690(int64_t obj, int inventory_location)
{
    int cnt;
    int index;
    int64_t item_obj;

    cnt = obj_field_int32_get(obj, OBJ_F_CRITTER_INVENTORY_NUM);
    for (index = 0; index < cnt; index++) {
        item_obj = obj_arrayfield_handle_get(obj, OBJ_F_CRITTER_INVENTORY_LIST_IDX, index);
        if (!item_is_item(item_obj)) {
            tig_debug_printf("obj with d %d n %d contains object d %d n %d\n",
                obj_field_int32_get(obj, OBJ_F_DESCRIPTION),
                obj_field_int32_get(obj, OBJ_F_NAME),
                obj_field_int32_get(item_obj, OBJ_F_DESCRIPTION),
                obj_field_int32_get(item_obj, OBJ_F_NAME));
        }

        if (item_inventory_location_get(item_obj) == inventory_location) {
            return item_obj;
        }
    }

    return OBJ_HANDLE_NULL;
}

// 0x465760
void item_location_set(int64_t obj, int location)
{
    int64_t owner_obj;

    item_parent(obj, &owner_obj);
    item_remove(obj);
    item_insert(obj, owner_obj, location);
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
int item_ammo_quantity_get(object_id_t obj, int ammo_type)
{
    object_id_t ammo_obj;

    if (obj_field_int32_get(obj, OBJ_F_TYPE) == OBJ_TYPE_AMMO) {
        ammo_obj = obj;
    } else {
        ammo_obj = item_ammo_obj(obj, ammo_type);
    }

    if (ammo_obj != OBJ_HANDLE_NULL) {
        return obj_field_int32_get(ammo_obj, OBJ_F_AMMO_QUANTITY);
    } else {
        return 0;
    }
}

// 0x465870
int64_t item_ammo_obj(object_id_t obj, int ammo_type)
{
    switch (obj_field_int32_get(obj, OBJ_F_TYPE)) {
    case OBJ_TYPE_PC:
    case OBJ_TYPE_NPC:
        return obj_field_handle_get(obj, dword_5B32A0[ammo_type]);
    case OBJ_TYPE_CONTAINER:
        return sub_462540(obj, dword_5B32B0[ammo_type], 0);
    default:
        return OBJ_HANDLE_NULL;
    }
}

// 0x465A40
int64_t item_ammo_quantity_set(int quantity, int ammo_type, int64_t obj)
{
    int64_t ammo_obj;

    if (sub_4ED8B0(dword_5B32B0[ammo_type], obj, &ammo_obj)) {
        sub_4EFDD0(ammo_obj, OBJ_F_AMMO_QUANTITY, quantity);
    }

    return ammo_obj;
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

// 0x465AE0
bool sub_465AE0(int64_t a1, int64_t a2, tig_art_id_t* art_id_ptr)
{
    tig_art_id_t current_aid;
    tig_art_id_t use_aid_fragment;
    int subtype;
    tig_art_id_t aid;

    if (a1 != OBJ_HANDLE_NULL) {
        current_aid = obj_field_int32_get(a2, OBJ_F_CURRENT_AID);
        use_aid_fragment = obj_field_int32_get(a1, OBJ_F_ITEM_USE_AID_FRAGMENT);
        subtype = tig_art_item_id_subtype_get(use_aid_fragment);
        aid = sub_504060(current_aid, subtype);
        if (sub_504030(aid) != subtype) {
            return false;
        }

        *art_id_ptr = tig_art_id_palette_set(aid, tig_art_id_palette_get(use_aid_fragment));
        return true;
    }

    current_aid = obj_field_int32_get(a2, OBJ_F_CURRENT_AID);
    use_aid_fragment = obj_field_int32_get(a2, OBJ_F_AID);
    aid = sub_504060(current_aid, 0);
    *art_id_ptr = tig_art_id_palette_set(aid, tig_art_id_palette_get(use_aid_fragment));
    return true;
}

// 0x465BA0
int item_armor_ac_adj(int64_t item_obj, int64_t owner_obj, bool a3)
{
    int ac_adj;
    int magic_ac_adj;

    if (obj_field_int32_get(item_obj, OBJ_F_TYPE) != OBJ_TYPE_ITEM_ARMOR) {
        return 0;
    }

    ac_adj = obj_field_int32_get(item_obj, OBJ_F_ARMOR_AC_ADJ);

    if (!a3
        || -obj_field_int32_get(item_obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY) > 0
        || item_is_identified(item_obj)) {
        magic_ac_adj = obj_field_int32_get(item_obj, OBJ_F_ARMOR_MAGIC_AC_ADJ);
        if (owner_obj != OBJ_HANDLE_NULL) {
            magic_ac_adj = sub_461590(item_obj, owner_obj, magic_ac_adj);
        }
        ac_adj += magic_ac_adj;
    }

    return ac_adj;
}

// 0x465C30
int item_armor_coverage(int64_t obj)
{
    tig_art_id_t inv_aid;

    if (obj == OBJ_HANDLE_NULL
        || !item_is_item(obj)
        || obj_field_int32_get(obj, OBJ_F_TYPE) != OBJ_TYPE_ITEM_ARMOR) {
        return -1;
    }

    inv_aid = obj_field_int32_get(obj, OBJ_F_ITEM_INV_AID);
    return tig_art_item_id_armor_coverage_get(inv_aid);
}

// 0x465C90
int sub_465C90(int race)
{
    return dword_5B32C0[race];
}

// 0x465CA0
int item_weapon_ammo_type(object_id_t item_id)
{
    if (item_id == OBJ_HANDLE_NULL
        || obj_field_int32_get(item_id, OBJ_F_TYPE) != OBJ_TYPE_WEAPON) {
        return 10000;
    }

    return obj_field_int32_get(item_id, OBJ_F_WEAPON_AMMO_TYPE);
}

// 0x465CF0
int item_weapon_magic_speed(int64_t item_obj, int64_t owner_obj)
{
    int speed_adj;
    int skill;
    int training;

    if (item_obj == OBJ_HANDLE_NULL) {
        return 10;
    }

    speed_adj = obj_field_int32_get(item_obj, OBJ_F_WEAPON_MAGIC_SPEED_ADJ);

    if (owner_obj != OBJ_HANDLE_NULL) {
        speed_adj = sub_461590(item_obj, owner_obj, speed_adj);
        skill = item_weapon_skill(item_obj);
        if (IS_TECH_SKILL(skill)) {
            training = tech_skill_get_training(owner_obj, GET_TECH_SKILL(skill));
        } else {
            training = basic_skill_get_training(owner_obj, GET_BASIC_SKILL(skill));
        }
        if (training >= TRAINING_APPRENTICE) {
            speed_adj += 5;
        }
    }

    return speed_adj + obj_field_int32_get(item_obj, OBJ_F_WEAPON_SPEED_FACTOR);
}

// 0x465D80
int item_weapon_skill(int64_t obj)
{
    int tech_complexity;
    int ammo_type;

    if (obj == OBJ_HANDLE_NULL) {
        return SKILL_MELEE;
    }

    if (obj_field_int32_get(obj, OBJ_F_TYPE) != OBJ_TYPE_WEAPON) {
        return SKILL_THROWING;
    }

    if ((obj_field_int32_get(obj, OBJ_F_WEAPON_FLAGS) & OWF_BOOMERANGS) != 0) {
        return SKILL_THROWING;
    }

    tech_complexity = -obj_field_int32_get(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY);
    ammo_type = item_weapon_ammo_type(obj);
    if (tech_complexity > 0
        && obj_field_int32_get(obj, OBJ_F_WEAPON_RANGE) >= 3
        && (ammo_type == AMMUNITION_TYPE_BULLET
            || ammo_type == AMMUNITION_TYPE_FUEL
            || ammo_type == AMMUNITION_TYPE_BATTERY)) {
        return SKILL_FIREARMS;
    }

    if (ammo_type == AMMUNITION_TYPE_ARROW) {
        return SKILL_BOW;
    }

    return SKILL_MELEE;
}

// 0x465E30
int item_weapon_range(object_id_t item_id, object_id_t critter_id)
{
    int magic_range_adj;

    if (item_id == OBJ_HANDLE_NULL
        || obj_field_int32_get(item_id, OBJ_F_TYPE) != OBJ_TYPE_WEAPON) {
        return 1;
    }

    magic_range_adj = obj_field_int32_get(item_id, OBJ_F_WEAPON_MAGIC_RANGE_ADJ);
    return obj_field_int32_get(item_id, OBJ_F_WEAPON_RANGE) + sub_461590(item_id, critter_id, magic_range_adj);
}

// 0x466230
int sub_466230(int64_t obj)
{
    if (obj_field_int32_get(obj, OBJ_F_ITEM_SPELL_1) != 10000) {
        return 0;
    }

    return sub_4502B0(sub_4CB790(10000));
}

// 0x466A00
bool sub_466A00(int64_t a1, int64_t key_obj)
{
    int64_t key_ring_obj;

    key_ring_obj = sub_4631A0(a1);
    if (key_ring_obj == OBJ_HANDLE_NULL) {
        return false;
    }

    sub_466A50(key_obj, key_ring_obj);
    sub_466BD0(key_ring_obj);

    return true;
}

// 0x466A50
void sub_466A50(int64_t key_obj, int64_t key_ring_obj)
{
    int index;
    int key_id;

    index = obj_arrayfield_length_get(key_ring_obj, OBJ_F_KEY_RING_LIST_IDX);
    key_id = obj_field_int32_get(key_obj, OBJ_F_KEY_KEY_ID);
    sub_4074E0(key_ring_obj,
        OBJ_F_KEY_RING_LIST_IDX,
        index,
        key_id);
    sub_43CCA0(key_obj);
}

// 0x466BD0
void sub_466BD0(int64_t key_ring_obj)
{
    tig_art_id_t aid;

    if (obj_arrayfield_length_get(key_ring_obj, OBJ_F_KEY_RING_LIST_IDX) != 0) {
        tig_art_item_id_create(0, 1, 0, 0, 0, 7, 0, 0, &aid);
    } else {
        tig_art_item_id_create(1, 1, 0, 0, 0, 7, 0, 0, &aid);
    }

    obj_field_int32_set(key_ring_obj, OBJ_F_ITEM_INV_AID, aid);
}

// 0x466CF0
bool item_insert_failure(ItemInsertInfo* item_insert_info)
{
    char name[256];
    unsigned int flags;

    sub_441B60(item_insert_info->field_0, item_insert_info->field_0, name);
    tig_debug_printf("MP: Item: item_insert_failure: removing from %s OIF_MP_INSERTED.\n", name);
    flags = obj_field_int32_get(item_insert_info->field_0, OBJ_F_ITEM_FLAGS);
    flags |= OIF_MP_INSERTED;
    obj_field_int32_set(item_insert_info->field_0, OBJ_F_ITEM_FLAGS, flags);
    FREE(item_insert_info);

    return true;
}

// 0x466D60
void sub_466D60(int64_t obj)
{
    if (obj == OBJ_HANDLE_NULL) {
        return;
    }

    sub_436FA0(obj);

    if (player_is_pc_obj(obj)) {
        sub_4605D0();
    }

    sub_4601F0(obj);
}

// 0x466DA0
int sub_466DA0(int64_t obj)
{
    if ((obj_field_int32_get(obj, OBJ_F_ITEM_FLAGS) & OIF_NO_DROP) != 0) {
        return ITEM_CANNOT_NOT_DROPPABLE;
    }

    if (item_inventory_location_get(obj) < 1000 || item_inventory_location_get(obj) > 1008) {
        return ITEM_CANNOT_OK;
    }

    return sub_465010(obj);
}

// 0x466E00
void item_remove(int64_t obj)
{
    int64_t owner_obj;

    if (!item_parent(obj, &owner_obj)) {
        tig_debug_printf("Warning: item_remove called on item that doesn't think it has a parent.");
        return;
    }

    item_force_remove(owner_obj, obj);
}

// 0x4673B0
const char* item_cannot_msg(int reason)
{
    MesFileEntry mes_file_entry;

    if (reason <= 0) {
        return NULL;
    }

    mes_file_entry.num = reason + 100;
    mes_get_msg(item_mes_file, &mes_file_entry);

    return mes_file_entry.str;
}

// 0x4673F0
void sub_4673F0(int64_t obj, int reason)
{
    John v1;
    const char* str;

    if (player_is_pc_obj(obj)) {
        str = item_cannot_msg(reason);
        if (str != NULL) {
            v1.type = 4;
            v1.str = str;
            sub_460630(&v1);
        }
    }
}

// 0x467440
void sub_467440(int64_t a1, int64_t a2, int64_t a3, int a4)
{
    unsigned int flags;
    Packet125 pkt;

    if ((tig_net_flags & TIG_NET_CONNECTED) == 0
        || (tig_net_flags & TIG_NET_HOST) != 0) {
        flags = obj_field_int32_get(a1, OBJ_F_ITEM_FLAGS);
        flags |= OIF_IDENTIFIED;
        obj_field_int32_set(a1, OBJ_F_ITEM_FLAGS, flags);
        sub_464830(a3, a2, a4, OBJ_HANDLE_NULL);
        sub_4EE3A0(a3, a1);
    } else {
        pkt.type = 125;
        sub_4F0640(a1, &(pkt.field_8));
        sub_4F0640(a2, &(pkt.field_20));
        sub_4F0640(a3, &(pkt.field_38));
        pkt.field_50 = a4;
        tig_net_send_app_all(&pkt, sizeof(pkt));
    }
}

// 0x4677B0
void sub_4677B0(int64_t a1, int64_t a2, int a3)
{
    tig_art_id_t aid;

    switch (a3) {
    case 1004:
    case 1005:
        aid = sub_465020(a2);
        object_set_current_aid(a2, aid);

        if (player_is_pc_obj(a2)) {
            sub_4605D0();
        }
        break;
    case 1006:
        if (sub_465AE0(a1, a2, &aid)) {
            object_set_current_aid(a2, aid);
        }
        break;
    }

    sub_4CB7D0(a1, a2);
    sub_467E80(a1, a2);
    sub_441980(a2, a1, OBJ_HANDLE_NULL, 25, 0);
}

// 0x467CB0
void sub_467CB0(int64_t a1, int64_t a2, int a3)
{
    tig_art_id_t aid;

    switch (a3) {
    case 1004:
    case 1005:
        aid = sub_465020(a2);
        object_set_current_aid(a2, aid);

        if (player_is_pc_obj(a2)) {
            sub_4605D0();
        }
        break;
    case 1006:
        if (sub_465AE0(OBJ_HANDLE_NULL, a2, &aid)) {
            object_set_current_aid(a2, aid);
        }
        break;
    }

    sub_4CBFC0(a1, a2);
    sub_467E80(a1, a2);
    sub_441980(a2, a1, OBJ_HANDLE_NULL, 25, 0);
}

// 0x467E00
bool item_force_remove_failure(ItemRemoveInfo* item_remove_info)
{
    char name[256];
    unsigned int flags;

    sub_441B60(item_remove_info->field_0, item_remove_info->field_0, name);
    tig_debug_printf("MP: Item: item_force_remove_failure: adding to %s OIF_MP_INSERTED.\n", name);
    flags = obj_field_int32_get(item_remove_info->field_0, OBJ_F_ITEM_FLAGS);
    flags |= OIF_MP_INSERTED;
    obj_field_int32_set(item_remove_info->field_0, OBJ_F_ITEM_FLAGS, flags);
    FREE(item_remove_info);

    return true;
}

// 0x467E70
bool sub_467E70()
{
    bool ret;

    ret = dword_5E8820;
    dword_5E8820 = false;

    return ret;
}

// 0x467FC0
bool item_decay_timeevent_process(TimeEvent* timeevent)
{
    int64_t obj;

    obj = timeevent->params[0].object_value;
    if (dword_5E8800 > 0) {
        if (item_can_decay(obj)) {
            sub_43CCA0(obj);
        }
    } else {
        sub_468090(obj, 60000);
    }

    return true;
}

// 0x468010
bool item_can_decay(int64_t obj)
{
    int64_t owner_obj;

    if (obj == OBJ_HANDLE_NULL) {
        return false;
    }

    if (dword_5E87E4 != 0) {
        return false;
    }

    if ((obj_field_int32_get(obj, OBJ_F_ITEM_FLAGS) & OIF_NO_DECAY) != 0) {
        return false;
    }

    return !item_parent(obj, &owner_obj)
        || sub_49B290(owner_obj) == DESCRIPTION_JUNK_PILE;
}

// 0x468090
bool sub_468090(int64_t obj, int ms)
{
    TimeEvent timeevent;
    DateTime datetime;

    if (!item_can_decay(obj)) {
        return false;
    }

    qword_5E87F0 = obj;
    timeevent_clear_all_ex(TIMEEVENT_TYPE_ITEM_DECAY, sub_468150);

    timeevent.type = TIMEEVENT_TYPE_ITEM_DECAY;
    timeevent.params[0].object_value = obj;
    timeevent.params[1].integer_value = sub_45A7F0();
    sub_45A950(&datetime, ms);
    return sub_45B800(&timeevent, &datetime);
}

// 0x468110
bool sub_468110(int64_t obj)
{
    if (item_can_decay(obj)) {
        return false;
    }

    qword_5E87F0 = obj;
    timeevent_clear_all_ex(TIMEEVENT_TYPE_ITEM_DECAY, sub_468150);

    return true;
}

// 0x468150
bool sub_468150(TimeEvent* timeevent)
{
    return timeevent != NULL && timeevent->params[0].object_value == qword_5E87F0;
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
