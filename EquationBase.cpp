#include "stdafx.h"
#include "EquationBase.h"

EquationBase::EquationBase(double _tfirst, double _tlast)
{
	step = 1.0;
	eps = 0.0001;

	t_first = _tfirst;
	t_last = _tlast;

	machEps = CalcMachEps();
}


EquationBase::~EquationBase()
{
	
}

void EquationBase::setEps(double _eps)
{
	eps = _eps;
}

//вычислеие машинного эпсилона
double EquationBase::CalcMachEps()
{
	double res;
	int N = 1;

	while (1)
	{
		res = pow(2., -N);
		if (res + 1. == 1.)
		{
			N--;
			res = pow(2., -N);
			return res;
		}
		else
			N++;
	}
}