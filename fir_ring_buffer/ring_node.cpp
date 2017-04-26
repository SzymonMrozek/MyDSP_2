//
//  ring_node.cpp
//  TuneTheRoom
//
//  Created by Szymon Mrozek on 21.03.2017.
//  Copyright © 2017 Szymon Mrozek. All rights reserved.
//

#include "ring_node.h"

RingNode :: RingNode(double value) : value_(value){
    
}
RingNode :: ~RingNode(){
    
    delete this -> next_;
    delete this -> previous_;
}


void RingNode:: Insert(double value){
    
    RingNode * temp = this -> next_;
    RingNode * new_node = new RingNode(value);
    
    temp -> previous_ = new_node;
    this -> next_     = new_node;
    
    new_node -> previous_ = this;
    new_node -> next_     = temp;
}

void RingNode :: Add(double value){
    
    
    RingNode  * temp = this;
    while(temp -> next_ != this){
        temp = temp -> next_;
    }
    temp->Insert(value);
    
}

void RingNode:: Remove(RingNode *to_remove){
    
    if( this -> next_ == to_remove ){
        if (to_remove -> next_){
            this -> next_ = to_remove -> next_;
            this -> next_ -> previous_ = this;
        }
        to_remove -> next_     = nullptr;
        to_remove -> previous_ = nullptr;
        delete  to_remove;
        
    }else{
        this->next_->Remove(to_remove);
    }
}

