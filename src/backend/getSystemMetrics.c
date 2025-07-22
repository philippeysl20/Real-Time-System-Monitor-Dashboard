#if defined(_WIN32)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h> // For general system info, disk info
#include <pdh.h>     // For CPU usage
#include <psapi.h>   // For memory info
#include "getSystemMetrics.h"
#include "buildJSON.c"

// Link libraries
#pragma comment (lib, "pdh.lib")
#pragma comment (lib, "psapi.lib")

char *getCPUUsage() {

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
    char *json = buildCPUJSON(gatheredCPUData);

    // Return the final result
    return json;
}

char *getMemoryUsage() {


    // Initialize a variable to hold the values gathered
    double gatheredMemoryData;

    // Build the JSON using the gathered memory data
    char *json = buildMemoryJSON(gatheredMemoryData);

    // Return the final result
    return json;
}

char *getDiskUsage() {


//    ULARGE_INTEGER freeBytesAvailable, totalBytes, totalFreeBytes;
    
    // check if Windows API function GetDiskFreeSpaceEx successfully retrieves disk info
    // returning nonzero (true), means success, zero means failure
//    if (GetDiskFreeSpaceEx(L"C:\\", &freeBytesAvailable, &totalBytes, &totalFreeBytes)) { // L"C:\\" is string literal for root of C: drive
//        ULONGLONG totalUsedBytes = totalBytes.QuadPart - totalFreeBytes.QuadPart;         // compute used
//        printf("Total Space: %llu GB\n", totalBytes.QuadPart / (1024 * 1024 * 1024));     // print total
//        printf("Free Space: %llu GB\n", totalFreeBytes.QuadPart / (1024 * 1024 * 1024));  // print free
//        printf("Used space: %llu GB\n", totalUsedBytes / (1024 * 1024 * 1024));           // print used
//    }

    // Initialize a variable to hold the values gathered
    double gatheredDiskData;

    // Build the JSON using the gathered memory data
    char *json = buildMemoryJSON(gatheredDiskData);

    // Return the final result
    return json;
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

