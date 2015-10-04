#pragma once

static const unsigned long FontMagic = MAKEFOURCC('f','d','t',' ');
static const unsigned long FontVersion = 0x00010004;
static const unsigned long FontChars = 127 - 32;

#include <pshpack1.h>

struct FontElement
{
	char code;

	long left;
	long top;

	long advancex;
	long advancey;

	unsigned short x;
	unsigned short y;
	unsigned short width;
	unsigned short height;
};

struct FontHeader
{
	unsigned long	magic;
	unsigned long	version;
	unsigned long	chars;
	unsigned long	height;
	float			uscale;
	unsigned long	sizex;
	unsigned long	sizey;
	unsigned long	tsize;
	unsigned long	ascent;
	unsigned long	descent;
	unsigned long	outmargt;
	unsigned long	outmargb;
	unsigned long	outmargl;
	unsigned long	outmargr;
	unsigned long	inmargt;
	unsigned long	inmargb;
	unsigned long	inmargl;
	unsigned long	inmargr;
};

#include <poppack.h>

class Font
{
public:
	Font(void) : mTextureID(NULL) {}
	~Font(void) {}

	bool LoadFromFile(LPCTSTR path);
	bool LoadFromResource(LPCTSTR id,LPCTSTR group);
	bool LoadFromMemory(const unsigned char* data,unsigned long size);
	void Unload(void);

	void DrawText(const char* string,unsigned int length,const Vector2& point,const Color& color);
	void DrawTextShadow(const char* string,unsigned int length,const Vector2& point,const Color& color) { 
		DrawText(string,length,point + Vector2(2.0f,1.0f),Color(color.a*0.1f,0.0f,0.0f,0.0f));
		DrawText(string,length,point + Vector2(0.0f,1.0f),Color(color.a*0.1f,0.0f,0.0f,0.0f));
		DrawText(string,length,point + Vector2(1.0f,2.0f),Color(color.a*0.1f,0.0f,0.0f,0.0f));
		DrawText(string,length,point + Vector2(1.0f,0.0f),Color(color.a*0.1f,0.0f,0.0f,0.0f));
		DrawText(string,length,point + Vector2(1.0f,1.0f),Color(color.a*0.1f,0.0f,0.0f,0.0f));
		DrawText(string,length,point,color); }
	void DrawTextOutline(const char* string,unsigned int length,const Vector2& point,const Color& color) { 
		//DrawText(string,length,point + Vector2(1.0f,0.0f),Color(color.a,0.0f,0.0f,0.0f));
		//DrawText(string,length,point + Vector2(0.0f,1.0f),Color(color.a,0.0f,0.0f,0.0f));
		//DrawText(string,length,point + Vector2(-1.0f,0.0f),Color(color.a,0.0f,0.0f,0.0f));
		//DrawText(string,length,point + Vector2(0.0f,-1.0f),Color(color.a,0.0f,0.0f,0.0f));

		DrawText(string,length,point + Vector2(1.0f,-1.0f),Color(color.a,0.0f,0.0f,0.0f));
		DrawText(string,length,point + Vector2(-1.0f,-1.0f),Color(color.a,0.0f,0.0f,0.0f));
		DrawText(string,length,point + Vector2(1.0f,1.0f),Color(color.a,0.0f,0.0f,0.0f));
		DrawText(string,length,point + Vector2(-1.0f,1.0f),Color(color.a,0.0f,0.0f,0.0f));
		DrawText(string,length,point,color); }

	void GetTextSize(const char* string,unsigned int length,Vector2* size);

public:
	GLuint			mTextureID;

	unsigned long	mTextureWidth;
	unsigned long	mTextureHeight;

	FontHeader		mHeader;
	FontElement		mElements[FontChars];
};