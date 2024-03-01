#ifndef ARCANUM_TIG_DEBUG_H_
#define ARCANUM_TIG_DEBUG_H_

#include <stdbool.h>

#include "tig/types.h"

#ifdef __cplusplus
extern "C" {
#endif

int tig_debug_init(TigContext* ctx);
void tig_debug_exit();
void tig_debug_printf(const char* format, ...);
void tig_debug_println(const char* string);

#ifdef __cplusplus
}
#endif

#endif /* ARCANUM_TIG_DEBUG_H_ */
