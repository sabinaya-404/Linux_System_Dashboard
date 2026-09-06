#include "cpu.h"
#include <stdio.h>
#include <stdint.h>
#include <string.h>

static int read_cpu_snapshot(CpuRawSnapShot *snap) {
    if (!snap) return -1;

    FILE *fp = fopen("/proc/stat", "r");
    if (!fp) {
        perror("Failed to open /proc/stat");
        return -1;
    }

    char line[256];
    if (!fgets(line, sizeof(line), fp)) {
        fclose(fp);
        return -1;
    }
    fclose(fp);

    /* Parse the eight tick fields we care about. */
    int matched = sscanf(line,
        "cpu  %lu %lu %lu %lu %lu %lu %lu %lu",
        &snap->user, &snap->nice, &snap->system,
        &snap->idle, &snap->iowait, &snap->irq,
        &snap->softirq, &snap->steal);

    if (matched < 8) {
        fprintf(stderr, "Unexpected format in /proc/stat\n");
        return -1;
    }
    return 0;
}
int cpu_get_stats(CpuStats *out_stats) {
    if (!out_stats) return -1;

    static CpuRawSnapShot prev = {0};
    static int have_prev = 0;          /* true after first successful read */

    CpuRawSnapShot cur;
    if (read_cpu_snapshot(&cur) != 0) return -1;

    /* Compute totals for the current sample */
    uint64_t cur_total = cur.user + cur.nice + cur.system +
                         cur.idle + cur.iowait + cur.irq +
                         cur.softirq + cur.steal;
    uint64_t cur_idle = cur.idle + cur.iowait;

    if (!have_prev) {
        // First call – we cannot compute a delta yet.
        prev = cur;
        have_prev = 1;
        out_stats->usage_percent = 0.0;
        out_stats->total_ticks   = cur_total;
        out_stats->idle_ticks    = cur_idle;
        return 0;
    }
    /*  Delta calculation , see docs/mistakes.md for the formula */
    uint64_t prev_total = prev.user + prev.nice + prev.system +
                          prev.idle + prev.iowait + prev.irq +
                          prev.softirq + prev.steal;
    uint64_t prev_idle  = prev.idle + prev.iowait;

    uint64_t delta_total = cur_total - prev_total;
    uint64_t delta_idle  = cur_idle  - prev_idle;

    double usage = 0.0;
    if (delta_total > 0) {
        usage = (double)(delta_total - delta_idle) /
                (double)delta_total * 100.0;
    }

    out_stats->usage_percent = usage;
    out_stats->total_ticks   = cur_total;
    out_stats->idle_ticks    = cur_idle;

    prev = cur;
    return 0;
}
