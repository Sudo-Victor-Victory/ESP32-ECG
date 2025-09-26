# ESP32 ECG Firmware 💾 

This repository contains the core **ESP32 firmware** for real-time ECG signal acquisition, processing, and transmission.  
It brings together **digital signal processing algorithms** such as FIR and Kalman, and **BLE connectivity** to enable a bluetooth client to receive, visualize, and analyze heart data in real time without compromising on battery life.
For an example client repo, I created [A Flutter app that accomplishes just that.](https://github.com/Sudo-Victor-Victory/ecg_app)

---

## Features 🚀

- **ECG Signal Acquisition** ❤️
  - Reads electrical activity of the heart using the SparkFun AD8232 ECG module and ESP32 ADC.  

- **Signal Processing** 📡
  - **FIR Filtering** – reduces noise and isolates relevant ECG frequency bands.  
  - **Kalman Filtering** – smooths signals and reduces motion artifacts.
  - Both increase the SNR (Signal to noise ratio)

- **Heart Analysis** 🔎
  - Real-time **BPM calculation** from ECG waveform with R-R interval calculation. 

- **System Architecture** 🏛️
  - Utilizes a dual processor setup with **FreeRTOS** tasks and queues
    - Handles sampling, processing, and transmission efficiently.  
  - Ensures ECG streaming without overloading the BLE stack.  

- **Connectivity** 🛜
  - **Bluetooth Low Energy (BLE)** GATT server for sending ECG and BPM data to a client.
  - Designed for integration with a Flutter-based ECG app.
  - Server's code is transmitted as bytes, so a client repo can be made in any BLE-friendly framework, language, or device.

---


## Hardware & Software Requirements 🛠️

### Hardware
| Component           | Description                  |
|--------------------|-----------------------------|
| ESP32 DevKit V1     | Microcontroller board        |
| SparkFun AD8232     | ECG sensor                   |
| Micro USB Cable           | To upload firmware to ESP32    |
| TPS61023 | To maintain a stable voltage (step down) | 
| TP4056 | To power the ECG32 via battery |
| 3000mAh LiPo battery | The power source of the ESP32 | 

### Software
- **Flutter** ≥ 3.0  
- **PlatformIO** for ESP32 firmware  
- Arduino framework for ESP32  


