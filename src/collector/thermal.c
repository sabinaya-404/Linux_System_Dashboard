#include "thermal.h"
#include <stdio.h>
#include <string.h>

int thermal_get_stats(ThermalStats *stats) {
    if (!stats) return -1;
    memset(stats, 0, sizeof(ThermalStats));

    /* Common thermal sysfs sensor paths on Linux */
    const char *paths[] = {
        "/sys/class/thermal/thermal_zone0/temp",
        "/sys/class/thermal/thermal_zone1/temp",
        "/sys/class/hwmon/hwmon0/temp1_input",
        "/sys/class/hwmon/hwmon1/temp1_input",
        NULL
    };

    for (int i = 0; paths[i] != NULL; i++) {
        FILE *fp = fopen(paths[i], "r");
        if (fp) {
            long milli = 0;
            if (fscanf(fp, "%ld", &milli) == 1 && milli > 0) {
                stats->temp_celsius = (double)milli / 1000.0;
                stats->sensor_found = 1;
                fclose(fp);
                return 0;
            }
            fclose(fp);
        }
    }

    /* Fallback for virtual machines or WSL without hardware passthrough */
    stats->temp_celsius = 0.0;
    stats->sensor_found = 0;
    return 0;
}
