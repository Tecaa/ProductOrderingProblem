#include "stdafx.h"
#include "AntColonySystem.h"
#include "MultiplePath.h"
#include "ProductPath.h"
#include <algorithm>    // std::min
#include <tuple>
#include <iostream>
using namespace std;

AntColonySystem::AntColonySystem(Solucion* sol, int presupuesto, float q0, float a, float b, float p, int m, int Q, float T0, int cl)
{
	this->nueva = sol->clone();
	this->solucion = sol;
	this->mejor_solucion = sol->clone();
	for (int p = 0; p < Descuento::DescuentosPorProveedor.size(); ++p)
	{
		//Asumimos primer tipo de descuento para todos los proveedores en solucion vacia
		this->dbp_mejorSolucion.push_back(Descuento::DescuentosPorProveedor.at(p).at(0));
	}
	this->q0 = q0;
	this->m = m;
	this->a = a;
	this->b = b;
	this->p = p;
	this->T0 = T0;
	this->presupuesto = presupuesto;
	this->cl = cl;
	this->Q = Q;
	build_paths();
	//fill_candidates();
	doACS();
}

void AntColonySystem::doACS()
{
	for (int y = 1; y <= this->Q; ++y)
	{
		//if (y == 5)
		//	return;
		//cout << "\nIntento " << y;
		for (int j = 1; j <= this->m; ++j)
		{
			//cout << "\n\tHormiga " << j;
			*this->solucion = nueva.clone();
			vector<bool> firstByProvider (this->solucion->proveedores.size(), true);
			vector<Descuento> discountByProvider (this->solucion->proveedores.size());
			for (int i = 0; i < this->solucion->productos.size(); ++i)
			{

			//	cout << "\n\t\tProducto " << i << " Pr " << this->solucion->productos.at(i).index_proveedor <<"\t";
				int index_provider = this->solucion->productos.at(i).index_proveedor;
				int quantity_chosen = -1;

				if (firstByProvider.at(index_provider))
				{
					Descuento d;
					quantity_chosen = choose_path(this->solucion->productos.at(i), &d);
					discountByProvider.at(index_provider) = d;
					firstByProvider.at(index_provider) = false;
			//		cout << "desc " << d.percentage ;
				}
				else
					quantity_chosen = choose_path(this->solucion->productos.at(i), discountByProvider.at(index_provider));
			//	cout << " Cantidad " << quantity_chosen;
				this->solucion->proveedores[this->solucion->productos.at(i).index_proveedor].total_solicitados
					+= this->solucion->productos.at(i).SetCant_solicitar(quantity_chosen);
				//Si bien se trabaja sobre infactibles momentaneamente se asume que al completar la solucion seran factibles.
				//(Se asume descuentos que no se pueden aplicar a cantidades actuales pero que se esperan alcanzar a futuro)
				this->solucion->CalcularValoresInternosFixed(discountByProvider);
				float t1 = this->solucion->Presupuesto();
				float t2 = this->solucion->GetTotalCostosFixed();
				if (t1 < t2)
					int exedimos = 1;
				if (quantity_chosen == -1)
					int menosuno = 2;
				this->updatePheromone(this->solucion->productos.at(i), discountByProvider.at(index_provider), quantity_chosen);
			}
			//Si es factiblemente mejor se guarda
			//TODO: Quizas se pueda eliminar la parte de presupuesto controlando que se cumpla previamente.
			if (this->solucion->GetFOValue() > this->mejor_solucion.GetFOValue() && this->solucion->GetTotalCostos() <= this->solucion->Presupuesto())
			{
				this->mejor_solucion = this->solucion->clone();
				this->dbp_mejorSolucion = discountByProvider;
			}
		}
		//reforzar mejor solucion
		for (int i = 0; i < this->mejor_solucion.productos.size(); ++i)
		{
			this->updatePheromone(this->mejor_solucion.productos.at(i),
				this->dbp_mejorSolucion.at(this->mejor_solucion.productos.at(i).index_proveedor),
				this->mejor_solucion.productos.at(i).GetCant_solicitar());
		}
	}
}
void AntColonySystem::build_paths()
{
	
	for (int i = 0; i < this->solucion->proveedores.size(); i++)
	{
		vector<ProductPath> proveedorPath;


		caminos.push_back(proveedorPath);


		//ants.push_back(Ant());
	}

	for (int j = 0; j < this->solucion->productos.size(); j++)
	{
		ProductPath pp = ProductPath(j, this->solucion->productos.at(j).cant_mensual, this->solucion->productos.at(j).index_proveedor, this->T0);
		int indexInProviders = caminos.at(this->solucion->productos.at(j).index_proveedor).size();
		this->indexesInProviders.push_back(indexInProviders);
		caminos.at(this->solucion->productos.at(j).index_proveedor).push_back(pp);
	}
}
int AntColonySystem::choose_path(Product final_product, Descuento * discount)
{
	int max_value = 0, q_max = 0;
	int value = -1;
	float r = ((double)rand() / (RAND_MAX));
	if (r <= q0)
	{
		//cout << "ami ";
		return arg_max_initial(final_product, discount);
	}
	else
	{
		//cout << "pi ";
		return prob_initial(final_product, discount);
		
	}
}
int AntColonySystem::choose_path(Product final_product, Descuento discount)
{
	if (hay_candidatos(final_product, discount))
	{
		if (((double)rand() / (RAND_MAX)) <= q0)
		{
			//cout << "am ";
			return arg_max(final_product, discount);
		}
		else
		{
		//	cout << "p ";
			return prob(final_product, discount);
		}
	}
	else
	{
		//cout << "mg ";
		return mayor_ganancia(final_product, discount);
	}

}
bool AntColonySystem::hay_candidatos(Product final_product, Descuento discount)
{
	float argMax = -1;
	int quantity = -1;
	int proveedor_solicitar = this->solucion->proveedores[final_product.index_proveedor].total_solicitados;

	//Comprobar desde que elecciones de stock se puede continuar a futuro alcanzando el mínimo
	// Por ejemplo si quedan 3 productos con stock máximo (5, 3, 2) y tenemos que alcanzar el mínimo de 10 
	// y llevamos 2 necesitamos alcanzar 8 productos, 
	// en el primer producto se bloquean casos 0, 1, 2 solo
	// pudiendo pedir 4 y 5. En el segundo y tercer producto se bloquean el caso 0
	int min_cant_pedir = comprobar_minimo(discount, final_product);

	int candidatos = 0;
	for (int q = final_product.cant_mensual; q >= 0; --q)
	{
		// if we have exceeded current budget or we are requesting more products than discount.unidades_max
		// or we need certain products to reach the minimun
		int costoFixed = this->solucion->GetTotalCostosFixed();
		int nuevoCosto = costoFixed + (final_product.valor_compra * discount.percentage) * q;
		if (this->presupuesto < nuevoCosto
			|| (discount.unidades_max != 0 && discount.unidades_max - proveedor_solicitar < q)
			|| min_cant_pedir > q)
		{
			++candidatos;
			if (candidatos >= this->cl)
				return false;
			continue;
		}
		return true;
	}
	return false;
}
//Quizas current_budget no es necesario y basta con this->solucion->GetTotalCostos();
//int current_budget = 0;
int AntColonySystem::arg_max(Product final_product, Descuento discount)
{
	float argMax = -1;
	int quantity = -1;
	int proveedor_solicitar = this->solucion->proveedores[final_product.index_proveedor].total_solicitados;

	//Comprobar desde que elecciones de stock se puede continuar a futuro alcanzando el mínimo
	// Por ejemplo si quedan 3 productos con stock máximo (5, 3, 2) y tenemos que alcanzar el mínimo de 10 
	// y llevamos 2 necesitamos alcanzar 8 productos, 
	// en el primer producto se bloquean casos 0, 1, 2 solo
	// pudiendo pedir 4 y 5. En el segundo y tercer producto se bloquean el caso 0
	int min_cant_pedir = comprobar_minimo(discount, final_product);

	//Mayor valor de ganancia
	float max_gain = (final_product.valor_venta - final_product.valor_compra * discount.percentage) * (final_product.cant_mensual);

	int candidatos = 0;
	for (int q = final_product.cant_mensual; q >= 0; --q, ++candidatos)
	{
		// if we have exceeded current budget or we are requesting more products than discount.unidades_max
		// or we need certain products to reach the minimun
		int costoF = this->solucion->GetTotalCostosFixed();
		int costoNuevo = costoF + (final_product.valor_compra * discount.percentage) * q;
		if (this->presupuesto <  costoNuevo
			|| (discount.unidades_max != 0 && discount.unidades_max - proveedor_solicitar < q)
			|| min_cant_pedir > q
			|| candidatos >= this->cl)
			continue;
		float f = this->getPheromone(final_product.index_proveedor, final_product.index, q, discount);

		float n = ((final_product.valor_venta - final_product.valor_compra * discount.percentage) * q) / max_gain;
		//float n = 1/((final_product.valor_venta - final_product.valor_compra * discount.percentage) * (q+1));
		float temp = pow(f, this->a) * pow(n, this->b);
		if (temp > argMax)
		{
			argMax = temp;
			quantity = q;
		}
	}
	return quantity;
}
//////
// Buscar en los 3 tipos de descuento cl candidatos y comparar entre ellos
// las ganancias para ordenar de mayor a menor y elegir el mejor(considerando feromona)
int AntColonySystem::arg_max_initial(Product final_product, Descuento* discount)
{
	float argMax = -1;
	int quantity_max = -1;
	int discount_max = -1;

	vector<tuple<int, int, float>> caminos_por_descuento; //pair (index_product, index_discount, value)
	vector<Descuento> descuentos = Descuento::DescuentosPorProveedor.at(final_product.index_proveedor);

	//Mayor valor de ganancia
	float max_gain = (final_product.valor_venta - final_product.valor_compra * descuentos.at(descuentos.size()-1).percentage) * (final_product.cant_mensual);

	for (int d = 0; d < descuentos.size(); ++d)
	{
		int candidatos = 0;
		for (int q = final_product.cant_mensual; q >= 0; --q, ++candidatos)
		{
			float temp;
			// if we have exceeded current budget or we are requesting more products than discount.unidades_max
			// or we need certain products to reach the minimun
			float costoNuevo = this->solucion->GetTotalCostosFixed() + (final_product.valor_compra * descuentos.at(d).percentage) * q;
			if (this->presupuesto < costoNuevo || candidatos >= this->cl)
			{
				temp = 0;

			}
			else
			{
				float f = this->getPheromone(final_product.index_proveedor, final_product.index, q, descuentos.at(d));
				
				float n = ((final_product.valor_venta - final_product.valor_compra * descuentos.at(d).percentage) * q)/max_gain;
				temp = pow(f, this->a) * pow(n, this->b);
				if (temp > argMax)
				{
					argMax = temp;
					
					quantity_max = q;
					discount_max = d;
				}
			}
			//caminos_por_descuento.push_back(tuple<int, int, int>(q, d, temp));

		}
	}
	*discount = descuentos.at(discount_max);
	return quantity_max;
}
int sort_function(const void *one, const void *two)
{
	tuple<int, int, int> a = *((tuple<int, int, int>*)one);
	tuple<int, int, int> b = *((tuple<int, int, int>*)two);
	
	if (get<2>(a) < get<2>(b))
		return -1;
	if (get<2>(a) == get<2>(b))
		return 0;
	return 1;

}
int AntColonySystem::prob(Product final_product, Descuento discount)
{
	vector<float> values;
	float total = 0;
	int proveedor_solicitar = this->solucion->proveedores[final_product.index_proveedor].total_solicitados;
	
	// Comprobar desde que elecciones de stock se puede continuar a futuro alcanzando el mínimo
	// Por ejemplo si quedan 3 productos con stock máximo (5, 3, 2) y tenemos que alcanzar el mínimo de 10 
	// y llevamos 2 necesitamos alcanzar 8 productos, 
	// en el primer producto se bloquean casos 0, 1, 2 solo
	// pudiendo pedir 4 y 5. En el segundo y tercer producto se bloquean el caso 0
	int min_cant_pedir = comprobar_minimo(discount, final_product);
	
	//Mayor valor de ganancia
	float max_gain = (final_product.valor_venta - final_product.valor_compra * discount.percentage) * (final_product.cant_mensual);


	int candidatos = 0;
	for (int q = final_product.cant_mensual; q >= 0; --q)
	{
		// if we haven't exceed current budget and discount.unidades_max
		float temp = 0;
		int costoF = this->solucion->GetTotalCostosFixed();
		int costoNuevo = costoF + (final_product.valor_compra * discount.percentage) * q;
		if (this->presupuesto >=  costoNuevo
			&& (discount.unidades_max == 0 || discount.unidades_max - proveedor_solicitar >= q)
			&& q >= min_cant_pedir
			&& candidatos < this->cl)
		{
			//De esta manera el llevar 0 unidades también tiene una probabilidad de ser elegida aunque baja.
			float q2 = q;
			if (q == 0)
				q2 = final_product.cant_mensual;

			float f = this->getPheromone(final_product.index_proveedor, final_product.index, q, discount);
			float n = ((final_product.valor_venta - final_product.valor_compra * discount.percentage) * q2) / max_gain;
			//if (q == 0)
			//	n = 1;
			temp = pow(f, this->a) * pow(n, this->b);
			total += temp;
			++candidatos;
		}
		values.insert(values.begin(), temp);
	}
	float random = ((double)rand() / (RAND_MAX));
	float tarta = 0;
	for (int q = 0; q < values.size(); ++q)
	{
		tarta += values.at(q)/total;
		if (tarta >= random)
			return q;
	}
	return -1;
}
//////
// Buscar en los 3 tipos de descuento cl candidatos y comparar entre ellos
// las ganancias para ordenar de mayor a menor y quedarse con los cl mejores para hacer ahi la torta de probabilidad
int AntColonySystem::prob_initial(Product final_product, Descuento* discount)
{
	float total = 0;
	vector<tuple<int, int, float>> caminos_por_descuento; //pair (index_product, index_discount)
	vector<Descuento> descuentos = Descuento::DescuentosPorProveedor.at(final_product.index_proveedor);
	
	//Mayor valor de ganancia
	float max_gain = (final_product.valor_venta - final_product.valor_compra * descuentos.at(descuentos.size()-1).percentage) * (final_product.cant_mensual);

	for (int d = 0; d < descuentos.size(); ++d)
	{
		int candidatos = 0;
		for (int q = final_product.cant_mensual; q >= 0; --q)
		{
			float temp;
			// if we have exceeded current budget or we are requesting more products than discount.unidades_max
			// or we need certain products to reach the minimun
			if (this->presupuesto < this->solucion->GetTotalCostosFixed() + (final_product.valor_compra * descuentos.at(d).percentage) * q
				|| candidatos >= this->cl)
			{
				temp = 0;

			}
			else
			{
				//De esta manera el llevar 0 unidades también tiene una probabilidad de ser elegida aunque baja.
				float q2 = q;
				if (q == 0)
					q2 = final_product.cant_mensual;

				float f = this->getPheromone(final_product.index_proveedor, final_product.index, q, descuentos.at(d));
				float n = ((final_product.valor_venta - final_product.valor_compra * descuentos.at(d).percentage) * q2) / max_gain;
				
				temp = pow(f, this->a) * pow(n, this->b);
				total += temp; 
				++candidatos;
			}
			caminos_por_descuento.insert(caminos_por_descuento.begin(), tuple<int, int, float>(q, d, temp));
		}
	}
	float random = ((double)rand() / (RAND_MAX));
	float tarta = 0;
	for (int i = 0; i < caminos_por_descuento.size(); ++i)
	{
		tarta += get<2>(caminos_por_descuento.at(i)) / total;
		if (tarta >= random)
		{
			*discount = descuentos.at(get<1>(caminos_por_descuento.at(i)));
			return get<0>(caminos_por_descuento.at(i));
		}
	}

	discount = NULL;
	return -1;
}

