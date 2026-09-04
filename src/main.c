#include "memory.h"
#include <stdio.h>

 int main(void) {
    MemoryStats mem;

    if (memory_get_stats(&mem) != 0) {
        fprintf(stderr, "Error: Failed to read system memory.\n");
        return 1;
    }

    printf("Linux System Telementary\n");

    uint64_t days = mem.uptime_seconds / 86400;
    uint64_t hours = (mem.uptime_seconds % 86400) / 3600;
    uint64_t mins = (mem.uptime_seconds % 3600) / 60;
    uint64_t secs = mem.uptime_seconds % 60;

    printf("Uptime:      %lu days, %02lu:%02lu:%02lu\n", days, hours, mins, secs);

    printf("Total RAM:    %.2f GB\n", (double)mem.total_ram_kb / (1024.0*1024.0));
    printf("Used RAM :      %.2f GB (%.1f%%)\n", (double)mem.used_ram_kb / (1024.0*1024.0), mem.ram_usage_percent);
    printf("Free RAM:    %.2f GB\n", (double)mem.available_ram_kb / (1024.0 * 1024.0));
    printf("\n");
    if (mem.total_swap_kb > 0) {
        printf("Total Swap:  %.2f GB\n", (double)mem.total_swap_kb / (1024.0 * 1024.0));
        printf("Used Swap:   %.2f GB (%.1f%%)\n", 
               (double)mem.used_swap_kb / (1024.0 * 1024.0), 
               mem.swap_usage_percent);
    } else {
        printf("Swap:        Disabled / None\n");
    }
    printf("\n");
    return 0;
 }