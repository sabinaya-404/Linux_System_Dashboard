#ifndef  CPU_H
#define CPU_H
#include <stdint.h>

 typedef struct {
     uint64_t user;
     uint64_t nice;
     uint64_t system;
     uint64_t idle;
     uint64_t iowait;
     uint64_t irq;
     uint64_t softirq;
     uint64_t steal;
 } CpuRawSnapShot;

 typedef struct {
     double usage_percent;
     uint64_t total_ticks;
     uint64_t idle_ticks;
 } CpuStats;

    int cpu_get_stats(CpuStats *out_stats);
     #endif
