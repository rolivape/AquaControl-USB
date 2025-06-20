#pragma once
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C"
{
#endif

    void usb_cdc_init(void);
    void usb_cdc_write_str(const char *str);
    bool usb_cdc_read_line(char *buffer, size_t max_len, uint32_t timeout_ms);

#ifdef __cplusplus
}
#endif