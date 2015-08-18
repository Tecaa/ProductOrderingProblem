#pragma once
#include "Product.h"
#include <string>
#include <vector>
#include "Pack.h"

using namespace std;

class CsvReader
{
private:
	static void split(const string& s, char c, vector<string>& v);
public:
	CsvReader();
	static void ReadCsvProductos(string path, vector<Product> &products, vector<Proveedor> proveedores, int &presupuesto);
	static void ReadCsvDescuentos(string path, vector<Proveedor> &proveedores);
	static void ReadCsvPacks(string path, vector<Pack> &packs, vector<Proveedor> &proveedores, vector<Product> &productos);
	~CsvReader();
};

