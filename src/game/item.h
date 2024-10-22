#ifndef ARCANUM_GAME_ITEM_H_
#define ARCANUM_GAME_ITEM_H_

#include "game/context.h"
#include "game/obj.h"
#include "game/timeevent.h"

typedef enum AmmunitionType {
    AMMUNITION_TYPE_ARROW,
    AMMUNITION_TYPE_BULLET,
    AMMUNITION_TYPE_BATTERY,
    AMMUNITION_TYPE_FUEL,
    AMMUNITION_TYPE_COUNT,
} AmmunitionType;

typedef enum ArmorCoverageType {
    ARMOR_COVERAGE_TYPE_TORSO,
    ARMOR_COVERAGE_TYPE_SHIELD,
    ARMOR_COVERAGE_TYPE_HELMET,
    ARMOR_COVERAGE_TYPE_GAUNTLETS,
    ARMOR_COVERAGE_TYPE_BOOTS,
    ARMOR_COVERAGE_TYPE_RING,
    ARMOR_COVERAGE_TYPE_MEDALLION,
    ARMOR_COVERAGE_TYPE_COUNT,
} ArmorCoverageType;

typedef enum ItemCannot {
    ITEM_CANNOT_OK,
    ITEM_CANNOT_TOO_HEAVY, // The item is too heavy.
    ITEM_CANNOT_NO_ROOM, // There is no room for that item.
    ITEM_CANNOT_WRONG_TYPE, // The item is the wrong type for that slot.
    ITEM_CANNOT_NO_FREE_HAND, // There is no free hand to use that item.
    ITEM_CANNOT_CRIPPLED, // A crippled arm prevents the wielding of that item.
    ITEM_CANNOT_NOT_USABLE, // The item cannot be used.
    ITEM_CANNOT_BROKEN, // The item is broken.
    ITEM_CANNOT_WRONG_WEARABLE_SIZE, // The clothing or armor is the wrong size and cannot be worn.
    ITEM_CANNOT_NOT_WIELDABLE, // The item cannot be wielded.
    ITEM_CANNOT_WRONG_WEARABLE_GENDER, // The clothing or armor is for the opposite gender and cannot be worn.
    ITEM_CANNOT_NOT_DROPPABLE, // The item cannot be dropped.
    ITEM_CANNOT_HEXED, // The item is hexed and cannot be unwielded.
    ITEM_CANNOT_DUMB, // The scroll requires an Intelligence of 5 or more to use.
    ITEM_CANNOT_PICKUP_MAGIC_ITEMS, // You cannot pick up magickal items.
    ITEM_CANNOT_PICKUP_TECH_ITEMS, // You cannot pick up technological items.
    ITEM_CANNOT_USE_MAGIC_ITEMS, // You cannot use magickal items.
    ITEM_CANNOT_USE_TECH_ITEMS, // You cannot use technological items.
    ITEM_CANNOT_WIELD_MAGIC_ITEMS, // You cannot wield magickal items.
    ITEM_CANNOT_WIELD_TECH_ITEMS, // You cannot wield technological items.
} ItemCannot;

