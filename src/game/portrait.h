#ifndef ARCANUM_GAME_PORTRAIT_H_
#define ARCANUM_GAME_PORTRAIT_H_

#include "game/context.h"

bool portait_init(GameInitInfo* init_info);
void portrait_exit();
void sub_4CE3C0(int num, char* path, int size);
void portrait_draw_native(int64_t obj, int num, tig_window_handle_t window_handle, int x, int y);
void portrait_draw_128x128(int64_t obj, int num, tig_window_handle_t window_handle, int x, int y);
void portrait_draw_32x32(int64_t obj, int num, tig_window_handle_t window_handle, int x, int y);
bool portrait_find_first(int64_t obj, int* portrait_ptr);
bool portrait_find_next(int64_t obj, int* portrait_ptr);
bool portarit_find_last(int64_t obj, int* portrait_ptr);
bool portrait_find_prev(int64_t obj, int* portrait_ptr);
int sub_4CEB80(int64_t obj);

#endif /* ARCANUM_GAME_PORTRAIT_H_ */
