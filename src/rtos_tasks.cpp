#include "rtos_tasks.h"
#include "FIR_Filter.h"
#include "Kalman_Filter.h"
#include "ble_service.h"


// These pins are for the ESP-WROOM-32.

TickType_t xLastWakeTime = xTaskGetTickCount();


// SHARED VALUE between cores. Just a test.
volatile unsigned long currentMillis = 0;
float ecg_sample;

// Chose 2.0 & 20.0 to reduce noise at the cost of slightly flattening the QRS.



float filtered_ecg = 0.0;


static unsigned long last_print = 0;
int print_delay = 20;

void startTasks(EcgSharedValues* sharedValues){
// Pins ECG processing to Core 1
  xTaskCreatePinnedToCore(TaskECG, "ECGTask", 4096, &sharedValues, 1, &TaskECGHandle, 1);
  vTaskSuspend(TaskECGHandle);  

  // Pins BLE processing to Core 0
  xTaskCreatePinnedToCore(TaskBLE, "BLETask", 4096,  &sharedValues, 1, &TaskBLEHandle, 0);
  
  Serial.println("All setup complete. Resuming ECG task...");
  vTaskResume(TaskECGHandle);
}



void TaskBLE(void* pvParameters) {
  Serial.printf("[BLE Task] Running on core %d\n", xPortGetCoreID());
  while (true) {
    updateBLE(currentMillis / 1000);
    vTaskDelay(pdMS_TO_TICKS(1000)); // 1 second delay
  }
}


void TaskECG(void* pvParameters) {
    EcgSharedValues* shared = static_cast<EcgSharedValues*>(pvParameters);
    Serial.printf("[ECG Task] Running on core %d\n", xPortGetCoreID());

    const TickType_t xFrequency = pdMS_TO_TICKS(4); // 4ms period = 250 Hz
    TickType_t xLastWakeTime = xTaskGetTickCount();

  while (true) {
    int lo_plus_val = digitalRead(shared->lo_plus);
    int lo_minus_val = digitalRead(shared->lo_minus);

    if (lo_plus_val == HIGH || lo_minus_val == HIGH) {
      Serial.printf("Lead detection failure: LO+ %d and LO- %d\n", lo_plus_val, lo_minus_val);
      delay(5);  
    } 
    else {
      ecg_sample = (float)analogRead(shared->ecg_output_pin);
      shared->fir->process(&ecg_sample, &filtered_ecg, 1);
      float kalman_ecg = shared->kalman->update_k(filtered_ecg);

      if (millis() - last_print >= print_delay) {
        Serial.printf(">kalmanVal:%.2f\n", kalman_ecg);
        last_print = millis();
      }

      currentMillis = millis();
    }

    vTaskDelayUntil(&xLastWakeTime, xFrequency);
  }
}