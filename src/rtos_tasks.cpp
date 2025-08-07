#include "rtos_tasks.h"
#include "FIR_Filter.h"
#include "Kalman_Filter.h"
#include "ble_service.h"

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
  vTaskDelay(pdMS_TO_TICKS(100)); // Required to give time for pin setup
  vTaskResume(TaskECGHandle);

}

void TaskBLE(void* pvParameters) {
  Serial.printf("[BLE Task] Running on core %d\n", xPortGetCoreID());
  ECGDataBatch dequeued_ecg_data;
  while (true) {
    if (!deviceConnected) {
      Serial.println("[BLE Task] No device connected. Halting.");
      vTaskDelay(pdMS_TO_TICKS(1000));
      continue;
    }

    if (xQueueReceive(ble_queue, &dequeued_ecg_data, portMAX_DELAY) == pdTRUE) {
      updateBLE((uint8_t*)&dequeued_ecg_data, sizeof(dequeued_ecg_data));
    } 
    else {
      Serial.println("[BLE Task] Queue full! Dropping batch.");
    }

    vTaskDelay(pdMS_TO_TICKS(10));
  }
}

void TaskECG(void* pvParameters) {
  EcgSharedValues* shared = static_cast<EcgSharedValues*>(pvParameters);
  Serial.printf("[ECG Task] Running on core %d\n", xPortGetCoreID());

  const TickType_t xFrequency = pdMS_TO_TICKS(4);  // 250Hz
  TickType_t xLastWakeTime = xTaskGetTickCount(); 
  int batch_index = 0;
  ECGDataBatch ecg_batch;

  while (true) {
    int lo_plus_val = digitalRead(shared->lo_plus);
    int lo_minus_val = digitalRead(shared->lo_minus);

    if (lo_plus_val == HIGH || lo_minus_val == HIGH) {
      Serial.printf("Lead detection failure: LO+ %d and LO- %d\n", lo_plus_val, lo_minus_val);
      vTaskDelay(pdMS_TO_TICKS(5)); // Avoid using delay in RTOS. Use any vTaskDelay
    } 
    else {
      ecg_sample = (float)analogRead(shared->ecg_output_pin);
      shared->fir->process(&ecg_sample, &filtered_ecg, 1);
      float kalman_ecg = shared->kalman->update_k(filtered_ecg);
      // Converts a 32 bit float into a 16 bit int to save on bluetooth bandwidth. (Half of space used)
      uint16_t converted_sample = (uint16_t)round(kalman_ecg);
      if (millis() - last_print >= print_delay) {
        Serial.printf(">kalmanVal:%.2f\n", kalman_ecg);
        Serial.printf(">convertedSample:%u\n", converted_sample);
        last_print = millis();
      }
      ecg_batch.ecg_samples[batch_index++] = converted_sample;
      if (batch_index >= ECG_BATCH_SIZE) {
        ecg_batch.timestamp = xTaskGetTickCount() * portTICK_PERIOD_MS;  // Timestamp for charting
        if(deviceConnected){
          if (xQueueSend(ble_queue, &ecg_batch, 0) != pdTRUE) {
            Serial.println("ECG queue full! Dropping batch.");
          }
        }
        else {
          Serial.println("[ECG Task] No device connected. Will store the data (IMPLEMENT LATER)");
          // Call some function to save the ECG data to a form of persistent storage.
          // NOTE: 100 is too much. Will cause an unpaired ecg to look laggy on serial monitor.
          // Find a different val than 100.
          vTaskDelay(pdMS_TO_TICKS(100)); 
        }
        batch_index = 0;
      }
    }

    vTaskDelayUntil(&xLastWakeTime, xFrequency);
  }
}