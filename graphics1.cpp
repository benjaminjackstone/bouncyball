#include <cmath>
#include <cstring>
#include "glut.h"
#include <time.h>
#include "graphics1.h"
#include "circle.h"
#include "point.h"
#include <cstdlib>
#include <vector>
#include <iostream>
// Global Variables (Only what you need!)
double screen_x = 1000;
double screen_y = 700;
double screen_z = 2000;
double dT = GetDeltaTime();
int SIZE = 10;
double GRAVITY = -0.0005;
double AIR_FRICTION = .5;
bool Left = false;
bool Right, Front, Back, gSuperMode = false;
bool bounce = false;
std::vector<Circle> g_shapes;
std::vector<Point> gPoints;
double gX = 0;
double gY = 0;
double gZ = -1;
double eye[3] = { gX, gY, gZ }; // pick a nice vantage point.
double at[3] = { 1, 0, 1 };
double GetDeltaTime()
{
	static clock_t start_time = clock();
	static int current_frame = 0;
	clock_t current_time = clock();
	current_frame += 1;
	double total_time = double(current_time - start_time) / CLOCKS_PER_SEC;
	if (total_time == 0)
		total_time = .00001;
	double frames_per_second = (double)current_frame / total_time;
	double DT = 1.0 / frames_per_second;
	return DT;
}
void DrawLine(double x1, double y1, double z1, double x2, double y2, double z2, double w)
{
	glEnable(GL_LINE_SMOOTH);
	glLineWidth(w);
	glColor3d(.5,.5,.5);
	glBegin(GL_LINES);
	glVertex3d(x1, y1, z1);
	glVertex3d(x2, y2, z2);
	glEnd();
}
void SetScreenX(double x) {
	screen_y = x;
}
void SetScreenY(double y) {
	screen_y = y;
}
void SetScreenZ(double z) {
	screen_z = z;
}
double GetScreenX() {
	return screen_x;
}
double GetScreenY() {
	return screen_y;
}
double GetScreenZ() {
	return screen_z;
}
void handleCollisions(void) {
	for (int i = 0; i < SIZE; i++) {
		g_shapes[i].Gravity(GRAVITY, Left, Right, Front, Back);
		g_shapes[i].HandleBallCollisions(g_shapes, i, dT, GRAVITY);
		double x = g_shapes[i].GetX();
		double y = g_shapes[i].GetY();
		double z = g_shapes[i].GetZ();
		double r = g_shapes[i].GetR();
		double incX = g_shapes[i].GetIncX();
		double incY = g_shapes[i].GetIncY();
		double incZ = g_shapes[i].GetIncY();
		if (x + incX + r >= screen_x + 500) {
			g_shapes[i].SetIncX(-incX);
			Point p(x, y, z, g_shapes[i].GetRed(), g_shapes[i].GetGreen(), g_shapes[i].GetBlue());
			gPoints[i] = p;
		}
		if (x + incX - r <= -500) {
			g_shapes[i].SetIncX(-incX);
			Point p(x, y, z, g_shapes[i].GetRed(), g_shapes[i].GetGreen(), g_shapes[i].GetBlue());
			gPoints[i] = p;
		}
		if (z + incZ + r >= 2000) {
			g_shapes[i].SetIncZ(-incZ);
			Point p(x, y, z, g_shapes[i].GetRed(), g_shapes[i].GetGreen(), g_shapes[i].GetBlue());
			gPoints[i] = p;
		}
		if (z - incZ - r<= 0) {
			g_shapes[i].SetIncZ(-incZ);
		}
		if (y + incY + r >= screen_y +500) {
			g_shapes[i].SetIncY(-incY);
			Point p(x, y, z, g_shapes[i].GetRed(), g_shapes[i].GetGreen(), g_shapes[i].GetBlue());
			gPoints[i] = p;
		}
		if (y + incY - r <= -500) {
			g_shapes[i].SetIncY(-incY);
			Point p(x, y, z, g_shapes[i].GetRed(), g_shapes[i].GetGreen(), g_shapes[i].GetBlue());
			gPoints[i] = p;
		}
		if (bounce) {
			// randomize pos or neg direction
			int neg = std::rand() % 10;
			double b = (std::rand() % 20) /10.0;
			if (neg >= 5) b = -b;
			double b2 = (std::rand() % 20) /10.0;
			if (neg < 5) b2 = -b2;
			g_shapes[i].SetIncY(b);
			g_shapes[i].SetIncX(b2);
			g_shapes[i].SetIncZ(b2);
		}
		//std::cout << g_shapes[i].GetIncY() << std::endl;
		x += g_shapes[i].GetIncX() * AIR_FRICTION;
		y += g_shapes[i].GetIncY()* AIR_FRICTION;
		z += g_shapes[i].GetIncZ()* AIR_FRICTION;
		g_shapes[i].SetX(x);
		g_shapes[i].SetY(y);
		g_shapes[i].SetZ(z);
		//g_shapes[i].Paint();
		gPoints[i].Paint();
		GLfloat mat_amb_diff1[] = { g_shapes[i].GetRed(), g_shapes[i].GetGreen(), g_shapes[i].GetBlue(), 1 };
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, mat_amb_diff1);
		glPushMatrix();
		glTranslatef(g_shapes[i].GetX() , g_shapes[i].GetY() , g_shapes[i].GetZ());
		glutSolidSphere(g_shapes[i].GetR(), 10, 10);
		glPopMatrix();
	}
	if(gSuperMode){
		for (int i = 0; i < gPoints.size(); i++) {
			g_shapes[i].SetIncX(g_shapes[i].GetIncX() + .01);
			g_shapes[i].SetIncY(g_shapes[i].GetIncY() + .01);
		}
	}
	bounce = false;
}
void display(void)
{	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	gluLookAt(eye[0], eye[1], eye[2], at[0], at[1], at[2], 0, 1, 0);
	DrawLine(1500, -500, 0,    2500, -500, 5000, 50 ); // bottom left
	DrawLine(2200, -500, 4000, -800, -550, 5000, 50); // bottom middle
	DrawLine(2200, -550, 4000, 2200, 2300, 4000, 50); // left support
	DrawLine(1500, 1000, 0,    2200, 2300, 4000, 50); //top left
	DrawLine(2200, 2300, 4000, -800, 2200, 5000, 50); // top middle
	DrawLine(-800, 2200, 5000, -800, -550, 5000, 50); //right support
	DrawLine(-800, -550, 5000, -1500, -1850, 0, 50); // bottom right
	DrawLine(-800, 2200, 5000, -1500, 700, 0, 50); //top right
	handleCollisions();
	glutSwapBuffers();
	glutPostRedisplay(); //forcing the animation
}


