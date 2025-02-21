#ifndef __MYALGO_H
#define __MYALGO_H

#include "AlgorithmBase.h"


class MyAlgo: public AlgorithmBase{
    // vector<vector<Data*>> new_raw_datas; //new_raw_datas[i]: raw data recieved from grid i in this timeslot
    vector<vector<int>> new_raw_data_cnts;// new_raw_data_cnts[g][s]: # raw data from grid g which access sat is s
public:
    MyAlgo(string graphFile);

    void start() override;
    void init();
    void uploadRawDatas();
};



#endif