#pragma once
#include "Solucion.h"
#include "Ant.h"
#include "ProductPath.h"

class AntColonySystem
{
private:
	void build_paths();
	void doACS();
	void updatePheromone(Product p, Descuento d, int product_quantity);
	vector<Ant> ants;

	//Array that match de index of products in products lists to the providers lists.
	vector<int> indexesInProviders;

	//candidates list isn't necessary
	int choose_path();
	float q0, T0;
	int m, cl, Q, presupuesto;
	float a;
	float b;
	//Evaporation param
	float p;
	vector<Descuento> dbp_mejorSolucion;
	vector<vector<ProductPath>> caminos;
	bool hay_candidatos(Product final_product, Descuento discount);
	int choose_path(Product initial_product, Descuento* discount);
	int choose_path(Product initial_product, Descuento discount);
	int arg_max(Product final_product, Descuento discount);
	int arg_max_initial(Product final_product, Descuento* discount);
	int prob(Product initial_product, Descuento discount);
	int prob_initial(Product final_product, Descuento* discount);
	int mayor_ganancia(Product final_product, Descuento discount);
	void setPheromone(int index_provider, int index_product, int quantity, Descuento discount, float pheromone);
	float getPheromone(int index_provider, int index_product, int quantity, Descuento discount);
	int comprobar_minimo(Descuento discount, Product final_product);
	Solucion nueva;
public:
	AntColonySystem(Solucion* sol, int presupuesto, float q0, float a, float b, float p, int m, int Q, float T0, int cl);
	Solucion* solucion;
	Solucion mejor_solucion;
	~AntColonySystem();
};