#ifndef ARCANUM_UI_CHAREDIT_UI_H_
#define ARCANUM_UI_CHAREDIT_UI_H_

#include "game/context.h"

bool charedit_init(GameInitInfo* init_info);
void charedit_exit();
void charedit_reset();
bool charedit_is_created();
void sub_55A230();
void sub_55F160();
void sub_55F180();
void sub_55F1A0();
void sub_55F1E0();
void sub_55F200(int type);

#endif /* ARCANUM_UI_CHAREDIT_UI_H_ */
