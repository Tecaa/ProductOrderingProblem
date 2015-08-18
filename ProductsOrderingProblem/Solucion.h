#pragma once
#include "Product.h"
#include "Pack.h"

using namespace std;
class Solucion
{
public:
	Solucion();
	Solucion(vector<Product> &productos, vector<Proveedor> &proveedores, int presupuesto, vector<Pack> &packs);
	~Solucion();
	vector<Product> productos;
	vector<Proveedor> proveedores;
	vector<Pack> packs;
	void CalcularValoresInternos();
	void CalcularValoresInternosFixed(vector<Descuento> discountByProvider);
	int GetTotalCostos();
	int GetTotalCostosFixed();
	int GetFOValue();
	Solucion clone();
	int Presupuesto();
private:
	int fo_value;
	int fo_value_fixed;
	int presupuesto;
	int total_costos;
	int total_costos_fixed;
};