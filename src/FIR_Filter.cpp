#include "FIR_Filter.h"

FIR_Filter::FIR_Filter() {
  // Clear state buffer to zero just to be safe
  memset(state, 0, sizeof(state));
}

bool FIR_Filter::init(float* coeffs) {
  if (!coeffs) {
    Serial.println("FIR init error: null coefficients");
    return false;
  }
  
  // Initialize the FIR filter with coefficients and state buffer
  int err = dsps_fir_init_f32(&filter, coeffs, state, FIR_NUM_OF_COE);
  if (err != ESP_OK) {
    Serial.printf("FIR init error: %d\n", err);
    return false;
  }
  
  return true;
}

void FIR_Filter::process(float* input, float* output, int len) {
  if (!input || !output || len <= 0) {
        Serial.printf("FIR process error: invalid input=%p, output=%p, len=%d\n", (void*)input, (void*)output, len);
    return;
  }
  
  dsps_fir_f32(&filter, input, output, len);
}