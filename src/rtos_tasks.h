#ifndef RTOS_TASKS
#define RTOS_TASKS
#include "ecg_shared_vals.h"
#include <Arduino.h>

#define QUEUE_LENGTH 100   // Max number of samples stored in queue
#define QUEUE_ITEM_SIZE sizeof(uint16_t)  // Storing each ECG sample as uint16_t

extern TaskHandle_t TaskECGHandle;
extern TaskHandle_t TaskBLEHandle;
extern volatile unsigned long currentMillis;
extern QueueHandle_t ble_queue;

void startTasks(EcgSharedValues* sharedValues);  // start both ECG + BLE tasks
#endif