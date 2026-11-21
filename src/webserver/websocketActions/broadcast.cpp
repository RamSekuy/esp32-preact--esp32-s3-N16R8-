#include "websocketActions.h"

#include <string.h>
#include "esp_log.h"
#include "lwip/opt.h"

#define TAG "WEBSOCKET_UTILS"

void WebSocketUtils::broadcast(
    httpd_handle_t server,
    const char *message
) {
    size_t client_count = CONFIG_LWIP_MAX_SOCKETS;
    int client_fds[CONFIG_LWIP_MAX_SOCKETS];

    if (httpd_get_client_list(
        server,
        &client_count,
        client_fds
    ) != ESP_OK) {
        return;
    }

    httpd_ws_frame_t frame = {};
    frame.type = HTTPD_WS_TYPE_TEXT;
    frame.payload = (uint8_t *)message;
    frame.len = strlen(message);

    for (size_t i = 0; i < client_count; i++) {
        int fd = client_fds[i];

        if (httpd_ws_get_fd_info(server, fd) !=
            HTTPD_WS_CLIENT_WEBSOCKET) {
            continue;
        }

        esp_err_t err = httpd_ws_send_frame_async(
            server,
            fd,
            &frame
        );

        if (err != ESP_OK) {
            ESP_LOGW(TAG, "Failed to send to fd: %d, error: %s",
                fd, esp_err_to_name(err));
        }
    }
}