#ifndef __CHANNEL_H
#define __CHANNEL_H

#include "Node.h"

using namespace std;

class Link{
    Node *from, *to;
    int weight;
    long long capacity;           // the number of data this link can transmit within a timeslot
    long long used_capacity;
    
public:
    static const long long MAX_CAPACITY = 1e13;
    
    Link(Node *from, Node *to, long long capacity=MAX_CAPACITY, int weight=1);
    const Node* getFrom() const;
    const Node* getTo() const;
    int getWeight() const;

    string getName() const;
    long long getRemainCapacity() const;
    bool isAble() const;
    void assign(Data* data);
    void transmit(Data* data);
};

#endif