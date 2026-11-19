#include "storage.h"

#include "nvs_flash.h"
#include "esp_littlefs.h"
#include "esp_log.h"

static const char *TAG = "STORAGE";

esp_err_t nvs_init(void){
    esp_err_t err = nvs_flash_init();

    if (err == ESP_ERR_NVS_NO_FREE_PAGES ||
        err == ESP_ERR_NVS_NEW_VERSION_FOUND) {

        ESP_LOGW(TAG, "Erasing NVS partition");

        ESP_ERROR_CHECK(nvs_flash_erase());

        err = nvs_flash_init();
    }

    return err;
}

esp_err_t littleFS_init(void){
    esp_vfs_littlefs_conf_t conf = {};

    conf.base_path = "/littlefs";
    conf.partition_label = "storage";
    conf.format_if_mount_failed = true;
    conf.dont_mount = false;

    conf.partition = nullptr;
    conf.blockdev = nullptr;
    conf.read_only = false;
    conf.grow_on_mount = false;

    esp_err_t err = esp_vfs_littlefs_register(&conf);

    if (err != ESP_OK) {
        ESP_LOGE(TAG, "LittleFS mount failed: %s",
                 esp_err_to_name(err));
        return err;
    }

    size_t total = 0;
    size_t used = 0;

    err = esp_littlefs_info("storage", &total, &used);

    if (err == ESP_OK) {
        ESP_LOGI(TAG, "LittleFS total: %u bytes",
                 (unsigned)total);
        ESP_LOGI(TAG, "LittleFS used: %u bytes",
                 (unsigned)used);
    }

    return ESP_OK;
}
