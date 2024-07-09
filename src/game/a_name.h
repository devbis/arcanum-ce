#ifndef ARCANUM_GAME_A_NAME_H_
#define ARCANUM_GAME_A_NAME_H_

#include <tig/tig.h>

bool a_name_tile_init();
void a_name_tile_exit();
bool a_name_tile_aid_to_fname(tig_art_id_t aid, char* fname);
bool a_name_item_init();
bool a_name_light_init();
void a_name_light_exit();
bool a_name_light_aid_to_fname(tig_art_id_t aid, char* fname);
bool a_name_roof_init();
void a_name_roof_exit();
bool a_name_roof_aid_to_fname(tig_art_id_t aid, char* fname);
bool a_name_roof_fname_to_aid(const char* fname, tig_art_id_t* aid_ptr);

#endif /* ARCANUM_GAME_A_NAME_H_ */
