#ifndef BUILDJSON_H
#define BUILDJSON_H

char *buildCPUJSON(CPUInfo data);

char *buildDiskJSON(DriveInfo *data, int numberOfDisks);

char *buildMemoryJSON(MemoryInfo data);

#endif //BUILDJSON_H
