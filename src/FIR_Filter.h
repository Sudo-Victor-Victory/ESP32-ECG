#ifndef FIR_FILTER_H
#define FIR_FILTER_H

#include <Arduino.h>

extern "C" {
  #include "dsps_fir.h"
}

#define FIR_NUM_OF_COE 53

class FIR_Filter {
public:
    FIR_Filter();
    bool init(float* coeffs);
    void process(float* input, float* output, int len);

private:
    fir_f32_t filter;
    float state[FIR_NUM_OF_COE];  // fixed-size buffer
};

#endif