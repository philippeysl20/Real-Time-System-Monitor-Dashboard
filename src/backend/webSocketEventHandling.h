#ifndef WEBSOCKETEVENTHANDLING_H
#define WEBSOCKETEVENTHANDLING_H

#include "mongoose.h"

// Function for handling websocket events
void webSocketEventHandler(struct mg_connection *client, int eventType, void *event_data);

#endif //WEBSOCKETEVENTHANDLING_H
