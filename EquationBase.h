#ifndef _EQUATION_BASE_H_
#define _EQUATION_BASE_H_

#include <vector>

class EquationBase
{
protected:
	double step; //���
	double eps; //������� (���������� �����������)
	double machEps; //�������� �������

	// ������ �������� ����������� ����������
	double t_first;
	// ��������� �������� ����������� ����������
	double	t_last; 
	//��������� �������� �������
	double CalcMachEps();
public:
	EquationBase(double _tfirst, double _tlast);
	virtual ~EquationBase();
	
	//������������� ������� - ���������� �����������
	void setEps(double _eps);
	//��������� ���������
	virtual void Calculation() = 0;
};

#endif // !_EQUATION_BASE_H_