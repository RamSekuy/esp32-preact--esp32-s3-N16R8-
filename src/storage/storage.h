#pragma once

#include "esp_err.h"

#ifdef __cplusplus
extern "C" {
#endif
    esp_err_t nvs_init(void);
    esp_err_t littleFS_init(void);
    
#ifdef __cplusplus
}
#endif
