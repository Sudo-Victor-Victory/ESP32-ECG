#include "ble_service.h"
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

#define SERVICE_UUID        "b64cfb1e-045c-4975-89d6-65949bcb35aa"
#define CHARACTERISTIC_UUID "33737322-fb5c-4a6f-a4d9-e41c1b20c30d"

BLECharacteristic* pCharacteristic = nullptr;
bool deviceConnected = false;


class MyServerCallbacks : public BLEServerCallbacks {
  void onConnect(BLEServer* pServer) override {
    deviceConnected = true;
    Serial.println("Device connected");
  }

  void onDisconnect(BLEServer* pServer) override {
    deviceConnected = false;
    Serial.println("Peripheral discon");
    pServer->startAdvertising(); // Re-advertise
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

  BLEService* pService = pServer->createService(SERVICE_UUID);
  if (!pService) {
    Serial.println("Failed to create BLE service");
    return BLEInitStatus::SERVICE_FAILURE;
  }

  pCharacteristic = pService->createCharacteristic(
    CHARACTERISTIC_UUID,
    BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_NOTIFY
  );
  if (!pCharacteristic) {
    Serial.println("Failed to create BLE characteristic");
    return BLEInitStatus::CHARACTERISTIC_FAILURE;
  }

  pCharacteristic->addDescriptor(new BLE2902());
  pService->start();
  pServer->getAdvertising()->start();

  Serial.println("Waiting for client connection...");
  return BLEInitStatus::SUCCESS;
}

void updateBLE(unsigned long seconds) {
  if (deviceConnected && pCharacteristic) {
    String msg = "Uptime: " + String(seconds) + "s";
    pCharacteristic->setValue(msg.c_str());
    pCharacteristic->notify();
    Serial.println(msg);
  }
}