#ifndef __DATA_H
#define __DATA_H

#include <bits/stdc++.h>
#include "debug.h"
using namespace std;

class User;

class Data {
    int type;          // data type: raw data(0) or tree(1)
    User *user;          // which user this data belongs to
    long long data_cnt;      // # of raw data in this data
    vector<pair<int,int>> path; // {to,from}

public:
    inline const static int RAWDATA = 0;
    inline const static int TREE = 1;
    
    Data(int type, User *user, long long data_cnt = 1);
    ~Data(){};
    int getType() const;
    User* getUser() const;
    long long getDataCnt() const;
    
    int getGrid() const;
    long long getSize() const;
    void pathAdd(pair<int,int> link); // add a link to path vector
};


#endif