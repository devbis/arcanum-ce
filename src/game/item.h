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
bool item_parent(object_id_t object_id, object_id_t* parent_object_id);
bool item_is_item(object_id_t object_id);
int item_total_weight(object_id_t obj);
int sub_461620(int64_t item_obj, int64_t owner_obj, int64_t a3);
int item_throwing_distance(int64_t item_obj, int64_t critter_obj);
int64_t item_find_by_name(int64_t obj, int name);
int64_t item_find_first_of_type(int64_t obj, int type);
int64_t item_find_first_generic(int64_t obj, unsigned int flags);
int sub_462C30(int64_t a1, int64_t a2);
bool npc_respawn_timevent_process(TimeEvent* timeevent);
bool item_is_identified(int64_t obj);
int item_gold_get(int64_t obj);
int64_t item_gold_set(int amount, int64_t obj);
void sub_4654F0(int64_t a, int64_t b);
void sub_465530(int64_t obj);
bool sub_4655C0();
int item_location_get(int64_t obj);
void item_location_set(int64_t obj, int location);
int item_ammo_quantity_get(object_id_t obj, int ammo_type);
int64_t item_ammo_quantity_set(int quantity, int ammo_type, int64_t obj);
int item_armor_ac_adj(int64_t item_obj, int64_t owner_obj, bool a3);
int item_armor_coverage(int64_t obj);
int sub_465C90(int race);
int item_weapon_ammo_type(object_id_t item_id);
int item_weapon_magic_speed(int64_t item_obj, int64_t owner_obj);
int item_weapon_skill(int64_t obj);
int item_weapon_range(object_id_t item_id, object_id_t critter_id);
int sub_466230(int64_t obj);
void sub_466D60(int64_t obj);
int sub_466DA0(int64_t obj);
void item_remove(int64_t obj);
bool item_decay_timeevent_process(TimeEvent* timeevent);
bool item_can_decay(int64_t obj);
bool sub_468090(int64_t obj, int ms);

#endif /* ARCANUM_GAME_ITEM_H_ */
