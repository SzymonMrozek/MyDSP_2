//
// Created by Szymon Mrozek on 25/04/2017.
//

#ifndef MYDSPV2_BP_FIR_FILTER_H
#define MYDSPV2_BP_FIR_FILTER_H

#include "../fir_filter/fir_filter.h"

class BPFirFilter : public FirFilter {

public:

    BPFirFilter(double stop_band_frequency_left, double cutoff_frequency_left, double cutoff_frequency_right,
                double stop_band_frequency_right, double sampling_frequency, double pass_band_ripple, double stop_band_ripple);

    double * GenerateImpulseResponse() override;


private:
    double cutoff_frequency_left_;
    double stop_band_frequency_left_;
    double cutoff_frequency_right_;
    double stop_band_frequency_right_;

    double transition_band_center_left_;
    double transition_band_center_right_;

    double normalized_omega_left_;
    double normalized_omega_right_;
};


#endif //MYDSPV2_BP_FIR_FILTER_H
