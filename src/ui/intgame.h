#ifndef ARCANUM_UI_INTGAME_H_
#define ARCANUM_UI_INTGAME_H_

#include "game/context.h"

bool intgame_init(GameInitInfo* init_info);
void intgame_reset();
void intgame_exit();
bool intgame_save(TigFile* stream);
bool intgame_load(GameLoadInfo* load_info);
bool iso_interface_create(tig_window_handle_t window_handle);
void iso_interface_destroy();

#endif /* ARCANUM_UI_INTGAME_H_ */
