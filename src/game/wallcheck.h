#ifndef ARCANUM_GAME_WALLCHECK_H_
#define ARCANUM_GAME_WALLCHECK_H_

#include "game/context.h"

bool wallcheck_init(GameInitInfo* init_info);
void wallcheck_reset();
void wallcheck_exit();
void wallcheck_ping(tig_timestamp_t timestamp);
void sub_437E50(int64_t a1);
void wallcheck_flush();
void sub_438500(bool enabled);
bool sub_438520();
void sub_438530(int64_t obj);

#endif /* ARCANUM_GAME_WALLCHECK_H_ */
