#ifndef __NODE_H
#define __NODE_H

#include <bits/stdc++.h>
#include "Data.h"
using namespace std;

class Node {
protected:
    int id;
    int timeslot;      // for index in TEG
    int type;          // node type: satellite(0) or user(1) node
    long long capacity;      // total storage on this node
    long long used_capacity;       // number of volume store at the node
    // raw_datas[i]: raw datas belong to grid i
    map<int, queue<Data*>> raw_datas;
    // tree[i]: the tree belongs to grid i
    map<int, Data*> trees;

public:
    static const long long MAX_CAPACITY = 1e13;
    inline const static int SATELLITE = 0;
    inline const static int USER = 1;
    
    Node(int id, int timeslot, int type, long long capacity=MAX_CAPACITY);
    virtual ~Node(){};
    int getId() const;
    int getTimeslot() const;
    int getType() const;
    long long getCapacity()const;
    long long getUsedCapacity()const;
    
    virtual string getName() const=0;
    /**
     * \brief data transimit to this node
     */
    void dataIn(Data *data);

    /**
     * \brief data transimit from this node
     */
    void dataOut(Data *data);
};


class User: public Node{
    int grid;

public:
    User(int id, int grid, int timeslot);
    ~User(){};
    int getGrid() const;
    Data* getRawData();

    string getName() const override;
};

class Satellite: public Node{
public:
    Satellite(int id, int timeslot, long long storage);
    ~Satellite(){};

    string getName() const override;
};

#endif