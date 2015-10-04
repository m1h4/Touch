#pragma once

#pragma warning(push)
#pragma warning(disable:4201)

class Color
{
public:
	Color(void) {}
	Color(float a,float r,float g,float b) : a(a), r(r), g(g), b(b) {}
	Color(float r,float g,float b) : a(1.0f), r(r), g(g), b(b) {}
	Color(float a,float value) : a(a), r(value), g(value), b(value) {}
	Color(float value) : a(1.0f), r(value), g(value), b(value) {}
	Color(const Color& other) : a(other.a), r(other.r), g(other.g), b(other.b) {}
	Color(float a,const Color& other) : a(a), r(other.r), g(other.g), b(other.b) {}
	Color(const float* m) : a(m[0]), r(m[1]), g(m[2]), b(m[3]) {}

	void SetEmpty(void) { a = 0.0f; r = 0.0f; g = 0.0f; b = 0.0f; }
	bool IsEmpty(void) { return a == 0.0f && r == 0.0f && g == 0.0f && b == 0.0f; }

	bool operator==(const Color& other) const { return a == other.a && r == other.r && g == other.g && b == other.b; }
	bool operator!=(const Color& other) const { return a != other.a && r != other.r && g != other.g && b != other.b; }
	bool operator<(const Color& other) const { return a < other.a && r < other.r && g < other.g && b < other.b; }
	bool operator>(const Color& other) const { return a > other.a && r > other.r && g > other.g && b > other.b; }
	bool operator<=(const Color& other) const { return a <= other.a && r <= other.r && g <= other.g && b <= other.b; }
	bool operator>=(const Color& other) const { return a >= other.a && r >= other.r && g >= other.g && b >= other.b; }

	void Clamp(float mn = 0.0f,float mx = 1.0f) { a = min(max(a,mn),mx); r = min(max(r,mn),mx); g = min(max(g,mn),mx); b = min(max(b,mn),mx); }
	bool Clamped(float mn = 0.0f,float mx = 1.0f) const { return a <= mx && a >= mn && r <= mx && r >= mn && g <= mx && g >= mn && b <= mx && b >= mn; }

	void LinearInterp(const Color& dest,float s);
	void CubicInterp(const Color& dest,float s);
	void CosInterp(const Color& dest,float s);

	void Brighten(float s);
	void Darken(float s);
	void Saturation(float s);
	
	void Negative(void) { r = 1.0f - r; g = 1.0f - g; b = 1.0f - b; }
	void Grayscale(void) { float v = (r + g + b) / 3.0f; r = g = b = v; }
	void Shadow(void) { a *= 0.1f; r = g = b = 0.0f; }

	operator unsigned long(void) const { return (unsigned long)(((((unsigned long)((a)*255.0f))&0xFF)<<24)|((((unsigned long)((r)*255.0f))&0xFF)<<16)|((((unsigned long)((g)*255.0f))&0xFF)<<8)|(((unsigned long)((b)*255.0f))&0xFF)); }

	Color operator+(const Color& other) const { return Color(a+other.a,r+other.r,g+other.g,b+other.b); }
	Color operator-(const Color& other) const { return Color(a-other.a,r-other.r,g-other.g,b-other.b); }

	Color& operator+=(const Color& other) { a += other.a; r += other.r; g += other.g; b += other.b; return *this; }
	Color& operator-=(const Color& other) { a -= other.a; r -= other.r; g -= other.g; b -= other.b; return *this; }

	Color operator*(const float scalar) const { return Color(a*scalar,r*scalar,g*scalar,b*scalar); }
	Color operator/(const float scalar) const { return Color(a/scalar,r/scalar,g/scalar,b/scalar); }

	Color& operator*=(const float scalar) { a *= scalar; r *= scalar; g *= scalar; b *= scalar; return *this; }
	Color& operator/=(const float scalar) { a /= scalar; r /= scalar; g /= scalar; b /= scalar; return *this; }

	Color operator*(const Color& other) const { return Color(a * other.a,r * other.r,g * other.g,b * other.b); }

	Color& operator*=(const Color& other) { a *= other.a; r *= other.r; g *= other.g; b *= other.b; return *this; }

	operator float*(void) { return m; }
	operator const float*(void) const { return m; }

public:
	union 
	{
		struct 
		{
			float a;
			float r;
			float g;
			float b;
		};

		float m[4];
	};
};

#pragma warning(pop)