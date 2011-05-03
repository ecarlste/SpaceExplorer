
#include "spaceexplorer.h"

#define SCREEN_WIDTH 1280								// We want our screen width 800 pixels
#define SCREEN_HEIGHT 720								// We want our screen height 600 pixels

CCamera g_Camera;										// This is our global camera object

bool  g_bFullScreen = false;							// Set full screen as default
HWND  g_hWnd;											// This is the handle for the window
RECT  g_rRect;											// This holds the window dimensions
HDC   g_hDC;											// General HDC - (handle to device context)
HGLRC g_hRC;											// General OpenGL_DC - Our Rendering Context for OpenGL
HINSTANCE g_hInstance;									// This holds the global hInstance for UnregisterClass() in DeInit()

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

///////////////////////////////// CREATE MY WINDOW \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////   This function creates a window, but doesn't have a message loop
/////
///////////////////////////////// CREATE MY WINDOW \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

HWND CreateMyWindow(LPSTR strWindowName, int width, int height, DWORD dwStyle, bool bFullScreen, HINSTANCE hInstance)
{
    HWND hWnd;
    WNDCLASS wndclass;
    
    memset(&wndclass, 0, sizeof(WNDCLASS));               // Init the size of the class
    wndclass.style = CS_HREDRAW | CS_VREDRAW;           // Regular drawing capabilities
    wndclass.lpfnWndProc = WinProc;                       // Pass our function pointer as the window procedure
    wndclass.hInstance = hInstance;                       // Assign our hInstance
    wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);    // General icon
    wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);      // An arrow for the cursor
    wndclass.hbrBackground = (HBRUSH) (COLOR_WINDOW+1);  // A white window
    wndclass.lpszClassName = "GameTutorials";         // Assign the class name

    RegisterClass(&wndclass);                           // Register the class
    
	if(!dwStyle)                                  // Assign styles to the window depending on the choice
        dwStyle = WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
    
    g_hInstance = hInstance;                          // Assign our global hInstance to the window's hInstance

    // Below, we need to adjust the window to it's true requested size.  If we say we
    // want a window that is 800 by 600, that means we want the client rectangle to
    // be that big, not the entire window.  If we go into window mode, it will cut off
    // some of the client rect and stretch the remaining which causes slow down.  We fix this below.

    RECT rWindow;
    rWindow.left    = 0;                               // Set Left Value To 0
    rWindow.right   = width;                           // Set Right Value To Requested Width
    rWindow.top     = 0;                               // Set Top Value To 0
    rWindow.bottom  = height;                          // Set Bottom Value To Requested Height

    AdjustWindowRect( &rWindow, dwStyle, false);     // Adjust Window To True Requested Size

                                                        // Create the window
    hWnd = CreateWindow("GameTutorials", strWindowName, dwStyle, 0, 0,
                        rWindow.right  - rWindow.left, rWindow.bottom - rWindow.top, 
                        NULL, NULL, hInstance, NULL);

    if(!hWnd) return NULL;                                // If we could get a handle, return NULL

    ShowWindow(hWnd, SW_SHOWNORMAL);                   // Show the window
    UpdateWindow(hWnd);                                 // Draw the window

    SetFocus(hWnd);                                     // Sets Keyboard Focus To The Window  

    return hWnd;
}


/////////////////////////////// ANIMATE NEXT FRAME \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////	This function clamps a specified frame rate for consistency
/////
/////////////////////////////// ANIMATE NEXT FRAME \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

bool AnimateNextFrame(int desiredFrameRate)
{
	static float lastTime = GetTickCount() * 0.001f;
	static float elapsedTime = 0.0f;

	float currentTime = GetTickCount() * 0.001f; // Get the time (milliseconds = seconds * .001)
	float deltaTime = currentTime - lastTime; // Get the slice of time
	float desiredFPS = 1.0f / desiredFrameRate; // Store 1 / desiredFrameRate

	elapsedTime += deltaTime; // Add to the elapsed time
	lastTime = currentTime; // Update lastTime

	// Check if the time since we last checked is greater than our desiredFPS
	if( elapsedTime > desiredFPS )
	{
		elapsedTime -= desiredFPS; // Adjust our elapsed time

		// Return true, to animate the next frame of animation
		return true;
	}

	// We don't animate right now.
	return false;
}


