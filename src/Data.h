#ifndef __DATA_H
#define __DATA_H

#include <bits/stdc++.h>
#include "debug.h"
using namespace std;

class User;
class Node;
class Link;

class Data {
    string type;          // data type: raw data or tree
    
protected:
    long long data_cnt;      // # of raw data in this data
    vector<Link*> path; // every links along path in TEG
    
public:
    inline const static string RAWDATA = "RAWDATA";
    inline const static string TREE = "TREE";
    
    Data(string type, long long data_cnt = 1);
    virtual ~Data(){};
    string getType() const;
    long long getDataCnt() const;
    vector<Link*> getPath() const;
    
    long long getSize() const;
    virtual int getGrid() const=0;
    virtual string getName() const=0;
    void pathAdd(Link* link); // links along with path in TEG
};

class RawData: public Data {
    User* user;          // which user this data belongs to
public:
    RawData(User *user, long long data_cnt = 1);
    ~RawData(){};
    User* getUser() const;

    int getGrid() const;
    string getName() const;
};

class Tree: public Data {
    int grid;          // which grid this data belongs to
public:
    Tree(int grid, long long data_cnt);
    ~Tree(){};

    int getGrid() const;
    
    string getName() const;
    void insert(RawData* data);
};

#endif