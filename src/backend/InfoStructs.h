#ifndef InfoStructs.h
#define InfoStructs.h
#define MAX_DRIVE_PATH MAX_PATH // from <windows.h>.

#include <windows.h>

// Struct to hold information about the CPU
typedef struct {
    char modelName[256];
    double CPUUsage;                      // In percent
    double CPUSpeed;                      // in GHz
    int CPUProcesses;
    int CPUThreads;

} CPUInfo;

// Struct to hold information about the RAM usage and memory
typedef struct {
    double totalPhysical;                 // In GB
    double totalAvailablePhysical;        // In GB
    double usedPhysical;                  // In GB
    double percentageUsed;                // In percent

} MemoryInfo;

// Struct to hold information about the drives
typedef struct {
    char           path[MAX_DRIVE_PATH];  // e.g. "C:\\"
    char type[3];                         // SSD or HDD
    ULARGE_INTEGER freeDiskSpace;         // bytes available to this user
    ULARGE_INTEGER totalDiskSpace;        // total volume size
    ULARGE_INTEGER userFree;              // all free bytes on volume
    double readSpeed;                     // In B/s
    double writeSpeed;                    // In B/s


} DriveInfo;



#endif //DISKINFOSTRUCT_H