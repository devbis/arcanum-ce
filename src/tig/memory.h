#ifndef ARCANUM_TIG_MEMORY_H_
#define ARCANUM_TIG_MEMORY_H_

#include "tig/types.h"

typedef void(TigMemoryOutputFunc)(const char* string);

typedef enum TigMemoryPrintStatsOptions {
    TIG_MEMORY_STATS_PRINT_OVERHEAD = 1 << 0,
    TIG_MEMORY_STATS_PRINT_ALL_BLOCKS = 1 << 1,
    TIG_MEMORY_STATS_PRINT_GROUPED_BLOCKS = 1 << 2,
};

int tig_memory_init(TigContext* ctx);
void tig_memory_exit();
void tig_memory_set_output_func(TigMemoryOutputFunc* func);
void tig_memory_print_stats(unsigned int stats);
void tig_memory_get_system_status(size_t* total, size_t* available);

#endif /* ARCANUM_TIG_MEMORY_H_ */
