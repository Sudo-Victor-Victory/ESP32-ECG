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
void updateBLE(uint16_t ecg_data);
extern bool deviceConnected;

#endif