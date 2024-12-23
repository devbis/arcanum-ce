#ifndef ARCANUM_UI_CHAREDIT_UI_H_
#define ARCANUM_UI_CHAREDIT_UI_H_

#include "game/context.h"
#include "game/ui.h"
#include "ui/types.h"

extern John stru_5C8990;
extern const char* dword_64D3C4[23];
extern bool charedit_created;

bool charedit_init(GameInitInfo* init_info);
void charedit_exit();
void charedit_reset();
bool charedit_create(int64_t obj, int a2);
void charedit_destroy();
bool charedit_is_created();
void sub_55A230();
void sub_55B0E0(bool a1);
void sub_55F160();
void sub_55F180();
void sub_55F1A0();
void sub_55F1E0();
void sub_55F200(int stat);
void sub_55F320();
void sub_55F340();
void sub_55F360(int player);
void sub_55F450(int player, int type, int param);
void sub_55F5F0(int a1, int a2, int a3);

#endif /* ARCANUM_UI_CHAREDIT_UI_H_ */
