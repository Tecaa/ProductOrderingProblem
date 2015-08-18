#pragma once
class AntPath
{
private:
	int producto_index;
	int cantidad_producto;
	float path_discount;
public:
	AntPath(int product_index, int cantidad_producto, float discount, float initial_pheromone);
	float feromona;
	~AntPath();
	float Costo();
};

