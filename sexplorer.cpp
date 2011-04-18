#include <Windows.h>
#include <string>
#include <sstream>
#include <math.h>
#include <GL/glut.h>
#include "texture.h"
#include "3dsloader.h"
#include "CelestialBody.h"

#define SCREEN_WIDTH 1000								// We want our screen width 800 pixels
#define SCREEN_HEIGHT 1000								// We want our screen height 600 pixels

CelestialBody planetEarth;
int frameCount = 0;

// Absolute rotation values (0-359 degrees) and rotation increments for each frame
double rotation_x=0, rotation_x_increment=0.1;
double rotation_y=0, rotation_y_increment=0.05;
double rotation_z=0, rotation_z_increment=0.03;

//ADDED
int filling=1; //0=OFF 1=ON

//ADDED
obj_type object;

void initSolarSystem()
{
	planetEarth = CelestialBody(6371000.0f, 149598261.0f,
		0.01671123f, 365.256363004f);
}


void init()
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glShadeModel(GL_SMOOTH);

	glViewport(0,0,SCREEN_WIDTH, SCREEN_HEIGHT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
    gluPerspective(45.0f, (GLdouble)SCREEN_WIDTH/(GLdouble)SCREEN_HEIGHT, 0.1, 6063120000.0);

	glEnable(GL_DEPTH_TEST); // We enable the depth test (also called z buffer)
    glPolygonMode (GL_FRONT_AND_BACK, GL_FILL); // Polygon rasterization mode (polygon filled)
    
    glEnable(GL_TEXTURE_2D); // This Enable the Texture mapping

    Load3DS (&object,"spaceship.3ds");

    object.id_texture=LoadBitmap("spaceshiptexture.bmp"); // The Function LoadBitmap() return the current texture ID
    
	initSolarSystem();

    // * Note * - The farthest distance should be at least 1 if you don't want some
    // funny artifacts when dealing with lighting and distance polygons.  This is a special
    // thing that not many people know about.  If it's less than 1 it creates little flashes
    // on far away polygons when lighting is enabled.

    //glMatrixMode(GL_MODELVIEW);                         // Select The Modelview Matrix
    //glLoadIdentity();
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


void renderShip()
{
	glBindTexture(GL_TEXTURE_2D, object.id_texture); // We set the active texture 

    glBegin(GL_TRIANGLES); // glBegin and glEnd delimit the vertices that define a primitive (in our case triangles)
    for (int l_index=0;l_index<object.polygons_qty;l_index++)
    {
        //----------------- FIRST VERTEX -----------------
        // Texture coordinates of the first vertex
        glTexCoord2f( object.mapcoord[ object.polygon[l_index].a ].u,
                      object.mapcoord[ object.polygon[l_index].a ].v);
        // Coordinates of the first vertex
        glVertex3f( object.vertex[ object.polygon[l_index].a ].x,
                    object.vertex[ object.polygon[l_index].a ].y,
                    object.vertex[ object.polygon[l_index].a ].z); //Vertex definition

        //----------------- SECOND VERTEX -----------------
        // Texture coordinates of the second vertex
        glTexCoord2f( object.mapcoord[ object.polygon[l_index].b ].u,
                      object.mapcoord[ object.polygon[l_index].b ].v);
        // Coordinates of the second vertex
        glVertex3f( object.vertex[ object.polygon[l_index].b ].x,
                    object.vertex[ object.polygon[l_index].b ].y,
                    object.vertex[ object.polygon[l_index].b ].z);
        
        //----------------- THIRD VERTEX -----------------
        // Texture coordinates of the third vertex
        glTexCoord2f( object.mapcoord[ object.polygon[l_index].c ].u,
                      object.mapcoord[ object.polygon[l_index].c ].v);
        // Coordinates of the Third vertex
        glVertex3f( object.vertex[ object.polygon[l_index].c ].x,
                    object.vertex[ object.polygon[l_index].c ].y,
                    object.vertex[ object.polygon[l_index].c ].z);
    }
    glEnd();
}


void DrawString(std::string in)
{
	for (size_t i = 0; i < in.length(); i++)
	{
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, in[i]);
	}
}

void renderScene(void) {
	int l_index;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);   // Clear The Screen And The Depth Buffer
    glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();                                   // Reset The View

	glTranslatef(0.0f, 0.0f, -300.0f);

	rotation_x = rotation_x + rotation_x_increment;
    rotation_y = rotation_y + rotation_y_increment;
    rotation_z = rotation_z + rotation_z_increment;

    if (rotation_x > 359) rotation_x = 0;
    if (rotation_y > 359) rotation_y = 0;
    if (rotation_z > 359) rotation_z = 0;

    glRotatef(rotation_x,1.0,0.0,0.0); // Rotations of the object (the model matrix is multiplied by the rotation matrices)
    glRotatef(rotation_y,0.0,1.0,0.0);
    glRotatef(rotation_z,0.0,0.0,1.0);
	
	glBindTexture(GL_TEXTURE_2D, object.id_texture); // We set the active texture 

    glBegin(GL_TRIANGLES); // glBegin and glEnd delimit the vertices that define a primitive (in our case triangles)
    for (l_index=0;l_index<object.polygons_qty;l_index++)
    {
        //----------------- FIRST VERTEX -----------------
        // Texture coordinates of the first vertex
        glTexCoord2f( object.mapcoord[ object.polygon[l_index].a ].u,
                      object.mapcoord[ object.polygon[l_index].a ].v);
        // Coordinates of the first vertex
        glVertex3f( object.vertex[ object.polygon[l_index].a ].x,
                    object.vertex[ object.polygon[l_index].a ].y,
                    object.vertex[ object.polygon[l_index].a ].z); //Vertex definition

        //----------------- SECOND VERTEX -----------------
        // Texture coordinates of the second vertex
        glTexCoord2f( object.mapcoord[ object.polygon[l_index].b ].u,
                      object.mapcoord[ object.polygon[l_index].b ].v);
        // Coordinates of the second vertex
        glVertex3f( object.vertex[ object.polygon[l_index].b ].x,
                    object.vertex[ object.polygon[l_index].b ].y,
                    object.vertex[ object.polygon[l_index].b ].z);
        
        //----------------- THIRD VERTEX -----------------
        // Texture coordinates of the third vertex
        glTexCoord2f( object.mapcoord[ object.polygon[l_index].c ].u,
                      object.mapcoord[ object.polygon[l_index].c ].v);
        // Coordinates of the Third vertex
        glVertex3f( object.vertex[ object.polygon[l_index].c ].x,
                    object.vertex[ object.polygon[l_index].c ].y,
                    object.vertex[ object.polygon[l_index].c ].z);
    }
    glEnd();

	glFlush();
    glutSwapBuffers();
}

void main(int argc, char **argv) {

	// init GLUT and create Window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(SCREEN_WIDTH, SCREEN_HEIGHT);
	glutCreateWindow("Space Explorer - Telescope Simulator");
	
	// register callbacks
	glutDisplayFunc(renderScene);
	glutIdleFunc(renderScene);

	init();

	// enter GLUT event processing cycle
	glutMainLoop();

}