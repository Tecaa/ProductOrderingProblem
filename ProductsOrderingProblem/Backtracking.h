#pragma once
#include "Solucion.h"

class Backtracking
{
public:
	Backtracking(Solucion sol);
	~Backtracking();
	Solucion mejorSolucion;
private:
	void doBactracking(Solucion solucion, int i);
	void doBactracking(Solucion sol, int j, int i);
};

