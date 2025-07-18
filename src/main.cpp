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

// CURRENT FIR settings:
// sampling rate: 250 Hz, cutoff 20.00 Hz, Transition 15.00
// window: hamming
const int num_of_coe = 53;
float fir_coeffs[num_of_coe] = { 
    -0.000574906859963121,
    0.000000000000000001,
    0.000726878656656865,
    0.001444470981603404,
    0.001823411763234968,
    0.001436279187495883,
    -0.000000000000000002,
    -0.002310616106483979,
    -0.004680774400172320,
    -0.005801495059961710,
    -0.004401148062126011,
    0.000000000000000003,
    0.006466871282307920,
    0.012557954337108400,
    0.015001347272149222,
    0.011042272216301127,
    -0.000000000000000005,
    -0.015648716510277744,
    -0.030281609195214557,
    -0.036496051465025964,
    -0.027538264420590371,
    0.000000000000000007,
    0.044249959513353213,
    0.097761326729313300,
    0.149140033916397047,
    0.186218067602480941,
    0.199729417242827056,
    0.186218067602480941,
    0.149140033916397047,
    0.097761326729313300,
    0.044249959513353219,
    0.000000000000000007,
    -0.027538264420590371,
    -0.036496051465025971,
    -0.030281609195214564,
    -0.015648716510277747,
    -0.000000000000000005,
    0.011042272216301129,
    0.015001347272149230,
    0.012557954337108400,
    0.006466871282307920,
    0.000000000000000003,
    -0.004401148062126011,
    -0.005801495059961714,
    -0.004680774400172329,
    -0.002310616106483979,
    -0.000000000000000002,
    0.001436279187495883,
    0.001823411763234967,
    0.001444470981603405,
    0.000726878656656865,
    0.000000000000000001,
    -0.000574906859963121,
};

fir_f32_t fir_filter;  
float ecg_sample;
float filtered_ecg = 0.0;
float fir_state[num_of_coe];  

struct KalmanFilter {
  float estimate;
  float error_cov;
  float q; // Signal variance weight
  float r; // Noisy measurement weight
  float k; // Kalman gain variable


  KalmanFilter(float q, float r) {
    this->estimate = 0.0f;
    this->error_cov = 1.0f;
    this->q = q;
    this->r = r;
    this->k = 0.0f; 
  }

  float update_k(float ecg_measurement) {
    error_cov += q;
    float k = error_cov / (error_cov + r);
    estimate += k * (ecg_measurement - estimate);
    error_cov *= (1 - k);
    return estimate;
  }
};

// Chose 2.0 & 20.0 to reduce noise at the cost of slightly flattening the QRS.
float kalman_q = 2.0f;
float kalman_r = 20.0f;

KalmanFilter ecg_kalman(kalman_q, kalman_r);  
static unsigned long last_print = 0;
int print_delay = 20;

void setup() {
  Serial.begin(115200);
  pinMode(lo_minus, INPUT);
  pinMode(lo_plus, INPUT);

  analogReadResolution(12);        // Uses the ESP's 12 bit ADC
  analogSetAttenuation(ADC_11db);  // Uses ESP's default ref. voltage

  dsps_fir_init_f32(&fir_filter, fir_coeffs, fir_state, num_of_coe );
  delay(1000);
}

void loop() {
  int lo_plus_val = digitalRead(lo_plus);
  int lo_minus_val = digitalRead(lo_minus);

  if (lo_plus_val == HIGH || lo_minus_val == HIGH) {
    Serial.printf("Lead detection failure: LO+ %d and LO- %d\n", lo_plus_val, lo_minus_val  );
    delay(5);
  } 

  else {
    ecg_sample = (float)analogRead(ecg_output_pin);
    dsps_fir_f32(&fir_filter, &ecg_sample, &filtered_ecg, 1);

    float kalman_ecg = ecg_kalman.update_k(filtered_ecg);
    // Limit Serial output rate to prevent overwhelming the buffer at 115200 baud
    if (millis() - last_print >= print_delay) {
      Serial.printf(">filteredVal:%.2f\n", filtered_ecg);
      Serial.printf(">kalmanVal:%.2f\n", kalman_ecg);
      last_print = millis();
    }
  }

  delay(4);
}
