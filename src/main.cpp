#include <Arduino.h>
#include "Kalman_Filter.h"
#include "FIR_Filter.h"
#include "ble_service.h"
#include "rtos_tasks.h"
#include "ecg_shared_vals.h"

// These pins are for the ESP-WROOM-32.
const int ecg_output_pin = 34;  // TX2
const int lo_minus = 19;        // D5
const int lo_plus = 18;         // D18

// CURRENT FIR settings:
// sampling rate: 250 Hz, cutoff 20.00 Hz, Transition 15.00, # of coefficients = 53
// window: hamming

FIR_Filter fir_filter;

float kalman_q = 2.0f;
float kalman_r = 20.0f;
KalmanFilter ecg_kalman(kalman_q, kalman_r);  

EcgSharedValues sharedValues = {
  .fir = &fir_filter,
  .kalman = &ecg_kalman,
  .ecg_output_pin = ecg_output_pin,
  .lo_plus = lo_plus,
  .lo_minus = lo_minus,
};

TaskHandle_t TaskECGHandle = NULL;
TaskHandle_t TaskBLEHandle = NULL;
QueueHandle_t ble_queue = NULL;

void setup() {
  Serial.begin(115200);
  pinMode(lo_plus, INPUT);
  pinMode(lo_minus, INPUT);
  analogReadResolution(12);        // Uses the ESP's 12 bit ADC
  analogSetAttenuation(ADC_11db);  // Uses ESP's default ref. voltage


  // Initialize FIR filter (If it fails good luck it's hard to debug </3)
  if (!fir_filter.init()) {
    Serial.println("FIR filter init failed. Halting.");
    while (true) {
      delay(1000);
    }
  }

  Serial.println("FIR filter initialized successfully!");

  BLEInitStatus ble_status = initBLE();
  if (ble_status != BLEInitStatus::SUCCESS) {
    Serial.println("BLE initialization failed. Halting.");
    while (true) {
      delay(1000);
    }   
  }
  ble_queue = xQueueCreate(QUEUE_LENGTH, QUEUE_ITEM_SIZE);
  if (ble_queue == NULL) {
    Serial.println("Failed to create ECG queue. Halting.");
    while (true) {
      delay(1000);
    } 
  }
  startTasks(&sharedValues);
}

void loop() {

}