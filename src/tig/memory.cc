#include "tig/memory.h"

#include <stdarg.h>
#include <stdio.h>

#define MEMORY_BLOCK_HEADER_GUARD 0xAAAAAAAA
#define MEMORY_BLOCK_FOOTER_GUARD 0xBBBBBBBB

typedef struct TigMemoryBlock {
    void* data;
    size_t size;
    const char* file;
    int line;
    struct TigMemoryBlock* next;
    int guard;
};

static int tig_memory_sort_blocks(const void* a1, const void* a2);
static void tig_memory_fatal_error(const char* format, ...);

// 0x603DE8
static size_t tig_memory_max_overhead;

// 0x603DEC
static size_t tig_memory_max_blocks;

// 0x603DF0
static size_t tig_memory_current_overhead;

// 0x603DF4
static size_t tig_memory_max_allocated;

// 0x603DF8
static size_t tig_memory_current_allocated;

// 0x603DFC
static size_t tig_memory_current_blocks;

// 0x0603E00
static TigMemoryOutputFunc* tig_memory_output_func;

// 0x603E04
static char tig_memory_output_buffer[1024];

// 0x604204
static TigMemoryBlock* tig_memory_blocks;

// 0x604208
static bool tig_memory_initialized;

// 0x739F40
static CRITICAL_SECTION tig_memory_critical_section;

// 0x4FE380
int tig_memory_init(TigContext* ctx)
{
    InitializeCriticalSection(&tig_memory_critical_section);
    tig_memory_initialized = true;
    return TIG_OK;
}

// 0x4FE3A0
void tig_memory_exit()
{
    DeleteCriticalSection(&tig_memory_critical_section);
    tig_memory_initialized = false;
}

// 0x4FE790
void tig_memory_set_output_func(TigMemoryOutputFunc* func)
{
    tig_memory_output_func = func;
}

// 0x4FE7A0
void tig_memory_print_stats(unsigned int stats)
{
    if (tig_memory_output_func != NULL) {
        tig_memory_output_func("\n--- Dynamic memory usage: ---");

        if ((stats & TIG_MEMORY_STATS_PRINT_OVERHEAD) != 0) {
            sprintf(tig_memory_output_buffer,
                "Peak memory management overhead: %u bytes.",
                tig_memory_max_overhead);
            tig_memory_output_func(tig_memory_output_buffer);

            sprintf(tig_memory_output_buffer,
                "Current memory management overhead: %u bytes.",
                tig_memory_current_overhead);
            tig_memory_output_func(tig_memory_output_buffer);
        }

        sprintf(tig_memory_output_buffer,
            "Peak program memory usage: %u blocks, %u bytes.",
            tig_memory_max_blocks,
            tig_memory_max_allocated);
        tig_memory_output_func(tig_memory_output_buffer);

        sprintf(tig_memory_output_buffer,
            "Current program memory usage: %u blocks totaling %u bytes.",
            tig_memory_current_blocks,
            tig_memory_current_allocated);
        tig_memory_output_func(tig_memory_output_buffer);

        if ((stats & TIG_MEMORY_STATS_PRINT_ALL_BLOCKS) != 0) {
            TigMemoryBlock* curr = tig_memory_blocks;
            while (curr != NULL) {
                sprintf(tig_memory_output_buffer,
                    "    %s, line %d:  %u bytes at 0x%08x.",
                    curr->file,
                    curr->line,
                    curr->data);
                tig_memory_output_func(tig_memory_output_buffer);
                curr = curr->next;
            }
        } else if ((stats & TIG_MEMORY_STATS_PRINT_GROUPED_BLOCKS) != 0) {
            TigMemoryBlock** ptrs = (TigMemoryBlock**)malloc(sizeof(TigMemoryBlock**) * tig_memory_current_blocks);
            TigMemoryBlock** curr_ptr = ptrs;

            TigMemoryBlock* curr = tig_memory_blocks;
            while (curr != NULL) {
                *curr_ptr++ = curr;
                curr = curr->next;
            }

            qsort(ptrs, tig_memory_current_blocks, sizeof(ptrs), tig_memory_sort_blocks);

            size_t allocated = 0;
            size_t blocks = 0;
            for (int index = 0; index < tig_memory_current_blocks; index++) {
                allocated += ptrs[index]->size;
                blocks++;

                if (index == tig_memory_current_blocks - 1
                    || strcmpi(ptrs[index]->file, ptrs[index + 1]->file) != 0
                    || ptrs[index]->line != ptrs[index + 1]->line) {
                    sprintf(tig_memory_output_buffer,
                        "    %s, line %d:  %u blocks totaling %u bytes.",
                        ptrs[index]->file,
                        ptrs[index]->line,
                        blocks,
                        allocated);
                    allocated = 0;
                    blocks = 0;
                }
            }
        }
    }
}

// 0x4FE990
int tig_memory_sort_blocks(const void* a1, const void* a2)
{
    TigMemoryBlock* v1 = *(TigMemoryBlock**)a1;
    TigMemoryBlock* v2 = *(TigMemoryBlock**)a2;

    int cmp = strcmpi(v1->file, v2->file);
    if (cmp == 0) {
        cmp = v1->line - v2->line;
    }

    return cmp;
}

// 0x4FEA30
void tig_memory_get_system_status(size_t* total, size_t* available)
{
    MEMORYSTATUS memory_status;
    GlobalMemoryStatus(&memory_status);

    *total = memory_status.dwTotalPhys;
    *available = memory_status.dwAvailPhys;
}

// 0x4FEA60
void tig_memory_fatal_error(const char* format, ...)
{
    va_list args;
    va_start(args, format);

    if (tig_memory_output_func != NULL) {
        vsprintf(tig_memory_output_buffer, format, args);
        tig_memory_output_func(tig_memory_output_buffer);
    }

    va_end(args);

    exit(EXIT_FAILURE);
}
