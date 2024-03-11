#ifndef ARCANUM_TIG_TIMER_H_
#define ARCANUM_TIG_TIMER_H_

#include "tig/types.h"

#ifdef __cplusplus
extern "C" {
#endif

/// Represents time in milliseconds.
///
/// NOTE: On Windows this value represents number of milliseconds since the
/// system was started. On other platforms it might be something else (relative
/// to start of the game for example). In general the game does not rely on it's
/// meaning. Instead it is considered as an time point used to calculate
/// duration between two events.
typedef unsigned int timer_t;

/// Represents duration between two `timer_t` objects (in milliseconds).
///
/// NOTE: There is no consensus about signness of this type. The majority of
/// call sites use signed compares, however there are several places with
/// unsigned checks. One of the arguments for it to be signed is that
/// `tig_timer_between` caps duration at 0x7FFFFFFF (that is max 32 bit signed
/// int).
typedef int duration_t;

int tig_timer_init(TigContext* ctx);
void tig_timer_exit();
int tig_timer_start(timer_t* start);
duration_t tig_timer_elapsed(timer_t start);
duration_t tig_timer_between(timer_t start, timer_t end);

#ifdef __cplusplus
}
#endif

#endif /* ARCANUM_TIG_TIMER_H_ */
