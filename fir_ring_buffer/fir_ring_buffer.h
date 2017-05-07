//
//  fir_ring_buffer.h
//  TuneTheRoom
//
//  Created by Szymon Mrozek on 21.03.2017.
//  Copyright © 2017 Szymon Mrozek. All rights reserved.
//

#ifndef fir_ring_buffer_h
#define fir_ring_buffer_h


class RingNode;
class FirRingBuffer{
    
    
public:
    
    FirRingBuffer(double coefficients [], int length );
    
    bool IsEmpty();
    double* GetImpulseResponse();
    void SetImpulseResponse(double* impulse_response, int length);

    void AddToBuffer(double value);
    double OutputOfCurrent();
    
private:
    
    RingNode * buffer_;
    double * impulse_response_;
    
    bool is_empty_;
    int  length_;
    
    
};

#endif /* fir_ring_buffer_h */
