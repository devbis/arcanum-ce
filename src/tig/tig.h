#ifndef ARCANUM_TIG_TIG_H_
#define ARCANUM_TIG_TIG_H_

#include "tig/types.h"

extern unsigned int tig_ping_time;

int tig_init(TigContext* ctx);
void tig_exit();
void tig_ping();
void sub_51F250();
const char* tig_get_executable(bool file_name_only);
void tig_set_active(bool is_active);
bool tig_get_active();

#endif /* ARCANUM_TIG_TIG_H_ */
