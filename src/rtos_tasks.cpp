#include "rtos_tasks.h"
#include "FIR_Filter.h"
#include "Kalman_Filter.h"
#include "ble_service.h"



// SHARED VALUE between cores. Just a test.
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
  ECGDataBatch dequeued_ecg_data;
  while (true) {
    // Prevents task from further dequeueing any data until a client is connected.
    if (!deviceConnected) {
      Serial.println("[BLE Task] No device connected. Halting.");
      vTaskDelay(pdMS_TO_TICKS(100)); 
      continue;
    }
    uint16_t ecg_value;
    if (xQueueReceive(ble_queue, &dequeued_ecg_data, portMAX_DELAY) == pdTRUE) {
      // Send full struct (samples + timestamp) as binary. 
      updateBLE((uint8_t*)&dequeued_ecg_data, sizeof(dequeued_ecg_data));  
    
      Serial.printf("[BLE Task] Dequeued RTOS queue starting at time: %lu\n", dequeued_ecg_data.timestamp);
    }

    // 10 ticks was chosen to give the bluetooth chip space to send 10 batches every 10 ms, and possibly sent heart rate data.
    vTaskDelay(pdMS_TO_TICKS(10));
  }
}

void TaskECG(void* pvParameters) {
  EcgSharedValues* shared = static_cast<EcgSharedValues*>(pvParameters);
  Serial.printf("[ECG Task] Running on core %d\n", xPortGetCoreID());

  const TickType_t xFrequency = pdMS_TO_TICKS(4); // 4ms period = 250 Hz
  TickType_t xLastWakeTime = xTaskGetTickCount();
  int batch_index = 0;

  // Check rtos_tasks.h for member details. 
  ECGDataBatch ecg_batch;
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
      // Converts a 32 bit float into a 16 bit int. May modify ecg_sample's type.
      uint16_t converted_sample = (uint16_t)(kalman_ecg * 1000);
      ecg_batch.ecg_samples[batch_index++] = converted_sample;
      if (batch_index >= ECG_BATCH_SIZE) {
        ecg_batch.timestamp = millis();  // Timestamp for charting
        if(deviceConnected){
          if (xQueueSend(ble_queue, &ecg_batch, 0) != pdTRUE) {
            Serial.println("ECG queue full! Dropping batch.");
          }
        }
        else {
          Serial.println("[ECG Task] No device connected. Will store the data (IMPLEMENT LATER)");
          // Call some function to save the ECG data to a form of persistent storage.
          vTaskDelay(pdMS_TO_TICKS(100)); 
        }
        batch_index = 0;
      }
    }

    vTaskDelayUntil(&xLastWakeTime, xFrequency);
  }
}