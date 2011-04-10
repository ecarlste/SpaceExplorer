#pragma once

#include "Vector.h"

#define PI 3.14159265

class CelestialBody
{
private:
	//physical characteristics
	float radius;

	// orbital characteristics
	float semiMajorAxis;
	float eccentricity;
	float orbitalPeriod;
	
	Vector::Vector3 position;

public:
	CelestialBody(void);
	CelestialBody(float radius, float semiMajorAxis, float eccentricity,
		float orbitalPeriod);
	~CelestialBody(void);
	
	void render() const;
	void updatePosition(float const& time);

	Vector::Vector3 const& getPosition() const;

};

