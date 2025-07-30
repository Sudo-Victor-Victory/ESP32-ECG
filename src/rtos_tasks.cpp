#include "rtos_tasks.h"
#include "FIR_Filter.h"
#include "Kalman_Filter.h"
#include "ble_service.h"


// SHARED VALUE between cores. Just a test.
volatile unsigned long currentMillis = 0;

float ecg_sample;
float filtered_ecg = 0.0;

static unsigned long last_print = 0;
int print_delay = 20;

void startTasks(EcgSharedValues* sharedValues){
// Pins ECG processing to Core 1
  xTaskCreatePinnedToCore(TaskECG, "ECGTask", 4096, sharedValues, 1, &TaskECGHandle, 1);
  vTaskSuspend(TaskECGHandle);  

  // Pins BLE processing to Core 0
  xTaskCreatePinnedToCore(TaskBLE, "BLETask", 4096,  sharedValues, 1, &TaskBLEHandle, 0);
  
  Serial.println("All setup complete. Resuming ECG task...");
  delay(100);  // Required to give time for pin setup
  vTaskResume(TaskECGHandle);
}



void TaskBLE(void* pvParameters) {
  Serial.printf("[BLE Task] Running on core %d\n", xPortGetCoreID());

  while (true) {
    uint16_t ecg_value;
    // Dequeues everything without blocking the queue
    while (xQueueReceive(ble_queue, &ecg_value, 0) == pdTRUE) {
      Serial.printf("[BLE Task] ECG value sent: %u\n", ecg_value);
    }

    // Update BLE every second (or adjust as needed)
    updateBLE(currentMillis / 1000);

    // 50 ticks was chosen to be 20x faster than TaskECG's send rate. TEMPORARY.
    vTaskDelay(pdMS_TO_TICKS(50)); // 50ms or tune this
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
      // Converts a 32 bit float into a 16 bit int. May modify ecg_sample's type.
      uint16_t converted_sample = (uint16_t)(kalman_ecg * 1000);
      if( xQueueSend(ble_queue, &converted_sample, 0) != pdTRUE) {
            // Queue full, handle overflow here if needed
            Serial.println("ECG queue full! Dropping sample.");
        }

    }

    vTaskDelayUntil(&xLastWakeTime, xFrequency);
  }
}