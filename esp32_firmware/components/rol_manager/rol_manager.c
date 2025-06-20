#include "rol_manager.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "nvs_flash.h"
#include "nvs.h"

#define TIMEOUT_MS 5000 // 5 segundos

static void guardar_rol_nvs(int rol)
{
    nvs_handle_t handle;
    esp_err_t err = nvs_open("config", NVS_READWRITE, &handle);
    if (err != ESP_OK)
    {
        printf("Error abriendo NVS para guardar rol: %d\n", err);
        return;
    }

    err = nvs_set_i32(handle, "modulo_rol", rol);
    if (err != ESP_OK)
    {
        printf("Error estableciendo rol en NVS: %d\n", err);
        nvs_close(handle);
        return;
    }

    err = nvs_commit(handle);
    if (err != ESP_OK)
    {
        printf("Error confirmando cambios en NVS: %d\n", err);
    }

    nvs_close(handle);
    printf("Rol guardado en NVS: %d\n", rol);
}

static int leer_rol_nvs(void)
{
    nvs_handle_t handle;
    int32_t rol = 0;
    esp_err_t err = nvs_open("config", NVS_READONLY, &handle);
    if (err != ESP_OK)
    {
        printf("Error abriendo NVS para leer rol: %d\n", err);
        return 0;
    }

    err = nvs_get_i32(handle, "modulo_rol", &rol);
    if (err != ESP_OK)
    {
        printf("No se encontró rol en NVS o error: %d\n", err);
        nvs_close(handle);
        return 0;
    }

    nvs_close(handle);
    return rol;
}

static int pedir_rol_usuario(void)
{
    int intentos = 0;
    const int MAX_INTENTOS = 3;
    while (intentos < MAX_INTENTOS)
    {
        char c = 0;
        char str[16] = {0};
        int pos = 0;

        printf("\n------------------------------------\n");
        printf("Introduce tu rol (1=AC, 2=DC, 3=IO) y presiona ENTER: [Tienes %d segundos para responder]\n", TIMEOUT_MS / 1000);

        int elapsed_ms = 0;
        while (elapsed_ms < TIMEOUT_MS && pos < (sizeof(str) - 1))
        {
            c = getchar();
            if (c != 0xFF)
            {
                if (c == '\n' || c == '\r')
                    break;
                str[pos++] = c;
                printf("%c", c); // eco
            }
            vTaskDelay(100 / portTICK_PERIOD_MS);
            elapsed_ms += 100;
        }

        if (pos == 0)
        {
            printf("\nTimeout de %d segundos alcanzado. No se recibió entrada.\n", TIMEOUT_MS / 1000);
            intentos++;
            continue;
        }

        str[pos] = '\0';
        int option = atoi(str);

        switch (option)
        {
        case 1:
            printf("\nSeleccionaste MODULO_AC\n");
            return 1;
        case 2:
            printf("\nSeleccionaste MODULO_DC\n");
            return 2;
        case 3:
            printf("\nSeleccionaste MODULO_IO\n");
            return 3;
        default:
            printf("\nRol no válido, intenta de nuevo (%d/3).\n", intentos + 1);
            intentos++;
            break;
        }
    }

    printf("\nDemasiados intentos fallidos o timeout. Usando valor por defecto (MODULO_AC).\n");
    return 1; // Valor por defecto
}

int rol_manager_init(void)
{
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        nvs_flash_erase();
        nvs_flash_init();
    }

    int rol = leer_rol_nvs();

    if (rol < 1 || rol > 3)
    {
        rol = pedir_rol_usuario();
        guardar_rol_nvs(rol);
    }
    else
    {
        printf("\nRol previamente configurado (guardado en NVS): ");
        switch (rol)
        {
        case 1:
            printf("MODULO_AC\n");
            break;
        case 2:
            printf("MODULO_DC\n");
            break;
        case 3:
            printf("MODULO_IO\n");
            break;
        }
        printf("¿Desea cambiar el rol? Ingrese nuevo valor (1=AC, 2=DC, 3=IO) y ENTER en los próximos 5 segundos,\no espere para continuar:\n");

        char c = 0;
        char str[16] = {0};
        int pos = 0;
        int elapsed_ms = 0;

        while (elapsed_ms < TIMEOUT_MS && pos < (sizeof(str) - 1))
        {
            c = getchar();
            if (c != 0xFF)
            {
                if (c == '\n' || c == '\r')
                    break;
                str[pos++] = c;
                printf("%c", c);
            }
            vTaskDelay(100 / portTICK_PERIOD_MS);
            elapsed_ms += 100;
        }

        if (pos > 0)
        {
            str[pos] = '\0';
            int new_rol = atoi(str);
            if (new_rol >= 1 && new_rol <= 3)
            {
                rol = new_rol;
                guardar_rol_nvs(rol);
                printf("\nNuevo rol guardado: %d\n", rol);
            }
            else
            {
                printf("\nInput inválido. Usando el rol anterior.\n");
            }
        }
        else
        {
            printf("\nNo se ingresó nuevo rol. Continuando con el rol previo.\n");
        }
    }
    return rol;
}