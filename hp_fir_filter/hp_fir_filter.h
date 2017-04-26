//
// Created by Szymon Mrozek on 25/04/2017.
//

#ifndef MYDSPV2_HP_FIR_FILTER_H
#define MYDSPV2_HP_FIR_FILTER_H

#include "../fir_filter/fir_filter.h"

class HPFirFilter : public FirFilter {

public:

    HPFirFilter(double cutoff_frequency, double stop_band_frequency, double sampling_frequency,
                double pass_band_ripple, double stop_band_ripple);

    double* GenerateImpulseResponse() override;

    void initialize(bool enable, double cutoff_frequency);


private:
    double cutoff_frequency_;
    double stop_band_frequency_;
    double transition_band_center_;
    double normalized_omega_;

};


#endif //MYDSPV2_HP_FIR_FILTER_H
