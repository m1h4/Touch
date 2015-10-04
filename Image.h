#pragma once

class Image
{
public:
	Image(void);
	~Image(void);

	bool LoadFromFile(LPCTSTR path);
	bool LoadFromMemory(const unsigned char* data, unsigned long size);
	bool LoadFromResource(LPCTSTR id,LPCTSTR group);
	void Unload(void);

	void Draw(float x, float y,const Color& color);
	void DrawFlipped(float x, float y,const Color& color);

	unsigned long GetWidth(void) const { return mTextureWidth; }
	unsigned long GetHeight(void) const { return mTextureHeight; }

protected:
	GLuint			mTextureID;

	unsigned long	mTextureWidth;
	unsigned long	mTextureHeight;
};
