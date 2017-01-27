#pragma once
#include"stdafx.h"
#include"Metrix.h"
class Vector2D
{
private:
	double _x;
	double _y;
	double _z;
public:
	Vector2D() : _x(0.0f), _y(0.0f), _z(1.0f)
	{}
	Vector2D(double x, double y) : _x(x), _y(y), _z(1.0f)
	{}
	Vector2D(const Vector2D & v)
	{
		_x = v._x;
		_y = v._y;
		_z = v._z;
	}
	void operator =(const Vector2D &v) {
		this->_x = v._x;
		this->_y = v._y;
		this->_z = v._z;
	}
	bool operator ==(const Vector2D &v) {
		if (v._x != _x || v._y != _y || v._z != _z) return false;
		return true;
	}
	Vector2D * operator* (U2DMETRIX & m);
	void SetVect2D(double dx, double dy);
	Vector2D & getVect2D();
	void Vprintf();
	double GetVec_x() const { return _x; }
	double GetVec_y() const { return _y; }
	double GetVec_z() const { return _z; }
};