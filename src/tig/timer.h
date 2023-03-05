#ifndef ARCANUM_TIG_TIMER_H_
#define ARCANUM_TIG_TIMER_H_

#include "tig/types.h"

int tig_timer_init(TigContext* ctx);
void tig_timer_exit();
int tig_timer_start(unsigned int* start);
unsigned int tig_timer_elapsed(unsigned int start);
int tig_timer_between(unsigned int start, unsigned int end);

#endif /* ARCANUM_TIG_TIMER_H_ */
