#ifndef ECG_SHARED_H
#define ECG_SHARED_H

#include "FIR_Filter.h"
#include "Kalman_Filter.h"

struct EcgSharedValues {
  FIR_Filter* fir;
  KalmanFilter* kalman;
  const int ecg_output_pin;
  const int lo_plus;
  const int lo_minus;
};

#endif