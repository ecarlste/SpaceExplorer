#include <Windows.h>
#include <string>
#include <sstream>
#include <math.h>
#include <GL/glut.h>
#include "load_3ds.h"
#include "load_bmp.h"
#include "object.h"
#include "CelestialBody.h"

#define SCREEN_WIDTH 1000								// We want our screen width 800 pixels
#define SCREEN_HEIGHT 1000								// We want our screen height 600 pixels

CelestialBody planetEarth;
int frameCount = 0;

//ADDED
int filling=1; //0=OFF 1=ON

//ADDED
//obj_type object;
//Lights settings
GLfloat light_ambient[]= { 0.1f, 0.1f, 0.1f, 0.1f };
GLfloat light_diffuse[]= { 0.3f, 0.2f, 0.3f, 0.0f };
GLfloat light_specular[]= { 0.3f, 0.3f, 0.3f, 0.0f };
GLfloat light_position[]= { 0.0f, 0.0f, 100.0f, 1.0f };

//Materials Color Settings
//Green
GLfloat mat_ambient_green[]= { 0.1f, 0.8f, 0.3f, 0.1f };
GLfloat mat_diffuse_green[]= { 1.0f, 1.0f, 1.0f, 0.0f };
GLfloat mat_specular_green[]= { 0.2f, 0.2f, 0.2f, 0.0f };
//shrub easy Green
GLfloat mat_ambient_grn[]= { 0.1f, 1.0f, 0.4f, 0.1f };
GLfloat mat_diffuse_grn[]= { 0.7f, 0.8f, 0.7f, 0.0f };
GLfloat mat_specular_grn[]= { 0.2f, 0.2f, 0.2f, 0.0f };

//Red
GLfloat mat_ambient_red[]= { 0.9f, 0.1f, 0.2f, 0.0f };
GLfloat mat_diffuse_red[]= { 1.0f, 0.0f, 0.0f, 0.0f };
GLfloat mat_specular_red[]= { 0.2f, 0.2f, 0.2f, 0.0f };

//Blue
GLfloat mat_ambient_blue[]= { 0.1f, 0.2f, 0.8f, 0.1f };
GLfloat mat_diffuse_blue[]= { 1.0f, 1.0f, 1.0f, 0.0f };
GLfloat mat_specular_blue[]= { 0.2f, 0.2f, 0.2f, 0.0f };

//Gray
GLfloat mat_ambient[]= { 0.1f, 0.1f, 0.1f, 0.0f };
GLfloat mat_diffuse[]= { 1.0f, 1.0f, 1.0f, 0.0f };
GLfloat mat_specular[]= { 0.2f, 0.2f, 0.2f, 0.0f };
GLfloat mat_shininess[]= { 1.0f };



void initSolarSystem()
{
	planetEarth = CelestialBody(6371000.0f, 149598261.0f,
		0.01671123f, 365.256363004f);
}


