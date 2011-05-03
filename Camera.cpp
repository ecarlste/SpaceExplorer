
#include "spaceexplorer.h"
#include "Camera.h"

// This is how fast our camera moves (Sped up a bit due to normalizing our vectors)
#define kSpeed	0.2f	
#define UP		1
#define DOWN	-1

#define _USE_MATH_DEFINES
#include <cmath>

/////////////////////////////////////// CROSS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////	This returns a perpendicular vector from 2 given vectors by taking the cross product.
/////
/////////////////////////////////////// CROSS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
												
Vector::Vector3 Cross(Vector::Vector3 vVector1, Vector::Vector3 vVector2)
{
	Vector::Vector3 vNormal;	

	// Calculate the cross product with the non communitive equation
	vNormal.x = ((vVector1.y * vVector2.z) - (vVector1.z * vVector2.y));
	vNormal.y = ((vVector1.z * vVector2.x) - (vVector1.x * vVector2.z));
	vNormal.z = ((vVector1.x * vVector2.y) - (vVector1.y * vVector2.x));

	// Return the cross product
	return vNormal;										 
}


/////////////////////////////////////// MAGNITUDE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////	This returns the magnitude of a vector
/////
/////////////////////////////////////// MAGNITUDE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

float Magnitude(Vector::Vector3 vNormal)
{
	// Here is the equation:  magnitude = sqrt(V.x^2 + V.y^2 + V.z^2) : Where V is the vector
	return (float)sqrt( (vNormal.x * vNormal.x) + 
						(vNormal.y * vNormal.y) + 
						(vNormal.z * vNormal.z) );
}


/////////////////////////////////////// NORMALIZE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////	This returns a normalize vector (A vector exactly of length 1)
/////
/////////////////////////////////////// NORMALIZE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

Vector::Vector3 Normalize(Vector::Vector3 vVector)
{
	// Get the magnitude of our normal
	float magnitude = Magnitude(vVector);				

	// Now that we have the magnitude, we can divide our vector by that magnitude.
	// That will make our vector a total length of 1.  
	vVector = vVector / magnitude;		
	
	// Finally, return our normalized vector
	return vVector;										
}


///////////////////////////////// CCAMERA \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////	This is the class constructor
/////
///////////////////////////////// CCAMERA \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

CCamera::CCamera()
{
	Vector::Vector3 vZero = Vector::Vector3(0.0, 0.0, 0.0);		// Init a vVector to 0 0 0 for our position
	Vector::Vector3 vView = Vector::Vector3(0.0, 1.0, 0.5);		// Init a starting view vVector (looking up and out the screen) 
	Vector::Vector3 vUp   = Vector::Vector3(0.0, 0.0, 1.0);		// Init a standard up vVector (Rarely ever changes)

	m_vPosition	= vZero;					// Init the position to zero
	m_vView		= vView;					// Init the view to a std starting view
	m_vUpVector	= vUp;						// Init the UpVector
}


///////////////////////////////// POSITION CAMERA \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////	This function sets the camera's position and view and up vVector.
/////
///////////////////////////////// POSITION CAMERA \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void CCamera::PositionCamera(float positionX, float positionY, float positionZ,
				  		     float viewX,     float viewY,     float viewZ,
							 float upVectorX, float upVectorY, float upVectorZ)
{
	Vector::Vector3 vPosition	= Vector::Vector3(positionX, positionY, positionZ);
	Vector::Vector3 vView		= Vector::Vector3(viewX, viewY, viewZ);
	Vector::Vector3 vUpVector	= Vector::Vector3(upVectorX, upVectorY, upVectorZ);

	// The code above just makes it cleaner to set the variables.
	// Otherwise we would have to set each variable x y and z.

	m_vPosition = vPosition;					// Assign the position
	m_vView     = vView;						// Assign the view
	m_vUpVector = vUpVector;					// Assign the up vector
}


