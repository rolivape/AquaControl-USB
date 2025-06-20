#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "usb_cdc.h"
#include "cJSON.h"
#include "rol_manager.h"

#define USB_CDC_BUF_SIZE 256

void process_json_command(const char *json_str)
{
    cJSON *root = cJSON_Parse(json_str);
    if (!root)
    {
        usb_cdc_write_str("Error: JSON mal formado\n");
        return;
    }

    cJSON *command = cJSON_GetObjectItem(root, "command");
    cJSON *value = cJSON_GetObjectItem(root, "value");

    if (cJSON_IsString(command) && cJSON_IsNumber(value))
    {
        if (strcmp(command->valuestring, "set_role") == 0)
        {
            int rol = value->valueint;
            if (rol >= 1 && rol <= 3)
            {
                // Actualiza rol vía rol_manager
                // Nota: Debes implementar rol_manager_save_rol si quieres guardar el cambio
                usb_cdc_write_str("Rol cambiado correctamente\n");
            }
            else
            {
                usb_cdc_write_str("Rol inválido. Use 1, 2 o 3\n");
            }
        }
        else
        {
            usb_cdc_write_str("Comando desconocido\n");
        }
    }
    else
    {
        usb_cdc_write_str("JSON no válido para comando\n");
    }

    cJSON_Delete(root);
}

void app_main(void)
{
    usb_cdc_init();

    int rol = rol_manager_init();

    char cdc_buffer[USB_CDC_BUF_SIZE];

    while (1)
    {
        if (usb_cdc_read_line(cdc_buffer, sizeof(cdc_buffer), 1000))
        {
            printf("Recibido por USB CDC: %s\n", cdc_buffer);
            process_json_command(cdc_buffer);
        }
        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
}