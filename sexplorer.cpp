#include <string>
#include <sstream>
#include <math.h>
#include <GL/glut.h>
#include "CelestialBody.h"

#define SCREEN_WIDTH 1000								// We want our screen width 800 pixels
#define SCREEN_HEIGHT 1000								// We want our screen height 600 pixels

CelestialBody planetEarth;
int frameCount = 0;

void initSolarSystem()
{
	planetEarth = CelestialBody(6371000.0f, 149598261.0f,
		0.01671123f, 365.256363004f);
}


void init(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(SCREEN_WIDTH, SCREEN_HEIGHT);
	glutCreateWindow("Space Explorer - Telescope Simulator");

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
    gluPerspective(60.0, (GLdouble)SCREEN_WIDTH/(GLdouble)SCREEN_HEIGHT, 0.0, 6063120000.0);

	initSolarSystem();

    // * Note * - The farthest distance should be at least 1 if you don't want some
    // funny artifacts when dealing with lighting and distance polygons.  This is a special
    // thing that not many people know about.  If it's less than 1 it creates little flashes
    // on far away polygons when lighting is enabled.

    glMatrixMode(GL_MODELVIEW);                         // Select The Modelview Matrix
    glLoadIdentity();
}


void renderSun()
{
	glPushMatrix();
	//glTranslatef(0.0f, 0.0f, 0.0f);
	glScalef(6950000.0, 6950000.0, 6950000.0);
	glColor3f(1.0f, 1.0f, 0.0f);
	glutWireSphere(1.0, 16, 16);
	glPopMatrix();
}

void DrawString(std::string in)
{
	for (size_t i = 0; i < in.length(); i++)
	{
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, in[i]);
	}
}

void renderScene(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);   // Clear The Screen And The Depth Buffer
    glLoadIdentity();                                   // Reset The View
	gluLookAt( 0.0, 0.0, 300000000.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0 );
	glMatrixMode(GL_MODELVIEW);

	// 149600000.0

	renderSun();

	/*
	glRasterPos2f(-100.0f, 60.0f);
	std::stringstream ss;
	ss << "x = " << planetEarth.getPosition().x << ", y = " 
		<< planetEarth.getPosition().y << ", frameCount = " << frameCount;
	DrawString(ss.str());
	*/

	glColor3f(0.0f, 0.0f, 1.0f);
	planetEarth.updatePosition((float)frameCount++);
	planetEarth.render();

    glutSwapBuffers();
	glutPostRedisplay();
}

void main(int argc, char **argv) {

	// init GLUT and create Window
	init(argc, argv);
	// register callbacks
	glutDisplayFunc(renderScene);

	// enter GLUT event processing cycle
	glutMainLoop();
}