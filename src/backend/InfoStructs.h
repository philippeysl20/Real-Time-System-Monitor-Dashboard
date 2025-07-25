#ifndef InfoStructs.h
#define InfoStructs.h
#define MAX_DRIVE_PATH MAX_PATH // from <windows.h>.

#include <windows.h>

typedef struct {
    char           path[MAX_DRIVE_PATH];  // e.g. "C:\\"
    ULARGE_INTEGER freeDiskSpace;         // bytes available to this user
    ULARGE_INTEGER totalDiskSpace;        // total volume size
    ULARGE_INTEGER userFree;              // all free bytes on volume

} CPUInfo;

typedef struct {
    double totalPhysical;
    double totalAvailablePhysical;
    double usedPhysical;
    double percentageUsed;

} MemoryInfo;

typedef struct {
    char           path[MAX_DRIVE_PATH];  // e.g. "C:\\"
    ULARGE_INTEGER freeDiskSpace;         // bytes available to this user
    ULARGE_INTEGER totalDiskSpace;        // total volume size
    ULARGE_INTEGER userFree;              // all free bytes on volume

} DriveInfo;



#endif //DISKINFOSTRUCT_H