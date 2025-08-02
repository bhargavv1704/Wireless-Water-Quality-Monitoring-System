# Wireless Water Quality Monitor (ESP8266 + pH + DS18B20)

Simple, Wi-Fi–enabled water quality monitor built on **NodeMCU ESP8266**.  
It reads **pH** (analog) and **temperature** (DS18B20) and exposes the values over a **local HTTP endpoint**.

> Firmware: Arduino/ESP8266 (`ESP8266WebServer`, `OneWire`, `DallasTemperature`)

---

## Features
- Reads **pH** via A0 (with an external pH conditioning board).
- Reads **temperature** via DS18B20 on GPIO2 (D4) using OneWire.
- Serves a simple **HTTP API** on port 80:
  - `GET /` → `pH Value: <pH> | Temperature: <temp> °C`
- Easy to calibrate (two/three-point pH calibration supported in code).
- Designed to be extended (MQTT/Cloud, NRF24L01 links, dashboards, etc.).

---

## Hardware

| Part | Notes |
|---|---|
| NodeMCU ESP8266 (ESP-12E/12F) | 3.3V logic. Analog input A0. |
| pH probe + conditioning board | Output must be **0–1.0V** (ESP8266 ADC) or **0–3.2V** if your DevKit has an onboard divider. See **ADC scaling** note below. |
| DS18B20 temperature sensor | OneWire on **GPIO2 (D4)** + **4.7kΩ pull-up** from data to 3.3V |
| Jumper wires, breadboard | — |

### Pinout
- **DS18B20** → Data: **D4 / GPIO2** (with 4.7kΩ pull-up to 3.3V); VCC: 3.3V; GND: GND
- **pH board output** → **A0**

> ⚠️ **ADC scaling (important):**  
> `analogRead(A0)` on the ESP8266 is 10-bit (0–1023) referenced to **1.0V internal**.  
> Many NodeMCU DevKits include an onboard divider so A0 accepts **0–3.2V**. Check your board.  
> In the sketch, set `vref` accordingly (see **Calibration**).

---

## Firmware Setup

1. **Arduino IDE**
   - Boards Manager → Install **ESP8266** core.
   - Select board: **NodeMCU 1.0 (ESP-12E Module)** (or your exact variant).
2. **Libraries**
   - Install via Library Manager:
     - **ESP8266WiFi**
     - **ESP8266WebServer**
     - **OneWire**
     - **DallasTemperature**
3. **Configure Wi-Fi**
   - In the sketch, set:
     ```cpp
     const char* ssid = "your-ssid";
     const char* password = "your-password";
     ```
4. **Upload**
   - Serial Monitor at **115200 baud**.
   - After boot, note the printed **IP address**.

---

## Run & Test

- Open a browser to `http://<device-ip>/`  
  You should see:
