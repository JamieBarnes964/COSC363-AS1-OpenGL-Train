//  ========================================================================
//  COSC363: Computer Graphics (2022);  University of Canterbury.
//
//  FILE NAME: RailwayWorld.cpp
//  See Lab02.pdf for details
//  ========================================================================

#include <iostream>
#include <fstream>
#include <cmath>
#include <GL/freeglut.h>
#include "RailModels.h"
using namespace std;

float theta = 0;

const int NPTS = 492; 	// Number of points on the rail path

float ptx[NPTS], ptz[NPTS];

int indx = 0;


float angle=0, look_x, look_z=-1., eye_x, eye_z, height=10.;  //Camera parameters

void special(int key, int x, int y)
{
	if(key == GLUT_KEY_LEFT) angle -= 0.1;  //Change direction
	else if(key == GLUT_KEY_RIGHT) angle += 0.1;
	else if(key == GLUT_KEY_PAGE_UP) height += 1;
	else if(key == GLUT_KEY_PAGE_DOWN) height -= 1;
	else if(key == GLUT_KEY_DOWN)
	{  //Move backward
		eye_x -= 1*sin(angle);
		eye_z += 1*cos(angle);
	}
	else if(key == GLUT_KEY_UP)
	{ //Move forward
		eye_x += 1*sin(angle);
		eye_z -= 1*cos(angle);
	}

	look_x = eye_x + 100*sin(angle);
	look_z = eye_z - 100*cos(angle);
	glutPostRedisplay();
}

//Reads rail path data from Oval.txt
void loadRailPath()
{
	ifstream ifile;
	ifile.open("Oval.txt");
	for (int i = 0; i < NPTS; i++)
		ifile >> ptx[i] >> ptz[i];
	ifile.close();
}


void drawRailPath()
{
	glColor3f(0.0, 0.0, 0.3);
	glDisable(GL_LIGHTING);
	glBegin(GL_LINE_LOOP);
	for (int i = 0; i < NPTS; i++)
		glVertex3f(ptx[i], 1, ptz[i]);
	glEnd();
	glEnable(GL_LIGHTING);
}

// Draw a single GL_QUAD_STRIP following the rail points with the given offtets of widths and height
void drawRailStrip(int w1, int w2, int h1, int h2)
{
	glBegin(GL_QUAD_STRIP);
		for (int i = 0; i < NPTS; i++) {
			if (i == 0) {
				float u[] = {ptx[0] - ptx[NPTS-1], ptz[0] - ptz[NPTS-1]};
				float uLength = sqrt(pow(u[0], 2) + pow(u[1], 2));
				u[0] = u[0] / uLength;
				u[1] = u[1] / uLength;
				float v[] = {u[1], -u[0]};

				glVertex3f(ptx[NPTS-1] - (v[0] * w1), h1, ptz[NPTS-1] - (v[1] * w1));
				glVertex3f(ptx[NPTS-1] - (v[0] * w2), h2, ptz[NPTS-1] - (v[1] * w2));
			}

			float u[] = {ptx[i + 1] - ptx[i], ptz[i + 1] - ptz[i]};

			if (i == NPTS-1) {
				u[0] = ptx[0] - ptx[i];
				u[1] = ptz[0] - ptz[i];
			}

			float uLength = sqrt(pow(u[0], 2) + pow(u[1], 2));
			u[0] = u[0] / uLength;
			u[1] = u[1] / uLength;
			float v[] = {u[1], -u[0]};

			if (h1 != h2) glNormal3f(v[0], 0, v[1]);
			else glNormal3f(0, 1, 0);
			glVertex3f(ptx[i] - (v[0] * w1), h1, ptz[i] - (v[1] * w1));
			glVertex3f(ptx[i] - (v[0] * w2), h2, ptz[i] - (v[1] * w2));
		}
	glEnd();
}

// Draw rails
void drawRail()
{
	int w1 = 4;
	int w2 = 5;
	glColor3f(0.1, 0.1, 0.2);

	// Outer
	drawRailStrip(5, 6, 1, 1); // TOP
	drawRailStrip(6, 6, 1, 0); // OUTER
	drawRailStrip(5, 5, 1, 0); // INNER

	// Inner
	drawRailStrip(-5, -6, 1, 1); // TOP
	drawRailStrip(-6, -6, 1, 0); // OUTER
	drawRailStrip(-5, -5, 1, 0); // INNER
}

// Draw Sleepers
void drawSleepers()
{
	glColor3f(0.4, 0.2, 0.0);
	float width=8.0, thickness=0.5, height=0.1;

	for (int i = 0; i < NPTS; i += 3) {
		glBegin(GL_QUADS);
			float u[] = {ptx[i + 1] - ptx[i], ptz[i + 1] - ptz[i]};
			float uLength = sqrt(pow(u[0], 2) + pow(u[1], 2));
			u[0] = u[0] / uLength;
			u[1] = u[1] / uLength;
			float v[] = {u[1], -u[0]};

			glVertex3f(ptx[i] + (v[0] * width) - (u[0] * thickness), height, ptz[i] + (v[1] * width) - (u[1] * thickness));
			glVertex3f(ptx[i] - (v[0] * width) - (u[0] * thickness), height, ptz[i] - (v[1] * width) - (u[1] * thickness));
			glVertex3f(ptx[i] - (v[0] * width) + (u[0] * thickness), height, ptz[i] - (v[1] * width) + (u[1] * thickness));
			glVertex3f(ptx[i] + (v[0] * width) + (u[0] * thickness), height, ptz[i] + (v[1] * width) + (u[1] * thickness));
		glEnd();
	}
}


