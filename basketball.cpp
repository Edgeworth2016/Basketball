///////////////////////////////////////////////////////////////////////////////////////          
// basketball.cpp
// Author:  Kai bernstein
// kdbernst@syr.edu
// Assignment 1
// Visual Studio 2017
// February 5, 2018
// Interaction:
// Press V/v to increase/decrease the number of longitudinal (verticle) slices.
// Press H/h to increase/decrease the number of latitudinal (horizontal) slices.
// Press p to change from parallel to persepctive view and vice-versa.
// Press s to shoot the basketball!
// Click the right mouse button to quit
/////////////////////////////////////////////////////////////////////////////////////// 
#include "stdafx.h"
#include <cmath>
#include <iostream>

#ifdef __APPLE__
#  include <GLUT/glut.h>
#else
#  include <GL/glut.h>
#endif

#define PI 3.14159265

using namespace std;

// Globals.
static int score = 12;
static bool perspective = 0;
static float R = 1.0; // Radius of head.
static int v = 6; // Number of longitudinal (verticle) slices.
static int h = 4; // Number of latitudinal (horizontal) slices 
				  //on hemisphere.
static float cx = 2.0, cy = -12.0, cz = -10.0;  //center of sphere
static float bx = -18.0, by = -15.0, bz = -15.0;
static float ax = 14.0, ay = -15.0, az = -10.0;
static float bodylength = 4.0;
static float armlength = 5.0;
static float leglength = 6.0;
static float armanglea = 2.2*PI / 3.0;
static float armanglea2 = 4.0*PI / 3.0;
static float armangleb = 4.0*PI / 3.0;
static float armangleb2 = 4.0*PI / 3.0;
static float armanglec = 0.9*PI / 3.0;
static float armanglec2 = 2.1*PI / 3.0;
static float legangle=-(4.0*PI/10.0);
static float bcx = 5, bcy = -10, bcz = -10; //center of basketball
//static float legangle = -(1.0*PI / 10.0);
static double colorArray[] =
{ 0.0, 0.0, 1.0,
1.0, 0.0, 0.0,
1.0, 0.0, 0.0,
1.0, 0.0, 0.0 };
static double vertexArray[] =
{ -25.0, -20.0, -20.0,
  30.0, -20.0, -20.0,
  30.0, 0.0, -30.0,
 -25.0, 0.0, -30.0};

void drawHead(float dx, float dy, float dz)
{
	int i, j;
	// Based on hemisphere.cpp
	// cx, cy,cz is the center of the sphere, R is the radius.
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glColor3f(0.0, 0.0, 0.0);

	for (j = 0; j < h; j++)
	{
		// One latitudinal triangle strip. top half
		glBegin(GL_TRIANGLE_STRIP);
		for (i = 0; i <= v; i++)
		{
			glVertex3f(R * cos((float)(j + 1) / h * PI / 2.0) * cos(2.0 * (float)i / v * PI) + dx,
				R * sin((float)(j + 1) / h * PI / 2.0) + dy,
				R * cos((float)(j + 1) / h * PI / 2.0) * sin(2.0 * (float)i / v * PI) + dz);
			glVertex3f(R * cos((float)j / h * PI / 2.0) * cos(2.0 * (float)i / v * PI) + dx,
				R * sin((float)j / h * PI / 2.0) + dy,
				R * cos((float)j / h * PI / 2.0) * sin(2.0 * (float)i / v * PI) + dz);
		}
		glEnd();
		// One latitudinal triangle strip. bottom half
		glBegin(GL_TRIANGLE_STRIP);
		for (i = 0; i <= v; i++)
		{
			glVertex3f(R * cos((float)(j + 1) / h * PI / 2.0) * cos(2.0 * (float)i / v * PI) + dx,
				-1 * R * sin((float)(j + 1) / h * PI / 2.0) + dy,
				R * cos((float)(j + 1) / h * PI / 2.0) * sin(2.0 * (float)i / v * PI) + dz);
			glVertex3f(R * cos((float)j / h * PI / 2.0) * cos(2.0 * (float)i / v * PI) + dx,
				-1 * R * sin((float)j / h * PI / 2.0) + dy,
				R * cos((float)j / h * PI / 2.0) * sin(2.0 * (float)i / v * PI) + dz);
		}
		glEnd();
	}
}

