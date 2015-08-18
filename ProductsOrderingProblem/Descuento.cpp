#include "stdafx.h"
#include "Descuento.h"
#include <map>
//map<int, vector<Descuento>> DescuentosPorProveedor;
vector<vector<Descuento>> DescuentosPorProveedor;
Descuento::Descuento(int _unidades_min, int _unidades_max, float _porcentaje, int index)
{
	this->percentage = _porcentaje;
	this->unidades_max = _unidades_max;
	this->unidades_min = _unidades_min;
	this->index = index;
}

Descuento::Descuento()
{
}


Descuento::~Descuento()
{
}
