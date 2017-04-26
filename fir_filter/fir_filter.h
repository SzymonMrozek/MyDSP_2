//
//  fir_filter.h
//  TuneTheRoom
//
//  Created by Szymon Mrozek on 21.03.2017.
//  Copyright © 2017 Szymon Mrozek. All rights reserved.
//

#ifndef fir_filter_h
#define fir_filter_h

#include <math.h>
#include <stdlib.h>

#define PIx2 6.28318530718

class FirRingBuffer;
class FirFilter{
    
public:
    
    FirFilter(double sampling_frequency, double pass_band_ripple = 0.001 , double stop_band_ripple = 0.001);
    double FilterSample(double sample);
    double * FilterBuffer(double *stamples_buffer, int count);

    void setEnable(bool enable);
    bool isEnable();

    int GetLength();
    double* GetImpulseResponse();
    void SetImpulseResponse(double* impulse_response);

    
protected:
    
    int factorial(int n);
    double bessel(double arg);
    
    void Attenuation();
    void Beta();
    void Length();
    
    double* GenerateWindow();
    
    virtual double* GenerateImpulseResponse() = 0;


    bool enable_;

    FirRingBuffer* filter_;
    
    int length_;
    int width_; // width_ = (length -1) / 2
    double pass_band_ripple_;
    double stop_band_ripple_;
    double sampling_frequency_;
    
    double beta_;
    double attenuation_;
    double transition_bandwidth_;
    
    double* window_;
    
    
};

#endif /* fir_filter_h */














