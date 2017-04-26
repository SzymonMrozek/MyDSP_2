//
//  FFT.cpp
//  Fft
//
//  Created by Szymon Mrozek on 14.03.2017.
//  Copyright © 2017 Szymon Mrozek. All rights reserved.
//

#include <iostream>
#include <math.h>
#include <ctime>
#include <fstream>
#include "fft.h"

#define PI2 6.28318530718

using namespace std;

// *** PUBLIC *** //

FFT :: FFT(int buffer_size, double sampling_frequency,bool enable_window) : buffer_size_(buffer_size),real_buffer_size_(buffer_size /2), butterflies_(log2(buffer_size)),sampling_frequency_(sampling_frequency),is_window_enabled_(enable_window){
    
    indexes_ = new int[buffer_size];
    FillRevertedIndexes();
    
    sines_   = new double[butterflies_ + 1];
    cosines_ = new double[butterflies_ + 1];
    FillSinesAndCosines();
    
    fft_freqs_  = new double[real_buffer_size_];
    Freqs();
    
    window_buffer_ = new double[buffer_size];
    FillWindowBuffer();
    
    fft_real_   = new double[buffer_size];
    fft_imag_   = new double[buffer_size];
    
    fft_magnitude_ = new double[real_buffer_size_];
    fft_phase_     = new double[real_buffer_size_];
    average_from_magnitude_ = 0.0;
}

FFT :: ~FFT(){
    
    
    delete [] indexes_;
    delete [] sines_;
    delete [] cosines_;
    delete [] window_buffer_;
    delete [] fft_freqs_;
    delete [] fft_real_;
    delete [] fft_imag_;
    delete [] fft_magnitude_;
    delete [] fft_phase_;
    
}


void FFT :: Fft(double *signal){
    
    
    double * signal_copy = new double[buffer_size_];
    if(is_window_enabled_){
        signal_copy = WindowHamming(signal);
    }
    else{
        
        copy(signal, signal+buffer_size_, signal_copy);
    }
    
    // Global iterator - used to iterate over whole Fft array
    
    int global_iterator,element;
    
    // Index even is index of first element, odd is index to corresponding element
    int index_even,index_odd;
    
    // odd and even values are used to minimize multiplications
    double odd_real,odd_imag,even_real,even_imag;
    
    double omega_real, omega_imag,actual_omega_real,actual_omega_imag,old_omega_real,old_omega_imag;
    
    
    // Fitst butterfly - segment containg pair of values
    int dft_size = 2;
    
    ///////////////////////////////////////////////////
    
    for(int currentSegment = 0 ; currentSegment < buffer_size_ / 2 ;currentSegment ++){
        
        global_iterator = currentSegment << 1;
        
        // actual segment even and odd index in whole Fft
        index_even   =  global_iterator;
        index_odd    =  global_iterator + 1;
        // assign to temp values
        even_real = signal_copy[indexes_[index_even]];
        odd_real = signal_copy[indexes_[index_odd]];
        
        // butterfly
        fft_real_[index_even] = even_real + odd_real;
        fft_imag_[index_even] = 0;
        fft_real_[index_odd] = even_real - odd_real;
        fft_imag_[index_odd] = 0;
        
    }
    
    int current_butterfly = 2;
    dft_size = 4;
    int jump = 2;
    
    
    for (; current_butterfly <= butterflies_ ; current_butterfly ++){
        
        // assign the Wn^1
        omega_real = cosines_[current_butterfly];
        omega_imag = sines_  [current_butterfly];
        
        for(int currentSegment = 0 ; currentSegment < buffer_size_ / dft_size ;currentSegment ++){
            
            element = 0;
            global_iterator = currentSegment * dft_size;
            
            // actual segment even and odd index in whole Fft
            index_even   = global_iterator;
            index_odd    = global_iterator + jump;
            // assign to temp values
            odd_real = fft_real_[index_odd];
            odd_imag = fft_imag_[index_odd];
            even_real = fft_real_[index_even];
            even_imag = fft_imag_[index_even];
            
            // butterfly
            fft_real_[index_even] = even_real + odd_real;
            fft_imag_[index_even] = even_imag + odd_imag;
            fft_real_[index_odd] = even_real - odd_real;
            fft_imag_[index_odd] = even_imag - odd_imag;
            
            // Actual omega = Wn ^ 1
            actual_omega_imag = omega_imag;
            actual_omega_real = omega_real;
            
            ++ element;
            while(element < dft_size / 2 ){
                // actual segment even and odd index in whole Fft
                index_even   = global_iterator + element;
                index_odd    = global_iterator + element + jump;
                
                // calculate omega * Fft[index_odd] to temp
                odd_real = actual_omega_real * fft_real_[index_odd] - actual_omega_imag * fft_imag_[index_odd];
                odd_imag = actual_omega_imag * fft_real_[index_odd] + actual_omega_real * fft_imag_[index_odd];
                
                // store value of Fft[index_even]
                even_real = fft_real_[index_even];
                even_imag = fft_imag_[index_even];
                
                // butterfly
                fft_real_[index_even] = even_real + odd_real;
                fft_imag_[index_even] = even_imag + odd_imag;
                fft_real_[index_odd] = even_real - odd_real;
                fft_imag_[index_odd] = even_imag - odd_imag;
                
                // Actual omega = Wn ^ k;
                old_omega_real = actual_omega_real;
                old_omega_imag = actual_omega_imag;
                actual_omega_real = old_omega_real * omega_real - old_omega_imag * omega_imag;
                actual_omega_imag = old_omega_imag * omega_real + old_omega_real * omega_imag;
                ++element;
            }
        }
        
        dft_size <<= 1;
        jump <<= 1;
    }
    
}