void init()
{
	MatrGenerateLookupTab();
	glClearColor(0.0, 0.0, 0.0, 0.0);  //black background 

	glViewport(0,0,SCREEN_WIDTH, SCREEN_HEIGHT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
    gluPerspective(45.0f, (GLdouble)SCREEN_WIDTH/(GLdouble)SCREEN_HEIGHT, 0.0, 6063120000.0);

	//Lights initialization and activation
    glLightfv (GL_LIGHT1, GL_AMBIENT, light_ambient);
    glLightfv (GL_LIGHT1, GL_DIFFUSE, light_diffuse);
    glLightfv (GL_LIGHT1, GL_SPECULAR, light_specular);
    glLightfv (GL_LIGHT1, GL_POSITION, light_position);    
    glEnable (GL_LIGHT1);
    glEnable (GL_LIGHTING);

    //Materials initialization and activation
	glMaterialfv (GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv (GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv (GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv (GL_FRONT, GL_SHININESS, mat_shininess);

	

	//Other initializations
    glShadeModel(GL_SMOOTH); // Type of shading for the polygons
	glHint (GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST); // Texture mapping perspective correction (OpenGL... thank you so much!)
    glEnable(GL_TEXTURE_2D); // Texture mapping ON
    glPolygonMode (GL_FRONT_AND_BACK, GL_FILL); // Polygon rasterization mode (polygon filled)
	glEnable(GL_CULL_FACE); // Enable the back face culling
    glEnable(GL_DEPTH_TEST); // Enable the depth test (also called z buffer)

	//Objects loading	
	ObjLoad ("shrub.3DS",'\0',                        0.0, 0.0, 0.0,		0,0,0,     1.0, 1.0, 4.0);
	ObjSetMaterial(&object[0],mat_ambient_grn, mat_diffuse_grn, mat_specular_grn, mat_shininess);

	ObjLoad ("grass-block.3DS",'\0',                    0.0, 0.0, 0.0,     0,0,0,      1.0,1.0,1.0);
	ObjSetMaterial(&object[1], mat_ambient_green, mat_diffuse_green, mat_specular_green, mat_shininess);
    
	initSolarSystem();

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

void renderObjects(void)
{
	int i,j;
	
	for (i=0;i<obj_qty;i++)
	{
		glMaterialfv(GL_FRONT, GL_AMBIENT,object[i].mat_ambient);
		glMaterialfv(GL_FRONT, GL_DIFFUSE,object[i].mat_diffuse);
		glMaterialfv(GL_FRONT, GL_SPECULAR,object[i].mat_specular);
		glMaterialfv(GL_FRONT, GL_SHININESS,object[i].mat_shininess);

		glPushMatrix(); // We save the current matrix
	
		glMultMatrixf(&object[i].matrix[0][0]); // Now let's multiply the object matrix by the identity-first matrix
		
		if (object[i].id_texture!=-1) 
		{
			glBindTexture(GL_TEXTURE_2D, object[i].id_texture); // We set the active texture 
		    glEnable(GL_TEXTURE_2D); // Texture mapping ON
		}
		else
		    glDisable(GL_TEXTURE_2D); // Texture mapping OFF

		glBegin(GL_TRIANGLES); // glBegin and glEnd delimit the vertices that define a primitive (in our case triangles)
		for (j=0;j<object[i].polygons_qty;j++)
		{
			//----------------- FIRST VERTEX -----------------
			//Normal coordinates of the first vertex
			glNormal3f( object[i].normal[ object[i].polygon[j].a ].x,
						object[i].normal[ object[i].polygon[j].a ].y,
						object[i].normal[ object[i].polygon[j].a ].z);
			// Texture coordinates of the first vertex
			glTexCoord2f( object[i].mapcoord[ object[i].polygon[j].a ].u,
						  object[i].mapcoord[ object[i].polygon[j].a ].v);
			// Coordinates of the first vertex
			glVertex3f( object[i].vertex[ object[i].polygon[j].a ].x,
						object[i].vertex[ object[i].polygon[j].a ].y,
						object[i].vertex[ object[i].polygon[j].a ].z);
			//----------------- SECOND VERTEX -----------------
			//Normal coordinates of the second vertex
			glNormal3f( object[i].normal[ object[i].polygon[j].b ].x,
						object[i].normal[ object[i].polygon[j].b ].y,
						object[i].normal[ object[i].polygon[j].b ].z);
			// Texture coordinates of the second vertex
			glTexCoord2f( object[i].mapcoord[ object[i].polygon[j].b ].u,
						  object[i].mapcoord[ object[i].polygon[j].b ].v);
			// Coordinates of the second vertex
			glVertex3f( object[i].vertex[ object[i].polygon[j].b ].x,
						object[i].vertex[ object[i].polygon[j].b ].y,
						object[i].vertex[ object[i].polygon[j].b ].z);
			//----------------- THIRD VERTEX -----------------
			//Normal coordinates of the third vertex
			glNormal3f( object[i].normal[ object[i].polygon[j].c ].x,
						object[i].normal[ object[i].polygon[j].c ].y,
						object[i].normal[ object[i].polygon[j].c ].z);
			// Texture coordinates of the third vertex
			glTexCoord2f( object[i].mapcoord[ object[i].polygon[j].c ].u,
						  object[i].mapcoord[ object[i].polygon[j].c ].v);
			// Coordinates of the Third vertex
			glVertex3f( object[i].vertex[ object[i].polygon[j].c ].x,
						object[i].vertex[ object[i].polygon[j].c ].y,
						object[i].vertex[ object[i].polygon[j].c ].z);

		}
		glEnd();
		glPopMatrix(); // Restore the previous matrix 
	}
}
//renderObject(int index)  
//  
void renderObject(int i)
{
		int j;
		glMaterialfv(GL_FRONT, GL_AMBIENT,object[i].mat_ambient);
		glMaterialfv(GL_FRONT, GL_DIFFUSE,object[i].mat_diffuse);
		glMaterialfv(GL_FRONT, GL_SPECULAR,object[i].mat_specular);
		glMaterialfv(GL_FRONT, GL_SHININESS,object[i].mat_shininess);

		glPushMatrix(); // We save the current matrix
	
		glMultMatrixf(&object[i].matrix[0][0]); // Now let's multiply the object matrix by the identity-first matrix
		
		if (object[i].id_texture!=-1) 
		{
			glBindTexture(GL_TEXTURE_2D, object[i].id_texture); // We set the active texture 
		    glEnable(GL_TEXTURE_2D); // Texture mapping ON
		}
		else
		    glDisable(GL_TEXTURE_2D); // Texture mapping OFF

		glBegin(GL_TRIANGLES); // glBegin and glEnd delimit the vertices that define a primitive (in our case triangles)
		for (j=0;j<object[i].polygons_qty;j++)
		{
			//----------------- FIRST VERTEX -----------------
			//Normal coordinates of the first vertex
			glNormal3f( object[i].normal[ object[i].polygon[j].a ].x,
						object[i].normal[ object[i].polygon[j].a ].y,
						object[i].normal[ object[i].polygon[j].a ].z);
			// Texture coordinates of the first vertex
			glTexCoord2f( object[i].mapcoord[ object[i].polygon[j].a ].u,
						  object[i].mapcoord[ object[i].polygon[j].a ].v);
			// Coordinates of the first vertex
			glVertex3f( object[i].vertex[ object[i].polygon[j].a ].x,
						object[i].vertex[ object[i].polygon[j].a ].y,
						object[i].vertex[ object[i].polygon[j].a ].z);
			//----------------- SECOND VERTEX -----------------
			//Normal coordinates of the second vertex
			glNormal3f( object[i].normal[ object[i].polygon[j].b ].x,
						object[i].normal[ object[i].polygon[j].b ].y,
						object[i].normal[ object[i].polygon[j].b ].z);
			// Texture coordinates of the second vertex
			glTexCoord2f( object[i].mapcoord[ object[i].polygon[j].b ].u,
						  object[i].mapcoord[ object[i].polygon[j].b ].v);
			// Coordinates of the second vertex
			glVertex3f( object[i].vertex[ object[i].polygon[j].b ].x,
						object[i].vertex[ object[i].polygon[j].b ].y,
						object[i].vertex[ object[i].polygon[j].b ].z);
			//----------------- THIRD VERTEX -----------------
			//Normal coordinates of the third vertex
			glNormal3f( object[i].normal[ object[i].polygon[j].c ].x,
						object[i].normal[ object[i].polygon[j].c ].y,
						object[i].normal[ object[i].polygon[j].c ].z);
			// Texture coordinates of the third vertex
			glTexCoord2f( object[i].mapcoord[ object[i].polygon[j].c ].u,
						  object[i].mapcoord[ object[i].polygon[j].c ].v);
			// Coordinates of the Third vertex
			glVertex3f( object[i].vertex[ object[i].polygon[j].c ].x,
						object[i].vertex[ object[i].polygon[j].c ].y,
						object[i].vertex[ object[i].polygon[j].c ].z);

		}
		glEnd();
		glPopMatrix(); // Restore the previous matrix 
	}

//scale key
void renderUnitCube()
{
	//Materials initialization and activation
	glMaterialfv (GL_FRONT, GL_AMBIENT, mat_ambient_red);
    glMaterialfv (GL_FRONT, GL_DIFFUSE, mat_diffuse_red);
    glMaterialfv (GL_FRONT, GL_SPECULAR, mat_specular_red);
	
	//to make our unit cube = 1 meter:
	glPushMatrix();
	glTranslatef(0.0, 0.0, 0.002);
	glScalef(.001, .001, .001);
	
	
    glutSolidCube(1);
	glPopMatrix();
}

//renderGrassField
//	calls renderObject however many times
void renderGrassField()
{
	int j,k;
	float dx=23.0f;
	float dy=23.0f;

	for(k=-5;k<4;k++)
	{
		for(j=-7;j<5;j++)
		{
			glPushMatrix();
			glTranslatef(k*dx, j*(-dy), 0.0f);
			renderObject(1);
			glPopMatrix();
		}
	}
	
}

//renderShrubFence
//	calls renderObject however many times
void renderShrubFence()
{
	int j,k;
	float dx=23.0f;
	float dy=23.0f;
	
	/*glPushMatrix();
	renderObject(0);
	glPopMatrix();*/

	//along y-direction
	for(k=-6;k<9;k++)
	{
			glPushMatrix();
			glTranslatef(5*dx, 0.75*k*dy, 0.3f);
			renderObject(0);
			glPopMatrix();		
	}
	//along x-direction
	for(j=-10;j<12;j++)
	{
		glPushMatrix();
		glTranslatef(0.45*j*dx, -5*dy, 0.0f);
		renderObject(0);
		glPopMatrix();
	}
	
}

void renderScene(void) {
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);   // Clear The Screen And The Depth Buffer
    glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();                                   // Reset The View

	gluLookAt(-120.0, 250.0, 90.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0);

	//renderObjects();
	renderGrassField();
	renderUnitCube();
	renderShrubFence();
	
    glFlush(); // This forces the execution of OpenGL commands
    glutSwapBuffers(); // In double buffered mode we invert the positions of the visible buffer and the writing buffer
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