// This callback function gets called by the Glut
// system whenever a key is pressed.
void keyboard(unsigned char c, int x, int y)
{
	switch (c) 
	{
		case 27: // escape character means to quit the program
			exit(0);
			break;
		case 'b':
			if (GRAVITY > 0) {
				GRAVITY = -GRAVITY;
			}
			Left = false;
			Right = false;
			Front = false;
			Back = false;
			break;
		case 't':
			if (GRAVITY < 0) {
				GRAVITY = -GRAVITY;
			}
			Left = false;
			Right = false;
			Front = false;
			Back = false;
			break;
		case 'l':
			if (GRAVITY > 0) {
				GRAVITY = -GRAVITY;
			}
			Left = true;
			Right = false;
			Front = false;
			Back = false;
			break;
		case 'r':
			Right = true;
			Left = false;
			Front = false;
			Back = false;
			if (GRAVITY < 0) {
				GRAVITY = -GRAVITY;
			}
			break;
		case 'f':
			Right = false;
			Left = false;
			Front = true;
			Back = false;
			if (GRAVITY < 0) {
				GRAVITY = -GRAVITY;
			}
			break;
		case 'g':
			Right = false;
			Left = false;
			Front = false;
			Back = true;
			if (GRAVITY > 0) {
				GRAVITY = -GRAVITY;
			}
			break;
		case 'z':
			bounce = true;
			break;
		case 's':
			gSuperMode = true;
			break;
		case 'a':
			gSuperMode = false;
			break;
		default:
			return; // if we don't care, return without glutPostRedisplay()
	}

	glutPostRedisplay();
}


// This callback function gets called by the Glut
// system whenever the window is resized by the user.
void SetPerspectiveView(int w, int h)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	double aspectRatio = (GLdouble)w / (GLdouble)h;
	gluPerspective(
		/* field of view in degree */ 90.0,
		/* aspect ratio */ aspectRatio,
		/* Z near */ 30, /* Z far */ 5000.0);
	glMatrixMode(GL_MODELVIEW);
}
void reshape(int w, int h)
{
	screen_x = w;
	screen_y = h;

	// Set the pixel resolution of the final picture (Screen coordinates).
	glViewport(0, 0, w, h);

	SetPerspectiveView(w, h);

}

