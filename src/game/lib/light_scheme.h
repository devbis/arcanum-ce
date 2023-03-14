#ifndef ARCANUM_GAME_LIB_LIGHT_SCHEME_H_
#define ARCANUM_GAME_LIB_LIGHT_SCHEME_H_

#include "game/context.h"
#include "tig/file.h"

#define LIGHT_SCHEME_MAX 24

bool light_scheme_init(GameContext* ctx);
void light_scheme_reset();
bool light_scheme_mod_load();
void light_scheme_mod_unload();
bool light_scheme_save(TigFile* stream);
bool light_scheme_load(LoadContext* ctx);
bool sub_4A7DA0(int a1);
int sub_4A7DC0();
bool sub_4A7DD0(int a1, int a2);
int sub_4A7EA0();
bool light_scheme_set_current(int a2);
int light_scheme_get_current();
int light_scheme_is_changing();

#endif /* ARCANUM_GAME_LIB_LIGHT_SCHEME_H_ */
