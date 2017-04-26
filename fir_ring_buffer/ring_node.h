//
//  ring_node.h
//  TuneTheRoom
//
//  Created by Szymon Mrozek on 21.03.2017.
//  Copyright © 2017 Szymon Mrozek. All rights reserved.
//

#ifndef ring_node_h
#define ring_node_h


class RingNode{
    
    
private:
    
    RingNode(double value);
    ~RingNode();
    
    void Insert(double value);
    void Add(double value);
    void Remove(RingNode *to_remove);
    
    
    double value_;
    RingNode * next_;
    RingNode * previous_;
    
    friend class FirRingBuffer;
    
};

#endif /* ring_node_h */