// This callback function gets called by the Glut
// system whenever any mouse button goes up or down.
void mouse(int mouse_button, int state, int x1, int y1)
{
	if (mouse_button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) 
	{
		//need to increment size for every new ball added
		SIZE += 1;
		Circle cir;
		double incX = (std::rand() % 100 + 1) / 1000.0;
		double incY = (std::rand() % 200 + 1) / 1000.0;
		double incZ = (std::rand() % 100 + 1) / 1000.0;
		double z = (std::rand() % 2000 + 1);
		double radius = std::rand() % 100 + 20;
		double x = x1;
		double y = screen_y - y1;
		double red = (std::rand() % 101) / 100.0;
		double green = (std::rand() % 101) / 100.0;
		double blue = (std::rand() % 101) / 100.0;
		std::cout << x << ' ' << y << ' ' << z << " " << radius << std::endl;
		cir = Circle(x, y, z, radius, red, green, blue, incX, incY, incZ);
		g_shapes.push_back(cir);
		Point p(0,0,0,0,0,0);
		gPoints.push_back(p);
	}
	if (mouse_button == GLUT_LEFT_BUTTON && state == GLUT_UP) 
	{
	}
	if (mouse_button == GLUT_MIDDLE_BUTTON && state == GLUT_DOWN) 
	{
	}
	if (mouse_button == GLUT_MIDDLE_BUTTON && state == GLUT_UP) 
	{
	}
	glutPostRedisplay();
}
// Your initialization code goes here.
void InitializeMyStuff()
{
	Point p(0, 0, 0, 0, 0, 0);
	for (int i = 0; i < SIZE; i++) {
		gPoints.push_back(p);
	}
	// set material's specular properties
	GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat mat_shininess[] = { 50.0 };
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

	// set light properties
	GLfloat light_position[] = { 1000, 1500,0,0}; //if fourth paramter is a one, then it a positional light. the light goes in every direction
											  // if its a zero then the light comes from the vector, or one source
	GLfloat white_light[] = { 1,1,1,1 };
	GLfloat low_light[] = { .3,.3,.3,1 };
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, white_light); // specify first light's color
	glLightfv(GL_LIGHT0, GL_SPECULAR, low_light);
	glEnable(GL_DEPTH_TEST); // turn on depth buffering
	glEnable(GL_LIGHTING);	// enable general lighting
	glEnable(GL_LIGHT0);	// enable the first light.
	Circle cir;
	std::vector<std::vector<double>> spawnx;
	int screenx = screen_x - 100;
	int screeny = screen_y - 100;
	int screenz = screen_z - 100;
	for (int i = 0; i < SIZE; i++) {
		std::vector<double> spawny;
		double incX = (std::rand() % 100 + 1)/1000.0;
		double incY = (std::rand() % 200 + 1) / 1000.0;
		double incZ = (std::rand() % 200 + 1) / 1000.0;
		double radius = std::rand() % 100 + 20;
		//calc screen values to mod by so it doesn't spawn in a wall
		double x = (std::rand() % screenx + radius) + incX;
		double y = (std::rand() % screeny + radius) + incY;
		double z = (std::rand() % 1000) + incZ;
		//vector of previous spawns so check against spawning inside each other
		for (int j = 0; j < spawnx.size(); j++) {
			double xdist = spawnx[j][0] - x;
			double ydist = (spawnx[j][1]) - y;
			double zdist = (spawnx[j][2]) - z;
			double hyp = sqrt(xdist*xdist + ydist*ydist +zdist*zdist);
			//keep searching for a spawn point
			while (hyp < spawnx[j][3] + radius) {
				x = (std::rand() % screenx + radius) + incX;
				y = (std::rand() %  screeny + radius) + incY;
				z = (std::rand() % 1000) + incZ;
				xdist = spawnx[j][0] - x;
				zdist = (spawnx[j][2]) - z;
				hyp = sqrt(xdist*xdist + ydist*ydist + zdist*zdist);
			}
		}
		//push vector of points into vector of spawns points
		spawny.push_back(x);
		spawny.push_back(y);
		spawny.push_back(z);
		spawny.push_back(radius);
		spawnx.push_back(spawny);
		double red = (std::rand() % 101)/100.0;
		double green = (std::rand() % 101) / 100.0;
		double blue = (std::rand() % 101) / 100.0;
		std::cout << x << ' ' << y << ' ' << radius << ' ' << red << ' ' << green << ' ' << blue << ' '<< std::endl;
		cir = Circle(x, y, z, radius, red, green, blue, incX, incY, incZ);
		g_shapes.push_back(cir);
	}
}
/*void motion(int x, int y) {
	double x1 = x;
	double y1 = screen_y - y;
	Circle c;
	c = Circle(x1, y1, 50, 0, 0, 0, 0, 0);
	g_shapes.push_back(c);
	SIZE += 1;
	c.HandleBallCollisions(g_shapes, g_shapes.size()-1, dT, GRAVITY);
	g_shapes.erase(g_shapes.begin()+(SIZE-1));
	SIZE -= 1;
	std::cout << "motion?";
	glutPostRedisplay();
}*/

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	std::srand(time(0));
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(screen_x, screen_y);
	glutInitWindowPosition(50, 50);

	int fullscreen = 0;
	if (fullscreen) 
	{
		glutGameModeString("800x600:32");
		glutEnterGameMode();
	} 
	else 
	{
		glutCreateWindow("This appears in the title bar");
	}

	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutReshapeFunc(reshape);
	//glutMotionFunc(motion);
	glutMouseFunc(mouse);

	//glColor3d(0,0,0); // forground color
	glClearColor(1, 1, 1, 0); // background color
	InitializeMyStuff();

	glutMainLoop();

	return 0;
}
