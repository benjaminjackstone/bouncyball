#ifndef _CIRCLE_H_
#define _CIRCLE_H_
#include <cmath>
#include <vector>

class Circle
{
public:
	Circle();
	Circle(double x1, double y1, double radius, double r, double g, double b, double incX, double incY);
	void Paint();
	void HandleBallCollisions(std::vector<Circle> &g_shapes, int current, double dT, double G);
	void Collide(int p1, int p2, std::vector<Circle> &particles, double dT);
	void Gravity(double G, bool l, bool r);
	void SetX(double new_x);
	void SetY(double new_y);
	void SetIncX(double inc);
	void SetIncY(double inc);
	double GetX();
	double GetY();
	double GetR();
	double GetIncX();
	double GetIncY();
protected:
	double mx1, my1;
	double mradius;
	double mr, mg, mb;
	double mincX, mincY;
private:
};

#endif