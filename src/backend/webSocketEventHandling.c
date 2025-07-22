#include "mongoose.h"
#include "getSystemMetrics.h"
#include "string.h"
#include "globals.h"
#include <stdio.h>


void webSocketEventHandler(struct mg_connection *client, const int event, void *event_data) {
    if (event == MG_EV_WS_OPEN) {
        printf("WebSocket open. Client connected\n");
        char *welcomeMessage = "You have successfully been connected";
        mg_ws_send(client, welcomeMessage, strlen(welcomeMessage), WEBSOCKET_OP_TEXT);
    } else if (event == MG_EV_WS_MSG) {
        printf("Client is ready to recieve data.\n");
        // Initialize a variable to hold a string containing the usage in %
        char *CPUData = getCPUUsage();
        char *diskData = getDiskUsage();
        char *memoryData = getMemoryUsage();

        // Send the data to the user
        mg_ws_send(client, CPUData, strlen(CPUData), WEBSOCKET_OP_TEXT);
        mg_ws_send(client, diskData, strlen(diskData), WEBSOCKET_OP_TEXT);
        mg_ws_send(client, memoryData, strlen(memoryData), WEBSOCKET_OP_TEXT);

        // Free the memory allocated on the heap that contained the string
        free(CPUData);
        free(diskData);
        free(memoryData);
    } else if(event == MG_EV_CLOSE) {
        running = 0;
        printf("WebSocket closed. Client disconnected\n");
    }
}
