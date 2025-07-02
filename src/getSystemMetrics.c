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

void getCPUUsage() {

}

void getMemoryUsage() {

}

void getDiskUsage() {

}

#elif defined(__APPLE__)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/sysctl.h>
#include <mach/mach.h>
#include "getSystemMetrics.h"

void getCPUUsage() {

}

void getMemoryUsage() {

}

void getDiskUsage() {

}

#elif defined(__linux__)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/sysinfo.h>
#include "getSystemMetrics.h"

void getCPUUsage() {

}

void getMemoryUsage() {

}

void getDiskUsage() {

}

#endif

