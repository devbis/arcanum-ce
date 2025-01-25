#ifndef ARCANUM_UI_CHAREDIT_UI_H_
#define ARCANUM_UI_CHAREDIT_UI_H_

#include "game/context.h"

bool charedit_init(GameInitInfo* init_info);
void charedit_exit();
void charedit_reset();
bool charedit_create(int64_t obj, int mode);
void charedit_destroy();
bool charedit_is_created();
void sub_55A230();
void charedit_error_not_enough_character_points();
void charedit_error_not_enough_level();
void charedit_error_not_enough_intelligence();
void charedit_error_not_enough_willpower();
void charedit_error_skill_at_max();
void charedit_error_not_enough_stat(int stat);
void charedit_error_skill_is_zero();
void charedit_error_skill_at_min();
void sub_55F360(int player);
void sub_55F450(int player, int type, int param);
void sub_55F5F0(int a1, int a2, int a3);

#endif /* ARCANUM_UI_CHAREDIT_UI_H_ */
