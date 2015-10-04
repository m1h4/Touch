#include "Globals.h"
#include "Font.h"
#include "Debug.h"
#include "Targa.h"

bool Font::LoadFromFile(LPCTSTR path)
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

bool Font::LoadFromResource(LPCTSTR id,LPCTSTR group)
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

bool Font::LoadFromMemory(const unsigned char* data,unsigned long size)
{
	ASSERT(data);
	ASSERT(size);

	if(size < sizeof(mHeader))
		return false;

	ZeroMemory(&mHeader,sizeof(mHeader));
	ZeroMemory(mElements,sizeof(mElements));
	CopyMemory(&mHeader,data,sizeof(mHeader));

	data += sizeof(mHeader);
	size -= sizeof(mHeader);

	if(mHeader.magic != FontMagic)
		return false;

	if(mHeader.version != FontVersion)
		return false;

	if(!mHeader.tsize)
		return false;

	if(mHeader.chars > FontChars)
		return false;

	if(size < sizeof(mElements[0]) * mHeader.chars)
		return false;

	for(unsigned long i = 0; i < mHeader.chars; ++i)
	{
		CopyMemory(&mElements[i],data,sizeof(mElements[i]));

		data += sizeof(mElements[i]);
		size -= sizeof(mElements[i]);
	}

	if(size < mHeader.tsize)
		return false;

	Targa tga;

	if(!tga.LoadFromMemory(data,mHeader.tsize))
		return false;

	mTextureWidth = tga.GetWidth();
	mTextureHeight = tga.GetHeight();

	glGenTextures(1,&mTextureID);
	if(glGetError() != GL_NO_ERROR)
		return false;

	glBindTexture(GL_TEXTURE_2D,mTextureID);
	if(glGetError() != GL_NO_ERROR)
		return false;

	glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,mTextureWidth,mTextureHeight,0,GL_RGBA,GL_UNSIGNED_BYTE,tga.GetData());
	if(glGetError() != GL_NO_ERROR)
		return false;

	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D,NULL);

	return true;
}

void Font::Unload(void)
{
	glDeleteTextures(1,&mTextureID);
}

void Font::DrawText(const char* string,unsigned int length,const Vector2& point,const Color& color)
{
	if(!mTextureID)
		return;

	if(length == UINT_MAX)
		length = strlen(string);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,mTextureID);
	glBegin(GL_QUADS);

	glColor4f(color.r,color.g,color.b,color.a);

	float x = point.x;
	float y = point.y + mHeader.ascent*mHeader.uscale;

	unsigned int ch;

	for(unsigned int i = 0; i < length; ++i)
	{
		ch = *(unsigned char*)&string[i];

		if(ch == '\n')
		{
			x = point.x;
			y += mHeader.height*mHeader.uscale;
		}

		if(ch < 32)
			continue;

		ch -= 32;

		float rectleft = mElements[ch].x - (float)mHeader.outmargl;
		float recttop = mElements[ch].y - (float)mHeader.outmargt;
		float rectright = mElements[ch].x + mElements[ch].width + (float)mHeader.outmargr;
		float rectbottom = mElements[ch].y + mElements[ch].height + (float)mHeader.outmargb;


		float topleftx = (float)x - (float)mHeader.outmargl*mHeader.uscale + (float)mElements[ch].left*mHeader.uscale;
		float toplefty = (float)y - (float)mHeader.outmargt*mHeader.uscale + (float)mElements[ch].top*mHeader.uscale;
		float bottomrightx = topleftx + (rectright - rectleft) * mHeader.uscale;
		float bottomrighty = toplefty + (rectbottom - recttop) * mHeader.uscale;

		glTexCoord2f(rectleft / (float)mTextureWidth,recttop / (float)mTextureHeight);
		glVertex2f(topleftx,toplefty);

		glTexCoord2f(rectright / (float)mTextureWidth,recttop / (float)mTextureHeight);
		glVertex2f(bottomrightx,toplefty);

		glTexCoord2f(rectright / (float)mTextureWidth,rectbottom / (float)mTextureHeight);
		glVertex2f(bottomrightx,bottomrighty);

		glTexCoord2f(rectleft / (float)mTextureWidth,rectbottom / (float)mTextureHeight);
		glVertex2f(topleftx,bottomrighty);

		x += mElements[ch].advancex*mHeader.uscale;
	}

	glEnd();
	glDisable(GL_TEXTURE_2D);
}

void Font::GetTextSize(const char* string,unsigned int length,Vector2* size)
{
	if(!mTextureID)
		return;

	if(length == UINT_MAX)
		length = strlen(string);

	float x = 0;
	float y = 0;
	float mx = 0;

	unsigned int ch;

	for(unsigned int i = 0; i < length; ++i)
	{
		ch = *(unsigned char*)&string[i];

		if(ch == '\n')
		{
			x = 0;
			y += mHeader.height*mHeader.uscale;
		}

		if(ch < 32)
			continue;

		ch -= 32;

		if(i == length-1)
			x += mElements[ch].left*mHeader.uscale + mElements[ch].width*mHeader.uscale;
		else
			x += mElements[ch].advancex*mHeader.uscale;
		
		mx = max(mx,x);
	}

	size->x = mx;
	size->y = y + mHeader.height*mHeader.uscale;
}