int AntColonySystem::mayor_ganancia(Product final_product, Descuento discount)
{
	int proveedor_solicitados = this->solucion->proveedores[final_product.index_proveedor].total_solicitados;

	//Comprobar desde que elecciones de stock se puede continuar a futuro alcanzando el mínimo
	// Por ejemplo si quedan 3 productos con stock máximo (5, 3, 2) y tenemos que alcanzar el mínimo de 10 
	// y llevamos 2 necesitamos alcanzar 8 productos, 
	// en el primer producto se bloquean casos 0, 1, 2 solo
	// pudiendo pedir 4 y 5. En el segundo y tercer producto se bloquean el caso 0
	int min_cant_pedir = comprobar_minimo(discount, final_product);
	int max_pedir_tentativo;
	if (discount.unidades_max == 0)
		max_pedir_tentativo = final_product.cant_mensual;
	else
		max_pedir_tentativo =  min(discount.unidades_max - proveedor_solicitados, final_product.cant_mensual);
	for (int q = max_pedir_tentativo; q >= 0; --q)
	{
		// if we have exceeded current budget or we are requesting more products than discount.unidades_max
		// or we need certain products to reach the minimun
		int costoFixed = this->solucion->GetTotalCostosFixed();
		int nuevoCosto = costoFixed + (final_product.valor_compra * discount.percentage) * q;
		if (this->presupuesto < nuevoCosto
			|| min_cant_pedir > q)
			continue;
		return q;
	}
	return -1;
}
int AntColonySystem::comprobar_minimo(Descuento discount, Product final_product){
	int proveedor_solicitar = this->solucion->proveedores[final_product.index_proveedor].total_solicitados;
	int faltantes_min = discount.unidades_min - proveedor_solicitar;
	int min_cant_pedir = 0;
	if (faltantes_min <= 0)
	{
		int suma_maxima = 0;
		for (int i = final_product.index; i < this->solucion->productos.size(); i++)
		{
			suma_maxima += this->solucion->productos.at(i).GetCant_solicitar();
		}
		if (faltantes_min > suma_maxima)
			// No se podrá alcanzar el mínimo si ahora no se bloquean casos
		{
			min_cant_pedir = faltantes_min - suma_maxima;
		}
	}
	return min_cant_pedir;
}
void AntColonySystem::updatePheromone(Product p, Descuento d, int p_q)
{
	this->setPheromone(p.index_proveedor, p.index, p_q, d, (1 - this->p)*this->getPheromone(p.index_proveedor, p.index, p_q, d) + this->p * this->T0);
}
void AntColonySystem::setPheromone(int index_provider, int index_product, int quantity, Descuento discount, float pheromone)
{
	int index = this->indexesInProviders.at(index_product);
	this->caminos.at(index_provider).at(index).ProductByDiscountPaths.at(discount.index).paths.at(quantity).feromona = pheromone;
}
float AntColonySystem::getPheromone(int index_provider, int index_product, int quantity, Descuento discount)
{
	int index = this->indexesInProviders.at(index_product);
	return this->caminos.at(index_provider).at(index).ProductByDiscountPaths.at(discount.index).paths.at(quantity).feromona;
}
AntColonySystem::~AntColonySystem()
{
}