bool item_init(GameInitInfo* init_info);
void item_exit();
void item_resize(ResizeInfo *resize_info);
void sub_460FF0(int64_t obj);
bool item_parent(object_id_t object_id, object_id_t* parent_object_id);
bool item_is_item(object_id_t object_id);
int item_inventory_location_get(object_id_t object_id);
int item_total_weight(object_id_t obj);
int item_magic_tech_complexity(object_id_t item_id);
int sub_461540(object_id_t item_id, object_id_t owner_id);
int sub_461590(object_id_t item_id, object_id_t owner_id, int a3);
int sub_461620(int64_t item_obj, int64_t owner_obj, int64_t a3);
void item_inv_icon_size(object_id_t item_id, int* width, int* height);
void sub_4617F0(int64_t a1, int64_t a2);
void sub_461810(int64_t a1, int64_t a2, int inventory_location);
bool sub_461AB0(int64_t a1, int a2);
int item_worth(object_id_t item_id);
bool sub_461F60(object_id_t item_id);
int sub_461F80(int64_t a1, int64_t a2, int64_t a3, int a4);
int item_throwing_distance(int64_t item_obj, int64_t critter_obj);
void sub_462330(int64_t item_obj, int index, int* min_damage, int* max_damage);
int sub_462410(object_id_t item_id, int* quantity_field_ptr);
int64_t item_find_by_name(int64_t obj, int name);
int64_t sub_462540(int64_t a1, int64_t a2, unsigned int flags);
int64_t item_find_first_of_type(int64_t obj, int type);
int64_t item_find_first_generic(int64_t obj, unsigned int flags);
int64_t item_find_first(object_id_t obj);
int item_get_all(object_id_t obj, object_id_t** list_ptr);
void sub_462920(object_id_t* list);
int64_t item_find_first_matching_prototype(int64_t obj, int64_t a2);
int64_t sub_462A30(int64_t obj, int64_t a2);
int item_count_items_matching_prototype(int64_t obj, int64_t a2);
int sub_462C30(int64_t a1, int64_t a2);
int sub_462CC0(int64_t a1, int64_t a2, int64_t a3);
bool sub_463370(int64_t obj, int key_id);
void sub_463860(int64_t obj, bool a2);
void sub_463E20(int64_t obj);
void sub_4640C0(int64_t obj);
bool npc_respawn_timevent_process(TimeEvent* timeevent);
int item_inventory_source(object_id_t obj);
bool sub_4642C0(int64_t obj, int64_t item_obj);
bool item_is_identified(int64_t obj);
void item_identify_all(int64_t obj);
void sub_464470(int64_t obj, int* a2, int* a3);
int item_total_attack(int64_t critter_obj);
int item_total_defence(int64_t critter_obj);
int item_gold_get(int64_t obj);
bool sub_464830(int64_t a1, int64_t a2, int a3, int64_t a4);
int64_t item_gold_set(int amount, int64_t obj);
int64_t item_wield_get(int64_t obj, int inventory_location);
bool item_wield_set(int64_t item_obj, int inventory_location);
bool sub_464C50(int64_t obj, int inventory_location);
bool sub_464C80(int64_t item_obj);
int sub_464D20(int64_t a1, int a2, int64_t a3);
void sub_465170(int64_t a1, int a2, int64_t a3);
void sub_4654F0(int64_t a, int64_t b);
void sub_465530(int64_t obj);
bool sub_4655C0();
int item_location_get(int64_t obj);
int64_t sub_465690(int64_t obj, int inventory_location);
void item_location_set(int64_t obj, int location);
int item_ammo_quantity_get(object_id_t obj, int ammo_type);
bool item_ammo_move(int64_t from_obj, int64_t to_obj, int qty, int ammo_type, int64_t ammo_obj);
int64_t item_ammo_create(int quantity, int ammo_type, int64_t obj);
int item_armor_ac_adj(int64_t item_obj, int64_t owner_obj, bool a3);
int item_armor_coverage(int64_t obj);
int sub_465C90(int race);
int item_weapon_ammo_type(object_id_t item_id);
int item_weapon_magic_speed(int64_t item_obj, int64_t owner_obj);
int item_weapon_skill(int64_t obj);
int item_weapon_range(object_id_t item_id, object_id_t critter_id);
int item_weapon_min_strength(int64_t item_obj, int64_t critter_obj);
void item_weapon_damage(int64_t weapon_obj, int64_t critter_obj, int damage_type, int skill, bool a5, int* min_dam_ptr, int* max_dam_ptr);
int sub_466230(int64_t obj);
void sub_466260(int64_t a1, void* a2);
bool sub_466510(int64_t a1, int64_t a2, int* a3);
void item_insert(int64_t a1, int64_t a2, int a3);
void sub_466D60(int64_t obj);
int sub_466DA0(int64_t obj);
void item_remove(int64_t obj);
void sub_466E50(int64_t a1, int64_t a2);
void sub_4670A0(int64_t a1, int a2);
void sub_4673F0(int64_t obj, int reason);
void sub_467440(int64_t a1, int64_t a2, int64_t a3, int a4);
void sub_467520(int64_t obj);
bool item_decay_timeevent_process(TimeEvent* timeevent);
bool item_can_decay(int64_t obj);
void item_force_remove(int64_t a1, int64_t a2);
bool sub_468090(int64_t obj, int ms);
void sub_468180();
void sub_468190();
int sub_4681A0();

#endif /* ARCANUM_GAME_ITEM_H_ */
