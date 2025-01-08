#ifndef ARCANUM_GAME_TILE_H_
#define ARCANUM_GAME_TILE_H_

#include "game/context.h"

bool tile_init(GameInitInfo* init_info);
void tile_exit();
void tile_resize(GameResizeInfo* resize_info);
bool tile_update_view(ViewOptions* view_options);
void tile_toggle_visibility();
void tile_render(UnknownContext* render_info);
int tile_id_from_loc(int64_t loc);
tig_art_id_t tile_art_id_at(int64_t loc);
bool sub_4D7110(int64_t loc, bool a2);
bool sub_4D7180(int64_t loc);
bool sub_4D71A0(int64_t loc);
bool sub_4D71C0(int64_t loc);
void sub_4D7430(int64_t loc);
tig_art_id_t sub_4D7480(tig_art_id_t art_id, int num2, bool flippable2, int a4);
void sub_4D7590(tig_art_id_t art_id, TigVideoBuffer* video_buffer);

#define TILE_X(tile) ((tile) & 0x3F)
#define TILE_Y(tile) (((tile) >> 6) & 0x3F)
#define TILE_MAKE(x, y) ((x) | (y << 6))

#endif /* ARCANUM_GAME_TILE_H_ */
