//
// Created by Szymon Mrozek on 26/04/2017.
//

#include "fir_equalizer.h"
#include "../lp_fir_filter/lp_fir_filter.h"
#include "../hp_fir_filter/hp_fir_filter.h"
#include "../parametric_mid_fir_filter/parametric_mid_fir_filter.h"


FirEqualizer::FirEqualizer(bool enable, double lowpass_cutoff, double mid_low_q_factor, double mid_low_center_frequency,
                           double mid_low_gain, double mid_high_q_factor, double mid_high_center_frequency,
                           double mid_high_gain, double highpass_cutoff, double sampling_frequency,
                           double pass_band_ripple, double stop_band_ripple) {



    lowpass = new LPFirFilter(lowpass_cutoff,lowpass_cutoff+lowpass_cutoff/4.0,sampling_frequency,pass_band_ripple,stop_band_ripple);
    lowpass -> initialize(enable,lowpass_cutoff);

    mid_low = new ParametricMidFirFilter(mid_low_center_frequency,mid_low_q_factor,mid_low_gain,sampling_frequency,pass_band_ripple,stop_band_ripple);
    mid_low -> initialize(enable,mid_low_q_factor,mid_low_center_frequency,mid_low_gain);

    mid_high = new ParametricMidFirFilter(mid_high_center_frequency,mid_high_q_factor,mid_high_gain,sampling_frequency,pass_band_ripple,stop_band_ripple);
    mid_high -> initialize(enable,mid_high_q_factor,mid_high_center_frequency,mid_high_gain);

    highpass = new HPFirFilter(highpass_cutoff+highpass_cutoff/4.0,highpass_cutoff,sampling_frequency,pass_band_ripple,stop_band_ripple);
    highpass -> initialize(enable,highpass_cutoff);
}

void FirEqualizer::SetLowpass(bool enable, double lowpass_cutoff) {

    lowpass -> initialize(enable,lowpass_cutoff);
}

void FirEqualizer::SetMidLow(bool enable, double q_factor, double center_frequency, double gain) {

    mid_low -> initialize(enable,q_factor,center_frequency,gain);
}

void FirEqualizer::SetMidHigh(bool enable, double q_factor, double center_frequency, double gain) {

    mid_high -> initialize(enable,q_factor,center_frequency,gain);
}

void FirEqualizer::SetHighpass(bool enable, double highpass_cutoff) {

    highpass -> initialize(enable,highpass_cutoff);
}

double FirEqualizer::FilterSample(double sample) {

    if(lowpass -> isEnable()){
        sample = lowpass -> FilterSample(sample);
    }
    if(mid_low -> isEnable()){
        sample = mid_low -> FilterSample(sample);
    }
    if(mid_high -> isEnable()){
        sample = mid_high -> FilterSample(sample);
    }
    if(highpass -> isEnable()){
        sample = highpass -> FilterSample(sample);
    }
    return sample;
}

double* FirEqualizer::FilterBuffer(double *samples_buffer, int count) {

    if(lowpass -> isEnable()){
        samples_buffer = lowpass -> FilterBuffer(samples_buffer,count);
    }
    if(mid_low -> isEnable()){
        samples_buffer = mid_low -> FilterBuffer(samples_buffer,count);
    }
    if(mid_high -> isEnable()){
        samples_buffer = mid_high -> FilterBuffer(samples_buffer,count);
    }
    if(highpass -> isEnable()){
        samples_buffer = highpass -> FilterBuffer(samples_buffer,count);
    }
    return samples_buffer;
}

