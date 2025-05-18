# Xiaomi BLE Temperature Sensors with ESP32 and Raspberry Pi Visualization

## English

This project connects two Xiaomi BLE temperature and humidity sensors to an ESP32 microcontroller via Bluetooth. The ESP32 sends the sensor data over MQTT to a Raspberry Pi, where the data is visualized using Node-RED.

### Features
- BLE communication with Xiaomi sensors (LYWSD03MMC)
- MQTT data transfer from ESP32 to Raspberry Pi
- Node-RED dashboard visualization

### Requirements
- Xiaomi BLE temperature sensors
- ESP32 board
- Raspberry Pi with MQTT broker (Mosquitto)
- Node-RED installed on Raspberry Pi

### Usage
- Upload the `esphome.ino` code to ESP32 using Arduino IDE.
- Configure your WiFi SSID, password, and MQTT broker IP in the code.
- Start Mosquitto and Node-RED on the Raspberry Pi.
- Open the Node-RED dashboard to monitor temperature and humidity.

---

## Deutsch

Dieses Projekt verbindet zwei Xiaomi BLE Temperatur- und Feuchtigkeitssensoren via Bluetooth mit einem ESP32 Mikrocontroller. Der ESP32 sendet die Sensordaten über MQTT an einen Raspberry Pi, wo die Daten mit Node-RED visualisiert werden.

### Funktionen
- BLE-Kommunikation mit Xiaomi Sensoren (LYWSD03MMC)
- MQTT-Datenübertragung vom ESP32 zum Raspberry Pi
- Node-RED Dashboard Visualisierung

### Voraussetzungen
- Xiaomi BLE Temperatursensoren
- ESP32 Board
- Raspberry Pi mit MQTT Broker (Mosquitto)
- Node-RED auf dem Raspberry Pi installiert

### Verwendung
- Lade den `esphome.ino` Code mit der Arduino IDE auf den ESP32.
- Konfiguriere WLAN SSID, Passwort und MQTT Broker IP im Code.
- Starte Mosquitto und Node-RED auf dem Raspberry Pi.
- Öffne das Node-RED Dashboard zur Überwachung der Sensordaten.
