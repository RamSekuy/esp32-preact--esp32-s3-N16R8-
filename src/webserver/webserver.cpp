#include "webserver.h"
#include "websocket.h"

#include <string.h>

#include "esp_http_server.h"
#include "esp_log.h"

static const char *TAG = "WEBSERVER";

// LittleFS mount point
#define WEB_ROOT "/littlefs"

// HTTP server instance
static httpd_handle_t server = nullptr;

static const char *get_content_type(const char *path)
{
    const char *ext = strrchr(path, '.');

    if (!ext) return "application/octet-stream";

    if (strcmp(ext, ".html") == 0)
        return "text/html";

    if (strcmp(ext, ".css") == 0)
        return "text/css";

    if (strcmp(ext, ".js") == 0)
        return "application/javascript";

    if (strcmp(ext, ".json") == 0)
        return "application/json";

    if (strcmp(ext, ".svg") == 0)
        return "image/svg+xml";

    if (strcmp(ext, ".png") == 0)
        return "image/png";

    if (strcmp(ext, ".jpg") == 0 ||
        strcmp(ext, ".jpeg") == 0)
        return "image/jpeg";

    if (strcmp(ext, ".ico") == 0)
        return "image/x-icon";

    if (strcmp(ext, ".woff2") == 0)
        return "font/woff2";

    if (strcmp(ext, ".woff") == 0)
        return "font/woff";

    return "application/octet-stream";
}

// --------------------------------------------------
// SEND FILE
// --------------------------------------------------

static esp_err_t send_file(
    httpd_req_t *req,
    const char *path
) {
    FILE *file = fopen(path, "rb");

    if (!file) {
        ESP_LOGW(TAG, "File not found: %s", path);

        httpd_resp_send_err(
            req,
            HTTPD_404_NOT_FOUND,
            "File not found"
        );

        return ESP_OK;
    }

    httpd_resp_set_type(
        req,
        get_content_type(path)
    );

    // Static files can be cached by browser.
    httpd_resp_set_hdr(
        req,
        "Cache-Control",
        "public, max-age=3600"
    );

    char buffer[1024];
    size_t read_bytes;

    while ((read_bytes = fread(
        buffer,
        1,
        sizeof(buffer),
        file
    )) > 0) {

        esp_err_t err = httpd_resp_send_chunk(
            req,
            buffer,
            read_bytes
        );

        if (err != ESP_OK) {
            ESP_LOGE(TAG, "Failed to send file");
            fclose(file);
            return err;
        }
    }

    fclose(file);

    // End chunked response
    return httpd_resp_send_chunk(req, nullptr, 0);
}

// --------------------------------------------------
// PAGE HANDLERS
// --------------------------------------------------

static esp_err_t dashboard_handler(httpd_req_t *req)
{
    return send_file(
        req,
        WEB_ROOT "/index.html"
    );
}

static esp_err_t assets_handler(httpd_req_t *req)
{
    char uri[256];

    // Copy URI safely
    size_t len = strnlen(req->uri, sizeof(uri));

    if (len >= sizeof(uri)) {
        httpd_resp_send_err(
            req,
            HTTPD_414_URI_TOO_LONG,
            "URI too long"
        );
        return ESP_OK;
    }

    memcpy(uri, req->uri, len + 1);

    // Remove query string
    char *query = strchr(uri, '?');
    if (query) *query = '\0';

    // Prevent path traversal
    if (strstr(uri, "..") ||
        strchr(uri, '\\')) {

        httpd_resp_send_err(
            req,
            HTTPD_400_BAD_REQUEST,
            "Invalid path"
        );

        return ESP_OK;
    }

    char path[512];

    int written = snprintf(
        path,
        sizeof(path),
        WEB_ROOT "%s",
        uri
    );

    if (written < 0 || written >= sizeof(path)) {
        httpd_resp_send_err(
            req,
            HTTPD_414_URI_TOO_LONG,
            "Path too long"
        );

        return ESP_OK;
    }

    return send_file(req, path);
}

// --------------------------------------------------
// REGISTER ROUTES
// --------------------------------------------------
static esp_err_t register_routes(void)
{
    const httpd_uri_t routes[] = {
        {
            .uri = "/",
            .method = HTTP_GET,
            .handler = dashboard_handler,
            .user_ctx = nullptr,
            .is_websocket = false,
            .handle_ws_control_frames = false,
            .supported_subprotocol = nullptr
        },
        {
            .uri = "/assets/*",
            .method = HTTP_GET,
            .handler = assets_handler,
            .user_ctx = nullptr,
            .is_websocket = false,
            .handle_ws_control_frames = false,
            .supported_subprotocol = nullptr
        }
    };

    for (const auto &route : routes) {
        esp_err_t err = httpd_register_uri_handler(
            server,
            &route
        );

        if (err != ESP_OK) {
            ESP_LOGE(TAG, "Failed to register route: %s", route.uri);
            return err;
        }
    }

    return ESP_OK;
}

// --------------------------------------------------
// START / STOP SERVER
// --------------------------------------------------

esp_err_t webserver_start(void)
{
    if (server != nullptr) {
        ESP_LOGW(TAG, "Server already running");
        return ESP_OK;
    }

    httpd_config_t config = HTTPD_DEFAULT_CONFIG();

    config.server_port = 80;
    config.max_uri_handlers = 12;
    config.stack_size = 6144;

    // Enable wildcard URI matching
    config.uri_match_fn = httpd_uri_match_wildcard;

    esp_err_t err = httpd_start(
        &server,
        &config
    );

    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Failed to start HTTP server");
        server = nullptr;
        return err;
    }

    err = register_routes();

    if (err != ESP_OK) {
        httpd_stop(server);
        server = nullptr;
        return err;
    }

    err = websocket_register(server);
    if (err != ESP_OK) {
        httpd_stop(server);
        server = nullptr;
        return err;
    }


    ESP_LOGI(TAG, "HTTP server started on port 80");

    return ESP_OK;
}

esp_err_t webserver_stop(void)
{
    if (server == nullptr) {
        return ESP_OK;
    }

    esp_err_t err = httpd_stop(server);

    if (err == ESP_OK) {
        server = nullptr;
        ESP_LOGI(TAG, "HTTP server stopped");
    }

    return err;
}
