#include "game/item.h"

#include "game/description.h"
#include "game/mes.h"
#include "game/mt_item.h"
#include "game/object.h"
#include "game/player.h"
#include "game/random.h"
#include "game/skill.h"
#include "game/stat.h"
#include "game/timeevent.h"

#define FIRST_AMMUNITION_TYPE_ID 0
#define FIRST_ARMOR_COVERAGE_TYPE_ID (FIRST_AMMUNITION_TYPE_ID + AMMUNITION_TYPE_COUNT)

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
        sub_4D43A0(item_mes_file, &msg);
        item_ammunition_type_names[index] = msg.str;
    }

    for (index = 0; index < ARMOR_COVERAGE_TYPE_COUNT; index++) {
        msg.num = index + FIRST_ARMOR_COVERAGE_TYPE_ID;
        sub_4D43A0(item_mes_file, &msg);
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
    if (object_id != 0) {
        int type = obj_field_int32_get(object_id, OBJ_F_TYPE);
        if (type >= OBJ_TYPE_WEAPON && type <= OBJ_TYPE_ITEM_GENERIC) {
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
int sub_461410(object_id_t obj)
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
    int complexity = item_magic_tech_complexity(item_id);

    int owner_type = obj_field_int32_get(owner_id, OBJ_F_TYPE);
    if (owner_type != OBJ_TYPE_PC && owner_type != OBJ_TYPE_NPC) {
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
    return obj_field_int32_get(item_id, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY);
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

// 0x461F20
int item_worth(object_id_t item_id)
{
    int worth;

    if (!sub_464370(item_id)) {
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

// 0x462230
bool sub_462230(int64_t a1, int64_t a2, int64_t a3)
{
    (void)a2;

    return sub_4C60C0(a3, PRIMARY_SKILL_HAGGLE) >= 3
        || ((obj_field_int32_get(a1, OBJ_F_ITEM_FLAGS) & OIF_WONT_SELL) == 0
            && (sub_461340(a1) < 1000 || sub_461340(a1) > 1008));
}

// 0x4622A0
int item_throwing_distance(int64_t item_obj, int64_t critter_obj)
{
    int distance;
    int weight;

    distance = 50 * stat_level(critter_obj, STAT_STRENGTH);
    if (sub_4C60C0(critter_obj, PRIMARY_SKILL_THROWING) >= 2) {
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

// 0x462820
int sub_462820(object_id_t item_id)
{
    int inventory_count_field;
    int inventory_list_field;

    if (obj_field_int32_get(item_id, OBJ_F_TYPE) == OBJ_TYPE_CONTAINER) {
        inventory_count_field = OBJ_F_CONTAINER_INVENTORY_NUM;
        inventory_list_field = OBJ_F_CONTAINER_INVENTORY_LIST_IDX;
    } else {
        inventory_count_field = OBJ_F_CRITTER_INVENTORY_NUM;
        inventory_list_field = OBJ_F_CRITTER_INVENTORY_LIST_IDX;
    }

    if (obj_field_int32_get(item_id, inventory_count_field) > 0) {
        return obj_arrayfield_handle_get(item_id, inventory_list_field, 0);
    } else {
        return 0;
    }
}

// 0x462880
int sub_462880(object_id_t obj, object_id_t** list_ptr)
{
    int inventory_count_field;
    int inventory_list_field;
    int index;
    int count;

    *list_ptr = NULL;

    if (obj_field_int32_get(obj, OBJ_F_TYPE) == OBJ_TYPE_CONTAINER) {
        inventory_count_field = OBJ_F_CONTAINER_INVENTORY_NUM;
        inventory_list_field = OBJ_F_CONTAINER_INVENTORY_LIST_IDX;
    } else {
        inventory_count_field = OBJ_F_CRITTER_INVENTORY_NUM;
        inventory_list_field = OBJ_F_CRITTER_INVENTORY_LIST_IDX;
    }

    count = obj_field_int32_get(obj, inventory_count_field);
    if (count > 0) {
        *list_ptr = (object_id_t*)MALLOC(sizeof(**list_ptr));
        for (index = 0; index < count; index++) {
            list_ptr[index] = obj_arrayfield_handle_get(obj, inventory_list_field, index);
        }
    }
    return count;
}

// 0x462920
void sub_462920(object_id_t* list)
{
    if (list != NULL) {
        FREE(list);
    }
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
    if (obj != 0) {
        switch (obj_field_int32_get(obj, OBJ_F_TYPE)) {
        case OBJ_TYPE_CONTAINER:
            return obj_field_int32_get(obj, OBJ_F_CONTAINER_INVENTORY_SOURCE);
        case OBJ_TYPE_NPC:
            return obj_field_int32_get(obj, OBJ_F_CRITTER_INVENTORY_SOURCE);
        }
    }
    return 0;
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
        switch (sub_465C30(obj)) {
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

// 0x466DA0
int sub_466DA0(int64_t obj)
{
    if ((obj_field_int32_get(obj, OBJ_F_ITEM_FLAGS) & OIF_NO_DROP) != 0) {
        return ITEM_CANNOT_NOT_DROPPABLE;
    }

    if (sub_461340(obj) < 1000 || sub_461340(obj) > 1008) {
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
void item_decay_timeevent_process(TimeEvent* timeevent)
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
