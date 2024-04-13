#include "tig/timer.h"

#include <limits.h>

// clang-format off
#include <windows.h>
#include <timeapi.h>
// clang-format on

#define RESOLUTION 1

// 0x52DF80
int tig_timer_init(TigInitializeInfo* init_info)
{
    (void)init_info;

    if (timeBeginPeriod(RESOLUTION) != TIMERR_NOERROR) {
        return TIG_ERR_16;
    }

    return TIG_OK;
}

// 0x52DF90
void tig_timer_exit()
{
    timeEndPeriod(RESOLUTION);
}

// 0x52DFA0
int tig_timer_start(timer_t* start)
{
    *start = timeGetTime();
    return TIG_OK;
}

// 0x52DFB0
duration_t tig_timer_elapsed(timer_t start)
{
    return timeGetTime() - start;
}

// 0x52DFC0
duration_t tig_timer_between(timer_t start, timer_t end)
{
    unsigned int diff = end - start;
    if (diff > INT_MAX) {
        diff = INT_MAX;
    }
    return diff;
}
