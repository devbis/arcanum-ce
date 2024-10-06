#ifndef ARCANUM_GAME_A_NAME_H_
#define ARCANUM_GAME_A_NAME_H_

#include <tig/tig.h>

bool a_name_tile_init();
void a_name_tile_exit();
bool a_name_tile_aid_to_fname(tig_art_id_t aid, char* fname);
bool sub_4EBA30(tig_art_id_t a, tig_art_id_t b);
bool sub_4EBAB0(tig_art_id_t aid);
bool sub_4EBB30(tig_art_id_t aid);
bool sub_4EBB80(tig_art_id_t aid);
bool sub_4EBBA0(tig_art_id_t aid);
bool sub_4EBBC0(tig_art_id_t aid);
int sub_4EBBE0(tig_art_id_t aid);
void sub_4EBC40();
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
int a_name_num_wall_structures();
char* a_name_wall_get_structure(int index);
bool a_name_light_init();
void a_name_light_exit();
bool a_name_light_aid_to_fname(tig_art_id_t aid, char* fname);
bool a_name_roof_init();
void a_name_roof_exit();
bool a_name_roof_aid_to_fname(tig_art_id_t aid, char* fname);
bool a_name_roof_fname_to_aid(const char* fname, tig_art_id_t* aid_ptr);

#endif /* ARCANUM_GAME_A_NAME_H_ */
