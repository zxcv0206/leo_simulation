#ifndef DEBUG_H
#define DEBUG_H

#include <bits/stdc++.h>
using namespace std;

const bool DEBUG_MODE = true;

extern fstream debugOut;
#define DEBUG(x) if (DEBUG_MODE) debugOut << x << std::endl

#endif