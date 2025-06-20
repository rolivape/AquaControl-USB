#ifndef ROL_MANAGER_H
#define ROL_MANAGER_H

#ifdef __cplusplus
extern "C"
{
#endif

    // Inicializa el rol (lee desde NVS o muestra menú)
    int rol_manager_init(void);

    // Si quieres agregar más funciones, decláralas aquí
    // void rol_manager_save_rol(int rol);

#ifdef __cplusplus
}
#endif

#endif