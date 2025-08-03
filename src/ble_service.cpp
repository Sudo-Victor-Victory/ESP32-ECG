#include "ble_service.h"
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

#define ECG_SERVICE_UUID        "b64cfb1e-045c-4975-89d6-65949bcb35aa"
#define ECG_CHAR_UUID           "33737322-fb5c-4a6f-a4d9-e41c1b20c30d"

#define STATUS_SERVICE_UUID     "5bd3b4b0-f359-474c-8f0c-c92d7acc654d"
#define STATUS_CHAR_UUID        "9fcc05ea-2c47-44e6-b776-ebcda7aedd50"

BLECharacteristic* ecg_characteristic = nullptr;
BLECharacteristic* conn_status_characteristic = nullptr;

BLEServer* pServer = nullptr;
volatile bool deviceConnected = false;

class MyServerCallbacks : public BLEServerCallbacks {
  void onConnect(BLEServer* pServer) override {
    deviceConnected = true; 
    conn_status_characteristic->setValue("connected");
    Serial.println("[BLE] Device connected");
    conn_status_characteristic->notify();
  }

  void onDisconnect(BLEServer* pServer) override {
    deviceConnected = false;
    conn_status_characteristic->setValue("disconnected");
    Serial.println("[BLE] Device disconnected");
    pServer->startAdvertising(); // Re-advertise that the device is open for connection
  }
};

BLEInitStatus initBLE() {
  Serial.println("ESP32 booting...");
  BLEDevice::init("Victor ESP32");

  BLEServer* pServer = BLEDevice::createServer();
  if (!pServer){
    Serial.println("Failed in initializing the server");
    return BLEInitStatus::SERVER_FAILURE;
  }
  pServer->setCallbacks(new MyServerCallbacks());

  // ECG Service 
  BLEService* ecgService = pServer->createService(ECG_SERVICE_UUID);
  if (!ecgService) {
    Serial.println("Failed to create ECG service");
    return BLEInitStatus::SERVICE_FAILURE;
  }

  // Connection status service
  BLEService* statusService = pServer->createService(STATUS_SERVICE_UUID);
  if (!statusService) {
    Serial.println("Failed to create Connection Status service");
    return BLEInitStatus::SERVICE_FAILURE;
  }

  // Characteristic creation (both)
  ecg_characteristic = ecgService->createCharacteristic(
    ECG_CHAR_UUID,
    BLECharacteristic::PROPERTY_NOTIFY | BLECharacteristic::PROPERTY_READ
  );
  if (!ecg_characteristic) {
    Serial.println("Failed to create ECG characteristic");
    return BLEInitStatus::CHARACTERISTIC_FAILURE;
  }

  conn_status_characteristic = statusService->createCharacteristic(
    STATUS_CHAR_UUID,
    BLECharacteristic::PROPERTY_NOTIFY | BLECharacteristic::PROPERTY_READ
  );
  if (!conn_status_characteristic) {
    Serial.println("Failed to create Connection Status characteristic");
    return BLEInitStatus::CHARACTERISTIC_FAILURE;
  }

  // Start service and enable notifications of data
  ecgService->start();
  statusService->start();
  ecg_characteristic->addDescriptor(new BLE2902());
  conn_status_characteristic->addDescriptor(new BLE2902());

  // Begins advertising 
  BLEAdvertising* pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->setScanResponse(false);
  // These lines specifically set the connection intervals from 7.5ms MIN to 22.5 ms MAX.
  //  Gives the client (Flutter) more space to manage connectivity.
  pAdvertising->setMinPreferred(0x06);
  pAdvertising->setMinPreferred(0x12);
  pServer->getAdvertising()->start();
  
  Serial.println("Waiting for client connection...");
  return BLEInitStatus::SUCCESS;
}

void updateBLE(uint8_t* ecg_data, int data_size) {
  if (deviceConnected && ecg_characteristic) {
    ecg_characteristic->setValue(ecg_data, data_size);
    ecg_characteristic->notify();
    Serial.printf("Sent ECG batch (%d bytes)\n", data_size);
  }
}