#pragma once

#pragma warning(push)
#pragma warning(disable:4201)

class Plane2
{
public:
	Plane2(void) {}
	Plane2(float a,float b,float c) : a(a), b(b), c(c) {}
	Plane2(const Vector2& normal,const Vector2& p) { a = normal.x; b = normal.y; c = - a*p.x - b*p.y; }
	Plane2(const Vector2 (&p)[2]) { a = p[0].y - p[1].y; b = p[1].x - p[0].x; c = - a*p[0].x - b*p[0].y; }
	Plane2(const Plane2& other) : a(other.a), b(other.b), c(other.c) {}
	
	void SetEmpty(void) { a = b = c = 0.0f; }
	bool IsEmpty(void) const { return a == 0.0f && b == 0.0f && c == 0.0f; }

	float GetLength(void) const;
	float GetLengthSqr(void) const { return a * a + b * b; }

	float Normalize(void);
	bool IsNormalized(void) const { return GetLengthSqr() == 1.0f; }

	float operator * (const Vector2& other) const { return a * other.x + b * other.y + c; }	// Dot product

public:
	union 
	{
        struct 
		{
        	float a;
			float b;
			float c;
        };

        float m[3];
    };
};

Plane2 Plane2Normalize(const Plane2& plane)
{
	Plane2 out(plane);

	out.Normalize();

	return out;
}

#pragma warning(pop)