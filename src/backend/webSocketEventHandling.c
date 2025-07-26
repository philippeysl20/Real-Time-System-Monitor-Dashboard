#include "mongoose.h"
#include "getSystemMetrics.h"
#include "string.h"
#include "globals.h"
#include <stdio.h>

int running;

void webSocketEventHandler(struct mg_connection *client, const int event, void *event_data) {
    if (event == MG_EV_WS_OPEN) {
        // Print an opening message to the console
        printf("WebSocket open. Client connected\n");

        // Initialize a welcome message
        char *welcomeMessage = "You have successfully been connected";

        // Send the welcome message to the user
        mg_ws_send(client, welcomeMessage, strlen(welcomeMessage), WEBSOCKET_OP_TEXT);
    } else if (event == MG_EV_WS_MSG) {
        // Print if the client is ready to recieve messages
        printf("Client is ready to recieve data.\n");
        // Initialize variables to hold the data collected

        // CPU variables
        char *CPUData = getCPUUsage();

        // Disk variables
        char *diskData = getDiskUsage();

        // Memory variables
        char *memoryData = getMemoryUsage();

        // Send the structs to the user
        mg_ws_send(client, CPUData, strlen(CPUData), WEBSOCKET_OP_TEXT);
        mg_ws_send(client, diskData, strlen(diskData), WEBSOCKET_OP_TEXT);
        mg_ws_send(client, memoryData, strlen(memoryData), WEBSOCKET_OP_TEXT);

        // Free the memory allocated on the heap that contained the structs
        free(CPUData);
        free(diskData);
        free(memoryData);

    } else if(event == MG_EV_CLOSE) {
        // Set the 'running' variable to 0 which will stop the polling loop
        running = 0;

        // Print a closing message to the console
        printf("WebSocket closed. Client disconnected\n");
    }
}
