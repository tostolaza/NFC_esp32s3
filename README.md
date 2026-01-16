# 🔐 ESP32-S3 + PN532 — NFC / MIFARE Audit Toolkit

![Platform](https://img.shields.io/badge/platform-ESP32--S3-blue)
![NFC](https://img.shields.io/badge/NFC-PN532-green)
![Protocol](https://img.shields.io/badge/ISO-14443A-orange)
![License](https://img.shields.io/badge/license-Educational%20Use%20Only-red)
![Status](https://img.shields.io/badge/status-Stable-success)

> 🧪 **Herramienta de auditoría NFC / MIFARE** basada en **ESP32-S3 + PN532**, orientada a  
> **reconocimiento, análisis y pre-auditoría** antes de usar herramientas avanzadas como **Proxmark3**.

⚠️ **Uso exclusivo para aprendizaje, investigación y auditorías con autorización.**

---

## ✨ Características

### 🆔 Identificación NFC
- Lectura de **UID** (4 / 7 bytes)
- Detección **ISO14443A**
- Clasificación automática:
  - MIFARE Classic
  - Ultralight
  - NTAG

### 🏷️ Detección NTAG exacta
- Lectura del **Capability Container (CC)**
- Identificación precisa:
  - **NTAG213**
  - **NTAG215**
  - **NTAG216**
- Cálculo de memoria útil

### 🔐 Auditoría MIFARE Classic
- Autenticación con **claves por defecto**
- Lectura de **sector trailers**
- Decodificación de **Access Bits (C1, C2, C3)**
- Detección de:
  - Sectores vulnerables
  - Sectores correctamente protegidos

# 🔌 Hardware requerido

- ESP32-S3 DevKitC-1
- Módulo PN532 (modo **I2C**)
- Tarjetas NFC:
  - MIFARE Classic 1K / 4K
  - NTAG213 / 215 / 216

### 📐 Conexión PN532 (I2C)

| PN532 | ESP32-S3 |
|------|----------|
| VCC  | 3V3 |
| GND  | GND |
| SDA  | GPIO 8 |
| SCL  | GPIO 9 |
| IRQ  | — |
| RSTO | — |

> ⚠️ PN532 en **modo I2C**  
> ⚠️ **NO usar 5V**

---

## 🧩 Software

### ESP32
- Arduino IDE
- Librerías:
  - `Adafruit PN532`
  - `Adafruit BusIO`

### Linux (monitor serie)
Recomendado:
```bash
picocom -b 115200 /dev/ttyACM0

