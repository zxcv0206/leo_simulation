#include "AlgorithmBase.h"

AlgorithmBase::AlgorithmBase(string algoName, string graphFile)
    :algoName(algoName),timeslot(0) {
    buildGraph(graphFile);
    addReq();
}

AlgorithmBase::~AlgorithmBase(){
    for(int t = 0; t < total_timeslots; t++) {
        for(int n = 0; n < total_nodes; n++){
            for(auto &e: links[t][n]) {
                delete e.second;
                e.second = nullptr;
            }
        }
        links[t].clear();
    }
    links.clear();

    for(int t = 0; t < total_timeslots; t++) {
        for(int n = 0; n < total_nodes; n++){
            delete nodes[t][n];
            nodes[t][n] = nullptr;
        }
        nodes[t].clear();
    }
    nodes.clear();

    for(int g = 0; g < total_grids; g++) {
        for(auto &raw_data: raw_datas[g]) {
            delete raw_data;
        }
        raw_datas[g].clear();
        delete trees[g];
    }
    raw_datas.clear();
    trees.clear();
}

string AlgorithmBase::getName(){
    return algoName;
}

void AlgorithmBase::buildGraph(string file){
    graphIn.open(file);
    if(graphIn.fail()){
        cout << "input file opening failed";
        exit(1); // 程式錯誤終止
    }
    
    graphIn >> total_satellites >> total_grids >> total_timeslots;
    long long capacity;
    graphIn >> capacity;
    // construct nodes
    nodes.resize(total_timeslots+1);
    for(int s = 0; s < total_satellites; s++){
        for(int t = 0; t < total_timeslots+1; t++) {
            Node *satellite = new Satellite(s, t, capacity);
            nodes[t].push_back(satellite);
        }
    }
    total_users = 0;
    user_cnts.resize(total_grids);
    for(int g = 0; g < total_grids; g++) {
        graphIn >> user_cnts[g];
        for(int u = total_users; u < total_users+user_cnts[g]; u++) {
            for(int t = 0; t < total_timeslots+1; t++) {
                Node *user = new User(total_satellites+u, g, t);
                nodes[t].push_back(user);
            }
        }
        total_users += user_cnts[g];
    }

    // construct links
    links.resize(total_timeslots);
    total_nodes = total_satellites + total_users;
    for(int t = 0; t < total_timeslots; t++){
        links[t].resize(total_nodes);
    }

    for(int t = 0; t < total_timeslots; t++){
        int link_cnt;
        graphIn >> link_cnt;
        int u, v;
        // bidirectional link
        for(int e = 0; e < link_cnt; e++){
            graphIn >> u >> v;
            links[t][u][v] = new Link(nodes[t][u], nodes[t+1][v], 1);
            links[t][v][u] = new Link(nodes[t][v], nodes[t+1][u], 1);
        }
        // storage link
        for(int n = 0; n < total_nodes; n++) {
            links[t][n][n] = new Link(nodes[t][n], nodes[t+1][n]);
        }
    }
    graphIn.close();
}


void AlgorithmBase::addReq(){
    raw_datas.resize(total_grids);
    trees.resize(total_grids);

    // all users has its raw data in the beginning
    for(int u = total_satellites; u<total_satellites+total_users; u++){
        User* user = (User *)nodes[0][u];
        RawData* raw_data = new RawData(user);
        user->dataIn(raw_data);
        DEBUG("AlgorithmBase::addReq : " << raw_data->getName());
        raw_datas[user->getGrid()].push_back(raw_data);
    }
}

void AlgorithmBase::start() {

}

void AlgorithmBase::debug(){
    // TEG
    debugOut << "TEG" << endl;
    for(int t = 0; t < total_timeslots; t++) {
        debugOut << "timeslot " << t << endl;
        for(int u = 0; u < total_nodes; u++) {
            for(auto &links_kv: links[t][u]) {
                Link* link = links_kv.second;
                debugOut << link->getName() << endl;
            }
        }
    }
    debugOut << "=================" << endl;

    // data path
    debugOut << "Data path" << endl;
    for(auto &raw_datas_g: raw_datas) {
        for(auto &raw_data: raw_datas_g) {
            debugOut << raw_data->getName() + " path:" << endl;
            for(auto &node: raw_data->getPath()) {
                debugOut << node->getName() << " -> ";
            }
            debugOut << endl;
        }
    }
    debugOut << "=================" << endl;
}