///////////////////////////////// SET VIEW BY MOUSE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////	This allows us to look around using the mouse, like in most first person games.
/////
///////////////////////////////// SET VIEW BY MOUSE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void CCamera::SetViewByMouse()
{
	POINT mousePos;									// This is a window structure that holds an X and Y
	int middleX = SCREEN_WIDTH  >> 1;				// This is a binary shift to get half the width
	int middleY = SCREEN_HEIGHT >> 1;				// This is a binary shift to get half the height
	float angleY = 0.0f;							// This is the direction for looking up or down
	float angleZ = 0.0f;							// This will be the value we need to rotate around the Y axis (Left and Right)
	static float currentRotX = 0.0f;
	
	// Get the mouse's current X,Y position
	GetCursorPos(&mousePos);						
	
	// If our cursor is still in the middle, we never moved... so don't update the screen
	if( (mousePos.x == middleX) && (mousePos.y == middleY) ) return;

	// Set the mouse position to the middle of our window
	SetCursorPos(middleX, middleY);							

	// Get the direction the mouse moved in, but bring the number down to a reasonable amount
	angleY = (float)( (middleX - mousePos.x) ) / 1000.0f;		
	angleZ = (float)( (middleY - mousePos.y) ) / 1000.0f;		

	static float lastRotX = 0.0f; 
 	lastRotX = currentRotX; // We store off the currentRotX and will use it in when the angle is capped
	
	// Here we keep track of the current rotation (for up and down) so that
	// we can restrict the camera from doing a full 360 loop.
	currentRotX += angleZ;
 
	// If the current rotation (in radians) is greater than 1.0, we want to cap it.
	if(currentRotX > 1.0f)     
	{
		currentRotX = 1.0f;
		
		// Rotate by remaining angle if there is any
		if(lastRotX != 1.0f) 
		{
			// To find the axis we need to rotate around for up and down
			// movements, we need to get a perpendicular vector from the
			// camera's view vector and up vector.  This will be the axis.
			// Before using the axis, it's a good idea to normalize it first.
			Vector::Vector3 vAxis = Cross(m_vView - m_vPosition, m_vUpVector);
			vAxis = Normalize(vAxis);
				
			// rotate the camera by the remaining angle (1.0f - lastRotX)
			RotateView( 1.0f - lastRotX, vAxis.x, vAxis.y, vAxis.z);
		}
	}
	// Check if the rotation is below -1.0, if so we want to make sure it doesn't continue
	else if(currentRotX < -1.0f)
	{
		currentRotX = -1.0f;
		
		// Rotate by the remaining angle if there is any
		if(lastRotX != -1.0f)
		{
			// To find the axis we need to rotate around for up and down
			// movements, we need to get a perpendicular vector from the
			// camera's view vector and up vector.  This will be the axis.
			// Before using the axis, it's a good idea to normalize it first.
			Vector::Vector3 vAxis = Cross(m_vView - m_vPosition, m_vUpVector);
			vAxis = Normalize(vAxis);
			
			// rotate the camera by ( -1.0f - lastRotX)
			RotateView( -1.0f - lastRotX, vAxis.x, vAxis.y, vAxis.z);
		}
	}
	// Otherwise, we can rotate the view around our position
	else 
	{	
		// To find the axis we need to rotate around for up and down
		// movements, we need to get a perpendicular vector from the
		// camera's view vector and up vector.  This will be the axis.
		// Before using the axis, it's a good idea to normalize it first.
		Vector::Vector3 vAxis = Cross(m_vView - m_vPosition, m_vUpVector);
		vAxis = Normalize(vAxis);
	
		// Rotate around our perpendicular axis
		RotateView(angleZ, vAxis.x, vAxis.y, vAxis.z);
	}

	// Always rotate the camera around the y-axis
	RotateView(angleY, 0, 1, 0);
}