void drawStickBody(float dx, float dy, float dz, int i)
{
	glLineWidth(5.0);
	if (i == 1)
		glColor3f(0.0, 0.0, 1.0);
	else
		glColor3f(1.0, 0.0, 0.0);
	glBegin(GL_LINES);
	glVertex3f(dx, dy - R, dz);
	glVertex3f(dx, dy - R - bodylength, dz);
	glEnd();
	glLineWidth(1.0);
	glColor3f(0.0, 0.0, 0.0);

}

void drawStickArms(float dx, float dy, float dz, float armangle, float armangle2, int k)
{
	glLineWidth(5.0);
	glColor3f(0.3, 0.3, 1.0);
	glBegin(GL_LINES);
	glVertex3f(dx, dy - R - .2*bodylength, dz); //down 20% of body
	if (k == 3 && score == 12)
		glVertex3f(dx + armlength * cos(armangle), dy - R - .2*bodylength + armlength * sin(armangle), dz-1);
	else
		glVertex3f(dx + armlength * cos(armangle), dy - R - .2*bodylength + armlength * sin(armangle), dz);
	glVertex3f(dx, dy - R - .2*bodylength, dz); //down 20% of body
	if (k == 3 && score == 12)
		glVertex3f(dx - armlength * cos(armangle2), dy - R - .2*bodylength + armlength * sin(armangle2), dz+1);
	else
		glVertex3f(dx - armlength * cos(armangle2), dy - R - .2*bodylength + armlength * sin(armangle2), dz);
	glEnd();
	glLineWidth(1.0);
	glColor3f(0.0, 0.0, 0.0);
}

void drawStickLegs(float dx, float dy, float dz)
{
	glLineWidth(5.0);
	glColor3f(0.0, 1.0, 0.0);
	glBegin(GL_LINES);
	glVertex3f(dx, dy - R - bodylength, dz); //down at bottom of body
	glVertex3f(dx + leglength * cos(legangle),
		//cy-R-.2*bodylength+leglength*sin(legangle),cz);
		dy - R - bodylength + leglength * sin(legangle), dz);//
	glVertex3f(dx, dy - R - bodylength, dz); //down at bottom of body
	glVertex3f(dx - leglength * cos(legangle),
		//cy-R-.2*bodylength+leglength*sin(legangle),cz);
		dy - R - bodylength + leglength * sin(legangle), dz);//
	glEnd();
	glLineWidth(1.0);
	glColor3f(0.0, 0.0, 0.0);
}

void drawPole() //draws the pole
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glColor3f(0.5, 0.5, 0.5);
	glBegin(GL_TRIANGLE_STRIP);
	for (int i = 0; i <= 40; i++) {
		glVertex3f(0.5*cos((i*2.0*PI) / 40) + 25, -25.0, 0.5*sin((i*2.0*PI) / 40) + -10);
		glVertex3f(0.5*cos((i*2.0*PI) / 40) + 25, 5.0, 0.5*sin((i*2.0*PI) / 40) + -10);
	}
	glEnd();
}

void drawBackboard() //draws the backboard
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glColor3f(0.5, 0.5, 0.5);
	glBegin(GL_POLYGON);
	glVertex3f(24.5, 0, -8);
	glVertex3f(24.5, 10, -8);
	glVertex3f(23.7, 10, -8);
	glVertex3f(23.7, 0, -8);
	glEnd();
	glBegin(GL_POLYGON);
	glVertex3f(23.7, 0, -8);
	glVertex3f(23.7, 10, -8);
	glVertex3f(23.7, 10, -12);
	glVertex3f(23.7, 0, -12);
	glEnd();
	glBegin(GL_POLYGON);
	glVertex3f(23.7, 0, -12);
	glVertex3f(23.7, 10, -12);
	glVertex3f(24.5, 10, -12);
	glVertex3f(24.5, 0, -12);
	glEnd();
	glBegin(GL_POLYGON);
	glVertex3f(24.5, 0, -8);
	glVertex3f(24.5, 10, -8);
	glVertex3f(24.5, 10, -12);
	glVertex3f(24.5, 0, -12);
	glEnd();
	glBegin(GL_POLYGON);
	glVertex3f(23.7, 0, -8);
	glVertex3f(23.7, 0, -8);
	glVertex3f(24.5, 0, -12);
	glVertex3f(24.5, 0, -12);
	glEnd();
	glBegin(GL_POLYGON);
	glVertex3f(23.7, 10, -8);
	glVertex3f(23.7, 10, -8);
	glVertex3f(24.5, 10, -12);
	glVertex3f(24.5, 10, -12);
	glEnd();
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glBegin(GL_POLYGON);
	glColor3f(0.0, 0.0, 0.0);
	glVertex3f(23.7, 0, -9.2);
	glVertex3f(23.7, 5, -9.2);
	glVertex3f(23.7, 5, -10.8);
	glVertex3f(23.7, 0, -10.8);
	glEnd();
}

