#ifndef RTOS_TASKS
#define RTOS_TASKS
#include "ecg_shared_vals.h"
#include <Arduino.h>

#define QUEUE_LENGTH 100   // Max number of samples stored in queue
#define QUEUE_ITEM_SIZE sizeof(ECGDataBatch)  // Storing each ECG sample as uint16_t
#define ECG_BATCH_SIZE 10

extern TaskHandle_t TaskECGHandle;
extern TaskHandle_t TaskBLEHandle;
extern volatile unsigned long currentMillis;
extern QueueHandle_t ble_queue;

// 10 samples (20 bytes) + timestamp (4 bytes) = 24 bytes
struct ECGDataBatch  {
    uint16_t ecg_samples[10];
    uint32_t timestamp;
};

void TaskBLE(void* pvParameters);
void TaskECG(void* pvParameters);

void startTasks(EcgSharedValues* sharedValues);  // start both ECG + BLE tasks
#endif