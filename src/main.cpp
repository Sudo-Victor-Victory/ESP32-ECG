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
// sampling rate: 250 Hz, ccutoff 16 Hz, Transition 20.01
// window: hamming
const int num_of_coe = 39;
float fir_coeffs[num_of_coe] = { 
    0.001305845773729295,
    0.001241706262412450,
    0.001028659660159184,
    0.000403570393775636,
    -0.000931259100997812,
    -0.003148180251232014,
    -0.006126471774819952,
    -0.009334145942710936,
    -0.011810191151871057,
    -0.012275445798530222,
    -0.009366513041530130,
    -0.001951028056476336,
    0.010545777388041201,
    0.027885353910952579,
    0.048904322959811709,
    0.071591764853380632,
    0.093357435304643938,
    0.111447175302926610,
    0.123424140043399674,
    0.127614966529870782,
    0.123424140043399688,
    0.111447175302926610,
    0.093357435304643965,
    0.071591764853380646,
    0.048904322959811709,
    0.027885353910952579,
    0.010545777388041197,
    -0.001951028056476336,
    -0.009366513041530132,
    -0.012275445798530222,
    -0.011810191151871064,
    -0.009334145942710944,
    -0.006126471774819955,
    -0.003148180251232013,
    -0.000931259100997811,
    0.000403570393775637,
    0.001028659660159184,
    0.001241706262412449,
    0.001305845773729295,
};

fir_f32_t fir_filter;  
float ecg_sample;
float filtered_ecg = 0.0;
float fir_state[num_of_coe];  
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

    Serial.print(">filteredVal:");
    Serial.println(filtered_ecg);  
    }

  delay(4);
}
