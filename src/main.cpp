#include <Arduino.h>

extern "C" {
  #include "dsps_fir.h"
  #include "dsp_common.h"
}
// These pins are for the ESP-WROOM-32.
int ecg_output_pin = 34;  // TX2
int lo_minus = 19;    // D5
int lo_plus = 18;     // D18

int digitalVal;

float fir_coeffs[8] = {0.1, 0.15, 0.2, 0.25, 0.25, 0.2, 0.15, 0.1};
float input[8] = {1, 2, 3, 4, 3, 2, 1, 0};
float output[8];
float delay_line[8]; 
fir_f32_t fir_filter;  

void setup() {
  Serial.begin(115200);
  pinMode(lo_minus, INPUT);
  pinMode(lo_plus, INPUT);

  analogReadResolution(12);        // Uses the ESP's 12 bit ADC
  analogSetAttenuation(ADC_11db);  // Uses ESP's default ref. voltage

    delay(1000);

  // Initialize FIR filter
  dsps_fir_init_f32(&fir_filter, fir_coeffs, delay_line, 8);

  dsps_fir_f32_ae32(&fir_filter, input, output, 8);

  Serial.println("FIR output:");
  for (int i = 0; i < 8; i++) {
    Serial.println(output[i], 4);
  }
}

void loop() {
  int lo_plus_val = digitalRead(lo_plus);
  int lo_minus_val = digitalRead(lo_minus);


    // Just a simple call to test if library functions are accessible
  float input_signal[8] = {0,1,2,3,4,5,6,7};
  float output_signal[8];

  // Perform FFT or other DSP operation


  if (lo_plus_val == HIGH || lo_minus_val == HIGH) {
    Serial.printf("Lead detection failure: LO+ %d and LO- %d\n", lo_plus_val, lo_minus_val  );
    delay(5);
  } 
  else {
    digitalVal = analogRead(ecg_output_pin);
    Serial.print(">digitalVal:");
    Serial.println(digitalVal);  
  }

  delay(20);
}