///////////////////////////////// ROTATE AROUND POINT \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////	This rotates the position around a given point
/////
///////////////////////////////// ROTATE AROUND POINT \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void CCamera::RotateAroundPoint(Vector::Vector3 vCenter, float angle, float X, float Y, float Z)
{
	Vector::Vector3 vNewPosition;			

	// To rotate our position around a point, what we need to do is find
	// a vector from our position to the center point we will be rotating around.
	// Once we get this vector, then we rotate it along the specified axis with
	// the specified degree.  Finally the new vector is added center point that we
	// rotated around (vCenter) to become our new position.  That's all it takes.

	// Get the vVector from our position to the center we are rotating around
	Vector::Vector3 vPos = m_vPosition - vCenter;

	// Calculate the sine and cosine of the angle once
	float cosTheta = (float)cos(angle);
	float sinTheta = (float)sin(angle);

	// Find the new x position for the new rotated point
	vNewPosition.x  = (cosTheta + (1 - cosTheta) * X * X)		* vPos.x;
	vNewPosition.x += ((1 - cosTheta) * X * Y - Z * sinTheta)	* vPos.y;
	vNewPosition.x += ((1 - cosTheta) * X * Z + Y * sinTheta)	* vPos.z;

	// Find the new y position for the new rotated point
	vNewPosition.y  = ((1 - cosTheta) * X * Y + Z * sinTheta)	* vPos.x;
	vNewPosition.y += (cosTheta + (1 - cosTheta) * Y * Y)		* vPos.y;
	vNewPosition.y += ((1 - cosTheta) * Y * Z - X * sinTheta)	* vPos.z;

	// Find the new z position for the new rotated point
	vNewPosition.z  = ((1 - cosTheta) * X * Z - Y * sinTheta)	* vPos.x;
	vNewPosition.z += ((1 - cosTheta) * Y * Z + X * sinTheta)	* vPos.y;
	vNewPosition.z += (cosTheta + (1 - cosTheta) * Z * Z)		* vPos.z;

	// Now we just add the newly rotated vector to our position to set
	// our new rotated position of our camera.
	m_vPosition = vCenter + vNewPosition;
}


///////////////////////////////// ROTATE VIEW \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////	This rotates the view around the position using an axis-angle rotation
/////
///////////////////////////////// ROTATE VIEW \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void CCamera::RotateView(float angle, float x, float y, float z)
{
	Vector::Vector3 vNewView;

	// Get the view vector (The direction we are facing)
	Vector::Vector3 vView = m_vView - m_vPosition;		

	// Calculate the sine and cosine of the angle once
	float cosTheta = (float)cos(angle);
	float sinTheta = (float)sin(angle);

	// Find the new x position for the new rotated point
	vNewView.x  = (cosTheta + (1 - cosTheta) * x * x)		* vView.x;
	vNewView.x += ((1 - cosTheta) * x * y - z * sinTheta)	* vView.y;
	vNewView.x += ((1 - cosTheta) * x * z + y * sinTheta)	* vView.z;

	// Find the new y position for the new rotated point
	vNewView.y  = ((1 - cosTheta) * x * y + z * sinTheta)	* vView.x;
	vNewView.y += (cosTheta + (1 - cosTheta) * y * y)		* vView.y;
	vNewView.y += ((1 - cosTheta) * y * z - x * sinTheta)	* vView.z;

	// Find the new z position for the new rotated point
	vNewView.z  = ((1 - cosTheta) * x * z - y * sinTheta)	* vView.x;
	vNewView.z += ((1 - cosTheta) * y * z + x * sinTheta)	* vView.y;
	vNewView.z += (cosTheta + (1 - cosTheta) * z * z)		* vView.z;

	// Now we just add the newly rotated vector to our position to set
	// our new rotated view of our camera.
	m_vView = m_vPosition + vNewView;
}


/////// * /////////// * /////////// * NEW * /////// * /////////// * /////////// *

///////////////////////////////// STRAFE CAMERA \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////	This strafes the camera left and right depending on the speed (-/+)
/////
///////////////////////////////// STRAFE CAMERA \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void CCamera::StrafeCamera(float speed)
{
	// Strafing is quite simple if you understand what the cross product is.
	// If you have 2 vectors (say the up vVector and the view vVector) you can
	// use the cross product formula to get a vVector that is 90 degrees from the 2 vectors.
	// For a better explanation on how this works, check out the OpenGL "Normals" tutorial.
	// In our new Update() function, we set the strafing vector (m_vStrafe).  Due
	// to the fact that we need this vector for many things including the strafing
	// movement and camera rotation (up and down), we just calculate it once.
	//
	// Like our MoveCamera() function, we add the strafing vector to our current position 
	// and view.  It's as simple as that.  It has already been calculated in Update().
	
	// Add the strafe vector to our position
	m_vPosition.x += m_vStrafe.x * speed;
	m_vPosition.z += m_vStrafe.z * speed;

	// Add the strafe vector to our view
	m_vView.x += m_vStrafe.x * speed;
	m_vView.z += m_vStrafe.z * speed;
}


