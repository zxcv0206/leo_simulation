#ifndef __ALGO_BASE_H
#define __ALGO_BASE_H

#include "Data.h"
#include "Node.h"
#include "Link.h"

using namespace std;

class AlgorithmBase{
    string algoName;
    fstream graphIn;
    void buildGraph(string file); // build time-expand graph according to input
    void addReq();
    void final();

protected:
    int total_satellites, total_users, total_nodes, total_grids, total_timeslots;
    int timeslot;
    vector<int> user_cnts; // user_cnt[g]: # users in grid g
    vector<vector<Node *>> nodes; // nodes[t][id]
    vector<vector<map<int, Link*>>> links;             // links[t][u][v]
    vector<vector<RawData*>> raw_datas; // raw_datas[g][i] 
    vector<Tree*> trees; // trees[g]

    // two graphs stores the same data, but different data structure
    // vector<map<pair<int, int>, Channel *>> graphs_matrix;   // graphs[t][{u, v}] -> Channel*  
    
    map<string, double> res;
    // void assignChannel(const Request &r, int t, int from, int to, long long data_size);
    // virtual void assignChannels()=0;

public:
    AlgorithmBase(string algoName, string graphFile);
    virtual ~AlgorithmBase();
    double getRes(string s);
    string getName();
    
    virtual void start();
    virtual void debug();
};

#endif