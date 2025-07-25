#if defined(_WIN32)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h> // For general system info, disk info
#include <pdh.h>     // For CPU usage
#include <psapi.h>   // For memory info
#include "getSystemMetrics.h"
#include "buildJSON.c"
#include "InfoStructs.h"

// Link libraries
#pragma comment (lib, "pdh.lib")
#pragma comment (lib, "psapi.lib")

CPUInfo *getCPUUsage() {
    // Get CPU usage using the libraries
    PDH_HQUERY cpuUsageQuery; // Initialize where the output and query should be stored

    // This will decide where we will be collecting data from (NULL meaning live data collection), the 0 means no custom
    // user data will be attached to the output. Third parameter decides where the query and output is stored.
    PdhOpenQuery(NULL, 0, &cpuUsageQuery);

    // Initialize a variable that will receive the counter handle
    PDH_HCOUNTER cpuUsageCounter;

    // Add a specific performance counter to the query
    PdhAddCounter(cpuUsageQuery, L"\\Processor(_Total)\\% Processor Time", 0, &cpuUsageCounter);

    // Capture the current performance counter values
    PdhCollectQueryData(cpuUsageQuery); // First call is to initialize
    Sleep(1000); // Wait 1000 milliseconds
    PdhCollectQueryData(cpuUsageQuery); // second call to get updated data

    // Retrieve the latest value for a counter in a readable format
    PDH_FMT_COUNTERVALUE cpuUsage; // Struct which will hold the result

    PdhGetFormattedCounterValue(cpuUsageQuery, PDH_FMT_DOUBLE, NULL, &cpuUsage); // Retrieval of values

    // Initialize a variable to hold the values gathered
    double gatheredCPUData = cpuUsage.doubleValue;

    // Build the JSON using the gathered CPU data
    char *jsonStringCPU = buildCPUJSON(gatheredCPUData);

    // Return the final result
    return jsonStringCPU;
}

MemoryInfo *getMemoryUsage(char **jsonStringMemoryPercentage, char **jsonStringMemoryTotal, char **jsonStringMemoryTotalAvailable, char **jsonStringMemoryUsed) {
    MEMORYSTATUSEX memStatus;
    MemoryInfo memInfo;

    memStatus.dwLength = sizeof(memStatus);

    GlobalMemoryStatusEx(&memStatus);

    // Initialize a variable to hold the values gathered
    memInfo.totalPhysical = (double)memStatus.ullTotalPhys / 1073741824.0;
    memInfo.totalAvailablePhysical = (double)memStatus.ullAvailPhys / 1073741824.0;
    memInfo.usedPhysical = ((double)(memStatus.ullTotalPhys - memStatus.ullAvailPhys)) / 1073741824.0;
    memInfo.percentageUsed = memStatus.dwMemoryLoad;

    // Build the JSON using the gathered memory data
    *jsonStringMemoryPercentage = buildMemoryPercentJSON(memInfo.percentageUsed);
    *jsonStringMemoryTotal = buildTotalMemoryJSON(memInfo.totalPhysical);
    *jsonStringMemoryTotalAvailable = buildTotalAvailableMemoryJSON(memInfo.totalAvailablePhysical);
    *jsonStringMemoryUsed = buildUsedMemoryJSON(memInfo.usedPhysical);
}

DriveInfo *getDiskUsage() {
    int size = GetLogicalDriveStrings(0, NULL);
    char *paths = (char *)malloc(size);
    GetLogicalDriveStrings(size, paths);

    int numberOfDrives = 0;
    for(char *tempPath = paths; *tempPath; tempPath += strlen(tempPath) + 1)
        numberOfDrives++;

    DriveInfo *diskInfos = malloc(sizeof(DriveInfo) * numberOfDrives);

    int index = 0; // Use to iterate over drives when extracting info.
    for(char *tempPath = paths; *tempPath; tempPath += strlen(tempPath) + 1) {
        strcpy(diskInfos[index].path, tempPath);

        GetDiskFreeSpaceEx(paths,
            &diskInfos[index].freeDiskSpace,
            &diskInfos[index].totalDiskSpace,
            &diskInfos[index].userFree);
        index++;
    }

}


#elif defined(__APPLE__)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/sysctl.h>
#include <mach/mach.h>
#include "getSystemMetrics.h"
#include "buildJSON.c"

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
    // Get CPU usage using the libraries
    double placeholderForGatheredData;


    // Build the JSON using the gathered usage data
    return buildDiskJSON(placeholderForGatheredData);
}

#elif defined(__linux__)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/sysinfo.h>
#include "getSystemMetrics.h"
#include "buildJSON.c"

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
    // Get CPU usage using the libraries
    double placeholderForGatheredData;


    // Build the JSON using the gathered usage data
    return buildDiskJSON(placeholderForGatheredData);
}

#endif