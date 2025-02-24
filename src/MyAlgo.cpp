#include "MyAlgo.h"


MyAlgo::MyAlgo(string graphFile)
    :AlgorithmBase("MyAlgo", graphFile){
	new_raw_data_cnts.resize(total_grids);
	for(auto &new_raw_data_cnts_g: new_raw_data_cnts) {
		new_raw_data_cnts_g.resize(total_satellites, 0);
	}

	compress_sats.resize(total_timeslots);
	for(auto &compress_sats_t: compress_sats) {
		compress_sats_t.resize(total_grids, -1);
	}
}


void MyAlgo::start() {
	calHops();
	for(timeslot = 0; timeslot < total_timeslots; timeslot++) {
		init();
		uploadRawDatas();
		findCompressSat();
		findPath();
	}
}

void MyAlgo::calHops() { // need to precalculate hops for future timeslot for Tree 
	hop_cnts.resize(total_timeslots);
	for(int t = 0; t < total_timeslots; t++) {
		hop_cnts[t].resize(total_satellites);
		for(int s1 = 0; s1 < total_satellites; s1++) {
			hop_cnts[t][s1].resize(total_satellites);
			for(int s2 = 0; s2 < total_satellites; s2++) {
				if(links[t][s1].find(s2) != links[t][s1].end()) hop_cnts[t][s1][s2] = 1;
				else hop_cnts[t][s1][s2] = LONG_LONG_MAX;
			}
		}
		// Floyd-Warshall
		for (int k = 0; k < total_satellites; k++) {  
			for (int i = 0; i < total_satellites; i++) {  
				for (int j = 0; j < total_satellites; j++) { 
					if (hop_cnts[t][i][k] != LONG_LONG_MAX && hop_cnts[t][k][j] != LONG_LONG_MAX) {
						hop_cnts[t][i][j] = min(hop_cnts[t][i][j], hop_cnts[t][i][k] + hop_cnts[t][k][j]);
					}
				}
			}
		}
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
			for(auto &link_kv: links[timeslot][u]) { // check sats then user itself, cuz sats has smaller id 
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

void MyAlgo::findCompressSat() {
	DEBUG("MyAlgo::findCompressSat : ");
	// set (1)cost, (2)processing time, (3) makespan 	
	vector<vector<int>> costs(total_grids, vector<int>(total_satellites, 0)); // costs[g][s]: 𝑡𝑜𝑡𝑎𝑙 ℎ𝑜𝑝𝑠 𝑓𝑜𝑟 𝑒𝑎𝑐ℎ 𝑟𝑎𝑤 𝑑𝑎𝑡𝑎 𝑜𝑓 𝑔𝑟𝑖𝑑 g 𝑎𝑛𝑑 𝑇𝑟𝑒𝑒_𝑗  𝑡𝑜 𝑠𝑎𝑡𝑒𝑙𝑙𝑖𝑡𝑒 s
	vector<vector<int>> processes(total_grids, vector<int>(total_satellites, 0)); // processes[g][s]: # raw data of grid g
	int makespan = 0;
	for(int g = 0; g < total_grids; g++) {
		for(int s2 = 0; s2 < total_satellites; s2++) {
			for(int s1 = 0; s1 < total_satellites; s1++) {
				costs[g][s2] += hop_cnts[timeslot][s1][s2] * new_raw_data_cnts[g][s1];
				processes[g][s2] += new_raw_data_cnts[g][s1];
			}
			// TODO: TREE
			
			makespan = max(makespan, processes[g][s2]);

			DEBUG("cost(" << g << "," << s2 << ") = " << costs[g][s2]);
		}
	}

	// greedy algo for GAP
	vector<int> remain_makespan(total_satellites, makespan); // remain_makespan[s]
	vector<int> rho(total_grids); // rho[g]: desirability

	int decided_cnt = 0;
	while(decided_cnt < total_grids) {
		int max_rho = -1, max_rho_g = -1, best_g_s = -1;
		for(int g = 0; g < total_grids; g++) {
			if(compress_sats[timeslot][g] != -1) continue; // this grid compress sat was already decided
			int mn = INT_MAX, sec_mn = INT_MAX, mn_s = -1;
			for(int s = 0; s < total_satellites; s++) {
				if(processes[g][s] <= remain_makespan[s] && costs[g][s] <= mn) { // according to weight function
					sec_mn = mn;
					mn = costs[g][s];
					mn_s = s;
				}
			}
			if(sec_mn - mn > max_rho) {
				max_rho = sec_mn - mn;
				max_rho_g = g;
				best_g_s = mn_s;
			}
		}

		assert(best_g_s != -1);
		compress_sats[timeslot][max_rho_g] = best_g_s;
		remain_makespan[best_g_s] -= processes[max_rho_g][best_g_s];
		decided_cnt++;
	}
}

void MyAlgo::findPath() {
}

void MyAlgo::debug() {
	AlgorithmBase::debug();

	// compress sat
    debugOut << "Compress satellite" << endl;
    for(int t = 0; t < total_timeslots; t++) {
        debugOut << "timeslot " << t << endl;
		for(int g = 0; g < total_grids; g++) {
			debugOut << "g" << g << ": " << compress_sats[t][g] << endl;
		}
	}
    debugOut << "=================" << endl;
}