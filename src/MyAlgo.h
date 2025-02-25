#ifndef __MYALGO_H
#define __MYALGO_H

#include "AlgorithmBase.h"


class MyAlgo: public AlgorithmBase{
    vector<vector<vector<Data*>>> new_raw_datas;// new_raw_datas[g][s]: all raw datas from grid g which access sat is s in this timeslot
    vector<vector<int>> new_raw_data_cnts;// new_raw_data_cnts[g][s]: # raw data from grid g which access sat is s
    vector<vector<vector<long long>>> hop_cnts; // hop_cnts[t][s1][s2]: # hops for every sats pair in each timeslot
    vector<vector<int>> compress_sats; // compress_sats[t][g]: the sat where raw data of grid g received in timeslot t should be compressed
    vector<vector<int>> finished_times; // finished_times[t][g]: the timeslot when tree for timeslot t finished compression
    vector<vector<int>> finished_sizes; // finished_times[t][g]: the r size when tree for timeslot t finished compression

public:
    MyAlgo(string graphFile);

    void start() override;
    void calHops();
    void init();
    void uploadRawDatas();
    void findCompressSat();
    void findAllPaths();
    void findPath(int t_start, int s, int d, int r, Data* data); // start in timeslot t, find path from s to d in G^r
    void compression();
    void debug() override;
};



#endif