#ifndef NETWORK_H
#define NETWORK_H

#include <stdint.h>

typedef struct {
    uint64_t rx_bytes;       /* Lifetime received bytes across non-loopback interfaces */
    uint64_t tx_bytes;       /* Lifetime transmitted bytes across non-loopback interfaces */
    double   rx_rate_kBps;   /* Current download rate in kB/s */
    double   tx_rate_kBps;   /* Current upload rate in kB/s */
} NetworkStats;

/**
 * Reads /proc/net/dev and calculates transfer rates since previous call.
 * Returns 0 on success, -1 on failure.
 */
int network_get_stats(NetworkStats *stats);

#endif /* NETWORK_H */