void drawHoop() //draws the basket
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glColor3f(0.5, 0.5, 0.5);
	glBegin(GL_TRIANGLE_STRIP);
	for (int i = 0; i <= 40; i++) {
		glVertex3f(1.0*cos((i*2.0*PI) / 40) + 22.2, -3, 1.0*sin((i*2.0*PI) / 40) + -10);
		glVertex3f(1.5*cos((i*2.0*PI) / 40) + 22.2, 1, 1.5*sin((i*2.0*PI) / 40) + -10);
	}
	glEnd();
}

void drawBall() //draws the basketball
{
	int i, j;
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glColor3f(1.0, 0.5, 0.0);
	for (j = 0; j < h; j++)
	{
		glBegin(GL_TRIANGLE_STRIP);
		for (i = 0; i <= v; i++)
		{
			glVertex3f(1 * cos((float)(j + 1) / h * PI / 2.0) * cos(2.0 * (float)i / v * PI) + bcx,
				1 * sin((float)(j + 1) / h * PI / 2.0) + bcy,
				1 * cos((float)(j + 1) / h * PI / 2.0) * sin(2.0 * (float)i / v * PI) + bcz);
			glVertex3f(1 * cos((float)j / h * PI / 2.0) * cos(2.0 * (float)i / v * PI) + bcx,
				1 * sin((float)j / h * PI / 2.0) + bcy,
				1 * cos((float)j / h * PI / 2.0) * sin(2.0 * (float)i / v * PI) + bcz);
		}
		glEnd();
		glBegin(GL_TRIANGLE_STRIP);
		for (i = 0; i <= v; i++)
		{
			glVertex3f(1 * cos((float)(j + 1) / h * PI / 2.0) * cos(2.0 * (float)i / v * PI) + bcx,
				-1 * 1 * sin((float)(j + 1) / h * PI / 2.0) + bcy,
				1 * cos((float)(j + 1) / h * PI / 2.0) * sin(2.0 * (float)i / v * PI) + bcz);
			glVertex3f(1 * cos((float)j / h * PI / 2.0) * cos(2.0 * (float)i / v * PI) + bcx,
				-1 * 1 * sin((float)j / h * PI / 2.0) + bcy,
				1 * cos((float)j / h * PI / 2.0) * sin(2.0 * (float)i / v * PI) + bcz);
		}
		glEnd();
	}
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glLineWidth(0.5);
	glColor3f(0.0, 0.0, 0.0);
	glBegin(GL_TRIANGLE_FAN);
	glVertex3f(bcx, bcy, bcz);
	for (int k = 0; k <= 40; k++) {
		glVertex3f(bcx, .95*sin((k*2.0*PI) / 40) + bcy, .95*cos((k*2.0*PI) / 40) + bcz);
	}
	glEnd();
	glLineWidth(0.5);
	glColor3f(0.0, 0.0, 0.0);
	glBegin(GL_TRIANGLE_FAN);
	glVertex3f(bcx, bcy, bcz);
	for (int k = 0; k <= 40; k++) {
		glVertex3f(.95*cos((k*2.0*PI) / 40) + bcx, bcy, .95*sin((k*2.0*PI) / 40) + bcz);
	}
	glEnd();
	glLineWidth(0.5);
	glColor3f(0.0, 0.0, 0.0);
	glBegin(GL_TRIANGLE_FAN);
	glVertex3f(bcx + 0.5, bcy, bcz);
	for (int k = 0; k <= 40; k++) {
		glVertex3f(bcx + 0.5, 0.8*sin((k*2.0*PI) / 40) + bcy, 0.8*cos((k*2.0*PI) / 40) + bcz);
	}
	glEnd();
	glLineWidth(0.5);
	glColor3f(0.0, 0.0, 0.0);
	glBegin(GL_TRIANGLE_FAN);
	glVertex3f(bcx - 0.6, bcy, bcz);
	for (int k = 0; k <= 40; k++) {
		glVertex3f(bcx - 0.6, .75*sin((k*2.0*PI) / 40) + bcy, .75*cos((k*2.0*PI) / 40) + bcz);
	}
	glEnd();
}

