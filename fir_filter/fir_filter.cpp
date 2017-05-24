//
//  fir_filter.cpp
//  TuneTheRoom
//
//  Created by Szymon Mrozek on 21.03.2017.
//  Copyright © 2017 Szymon Mrozek. All rights reserved.
//

#include <cstring>
#include "fir_filter.h"
#include "../fir_ring_buffer/fir_ring_buffer.h"

FirFilter :: FirFilter(double sampling_frequency, double pass_band_ripple, double stop_band_ripple) : sampling_frequency_(sampling_frequency), pass_band_ripple_(pass_band_ripple),stop_band_ripple_(stop_band_ripple){
}

double FirFilter :: FilterSample(double sample){


    filter_->AddToBuffer(sample);
    return filter_->OutputOfCurrent();
    
}

double * FirFilter :: FilterBuffer(double *samples_buffer, int count){
    
    double * temp = new double[count];
    std::memcpy(temp,samples_buffer,count * sizeof(double));
    
    for(int i = 0 ; i < count ; i ++ ){
        samples_buffer[i] = FilterSample(samples_buffer[i]);
    }
    delete [] temp;
    return samples_buffer;
}

void FirFilter::setEnable(bool enable) {

    enable_ = enable;
}

bool FirFilter::isEnable() {
    return enable_;
}



int FirFilter :: GetLength(){
    return length_;
}

double *FirFilter::GetImpulseResponse() {
    return filter_ -> GetImpulseResponse();
}


void FirFilter::SetImpulseResponse(double *impulse_response) {
    filter_ -> SetImpulseResponse(impulse_response,length_);
}


int FirFilter :: factorial(int n){
    return (n == 1 || n == 0) ? 1 : factorial(n - 1) * n;
}

double FirFilter :: bessel(double arg){
    
    double value = 1.0;
    for (int k = 1; k < 7; ++k){
        value += pow(pow(arg / 2, k) / factorial(k), 2);
    }
    return value;
}

void FirFilter :: Attenuation(){
    
    double minimalRipple = fmin(pass_band_ripple_,stop_band_ripple_);
    attenuation_ = -20 * log10(minimalRipple);
}

void  FirFilter :: Beta(){
    
    if ( attenuation_ < 21.0){
        beta_ = 0.0;
    }else if( attenuation_ >= 21.0 && attenuation_ <= 50.0){
        beta_ = pow(0.5842 * ( attenuation_ - 21.0),0.4) + 0.07886 * ( attenuation_ - 21.0);
    }else if(attenuation_ > 50.0){
        beta_ = 0.1102 * ( attenuation_ - 8.7 );
    }
    
}

void FirFilter :: Length(){
    
    double value;
    if( attenuation_ > 21.0 ) {
        value = ( attenuation_ - 7.95) / 14.36 ;
    }else{
        value = 0.922;
    }
    length_ = ceil(value * sampling_frequency_ / transition_bandwidth_ );
    if( length_ % 2 == 0){
        length_ ++;
    }
    width_ = (length_ - 1 ) / 2;
}

double * FirFilter :: GenerateWindow(){
    
    double * window = new double[length_];
    
    double a,b;
    double width_2 = double (width_ * width_);
    int index;
    for( int i = 0 ; i < length_ ; i ++ ){
        index = i - width_;
        a = bessel(beta_ * sqrt(1.0 - index * index / width_2));
        b = bessel(beta_);
        window[i] = a / b ;
    }
    
    return window;
    
}





