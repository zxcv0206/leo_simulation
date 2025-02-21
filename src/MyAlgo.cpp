#include "MyAlgo.h"


MyAlgo::MyAlgo(string graphFile)
    :AlgorithmBase("MyAlgo", graphFile){
	new_raw_data_cnts.resize(total_grids);
	for(auto &new_raw_data_cnts_g: new_raw_data_cnts) {
		new_raw_data_cnts_g.resize(total_satellites);
	}
}


void MyAlgo::start() {
	for(timeslot = 0; timeslot < total_timeslots; timeslot++) {
		init();
		uploadRawDatas();
		// findCompressSat();
		// findPath();
	}
}

void MyAlgo::init() {
	for(int g = 0; g < total_grids; g++) {
		for(int s = 0; s < total_satellites; s++) {
			new_raw_data_cnts[g][s] = 0;
		}
	}
}

void MyAlgo::uploadRawDatas() {
	// if user connected with sat, then upload
	for(int u = total_satellites; u < total_nodes; u++) {
		User* user = (User*)nodes[timeslot][u];

		Data* data;
		Link* link;
		int to;
		if ((data = user->getRawData()) != nullptr) { // user still has raw data
			int grid = user->getGrid();
			for(auto &link_kv: links[timeslot][u]) {
				to = link_kv.first;
				link = link_kv.second;
				if(link->isAble()) {
					link->assign(data);
					new_raw_data_cnts[grid][to]++;
					break;
				}
			}
		}
	}
}

// void MyAlgo::findCompressSat(){
// 	// get every grid raw data access sat (path first node)

// }
