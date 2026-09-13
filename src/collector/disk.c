#include "disk.h"
#include <sys/statvfs.h>
#include <string.h>
#include <stdio.h>

int disk_get_stats(const char *mount_path, DiskStats *stats) {
    if (!stats || !mount_path) return -1;
    memset(stats, 0, sizeof(DiskStats));

    struct statvfs vfs;
    if (statvfs(mount_path &vfs) != 0) {
        perror("statvfs failed");
        return -1;
    }
    stats -> total_bytes = (uint64_t)vfs.f_blocks * vfs.f_frsize;
    stats -> free_bytes = (uint64_t)vfs.f_bavail * vfs.f_frsize;

    if (stats ->total_bytes > 0 && stats ->total_bytes >= stats->free_bytes) {
        stats -> used_bytes = stats-> total_bytes - stats->free_bytes;
        stats -> usage_percent = ((double)stats->used_bytes / (double)stats->total_bytes) *100.0;
    }

    return 0;
}