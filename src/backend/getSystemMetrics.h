
#ifndef GETSYSTEMMETRICS_H
#define GETSYSTEMMETRICS_H

#include "InfoStructs.h"

char *getCPUUsage();
char *getMemoryUsage();
char *getDiskUsage();
DWORD getProcessCount();
DWORD getThreadCount();
DWORD getCPUFrequency();

#endif //GETSYSTEMMETRICS_H
