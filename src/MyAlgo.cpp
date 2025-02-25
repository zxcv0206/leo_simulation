#include "MyAlgo.h"


MyAlgo::MyAlgo(string graphFile)
    :AlgorithmBase("MyAlgo", graphFile), decided_cnt(0){
	new_raw_data_cnts.resize(total_grids);
	for(auto &new_raw_data_cnts_g: new_raw_data_cnts) {
		new_raw_data_cnts_g.resize(total_satellites, 0);
	}

	new_raw_datas.resize(total_grids);
	for(auto &new_raw_datas_g: new_raw_datas) {
		new_raw_datas_g.resize(total_satellites);
	}

	compress_sats.resize(total_timeslots);
	for(auto &compress_sats_t: compress_sats) {
		compress_sats_t.resize(total_grids, -1);
	}

	finished_times.resize(total_timeslots);
	for(auto &finished_times_t: finished_times) {
		finished_times_t.resize(total_grids, -1);
	}

	finished_sizes.resize(total_timeslots);
	for(auto &finished_sizes_t: finished_sizes) {
		finished_sizes_t.resize(total_grids, -1);
	}
}


void MyAlgo::start() {
	calHops();
	for(timeslot = 0; timeslot < total_timeslots; timeslot++) {
		uploadRawDatas();
		findCompressSat();
		findAllPaths();
		compression();
		refresh();

		
		if(decided_cnt == total_users) {
			for(auto tree: trees) {
				res = max(res, tree->getPath().back()->getTo()->getTimeslot());
			}
			return;
		}
	}
}

