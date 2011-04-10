#pragma once

namespace Vector
{
	class Vector3
	{
	public:
	
		Vector3();
		Vector3(float X, float Y, float Z); 
		Vector3 operator+(Vector3 vVector);
		Vector3 operator-(Vector3 vVector);
		Vector3 operator*(float num);
		Vector3 operator/(float num);

		float x, y, z;						
	};
}

