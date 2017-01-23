#include"stdafx.h"
#include"Vector2D.h"

Vector2D & Vector2D::getVect2D()
{
	// TODO: 여기에 반환 구문을 삽입합니다.
	return *this;
}

void Vector2D::Vprintf()
{
	cout << "x : " << _x
		<< " y : " << _y
		<< " z : " << _z << endl;
}

Vector2D * Vector2D::operator*(U2DMETRIX & m)
{
	Vector2D * v = new Vector2D();
	v->_x = (this->_x * m.Getmrx()[0]) + (this->_y * m.Getmrx()[3]) + (this->_z *  m.Getmrx()[6]);
	v->_y = (this->_x * m.Getmrx()[1]) + (this->_y * m.Getmrx()[4]) + (this->_z *  m.Getmrx()[7]);
	v->_z = (this->_x * m.Getmrx()[2]) + (this->_y * m.Getmrx()[5]) + (this->_z *  m.Getmrx()[8]);
	return v;
}

void Vector2D::SetVect2D(double dx, double dy)
{
	_x = dx;
	_y = dy;
	_z = 1;
}
