#pragma once
#include <vector>
#include "Proveedor.h"

using namespace std;
class Proveedor;
class Product
{
public:
	Product(int index, int id, int valor_compra, int valor_venta, int cant_mensual, int stock_actual, int index_proveedor);
	~Product();
	int id;
	int index;
	int valor_compra;
	int valor_venta;
	int cant_mensual;
	int stock_actual;
	int index_proveedor;
	int GetCostoTotal(vector<Proveedor> proveedores);
	int GetCostoTotalFixed(vector<Descuento> descuentosPorProveedor);
	int SetCant_solicitar(int valor);
	int GetCant_solicitar();
	
private:
	int cant_solicitar;
};

