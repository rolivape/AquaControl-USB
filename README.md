# AquaControl-USB

Sistema modular de automatización para acuarios marinos y de arrecife, basado en ESP32-S3 y comunicación USB CDC con un controlador central (Raspberry Pi).  
Este proyecto sigue una arquitectura robusta, escalable y local-first, con paneles dedicados (AC, DC, IO) seleccionables por menuconfig.

### Estructura del proyecto

- **main/**: Entrada principal y ciclo de aplicación.
- **hw/**: Abstracciones y drivers hardware para cada panel.
- **logic/**: Máquina de estados y lógica central (NORMAL/SAFE).
- **json/**: Serialización/deserialización de mensajes JSON (cJSON).
- **usb_cdc/**: Manejo de comunicación USB CDC nativa.

### Acuerdos principales

- Solo ESP-IDF oficial (sin PlatformIO).
- Selección de panel por menuconfig (AC, DC, IO).
- Evento crítico ("911") solo disparado por hardware (DIP, jumper, botón).
- SAFE MODE embebido en cada panel.
- Validación cruzada mediante scripts Python y pruebas hardware.

**Ver documentación técnica completa y acuerdos en `/docs/` si aplica.**

---

## 2. **.gitignore**

```gitignore
# ESP-IDF build artifacts
build/
sdkconfig
sdkconfig.old
*.pyc
__pycache__/
.vscode/
.idea/
*.log
*.swp
*.DS_Store
.env/