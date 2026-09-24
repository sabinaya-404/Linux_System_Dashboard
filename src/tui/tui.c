#include "tui.h"
#include <stdio.h>

/* Helper to render a 15-character ASCII meter */
static void render_bar(double percent, char *out, size_t out_len) {
    int total_chars = 15;
    int filled = (int)((percent / 100.0) * total_chars);
    if (filled > total_chars) filled = total_chars;
    if (filled < 0) filled = 0;

    size_t idx = 0;
    out[0] = '[';
    idx = 1;

    for (int i = 0; i < filled && idx < out_len - 10; i++) {
        /* UTF-8 full block: █ */
        out[idx++] = 0xE2;
        out[idx++] = 0x96;
        out[idx++] = 0x88;
    }
    for (int i = filled; i < total_chars && idx < out_len - 10; i++) {
        /* UTF-8 light shade: ░ */
        out[idx++] = 0xE2;
        out[idx++] = 0x96;
        out[idx++] = 0x91;
    }
    out[idx++] = ']';
    out[idx] = '\0';
}

void tui_render(const SystemSnapshot *snap) {
    char cpu_bar[64], ram_bar[64], disk_bar[64];
    render_bar(snap->cpu.usage_percent, cpu_bar, sizeof(cpu_bar));
    render_bar(snap->memory.ram_usage_percent, ram_bar, sizeof(ram_bar));
    render_bar(snap->disk.usage_percent, disk_bar, sizeof(disk_bar));

    uint64_t days = snap->memory.uptime_seconds / 86400;
    uint64_t hours = (snap->memory.uptime_seconds % 86400) / 3600;
    uint64_t mins = (snap->memory.uptime_seconds % 3600) / 60;
    uint64_t secs = snap->memory.uptime_seconds % 60;

    /* Clear screen & move cursor to top-left */
    printf("\033[H\033[J");

    /* ANSI Colors: \033[1;36m = Cyan Bold, \033[1;32m = Green, \033[0m = Reset */
    printf("\033[1;36m┌─────────────────────────────────────────────────────────────┐\033[0m\n");
    printf("\033[1;36m│\033[1;37m                   MY LINUX SYSTEM DASHBOARD                 \033[1;36m│\033[0m\n");
    printf("\033[1;36m├─────────────────────────────────────────────────────────────┤\033[0m\n");
    printf("│ Uptime:      %2lu days, %02lu:%02lu:%02lu                            │\n", days, hours, mins, secs);
    printf("│ CPU:         %-20s  \033[1;33m%5.1f%%\033[0m                   │\n", cpu_bar, snap->cpu.usage_percent);
    printf("│ RAM:         %-20s  \033[1;32m%5.1f%%\033[0m (%.1f/%.1f GB)   │\n",
           ram_bar, snap->memory.ram_usage_percent,
           (double)snap->memory.used_ram_kb / (1024.0 * 1024.0),
           (double)snap->memory.total_ram_kb / (1024.0 * 1024.0));
    printf("│ Disk (/):    %-20s  \033[1;35m%5.1f%%\033[0m (%.1f/%.1f GB)   │\n",
           disk_bar, snap->disk.usage_percent,
           (double)snap->disk.used_bytes / (1024.0 * 1024.0 * 1024.0),
           (double)snap->disk.total_bytes / (1024.0 * 1024.0 * 1024.0));

    if (snap->thermal.sensor_found) {
        printf("│ Temperature: \033[1;31m%5.1f°C\033[0m                                        │\n", snap->thermal.temp_celsius);
    } else {
        printf("│ Temperature: N/A (Virtual/WSL)                             │\n");
    }

    printf("\033[1;36m├─────────────────────────────────────────────────────────────┤\033[0m\n");
    printf("│ \033[1;34mNetwork Traffic\033[0m                                             │\n");
    printf("│  ↓ Down: %8.1f kB/s         ↑ Up: %8.1f kB/s             │\n",
           snap->network.rx_rate_kBps, snap->network.tx_rate_kBps);
    printf("\033[1;36m├─────────────────────────────────────────────────────────────┤\033[0m\n");
    printf("│ \033[1;34mTop Processes by Memory\033[0m (Total Active: %-4d)                │\n", snap->processes.total_running);
    for (int i = 0; i < TOP_PROCESSES_COUNT && i < 5; i++) {
        printf("│  • PID %-6d %-20s %7.1f MB               │\n",
               snap->processes.top_by_mem[i].pid,
               snap->processes.top_by_mem[i].name,
               (double)snap->processes.top_by_mem[i].memory_rss_kb / 1024.0);
    }
    printf("\033[1;36m└─────────────────────────────────────────────────────────────┘\033[0m\n");
    printf("  [Ctrl + C to quit]                                           \n");
    fflush(stdout);
}