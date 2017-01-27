#pragma once
#include"stdafx.h"
#include"Vector2D.h"

class U2DMETRIX
{
	typedef array<double, 9> mt2;
private:
	array <double, 9> M3trix;
	char name[20];
public:
	U2DMETRIX()
	{
		M3trix.fill(0.0f);
	}
	U2DMETRIX(double n11, double n12, double n21, double n22, double n31, double n32)
	{
		M3trix[0] = n11;
		M3trix[1] = n12;
		M3trix[2] = 0.0f;
		M3trix[3] = n21;
		M3trix[4] = n22;
		M3trix[5] = 0.0f;
		M3trix[6] = n31;
		M3trix[7] = n32;
		M3trix[8] = 1.0f;
	}
	U2DMETRIX(const U2DMETRIX &t)
	{
		M3trix = t.M3trix;
	}
	void reversMetrix();
	void operator=(const U2DMETRIX &t)
	{
		this->M3trix = t.M3trix;
	}
	bool operator==(U2DMETRIX &t)
	{
		for (auto n = 0; n < M3trix.max_size(); ++n)
		{
			if (t.M3trix[n] != this->M3trix[n]) {
				return false;
			}
			return true;
		}
	}
	U2DMETRIX * operator*(U2DMETRIX & t1)
	{
		U2DMETRIX * metrix = new U2DMETRIX();
		t1.reversMetrix();

		metrix->M3trix[0] = (M3trix[0] * t1.M3trix[0]) + (M3trix[1] * t1.M3trix[1]) + (M3trix[2] * t1.M3trix[2]);
		metrix->M3trix[1] = (M3trix[0] * t1.M3trix[3]) + (M3trix[1] * t1.M3trix[4]) + (M3trix[2] * t1.M3trix[5]);
		metrix->M3trix[2] = (M3trix[0] * t1.M3trix[6]) + (M3trix[1] * t1.M3trix[7]) + (M3trix[2] * t1.M3trix[8]);

		metrix->M3trix[3] = (M3trix[3] * t1.M3trix[0]) + (M3trix[4] * t1.M3trix[1]) + (M3trix[5] * t1.M3trix[2]);
		metrix->M3trix[4] = (M3trix[3] * t1.M3trix[3]) + (M3trix[4] * t1.M3trix[4]) + (M3trix[5] * t1.M3trix[5]);
		metrix->M3trix[5] = (M3trix[3] * t1.M3trix[6]) + (M3trix[4] * t1.M3trix[7]) + (M3trix[5] * t1.M3trix[8]);

		metrix->M3trix[6] = (M3trix[6] * t1.M3trix[0]) + (M3trix[7] * t1.M3trix[1]) + (M3trix[8] * t1.M3trix[2]);
		metrix->M3trix[7] = (M3trix[6] * t1.M3trix[3]) + (M3trix[7] * t1.M3trix[4]) + (M3trix[8] * t1.M3trix[5]);
		metrix->M3trix[8] = (M3trix[6] * t1.M3trix[6]) + (M3trix[7] * t1.M3trix[7]) + (M3trix[8] * t1.M3trix[8]);
		t1.reversMetrix();
		return metrix;
	}
	void SetScale(double dx, double dy);
	void SetRotate(float deg);
	mt2 & Getmrx() { return M3trix; }
	void SetName(const char * _name) {
		strcpy_s(name, _name);
	}
	void Msprint();
	~U2DMETRIX()
	{
		cout << name <<"¼Ò¸êÀÚ È£Ãâ" << endl;
	}
};