void drawCrowd() //draws the stands and crowd
{
	glEnableClientState(GL_COLOR_ARRAY);
	glEnableClientState(GL_VERTEX_ARRAY);
	glColorPointer(3, GL_DOUBLE, 0, colorArray);
	glVertexPointer(3, GL_DOUBLE, 0, vertexArray);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glBegin(GL_POLYGON);
	glArrayElement(0);
	glArrayElement(1);
	glArrayElement(2);
	glArrayElement(3);
	glEnd();
	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
	glColor3f(0.0, 0.0, 0.0);
	glBegin(GL_POLYGON);
	glVertex3f(-25.0, -25.0, -20.0);
	glVertex3f(30.0, -25.0, -20.0);
	glVertex3f(30.0, -20.0, -20.0);
	glVertex3f(-25.0, -20.0, -20.0);
	glEnd();
}

void drawScoreboard() //draws the scoreboard
{
	if (!perspective) {
		glColor3f(1.0, 0.0, 0.0);
		glRasterPos3f(-6.1, 19, -30.0);
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 'H');
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 'o');
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 'm');
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 'e');
		glColor3f(0.0, 0.0, 1.0);
		glRasterPos3f(5, 19, -30.0);
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 'A');
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 'w');
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 'a');
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 'y');
	}
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glColor3f(1.0, 1.0, 0.0);
	glLineWidth(2);
	glBegin(GL_LINE_STRIP);
	glVertex3f(-6, 16.5, -30);
	glVertex3f(-6, 9.5, -30);
	glEnd();
	if (score == 12) {
		glBegin(GL_LINE_STRIP);
		glVertex3f(-3, 16.5, -30);
		glVertex3f(0, 16.5, -30);
		glVertex3f(0, 13, -30);
		glVertex3f(-3, 13, -30);
		glVertex3f(-3, 9.5, -30);
		glVertex3f(0, 9.5, -30);
		glEnd();
	}
	else {
		glBegin(GL_LINE_STRIP);
		glVertex3f(-3, 16.5, -30);
		glVertex3f(-3, 13, -30);
		glVertex3f(0, 13, -30);
		glVertex3f(0, 16.5, -30);
		glVertex3f(0, 13, -30);
		glVertex3f(0, 9.5, -30);
		glEnd();
	}
	glBegin(GL_LINE_STRIP);
	glVertex3f(5, 16.5, -30);
	glVertex3f(5, 9.5, -30);
	glEnd();
	glBegin(GL_LINE_LOOP);
	glVertex3f(8, 16.5, -30);
	glVertex3f(11, 16.5, -30);
	glVertex3f(11, 9.5, -30);
	glVertex3f(8, 9.5, -30);
	glEnd();
	glColor3f(1.0, 1.0, 1.0);
	glBegin(GL_POLYGON);
	glVertex3f(-7.0, 18.5, -30.0);
	glVertex3f(1.0, 18.5, -30.0);
	glVertex3f(1.0, 21.0, -30.0);
	glVertex3f(-7.0, 21.0, -30.0);
	glEnd();
	glBegin(GL_POLYGON);
	glVertex3f(4.0, 18.5, -30.0);
	glVertex3f(12.0, 18.5, -30.0);
	glVertex3f(12.0, 21.0, -30.0);
	glVertex3f(4.0, 21.0, -30.0);
	glEnd();
	glBegin(GL_POLYGON);
	glVertex3f(-7.0, 9.0, -30.0);
	glVertex3f(1.0, 9.0, -30.0);
	glVertex3f(1.0, 17.0, -30.0);
	glVertex3f(-7.0, 17.0, -30.0);
	glEnd();
	glBegin(GL_POLYGON);
	glVertex3f(4.0, 9.0, -30.0);
	glVertex3f(12.0, 9.0, -30.0);
	glVertex3f(12.0, 17.0, -30.0);
	glVertex3f(4.0, 17.0, -30.0);
	glEnd();
	glColor3f(0.0, 0.0, 0.0);
	glBegin(GL_POLYGON);
	glVertex3f(-10.0, 7.0, -30.0);
	glVertex3f(15.0, 7.0, -30.0);
	glVertex3f(15.0, 23.0, -30.0);
	glVertex3f(-10.0, 23.0, -30.0);
	glEnd();
}

