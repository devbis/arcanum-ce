#ifndef TIG_CORE_H_
#define TIG_CORE_H_

#include "tig/types.h"

#ifdef __cplusplus
extern "C" {
#endif

extern unsigned int tig_ping_time;

int tig_init(TigContext* ctx);
void tig_exit(void);
void tig_ping();
void sub_51F250();
const char* tig_get_executable(bool file_name_only);
void tig_set_active(bool is_active);
bool tig_get_active();

#ifdef __cplusplus
}
#endif

#endif /* TIG_CORE_H_ */
