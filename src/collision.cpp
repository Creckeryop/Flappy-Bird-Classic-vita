/*this code was written by creckeryop*/
#include "collision.h"

point::point() : x(0), y(0), len(0), x_l(0), y_l(0){};
point::point(float x, float y) : x(x), y(y),y_l(-y),x_l(-x) {};
point point::operator+(const point & a) { return point(x + a.x, y + a.y); }
point point::operator-(const point & a) { return point(x - a.x, y - a.y); }
float point::operator*(const point & a) { return x * a.x + y * a.y; }
point point::operator*(const float & a) { return point(a * x, a * y); }
point point::operator/(const float & a) { return point(x / a, y / a); }
float point::length() 
{
	if (x_l != x || y_l != 	y)
	{
		x_l = x; y_l = y;
		len = sqrt(x * x + y * y);
	}
	return len; 
}
point point::normal() { return point(-y, x)/length(); }

obj::obj(float x, float y, float w, float h) : position(x, y), points(new point[4]{point(-w/2,-h/2),point(w/2,-h/2),point(w/2,h/2),point(-w/2,h/2)}) {};
obj::~obj() {delete[] points;}
void obj::rotate(float a)
{
    float c = cos(a), s = sin(a);
	points[0] = point(c * points[0].x - s * points[0].y, s * points[0].x + c * points[0].y);
	points[1] = point(c * points[1].x - s * points[1].y, s * points[1].x + c * points[1].y);
	points[2] = point(c * points[2].x - s * points[2].y, s * points[2].x + c * points[2].y);
	points[3] = point(c * points[3].x - s * points[3].y, s * points[3].x + c * points[3].y);
}
void proj_obj(point N, obj *obj, float &min, float &max)
{
	float d = (obj->points[0] + obj->position) * N;
	min = d; max = d;
	for (int i = 1; i < 4; ++i)
	{
		d = (obj->points[i] + obj->position) * N;
		if (d < min) min = d; else if (d > max)	max = d;
	}
}
point res_vec(obj *a, obj *b)
{
	point vec = point(1000, 1000);
	float lenN = 100000;
	for (int i = 0, j = 1; i < 4; ++i, ++j) 
	{
		j %= 4;
		point N = (b->points[j] - b->points[i]).normal();
		float min_a, max_a, min_b, max_b;
		proj_obj(N, a, min_a, max_a);
		proj_obj(N, b, min_b, max_b);
		if (min_b > max_a || min_a > max_b)
			return point();
		else {
			float len;
			if (abs(max_b - min_a) > abs(max_a - min_b))
				len = min_b - max_a;
			else
				len = max_b - min_a;
			if (abs(lenN) > abs(len)) 
			{
				vec = N;
				lenN = len;
			}
		}
	}
	return vec * lenN;
}