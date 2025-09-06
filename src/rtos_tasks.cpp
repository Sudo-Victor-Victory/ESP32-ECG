#include "FIR_Filter.h"
#include "Kalman_Filter.h"
#include "ble_service.h"
#include "rtos_tasks.h"

float ecg_sample;
float filtered_ecg = 0.0;

static unsigned long last_print = 0;
int print_delay = 20;

//                                            Variables for calculating BPM. 
static uint32_t last_r_peak_time = 0;
// QRS has  to be greater than this value to be considered for BPM.
const float R_THRESHOLD = 2800.0; 
const int REFRACTORY_MS = 300;     // Ignore peaks within 300ms
const int BPM_HISTORY_SIZE = 5;  // Buffer size to calculate avg
static uint16_t bpm_history[BPM_HISTORY_SIZE] = {0};
static int bpm_index = 0;
static uint16_t last_bpm = 60;  // Default until first peak

const TickType_t REFRACTORY_TICKS = pdMS_TO_TICKS(300);

void startTasks(EcgSharedValues* sharedValues) {
  // Pins ECG processing to Core 1
  xTaskCreatePinnedToCore(TaskECG, "ECGTask", 4096, sharedValues, 2,
                          &TaskECGHandle, 1);
  vTaskSuspend(TaskECGHandle);

  // Pins BLE processing to Core 0
  xTaskCreatePinnedToCore(TaskBLE, "BLETask", 4096, sharedValues, 1,
                          &TaskBLEHandle, 0);

  vTaskDelay(pdMS_TO_TICKS(100));  // Required to give time for pin setup
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

uint16_t detectBPM_batch(const ECGDataBatch& my_batch) {
  // Find max sample + index
  uint16_t max_sample = 0;
  int max_index = 0;
  for (int i = 0; i < ECG_BATCH_SIZE; i++) {
    if (my_batch.ecg_samples[i] > max_sample) {
      max_sample = my_batch.ecg_samples[i];
      max_index = i;
    }
  }

  // Estimate peak time in ms
  unsigned long peak_time =
      my_batch.timestamp - (ECG_BATCH_SIZE - 1 - max_index) * 4;
  uint16_t bpm = last_bpm;  // fallback to last known BPM

  // Check if this is a valid R-peak (threshold + refractory)
  if (max_sample > R_THRESHOLD) {
    unsigned long rr_interval = peak_time - last_r_peak_time;
    // Avoids high BPMs from timer issue. Rare.
    if (rr_interval > 300 && rr_interval < 2000) {
      bpm = 60000 / rr_interval;
      bpm_history[bpm_index] = bpm;
      bpm_index = (bpm_index + 1) % BPM_HISTORY_SIZE;
      last_bpm = (last_bpm * 0.4) + (bpm * 0.6);
      last_r_peak_time = peak_time;
    }
  }

  return bpm;
}

void TaskECG(void* pvParameters) {
  EcgSharedValues* shared = static_cast<EcgSharedValues*>(pvParameters);
  Serial.printf("[ECG Task] Running on core %d\n", xPortGetCoreID());

  const TickType_t xFrequency = pdMS_TO_TICKS(4);  // 4ms period = 250 Hz
  TickType_t xLastWakeTime = xTaskGetTickCount();
  int batch_index = 0;
  ECGDataBatch ecg_batch;
  const int batch_size = ECG_BATCH_SIZE;

  while (true) {
    vTaskDelayUntil(&xLastWakeTime, xFrequency);

    int lo_plus_val = digitalRead(shared->lo_plus);
    int lo_minus_val = digitalRead(shared->lo_minus);
    if (lo_plus_val == HIGH || lo_minus_val == HIGH) {
      Serial.printf("Lead detection failure: LO+ %d and LO- %d\n", lo_plus_val,
                    lo_minus_val);
      vTaskDelay(
          pdMS_TO_TICKS(5));  // Avoid using delay in RTOS. Use any vTaskDelay
    } 
    else {
      // Read and filter ECG
      float ecg_sample = (float)analogRead(shared->ecg_output_pin);
      shared->fir->process(&ecg_sample, &filtered_ecg, 1);
      float kalman_ecg = shared->kalman->update_k(filtered_ecg);

      // Convert to 16-bit for BLE batch
      uint16_t converted_sample = (uint16_t)round(kalman_ecg);
      ecg_batch.ecg_samples[batch_index++] = converted_sample;

      // Send batch when full
      if (batch_index >= batch_size) {
        ecg_batch.timestamp =
            xTaskGetTickCount() * portTICK_PERIOD_MS;  // Timestamp for charting
        // Called once per batch to get an avg BPM. 
        uint16_t bpm = detectBPM_batch(ecg_batch);

        ecg_batch.bpm = bpm;  
        if (deviceConnected) {
          xQueueSend(ble_queue, &ecg_batch, 0);
        }
        batch_index = 0;
      }
    }
  }
}
