#if defined(_WIN32)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h> // For general system info, disk info
#include <pdh.h>     // For CPU usage
#include <psapi.h>   // For memory info
#include "getSystemMetrics.h"

// Link libraries
#pragma comment (lib, "pdh.lib")
#pragma comment (lib, "psapi.lib")

void *getCPUUsage() {

}

void *getMemoryUsage() {

}

void *getDiskUsage() {
    ULARGE_INTEGER freeBytesAvailable, totalBytes, totalFreeBytes;
    
    // check if Windows API function GetDiskFreeSpaceEx successfully retrieves disk info
    // returning nonzero (true), means success, zero means failure
    if (GetDiskFreeSpaceEx(L"C:\\", &freeBytesAvailable, &totalBytes, &totalFreeBytes)) { // L"C:\\" is string literal for root of C: drive
        ULONGLONG totalUsedBytes = totalBytes.QuadPart - totalFreeBytes.QuadPart;         // compute used
        printf("Total Space: %llu GB\n", totalBytes.QuadPart / (1024 * 1024 * 1024));     // print total
        printf("Free Space: %llu GB\n", totalFreeBytes.QuadPart / (1024 * 1024 * 1024));  // print free
        printf("Used space: %llu GB\n", totalUsedBytes / (1024 * 1024 * 1024));           // print used
    }
}

#elif defined(__APPLE__)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/sysctl.h>
#include <mach/mach.h>
#include "getSystemMetrics.h"

char *getCPUUsage() {

}

char *getMemoryUsage() {

}

char *getDiskUsage() {

}

#elif defined(__linux__)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/sysinfo.h>
#include "getSystemMetrics.h"

char *getCPUUsage() {

}

char *getMemoryUsage() {

}

char *getDiskUsage() {

}

#endif

