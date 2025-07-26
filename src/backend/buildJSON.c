#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "cJSON.h"
#include "InfoStructs.h"

char *buildCPUJSON(CPUInfo data) {
    // Create a new JSON object
    cJSON *object = cJSON_CreateObject();

    // Store the information gathered in getSystemMetrics.c in the JSON object
    cJSON_AddStringToObject(object, "modelName", data.modelName);   // CPU model name
    cJSON_AddNumberToObject(object, "CPUUsage", data.CPUUsage);     // Current CPU usage percentage
    cJSON_AddNumberToObject(object, "CPUSpeed", data.CPUSpeed);     // Clock speed (MHz)
    cJSON_AddNumberToObject(object, "CPUProcesses", data.CPUProcesses); // Number of processes
    cJSON_AddNumberToObject(object, "CPUThreads", data.CPUThreads); // Number of threads

    // serialize object to a formatted JSON string
    char *jsonObject = cJSON_Print(object);

    // return heap-allocated JSON string
    return jsonObject;
}

char *buildDiskJSON(DriveInfo *data, int numberOfDisks) {
    // create a JSON array for all disks
    cJSON *arrayOfObjects = cJSON_CreateArray();

    for (int i = 0; i < numberOfDisks; i++) {
        // Create a JSON object per disk
        cJSON *object = cJSON_CreateObject();

        // Store the information gathered in getSystemMetrics.c in the JSON object
        cJSON_AddStringToObject(object, "path", data[i].path);      // Drive path (e.g., "C:\\")
        cJSON_AddStringToObject(object, "type", data[i].type);      // Disk type ("SSD" or "HDD")
        cJSON_AddNumberToObject(object, "freeBytes", data[i].freeDiskSpace.QuadPart);   // Free bytes available
        cJSON_AddNumberToObject(object, "totalBytes", data[i].totalDiskSpace.QuadPart); // Total bytes on drive
        cJSON_AddNumberToObject(object, "userFree", data[i].userFree.QuadPart);         // User-available bytes
        cJSON_AddNumberToObject(object, "readBps", data[i].readSpeed);   // Read throughput in B/s
        cJSON_AddNumberToObject(object, "writeBps", data[i].writeSpeed); // Write throughput in B/s

        // Append this disk object to the array
        cJSON_AddItemToArray(arrayOfObjects, object);
    }

    // serialize the array to a formatted JSON string
    char *jsonObject = cJSON_Print(arrayOfObjects);

    // return heap-allocated JSON string
    return jsonObject;
}

char *buildMemoryJSON(MemoryInfo data) {
    // Create a new JSON object
    cJSON *object = cJSON_CreateObject();

    // Store the information gathered in getSystemMetrics.c in the JSON object
    cJSON_AddNumberToObject(object, "totalPhysical", data.totalPhysical);          // total RAM in bytes
    cJSON_AddNumberToObject(object, "totalAvailablePhysical", data.totalAvailablePhysical); // available RAM
    cJSON_AddNumberToObject(object, "usedPhysical", data.usedPhysical);            // used RAM in bytes
    cJSON_AddNumberToObject(object, "percentageUsed", data.percentageUsed);        // RAM usage percentage

    // serialize object to a formatted JSON string
    char *jsonObject = cJSON_Print(object);

    // return heap-allocated JSON string
    return jsonObject;
}


