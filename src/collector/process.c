#include "process.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <ctype.h>

/* Comparator for qsort: sorts descending by memory_rss_kb */
static int compare_procs(const void *a, const void *b) {
    const ProcessInfo *pa = (const ProcessInfo *)a;
    const ProcessInfo *pb = (const ProcessInfo *)b;
    if (pb->memory_rss_kb > pa->memory_rss_kb) return 1;
    if (pb->memory_rss_kb < pa->memory_rss_kb) return -1;
    return 0;
}

static int parse_proc_status(int pid, ProcessInfo *proc) {
    char path[128];
    snprintf(path, sizeof(path), "/proc/%d/status", pid);

    FILE *fp = fopen(path, "r");
    if (!fp) return -1;

    proc->pid = pid;
    proc->memory_rss_kb = 0;
    strncpy(proc->name, "unknown", sizeof(proc->name));

    char line[256];
    while (fgets(line, sizeof(line), fp)) {
        if (strncmp(line, "Name:", 5) == 0) {
            sscanf(line, "Name:\t%63[^\n]", proc->name);
        } else if (strncmp(line, "VmRSS:", 6) == 0) {
            sscanf(line, "VmRSS:\t%lu kB", &proc->memory_rss_kb);
        }
    }
    fclose(fp);
    return 0;
}

int process_get_top(ProcessList *list) {
    if (!list) return -1;
    memset(list, 0, sizeof(ProcessList));

    DIR *dir = opendir("/proc");
    if (!dir) {
        perror("Failed to open /proc");
        return -1;
    }

    ProcessInfo all_procs[MAX_PROCESSES];
    int count = 0;

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        if (isdigit(entry->d_name[0])) {
            int pid = atoi(entry->d_name);
            if (count < MAX_PROCESSES) {
                if (parse_proc_status(pid, &all_procs[count]) == 0) {
                    count++;
                }
            }
        }
    }
    closedir(dir);

    list->total_running = count;

    /* Sort descending by RAM usage */
    qsort(all_procs, count, sizeof(ProcessInfo), compare_procs);

    /* Populate top entries */
    for (int i = 0; i < TOP_PROCESSES_COUNT && i < count; i++) {
        list->top_by_mem[i] = all_procs[i];
    }

    return 0;
}
