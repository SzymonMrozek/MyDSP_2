 //
//  lp_fir_filter.h
//  TuneTheRoom
//
//  Created by Szymon Mrozek on 22.03.2017.
//  Copyright © 2017 Szymon Mrozek. All rights reserved.
//

#ifndef lp_fir_filter_h
#define lp_fir_filter_h

#include "../fir_filter/fir_filter.h"

class LPFirFilter : public FirFilter{
    

    
public:
    
    LPFirFilter(double cutoff_frequency, double stop_band_frequency, double sampling_frequency,
                    double pass_band_ripple, double stop_band_ripple);
    
    double * GenerateImpulseResponse() override;

    void initialize(bool enable, double cutoff_frequency);
    
private:
    double cutoff_frequency_;
    double stop_band_frequency_;
    double transition_band_center_;
    double normalized_omega_;
    
};

#endif /* lp_fir_filter_h */

