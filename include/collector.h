#ifndef COLLECTOR_H
#define COLLECTOR_H

#include "memory.h"
#include "cpu.h"
#include "disk.h"
#include "network.h"
#include "thermal.h"
#include "process.h"

/* Unified telemetry snapshot */
typedef struct {
    MemoryStats  memory;
    CpuStats     cpu;
    DiskStats    disk;
    NetworkStats network;
    ThermalStats thermal;
    ProcessList  processes;
} SystemSnapshot;

/**
 * Gathers a full system telemetry snapshot across all subsystems.
 * Returns 0 on success, -1 on failure.
 */
int collector_get_snapshot(SystemSnapshot *snap);

#endif /* COLLECTOR_H */