///////////////////////////////// MAIN GAME LOOP \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////   This function Handles the main game loop
/////
///////////////////////////////// MAIN GAME LOOP \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

WPARAM MainLoop()
{
    MSG msg;

    while(1)                                            // Do our infinite loop
    {                                                   // Check if there was a message
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) 
        { 
            if(msg.message == WM_QUIT)                    // If the message was to quit
                break;
            TranslateMessage(&msg);                     // Find out what the message does
            DispatchMessage(&msg);                      // Execute the message
        }
        else                                            // If there wasn't a message
        { 
            if (AnimateNextFrame(60))
			{
				g_Camera.Update();
				RenderScene();
			}
			else Sleep(1);
        } 
    }

    return(msg.wParam);                                 // Return from the program
}

///////////////////////////////// SET UP PIXEL FORMAT \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////   This function sets the pixel format for OpenGL.
/////
///////////////////////////////// SET UP PIXEL FORMAT \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

bool bSetupPixelFormat(HDC hdc) 
{ 
    PIXELFORMATDESCRIPTOR pfd = {0}; 
    int pixelformat; 
 
    pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);            // Set the size of the structure
    pfd.nVersion = 1;                                 // Always set this to 1
                                                        // Pass in the appropriate OpenGL flags
    pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER; 
    pfd.dwLayerMask = PFD_MAIN_PLANE;                 // We want the standard mask (this is ignored anyway)
    pfd.iPixelType = PFD_TYPE_RGBA;                       // We want RGB and Alpha pixel type
    pfd.cColorBits = SCREEN_DEPTH;                        // Here we use our #define for the color bits
    pfd.cDepthBits = SCREEN_DEPTH;                        // Depthbits is ignored for RGBA, but we do it anyway
    pfd.cAccumBits = 0;                                   // No special bitplanes needed
    pfd.cStencilBits = 0;                             // We desire no stencil bits
 
    // This gets us a pixel format that best matches the one passed in from the device
    if ( (pixelformat = ChoosePixelFormat(hdc, &pfd)) == FALSE ) 
    { 
        MessageBox(NULL, "ChoosePixelFormat failed", "Error", MB_OK); 
        return FALSE; 
    } 
 
    // This sets the pixel format that we extracted from above
    if (SetPixelFormat(hdc, pixelformat, &pfd) == FALSE) 
    { 
        MessageBox(NULL, "SetPixelFormat failed", "Error", MB_OK); 
        return FALSE; 
    } 
 
    return TRUE;                                       // Return a success!
}

//////////////////////////// RESIZE OPENGL SCREEN \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////   This function resizes the viewport for OpenGL.
/////
//////////////////////////// RESIZE OPENGL SCREEN \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void SizeOpenGLScreen(int width, int height)            // Initialize The GL Window
{
    if (height==0)                                     // Prevent A Divide By Zero error
    {
        height=1;                                       // Make the Height Equal One
    }

    glViewport(0,0,width,height);                       // Make our viewport the whole window.
                                                        // We could make the view smaller inside
                                                        // Our window if we wanted too.
                                                        // The glViewport takes (x, y, width, height).
                                                        // This basically means, what are our drawing boundaries
                                                        // on the screen in client coordinates (you can have like 4
                                                        // viewports rendering different things in one window, like 3DS Max.

    glMatrixMode(GL_PROJECTION);                        // Select The Projection Matrix
    glLoadIdentity();                                   // Reset The Projection Matrix

                                                        // Calculate The Aspect Ratio Of The Window
                                                        // The parameters are:
                                                        // (view angle, aspect ratio of the width to the height, 
                                                        //  the closest distance to the camera before it clips, 
                  // FOV     // Ratio               //  the farthest distance before it stops drawing).
    gluPerspective(45.0f, (GLdouble)width/(GLdouble)height, 0.0, 6063120000.0);

    // * Note * - The farthest distance should be at least 1 if you don't want some
    // funny artifacts when dealing with lighting and distance polygons.  This is a special
    // thing that not many people know about.  If it's less than 1 it creates little flashes
    // on far away polygons when lighting is enabled.

    glMatrixMode(GL_MODELVIEW);                         // Select The Modelview Matrix
    glLoadIdentity();                                   // Reset The Modelview Matrix
}

