#if defined(_WIN32)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <pdh.h>
#include "getSystemMetrics.h"
#include "buildJSON.c"
#include "InfoStructs.h"
#include "GetCPUModelName.h"

// Link libraries
#pragma comment (lib, "pdh.lib")

char *getCPUUsage() {
    // Declare the struct that will contain the metrics gathered
    CPUInfo procInfo;

    // Extract the name of the CPU for display on the client-side
    char* modelName = getCPUModelName();

    // Store the name extracted in the struct
    strcpy(procInfo.modelName, modelName);

    // Initialize where the output and query should be stored
    PDH_HQUERY cpuUsageQuery;

    // This will decide where we will be collecting data from (NULL meaning live data collection), the 0 means no custom
    // user data will be attached to the output. Third parameter decides where the query and output is stored.
    PdhOpenQuery(NULL, 0, &cpuUsageQuery);

    // Initialize a variable that will receive the counter handle
    PDH_HCOUNTER cpuUsageCounter;

    // Add a specific performance counter to the query
    PdhAddCounter(cpuUsageQuery, L"\\Processor(_Total)\\% Processor Time", 0, &cpuUsageCounter);

    // Capture the current performance counter values
    // First call is to initialize
    PdhCollectQueryData(cpuUsageQuery);

    // Wait 1000 milliseconds
    Sleep(1000);

    // second call to get updated data
    PdhCollectQueryData(cpuUsageQuery);

    // Retrieve the latest value for a counter in a readable format
    // Struct which will hold the result
    PDH_FMT_COUNTERVALUE cpuUsage;

    // Retrieval of values
    PdhGetFormattedCounterValue(cpuUsageQuery, PDH_FMT_DOUBLE, NULL, &cpuUsage);

    // Store the values gathered in the struct
    procInfo.CPUUsage = cpuUsage.doubleValue;

    // Build the JSON using the gathered CPU data
    char *jsonObject = buildCPUJSON(procInfo);

    // Return the final result
    return jsonObject;
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

    // Initialize where the output and query should be stored
    PDH_HQUERY diskIOQuery;

    // Open the query
    PdhOpenQuery(NULL, 0, &diskIOQuery);

    // Initialize variables that will receive the counter handle
    PDH_HCOUNTER readCounter;
    PDH_HCOUNTER writeCounter;

    // Add specific performance counters to the query
    PdhAddCounter(diskIOQuery, L"\\PhysicalDisk(_Total)\\Disk Read Bytes/sec", 0, &readCounter);
    PdhAddCounter(diskIOQuery, L"\\PhysicalDisk(_Total)\\Disk Write Bytes/sec", 0, &writeCounter);

    // First call is to initialize
    PdhCollectQueryData(readCounter);
    PdhCollectQueryData(writeCounter);

    // Wait 1000 milliseconds
    Sleep(1000);

    // second call to get updated data
    PdhCollectQueryData(readCounter); // second call to get updated data
    PdhCollectQueryData(writeCounter);
    // Structs which will hold the result
    PDH_FMT_COUNTERVALUE diskInput;
    PDH_FMT_COUNTERVALUE diskOutput;

    // Extract the values
    PdhGetFormattedCounterValue(diskIOQuery, PDH_FMT_DOUBLE, NULL, &diskInput);
    PdhGetFormattedCounterValue(diskIOQuery, PDH_FMT_DOUBLE, NULL, &diskOutput);

    // Iterate over the disks and store the values in the array of structs
    for(char *tempPath = paths; *tempPath; tempPath += strlen(tempPath) + 1) {
        // Path/name of the disk
        strcpy(diskInfos[index].path, tempPath);
        // Free disk space, total space, user free space, read and write speed extracted and stored in the struct
        GetDiskFreeSpaceEx(tempPath,
            &diskInfos[index].freeDiskSpace,
            &diskInfos[index].totalDiskSpace,
            &diskInfos[index].userFree);
        diskInfos[index].readSpeed = diskInput.doubleValue;
        diskInfos[index].writeSpeed = diskOutput.doubleValue;

        index++;
    }

    // Build the array of JSON objects
    char *jsonObject = buildDiskJSON(diskInfos, numberOfDrives);

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