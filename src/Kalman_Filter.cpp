#include "Kalman_Filter.h"

KalmanFilter::KalmanFilter(float q, float r) {
    this->estimate = 0.0f;
    this->error_cov = 1.0f;
    this->q = q;
    this->r = r;
    this->k = 0.0f;
}

float KalmanFilter::update_k(float ecg_measurement) {
    error_cov += q;
    k = error_cov / (error_cov + r);
    estimate += k * (ecg_measurement - estimate);
    error_cov *= (1 - k);
    return estimate;
}