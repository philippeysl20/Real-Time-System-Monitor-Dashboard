#ifndef BUILDJSON_H
#define BUILDJSON_H

char *buildCPUJSON(double data);

char *buildDiskJSON(double data);

char *buildMemoryPercentJSON(double data);

char *buildTotalMemoryJSON(double data);

char *buildTotalAvailableMemoryJSON(double data);

char *buildUsedMemoryJSON(double data);

#endif //BUILDJSON_H
