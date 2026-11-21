#include "wifi/wifi.h"
#include "webserver/webserver.h"
#include "storage/storage.h"

extern "C" void app_main() {
    nvs_init();
    littleFS_init();
    // wifi_init_softap();
    wokwi_init();
    webserver_start();
}