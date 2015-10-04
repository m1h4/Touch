#include "Globals.h"
#include "Image.h"
#include "Targa.h"

Image::Image(void)
{
}

Image::~Image(void)
{
}

bool Image::LoadFromFile(LPCTSTR path)
{
	HANDLE hFile = CreateFile(path,GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,NULL,NULL);
	if(hFile == INVALID_HANDLE_VALUE)
		return false;

	unsigned char* buffer = new unsigned char[GetFileSize(hFile,NULL)];
	if(!buffer)
		return false;

	unsigned long read;

	ReadFile(hFile,buffer,GetFileSize(hFile,NULL),&read,NULL);

	if(!LoadFromMemory(buffer,read))
	{
		delete buffer;
		return false;
	}

	delete buffer;
	return true;
}

bool Image::LoadFromResource(LPCTSTR id,LPCTSTR group)
{
	HRSRC resource = FindResource(NULL,id,group);
	if(!resource)
		return false;

	HGLOBAL global = LoadResource(NULL,resource);
	if(!global)
		return false;

	LPBYTE data = (LPBYTE)LockResource(global);
	if(!data)
		return false;

	if(!LoadFromMemory(data,SizeofResource(NULL,resource)))
	{
		UnlockResource(global);
		FreeResource(global);
		return false;
	}

	UnlockResource(global);
	FreeResource(global);
	return true;
}

bool Image::LoadFromMemory(const unsigned char* data,unsigned long size)
{
	Targa tga;

	if(!tga.LoadFromMemory(data,size))
		return false;

	mTextureWidth = tga.GetWidth();
	mTextureHeight = tga.GetHeight();

	glGenTextures(1,&mTextureID);
	if(glGetError() != GL_NO_ERROR)
		return false;

	glBindTexture(GL_TEXTURE_2D,mTextureID);
	if(glGetError() != GL_NO_ERROR)
		return false;

	glTexImage2D(GL_TEXTURE_2D,0,tga.GetBitsPerPixel() == 32 ? GL_RGBA : GL_RGB,mTextureWidth,mTextureHeight,0,tga.GetBitsPerPixel() == 32 ? GL_RGBA : GL_RGB,GL_UNSIGNED_BYTE,tga.GetData());
	if(glGetError() != GL_NO_ERROR)
		return false;

	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D,NULL);

	return true;
}

void Image::Unload(void)
{
	glDeleteTextures(1,&mTextureID);
}

void Image::Draw(float x, float y,const Color& color)
{
	if(!mTextureID)
		return;

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,mTextureID);
	glBegin(GL_QUADS);

	glColor4f(color.r,color.g,color.b,color.a);

	glTexCoord2f(0,0);
	glVertex2f(x,y);
	glTexCoord2f(1,0);
	glVertex2f(x+mTextureWidth,y);
	glTexCoord2f(1,1);
	glVertex2f(x+mTextureWidth,y+mTextureHeight);
	glTexCoord2f(0,1);
	glVertex2f(x,y+mTextureHeight);

	glEnd();
	glDisable(GL_TEXTURE_2D);
}

void Image::DrawFlipped(float x, float y,const Color& color)
{
	if(!mTextureID)
		return;

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,mTextureID);
	glBegin(GL_QUADS);

	glColor4f(color.r,color.g,color.b,color.a);

	glTexCoord2f(0,1);
	glVertex2f(x,y);
	glTexCoord2f(1,1);
	glVertex2f(x+mTextureWidth,y);
	glTexCoord2f(1,0);
	glVertex2f(x+mTextureWidth,y+mTextureHeight);
	glTexCoord2f(0,0);
	glVertex2f(x,y+mTextureHeight);

	glEnd();
	glDisable(GL_TEXTURE_2D);
}