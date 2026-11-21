
#include "websocket.h"

#include <stdlib.h>
#include <string.h>

#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

static const char *TAG = "WEBSOCKET";

#define WS_URI "/ws"

// --------------------------------------------------
// WEBSOCKET HANDLER
// --------------------------------------------------

static esp_err_t websocket_handler(httpd_req_t *req)
{
    // WebSocket handshake
    if (req->method == HTTP_GET) {
        ESP_LOGI(TAG, "Client connected");

        return ESP_OK;
    }

    // Read incoming WebSocket frame header
    httpd_ws_frame_t ws_pkt = {};
    ws_pkt.type = HTTPD_WS_TYPE_TEXT;

    esp_err_t err = httpd_ws_recv_frame(
        req,
        &ws_pkt,
        0
    );

    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Failed to receive frame header: %s",
                 esp_err_to_name(err));
        return err;
    }

    // Allocate payload buffer
    size_t len = ws_pkt.len;

    if (len > 1024) {
        ESP_LOGW(TAG, "Frame too large: %u",
                 static_cast<unsigned>(len));

        return ESP_FAIL;
    }

    uint8_t *payload = nullptr;

    if (len > 0) {
        payload = static_cast<uint8_t *>(malloc(len + 1));

        if (!payload) {
            ESP_LOGE(TAG, "Memory allocation failed");
            return ESP_ERR_NO_MEM;
        }

        ws_pkt.payload = payload;

        // Receive actual frame payload
        err = httpd_ws_recv_frame(
            req,
            &ws_pkt,
            len
        );

        if (err != ESP_OK) {
            ESP_LOGE(TAG, "Failed to receive frame: %s",
                     esp_err_to_name(err));

            free(payload);
            return err;
        }

        payload[ws_pkt.len] = '\0';
    }

    // Handle incoming frame
    switch (ws_pkt.type) {

        case HTTPD_WS_TYPE_TEXT:
            ESP_LOGI(TAG, "Received: %s",
                     payload ? reinterpret_cast<char *>(payload) : "");

            websocketActionHandler(req,reinterpret_cast<char *>(payload));
            break;

        case HTTPD_WS_TYPE_BINARY:
            ESP_LOGI(TAG, "Received binary frame: %u bytes",
                     static_cast<unsigned>(ws_pkt.len));

            // Binary frames are currently ignored
            break;

        case HTTPD_WS_TYPE_CLOSE:
            ESP_LOGI(TAG, "Client disconnected");
            break;

        case HTTPD_WS_TYPE_PING:
            ESP_LOGI(TAG, "Received ping");
            break;

        case HTTPD_WS_TYPE_PONG:
            ESP_LOGI(TAG, "Received pong");
            break;

        default:
            ESP_LOGW(TAG, "Unknown frame type");
            break;
    }

    free(payload);

    return err;
}

// --------------------------------------------------
// REGISTER WEBSOCKET ROUTE / Task
// --------------------------------------------------

esp_err_t websocket_register(httpd_handle_t server)
{
    if (server == nullptr) {
        ESP_LOGE(TAG, "Invalid HTTP server handle");
        return ESP_ERR_INVALID_ARG;
    }

    const httpd_uri_t ws_route = {
    .uri = WS_URI,
    .method = HTTP_GET,
    .handler = websocket_handler,
    .user_ctx = nullptr,
    .is_websocket = true,
    .handle_ws_control_frames = false,
    .supported_subprotocol = nullptr
    };

    esp_err_t err = httpd_register_uri_handler(
        server,
        &ws_route
    );

    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Failed to register WebSocket route");
        return err;
    }

    ESP_LOGI(TAG, "WebSocket endpoint registered: %s", WS_URI);

    BaseType_t task_created = xTaskCreate(
    statusTask,
    "status_task",
    4096,
    server,
    5,
    NULL
);

    if (task_created != pdPASS) {
    ESP_LOGE(TAG, "Failed to create status task");
    return ESP_FAIL;
}

    return ESP_OK;
}
