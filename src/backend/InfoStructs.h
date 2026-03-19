#ifndef INFOSTRUCTS_H
#define INFOSTRUCTS_H

#if defined(_WIN32)
    #include <windows.h>
    #define MAX_DRIVE_PATH MAX_PATH

    typedef struct {
        char path[MAX_DRIVE_PATH];     // e.g. "C:\\"
        char type[4];                  // SSD, HDD, etc.
        ULARGE_INTEGER freeDiskSpace;  // bytes available to this user
        ULARGE_INTEGER totalDiskSpace; // total volume size
        ULARGE_INTEGER userFree;       // all free bytes on volume
        double readSpeed;              // In B/s
        double writeSpeed;             // In B/s
    } DriveInfo;

#elif defined(__APPLE__) || defined(__linux__)
#define MAX_DRIVE_PATH 1024

typedef struct {
    char path[MAX_DRIVE_PATH];    // e.g. "/"
    char driveType[4];            // SSD, HDD, etc.
    double freeSpace;             // bytes
    double totalSpace;            // bytes
    double availableSpace;        // bytes
    double readSpeed;             // In B/s
    double writeSpeed;            // In B/s
} DriveInfo;

#endif

typedef struct {
    char modelName[256];
    double CPUUsage;
    double CPUSpeed;
    int CPUProcesses;
    int CPUThreads;
} CPUInfo;

typedef struct {
    double totalPhysical;
    double totalAvailablePhysical;
    double usedPhysical;
    double percentageUsed;
} MemoryInfo;

#endif // INFOSTRUCTS_H
