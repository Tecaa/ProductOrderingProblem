#include "stdafx.h"
#include "Solucion.h"

Solucion::Solucion()
{}
Solucion::Solucion(vector<Product> &products, vector<Proveedor> &providers, int presupuesto, vector<Pack> &packs)
{
	this->productos = products;
	this->proveedores = providers;
	this->presupuesto = presupuesto;
	this->packs = packs;
}
int Solucion::GetFOValue()
{
	return this->fo_value;
}
int Solucion::GetTotalCostos()
{
	return this->total_costos;
}
int Solucion::GetTotalCostosFixed()
{
	return this->total_costos_fixed;
}
void Solucion::CalcularValoresInternos()
{
	int fo = 0;
	int costos = 0;

	if (this->productos[1].GetCant_solicitar() == 2 && this->productos[2].GetCant_solicitar() == 2 && this->packs[0].GetCant_solicitar() == 1)
		int asdf = 2;
	for (vector<Product>::iterator it = this->productos.begin(); it != this->productos.end(); ++it)
	{
		Product p = (*it);
		int cant = p.GetCant_solicitar();
		float descuento = proveedores[p.index_proveedor].GetDescuentoAplicado();
		int ganancia = (p.valor_venta - p.valor_compra * descuento) * cant;
		fo += ganancia;
		costos += p.GetCostoTotal(proveedores);
	}
	for (vector<Pack>::iterator it = this->packs.begin(); it != this->packs.end(); ++it)
	{
		int  costo_parcial = 0;
		for (vector<Product*>::iterator itt = it->productos.begin(); itt != it->productos.end(); ++itt)
		{
			Product* p = *itt;
			float descuento = proveedores[p->index_proveedor].GetDescuentoAplicado();
			costo_parcial += descuento * p->valor_compra;
		}
		int ganancia = (it->valor_venta - costo_parcial) * it->GetCant_solicitar();

		fo += ganancia;
		costos += costo_parcial * it->GetCant_solicitar();
	}

	this->total_costos = costos;
	this->fo_value = fo;
}
//Calcula valores internos fixed y reales
void Solucion::CalcularValoresInternosFixed(vector<Descuento> discountByProvider)
{
	int fo = 0;
	int costos = 0;
	int foReal = 0;
	int costosReal = 0;

	for (vector<Product>::iterator it = this->productos.begin(); it != this->productos.end(); ++it)
	{
		Product p = (*it);
		int cant = p.GetCant_solicitar();
		float descuento = discountByProvider[p.index_proveedor].percentage;
		int ganancia = (p.valor_venta - p.valor_compra * descuento) * cant;
		fo += ganancia;
		costos += p.GetCostoTotalFixed(discountByProvider);

		float descuentoReal = proveedores[p.index_proveedor].GetDescuentoAplicado();
		int gananciaReal = (p.valor_venta - p.valor_compra * descuentoReal) * cant;
		foReal += gananciaReal;
		costosReal += p.GetCostoTotal(proveedores);
	}

	for (vector<Pack>::iterator it = this->packs.begin(); it != this->packs.end(); ++it)
	{
		int costo_parcial_fixed = 0;
		for (vector<Product*>::iterator itt = it->productos.begin(); itt != it->productos.end(); ++itt)
		{
			Product* p = *itt;
			float descuento = discountByProvider[p->index_proveedor].percentage;
			costo_parcial_fixed += p->GetCostoTotalFixed(discountByProvider);
		}
		int ganancia = (it->valor_venta - costo_parcial_fixed) * it->GetCant_solicitar();
		fo += ganancia;
		costos += costo_parcial_fixed;



		int  costo_parcial = 0;
		for (vector<Product*>::iterator itt = it->productos.begin(); itt != it->productos.end(); ++itt)
		{
			Product* p = *itt;
			float descuento = proveedores[p->index_proveedor].GetDescuentoAplicado();
			costo_parcial += descuento * p->valor_compra;
		}
		ganancia = (it->valor_venta - costo_parcial) * it->GetCant_solicitar();

		foReal += ganancia;
		costosReal += costo_parcial;
	}



	this->total_costos_fixed = costos;
	this->fo_value_fixed = fo;
	this->total_costos = costosReal;
	this->fo_value = foReal;
}
int Solucion::Presupuesto()
{
	return this->presupuesto;
}
Solucion Solucion::clone()
{
	Solucion s = (Solucion());
	s.productos = this->productos;
	for (int j = 0; j < this->proveedores.size(); j++)
		s.proveedores.push_back(this->proveedores[j].clone());

	for (int j = 0; j < this->packs.size(); j++)
		s.packs.push_back(this->packs[j].clone());

	s.fo_value = this->fo_value;
	s.total_costos = this->total_costos;
	s.presupuesto = this->presupuesto;
	return s;
}
Solucion::~Solucion()
{
}