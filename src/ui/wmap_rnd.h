#ifndef ARCANUM_UI_WMAP_RND_H_
#define ARCANUM_UI_WMAP_RND_H_

#include "game/context.h"

bool wmap_rnd_init(GameInitInfo* init_info);
void wmap_rnd_exit();
void wmap_rnd_reset();
void wmap_rnd_mod_load();
void wmap_rnd_mod_unload();
void sub_5589D0();

#endif /* ARCANUM_UI_WMAP_RND_H_ */
