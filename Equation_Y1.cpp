#include "stdafx.h"
#include "Equation_Y1.h"


Equation_Y1::Equation_Y1(double &XP_0, double &X1_0, double &YP_0, double &Y1_0, double &L,
	double &L_0, double &nu, double &mu, double &mu_0,double &p, double &q, double &f_t, double &_tfirst, double &_tlast)
	: EquationBase(_tfirst, _tlast)
{
	XP.push_back(XP_0);
	X1.push_back(X1_0);
	YP.push_back(YP_0);
	Y1.push_back(Y1_0);

	lambda = L;
	lambda_0 = L_0;
	Nu = nu;
	Mu = mu;
	Mu_0 = mu_0;
	P = p;
	Q = q;
	F_trait = f_t;
}


Equation_Y1::~Equation_Y1()
{
	XP.~vector();
	X1.~vector();
	YP.~vector();
	Y1.~vector();
}

short int Equation_Y1::sign(double t)
{
	short int res = t > 0 ? 1 : t < 0 ? -1 : 0;
	return res;
}