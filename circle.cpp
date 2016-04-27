#include "circle.h"
#include "glut.h"
#include "graphics1.h"
#include <math.h>
#include <cmath>
#include <iostream>

const double COLLISION_FRICTION = 0.9999;

Circle::Circle()
{

}
Circle::Circle(double x1, double y1, double radius, double r, double g, double b, double incX, double incY)
	:mx1(x1), my1(y1), mradius(radius), mr(r), mg(g), mb(b), mincX(incX), mincY(incY)
{

}
void Circle::SetX(double new_x) {
	mx1 = new_x;
}
void Circle::SetY(double new_y) {
	my1 = new_y;
}
double Circle::GetX() {
	return mx1;
}
void Circle::SetIncX(double inc) {
	if (inc > .3) {
		inc = .29;
	}
	mincX = inc;
}
double Circle::GetIncX() {
	return mincX;
}
void Circle::SetIncY(double inc) {
	mincY = inc;
}
double Circle::GetIncY() {
	return mincY;
}
double Circle::GetY() {
	return my1;
}
double Circle::GetR() {
	return mradius;
}

void Circle::Gravity(double G, bool l, bool r) {
	if (l) {
		mincX += G;
		return;
	}
	if (r) {
		mincX += G;
		return;
	}
	else {
		if (mincY > .3) {
			G = G - .1;
		}
		mincY += G;
	}
}

void Circle::HandleBallCollisions(std::vector<Circle> &g_shapes, int current, double dT, double G) {
	double x1 = g_shapes[current].GetX() + (g_shapes[current].GetIncX()* dT);
	double y1 = g_shapes[current].GetY() + g_shapes[current].GetIncY()* dT;
	double r1 = g_shapes[current].GetR();
	for (int j = current +1; j < g_shapes.size(); j++) {
		if (j == current) {
			continue;
		}
		double x2 = g_shapes[j].GetX() + (g_shapes[j].GetIncX()* dT);
		double y2 = g_shapes[j].GetY() + g_shapes[j].GetIncY();
		double r2 = g_shapes[j].GetR();
		//use distance formula if hypotenuse is less than sum of the radius, its intersecting
		double x_dist = x2 - x1;
		double y_dist = y2 - y1;
		double hyp = sqrt(x_dist*x_dist + y_dist*y_dist);
		if (hyp < r1 + r2) {
			g_shapes[current].Collide(current, j, g_shapes, dT);
		}
	}
}
struct vectortype
{
	double x;
	double y;
};

void Circle::Collide(int p1, int p2, std::vector<Circle> &particles, double dT)
{
	vectortype en; // Center of Mass coordinate system, normal component
	vectortype et; // Center of Mass coordinate system, tangential component
	vectortype u[2]; // initial velocities of two particles
	vectortype um[2]; // initial velocities in Center of Mass coordinates
	double umt[2]; // initial velocities in Center of Mass coordinates, tangent component
	double umn[2]; // initial velocities in Center of Mass coordinates, normal component
	vectortype v[2]; // final velocities of two particles
	double m[2];	// mass of two particles
	double M; // mass of two particles together
	vectortype V; // velocity of two particles together
	double size;
	int i;

	double xdif = (particles[p1].GetX() + particles[p1].GetIncX()*dT) - (particles[p2].GetX() + particles[p2].GetIncX()*dT);
	double ydif = (particles[p1].GetY() + particles[p1].GetIncY()*dT) - (particles[p2].GetY() + particles[p2].GetIncY()*dT);

	// set Center of Mass coordinate system
	size = sqrt(xdif*xdif + ydif*ydif);
	xdif /= size; ydif /= size; // normalize
	en.x = xdif;
	en.y = ydif;
	et.x = ydif;
	et.y = -xdif;

	// set u values
	u[0].x = particles[p1].GetIncX();
	u[0].y = particles[p1].GetIncY();
	m[0] = particles[p1].GetR()*particles[p1].GetR();
	u[1].x = particles[p2].GetIncX();
	u[1].y = particles[p2].GetIncY();
	m[1] = particles[p2].GetR()*particles[p2].GetR();

	// set M and V
	M = m[0] + m[1];
	V.x = (u[0].x*m[0] + u[1].x*m[1]) / M;
	V.y = (u[0].y*m[0] + u[1].y*m[1]) / M;

	// set um values
	um[0].x = m[1] / M * (u[0].x - u[1].x);
	um[0].y = m[1] / M * (u[0].y - u[1].y);
	um[1].x = m[0] / M * (u[1].x - u[0].x);
	um[1].y = m[0] / M * (u[1].y - u[0].y);

	// set umt and umn values
	for (i = 0; i<2; i++)
	{
		umt[i] = um[i].x * et.x + um[i].y * et.y;
		umn[i] = um[i].x * en.x + um[i].y * en.y;
	}

	// set v values
	for (i = 0; i<2; i++)
	{
		v[i].x = umt[i] * et.x - umn[i] * en.x + V.x;
		v[i].y = umt[i] * et.y - umn[i] * en.y + V.y;
	}

	// reset particle values
	particles[p1].SetIncX(v[0].x*COLLISION_FRICTION);
	particles[p1].SetIncY(v[0].y*COLLISION_FRICTION);
	particles[p2].SetIncX(v[1].x*COLLISION_FRICTION);
	particles[p2].SetIncY(v[1].y*COLLISION_FRICTION);

}
void Circle::Paint()
{
	glColor3d(mr, mg, mb);

	glBegin(GL_POLYGON);
	for (int i = 0; i<32; i++)
	{
		double theta = (double)i / 32.0 * 2.0 * 3.1415926;
		double xx = mx1 + mradius * cos(theta);
		double yy = my1 + mradius * sin(theta);
		glVertex2d(xx, yy);
	}
	glEnd();
}
