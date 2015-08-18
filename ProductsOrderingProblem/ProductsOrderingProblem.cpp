// ProductsOrderingProblem.cpp: define el punto de entrada de la aplicación de consola.
//

#include "stdafx.h"
#include "CsvReader.h"
#include "Product.h"
#include "Solucion.h"
#include <vector>
#include <iostream>
#include "Backtracking.h"
#include "AntColonySystem.h"
#include <ctime>

using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{
	
	//srand(time(NULL));	//DON'T FORGET UNCOMMENT

	vector<Product> products;
	vector<Proveedor> proveedores;
	vector<Pack> packs;
	int presupuesto;
	const string filename = "3-2-1";

	CsvReader::ReadCsvDescuentos("../" + filename + "d.csv", proveedores);
	CsvReader::ReadCsvProductos("../" + filename + "p.csv", products, proveedores, presupuesto);
	CsvReader::ReadCsvPacks("../" + filename + "c.csv", packs, proveedores, products);

	Solucion sol = Solucion(products, proveedores, presupuesto, packs);
	clock_t begin = clock();
	Backtracking bt = Backtracking(sol);
	
	cout << "\nBacktracking: ";
	for (int m = 0; m<bt.mejorSolucion.productos.size(); m++)
		cout << bt.mejorSolucion.productos.at(m).GetCant_solicitar() << " ";
	cout << "|";
	for (int m = 0; m<bt.mejorSolucion.packs.size(); m++)
		cout << bt.mejorSolucion.packs.at(m).GetCant_solicitar() << " ";
	cout << "  FO best: " << bt.mejorSolucion.GetFOValue() << " costo " << bt.mejorSolucion.GetTotalCostos() << "\n";

	clock_t end = clock();
	double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
	cout << "\nelapsed seconds: " << elapsed_secs;
	




	begin = clock();

	//sol = Solucion(products, proveedores, presupuesto);
	AntColonySystem acs = AntColonySystem(&sol, presupuesto, 0.55f, 1, 2, 0.1, 10, 100, 0.01f, 15);
	cout << "\nAnt Colony System: ";
	for (int m = 0; m<acs.mejor_solucion.productos.size(); m++)
		cout << acs.mejor_solucion.productos.at(m).GetCant_solicitar() << " ";
	cout << "|";
	for (int m = 0; m<acs.mejor_solucion.packs.size(); m++)
		cout << acs.mejor_solucion.packs.at(m).GetCant_solicitar() << " ";
	cout << "  FO best: " << acs.mejor_solucion.GetFOValue() << " costo " << acs.mejor_solucion.GetTotalCostos() << "\n";
	cout << "   FO best: " << acs.mejor_solucion.GetFOValue() << " costo " << acs.mejor_solucion.GetTotalCostos() << "\n";

	end = clock();
	elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
	cout << "\nelapsed seconds: " << elapsed_secs;

		
	getchar();
	return 0;
}

