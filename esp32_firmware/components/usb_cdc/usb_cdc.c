#include "usb_cdc.h"
#include "tinyusb.h"
#include "class/cdc/cdc_device.h"
#include "esp_log.h"
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define TAG "USB_CDC"

void usb_cdc_init(void)
{
    ESP_LOGI(TAG, "Inicializando TinyUSB CDC...");
    tinyusb_config_t tusb_cfg = {0};
    ESP_ERROR_CHECK(tinyusb_driver_install(&tusb_cfg));
}

void usb_cdc_write_str(const char *str)
{
    tud_cdc_n_write_str(0, str);
    tud_cdc_n_write_flush(0);
}

bool usb_cdc_read_line(char *buffer, size_t max_len, uint32_t timeout_ms)
{
    static char internal_buffer[256];
    static size_t internal_idx = 0;

    uint32_t elapsed = 0;
    const uint32_t delay_per_loop = 10;

    while (elapsed < timeout_ms)
    {
        while (tud_cdc_n_available(0))
        {
            char c = tud_cdc_n_read_char(0);
            if (c == '\n' || c == '\r')
            {
                if (internal_idx > 0)
                {
                    internal_buffer[internal_idx] = '\0';
                    strncpy(buffer, internal_buffer, max_len);
                    internal_idx = 0;
                    return true;
                }
            }
            else if (internal_idx < sizeof(internal_buffer) - 1)
            {
                internal_buffer[internal_idx++] = c;
            }
        }
        vTaskDelay(delay_per_loop / portTICK_PERIOD_MS);
        elapsed += delay_per_loop;
    }
    return false;
}