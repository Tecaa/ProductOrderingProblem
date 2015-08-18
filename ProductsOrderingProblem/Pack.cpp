#include "stdafx.h"
#include "Pack.h"
Pack::Pack()
{
}
Pack::Pack(int index, int id, int valor_venta, int cant_mensual, int stock_actual)
{
	this->id = id;
	this->index = index;
	this->cant_mensual = cant_mensual;
	this->stock_actual = stock_actual;
	//this->valor_compra = valor_compra;
	this->valor_venta = valor_venta;
	//this->indexes_proveedores = index_proveedor;
	this->cant_solicitar = 0;
}
void Pack::AddProduct(Product* p)
{
	this->productos.push_back(p);
}
Pack::~Pack()
{
}

int Pack::GetCostoTotal(vector<Proveedor> proveedores)
{
	int costo = 0;
	for (int i = 0; i < this->productos.size(); ++i)
	{
		costo += this->cant_solicitar * proveedores[this->productos[i]->index_proveedor].GetDescuentoAplicado() * this->productos[i]->valor_compra;
	}
	return costo;
}

int Pack::GetCostoTotalFixed(vector<Descuento> descuentosPorProveedor)
{
	int costo = 0;
	for (int i = 0; i < this->productos.size(); ++i)
	{
		costo += this->cant_solicitar * descuentosPorProveedor.at(this->productos[i]->index_proveedor).percentage * this->productos[i]->valor_compra;
	}
	return costo;
}

int Pack::SetCant_solicitar(int valor)
{
	int temp = valor - this->cant_solicitar;
	this->cant_solicitar = valor;
	/*
	for (int i = 0; i < this->productos.size(); ++i)
	{
		Proveed
		Pack::solicitadosPorProveedor[this->productos[i]->index_proveedor] += temp;
		//actualiazmos los productos en si
		//this->productos[i]->SetCant_solicitar(this->productos[i]->GetCant_solicitar() + temp);
	}*/

	return temp;
}
int Pack::GetCant_solicitar(){
	return this->cant_solicitar;
}
Pack Pack::clone()
{
	Pack p = Pack();
	p.index = this->index;
	p.productos = this->productos;
	p.cant_mensual = this->cant_mensual;
	p.cant_solicitar = this->cant_solicitar;
	p.id = this->id;
	p.stock_actual = this->stock_actual;
	p.valor_venta = this->valor_venta;
	return p;
}