#ifndef DISK_H
define DISK_H

#include <stdint.h>

typedef struct {
    uint64_t total_bytes;
    uint64_t free_bytes;
    uint64_t used_bytes;
} DiskStats;
int disk_get_stats(const char *mount_path, DiskStats *stats);

endif /* DISK_H*/
