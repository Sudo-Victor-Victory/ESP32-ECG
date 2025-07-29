#ifndef RTOS_TASKS
#define RTOS_TASKS

#include "ecg_shared_vals.h"
#include <Arduino.h>

extern TaskHandle_t TaskECGHandle;
extern TaskHandle_t TaskBLEHandle;
extern volatile unsigned long currentMillis;

void startTasks(EcgSharedValues* sharedValues);  // start both ECG + BLE tasks
#endif