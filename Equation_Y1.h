#ifndef _EQUATION_Y1_H_
#define _EQUATION_Y1_H_


#include <cmath>
#include "EquationBase.h"

class Equation_Y1 : public EquationBase
{
private:
	//число Пи
	const double PI = 3.141592;

	//параметры, вводимые пользователем
	double lambda, lambda_0, Nu, Mu, Mu_0, P, Q, F_trait;

	//контрольный член Егорова
	double E;

	//возвращает -1 или 0 или 1
	short int sign(double t);

public:
	//вектора значений ДУ
	std::vector<double> Y1, YP, X1, XP;

	//конструтор
	Equation_Y1(double &XP_0, double &X1_0, double &YP_0, double &Y1_0, double &L, 
				double &L_0, double &nu, double &mu, double &mu_0,double &p, double &q, 
				double &f_t, double &_tfirst, double &_tlast); 
	//деструктор
	~Equation_Y1();

	/*
	вычисление значений ДУ (решение задачи Коши) c помощью метода Ингленда 4-го порядка
	(модификация Рунге-Кутта)
	*/
	virtual void Calculation() override
	{
		double k[6], l[6], m[6], n[6];

		int i = 0;

		double t = EquationBase::t_first;

		while (t <= EquationBase::t_last)
		{
			bool CorrectStep = true;

			do
			{
				//вычисление дополнительных параметров
				k[0] = EquationBase::step * YP[i];
				l[0] = EquationBase::step * Y1[i];
				m[0] = EquationBase::step * (-lambda_0 * (XP[i] - X1[i] - Nu) - P - Mu * cos(t * PI / 180));
				n[0] = EquationBase::step * (-(lambda * lambda) * (X1[i] - XP[i] + Nu) - Mu_0 * F_trait * sign(Y1[i]) - P + Q);

				k[1] = EquationBase::step * (YP[i] + 1. / 2. * m[0]);
				l[1] = EquationBase::step * (Y1[i] + 1. / 2. * n[0]);
				m[1] = EquationBase::step * (-(lambda_0) * ((XP[i] + 1. / 2. * k[0]) - (X1[i] + 1. / 2. * l[0]) - Nu)
					- P - Mu * cos((t + 1. / 2. * EquationBase::step) * PI / 180.));
				n[1] = EquationBase::step * (-(lambda * lambda) * ((X1[i] + 1. / 2. * l[0]) - (XP[i] + 1. / 2. * k[0]) + Nu) - Mu_0 *
					F_trait * sign(Y1[i] + 1. / 2. * n[0]) - P + Q);

				k[2] = EquationBase::step * (YP[i] + 1. / 4. * (m[0] + m[1]));
				l[2] = EquationBase::step * (Y1[i] + 1. / 4. * (n[0] + n[1]));
				m[2] = EquationBase::step * (-(lambda_0) * ((XP[i] + 1. / 4. * (k[0] + k[1])) - (X1[i] + 1. / 4. * (l[0] + l[1])) - Nu)
					- P - Mu * cos((t + 1. / 2. * EquationBase::step) * PI / 180.));
				n[2] = EquationBase::step * (-(lambda * lambda) * ((X1[i] + 1. / 4. * (l[0] + l[1])) - 
					(XP[i] + 1. / 4. * (k[0] + k[1])) + Nu) - Mu_0 * F_trait * sign(Y1[i] + 1. / 4. * (n[0] + n[1])) - P + Q);

				k[3] = EquationBase::step * (YP[i] - m[1] + 2 * m[2]);
				l[3] = EquationBase::step * (Y1[i] - n[0] + 2 * n[2]);
				m[3] = EquationBase::step * (-(lambda_0) * ((XP[i] - k[1] + 2 * k[2]) - (X1[i] - l[1] + 2 * l[2]) - Nu)
					- P - Mu * cos((t + EquationBase::step) * PI / 180));
				n[3] = EquationBase::step * (-(lambda * lambda) * ((X1[i] - l[1] + 2 * l[2]) - (XP[i] - k[1] + 2 * k[2]) + Nu)
					- Mu_0 * F_trait * sign(Y1[i] - n[1] + 2 * n[2]) - P + Q);

				k[4] = EquationBase::step * (YP[i] + 1. / 27. * (7 * m[0] + 10 * m[1] + m[3]));
				l[4] = EquationBase::step * (Y1[i] + 1. / 27. * (7 * n[0] + 10 * n[1] + n[3]));
				m[4] = EquationBase::step * (-(lambda_0) * ((XP[i] + 1. / 27. * (7 * k[0] + 10 * k[1] + k[3])) -
					(X1[i] + 1. / 27. * (7 * l[0] + 10 * l[1] + l[3])) - Nu) - P - Mu * cos((t + 2. / 3. * EquationBase::step) * PI / 180));
				n[4] = EquationBase::step * (-(lambda * lambda) * ((X1[i] + 1. / 27. * (7 * l[0] + 10 * l[1] + l[3]))
					- (XP[i] + 1. / 27. * (7 * k[0] + 10 * k[1] + k[3])) + Nu) - Mu_0 * F_trait *
					sign(Y1[i] + 1. / 27. * (7 * n[0] + 10 * n[1] + n[3])) - P + Q);

				k[5] = EquationBase::step * (YP[i] + 1. / 625. * (28 * m[0] - 125 * m[1] + 546 * m[2] + 54 * m[3] - 378 * m[4]));
				l[5] = EquationBase::step * (Y1[i] + 1. / 625. * (28 * n[0] - 125 * n[1] + 546 * n[2] + 54 * n[3] - 378 * n[4]));
				m[5] = EquationBase::step * (-(lambda_0) * ((XP[i] + 1. / 625. * (28 * k[0] - 125 * k[1] + 546 * k[2] + 54 * k[3]
					- 378 * k[4])) - (X1[i] + 1. / 625. * (28 * l[0] - 125 * l[1] + 546 * l[2] + 54 * l[3] - 378 * l[4])) - Nu)
					- P - Mu * cos((t + 1. / 5. * EquationBase::step) * PI / 180));
				n[5] = EquationBase::step * (-(lambda * lambda) *
					((X1[i] + 1. / 625. * (28 * l[0] - 125 * l[1] + 546 * l[2] + 54 * l[3] - 378 * l[4]))
					- (XP[i] + 1. / 625. * (28 * k[0] - 125 * k[1] + 546 * k[2] + 54 * k[3] - 378 * k[4])) + Nu) - Mu_0 * F_trait *
					sign(Y1[i] + 1. / 625. * (28 * n[0] - 125 * n[1] + 546 * n[2] + 54 * n[3] - 378 * n[4])) - P + Q);
				//конец вычисления дополнительных параметров

				//вычисляем контрольный член Егорова
				E = 1. / 336. * (-42 * k[0] - 224 * k[2] - 21 * k[3] + 162 * k[4] + 125 * k[5]);
				

				if (E >= machEps)
				{

					if ((fabs(E) - eps) >= machEps)
					{
						if (step / 2. >= machEps)
						{
							step = step / 2.;
							CorrectStep = false;
						}
					}

					if ((eps / 32. - fabs(E)) >= machEps)
					{
						step *= 2.;
						CorrectStep = false;
						continue;
					}
				}


			} while (CorrectStep == false);

			//следующий элемент Xp
			double nextXp = XP[i] + 1. / 6. * (k[0] + 4 * k[2] + k[3]);
			//следующий элемент X1
			double nextX1 = X1[i] + 1. / 6. * (l[0] + 4 * l[2] + l[3]);
			//следующий элемент Yp
			double nextYp = YP[i] + 1. / 6. * (m[0] + 4 * m[2] + m[3]);
			//следующий элемент X1
			double nextY1 = Y1[i] + 1. / 6. * (n[0] + 4 * n[2] + n[3]);

			XP.push_back(nextXp);
			X1.push_back(nextX1);
			YP.push_back(nextYp);
			Y1.push_back(nextY1);

			t += EquationBase::step;
			i++;
			
		} //while (t <= EquationBase::t_last)
		
	} 
};

#endif // !_EQUATION_Y1_H_
