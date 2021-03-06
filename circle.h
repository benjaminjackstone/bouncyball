#ifndef _CIRCLE_H_
#define _CIRCLE_H_
#include <cmath>
#include <vector>

class Circle
{
public:
	Circle();
	Circle(double x1, double y1, double z1, double radius, double r, double g, double b, double incX, double incY, double incZ);
	void Paint();
	void HandleBallCollisions(std::vector<Circle> &g_shapes, int current, double dT, double G);
	void Collide(int p1, int p2, std::vector<Circle> &particles, double dT);
	void Gravity(double G, bool l, bool r, bool b, bool f);
	void SetX(double new_x);
	void SetY(double new_y);
	void SetZ(double new_z);
	void SetIncZ(double inc);
	void SetIncX(double inc);
	void SetSuperMode(bool s) { mSuperMode = s; };
	void SetIncY(double inc);
	bool GetSuperMode() { return mSuperMode; };
	double GetX();
	double GetY();
	double GetR();
	double GetRed() { return mr; };
	double GetGreen() { return mg; };
	double GetBlue() { return mb; };
	double GetZ();
	double GetIncZ();
	double GetIncX();
	double GetIncY();
protected:
	double mx1, my1, mz1;
	double mradius;
	double mr, mg, mb;
	double mincX, mincY, mincZ;
	double mSuperMode;
private:
};

#endif