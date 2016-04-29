#include <iostream>
#include "point.h"
#include "graphics1.h"
#include "glut.h"
Point::Point()
{

}
Point::Point(double x_in, double y_in, double z_in, double r, double g, double b)
	: mX(x_in), mY(y_in), mZ(z_in), mR(r), mG(g), mB(b)
{
}
int Point::Clicked(double xx, double yy)  const {

	if (mX > xx      &&
		mX < xx + 10 &&
		mY > yy      &&
		mY < yy + 10) {
		return 1;
	}
	return 0;
}
void Point::Paint()
{
	glPointSize(5.0);
	glBegin(GL_POINTS);
	GLfloat mat_amb_diff1[] = { mR, mG, mB, 1 };
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, mat_amb_diff1);

	glVertex3d(mX,mY,mZ);
	glEnd();
}