// Set the GL matrix to translate and rotate accurately on a point on the rail
void railVehicle(int relative)
{
	int pos = indx + relative;
	if (pos >= NPTS) pos = relative;
	else if (pos < 0) pos += NPTS;

	float u[] = {-1, 0, 0};
	float v[] = {ptx[pos + 1] - ptx[pos], 0, ptz[pos + 1] - ptz[pos]};
	if (pos == NPTS-1) {
		v[0] = ptx[0] - ptx[pos];
		v[2] = ptz[0] - ptz[pos];
	}
	float vLength = sqrt(pow(v[0], 2) + pow(v[1], 2) + pow(v[2], 2));
	v[0] = v[0] / vLength;
	v[1] = v[1] / vLength;
	v[2] = v[2] / vLength;

	float theta = 180 * acos(v[0] * u[0] + v[1] * u[1] + v[2] * u[2]) / M_PI;

	if (pos > NPTS / 2) {
		theta = -theta;
	}

	glTranslatef(ptx[pos], 0.8, ptz[pos]);
	glRotatef(theta, 0, 1, 0);
}

//---------------------------------------------------------------------
void initialize(void)
{
    float grey[4] = {0.2, 0.2, 0.2, 1.0};
    float white[4]  = {1.0, 1.0, 1.0, 1.0};

	loadRailPath();

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);

	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	glEnable(GL_COLOR_MATERIAL);
	glMaterialfv(GL_FRONT, GL_SPECULAR, white);
	glMaterialf(GL_FRONT, GL_SHININESS, 50);

//	Define light's ambient, diffuse, specular properties
    glLightfv(GL_LIGHT0, GL_AMBIENT, grey);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, white);
    glLightfv(GL_LIGHT0, GL_SPECULAR, white);

    glLightfv(GL_LIGHT1, GL_AMBIENT, grey);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, white);
    glLightfv(GL_LIGHT1, GL_SPECULAR, white);
    glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 30.0);
    glLightf(GL_LIGHT1, GL_SPOT_EXPONENT,0.01);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);
	glClearColor (0.0, 0.0, 0.0, 0.0);  //Background colour

    glMatrixMode (GL_PROJECTION);
    glLoadIdentity ();
    gluPerspective(60., 1.0, 10.0, 1000.0);   //Perspective projection
}

//-------------------------------------------------------------------
void display(void)
{
	float lgt_pos[] = {0.0f, 50.0f, 0.0f, 1.0f};  //light0 position (directly above the origin)
	float spt_pos[] = {-10.0f, 14.0f, 0.0f, 1.0f};  //light1 position (directly above the origin)
	float spt_dir[] = {-1.0f, -1.0f, 0.0f};  //light1 direction (directly above the origin)

	glClear (GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	gluLookAt(eye_x, height, eye_z,  look_x, 0, look_z,   0, 1, 0);
	glLightfv(GL_LIGHT0, GL_POSITION, lgt_pos);   //light position

	glEnable(GL_LIGHTING);

	floor();

	// Draw Train
	glPushMatrix();
		// Draw engine
		glPushMatrix();
			railVehicle(0);
			engine();
		glPopMatrix();

		// Draw spotlight
		glPushMatrix();
			railVehicle(0);
			glLightfv(GL_LIGHT1, GL_POSITION, spt_pos);
		glPopMatrix();

		glPushMatrix();
			railVehicle(0);
			glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, spt_dir);
		glPopMatrix();

		// Draw wagons
		for (int i = 1; i<4; i++) {
			glPushMatrix();
				railVehicle(-23 * i);
				wagon();
			glPopMatrix();
		}
	glPopMatrix();

	// Draw tracks
	drawRail();
	drawSleepers();


	glutSwapBuffers();   //Useful for animation
}




//---------------------------------------------------------------------
void myTimer(int value)
{
	indx += 1;
	if (indx == NPTS) indx = 0;
	glutPostRedisplay();
	glutTimerFunc(25, myTimer, 0);
}


//---------------------------------------------------------------------
int main(int argc, char** argv)
{
   glutInit(&argc, argv);
   glutInitDisplayMode (GLUT_DOUBLE|GLUT_DEPTH);
   glutInitWindowSize (600, 600);
   glutInitWindowPosition (50, 50);
   glutCreateWindow ("Toy Train");
   initialize ();
   glutTimerFunc(25, myTimer, 0);
   glutDisplayFunc(display);
   glutSpecialFunc(special);
   glutMainLoop();
   return 0;
}
