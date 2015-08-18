#include "stdafx.h"
#include "MultiplePath.h"

MultiplePath::MultiplePath(int product_index, int max_camino, float discount, float initial_pheromone)
{
	for (int i = 0; i <= max_camino; i++)
	{
		paths.push_back(AntPath(product_index, i, discount, initial_pheromone));
	}
}


MultiplePath::~MultiplePath()
{
}