double * FFT :: GetFftReal(){
    double * fft_real_copy = new double[buffer_size_];
    copy(fft_real_, fft_real_+buffer_size_, fft_real_copy);
    return fft_real_copy;
}
double * FFT :: GetFftImag(){
    
    double * fft_imag_copy = new double[buffer_size_];
    copy(fft_imag_, fft_imag_ + buffer_size_,fft_imag_copy);
    return fft_imag_copy;
}


double * FFT :: IFft(double *real, double *imag){
    
    // Function takes REAL and IMAG part of Fourier Transformation
    // These parts are swapped in function below
    
    double * signal_real = new double[buffer_size_];
    double * signal_imag = new double[buffer_size_];
    
    // Global iterator - used to iterate over whole Fft array
    
    int global_iterator,element;
    
    // Index even is index of first element, odd is index to corresponding element
    int index_even,index_odd;
    
    // odd and even values are used to minimize multiplications
    double odd_real,odd_imag,even_real,even_imag;
    
    double omega_real, omega_imag, actual_omega_real, actual_omega_imag, old_omega_real, old_omega_imag;
    
    
    // Fitst butterfly - segment containg pair of values
    int dft_size = 2;
    
    for(int currentSegment = 0 ; currentSegment < buffer_size_ / 2 ;currentSegment ++){
        
        global_iterator = currentSegment << 1;
        
        // actual segment even and odd index in whole Fft
        index_even   =  global_iterator;
        index_odd    =  global_iterator + 1;
        
        // store value of index even and odd
        
        even_real = imag[indexes_[index_even]];
        even_imag = real[indexes_[index_even]];
        odd_real = imag[indexes_[index_odd]];
        odd_imag = real[indexes_[index_odd]];
        // butterfly
        signal_real[index_even] = even_real + odd_real;
        signal_imag[index_even] = even_imag + odd_imag;
        signal_real[index_odd]  = even_real - odd_real;
        signal_imag[index_odd]  = even_imag - odd_imag;
        
        
    }
    
    int current_butterfly = 2;
    dft_size = 4;
    int jump = 2;
    
    
    for (; current_butterfly <= butterflies_ ; current_butterfly ++){
        
        // assign the Wn^1
        omega_real = cosines_[current_butterfly];
        omega_imag = sines_  [current_butterfly];
        
        for(int currentSegment = 0 ; currentSegment < buffer_size_ / dft_size ;currentSegment ++){
            
            element = 0;
            global_iterator = currentSegment * dft_size;
            
            // actual segment even and odd index in whole Fft
            index_even   = global_iterator;
            index_odd    = global_iterator + jump;
            // assign to temp values
            odd_real = signal_real[index_odd];
            odd_imag = signal_imag[index_odd];
            even_real = signal_real[index_even];
            even_imag = signal_imag[index_even];
            
            // butterfly
            signal_real[index_even] = even_real + odd_real;
            signal_imag[index_even] = even_imag + odd_imag;
            signal_real[index_odd] = even_real - odd_real;
            signal_imag[index_odd] = even_imag - odd_imag;
            
            // Actual omega = Wn ^ 1
            actual_omega_imag = omega_imag;
            actual_omega_real = omega_real;
            
            ++ element;
            while(element < dft_size / 2 ){
                // actual segment even and odd index in whole Fft
                index_even   = global_iterator + element;
                index_odd    = global_iterator + element + jump;
                
                // calculate omega * Fft[index_odd] to temp
                odd_real = actual_omega_real * signal_real[index_odd] - actual_omega_imag * signal_imag[index_odd];
                odd_imag = actual_omega_imag * signal_real[index_odd] + actual_omega_real * signal_imag[index_odd];
                
                // store value of Fft[index_even]
                even_real = signal_real[index_even];
                even_imag = signal_imag[index_even];
                
                // butterfly
                signal_real[index_even] = even_real + odd_real;
                signal_imag[index_even] = even_imag + odd_imag;
                signal_real[index_odd] = even_real - odd_real;
                signal_imag[index_odd] = even_imag - odd_imag;
                
                // Actual omega = Wn ^ k;
                old_omega_real = actual_omega_real;
                old_omega_imag = actual_omega_imag;
                actual_omega_real = old_omega_real * omega_real - old_omega_imag * omega_imag;
                actual_omega_imag = old_omega_imag * omega_real + old_omega_real * omega_imag;
                ++element;
            }
        }
        
        dft_size <<= 1;
        jump <<= 1;
    }
    
    if (is_window_enabled_){
        signal_real = UnWindowHamming(signal_real);
        signal_imag = UnWindowHamming(signal_imag);
    }
    for(int i = 0; i < buffer_size_; ++i){
        signal_real[i] /= buffer_size_;
        signal_imag[i] /= buffer_size_;
    }
    
    return signal_imag;
}


