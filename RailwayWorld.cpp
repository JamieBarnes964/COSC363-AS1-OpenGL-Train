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
	glColor3f(0.0, 0.0, 1.0);
	glDisable(GL_LIGHTING);
	glBegin(GL_LINE_LOOP);
	for (int i = 0; i < NPTS; i++)
		glVertex3f(ptx[i], 1, ptz[i]);
	glEnd();
	glEnable(GL_LIGHTING);
}

void drawRail()
{
	int w1 = 4;
	int w2 = 5;
	glColor3f(1,0,1);
	glNormal3f(0.0, 1.0, 0.0);
	glBegin(GL_QUAD_STRIP);
		for (int i = 0; i < NPTS; i++) {
			if (i == 0) {
				float u[] = {ptx[0] - ptx[NPTS-1], ptz[0] - ptz[NPTS-1]};
				float uLength = sqrt(pow(u[0], 2) + pow(u[1], 2));
				u[0] = u[0] / uLength;
				u[1] = u[1] / uLength;
				float v[] = {u[1], -u[0]};

				glVertex3f(ptx[NPTS-1] - (v[0] * w1), 1, ptz[NPTS-1] - (v[1] * w1));
				glVertex3f(ptx[NPTS-1] - (v[0] * w2), 1, ptz[NPTS-1] - (v[1] * w2));
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

			glVertex3f(ptx[i] - (v[0] * w1), 1, ptz[i] - (v[1] * w1));
			glVertex3f(ptx[i] - (v[0] * w2), 1, ptz[i] - (v[1] * w2));
		}
	glEnd();

	glBegin(GL_QUAD_STRIP);
		for (int i = 0; i < NPTS; i++) {
			if (i == 0) {
				float u[] = {ptx[0] - ptx[NPTS-1], ptz[0] - ptz[NPTS-1]};
				float uLength = sqrt(pow(u[0], 2) + pow(u[1], 2));
				u[0] = u[0] / uLength;
				u[1] = u[1] / uLength;
				float v[] = {u[1], -u[0]};

				glVertex3f(ptx[NPTS-1] + (v[0] * w1), 1, ptz[NPTS-1] + (v[1] * w1));
				glVertex3f(ptx[NPTS-1] + (v[0] * w2), 1, ptz[NPTS-1] + (v[1] * w2));
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

			glVertex3f(ptx[i] + (v[0] * w1), 1, ptz[i] + (v[1] * w1));
			glVertex3f(ptx[i] + (v[0] * w2), 1, ptz[i] + (v[1] * w2));
		}

	glEnd();
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

	gluLookAt (0, 50, 150, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	glLightfv(GL_LIGHT0, GL_POSITION, lgt_pos);   //light position

	drawRailPath();

	floor();
	// tracks(120, 10);  //mean radius 120 units, width 10 units
	float u[] = {-1, 0, 0};

	float v[] = {ptx[indx + 1] - ptx[indx], 0, ptz[indx + 1] - ptz[indx]};
	float vLength = sqrt(pow(v[0], 2) + pow(v[1], 2) + pow(v[2], 2));
	v[0] = v[0] / vLength;
	v[1] = v[1] / vLength;
	v[2] = v[2] / vLength;

	float theta = 180 * acos(v[0] * u[0] + v[1] * u[1] + v[2] * u[2]) / M_PI;

	if (indx > NPTS / 2) {
		theta = -theta;
	}

	float w[] = {u[1]*v[2] - u[2]*v[1], u[2]*v[0] - u[0]*v[2], u[0]*v[1] - u[1]*v[0]};
	// cout << theta << "\n";



	glPushMatrix();

		glPushMatrix();
			glTranslatef(ptx[indx], 1, ptz[indx]);
			glRotatef(theta, 0, 1, 0);
			engine();		 //locomotive
		glPopMatrix();

		// glPushMatrix();
		// 	glLightfv(GL_LIGHT1, GL_POSITION, spt_pos);
		// glPopMatrix();
		//
		// glPushMatrix();
		// 	glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, spt_dir);
		// glPopMatrix();

	glPopMatrix();

	drawRail();

	// glPushMatrix();
	// 	glRotatef(theta-10.5, 0, 1, 0);
	// 	glTranslatef(0, 0, -120);
	// 	wagon();
	// glPopMatrix();
	//
	// glPushMatrix();
	// 	glRotatef(theta-21, 0, 1, 0);
	// 	glTranslatef(0, 0, -120);
	// 	wagon();
	// glPopMatrix();
	//
	// glPushMatrix();
	// 	glRotatef(theta-31.5, 0, 1, 0);
	// 	glTranslatef(0, 0, -120);
	// 	wagon();
	// glPopMatrix();

	drawRailPath();

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
   glutMainLoop();
   return 0;
}
