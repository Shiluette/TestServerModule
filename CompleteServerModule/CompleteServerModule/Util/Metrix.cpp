#include"stdafx.h"
#include"Metrix.h"

void U2DMETRIX::reversMetrix()
{
	mt2 m = { M3trix[0], M3trix[3], M3trix[6],
		M3trix[1], M3trix[4], M3trix[7],
		M3trix[2], M3trix[5], M3trix[8] };
	M3trix = m;
}

void U2DMETRIX::SetScale(double dx, double dy)
{
		M3trix.fill(0.0f);
		M3trix[0] = M3trix[4] = M3trix[8] = 1;
		M3trix[6] = dx;
		M3trix[7] = dy;
}

void U2DMETRIX::SetRotate(float deg)
{
	double pi = 3.14159265;
	double Sin = static_cast<double>(sin(deg * pi / 180)); //sin(deg);
	double Cos = static_cast<double>(cos(deg * pi / 180)); //cos(deg);
	M3trix.fill(0.0f);
	M3trix[8] = 1.0f;
	M3trix[0] = Cos;
	M3trix[1] = Sin;
	M3trix[3] = -Sin;
	M3trix[4] = Cos;
}

void U2DMETRIX::Msprint()
{
	printf(" %0.3f | %0.3f | %0.3f\n %0.3f | %0.3f | %0.3f\n %0.3f | %0.3f | %0.3f\n\n", M3trix[0], M3trix[1], M3trix[2], M3trix[3], M3trix[4], M3trix[5], M3trix[6], M3trix[7], M3trix[8]);
}
