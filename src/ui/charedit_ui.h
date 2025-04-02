#ifndef ARCANUM_UI_CHAREDIT_UI_H_
#define ARCANUM_UI_CHAREDIT_UI_H_

#include "game/context.h"

typedef enum ChareditMode {
    CHAREDIT_MODE_CREATE,
    CHAREDIT_MODE_ACTIVE,
    CHAREDIT_MODE_PASSIVE,
    CHAREDIT_MODE_3,
} ChareditMode;

bool charedit_init(GameInitInfo* init_info);
void charedit_exit();
void charedit_reset();
bool charedit_open(int64_t obj, ChareditMode mode);
void charedit_close();
bool charedit_is_created();
void charedit_refresh();
void charedit_error_not_enough_character_points();
void charedit_error_not_enough_level();
void charedit_error_not_enough_intelligence();
void charedit_error_not_enough_willpower();
void charedit_error_skill_at_max();
void charedit_error_not_enough_stat(int stat);
void charedit_error_skill_is_zero();
void charedit_error_skill_at_min();
void mp_charedit_cache_traits(int player);
void mp_charedit_trait_inc(int player, int type, int param);
void mp_charedit_trait_dec(int player, int type, int param);

#endif /* ARCANUM_UI_CHAREDIT_UI_H_ */
