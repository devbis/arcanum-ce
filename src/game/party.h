#ifndef ARCANUM_GAME_PARTY_H_
#define ARCANUM_GAME_PARTY_H_

#include "game/context.h"

bool party_init(GameInitInfo* init_info);
void party_exit();
void party_reset();
bool party_save(TigFile* stream);
bool party_load(GameLoadInfo* load_info);
int64_t party_find_first(int64_t obj, int* index_ptr);
int64_t party_find_next(int* index_ptr);
int sub_4BA020(int64_t obj);
bool sub_4BA1E0(int64_t obj);
bool sub_4BA2E0(int* party, int cnt);

#endif /* ARCANUM_GAME_PARTY_H_ */
