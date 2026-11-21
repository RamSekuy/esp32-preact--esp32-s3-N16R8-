
#ifndef WEBSOCKET_H
#define WEBSOCKET_H

#include "esp_err.h"
#include "esp_http_server.h"

// Register WebSocket route to existing HTTP server
esp_err_t websocket_register(httpd_handle_t server);

esp_err_t websocketActionHandler(httpd_req_t *req,const char *payload);

void statusTask(httpd_handle_t server);

#endif
