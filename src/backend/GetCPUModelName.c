#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Retrieve the CPU model name from the Windows registry
char* getCPUModelName() {
    // Registry key handle
    HKEY hKey;
    // Registry path to the first CPU processor info
    const char* subkey = "HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0";
    // Buffer to receive the CPU name string
    char buffer[256];
    // Size of the buffer in bytes
    DWORD bufferSize = sizeof(buffer);
    // Variable to store the registry value type
    DWORD type = 0;

    // Open the registry key for reading
    if (RegOpenKeyExA(HKEY_LOCAL_MACHINE, subkey, 0, KEY_READ, &hKey) != ERROR_SUCCESS) {
        // Return NULL if opening the key failed
        return NULL;
    }

    // Query the 'ProcessorNameString' value
    if (RegQueryValueExA(hKey, "ProcessorNameString", NULL, &type, (LPBYTE)buffer, &bufferSize) == ERROR_SUCCESS) {
        // Close the registry key when done
        RegCloseKey(hKey);
        // Allocate memory for the result string
        char* result = malloc(bufferSize + 1);
        if (result != NULL) {
            // Copy the CPU name into the allocated buffer
            strcpy(result, buffer);
        }
        // Return the allocated CPU model name
        return result;
    }

    // Close the key and return NULL if querying failed
    RegCloseKey(hKey);
    return NULL;
}
