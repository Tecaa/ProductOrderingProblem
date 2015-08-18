#pragma once
#include <map>
#include <vector>
using namespace std;
class Descuento
{
public:
	Descuento();
	Descuento(int unidades_min, int unidades_max, float porcentaje, int index);
	~Descuento();
	int unidades_min;
	int unidades_max;
	int index;
	float percentage;
	static vector<vector<Descuento>> DescuentosPorProveedor;
};

//typedef pair < int, vector<Descuento> > par;