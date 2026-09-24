#ifndef THERMAL_H
#define THERMAL_H

typedef struct {
    double temp_celsius;
    int    sensor_found; /* 1 if sensor detected, 0 if virtualized/none */
} ThermalStats;

/**
 * Scans thermal zones in /sys/class/thermal and /sys/class/hwmon.
 * Returns 0 on success, -1 on failure.
 */
int thermal_get_stats(ThermalStats *stats);

#endif /* THERMAL_H */
