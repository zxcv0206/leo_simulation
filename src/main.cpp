#include "MyAlgo.h"
#include<bits/stdc++.h>
using namespace std;

const bool DEBUG = true;

int main(){
    int total_testcases = 0;
   
    
    for(int testcase = 0; testcase <= total_testcases; testcase++) {
        string graphFile = "../testcase/" + to_string(testcase) + ".graph";
        string resFile = "../testcase/" + to_string(testcase) + ".ans";
        fstream resOut(resFile, ios::out);

        vector<AlgorithmBase *> algos;
        algos.emplace_back(new MyAlgo(graphFile));
        
        for(AlgorithmBase *algo:algos){
            cout << algo->getName() << endl;
            algo->start();
            if(DEBUG) algo->debug();
        }

        // delete algo
        for(AlgorithmBase *algo:algos){
            delete algo;
        }
    }
}