///////////////////////////////// INITIALIZE GL \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////   This function handles all the initialization for OpenGL.
/////
///////////////////////////////// INITIALIZE GL \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void InitializeOpenGL(int width, int height) 
{  
    g_hDC = GetDC(g_hWnd);                                // This sets our global HDC
                                                        // We don't free this hdc until the end of our program
    if (!bSetupPixelFormat(g_hDC))                     // This sets our pixel format/information
        PostQuitMessage (0);                           // If there's an error, quit

    // We need now to create a rendering context AFTER we setup the pixel format.
    // A rendering context is different than a device context (hdc), but that is
    // what OpenGL uses to draw/render to.  Because OpenGL can be used on
    // Macs/Linux/Windows/etc.. It has it's on type of rendering context that is
    // The same for EACH operating system, but it piggy backs our HDC information.
    g_hRC = wglCreateContext(g_hDC);                  // This creates a rendering context from our hdc
    wglMakeCurrent(g_hDC, g_hRC);                      // This makes the rendering context we just created the one we want to use.

	MatrGenerateLookupTab();

	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_LIGHTING);
	//glEnable(GL_LIGHT0);
	//glEnable(GL_NORMALIZE);
	glEnable(GL_CULL_FACE); // Enable the back face culling
    glShadeModel(GL_SMOOTH); // Type of shading for the polygons
	//glEnable(GL_TEXTURE_2D); // Texture mapping ON

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	//GLfloat ambientColor[] = {0.2f, 0.2f, 0.2f, 1.0f};
	//glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientColor);

	//GLfloat lightColor[] = {0.5f, 0.5f, 0.5f, 1.0f};
	//GLfloat lightPos[] = {10.0f, 10.0f, 10.0f, 1.0f};
	//glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor);
	//glLightfv(GL_LIGHT0, GL_POSITION, lightPos);

	//Lights initialization and activation
    glLightfv (GL_LIGHT1, GL_AMBIENT, light_ambient);
    glLightfv (GL_LIGHT1, GL_DIFFUSE, light_diffuse);
    glLightfv (GL_LIGHT1, GL_SPECULAR, light_specular);
    glLightfv (GL_LIGHT1, GL_POSITION, light_position);    
    glEnable (GL_LIGHT1);

	//Other initializations
    glHint (GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST); // Texture mapping perspective correction (OpenGL... thank you so much!)
    glPolygonMode (GL_FRONT_AND_BACK, GL_FILL); // Polygon rasterization mode (polygon filled)
    
    SizeOpenGLScreen(width, height);                   // Setup the screen translations and viewport

}

///////////////////////////////// INIT GAME WINDOW \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////   This function initializes the game window.
/////
///////////////////////////////// INIT GAME WINDOW \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void Init(HWND hWnd)
{
    g_hWnd = hWnd;                                        // Assign the window handle to a global window handle
    GetClientRect(g_hWnd, &g_rRect);                   // Assign the windows rectangle to a global RECT
    InitializeOpenGL(g_rRect.right, g_rRect.bottom);   // Init OpenGL with the global rect

	g_Camera.PositionCamera(-120.0f, 250.0f, 90.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);

	ShowCursor(FALSE);
    // *Hint* We will put all our game/program init stuff here.
    // Some things include loading models, textures and network initialization.

	//Objects loading	
	ObjLoad ("shrub.3DS",'\0',                        0.0, 0.0, 0.0,		0,0,0,     1.0, 1.0, 4.0);
	ObjSetMaterial(&object[0],mat_ambient_grn, mat_diffuse_grn, mat_specular_grn, mat_shininess);

	ObjLoad ("grass-block.3DS",'\0',                    0.0, 0.0, 0.0,     0,0,0,      1.0,1.0,1.0);
	ObjSetMaterial(&object[1], mat_ambient_green, mat_diffuse_green, mat_specular_green, mat_shininess);
    
	initSolarSystem();
}

