#pragma once
#include <vector>
#include "AntPath.h"

using namespace std;
class MultiplePath
{
public:
	MultiplePath(int product_index, int max_camino, float discount, float initial_pheromone);
	~MultiplePath();
	vector<AntPath> paths;
};

