
#include "websocket.h"
#include "websocketActions/websocketActions.h"
#include "esp_log.h"

#include <string.h>

#include "esp_system.h"
#include "esp_heap_caps.h"

#define TAG "WEBSOCKET_ACTION_HANDLER"

WebSocketActions actions;
WebSocketUtils utils;


// --------------------------------------------------
// WEBSOCKET ACTION HANDLER
// --------------------------------------------------
esp_err_t websocketActionHandler(
    httpd_req_t *req,
    const char *payload
) {
    if (strcmp(payload, "get_status") == 0) {
        ESP_LOGI(TAG, "Action: Get Status");
        return ESP_OK;
    }
    else if (strcmp(payload, "restart") == 0) {
        ESP_LOGI(TAG, "Action: Restart");

        // esp_restart();
        return ESP_OK;
    }

    ESP_LOGW(TAG, "Unknown action: %s", payload);

    return ESP_OK;
}


// --------------------------------------------------
// WEBSOCKET TASK
// --------------------------------------------------

void statusTask(httpd_handle_t server) {
    char payload[512];

    while (true) {
        actions.getStatus(payload, sizeof(payload));
        utils.broadcast(server,payload);

        vTaskDelay(pdMS_TO_TICKS(10000));
    }
}