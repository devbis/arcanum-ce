#ifndef ARCANUM_GAME_LIGHT_SCHEME_H_
#define ARCANUM_GAME_LIGHT_SCHEME_H_

#include "game/context.h"

// Magic value that indicates that a map default lighting scheme (set with
// `light_scheme_set_map_default`) should be used.
#define LIGHT_SCHEME_MAP_DEFAULT 0

// Default lighting scheme used if no other lighting scheme is specified.
#define LIGHT_SCHEME_DEFAULT_LIGHTING 1

bool light_scheme_init(GameInitInfo* init_info);
void light_scheme_reset();
bool light_scheme_mod_load();
void light_scheme_mod_unload();
bool light_scheme_save(TigFile* stream);
bool light_scheme_load(GameLoadInfo* load_info);
bool light_scheme_set_map_default(int light_scheme);
int light_scheme_get_map_default();
bool light_scheme_set(int light_scheme, int hour);
int light_scheme_get();
bool light_scheme_set_hour(int hour);
int light_scheme_get_hour();
int light_scheme_is_changing();

#endif /* ARCANUM_GAME_LIGHT_SCHEME_H_ */
