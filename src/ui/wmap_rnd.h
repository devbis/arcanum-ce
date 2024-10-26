#ifndef ARCANUM_UI_WMAP_RND_H_
#define ARCANUM_UI_WMAP_RND_H_

#include "game/context.h"

bool wmap_rnd_init(GameInitInfo* init_info);
void wmap_rnd_exit();
void wmap_rnd_reset();
bool wmap_rnd_mod_load();
void wmap_rnd_mod_unload();
bool wmap_rnd_save(TigFile* stream);
bool wmap_rnd_load(GameLoadInfo* load_info);
void sub_5589D0();
bool sub_5595B0(int a1);
void sub_559640();

#endif /* ARCANUM_UI_WMAP_RND_H_ */
