#ifndef ARCANUM_GAME_MT_ITEM_H_
#define ARCANUM_GAME_MT_ITEM_H_

#include "game/context.h"
#include "game/combat.h"

bool mt_item_init(GameInitInfo* init_info);
void mt_item_exit();
int mt_item_triggers(int index);
int sub_4CB790(int a1);
int mt_item_spell(int64_t obj_handle, int index);
void sub_4CB7D0(int64_t a1, int64_t a2);
void sub_4CBAA0(int64_t a1, int64_t a2);
void sub_4CBAD0(int64_t a1, CombatContext* combat, int64_t a3);
void sub_4CBB80(int64_t a1, int64_t a2);
void sub_4CBBF0(int64_t a1, int64_t a2);
void sub_4CBC60(int64_t a1, int64_t a2);
void sub_4CBD40(int64_t a1, int64_t a2);
void sub_4CBDB0(int64_t a1, int64_t a2, int64_t a3);
void sub_4CBE00(int64_t a1, int64_t a2);
void sub_4CBE70(int64_t a1, int64_t a2, int64_t a3);
void sub_4CBF70(int64_t a1, int64_t a2);
void sub_4CBFC0(int64_t a1, int64_t a2);
void sub_4CC130(int64_t a1, int64_t a2);
bool sub_4CC160(int64_t item_obj);

#endif /* ARCANUM_GAME_MT_ITEM_H_ */
