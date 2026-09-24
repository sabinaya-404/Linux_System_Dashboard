#include "collector.h"
#include <string.h>

int collector_get_snapshot(SystemSnapshot *snap) {
    if (!snap) return -1;
    memset(snap, 0, sizeof(SystemSnapshot));

    memory_get_stats(&snap->memory);
    cpu_get_stats(&snap->cpu);
    disk_get_stats("/", &snap->disk);
    network_get_stats(&snap->network);
    thermal_get_stats(&snap->thermal);
    process_get_top(&snap->processes);

    return 0;
}