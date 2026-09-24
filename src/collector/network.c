#include "network.h"
#include <stdio.h>
#include <string.h>
#include <time.h>

static int read_network_totals(uint64_t *total_rx, uint64_t *total_tx) {
    FILE *fp = fopen("/proc/net/dev", "r");
    if (!fp) return -1;

    char line[512];
    *total_rx = 0;
    *total_tx = 0;

    /* Skip the first 2 header lines */
    if (!fgets(line, sizeof(line), fp)) { fclose(fp); return -1; }
    if (!fgets(line, sizeof(line), fp)) { fclose(fp); return -1; }

    while (fgets(line, sizeof(line), fp)) {
        char iface[32];
        uint64_t rx = 0, tx = 0;
        uint64_t dummy;

        char *colon = strchr(line, ':');
        if (!colon) continue;

        *colon = ' '; /* Replace colon with space for simple sscanf */

        if (sscanf(line, "%31s %lu %lu %lu %lu %lu %lu %lu %lu %lu",
                   iface, &rx, &dummy, &dummy, &dummy, &dummy, &dummy, &dummy, &dummy, &tx) >= 2) {
            /* Exclude loopback */
            if (strcmp(iface, "lo") != 0) {
                *total_rx += rx;
                *total_tx += tx;
            }
        }
    }
    fclose(fp);
    return 0;
}

int network_get_stats(NetworkStats *stats) {
    if (!stats) return -1;

    static uint64_t prev_rx = 0;
    static uint64_t prev_tx = 0;
    static time_t prev_time = 0;
    static int have_prev = 0;

    uint64_t cur_rx = 0, cur_tx = 0;
    if (read_network_totals(&cur_rx, &cur_tx) != 0) return -1;

    time_t cur_time = time(NULL);

    if (!have_prev) {
        prev_rx = cur_rx;
        prev_tx = cur_tx;
        prev_time = cur_time;
        have_prev = 1;

        stats->rx_bytes = cur_rx;
        stats->tx_bytes = cur_tx;
        stats->rx_rate_kBps = 0.0;
        stats->tx_rate_kBps = 0.0;
        return 0;
    }

    double delta_sec = difftime(cur_time, prev_time);
    if (delta_sec <= 0.0) delta_sec = 1.0;

    stats->rx_bytes = cur_rx;
    stats->tx_bytes = cur_tx;

    if (cur_rx >= prev_rx) {
        stats->rx_rate_kBps = ((double)(cur_rx - prev_rx) / 1024.0) / delta_sec;
    } else {
        stats->rx_rate_kBps = 0.0;
    }

    if (cur_tx >= prev_tx) {
        stats->tx_rate_kBps = ((double)(cur_tx - prev_tx) / 1024.0) / delta_sec;
    } else {
        stats->tx_rate_kBps = 0.0;
    }

    prev_rx = cur_rx;
    prev_tx = cur_tx;
    prev_time = cur_time;
    return 0;
}
