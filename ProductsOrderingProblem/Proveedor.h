#pragma once
#include <vector>
#include "Descuento.h"
#include "Product.h"

using namespace std;

class Proveedor
{
public:
	Proveedor();
	Proveedor(int index, int id);
	~Proveedor();
	void SetDescuento(int range, float percentage);
	int index;
	int id;
	//static vector<Proveedor> Proveedores;
	vector<Descuento>* Descuentos;
	int total_solicitados;
	int total_pack_solicitados;
	float GetDescuentoAplicado();
	Proveedor clone();
};