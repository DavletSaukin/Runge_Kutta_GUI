#ifndef _EQUATION_BASE_H_
#define _EQUATION_BASE_H_

#include <vector>

class EquationBase
{
protected:
	double step; //шаг
	double eps; //эпсилон (допустимая погрешность)
	double machEps; //машинный эпсилон

	// первое значение независимой переменной
	double t_first;
	// последние значение независимой переменной
	double	t_last; 
	//вычисляем машинный эпсилон
	double CalcMachEps();
public:
	EquationBase(double _tfirst, double _tlast);
	virtual ~EquationBase();
	
	//устанавливаем эпсилон - допустимую погрешность
	void setEps(double _eps);
	//вычисляем уравнение
	virtual void Calculation() = 0;
};

#endif // !_EQUATION_BASE_H_