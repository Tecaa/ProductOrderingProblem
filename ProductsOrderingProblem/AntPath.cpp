#include "stdafx.h"
#include "AntPath.h"


AntPath::AntPath(int producto_index, int cantidad_producto, float discount, float initial_pheromone)
{
	this->cantidad_producto = cantidad_producto;
	this->producto_index = producto_index;
	this->feromona = 0;
	this->path_discount = discount;
	this->feromona = initial_pheromone;
}


AntPath::~AntPath()
{
}
