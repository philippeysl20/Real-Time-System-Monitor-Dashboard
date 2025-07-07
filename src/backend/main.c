#include "getSystemMetrics.h"
#include "mongoose.h"
#include "webSocketEventHandling.h"

int main(void) {
    struct mg_mgr mgr; // Declare manager object

    // Initialize the mongoose event handler
    mg_mgr_init(&mgr); // Handles incoming events, tracks all connections and runs the event loop

    // initialize the HTTP URL that the socket will listen on.
    char *WebSocket = "http://localhost:3000";

    mg_http_listen(&mgr, WebSocket, webSocketEventHandler, NULL);

    return 0;
}