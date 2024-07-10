#ifndef ARCANUM_GAME_A_NAME_H_
#define ARCANUM_GAME_A_NAME_H_

#include <tig/tig.h>

bool a_name_tile_init();
void a_name_tile_exit();
bool a_name_tile_aid_to_fname(tig_art_id_t aid, char* fname);
bool a_name_item_init();
void a_name_item_exit();
bool a_name_item_aid_to_fname(tig_art_id_t aid, char* fname);
bool a_name_facade_init();
void a_name_facade_exit();
bool a_name_facade_aid_to_fname(tig_art_id_t aid, char* fname);
bool a_name_portal_init();
void a_name_portal_exit();
bool a_name_portal_aid_to_fname(tig_art_id_t aid, char* fname);
tig_art_id_t sub_4EC830(tig_art_id_t aid);
bool a_name_wall_init();
void a_name_wall_exit();
bool a_name_light_init();
void a_name_light_exit();
bool a_name_light_aid_to_fname(tig_art_id_t aid, char* fname);
bool a_name_roof_init();
void a_name_roof_exit();
bool a_name_roof_aid_to_fname(tig_art_id_t aid, char* fname);
bool a_name_roof_fname_to_aid(const char* fname, tig_art_id_t* aid_ptr);

#endif /* ARCANUM_GAME_A_NAME_H_ */