///////////////////////////////// MOVE CAMERA \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////	This will move the camera forward or backward depending on the speed
/////
///////////////////////////////// MOVE CAMERA \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void CCamera::MoveCamera(float speed)
{
	// Get the current view vector (the direction we are looking)
	Vector::Vector3 vVector = m_vView - m_vPosition;


	// I snuck this change in here!  We now normalize our view vector when
	// moving throughout the world.  This is a MUST that needs to be done.
	// That way you don't move faster than you strafe, since the strafe vector
	// is normalized too.
	vVector = Normalize(vVector);
	
	m_vPosition.x += vVector.x * speed;		// Add our acceleration to our position's X
	m_vPosition.z += vVector.z * speed;		// Add our acceleration to our position's Z
	m_vView.x += vVector.x * speed;			// Add our acceleration to our view's X
	m_vView.z += vVector.z * speed;			// Add our acceleration to our view's Z

	float newY = m_vPosition.y + vVector.y * speed;

	if (newY > 2.0f)
		m_vView.y += vVector.y * speed;
	else
		newY = 2.0f;

	m_vPosition.y = newY;
}


// The next 3 functions were added to our camera class.  The less code in 
// Main.cpp the better.

//////////////////////////// CHECK FOR MOVEMENT \\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////	This function handles the input faster than in the WinProc()
/////
//////////////////////////// CHECK FOR MOVEMENT \\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void CCamera::CheckForMovement()
{
	// Check if we hit the Up arrow or the 'w' key
	if(GetKeyState(VK_UP) & 0x80 || GetKeyState('W') & 0x80) {				

		// Move our camera forward by a positive SPEED
		MoveCamera(kSpeed);
		//RotateAroundPoint( Vector::Vector3(0.0f, 0.0f, 0.0f), 0.01f, 1.0f, 0.0f, 0.0f);
	}

	// Check if we hit the Down arrow or the 's' key
	if(GetKeyState(VK_DOWN) & 0x80 || GetKeyState('S') & 0x80) {			

		// Move our camera backward by a negative SPEED
		MoveCamera(-kSpeed);				
		//RotateAroundPoint( Vector::Vector3(0.0f, 0.0f, 0.0f), -0.01f, 1.0f, 0.0f, 0.0f);
	}

	// Check if we hit the Left arrow or the 'a' key
	if(GetKeyState(VK_LEFT) & 0x80 || GetKeyState('A') & 0x80) {			

		// Strafe the camera left
		StrafeCamera(-kSpeed);
	}

	// Check if we hit the Right arrow or the 'd' key
	if(GetKeyState(VK_RIGHT) & 0x80 || GetKeyState('D') & 0x80) {			

		// Strafe the camera right
		StrafeCamera(kSpeed);
	}	
}


void CCamera::CheckForCameraRotation()
{
	// Check if we hit the Up arrow
	if(GetKeyState(VK_UP) & 0x80)
	{	
		if (isRotatable(UP))
		{
			Vector::Vector3 vAxis = Cross(m_vView - m_vPosition, m_vUpVector);
			vAxis = Normalize(vAxis);
			RotateAroundPoint( Vector::Vector3(0, 0, 0), -0.01f, vAxis.x, vAxis.y, vAxis.z);	
		}
		//RotateAroundPoint( Vector::Vector3(0.0f, 0.0f, 0.0f), 0.01f, 1, 0, 0);
	}

	// Check if we hit the Down arrow
	if(GetKeyState(VK_DOWN) & 0x80)
	{				
		if (isRotatable(DOWN))
		{
			Vector::Vector3 vAxis = Cross(m_vView - m_vPosition, m_vUpVector);
			vAxis = Normalize(vAxis);
			RotateAroundPoint( Vector::Vector3(0.0f, 0.0f, 0.0f), 0.01f, vAxis.x, vAxis.y, vAxis.z);
		}
		//RotateAroundPoint( Vector::Vector3(0.0f, 0.0f, 0.0f), -0.01f, 1, 0, 0);
	}

	// Check if we hit the Left arrow
	if(GetKeyState(VK_LEFT) & 0x80) {			
		RotateAroundPoint( Vector::Vector3(0.0f, m_vPosition.y, 0.0f), -0.01f, 0.0f, 1.0f, 0.0f);
	}

	// Check if we hit the Right arrow
	if(GetKeyState(VK_RIGHT) & 0x80) {			
		RotateAroundPoint( Vector::Vector3(0.0f, m_vPosition.y, 0.0f), 0.01f, 0.0f, 1.0f, 0.0f);
	}	
}


