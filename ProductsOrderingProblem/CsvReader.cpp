#include "stdafx.h"
#include "CsvReader.h"
#include <fstream>
#include <string>
#include <vector>
#include <string>
#include <sstream>
#include <iterator>
#include "Product.h"
#include "Pack.h"
#include <map>
#include "Descuento.h"

//map<int, vector<Descuento>> Descuento::DescuentosPorProveedor;
vector<vector<Descuento>> Descuento::DescuentosPorProveedor;
using namespace std;

CsvReader::CsvReader()
{
}

void CsvReader::ReadCsvProductos(string path, vector<Product> &products, vector<Proveedor> proveedores, int &presupuesto)
{
	ifstream file(path);
	string line;
	getline(file, line); //First line budget
	presupuesto = stoi(line);
	getline(file, line); //Second line skipped
	while (getline(file, line))
	{
		
		vector<string> elems;
		stringstream ss(line);
		char delim = ';';
		string item;
		while (getline(ss, item, delim)) {
			elems.push_back(item);
		}

		products.push_back(Product(products.size(), stoi(elems[0]), stoi(elems[1]), stoi(elems[2]), stoi(elems[3]), stoi(elems[4]), stoi(elems[5])));
	}
}

void CsvReader::ReadCsvDescuentos(string path, vector<Proveedor> &proveedores)
{
	ifstream file(path);
	string line;
	getline(file, line); //First line skipped
	int lastProvider = -1;
	int unidades_min = 0;
	while (getline(file, line))
	{
		vector<string> elems;
		stringstream ss(line);
		char delim = ';';
		string item;
		while (getline(ss, item, delim)) {
			elems.push_back(item);
		}
		if (lastProvider != stoi(elems[0]))
		{
			proveedores.push_back(Proveedor(proveedores.size(), stoi(elems[0])));
			lastProvider = stoi(elems[0]);
			//Nuevo proveedor
			vector<Descuento> vacio;
			Descuento::DescuentosPorProveedor.push_back(vacio);
		}
		int descuento_index = Descuento::DescuentosPorProveedor.at(proveedores.size() - 1).size();
		Descuento::DescuentosPorProveedor.at(proveedores.size()-1).push_back(Descuento(unidades_min, stoi(elems[1]), stof(elems[2]), descuento_index));
		//TODO estan buenas?????
		if (stoi(elems[1]) == 0)
			unidades_min = 0; 
		else
			unidades_min = stoi(elems[1]) + 1;
	}
	for (int i = 0; i < Descuento::DescuentosPorProveedor.size(); i++)
	{
		proveedores.at(i).Descuentos = &(Descuento::DescuentosPorProveedor.at(i));
	}
}

void CsvReader::ReadCsvPacks(string path, vector<Pack> &packs, vector<Proveedor> &proveedores, vector<Product> &productos)
{
	ifstream file(path);
	string line;
	getline(file, line); //First line skipped
	while (getline(file, line))
	{
		vector<string> elems, prods;
		stringstream ss(line);
		char delim = ';';
		string item;
		while (getline(ss, item, delim)) {
			elems.push_back(item);
		}
		Pack pk = Pack(packs.size(), packs.size(), stoi(elems[0]), stoi(elems[1]), stoi(elems[2]));
		delim = ',';
		split(elems[3], delim, prods);
		for (int i = 0; i < prods.size(); ++i)
		{
			Product* pp = NULL;
			for (int j = 0; j < productos.size(); ++j)
			{
				if (productos[j].id == stoi(prods[i]))
					pp = &productos[j];
			}
			pk.AddProduct(pp);
		}

		packs.push_back(pk);
	}
}
CsvReader::~CsvReader()
{
}
void CsvReader::split(const string& s, char c, vector<string>& v) {
	string::size_type i = 0;
	string::size_type j = s.find(c);

	while (j != string::npos) {
		v.push_back(s.substr(i, j - i));
		i = ++j;
		j = s.find(c, j);

		if (j == string::npos)
			v.push_back(s.substr(i, s.length()));
	}
}