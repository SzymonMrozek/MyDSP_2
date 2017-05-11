//
// Created by Szymon Mrozek on 25/04/2017.
//

#include "hp_fir_filter.h"
#include "../fir_ring_buffer/fir_ring_buffer.h"

HPFirFilter::HPFirFilter(double cutoff_frequency, double stop_band_frequency, double sampling_frequency,
                         double pass_band_ripple, double stop_band_ripple) :
        cutoff_frequency_(cutoff_frequency),stop_band_frequency_(stop_band_frequency),
        FirFilter(sampling_frequency, pass_band_ripple, stop_band_ripple){

    enable_ = false;
    Attenuation();
    Beta();
    transition_bandwidth_ = cutoff_frequency - stop_band_frequency;
    Length();
    window_ = GenerateWindow();

    transition_band_center_ = (stop_band_frequency + cutoff_frequency)/(2.0 * sampling_frequency);
    normalized_omega_ =  PIx2 * transition_band_center_ ;

    filter_ = new FirRingBuffer(GenerateImpulseResponse(),length_);

}

double* HPFirFilter :: GenerateImpulseResponse(){

    double* impulse_response = new double[length_];

    for ( int i = 0 ; i < width_; i++){
        impulse_response[i] = (window_[i] * -2.0 * transition_band_center_ * sin(normalized_omega_ * double(i - width_)))
                              /(normalized_omega_ * double(i - width_));
        impulse_response[i + width_ + 1] = (window_ [i+width_+1] * -2.0 * transition_band_center_ * sin(normalized_omega_ * double(i + 1)))
                                           /(normalized_omega_ * double(i + 1));
    }
    impulse_response [width_] = window_[width_] * (1.0 - 2.0 * transition_band_center_);
    return impulse_response;

}

void HPFirFilter::initialize(bool enable, double cutoff_frequency) {

    enable_ = enable;
    cutoff_frequency_ = cutoff_frequency;
    stop_band_frequency_ = cutoff_frequency - cutoff_frequency/4.0;
    transition_bandwidth_ = cutoff_frequency_/4.0;
    transition_band_center_ = (stop_band_frequency_ + cutoff_frequency_)/(2.0 * sampling_frequency_);
    normalized_omega_ =  PIx2 * transition_band_center_ ;
    Length();

    SetImpulseResponse(GenerateImpulseResponse());

}
