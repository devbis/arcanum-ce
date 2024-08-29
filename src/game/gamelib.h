#ifndef ARCANUM_GAME_GAMELIB_H_
#define ARCANUM_GAME_GAMELIB_H_

#include "game/context.h"
#include "game/settings.h"
#include "tig/video.h"

typedef struct GameModuleList {
    unsigned int count;
    unsigned int selected;
    char** paths;
} GameModuleList;

extern unsigned int gamelib_ping_time;
extern Settings settings;
extern TigVideoBuffer* dword_739E7C;

bool gamelib_init(GameInitInfo* init_info);
void gamelib_reset();
void gamelib_resize(ResizeInfo* resize_info);
void gamelib_modlist_create(GameModuleList* module_list, int type);
void gamelib_modlist_destroy(GameModuleList* module_list);
bool gamelib_mod_guid(GUID* guid_ptr);
void sub_402D30(TigRect* rect);
bool sub_402E50();

#endif /* ARCANUM_GAME_GAMELIB_H_ */
