#include "tig/timer.h"

// 0x52DF80
int tig_timer_init(TigContext* ctx)
{
    if (timeBeginPeriod(1u) != TIMERR_NOERROR) {
        return 16;
    }

    return TIG_OK;
}

// 0x52DF90
void tig_timer_exit()
{
    timeEndPeriod(1u);
}

// 0x52DFA0
int tig_timer_start(unsigned int* start)
{
    *start = timeGetTime();
    return 0;
}

// 0x52DFB0
unsigned int tig_timer_elapsed(unsigned int start)
{
    return timeGetTime() - start;
}

// 0x52DFC0
int tig_timer_between(unsigned int start, unsigned int end)
{
    unsigned int diff = end - start;
    if (diff > INT_MAX) {
        diff = INT_MAX;
    }
    return diff;
}
