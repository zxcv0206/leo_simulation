#include "MyAlgo.h"


MyAlgo::MyAlgo(string graphFile)
    :AlgorithmBase("MyAlgo", graphFile){}


void MyAlgo::start() {
	for(int t = 0; t < total_timeslots; t++) {
		// findCompressSat();
		// findPath();
	}
}