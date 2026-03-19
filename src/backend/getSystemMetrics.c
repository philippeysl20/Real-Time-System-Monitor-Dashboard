#if defined(_WIN32)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <pdh.h>
#include <tlhelp32.h>
#include "getSystemMetrics.h"
#include "BuildJSON.h"
#include "InfoStructs.h"
#include "GetCPUModelName.h"

// Link libraries
#pragma comment (lib, "pdh.lib")

static ULONGLONG fileTimeToUInt64(const FILETIME *time) {
    return ((ULONGLONG) time->dwHighDateTime << 32) | time->dwLowDateTime;
}

static double sampleCPUUsage(void) {
    static int hasPreviousSample = 0;
    static ULONGLONG previousIdle = 0;
    static ULONGLONG previousKernel = 0;
    static ULONGLONG previousUser = 0;

    FILETIME idleTime;
    FILETIME kernelTime;
    FILETIME userTime;

    if (!GetSystemTimes(&idleTime, &kernelTime, &userTime)) {
        return 0.0;
    }

    const ULONGLONG idle = fileTimeToUInt64(&idleTime);
    const ULONGLONG kernel = fileTimeToUInt64(&kernelTime);
    const ULONGLONG user = fileTimeToUInt64(&userTime);

    if (!hasPreviousSample) {
        previousIdle = idle;
        previousKernel = kernel;
        previousUser = user;
        hasPreviousSample = 1;
        return 0.0;
    }

    const ULONGLONG idleDelta = idle - previousIdle;
    const ULONGLONG kernelDelta = kernel - previousKernel;
    const ULONGLONG userDelta = user - previousUser;
    const ULONGLONG totalDelta = kernelDelta + userDelta;

    previousIdle = idle;
    previousKernel = kernel;
    previousUser = user;

    if (totalDelta == 0) {
        return 0.0;
    }

    return ((double) (totalDelta - idleDelta) * 100.0) / (double) totalDelta;
}

static void collectDiskThroughput(double *readBps, double *writeBps) {
    static int queryInitialized = 0;
    static PDH_HQUERY diskIOQuery = NULL;
    static PDH_HCOUNTER readCounter = NULL;
    static PDH_HCOUNTER writeCounter = NULL;

    *readBps = 0.0;
    *writeBps = 0.0;

    if (!queryInitialized) {
        if (PdhOpenQuery(NULL, 0, &diskIOQuery) != ERROR_SUCCESS) {
            return;
        }

        if (PdhAddCounter(diskIOQuery, "\\PhysicalDisk(_Total)\\Disk Read Bytes/sec", 0, &readCounter) != ERROR_SUCCESS ||
            PdhAddCounter(diskIOQuery, "\\PhysicalDisk(_Total)\\Disk Write Bytes/sec", 0, &writeCounter) != ERROR_SUCCESS) {
            PdhCloseQuery(diskIOQuery);
            diskIOQuery = NULL;
            return;
        }

        PdhCollectQueryData(diskIOQuery);
        queryInitialized = 1;
        return;
    }

    if (PdhCollectQueryData(diskIOQuery) != ERROR_SUCCESS) {
        return;
    }

    PDH_FMT_COUNTERVALUE diskInput;
    PDH_FMT_COUNTERVALUE diskOutput;

    if (PdhGetFormattedCounterValue(readCounter, PDH_FMT_DOUBLE, NULL, &diskInput) == ERROR_SUCCESS) {
        *readBps = diskInput.doubleValue;
    }

    if (PdhGetFormattedCounterValue(writeCounter, PDH_FMT_DOUBLE, NULL, &diskOutput) == ERROR_SUCCESS) {
        *writeBps = diskOutput.doubleValue;
    }
}

char *getCPUUsage() {
    // Declare the struct that will contain the metrics gathered
    CPUInfo procInfo;

    // Extract the name of the CPU for display on the client-side
    char* modelName = getCPUModelName();

    // Store the name extracted in the struct
    strcpy(procInfo.modelName, modelName != NULL ? modelName : "Unknown CPU");

    // Sample CPU usage from rolling system times so updates remain smooth in the live UI.
    procInfo.CPUUsage = sampleCPUUsage();
    procInfo.CPUSpeed = (double) getCPUFrequency();
    procInfo.CPUProcesses = getProcessCount();
    procInfo.CPUThreads = getThreadCount();

    // Build the JSON using the gathered CPU data
    char *jsonObject = buildCPUJSON(procInfo);
    if (modelName != NULL) {
        free(modelName);
    }

    // Return the final result
    return jsonObject;
}

static const char *getDriveTypeLabel(UINT driveType) {
    switch (driveType) {
        case DRIVE_FIXED:
            return "SSD";
        case DRIVE_REMOVABLE:
            return "RM";
        case DRIVE_REMOTE:
            return "NW";
        case DRIVE_CDROM:
            return "CD";
        case DRIVE_RAMDISK:
            return "RD";
        default:
            return "NA";
    }
}

