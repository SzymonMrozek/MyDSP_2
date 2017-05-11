//
// Created by Szymon Mrozek on 25/04/2017.
//

#include "parametric_mid_fir_filter.h"
#include "../bp_fir_filter/bp_fir_filter.h"
#include "../bs_fir_filter/bs_fir_filter.h"
#include "../fir_ring_buffer/fir_ring_buffer.h"


ParametricMidFirFilter::ParametricMidFirFilter(double center_frequency, double q_factor, double gain,
                                               double sampling_frequency, double pass_band_ripple,
                                               double stop_band_ripple) : FirFilter(sampling_frequency,pass_band_ripple,stop_band_ripple),
                                                q_factor_(q_factor), center_frequency_(center_frequency), gain_(gain) {
    
                                                    
                                                    
                                                                            
                                                    filter_ = new FirRingBuffer(new double [1],1);
                                                    initialize(false,q_factor_,center_frequency_,gain_);

}

double *ParametricMidFirFilter::GenerateImpulseResponse() {
    // NEVER USED
    return nullptr;
}


double ParametricMidFirFilter::GetQFactor() {
    return q_factor_;
}

void ParametricMidFirFilter::SetQFactor(double q_factor) {
    q_factor_ = q_factor;
    initialize(true,q_factor_,center_frequency_,gain_);

}


double ParametricMidFirFilter::GetCenterFrequency() {
    return center_frequency_;
}

void ParametricMidFirFilter::SetCenterFrequency(double center_frequency) {
    center_frequency_ = center_frequency;
    initialize(true, q_factor_,center_frequency_,gain_);

}

double ParametricMidFirFilter::GetGain() {
    return gain_;
}

void ParametricMidFirFilter::SetGain(double gain) {
    gain_ = gain;
    initialize(true, q_factor_,center_frequency_,gain_);

}

void ParametricMidFirFilter::initialize(bool enable, double q_factor, double center_frequency, double gain){

    enable_ = enable;
    auto cutoff_bandwidth = center_frequency / q_factor_;
    auto cutoff_bandwidth_divided = cutoff_bandwidth / 2.0;

    auto cutoff_frequency_left = center_frequency - cutoff_bandwidth_divided;
    auto cutoff_frequency_right = center_frequency + cutoff_bandwidth_divided;
    auto stop_frequency_left = center_frequency - cutoff_bandwidth;
    auto stop_frequency_right = center_frequency + cutoff_bandwidth;


    auto band_stop = new BSFirFilter(stop_frequency_left,cutoff_frequency_left,cutoff_frequency_right,stop_frequency_right,
                                     sampling_frequency_,pass_band_ripple_,stop_band_ripple_);
    auto band_pass = new BPFirFilter(stop_frequency_left,cutoff_frequency_left,cutoff_frequency_right,stop_frequency_right,
                                     sampling_frequency_,pass_band_ripple_,stop_band_ripple_);

    auto band_stop_response = band_stop -> GetImpulseResponse();
    auto band_pass_response = band_pass -> GetImpulseResponse();

    double* impulse_response = new double[band_pass -> GetLength()];
    for(auto i = 0; i < band_pass -> GetLength(); ++i){
        impulse_response[i] = band_stop_response[i] + band_pass_response[i] * gain;
    }
    filter_ -> SetImpulseResponse(impulse_response,band_stop->GetLength());
    //delete band_stop;
    //delete band_pass;

}
