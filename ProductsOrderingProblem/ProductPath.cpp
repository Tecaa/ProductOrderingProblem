#include "stdafx.h"
#include "ProductPath.h"
#include "Descuento.h"

ProductPath::ProductPath(int product_index, int max_camino, int provider_index, float initial_pheromone)
{
	for (int i = 0; i < Descuento::DescuentosPorProveedor.at(provider_index).size(); i++)
	{
		MultiplePath mp = MultiplePath(product_index, max_camino, Descuento::DescuentosPorProveedor.at(provider_index).at(i).percentage, initial_pheromone);
		this->ProductByDiscountPaths.push_back(mp);
	}
}


ProductPath::~ProductPath()
{
}
