#include "circle.h"
#include "glut.h"
#include "graphics1.h"
#include <math.h>
#include <cmath>
#include <iostream>

const double COLLISION_FRICTION = 0.990;
extern bool gSuperMode;
Circle::Circle()
{

}
Circle::Circle(double x1, double y1, double z1, double radius, double r, double g, double b, double incX, double incY, double incZ)
	:mx1(x1), my1(y1), mz1(z1), mradius(radius), mr(r), mg(g), mb(b), mincX(incX), mincY(incY), mincZ(incZ)
{
	mSuperMode = false;
}
void Circle::SetX(double new_x) {
	mx1 = new_x;
}
void Circle::SetY(double new_y) {
	my1 = new_y;
}
void Circle::SetZ(double new_z) {
	mz1 = new_z;
}
double Circle::GetZ() {
	return mz1;
}
double Circle::GetX() {
	return mx1;
}
void Circle::SetIncX(double inc) {
	/*if (inc > .3) {
		inc = .29;
	}*/
	mincX = inc;

}
void Circle::SetIncZ(double inc) {
	mincZ = inc;
}
void Circle::SetIncY(double inc) {
	mincY = inc;
}
double Circle::GetIncX() {
	return mincX;
}
double Circle::GetIncZ() {
	return mincZ;
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

void Circle::Gravity(double G, bool l, bool r, bool b, bool f) {
	if (l || r) {
		mincX += G;
		return;
	}
	else if (b || f) {
		mincZ += G;
		return;
	}
	else {
		if (mincY > 1) {
			G = G - .05;
		}
	}
		 mincY += G;
}

void Circle::HandleBallCollisions(std::vector<Circle> &g_shapes, int current, double dT, double G) {
	double x1 = g_shapes[current].GetX() + (g_shapes[current].GetIncX()* dT);
	double y1 = g_shapes[current].GetY() + g_shapes[current].GetIncY()* dT;
	double z1 = g_shapes[current].GetZ() + g_shapes[current].GetIncZ()* dT;
	double r1 = g_shapes[current].GetR();
	for (int j = current +1; j < g_shapes.size(); j++) {
		if (j == current) {
			continue;
		}
		double x2 = g_shapes[j].GetX() + (g_shapes[j].GetIncX());
		double y2 = g_shapes[j].GetY() + g_shapes[j].GetIncY();
		double z2 = g_shapes[j].GetZ() + g_shapes[j].GetIncZ();
		double r2 = g_shapes[j].GetR();
		//use distance formula if hypotenuse is less than sum of the radius, its intersecting
		double x_dist = x2 - x1;
		double y_dist = y2 - y1;
		double z_dist = z2 - z1;
		double hyp = sqrt(x_dist*x_dist + y_dist*y_dist + z_dist*z_dist);
		if (hyp <= r1 + r2 + 5) {
			g_shapes[current].Collide(current, j, g_shapes, dT);
			 x1 = g_shapes[current].GetX() + (g_shapes[current].GetIncX()* dT);
			 y1 = g_shapes[current].GetY() + g_shapes[current].GetIncY()* dT;
			 z1 = g_shapes[current].GetZ() + g_shapes[current].GetIncZ()* dT;
			 x2 = g_shapes[j].GetX() + (g_shapes[j].GetIncX()* dT);
			 y2 = g_shapes[j].GetY() + g_shapes[j].GetIncY();
			 z2 = g_shapes[j].GetZ() + g_shapes[j].GetIncZ()* dT;
			//use distance formula if hypotenuse is less than sum of the radius, its intersecting
			 x_dist = x2 - x1;
			 y_dist = y2 - y1;
			 z_dist = z2 - z1;
			 hyp = sqrt(x_dist*x_dist + y_dist*y_dist + z_dist*z_dist);
			 if (hyp <= r1 + r2) {
				 if (x1 < x2 && (x2+r2+1.5 < GetScreenX()+500)) {
					 g_shapes[current].SetX(g_shapes[current].GetX() - x1);
					 continue;
				 }
				 if (x2 < x1 && (x1 + r1 + 1.5 < GetScreenX()+500)) {
					 g_shapes[j].SetX(g_shapes[j].GetX() - x2);
					 continue;
				 }
				 if (x1 > x2 && (x2 - r2 - 1.5 < GetScreenX() - 500)) {
					 g_shapes[current].SetX(g_shapes[current].GetX() + x1);
					 continue;
				 }
				 if (x2 > x1 && (x1 - r1 - 1.5 > GetScreenX() - 500)) {
					 g_shapes[j].SetX(g_shapes[j].GetX() + x2);
					 continue;
				 }
			 }
		}
	}
}
struct vectortype
{
	double x;
	double y;
	double z;
};

void Circle::Collide(int p1, int p2, std::vector<Circle> &particles, double dT)
{
	vectortype en; // Center of Mass coordinate system, normal component
	vectortype et; // Center of Mass coordinate system, tangential component
	vectortype u[3]; // initial velocities of two particles
	vectortype um[3]; // initial velocities in Center of Mass coordinates
	double umt[3]; // initial velocities in Center of Mass coordinates, tangent component
	double umn[3]; // initial velocities in Center of Mass coordinates, normal component
	vectortype v[3]; // final velocities of two particles
	double m[3];	// mass of two particles
	double M; // mass of two particles together
	vectortype V; // velocity of two particles together
	double size;
	int i;

	double xdif = (particles[p1].GetX() + particles[p1].GetIncX()*dT) - (particles[p2].GetX() + particles[p2].GetIncX()*dT);
	double ydif = (particles[p1].GetY() + particles[p1].GetIncY()*dT) - (particles[p2].GetY() + particles[p2].GetIncY()*dT);
	double zdif = (particles[p1].GetZ() + particles[p1].GetIncZ()*dT) - (particles[p2].GetZ() + particles[p2].GetIncZ()*dT);

	// set Center of Mass coordinate system
	size = sqrt(xdif*xdif + ydif*ydif + zdif*zdif);
	xdif /= size; ydif /= size; zdif /= size; // normalize
	en.x = xdif;
	en.y = ydif;
	en.z = zdif;
	et.x = ydif;
	et.y = -xdif;
	et.z = zdif;

	// set u values
	u[0].x = particles[p1].GetIncX();
	u[0].y = particles[p1].GetIncY();
	u[0].z = particles[p1].GetIncZ();
	m[0] = particles[p1].GetR()*particles[p1].GetR();
	u[1].x = particles[p2].GetIncX();
	u[1].y = particles[p2].GetIncY();
	u[1].z = particles[p2].GetIncZ();
	m[1] = particles[p2].GetR()*particles[p2].GetR();

	// set M and V
	M = m[0] + m[1];
	V.x = (u[0].x*m[0] + u[1].x*m[1]) / M;
	V.y = (u[0].y*m[0] + u[1].y*m[1]) / M;
	V.z = (u[0].z*m[0] + u[1].z*m[1]) / M;

	// set um values
	um[0].x = m[1] / M * (u[0].x - u[1].x);
	um[0].y = m[1] / M * (u[0].y - u[1].y);
	um[0].z = m[1] / M * (u[0].z - u[1].z);
	um[1].x = m[0] / M * (u[1].x - u[0].x);
	um[1].y = m[0] / M * (u[1].y - u[0].y);
	um[1].z = m[0] / M * (u[1].z - u[0].z);
	// set umt and umn values
	for (i = 0; i<3; i++)
	{
		umt[i] = um[i].x * et.x + um[i].y * et.y + um[i].z * et.z;
		umn[i] = um[i].x * en.x + um[i].y * en.y + um[i].z * et.z;
		umn[i] = um[i].x * en.x + um[i].y * en.y + um[i].z * et.z;
	}

	// set v values
	for (i = 0; i<3; i++)
	{
		v[i].x = umt[i] * et.x - umn[i] * en.x + V.x;
		v[i].y = umt[i] * et.y - umn[i] * en.y + V.y;
		v[i].z = umt[i] * et.z - umn[i] * en.z + V.z;
	}

	// reset particle values
	particles[p1].SetIncX(v[0].x*COLLISION_FRICTION);
	particles[p1].SetIncY(v[0].y*COLLISION_FRICTION);
	particles[p1].SetIncZ(v[0].z*COLLISION_FRICTION);
	particles[p2].SetIncX(v[1].x*COLLISION_FRICTION);
	particles[p2].SetIncY(v[1].y*COLLISION_FRICTION);
	particles[p2].SetIncZ(v[1].z*COLLISION_FRICTION);

}
void Circle::Paint()
{
	GLfloat mat_amb_diff1[] = { mr, mg, mb, 1 };
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, mat_amb_diff1);
	//glColor3d(mr, mg, mb);

	glBegin(GL_POLYGON);
	for (int i = 0; i<32; i++)
	{
		double theta = (double)i / 32.0 * 2.0 * 3.1415926;
		double xx = mx1 + mradius * cos(theta);
		double yy = my1 + mradius * sin(theta);
		glVertex3d(xx, yy, mz1);
	}
	glEnd();
}
