#include "http_server.h"
#include <stdio.h>

int json_build_snapshot(const SystemSnapshot *snap, char *out, size_t max_len) {
    if (!snap || !out) return -1;

    int offset = snprintf(out, max_len,
        "{"
        "\"cpu\":{\"percent\":%.2f},"
        "\"ram\":{\"total_gb\":%.2f,\"used_gb\":%.2f,\"free_gb\":%.2f,\"percent\":%.2f},"
        "\"swap\":{\"total_gb\":%.2f,\"used_gb\":%.2f,\"percent\":%.2f},"
        "\"disk\":{\"total_gb\":%.2f,\"used_gb\":%.2f,\"percent\":%.2f},"
        "\"network\":{\"rx_kBps\":%.2f,\"tx_kBps\":%.2f},"
        "\"thermal\":{\"celsius\":%.1f,\"found\":%d},"
        "\"uptime\":%lu,"
        "\"processes\":[",
        snap->cpu.usage_percent,
        (double)snap->memory.total_ram_kb / (1024.0 * 1024.0),
        (double)snap->memory.used_ram_kb / (1024.0 * 1024.0),
        (double)snap->memory.available_ram_kb / (1024.0 * 1024.0),
        snap->memory.ram_usage_percent,
        (double)snap->memory.total_swap_kb / (1024.0 * 1024.0),
        (double)snap->memory.used_swap_kb / (1024.0 * 1024.0),
        snap->memory.swap_usage_percent,
        (double)snap->disk.total_bytes / (1024.0 * 1024.0 * 1024.0),
        (double)snap->disk.used_bytes / (1024.0 * 1024.0 * 1024.0),
        snap->disk.usage_percent,
        snap->network.rx_rate_kBps,
        snap->network.tx_rate_kBps,
        snap->thermal.temp_celsius,
        snap->thermal.sensor_found,
        snap->memory.uptime_seconds
    );

    for (int i = 0; i < TOP_PROCESSES_COUNT; i++) {
        offset += snprintf(out + offset, max_len - offset,
            "{\"pid\":%d,\"name\":\"%s\",\"rss_mb\":%.1f}%s",
            snap->processes.top_by_mem[i].pid,
            snap->processes.top_by_mem[i].name,
            (double)snap->processes.top_by_mem[i].memory_rss_kb / 1024.0,
            (i == TOP_PROCESSES_COUNT - 1) ? "" : ",");
    }

    snprintf(out + offset, max_len - offset, "]}");
    return 0;
}