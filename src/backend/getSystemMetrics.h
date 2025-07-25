
#ifndef GETSYSTEMMETRICS_H
#define GETSYSTEMMETRICS_H

#include "InfoStructs.h"

CPUInfo *getCPUUsage();
MemoryInfo *getMemoryUsage(char *jsonStringMemoryPercentage, char *jsonStringMemoryTotal, char *jsonStringMemoryTotalAvailable, char *jsonStringMemoryUsed);
DriveInfo *getDiskUsage();

#endif //GETSYSTEMMETRICS_H
