#ifndef BLE_SERVICE
#define BLE_SERVICE

#include <Arduino.h>


enum class BLEInitStatus {
  SUCCESS,
  SERVER_FAILURE,
  SERVICE_FAILURE,
  CHARACTERISTIC_FAILURE
};

BLEInitStatus initBLE();
void updateBLE(unsigned long seconds);
extern bool deviceConnected;

#endif