// Drawing routine.
void drawScene(void)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if (perspective)
		glFrustum(-20.0, 25.0, -20.0, 25.0, 8.0, 30.01);
	else
		glOrtho(-25.0, 30.0, -25.0, 30.0, 0.0, 40.01);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	drawHead(ax, ay, az);
	drawStickBody(ax, ay, az, 1);
	drawStickArms(ax, ay, az, armanglea, armanglea2, 1);
	drawStickLegs(ax, ay, az);
	drawHead(bx, by, bz);
	drawStickBody(bx, by, bz, 2);
	drawStickArms(bx, by, bz, armangleb, armangleb2, 2);
	drawStickLegs(bx, by, bz);
	drawHead(cx, cy, cz);
	drawStickBody(cx, cy, cz, 3);
	drawStickArms(cx, cy, cz, armanglec, armanglec2, 3);
	drawStickLegs(cx, cy, cz);
	drawPole();
	drawBackboard();
	drawHoop();
	drawBall();
	drawCrowd();
	drawScoreboard();
	glutSwapBuffers();
}

// Initialization routine.
void setup(void)
{
	//white background
	glClearColor(1.0, 1.0, 1.0, 0.0);
}

// OpenGL window reshape routine.
void resize(int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if (perspective)
		glFrustum(-20.0, 25.0, -20.0, 25.0, 8.0, 30.01);
	else
		glOrtho(-25.0, 30.0, -25.0, 30.0, 0.0, 40.01);
	glMatrixMode(GL_MODELVIEW);
}

// Keyboard input processing routine.
void keyInput(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 27:
		exit(0);
		break;
	case 'V':
		v += 1;
		glutPostRedisplay();
		break;
	case 'v':
		if (v > 3) v -= 1;
		glutPostRedisplay();
		break;
	case 'H':
		h += 1;
		glutPostRedisplay();
		break;
	case 'h':
		if (h > 3) h -= 1;
		glutPostRedisplay();
		break;
	case 'p':
		perspective = (!perspective);
		glutPostRedisplay();
		break;
	case 's':
		cy = -15;
		armanglea = (4 * PI) / 3; armanglec = (4 * PI) / 3; armanglec2 = 4* PI / 3; armanglea2 = 4 * PI / 3;
		armangleb = PI / 3; armangleb2 = PI / 3;
		bcx = 22.2; bcy = 0.5;
		score = 14;
		glutPostRedisplay();
		break;
	default:
		break;
	}
}

void mouseButton(int button, int state, int x, int y)
{
	if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) exit(0);
}

// Routine to output interaction instructions to the C++ window.
void printInteraction(void)
{
	cout << "Interaction:" << endl;
	cout << "Press V/v to increase/decrease the number of longitudinal slices." << endl
		<< "Press H/h to increase/decrease the number of latitudinal slices." << endl
		<< "Press p to switch from parallel to perspective view and vice-versa." << endl
		<< "Press s to shoot the basketball!" << endl
		<< "Click right mouse button to quit" << endl;
}

// Main routine.
int main(int argc, char **argv)
{
	printInteraction();
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("A Basketball Game");
	setup();
	glutDisplayFunc(drawScene);
	//glClear(GL_DEPTH_BUFFER_BIT);
	glutReshapeFunc(resize);
	glutKeyboardFunc(keyInput);
	glutMouseFunc(mouseButton);
	glutMainLoop();

	return 0;
}
