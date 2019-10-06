/*this code was written by creckeryop*/
#pragma once
#include <math.h>
class point
{
private:
	float len, x_l, y_l;
public:
	float x, y;
	point(float x, float y);
	point();
	float length();
	point normal();
	point operator+(const point & a);
	point operator-(const point & a);
	void operator+=(const point & a);
	void operator-=(const point & a);
	float operator*(const point & a);
	point operator*(const float & a);
	point operator/(const float & a);
	point operator-();
};
class obj
{
	public:
	obj(float x, float y, float w, float h);
	point position;
	point* points;
	void rotate(float a);
	~obj();
};

point res_vec(obj *a, obj *b);