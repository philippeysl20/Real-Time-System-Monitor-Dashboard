#include "mongoose.h"
#include "webSocketEventHandling.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "globals.h"

int main(void) {
    struct mg_mgr mgr; // Declare manager object

    // Initialize the mongoose event handler
    mg_mgr_init(&mgr); // Handles incoming events, tracks all connections and runs the event loop

    // initialize the HTTP URL that the socket will listen on.
    const char *WebSocket = "http://localhost:3000";

    // The mg_http_listen function takes 4 parameters. The first being the manager object, the second
    // being the websocket itself, the third being an event handler function which has been created
    // in webSocketEventHandler.c, and the third being data about the user connected, which is optional
    // and can be set as NULL.
    mg_http_listen(&mgr, WebSocket, webSocketEventHandler, NULL); // Begin listening on the URL

    // set the loop control flag to true
    running = 1;
    while (running) { // process network/events for up to 100 ms
        mg_mgr_poll(&mgr, 100);
    }

    // clean up and free Mongoose manager resources
    mg_mgr_free(&mgr);

    // exit successfully
    return 0;
}
