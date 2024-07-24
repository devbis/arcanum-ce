#ifndef ARCANUM_GAME_ITEM_H_
#define ARCANUM_GAME_ITEM_H_

#include "game/context.h"
#include "game/obj.h"

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

bool item_init(GameInitInfo* init_info);
void item_exit();
void item_resize(ResizeInfo *resize_info);
bool item_parent(object_id_t object_id, object_id_t* parent_object_id);
bool item_is_item(object_id_t object_id);
int item_throwing_distance(int64_t item_obj, int64_t critter_obj);
int sub_462C30(int64_t a1, int64_t a2);
bool npc_respawn_timevent_process(TimeEvent* timeevent);
bool item_is_identified(int64_t obj);
int item_gold_get(int64_t obj);
int64_t item_gold_set(int amount, int64_t obj);
void sub_4654F0(int64_t a, int64_t b);
void sub_465530(int64_t obj);
bool sub_4655C0();

#endif /* ARCANUM_GAME_ITEM_H_ */
