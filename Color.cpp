#include "Globals.h"
#include "Color.h"
#include "Interpolations.h"

void Color::LinearInterp(const Color& dest,float s)
{
	a = LinearInterpolation(a,dest.a,s);
	r = LinearInterpolation(r,dest.r,s);
	g = LinearInterpolation(g,dest.g,s);
	b = LinearInterpolation(b,dest.b,s);
}

void Color::CubicInterp(const Color& dest,float s)
{
	a = CubicInterpolation(a,dest.a,s);
	r = CubicInterpolation(r,dest.r,s);
	g = CubicInterpolation(g,dest.g,s);
	b = CubicInterpolation(b,dest.b,s);
}

void Color::CosInterp(const Color& dest,float s)
{
	a = CosineInterpolation(a,dest.a,s);
	r = CosineInterpolation(r,dest.r,s);
	g = CosineInterpolation(g,dest.g,s);
	b = CosineInterpolation(b,dest.b,s);
}

void Color::Brighten(float s)
{
	LinearInterp(Color(a,1.0f,1.0f,1.0f),s);
}

void Color::Darken(float s)
{
	LinearInterp(Color(a,0.0f,0.0f,0.0f),s);
}

void Color::Saturation(float s)
{
	float v = (r + g + b) / 3.0f;
	
	LinearInterp(Color(a,v,v,v),s);
}