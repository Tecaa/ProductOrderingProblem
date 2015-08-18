#pragma once
#include "MultiplePath.h"
class ProductPath
{
public:
	ProductPath(int product_index, int max_camino, int provider_index, float initial_pheromone);
	~ProductPath();
	vector<MultiplePath> ProductByDiscountPaths;
};

