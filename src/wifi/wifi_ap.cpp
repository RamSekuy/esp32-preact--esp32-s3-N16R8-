#include "wifi.h"

#include <cstring>

extern "C" {
    #include "esp_wifi.h"
    #include "esp_event.h"
    #include "esp_netif.h"
    #include "esp_log.h"
}

#define AP_SSID      "ESP32_Hotspot"
#define AP_PASSWORD  "12345678"
#define AP_CHANNEL   1
#define AP_MAX_CONN  4

static const char *TAG = "WIFI";

void wifi_init_softap() {
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());

    esp_netif_create_default_wifi_ap();

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    wifi_config_t wifi_config = {};
    strcpy((char *)wifi_config.ap.ssid, AP_SSID);
    strcpy((char *)wifi_config.ap.password, AP_PASSWORD);

    wifi_config.ap.ssid_len = strlen(AP_SSID);
    wifi_config.ap.channel = AP_CHANNEL;
    wifi_config.ap.max_connection = AP_MAX_CONN;
    wifi_config.ap.authmode = WIFI_AUTH_WPA2_PSK;

    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_AP));

    ESP_ERROR_CHECK(
        esp_wifi_set_config(WIFI_IF_AP, &wifi_config)
    );

    ESP_ERROR_CHECK(esp_wifi_start());

    ESP_LOGI(TAG, "SSID: %s", AP_SSID);
    ESP_LOGI(TAG, "Password: %s", AP_PASSWORD);
    ESP_LOGI(TAG, "IP: 192.168.4.1");
}