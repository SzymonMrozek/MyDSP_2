//
//  main.cpp
//  Fft
//
//  Created by Szymon Mrozek on 16.03.2017.
//  Copyright © 2017 Szymon Mrozek. All rights reserved.
//

#include <iostream>
#include <iomanip>
#include <math.h>

#include "fft/fft.h"
#include "lp_fir_filter/lp_fir_filter.h"
#include "hp_fir_filter/hp_fir_filter.h"
#include "bs_fir_filter/bs_fir_filter.h"
#include "bp_fir_filter/bp_fir_filter.h"
#include "parametric_mid_fir_filter/parametric_mid_fir_filter.h"
#include "fir_equalizer/fir_equalizer.h"

void sumSignals(double signal[],double signal2[],double signal3[]);

using namespace std;

double * generateSine(const double signalFrequency,const double samplingFrequency,const int bufferSize){
    
    double * signal = new double [ bufferSize];
    double samplingPeriod = 1.0 / samplingFrequency;
    for ( int i = 0 ; i < bufferSize ; i ++ ){
        
        signal [i] = sin(PIx2 * signalFrequency * i * samplingPeriod);
        
    }
    
    return signal;
}

double * generateRect(const double signalFrequency,const double samplingFrequency, const int bufferSize){
    
    double * signal = new double [bufferSize];
    int x;
    double xdouble,time;
    double samplingPeriod = 1.0 / samplingFrequency;
    
    for ( int i = 0; i < bufferSize ; i++){
        
        time = static_cast<double>(i) * samplingPeriod;
        x = static_cast<int>(( signalFrequency * PIx2 * time )/ PIx2);
        xdouble = time * PIx2 * signalFrequency;
        if( (xdouble -  x * PIx2) < PIx2){
            signal[i] = 1;
        }else{
            signal[i] = -1;
        }
    }
    
    return signal;
}

double* generateImpulse(const int buffer_size){
    double* impulse = new double[buffer_size];
    impulse[0]  = 1;
    for(auto i = 1; i < buffer_size ; ++i){
        impulse[i] = 0;
    }
    return impulse;
}
void printImpulseResponse(double impulseResponse [], int length){

    for ( int i = 0 ; i < length; i ++ ){
        cout << impulseResponse[i] << "  " ;
    }
    cout << endl;
    
}

int main(){

    FFT * fft = new FFT(8192,44100.0,true);
    double * signal = generateSine(1000.0, 44100.0,8192);
    double * signal2 = generateSine(3000.0,44100.0 , 8192);
    double * signal3 = generateSine(5000.0, 44100.0, 8192);
    double * signal4 = generateSine(10000.0, 44100.0, 8192);
    double * signal5 = generateSine(350.0, 44100.0, 8192);


    sumSignals(signal,signal2,signal3);
    sumSignals(signal,signal4,signal5);

    fft->Fft(signal);
    fft->Magnitude();
    fft->Phase();

    fft->WriteToFiles();
    double * sztos = fft->IFft(fft->GetFftReal(), fft->GetFftImag());

/*    for ( int i = 0 ; i < 2000 ; ++i){
        cout << fixed;
        cout << setprecision(5);
        cout << signal[i] - sztos[i] << "    ";
    }*/


    LPFirFilter* lpFirFilter = new LPFirFilter(3200.0,3400.0,44100.0,0.001,0.00001);
    HPFirFilter* hpFirFilter = new HPFirFilter(3400.0,3200.0,44100.0,0.001,0.00001);
    signal = generateImpulse(8192);
    BSFirFilter* bsFirFilter = new BSFirFilter(300.0, 900.0, 3200.0, 3500.0, 44100.0, 0.001, 0.001);
    BPFirFilter* bpFirFilter = new BPFirFilter(300.0, 330.0, 1200.0, 4000.0, 44100.0, 0.001, 0.001);
    ParametricMidFirFilter* pmFirFilter = new ParametricMidFirFilter(500.0,2.0,0.707,44100.0,0.0001,0.0001);
    
    FirEqualizer* eq = new FirEqualizer(true,15000.0,1.0,1000.0,0.0001, 0.8,1200.0,1.0,200.0,44100.0,0.0001,0.0001);
    //signal = eq -> FilterBuffer(signal,8192);
    signal = pmFirFilter -> FilterBuffer(signal,8192);

    printImpulseResponse(pmFirFilter->GetImpulseResponse(),100);

    //signal = pmFirFilter->FilterBuffer(signal, 8192);
    fft->Fft(signal);
    fft->Magnitude();
    fft->Phase();
    fft->WriteToFiles();

}

void sumSignals(double signal[], double signal2[], double signal3[]) {
    for(int i = 0  ; i < 8192 ; i ++ ) {
        signal[i] += signal2[i] + signal3[i];
    }
}
