#pragma once

#pragma warning(push)
#pragma warning(disable:4201)

class AABB2
{
public:
	AABB2(void) {}
	AABB2(float width,float height) : minx(-width / 2.0f), miny(-height / 2.0f), maxx(width / 2.0f), maxy(height / 2.0f) {}
	AABB2(const Vector2& tl,const Vector2& br) : minx(tl.x), miny(tl.y), maxx(br.x), maxy(br.y) {}
	AABB2(float minx,float miny,float maxx,float maxy) : minx(minx), miny(miny), maxx(maxx), maxy(maxy) {}
	AABB2(const AABB2& other) : minx(other.minx), miny(other.miny), maxx(other.maxx), maxy(other.maxy) {}

	void SetEmpty(void) { minx = miny = maxx = maxy = 0.0f; }
	bool IsEmpty(void) const { return minx == 0.0f && miny == 0.0f && maxx == 0.0f && maxy == 0.0f; }

	bool PointIn(const Vector2& point) const { return point.x >= minx && point.y <= maxx && point.y >= miny && point.y <= maxy; }
	bool Overlap(const AABB2& other) const { return minx <= other.maxx && maxx >= other.minx && miny <= other.maxy && maxy >= other.miny; }

	AABB2 operator + (const Vector2& other) const { return AABB2(minx + other.x,miny + other.y,maxx + other.x,maxy + other.y); }
	AABB2 operator - (const Vector2& other) const { return AABB2(minx - other.x,miny - other.y,maxx - other.x,maxy - other.y); }

	AABB2& operator += (const Vector2& other) { minx += other.x; miny += other.y; maxx += other.x; maxy += other.y;	return *this; }
	AABB2& operator -= (const Vector2& other) { minx -= other.x; miny -= other.y; maxx -= other.x; maxy -= other.y;	return *this; }

	bool operator == (const AABB2& other) const { return minx == other.minx && miny == other.miny && maxx == other.maxx && maxy == other.maxx; }
	bool operator != (const AABB2& other) const { return minx != other.minx && miny != other.miny && maxx != other.maxx && maxy != other.maxx; }

	operator float*(void) { return m; }
	operator const float*(void) const { return m; }

public:
	union 
	{
        struct 
		{
        	float minx;
			float miny;
			float maxx;
			float maxy;
        };

        float m[4];
    };
};

#pragma warning(pop)