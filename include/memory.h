#ifndef MEMORY_H
#define MEMORY_H

#include <stdint.h>

typedef struct {
    uint64_t total_ram_kb;  //shows total physical ram in kb
    uint64_t free_ram_kb;
    uint64_t available_ram_kb;
    uint64_t used_ram_kb;
    double ram_usage_percent; // shows ram usage in percent 

    uint64_t total_swap_kb; // total configured swap space in kb
    uint64_t free_swap_kb;
    uint64_t used_swap_kb;
    double swap_usage_percent;  // shows swap usage in percent, 

    uint64_t uptime_seconds;  // Shows time since system boot in seconds

} MemoryStats;
int memory_get_stats(MemoryStats *stats);

#endif 