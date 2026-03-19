#include "mongoose.h"
#include "getSystemMetrics.h"
#include "string.h"
#include "globals.h"
#include <stdio.h>
#include <stdlib.h>

int running;

static void sendMetrics(struct mg_connection *client) {
    char *CPUData = getCPUUsage();
    char *diskData = getDiskUsage();
    char *memoryData = getMemoryUsage();

    if (CPUData != NULL) {
        mg_ws_send(client, CPUData, strlen(CPUData), WEBSOCKET_OP_TEXT);
        free(CPUData);
    }

    if (diskData != NULL) {
        mg_ws_send(client, diskData, strlen(diskData), WEBSOCKET_OP_TEXT);
        free(diskData);
    }

    if (memoryData != NULL) {
        mg_ws_send(client, memoryData, strlen(memoryData), WEBSOCKET_OP_TEXT);
        free(memoryData);
    }
}

void webSocketEventHandler(struct mg_connection *client, const int event, void *event_data) {
    static unsigned long lastMetricsPushAt = 0;

    if (event == MG_EV_HTTP_MSG) {
        struct mg_http_message *message = (struct mg_http_message *) event_data;
        mg_ws_upgrade(client, message, NULL);
    } else if (event == MG_EV_WS_OPEN) {
        // Print an opening message to the console
        printf("WebSocket open. Client connected\n");
        sendMetrics(client);
        lastMetricsPushAt = mg_millis();
    } else if (event == MG_EV_WS_MSG) {
        // Accept incoming websocket messages without requiring them for updates.
        printf("WebSocket message received\n");
    } else if (event == MG_EV_POLL && client->is_websocket) {
        unsigned long now = mg_millis();
        if (now - lastMetricsPushAt >= 1000) {
            sendMetrics(client);
            lastMetricsPushAt = now;
        }
    } else if (event == MG_EV_CLOSE) {
        // Print a closing message to the console
        printf("WebSocket closed. Client disconnected\n");
    }
}
