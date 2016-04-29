#pragma once
#ifndef _POINT_H_
#define _POINT_H_

class Point
{
public:
	Point();
	Point(double x_in, double y_in, double z_in, double r, double g, double b);
	virtual void Paint();
	int Clicked(double x, double y) const;
	void SetY(double y) { mY = y; }
	void SetX(double x) { mX = x; }
	void SetZ(double z) { mZ = z; }
	double GetY() { return mY; }
	double GetX() { return mX; }
	double GetZ() { return mZ; }
	double GetRed() { return mR; };
	double GetGreen() { return mG; };
	double GetBlue() { return mB; };
protected:
	double mX, mY, mZ, mR, mG, mB;
private:
};
#endif