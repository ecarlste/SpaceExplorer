#include "CelestialBody.h"
#include <math.h>
#include <Windows.h>
#include <GL\GL.h>
#include <GL\glut.h>


CelestialBody::CelestialBody(void)
{}


CelestialBody::CelestialBody(float radius, float semiMajorAxis,
	float eccentricity, float orbitalPeriod) : radius(radius),
	semiMajorAxis(semiMajorAxis), eccentricity(eccentricity),
	orbitalPeriod(orbitalPeriod)
{
}


CelestialBody::~CelestialBody(void)
{
}


void CelestialBody::render() const
{
	glPushMatrix();
	glTranslatef(position.x, position.y, 0.0f);
	//glScalef(radius, radius, radius);
	glutWireSphere(radius, 16, 16);
	glPopMatrix();
}


void CelestialBody::updatePosition(float const& time)
{
	// compute the mean anomaly
	float M = 2 * PI * time / orbitalPeriod;

	// approximate the eccentric anomaly E by solving Kepler's equation to
	// within a tolerane of 10^-5. right now i'm using a very simple way to
	// solve this respectively just because I wanted to get this working
	// ideally we should use newton's method or some other solver for solving
	// converging series to find a more accurate solution in only 2 iterations
	// when I tried putting this current rudimentary solver at 10^-9 it would
	// not converge on a solution for some values of M
	float tolerance = 0.00001;
	float E;
	float E1 = M;
	bool done = false;
	
	while (!done)
	{
		E = M + eccentricity * sin(E1);

		if (abs(E - E1) < tolerance)
			done = true;

		E1 = E;
	}

	E = fmod(E , 2.0f * (float) PI);

	// compute the true anomaly theta
	float theta = atan(sqrt(1.0f + eccentricity / 1.0f - eccentricity)
		* tan(E/2.0f)) * 2.0f;

	// compute the heliocentric distance r from kepler's first law
	float r = semiMajorAxis * (1.0f - pow(eccentricity, 2.0f))
		/ 1.0f + eccentricity * cos(theta);
	
	position.x = r * cos(theta);
	position.y = r * sin(theta);
	position.z = 0;
}


Vector::Vector3 const& CelestialBody::getPosition() const
{ return position; }

