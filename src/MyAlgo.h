#ifndef __MYALGO_H
#define __MYALGO_H

#include "AlgorithmBase.h"

typedef pair<long long, long long> plli;
const long long INF = 0x3f3f3f3f3f3f3f3f;


class MyAlgo: public AlgorithmBase{

public:
    MyAlgo(string graphFile);

    void start() override;
};



#endif