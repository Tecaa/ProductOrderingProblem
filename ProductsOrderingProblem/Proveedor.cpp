#include "stdafx.h"
#include "Proveedor.h"
#include "Pack.h"
#include <iostream>

Proveedor::Proveedor()
{
}
Proveedor::Proveedor(int index, int id)
{
	this->index = index;
	this->id = id;
	this->total_solicitados = 0;
	this->total_pack_solicitados = 0;
}

/*
	Ejemplo: [[6,1],[15,0.95],[0,0.9]]
	Significa:
	0 -> 6		Costo 1
	7 -> 15		Costo 0.95
	16 -> inf	Costo 0.9
*/
float Proveedor::GetDescuentoAplicado()
{
	float descuento = 1;
	for (int i = 0; i < (*this->Descuentos).size(); i++)
	{
		if (this->total_solicitados + this->total_pack_solicitados <= (*this->Descuentos).at(i).unidades_max
			|| (*this->Descuentos).at(i).unidades_max == 0)
		{
			descuento = (*this->Descuentos).at(i).percentage;
			break;
		}
	}
	return descuento;
}
Proveedor Proveedor::clone()
{
	Proveedor p = Proveedor();
	p.index = this->index;
	p.Descuentos = &(Descuento::DescuentosPorProveedor.at(this->index));
	p.total_solicitados = this->total_solicitados;
	p.total_pack_solicitados = this->total_pack_solicitados;
	return p;
}
Proveedor::~Proveedor()
{
}
