#include <Arduino.h>
#include "Kalman_Filter.h"
#include "FIR_Filter.h"
#include "ble_service.h"

// These pins are for the ESP-WROOM-32.
int ecg_output_pin = 34;  // TX2
int lo_minus = 19;    // D5
int lo_plus = 18;     // D18

// CURRENT FIR settings:
// sampling rate: 250 Hz, cutoff 20.00 Hz, Transition 15.00
// window: hamming
const int num_of_coe = 53;
float ecg_sample;
float filtered_ecg = 0.0;

// Chose 2.0 & 20.0 to reduce noise at the cost of slightly flattening the QRS.
float kalman_q = 2.0f;
float kalman_r = 20.0f;


FIR_Filter fir_filter;
KalmanFilter ecg_kalman(kalman_q, kalman_r);  // Q and R

static unsigned long last_print = 0;
int print_delay = 20;

TaskHandle_t TaskECGHandle = NULL;
TaskHandle_t TaskBLEHandle = NULL;
// SHARED VALUE between cores. Just a test.
volatile unsigned long currentMillis = 0;

void TaskBLE(void* pvParameters) {
  Serial.printf("[BLE Task] Running on core %d\n", xPortGetCoreID());
  while (true) {
    updateBLE(currentMillis / 1000);
    vTaskDelay(pdMS_TO_TICKS(1000)); // 1 second delay
  }
}
TickType_t xLastWakeTime = xTaskGetTickCount();
void TaskECG(void* pvParameters) {
  Serial.printf("[ECG Task] Running on core %d\n", xPortGetCoreID());

  const TickType_t xFrequency = pdMS_TO_TICKS(4); // 4ms period = 250 Hz
  TickType_t xLastWakeTime = xTaskGetTickCount();

  while (true) {
    int lo_plus_val = digitalRead(lo_plus);
    int lo_minus_val = digitalRead(lo_minus);

    if (lo_plus_val == HIGH || lo_minus_val == HIGH) {
      Serial.printf("Lead detection failure: LO+ %d and LO- %d\n", lo_plus_val, lo_minus_val);
      delay(5);  
    } 
    else {
      ecg_sample = (float)analogRead(ecg_output_pin);
      fir_filter.process(&ecg_sample, &filtered_ecg, 1);
      float kalman_ecg = ecg_kalman.update_k(filtered_ecg);

      if (millis() - last_print >= print_delay) {
        Serial.printf(">kalmanVal:%.2f\n", kalman_ecg);
        last_print = millis();
      }

      currentMillis = millis();
    }

    vTaskDelayUntil(&xLastWakeTime, xFrequency);
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(lo_minus, INPUT);
  pinMode(lo_plus, INPUT);

  analogReadResolution(12);        // Uses the ESP's 12 bit ADC
  analogSetAttenuation(ADC_11db);  // Uses ESP's default ref. voltage


  // Initialize FIR filter (If it fails good luck it's hard to debug </3)
  if (!fir_filter.init()) {
    Serial.println("FIR filter init failed. Halting.");
    while (true) delay(1000);
  }

  BLEInitStatus ble_status = initBLE();
  if (ble_status != BLEInitStatus::SUCCESS) {
    Serial.println("BLE initialization failed. Halting.");
    while (true) {
      delay(1000);
    }   
  }

  // Pins ECG processing to Core 1
  xTaskCreatePinnedToCore(TaskECG, "ECGTask", 4096, NULL, 1, &TaskECGHandle, 1);
  vTaskSuspend(TaskECGHandle);  

  // Pins BLE processing to Core 0
  xTaskCreatePinnedToCore(TaskBLE, "BLETask", 4096, NULL, 1, &TaskBLEHandle, 0);
  
  Serial.println("All setup complete. Resuming ECG task...");
  vTaskResume(TaskECGHandle);
}

void loop() {

}