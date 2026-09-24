#ifndef CPU_H
#define CPU_H

#include <stdint.h>

/* Raw snapshot of the aggregate CPU line from /proc/stat */
typedef struct {
    uint64_t user;
    uint64_t nice;
    uint64_t system;
    uint64_t idle;
    uint64_t iowait;
    uint64_t irq;
    uint64_t softirq;
    uint64_t steal;
} CpuRawSnapshot;

/* Processed CPU telemetry */
typedef struct {
    double   usage_percent; /* 0.0 to 100.0 */
    uint64_t total_ticks;
    uint64_t idle_ticks;
} CpuStats;

/**
 * Reads /proc/stat, calculates delta since previous call, and populates CpuStats.
 * Returns 0 on success, -1 on failure.
 */
int cpu_get_stats(CpuStats *out_stats);

#endif /* CPU_H */
