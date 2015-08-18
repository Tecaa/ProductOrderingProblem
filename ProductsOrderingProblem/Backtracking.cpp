#include "stdafx.h"
#include "Backtracking.h"
#include <iostream>
int iter = 0;
Backtracking::Backtracking(Solucion sol)
{
	this->mejorSolucion = sol;
	doBactracking(sol, 0);

	cout << "\n iteraciones: " << iter << "\n";
}

void Backtracking::doBactracking(Solucion sol, int j, int i)
{
	if (i > sol.productos[j].cant_mensual - sol.productos[j].stock_actual)
		return;
	

	for (int k = 0; k < sol.productos.size(); k++)
	{
		sol.proveedores[sol.productos.at(k).index_proveedor].total_solicitados += sol.productos.at(k).SetCant_solicitar(i);
		sol.CalcularValoresInternos();

		if (sol.GetTotalCostos() < sol.Presupuesto() && sol.GetFOValue() > this->mejorSolucion.GetFOValue())
			this->mejorSolucion = sol;

		/*	IMPRESION DE VALORES POR PANTALLA */

		if (iter % 1 == 0) // % 100 para muestra
		{
			for (int m = 0; m < sol.productos.size(); m++)
				cout << sol.productos.at(m).GetCant_solicitar() << " ";
			cout << "  i: " << i << "  iter: " << iter << " costo " << sol.GetTotalCostos() << " FO actual: " << sol.GetFOValue() << "    FO best: " << this->mejorSolucion.GetFOValue() << "\n";
			cout << sol.proveedores[0].total_solicitados << " " << sol.proveedores[1].total_solicitados << " " << sol.proveedores[2].total_solicitados << "\n";
		}
		iter++;
		doBactracking(sol.clone(), k, i+1);
	}
}

void Backtracking::doBactracking(Solucion sol, int i)
{
	/*
	if (i >= sol.productos.size())
	{
		return;
	}*/
	if (i < sol.productos.size())
	{
		for (int k = 0; k <= sol.productos.at(i).cant_mensual - sol.productos.at(i).stock_actual; k++)
		{
			if (k != sol.productos.at(i).GetCant_solicitar())
			{
				sol.proveedores[sol.productos.at(i).index_proveedor].total_solicitados += sol.productos.at(i).SetCant_solicitar(k);
				sol.CalcularValoresInternos();

				if (sol.GetTotalCostos() < sol.Presupuesto() && sol.GetFOValue() > this->mejorSolucion.GetFOValue())
					this->mejorSolucion = sol;

				/*	IMPRESION DE VALORES POR PANTALLA */
				
				if (iter % 1 == 0) // % 100 para muestra
				{
					for (int m = 0; m < sol.productos.size(); m++)
						cout << sol.productos.at(m).GetCant_solicitar() << " ";
					cout << "|";
					for (int m = 0; m < sol.packs.size(); m++)
						cout << sol.packs.at(m).GetCant_solicitar() << " ";
					cout << "  i: " << i << "  iter: " << iter << " costo " << sol.GetTotalCostos() << " FO actual: " << sol.GetFOValue() << "    FO best: " << this->mejorSolucion.GetFOValue() << "\n";
				}
				++iter;
			}
			doBactracking(sol.clone(), i + 1);

		}
	}
	else if (i >= sol.productos.size() && i < sol.productos.size() + sol.packs.size())
	{
		int i2 = i - sol.productos.size();
		for (int k = 0; k <= sol.packs.at(i2).cant_mensual - sol.packs.at(i2).stock_actual; k++)
		{
			if (k != sol.packs.at(i2).GetCant_solicitar())
			{
				int diferencia_solicitar = sol.packs.at(i2).SetCant_solicitar(k);
				for (int m = 0; m < sol.packs.at(i2).productos.size(); ++m)
				{
					sol.proveedores[sol.packs.at(i2).productos.at(m)->index_proveedor].total_pack_solicitados += diferencia_solicitar;
				}
				
				sol.CalcularValoresInternos();

				if (sol.GetTotalCostos() < sol.Presupuesto() && sol.GetFOValue() > this->mejorSolucion.GetFOValue())
					this->mejorSolucion = sol;

				/*	IMPRESION DE VALORES POR PANTALLA */
				
				if (iter % 1 == 0) // % 100 para muestra
				{
				for (int m = 0; m < sol.productos.size(); m++)
					cout << sol.productos.at(m).GetCant_solicitar() << " ";
				cout << "|";
				for (int m = 0; m < sol.packs.size(); m++)
					cout << sol.packs.at(m).GetCant_solicitar() << " ";
				cout << "  i: " << i << "  iter: " << iter << " costo " << sol.GetTotalCostos() << " FO actual: " << sol.GetFOValue() << "    FO best: " << this->mejorSolucion.GetFOValue() << "\n";
				}
				++iter;
			}
			doBactracking(sol.clone(), i + 1);

		}
	}
	else
		return;
}

Backtracking::~Backtracking()
{
}
