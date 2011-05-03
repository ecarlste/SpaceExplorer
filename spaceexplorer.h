
#include <Windows.h>
#include <string>
#include <sstream>
#include <math.h>
#include <GL/glut.h>
#include "load_3ds.h"
#include "load_bmp.h"
#include "object.h"
#include "CelestialBody.h"
#include "Camera.h"

#define SCREEN_WIDTH 1280								// We want our screen width 800 pixels
#define SCREEN_HEIGHT 720								// We want our screen height 600 pixels
#define SCREEN_DEPTH 16									// We want 16 bits per pixel

// This is our MAIN() for windows
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hprev, PSTR cmdline, int ishow);

// The window proc which handles all of window's messages.
LRESULT CALLBACK WinProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

// This controls our main program loop
WPARAM MainLoop();

// This changes the screen to full screen mode
void ChangeToFullScreen();

// This is our own function that makes creating a window modular and easy
HWND CreateMyWindow(LPSTR strWindowName, int width, int height, DWORD dwStyle, bool bFullScreen, HINSTANCE hInstance);

// This allows us to configure our window for OpenGL and backbuffering
bool bSetupPixelFormat(HDC hdc);

// This inits our screen translations and projections
void SizeOpenGLScreen(int width, int height);

// This sets up OpenGL
void InitializeOpenGL(int width, int height);

// This initializes the whole program
void Init(HWND hWnd);

// This draws everything to the screen
void RenderScene();

// This frees all our memory in our program 
void DeInit();

// This method initializes all the celestial body objects in the solar system
void initSolarSystem();