void initSolarSystem()
{
	planetEarth = CelestialBody(6371000.0f, 149598261.0f,
		0.01671123f, 365.256363004f);
}


void DrawString(std::string in)
{
	for (size_t i = 0; i < in.length(); i++)
	{
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, in[i]);
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


///////////////////////////////// RENDER SCENE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////   This function renders the entire scene.
/////
///////////////////////////////// RENDER SCENE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void RenderScene() 
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);   // Clear The Screen And The Depth Buffer
    glLoadIdentity();                                   // Reset The View
	g_Camera.Look();
	glMatrixMode(GL_MODELVIEW);

	
	//renderObjects();
	renderGrassField();
	renderUnitCube();
	renderShrubFence();


    SwapBuffers(g_hDC);                                 // Swap the backbuffers to the foreground
}

///////////////////////////////// DE-INIT \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////   This function cleans up and then posts a quit message to the window
/////
///////////////////////////////// DE-INIT \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void DeInit()
{
    if (g_hRC)                                         
    {
        wglMakeCurrent(NULL, NULL);                        // This frees our rendering memory and sets everything back to normal
        wglDeleteContext(g_hRC);                        // Delete our OpenGL Rendering Context 
    }
    
    if (g_hDC) 
        ReleaseDC(g_hWnd, g_hDC);                      // Release our HDC from memory

    UnregisterClass("GameTutorials", g_hInstance);     // Free the window class

	ShowCursor(TRUE);

    PostQuitMessage (0);                               // Post a QUIT message to the window
}


///////////////////////////////// WIN MAIN \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////   This function handles registering and creating the window.
/////
///////////////////////////////// WIN MAIN \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hprev, PSTR cmdline, int ishow)
{   
    HWND hWnd;
    
    // Create our window with our function we create that passes in the:
    // name, width, height, any flags for the window, if we want fullscreen of not, and the hInstance.
    hWnd = CreateMyWindow("www.GameTutorials.com - First OpenGL Program", SCREEN_WIDTH, SCREEN_HEIGHT, 0, g_bFullScreen, hInstance);

    // If we never got a valid window handle, quit the program
    if(hWnd == NULL) return TRUE;

    // INIT OpenGL
    Init(hWnd);                                                 

    // Run our message loop and after it's done, return the result
    return MainLoop();                     
}

///////////////////////////////// WIN PROC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////   This function handles the window messages.
/////
///////////////////////////////// WIN PROC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

LRESULT CALLBACK WinProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    LONG    lRet = 0; 
    PAINTSTRUCT    ps;

    switch (uMsg)
    { 
    case WM_SIZE:                                      // If the window is resized
        SizeOpenGLScreen(LOWORD(lParam),HIWORD(lParam));
        GetClientRect(hWnd, &g_rRect);             // Get the window rectangle
		break; 
 
    case WM_PAINT:                                     // If we need to repaint the scene
        BeginPaint(hWnd, &ps);                         // Init the paint struct        
        EndPaint(hWnd, &ps);                           // EndPaint, Clean up
        break;

    case WM_KEYDOWN:
        if (wParam == VK_ESCAPE)   DeInit();               // Quit if we pressed ESCAPE
        break;

    case WM_DESTROY:                                   // If the window is destroyed
        DeInit();                                       // Release memory and restore settings
        break; 
     
    default:                                            // Return by default
        lRet = DefWindowProc (hWnd, uMsg, wParam, lParam); 
        break; 
    } 
 
    return lRet;                                       // Return by default
}