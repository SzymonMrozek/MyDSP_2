//
//  fir_ring_buffer.cpp
//  TuneTheRoom
//
//  Created by Szymon Mrozek on 21.03.2017.
//  Copyright © 2017 Szymon Mrozek. All rights reserved.
//


#include <assert.h>
#include "fir_ring_buffer.h"
#include "ring_node.h"



FirRingBuffer :: FirRingBuffer(double coeeficients [], int length) : length_(length){
    
    if(length_ > 0){
        impulse_response_ = coeeficients;
        
        RingNode * new_node = new RingNode(0.0);
        
        buffer_ = new_node;
        buffer_ -> next_     = buffer_;
        buffer_ -> previous_ = buffer_;
        
        
        for(int i = 1 ; i < length_ ; i ++ ){
            buffer_->Add(0.0);
        }
        is_empty_ = false;
    }else{
        is_empty_ = true;
    }
}

bool FirRingBuffer :: IsEmpty(){
    return is_empty_;
    
}

double * FirRingBuffer :: GetImpulseResponse(){
    
    return impulse_response_;
}

void FirRingBuffer::SetImpulseResponse(double *impulse_response) {
    impulse_response_ = impulse_response;
}

void FirRingBuffer :: AddToBuffer(double value){
    assert(!is_empty_);
    
    buffer_ = buffer_ -> next_;
    buffer_ -> value_ = value;
    
}

double FirRingBuffer :: OutputOfCurrent(){
    
    assert(!is_empty_);
    double output = 0.0;
    
    RingNode * buffer_iterator = buffer_;
    
    for(int i = 0 ; i < length_; i ++){
        output += (buffer_iterator -> value_) * impulse_response_[i];
        buffer_iterator = buffer_iterator -> previous_;
    }
    return output;
}








