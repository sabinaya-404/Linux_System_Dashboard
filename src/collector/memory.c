#include "memory.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

 int memory_get_stats(MemoryStats *stats) {
    // ensuring pointer is not null

    if (!stats) {
        return -1;
    }
 
 // zeroeing all fields so no garbage on memory

 memset(stats, 0, sizeof(MemoryStats));
 FILE *fp = fopen("/proc/meminfo", "r");
  if (!fp) {
    perror ("Failed to open /proc/meminfo");
    return -1;
  }
  char line[256];
  uint64_t value = 0;

  while(fgets(line, sizeof(line), fp)) {
    if (sscanf(line, "MemTotal: %lu kB", &value) == 1){
        stats->total_ram_kb = value;
    } else if (sscanf(line, "MemFree: %lu kB", &value) == 1) {
        stats->free_ram_kb = value;
    } else if (sscanf(line, "MemAvailable: %lu kB", &value) == 1) {
        stats->available_ram_kb = value;
    } else if (sscanf(line, "SwapTotal: %lu kB", &value) == 1) {
        stats->total_swap_kb = value;
    } else if (sscanf(line, "SwapFree: %lu kB", &value) == 1) {
        stats->free_swap_kb = value;
     }
  }

  fclose(fp);

   //RAM calculation logic
 if (stats->total_ram_kb > 0) {
        if (stats->available_ram_kb > 0) {
            stats->used_ram_kb = stats->total_ram_kb - stats->available_ram_kb;
        } else {
            // Fallback for old kernels 
            stats->used_ram_kb = stats->total_ram_kb - stats->free_ram_kb;
        }
        stats->ram_usage_percent = ((double)stats->used_ram_kb / (double)stats->total_ram_kb) * 100.0;
    }

    //Swap calculation
     if (stats->total_swap_kb > 0) {
        stats-> used_swap_kb = stats -> total_swap_kb - stats->free_swap_kb;
        stats->swap_usage_percent = ((double)stats->used_swap_kb / (double)stats->total_swap_kb) *100.0;
     } else {
        stats->used_swap_kb = 0;
        stats->swap_usage_percent = 0;
     }

     FILE *uptime_fp = fopen("/proc/uptime", "r");
      if (uptime_fp) {
        double uptime_sec = 0.0;
         if (fscanf(uptime_fp, "%lf", &uptime_sec) == 1) {
            stats->uptime_seconds = (uint64_t)uptime_sec;
         }
         fclose(uptime_fp);
         
      }
      return 0;
    }