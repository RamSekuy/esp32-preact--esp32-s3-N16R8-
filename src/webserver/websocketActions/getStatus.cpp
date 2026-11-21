#include "websocketActions.h"

#include <stdio.h>
#include "esp_heap_caps.h"

void WebSocketActions::getStatus(char *buffer, size_t bufferSize) {
    // Internal RAM
    uint32_t internalTotal =
        heap_caps_get_total_size(MALLOC_CAP_INTERNAL);

    uint32_t internalFree =
        heap_caps_get_free_size(MALLOC_CAP_INTERNAL);

    // PSRAM
    uint32_t psramTotal =
        heap_caps_get_total_size(MALLOC_CAP_SPIRAM);

    uint32_t psramFree =
        heap_caps_get_free_size(MALLOC_CAP_SPIRAM);

    snprintf(buffer, bufferSize,
        "{"
            "\"type\":\"event\","
            "\"action\":\"status_update\","
            "\"payload\":{"
                "\"ram\":{"
                    "\"internal\":{"
                        "\"total\":%lu,"
                        "\"free\":%lu"
                    "},"
                    "\"psram\":{"
                        "\"total\":%lu,"
                        "\"free\":%lu"
                    "}"
                "}"
            "}"
        "}",
        (unsigned long)internalTotal,
        (unsigned long)internalFree,
        (unsigned long)psramTotal,
        (unsigned long)psramFree
    );
}