///////////////////////////////// UPDATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////	This updates the camera's view and strafe vector
/////
///////////////////////////////// UPDATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void CCamera::Update() 
{
	// Below we calculate the strafe vector every time we update
	// the camera.  This is because many functions use it so we might
	// as well calculate it only once.  

	// Initialize a variable for the cross product result
	Vector::Vector3 vCross = Cross(m_vView - m_vPosition, m_vUpVector);

	// Normalize the strafe vector
	m_vStrafe = Normalize(vCross);

	// Move the camera's view by the mouse
	SetViewByMouse();

	// This checks to see if the keyboard was pressed
	CheckForMovement();

	// CheckForCameraRotation();
}


///////////////////////////////// LOOK \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////	This updates the camera according to the 
/////
///////////////////////////////// LOOK \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void CCamera::Look()
{
	// Give openGL our camera position, then camera view, then camera up vector
	gluLookAt(m_vPosition.x, m_vPosition.y, m_vPosition.z,	
			  m_vView.x,	 m_vView.y,     m_vView.z,	
			  m_vUpVector.x, m_vUpVector.y, m_vUpVector.z);
}


bool CCamera::isRotatable(int direction)
{
	// determine the angle between the (m_vView - m_vPosition) vector and the
	// y-axis

	float opp = sqrt(m_vPosition.x * m_vPosition.x + m_vPosition.z * m_vPosition.z);
	float theta = atan( opp/m_vPosition.y );

	if (direction == UP && theta >= .02)
		return true;
	else if (direction == DOWN && (theta <= (PI / 2) - .02))
		return true;
	else
		return false;
}


/////////////////////////////////////////////////////////////////////////////////
//
// * QUICK NOTES * 
//
// Now that we have all the basic camera functionality we can use this file in future 
// projects and not have to cut and paste code.  I created a camera.h for this purpose as well.
//
// We added strafing to this file.  3 other functions were added to our camera class
// so that it would clean up main.cpp a bit more.  In Camera.h you will find some
// new changes as well, which include the move of our camera data to private.  We now
// will use data access functions to get camera data.
//
// Here are the notes from main.cpp about the concept and application:
//
// This tutorial was taken from the Camera4 tutorial.  This important tutorial
// shows how to strafe the camera right or left.  This might seem easy when you think to
// attempt it, but if you don't know your linear algebra it can be tricky.  Strafing the
// camera is moving the camera 90 degrees left or right from the current view.  In other
// words, it's as if you were side stepping while you look forward.  This is used in most
// first person shooters games, and comes in handy when peering around corners or running
// past a hallway while firing off some rounds.  It's also a great way to move diagonal
// while doing a shootout at close ranges to avoid being hit.
//
// Since we understand what strafing is and what it does, let me actually explain how it works.
// We know that we want to walk in the direction that is 90 degrees from the view vVector (with
// the view vVector being m_vView - m_vPosition).  So how do we then get the vVector that is 90
// degrees from our view vVector?  If you know what the cross product is, you can easily see how
// this would be done.  The cross product is a mathematical formula that takes 2 vectors and
// returns the vVector 90 degrees from those 2 vectors.  This is how you find the normal to a plane.
// Well, we have a view vVector, but what would the other vVector be?  Does the up vVector come to mind?
// That's it!  We want to take the cross product between the up vVector and the view vVector.  This
// will return the vVector (or direction) that we want to strafe in.  In games like Descent, the
// up vVector will change because you can go upside down and spin in crazy directions.  The cross
// product will ensure that we will always strafe correctly no matter what orientation the camera is in.
// Once we have the strafe vVector, we need to add it to the position and view points.
// Here are the controls for this tutorial:
//
// w, s, UP_ARROW, DOWN_ARROW - Move the camera forward and backward
// a, d, RIGHT_ARROW, LEFT_ARROW - Strafe the camera left and right
// Mouse movement - Moves the view for first person mode
// ESCAPE - Quits the program
//
// You may notice that we don't use all the methods in the camera class.  I decided to leave
// them in there because it would be nice to just use this in future projects without having to 
// cut and paste code.  That's also why I finally added a camera.h so you don't have the camera 
// class in main.h.
// 
// Enjoy!
//
//
// � 2000-2005 GameTutorials
