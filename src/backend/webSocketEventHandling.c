#include "mongoose.h"
#include "getSystemMetrics.h"
#include "string.h"
#include "mongoose/mongoose.h"

void webSocketEventHandler(struct mg_connection *client, int event, void *event_data) {
    if (event == MG_EV_WS_OPEN) {
        char *welcomeMessage = "You have successfully been connected";
        mg_ws_send(client, welcomeMessage, strlen(welcomeMessage), WEBSOCKET_OP_TEXT);
    } else if (event == MG_EV_WS_MSG) {
        char CPUData[1024] = {0};
        char diskData[1024] = {0};
        char memoryData[1024] = {0};
        strcat(CPUData, getCPUUsage());
        strcat(diskData, getDiskUsage());
        strcat(memoryData, getMemoryUsage());
        mg_ws_send(client, CPUData, strlen(CPUData), WEBSOCKET_OP_TEXT);
        mg_ws_send(client, diskData, strlen(diskData), WEBSOCKET_OP_TEXT);
        mg_ws_send(client, memoryData, strlen(memoryData), WEBSOCKET_OP_TEXT);

    } else if(event == MG_EV_CLOSE) {
        // close the connection
    }
}
