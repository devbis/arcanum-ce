#ifndef ARCANUM_TIG_KB_H_
#define ARCANUM_TIG_KB_H_

#include "tig/types.h"

int tig_kb_init(TigContext* ctx);
void tig_kb_exit();
bool tig_kb_is_key_pressed(int key);
bool tig_kb_get_modifier(int key);
void tig_kb_ping();

#endif /* ARCANUM_TIG_KB_H_ */
