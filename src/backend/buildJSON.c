#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

char *buildCPUJSON(data) {
    char *result = (char *)malloc(35); // allocate space on the heap for the JSON object

    if (result == NULL) { // Check if memory has been allocated.
        printf("Memory has not been allocated");
    } else {
        printf("Memory has succesfully been allocated");
    }

    // Place the JSON object in the allocated memory
    sprintf(result, "{\"type\":\"CPU-data\",\"data\":%.1f}", data);

    // Print the JSON object
    printf("%s", result);

    // Return the JSON object
    return result;
}

char *buildDiskJSON(data) {
    char *result = (char *)malloc(35); // allocate space on the heap for the JSON object

    if (result == NULL) { // Check if memory has been allocated.
        printf("Memory has not been allocated");
    } else {
        printf("Memory has succesfully been allocated");
    }

    // Place the JSON object in the allocated memory
    sprintf(result, "{\"type\":\"Disk-data\",\"data\":%.1f}", data);

    // Print the JSON object
    printf("%s", result);

    // Return the JSON object
    return result;
}

char *buildMemoryJSON(data) {
    char *result = (char *)malloc(35); // allocate space on the heap for the JSON object

    if (result == NULL) { // Check if memory has been allocated.
        printf("Memory has not been allocated");
    } else {
        printf("Memory has succesfully been allocated");
    }

    // Place the JSON object in the allocated memory
    sprintf(result, "{\"type\":\"Memory-data\",\"data\":%.1f}", data);

    // Print the JSON object
    printf("%s", result);

    // Return the JSON object
    return result;
}
