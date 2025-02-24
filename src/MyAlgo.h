#ifndef __MYALGO_H
#define __MYALGO_H

#include "AlgorithmBase.h"


class MyAlgo: public AlgorithmBase{
    vector<vector<int>> new_raw_data_cnts;// new_raw_data_cnts[g][s]: # raw data from grid g which access sat is s
    vector<vector<vector<long long>>> hop_cnts; // hop_cnts[t][s1][s2]: # hops for every sats pair in each timeslot
    vector<vector<int>> compress_sats; // compress_sats[t][g]: the sat where raw data of grid g received in timeslot t should be compressed

public:
    MyAlgo(string graphFile);

    void start() override;
    void calHops();
    void init();
    void uploadRawDatas();
    void findCompressSat();
    void findPath();
    void debug() override;
};



#endif