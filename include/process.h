#ifndef PROCESS_H
#define PROCESS_H

#include <stdint.h>

#define MAX_PROCESSES 1024
#define TOP_PROCESSES_COUNT 8

typedef struct {
    int      pid;
    char     name[64];
    uint64_t memory_rss_kb; /* Resident Set Size in Kilobytes */
} ProcessInfo;

typedef struct {
    int         total_running;
    ProcessInfo top_by_mem[TOP_PROCESSES_COUNT];
} ProcessList;

/**
 * Scans /proc for numeric PIDs, collects memory consumption,
 * and extracts the top processes by RAM usage.
 * Returns 0 on success, -1 on failure.
 */
int process_get_top(ProcessList *list);

#endif /* PROCESS_H */
