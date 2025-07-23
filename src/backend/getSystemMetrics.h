
#ifndef GETSYSTEMMETRICS_H
#define GETSYSTEMMETRICS_H

char *getCPUUsage();
void *getMemoryUsage(char *jsonStringMemoryPercentage, char *jsonStringMemoryTotal, char *jsonStringMemoryTotalAvailable, char *jsonStringMemoryUsed);
char *getDiskUsage();

#endif //GETSYSTEMMETRICS_H
