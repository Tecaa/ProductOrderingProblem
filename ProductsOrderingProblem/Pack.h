#pragma once
#include <vector>
#include "Product.h"

class Pack
{
private:
	int id;
	int index;
	
	
	int cant_solicitar;
	
public:
	int stock_actual;
	int cant_mensual;
	vector<Product*> productos;
	Pack();
	Pack(int index, int id, int valor_venta, int cant_mensual, int stock_actual);
	~Pack();
	void AddProduct(Product* p);
	int GetCostoTotal(vector<Proveedor> proveedores);
	int GetCostoTotalFixed(vector<Descuento> descuentosPorProveedor);
	int SetCant_solicitar(int valor);
	int GetCant_solicitar();
	int valor_venta;
	Pack clone();
};