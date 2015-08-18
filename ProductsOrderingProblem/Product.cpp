#include "stdafx.h"
#include "Product.h"

using namespace std;

Product::Product(int index, int id, int valor_compra, int valor_venta, int cant_mensual, int stock_actual, int index_proveedor)
{
	this->id = id;
	this->index = index;
	this->cant_mensual = cant_mensual;
	this->stock_actual = stock_actual;
	this->valor_compra = valor_compra;
	this->valor_venta = valor_venta;
	this->index_proveedor = index_proveedor;
	this->cant_solicitar = 0;
}

int Product::GetCostoTotal(vector<Proveedor> proveedores)
{
	return this->cant_solicitar * proveedores[this->index_proveedor].GetDescuentoAplicado() * this->valor_compra;
}
int Product::GetCostoTotalFixed(vector<Descuento> descuentosPorProveedor)
{
	return this->cant_solicitar * descuentosPorProveedor.at(this->index_proveedor).percentage * this->valor_compra;
}
int Product::SetCant_solicitar(int valor)
{
	int temp= valor - this->cant_solicitar;
	this->cant_solicitar = valor;
	return temp;
}
int Product::GetCant_solicitar(){
	return this->cant_solicitar;
}
Product::~Product()
{
}
