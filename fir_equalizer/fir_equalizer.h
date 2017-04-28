//
// Created by Szymon Mrozek on 26/04/2017.
//

#ifndef MYDSPV2_FIR_EQUALIZER_H
#define MYDSPV2_FIR_EQUALIZER_H

class LPFirFilter;
class HPFirFilter;
class ParametricMidFirFilter;

class FirEqualizer {

public:

    FirEqualizer(bool enable, double lowpass_cutoff, double mid_low_q_factor, double mid_low_center_frequency, double       mid_low_gain, double mid_high_q_factor, double mid_high_center_frequency, double mid_high_gain,
        double highpass_cutoff,double sampling_frequency, double pass_band_ripple, double stop_band_ripple);


    void SetLowpass(bool enable, double lowpass_cutoff);
    void SetMidLow(bool enable, double q_factor, double center_frequency, double gain);
    void SetMidHigh(bool enable, double q_factor, double center_frequency, double gain);
    void SetHighpass(bool enable, double highpass_cutoff);

    double FilterSample(double sample);
    double * FilterBuffer(double *stamples_buffer, int count);




private:

    LPFirFilter* lowpass;
    ParametricMidFirFilter* mid_low;
    ParametricMidFirFilter* mid_high;
    HPFirFilter* highpass;

};


#endif //MYDSPV2_FIR_EQUALIZER_H
