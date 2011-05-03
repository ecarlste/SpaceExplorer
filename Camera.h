#pragma once

#include "Vector.h"

// This was created to allow us to use this camera code in other projects
// without having to cut and paste code.  This file and camera.cpp could be
// added to a future project.

// This is our camera class
class CCamera {

public:

	// Our camera constructor
	CCamera();	

	// These are are data access functions for our camera's private data
	Vector::Vector3 Position() {	return m_vPosition;		}
	Vector::Vector3 View()		{	return m_vView;			}
	Vector::Vector3 UpVector() {	return m_vUpVector;		}
	Vector::Vector3 Strafe()	{	return m_vStrafe;		}

	// This changes the position, view, and up vector of the camera.
	// This is primarily used for initialization
	void PositionCamera(float positionX, float positionY, float positionZ,
			 		    float viewX,     float viewY,     float viewZ,
						float upVectorX, float upVectorY, float upVectorZ);

	// This rotates the camera's view around the position depending on the values passed in.
	void RotateView(float angle, float X, float Y, float Z);

	// This moves the camera's view by the mouse movements (First person view)
	void SetViewByMouse(); 

	// This rotates the camera around a point (I.E. your character).
	void RotateAroundPoint(Vector::Vector3 vCenter, float angle, float X, float Y, float Z);

	// This strafes the camera left or right depending on the speed (+/-) 
	void StrafeCamera(float speed);

	// This will move the camera forward or backward depending on the speed
	void MoveCamera(float speed);

	// This checks for keyboard movement
	void CheckForMovement();

	// This checks for camera rotation
	void CheckForCameraRotation();

	// This updates the camera's view and other data (Should be called each frame)
	void Update();

	// This uses gluLookAt() to tell OpenGL where to look
	void Look();

private:

	// The camera's position
	Vector::Vector3 m_vPosition;					

	// The camera's view
	Vector::Vector3 m_vView;						

	// The camera's up vector
	Vector::Vector3 m_vUpVector;		
	
	// The camera's strafe vector
	Vector::Vector3 m_vStrafe;

	bool isRotatable(int);

};