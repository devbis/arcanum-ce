#ifndef ARCANUM_GAME_MT_ITEM_H_
#define ARCANUM_GAME_MT_ITEM_H_

#include "game/context.h"
#include "game/combat.h"

bool mt_item_init(GameInitInfo* init_info);
void mt_item_exit();
int mt_item_triggers(int index);
int sub_4CB790(int a1);
int mt_item_spell(int64_t obj_handle, int index);
void mt_item_notify_wear(int64_t item_obj, int64_t parent_obj);
void mt_item_notify_pickup(int64_t item_obj, int64_t parent_obj);
void mt_item_notify_parent_hit(int64_t a1, CombatContext* combat, int64_t a3);
void mt_item_notify_parent_stunned(int64_t a1, int64_t a2);
void mt_item_notify_parent_going_unconscious(int64_t a1, int64_t a2);
void mt_item_notify_parent_dying(int64_t a1, int64_t a2);
void mt_item_notify_parent_attacks_obj(int64_t a1, int64_t a2);
void sub_4CBDB0(int64_t a1, int64_t a2, int64_t a3);
void mt_item_notify_target_going_unconscious(int64_t a1, int64_t a2);
void sub_4CBE70(int64_t a1, int64_t a2, int64_t a3);
void sub_4CBF70(int64_t a1, int64_t a2);
void mt_item_notify_unwear(int64_t item_obj, int64_t parent_obj);
void sub_4CC130(int64_t a1, int64_t a2);
bool sub_4CC160(int64_t item_obj);

#endif /* ARCANUM_GAME_MT_ITEM_H_ */