void MyAlgo::calHops() { // need to precalculate hops for future timeslot for Tree 
	hop_cnts.resize(total_timeslots);
	for(int t = 0; t < total_timeslots; t++) {
		hop_cnts[t].resize(total_satellites);
		for(int s1 = 0; s1 < total_satellites; s1++) {
			hop_cnts[t][s1].resize(total_satellites);
			for(int s2 = 0; s2 < total_satellites; s2++) {
				if(s1 == s2) hop_cnts[t][s1][s2] = 0;
				else if(links[t][s1].find(s2) != links[t][s1].end()) hop_cnts[t][s1][s2] = 1;
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

void MyAlgo::refresh() {
	for(int g = 0; g < total_grids; g++) {
		for(int s = 0; s < total_satellites; s++) {
			new_raw_data_cnts[g][s] = 0;
			new_raw_datas[g][s].clear();
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
					new_raw_datas[grid][to].push_back(data);
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

void MyAlgo::findAllPaths() {
	// tree
	for(int g = 0; g < total_grids; g++) {
		if(trees[g] == nullptr) continue;
		int t_start = finished_times[timeslot-1][g];
		int s = compress_sats[timeslot-1][g];
		int d = compress_sats[timeslot][g];
		int r = finished_sizes[timeslot-1][g];
		findPath(t_start, s, d, r, trees[g]);
	}
	// raw datas
	for(int g = 0; g < total_grids; g++) {
		for(int s = 0; s < total_satellites; s++) {
			for(auto data: new_raw_datas[g][s]) {
				int d = compress_sats[timeslot][g];
				int r = 1;
				findPath(timeslot, s, d, r, data);
			}
		}
	}
}

void MyAlgo::findPath(int t_start, int s, int d, int r, Data* data) {
	//--build G^r
    vector<vector<map<int, Link*>>> vlinks(total_timeslots, vector<map<int, Link*>>(total_nodes)); // vlinks[t][u][v]: links in G^r
	Link* vlink;
	for(int t = t_start; t < total_timeslots; t++) {
		for(int u = 0; u < total_nodes; u++) {
			// self storage
			vlink = new Link(nodes[t][u], nodes[t+1][u]);
			vlinks[t][u][u] = vlink;
			// other links need to be able r timslots to be vlink
			for(auto &link_kv: links[t][u]) {
				int v = link_kv.first;
				if (v == u) continue;
				bool fg = true;
				for(int t_plum = t; t_plum < t+r; t_plum++) {
					if(links[t_plum][u].find(v) == links[t_plum][u].end() || !links[t_plum][u][v]->isAble()) {
						fg = false;
						break;
					}
				}
				if(fg) {
					vlink = new Link(nodes[t][u], nodes[t+r][u], 1, r);
					vlinks[t][u][v] = vlink;
				}
			}
		}
	}

	//--Dijkstra
	vector<vector<int>> dist(total_timeslots, vector<int>(total_nodes, INT_MAX));
	vector<vector<Link*>> prev(total_timeslots, vector<Link*>(total_nodes, nullptr));
	priority_queue<pair<int,pair<int, int>>, vector<pair<int,pair<int, int>>>, greater<>> pq; // {dst,{t,id}}
	
	dist[t_start][s] = 0;
	pq.push({0, {t_start, s}});

	while (!pq.empty()) {
        auto [dst, tu] = pq.top(); 
		pq.pop();
        auto [t, u] = tu;

        if (dst > dist[t][u]) continue;

		if(u == d) {
			// get shortest path
			vector<Link*> short_path;
			int t_plum = t;
			int u_plum = u;
			while(prev[t_plum][u_plum] != nullptr) {
				vlink = prev[t_plum][u_plum];
				short_path.push_back(vlink);
				const Node* from = vlink->getFrom();
				t_plum = from->getTimeslot();
				u_plum = from->getId();
			}
			assert(t_plum == t_start && u_plum == s);
			// assign link
			Link* link;
			int now_node = s, next_node, now_t = t_start;
			int cnt; // how many links consist the vlink
			for(int i = short_path.size()-1; i >= 0; i--) {
				vlink = short_path[i];
				next_node = vlink->getTo()->getId();
				if(next_node == now_node) cnt = 1; // self storage
				else cnt = r; // send to other nodes
				for(int t = now_t; t < now_t+cnt; t++) {
					link = links[t][now_node][next_node];
					link->assign(data);
				}
			} 
			return;
		}

        for (auto vlinks_kv : vlinks[t][u]) {
			vlink = vlinks_kv.second;
			const Node* v_node = vlink->getTo();
			int t_plum = v_node->getTimeslot();
			int v = v_node->getId();
            if (dist[t][u] + vlink->getWeight() < dist[t_plum][v]) {
                dist[t_plum][v] = dist[t][u] + vlink->getWeight();
				prev[t_plum][v] = vlink;
                pq.push({dist[t_plum][v], {t_plum, v}});
            }
        }
    }
}

void MyAlgo::compression() {
	// get # of raw data should be compressed and arrival time
	vector<int> compress_cnts(total_grids, 0); 
	vector<pair<int, Data*>> arrivals;
	// tree
	for(auto tree: trees) {
		if(tree == nullptr) continue;
		const Node* compress_sat = tree->getPath().back()->getTo();
		int arrival_time = compress_sat->getTimeslot();
		arrivals.push_back({arrival_time, tree});
	}
	// raw data
	for(int g = 0; g < total_grids; g++) {
		for(int s = 0; s < total_satellites; s++) {
			for(auto data: new_raw_datas[g][s]) {
				const Node* compress_sat = data->getPath().back()->getTo();
				int arrival_time = compress_sat->getTimeslot();
				compress_cnts[g]++;
				arrivals.push_back({arrival_time, data});
			}
		}
	}
	sort(arrivals.begin(), arrivals.end());

	int now = timeslot;
	vector<bool> has_tree(total_grids, false); // has_tree[g]: true if tree from g arrived to compress sat s
	vector<map<int,queue<RawData*>>> que(total_satellites); // que[s][g]: all datas from g has arrived to compress sat s  
	for(auto arrival_p: arrivals) {
		auto [t, this_data] = arrival_p;

		// TODO: different compression time
		while(now < t) { // compress greedily 
			for(int s = 0; s < total_satellites; s++) {
				for(auto &[g, q]: que[s]) {
					// if(compress_cnts[g]>0 && has_tree[g] && !q.empty()) {
						if(compress_cnts[g]>0 && !q.empty()) {
						// first compress data, create tree
						if(trees[g] == nullptr) {
							trees[g] = new Tree(g,0);
							has_tree[g] = true;
							for(int t_plum = 0; t_plum < now; t_plum++) {
								trees[g]->pathAdd(nullptr);
							}
						}
						if(!has_tree[g]) continue; // tree not arrive

						RawData* data = q.front();
						q.pop();
						trees[g]->insert(data);
						compress_cnts[g]--;
						decided_cnt++;

						// raw data storage path
						int arrival_time = data->getPath().back()->getTo()->getTimeslot();
						for(int t_plum = arrival_time; t_plum < now; t_plum++) {
							links[t_plum][s][s]->assign(data);
						}
						
						
						if(compress_cnts[g] == 0) {
							// tree storage path
							if(trees[g]->getPath().size() == 0 || trees[g]->getPath().back() == nullptr) arrival_time = now;
							else arrival_time = trees[g]->getPath().back()->getTo()->getTimeslot();
							for(int t_plum = arrival_time; t_plum <= now; t_plum++) {
								links[t_plum][s][s]->assign(trees[g]);
							}

							finished_times[timeslot][g] = now+1; // TODO: different compression time
							finished_sizes[timeslot][g] = trees[g]->getSize();
							que[s].erase(g);
						}
						break;
					}
				}
			}
			now++;
		}

		int g = this_data->getGrid();
		int s = compress_sats[timeslot][g];
		if(this_data->getType() == Data::TREE) has_tree[g] = true;
		else {
			que[s][g]; // make que[s][g] exist
			que[s][g].push((RawData*)this_data);
		}
	} 

	// compress remain raw data
	bool has_raw_data = true;
	while (has_raw_data) {
		has_raw_data = false;
		for(int s = 0; s < total_satellites; s++) {
			for(auto &[g, q]: que[s]) {
				has_raw_data = true;
				// if(compress_cnts[g]>0 && has_tree[g] && !q.empty()) {
				if(compress_cnts[g]>0 && !q.empty()) {
					// first compress data, create tree
					if(trees[g] == nullptr) {
						trees[g] = new Tree(g,0);
						has_tree[g] = true;
						for(int t_plum = 0; t_plum < now; t_plum++) {
							trees[g]->pathAdd(nullptr);
						}
					}

					RawData* data = q.front();
					q.pop();
					trees[g]->insert(data);
					compress_cnts[g]--;
					decided_cnt++;

					// raw data storage path
					int arrival_time = data->getPath().back()->getTo()->getTimeslot();
					for(int t_plum = arrival_time; t_plum < now; t_plum++) {
						links[t_plum][s][s]->assign(data);
					}
					
					
					if(compress_cnts[g] == 0) {
						// tree storage path
						if(trees[g]->getPath().size() == 0 || trees[g]->getPath().back() == nullptr) arrival_time = now;
						else arrival_time = trees[g]->getPath().back()->getTo()->getTimeslot();
						for(int t_plum = arrival_time; t_plum <= now; t_plum++) {
							links[t_plum][s][s]->assign(trees[g]);
						}

						finished_times[timeslot][g] = now+1; // TODO: different compression time
						finished_sizes[timeslot][g] = trees[g]->getSize();
						que[s].erase(g);
					}
					break;
				}
			}
		}
		now++;
	}
	

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