#ifndef KALMANFILTER_H
#define KALMANFILTER_H

class KalmanFilter {
public:
    KalmanFilter(float q, float r);
    float update_k(float ecg_measurement);
private:
    float estimate;
    float error_cov;
    float q; // Signal variance weight
    float r; // Noisy measurement weight
    float k; // Kalman gain
};

#endif