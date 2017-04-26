//
// Created by Szymon Mrozek on 25/04/2017.
//
#include <algorithm>
#include "bs_fir_filter.h"
#include "../fir_ring_buffer/fir_ring_buffer.h"

BSFirFilter::BSFirFilter(double cutoff_frequency_left, double stop_band_frequency_left,
                         double stop_band_frequency_right, double cutoff_frequency_right, double sampling_frequency,
                         double pass_band_ripple, double stop_band_ripple) :
                            FirFilter(sampling_frequency, pass_band_ripple, stop_band_ripple), cutoff_frequency_left_(cutoff_frequency_left),
                            stop_band_frequency_left_(stop_band_frequency_left), stop_band_frequency_right_(stop_band_frequency_right),
                            cutoff_frequency_right_(cutoff_frequency_right){

    auto transition_bandwidth_left = stop_band_frequency_left_ - cutoff_frequency_left_;
    auto transition_bandwidth_right = cutoff_frequency_right_ - stop_band_frequency_right_;
    transition_bandwidth_ = std::min(transition_bandwidth_left,transition_bandwidth_right);

    transition_band_center_left_ = (cutoff_frequency_left + (transition_bandwidth_ / 2.0))/sampling_frequency_;
    transition_band_center_right_ = (cutoff_frequency_right - (transition_bandwidth_ / 2.0))/sampling_frequency;

    normalized_omega_left_ = PIx2 * transition_band_center_left_;
    normalized_omega_right_ = PIx2 * transition_band_center_right_;


    Attenuation();
    Beta();
    Length();
    window_ = GenerateWindow();


    filter_ = new FirRingBuffer(GenerateImpulseResponse(),length_);

}

double *BSFirFilter::GenerateImpulseResponse() {


    double* impulse_response = new double[length_];

    for ( int i = 0 ; i < width_; i++){
        impulse_response[i] = window_[i] * ((2.0 * transition_band_center_left_ * sin(normalized_omega_left_ * double(i - width_)))
                              /(normalized_omega_left_ * double(i - width_)) -
                              (2.0 * transition_band_center_right_ * sin(normalized_omega_right_ * double(i - width_)))
                              /(normalized_omega_right_ * double(i - width_)));

        impulse_response[i + width_ + 1] = window_ [i+width_+1] * ((2.0 * transition_band_center_left_ * sin(normalized_omega_left_ * double(i + 1)))
                                           /(normalized_omega_left_ * double(i + 1)) -
                                           (2.0 * transition_band_center_right_ * sin(normalized_omega_right_ * double(i + 1)))
                                           /(normalized_omega_right_ * double(i + 1)));
    }
    impulse_response [width_] = window_[width_] * (1 + 2.0 * (transition_band_center_left_ - transition_band_center_right_));
    return impulse_response;

}
