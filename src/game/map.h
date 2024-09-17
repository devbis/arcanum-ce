#ifndef ARCANUM_GAME_MAP_H_
#define ARCANUM_GAME_MAP_H_

#include "game/context.h"

typedef enum MapType {
    MAP_TYPE_NONE,
    MAP_TYPE_START_MAP,
    MAP_TYPE_SHOPPING_MAP,
    MAP_TYPE_COUNT,
} MapType;

bool map_init(GameContext* ctx);
void map_reset();
void map_resize(ResizeContext* ctx);
bool map_mod_load();
void map_mod_unload();
void map_exit();
void map_ping(unsigned int time);
bool map_save(TigFile* stream);
bool map_load(LoadContext* ctx);
void map_update_view(ViewOptions* view_options);
bool map_get_name(int map, char** name);
int sub_40FF40();
int sub_40FF50(int map_type);
bool map_get_starting_location(int map, long long* x, long long* y);
bool map_get_area(int map, int* area);
bool map_get_worldmap(int map, int* worldmap);
bool map_is_clearing_objects();
void map_flush(unsigned int flags);
void sub_4101D0(int64_t location, int64_t a2);
int sub_410270();
void sub_410280(long long* location);
void sub_4102C0(char** name, char** folder);
void map_clear_objects();
void sub_4115D0(const char* name);
void map_enable_gender_check();
void map_gender_check();
bool map_list_info_load();
int map_list_info_find(const char* name);
bool map_list_info_set(int index, const char* name, long long x, long long y, bool is_start_map, int worldmap, int area);
bool map_list_info_add(const char* name, long long x, long long y, bool is_start_map);
bool map_list_info_add_internal(const char* name, long long x, long long y, bool is_start_map);

#endif /* ARCANUM_GAME_MAP_H_ */
