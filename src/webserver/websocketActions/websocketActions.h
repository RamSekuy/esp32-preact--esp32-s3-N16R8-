#pragma once

#include "esp_http_server.h"

class WebSocketActions {
public:
    void getStatus(char *buffer, size_t bufferSize);
};

class WebSocketUtils {
public:
    void broadcast(httpd_handle_t server, const char *message);
};