#include "Vector.h"

Vector::Vector3::Vector3() {}

// This is our constructor that allows us to initialize our data upon creating an instance
Vector::Vector3::Vector3(float X, float Y, float Z) 
{ 
	x = X; y = Y; z = Z;
}

// Here we overload the + operator so we can add vectors together 
Vector::Vector3 Vector::Vector3::operator+(Vector3 vVector)
{
	// Return the added vectors result.
	return Vector3(vVector.x + x, vVector.y + y, vVector.z + z);
}

// Here we overload the - operator so we can subtract vectors 
Vector::Vector3 Vector::Vector3::operator-(Vector3 vVector)
{
	// Return the subtracted vectors result
	return Vector3(x - vVector.x, y - vVector.y, z - vVector.z);
}
	
// Here we overload the * operator so we can multiply by scalars
Vector::Vector3 Vector::Vector3::operator*(float num)
{
	// Return the scaled vector
	return Vector3(x * num, y * num, z * num);
}

// Here we overload the / operator so we can divide by a scalar
Vector::Vector3 Vector::Vector3::operator/(float num)
{
	// Return the scale vector
	return Vector3(x / num, y / num, z / num);
}

