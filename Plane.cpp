#include "Globals.h"
#include "Plane.h"

float Plane2::GetLength(void) const
{
	return sqrtf(a*a+b*b);
}

float Plane2::Normalize(void)
{
	float length = GetLength();

	if(length == 0.0f)
		return 0.0f;
	
	a /= length;
	b /= length;
	c /= length;

	return length;
}