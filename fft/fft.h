//
//  Fft.h
//  Fft
//
//  Created by Szymon Mrozek on 14.03.2017.
//  Copyright © 2017 Szymon Mrozek. All rights reserved.
//

#ifndef fft_h
#define fft_h

// FFT Functionality :
// - calculate Fft - results saved into internal fftReal & fftImag arrays
// - IFft - returns signal
// - magnitude - returns Magnitude and calculates average
// - phase     - returns Phase
// - If you want to calculate IFft of fftReal and fftImag - use getters
// - If windowing option is enabled, it takes affect to both Fft and IFft functions

class FFT{
    
public:
    
    FFT(int buffer_size,double sampling_frequency,bool enable_window);
    ~FFT();

    double * Freqs();
    
    void Fft(double *signal);
    
    double * GetFftReal();
    double * GetFftImag();
    double * Magnitude();
    double * Phase();
    double GetAverrageFromMagnitude();

    
    double * IFft(double *real, double *imag);
    
    void WriteToFiles();
    
private:
    

    
    void FillRevertedIndexes();
    void FillSinesAndCosines();
    void FillWindowBuffer();
    
    void WindowHamming(double *signal);
    double * UnWindowHamming(double *signalCopy);
    
    
    // 1024 2048 4096 8192 16384 32768
    
    const int buffer_size_;
    const int real_buffer_size_;
    const double sampling_frequency_;
    
    int * indexes_;
    
    int butterflies_;
    
    double * sines_;
    double * cosines_;
    double * fft_freqs_;
    double * window_buffer_;
    
    double * fft_real_;
    double * fft_imag_;
    
    double * fft_magnitude_;
    double * fft_phase_;
    double average_from_magnitude_;
    
    bool is_window_enabled_;

    
};

#endif /* fft_h */
