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
// Used an uint8_t to send the data over as binary. 
void updateBLE(uint8_t* ecg_data, int data_size);
extern bool deviceConnected;

#endif