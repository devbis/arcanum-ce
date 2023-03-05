#ifndef ARCANUM_TIG_DEBUG_H_
#define ARCANUM_TIG_DEBUG_H_

#include "tig/types.h"

typedef void(TigDebugFunc)(const char* string);

int tig_debug_init(TigContext* ctx);
void tig_debug_exit();
void tig_debug_printf(const char* format, ...);
void tig_debug_println(const char* string);
bool tig_debug_add_func(TigDebugFunc* func);
bool tig_debug_remove_func(TigDebugFunc* func);

#endif /* ARCANUM_TIG_DEBUG_H_ */
