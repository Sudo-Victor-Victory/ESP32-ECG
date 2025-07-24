#ifndef BLE_SERVICE
#define BLE_SERVICE

#include <Arduino.h>


bool initBLE();
void updateBLE(unsigned long seconds);
extern bool deviceConnected;

#endif