double * FFT :: Magnitude(){
    
    // abs ( complex ) = sqrt( complex real ^ 2 + complex imag ^ 2 )
    double sum = 0.0,value;
    for ( int i = 0 ; i < real_buffer_size_ ; i++ ){
        value = sqrt(fft_real_[i] * fft_real_[i]+ fft_imag_[i] * fft_imag_[i]);
        fft_magnitude_[i] = value;
        sum += value;
    }
    average_from_magnitude_ = sum/double(real_buffer_size_);
    return fft_magnitude_;
}
double * FFT :: Phase(){
    
    for( int i = 0 ; i < real_buffer_size_ ; i ++){
        fft_phase_[i] = atan2(fft_imag_[i], fft_real_[i]);
    }
    return fft_phase_;
}

double FFT :: GetAverrageFromMagnitude(){
    
    return average_from_magnitude_;
}

void FFT :: WriteToFiles(){
    
    ofstream freqsTxt ("freqs.txt",ofstream::trunc);
    ofstream fftTxt   ("fft.txt",ofstream::trunc);
    
    if(freqsTxt.is_open() && fftTxt.is_open()){
        cout << "Succesfuly opened files" << endl;
        for(int i = 0 ; i < real_buffer_size_; i++){
            freqsTxt << fft_freqs_[i] << endl;
            fftTxt   << fft_magnitude_[i] << endl;
            
        }
        freqsTxt.close();
        fftTxt.close();
    }else{
        cout << "Unable to open files" << endl;
    }
}

// *** PRIVATE SECTION *** //


void FFT :: FillRevertedIndexes(){
    
    int temp,bitLength = butterflies_;
    for(int i = 0 ; i < buffer_size_; i++){
        
        temp = i;
        indexes_[i] = i;
        
        for( int j = 0 ; j < bitLength; j++){
            
            if( (1 << j) & temp){
                indexes_[i] |=   1 << (bitLength - 1 - j);
            }else{
                indexes_[i] &= ~(1 << (bitLength - 1 - j));
            }
        }
    }
}

void FFT :: FillSinesAndCosines(){
    
    int dft_size = 4;
    // size is log2(N) + 1 becouse of reading process
    for ( int i = 2 ; i < butterflies_ + 1 ; i++ ){
        
        double arg = PI2 / double(dft_size);
        sines_  [i] = sin(arg);
        cosines_[i] = cos(arg);
        dft_size <<= 1;
    }
}

void FFT :: FillWindowBuffer(){
    
    for( int i = 0 ; i < buffer_size_ ; i++ ) {
        
        window_buffer_ [i] = 0.54 - 0.46 * cos(PI2 * double(i) / double(buffer_size_ - 1));
    }
    
    
}

double * FFT :: Freqs(){
    
    // freuency = (sample index / buffer size) * sampling frequency
    for(int i = 0 ; i < real_buffer_size_ ; i ++ ){
        fft_freqs_[i] = (double(i)/double(buffer_size_))*sampling_frequency_;
    }
    return fft_freqs_;
}

double * FFT :: WindowHamming(double *signal){
    
    double * windowed = new double [buffer_size_];
    
    for(int i = 0 ; i < buffer_size_ ; i++){
        windowed[i] = signal[i] * window_buffer_[i];
    }
    
    return windowed;
}
double * FFT :: UnWindowHamming(double *signal){
    
    double * unwindowed = new double [buffer_size_];
    for(int i = 0 ; i < buffer_size_ ; ++i){
        unwindowed [i] = signal[i] / window_buffer_[i];
    }
    return unwindowed;
    
}
