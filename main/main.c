#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "rol_manager.h"
void app_main(void)
{
    int rol = rol_manager_init();

    switch (rol)
    {
    case 1:
        printf("Inicializando como MODULO_AC...\n");
        break;
    case 2:
        printf("Inicializando como MODULO_DC...\n");
        break;
    case 3:
        printf("Inicializando como MODULO_IO...\n");
        break;
    default:
        printf("Rol inesperado.\n");
        break;
    }

    while (1)
    {
        vTaskDelay(5000 / portTICK_PERIOD_MS);
    }
}