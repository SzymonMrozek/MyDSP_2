//
// Created by Szymon Mrozek on 25/04/2017.
//

#ifndef MYDSPV2_PARAMETRIC_MID_FIR_FILTER_H
#define MYDSPV2_PARAMETRIC_MID_FIR_FILTER_H

#include "../fir_filter/fir_filter.h"

class ParametricMidFirFilter : public FirFilter {

public:
    ParametricMidFirFilter(double center_frequency, double q_factor, double gain, double sampling_frequency,
                            double pass_band_ripple, double stop_band_ripple);

    double * GenerateImpulseResponse() override;

    double GetQFactor();
    void SetQFactor(double q_factor);

    double GetCenterFrequency();
    void SetCenterFrequency(double center_frequency);

    double GetGain();
    void SetGain(double gain);

    void initialize(bool enable, double q_factor, double center_frequency, double gain);

private:

    double q_factor_;
    double center_frequency_;
    double gain_;

};


#endif //MYDSPV2_PARAMETRIC_MID_FIR_FILTER_H