char *getDiskUsage() {
    int size = GetLogicalDriveStrings(0, NULL); // determine how many characters are needed for all drive strings
    char *paths = (char *)malloc(size);                    // allocate a buffer of that size
    GetLogicalDriveStrings(size, paths);                   // populate the buffer with each drive root (e.g. "C:\\", "D:\\")

    // Count how many drives a user has
    int numberOfDrives = 0;
    for(char *tempPath = paths; *tempPath; tempPath += strlen(tempPath) + 1)
        numberOfDrives++;

    // Allocate an array to hold a struct containing information for each drive
    DriveInfo *diskInfos = malloc(sizeof(DriveInfo) * numberOfDrives);

    int index = 0; // Use to iterate over drives when extracting info.

    double readBps = 0.0;
    double writeBps = 0.0;
    collectDiskThroughput(&readBps, &writeBps);

    // Iterate over the disks and store the values in the array of structs
    for(char *tempPath = paths; *tempPath; tempPath += strlen(tempPath) + 1) {
        // Path/name of the disk
        strcpy(diskInfos[index].path, tempPath);
        // Free disk space, total space, user free space, read and write speed extracted and stored in the struct
        GetDiskFreeSpaceEx(tempPath,
            &diskInfos[index].freeDiskSpace,
            &diskInfos[index].totalDiskSpace,
            &diskInfos[index].userFree);
        strcpy(diskInfos[index].type, getDriveTypeLabel(GetDriveType(tempPath)));
        diskInfos[index].readSpeed = readBps;
        diskInfos[index].writeSpeed = writeBps;

        index++;
    }

    // Build the array of JSON objects
    char *jsonObject = buildDiskJSON(diskInfos, numberOfDrives);

    free(paths);
    free(diskInfos);

    // Return the array of objects
    return jsonObject;
}

char *getMemoryUsage() {
    MEMORYSTATUSEX memStatus; // Windows struct for detailed memory info
    MemoryInfo memInfo;       // Your own struct to hold processed memory data

    // Must set size before calling the API
    memStatus.dwLength = sizeof(memStatus);

    // Store the current RAM usage in memStatus
    GlobalMemoryStatusEx(&memStatus);

    // Store the values in memStatus
    memInfo.totalPhysical = (double)memStatus.ullTotalPhys / 1073741824.0;
    memInfo.totalAvailablePhysical = (double)memStatus.ullAvailPhys / 1073741824.0;
    memInfo.usedPhysical = ((double)(memStatus.ullTotalPhys - memStatus.ullAvailPhys)) / 1073741824.0;
    memInfo.percentageUsed = memStatus.dwMemoryLoad;

    // Build the JSON using the gathered memory data
    char *jsonObject = buildMemoryJSON(memInfo);

    // Return the Object
    return jsonObject;
}

DWORD getProcessCount() {
    DWORD processCount = 0;
    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

    if (snapshot == INVALID_HANDLE_VALUE) {
        return 0;
    }

    PROCESSENTRY32 processEntry;
    processEntry.dwSize = sizeof(PROCESSENTRY32);

    if (Process32First(snapshot, &processEntry)) {
        do {
            processCount++;
        } while (Process32Next(snapshot, &processEntry));
    }

    CloseHandle(snapshot);
    return processCount;
}

DWORD getThreadCount() {
    DWORD threadCount = 0;
    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);

    if (snapshot == INVALID_HANDLE_VALUE) {
        return 0;
    }

    THREADENTRY32 threadEntry;
    threadEntry.dwSize = sizeof(THREADENTRY32);

    if (Thread32First(snapshot, &threadEntry)) {
        do {
            threadCount++;
        } while (Thread32Next(snapshot, &threadEntry));
    }

    CloseHandle(snapshot);
    return threadCount;
}

DWORD getCPUFrequency() {
    HKEY registryKey;
    DWORD frequencyMHz = 0;
    DWORD valueSize = sizeof(DWORD);

    if (RegOpenKeyExA(HKEY_LOCAL_MACHINE,
                      "HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0",
                      0,
                      KEY_READ,
                      &registryKey) == ERROR_SUCCESS) {
        RegQueryValueExA(registryKey, "~MHz", NULL, NULL, (LPBYTE) &frequencyMHz, &valueSize);
        RegCloseKey(registryKey);
    }

    return frequencyMHz;
}

#elif defined(__APPLE__)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/sysctl.h>
#include <sys/mount.h>
#include <mach/mach.h>
#include <CoreFoundation/CoreFoundation.h>
#include <IOKit/IOKitLib.h>
#include <IOKit/IOBSD.h>
#include <IOKit/storage/IOMedia.h>
#include "getSystemMetrics.h"
#include "buildJSON.c"
#include "InfoStructs.h"
#include "GetCPUModelName.h"

char *getCPUUsage() {
    // Get CPU usage using the libraries
    double placeholderForGatheredData;


    // Build the JSON using the gathered usage data
    buildMemoryJSON(placeholderForGatheredData);
}

char *getMemoryUsage() {
    // Get CPU usage using the libraries
    double placeholderForGatheredData;


    // Build the JSON using the gathered usage data
    buildCPUJSON(placeholderForGatheredData);
}

char *getDiskUsage() {
    DriveInfo *diskInfo;
    int numberOfDrives;

    struct statfs *paths;

    getmntinfo(&paths, MNT_NOWAIT);


    char *jsonObject = buildDiskJSON(diskInfo, numberOfDrives);
}

#endif
