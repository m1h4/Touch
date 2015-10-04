#include "Globals.h"
#include "Game.h"
#include "Interpolations.h"
#include "Timer.h"
#include "Debug.h"
#include "Graphics.h"
#include "Resource.h"
#include "Sound.h"
#include "Image.h"

#ifdef _DEBUG
bool debugDraw = true;
#else
bool debugDraw = false;
#endif

Font gameFont;
Image gameBackground;
Image gameShadow;

bool InitializeGame(HWND hWnd)
{
	if(!gameFont.LoadFromResource(MAKEINTRESOURCE(IDR_FONT1),TEXT("RT_FONT")))
		return false;

	if(!gameBackground.LoadFromResource(MAKEINTRESOURCE(IDR_IMAGE1),TEXT("RT_IMAGE")))
		return false;

	if(!gameShadow.LoadFromResource(MAKEINTRESOURCE(IDR_IMAGE2),TEXT("RT_IMAGE")))
		return false;

	return true;
}

void UninitializeGame(void)
{
	gameFont.Unload();
	gameBackground.Unload();
	gameShadow.Unload();
}

bool KeyboardGame(HWND hWnd,LPARAM /*lParam*/,WPARAM wParam)
{
	switch(wParam)
	{
	case VK_F2:
		debugDraw = !debugDraw;
		return true;
	}

	return false;
}

const char* menu[] = {"music","videos","pictures","radio","marketplace","social","internet","more","options"};

float time = 0.0f;
unsigned long selected = -1;
bool direction = false;

#define ANIMATION_END 40.0f
#define TEXT_HEIGHT 100.0f

bool MouseGame(HWND /*hWnd*/,LPARAM lParam,WPARAM /*wParam*/)
{
	time = 0;
	if(direction || time != ANIMATION_END)
		return true;

	long x = LOWORD(lParam);
	long y = HIWORD(lParam);

	for(unsigned long i = 0; i < _countof(menu); ++i)
	{
		if(x >= 15 && x <= 750 && y >= 15 + i * TEXT_HEIGHT && y <= 15 + i * TEXT_HEIGHT + TEXT_HEIGHT)
		{
			selected = i;
			direction = true;
			time = ANIMATION_END;
			break;
		}

		selected = -1;
	}

	return true;
}

bool UpdateGame(HWND /*hWnd*/)
{
	return true;
}

bool DrawGame(HWND hWnd)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	RECT rect;
	GetClientRect(hWnd,&rect);
	glOrtho(0,rect.right-rect.left,rect.bottom-rect.top,0,1.0f,0.0f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	gameBackground.Draw(0, 0, Color(1,1,1,1));
	gameShadow.Draw(0, 0, Color(1,1,1,1));
	gameShadow.DrawFlipped(0, 800 - gameShadow.GetHeight(), Color(1,1,1,1));

	for(unsigned long i = 0; i < _countof(menu); ++i)
	{
		float dx = 0;//5.0f * max(min(time/5.0f - i,1.0f),0.0f);
		float dy = 50.0f - 50.0f * max(min(time/5.0f - i,1.0f),0.0f);
		float alpha = max(min(time/5.0f - i,1.0f),0.0f);

		Color color(alpha*0.6f,0.0f,0.0f,0.0f);
		Color colorSelected(alpha*0.6f,0.6f,0.6f,0.6f);

		if(i == selected)
		{
			color = Color(alpha*0.6f,0.8f,0.8f,0.8f);
			colorSelected = Color(alpha*0.6f,0.9f,0.9f,0.9f);
		}

		//if(i == selected)
		{
			gameFont.DrawText(menu[i],UINT_MAX,Vector2(15.0f + 1 + dx,15.0f + 1 + i * TEXT_HEIGHT + dy),colorSelected);
			gameFont.DrawText(menu[i],UINT_MAX,Vector2(15.0f + 1 + dx,15.0f - 1 + i * TEXT_HEIGHT + dy),colorSelected);
			gameFont.DrawText(menu[i],UINT_MAX,Vector2(15.0f - 1 + dx,15.0f + 1 + i * TEXT_HEIGHT + dy),colorSelected);
			gameFont.DrawText(menu[i],UINT_MAX,Vector2(15.0f - 1 + dx,15.0f - 1 + i * TEXT_HEIGHT + dy),colorSelected);

			gameFont.DrawText(menu[i],UINT_MAX,Vector2(15.0f + 1 + dx,15.0f + i * TEXT_HEIGHT + dy),colorSelected);
			gameFont.DrawText(menu[i],UINT_MAX,Vector2(15.0f - 1 + dx,15.0f + i * TEXT_HEIGHT + dy),colorSelected);
			gameFont.DrawText(menu[i],UINT_MAX,Vector2(15.0f + dx,15.0f + 1 + i * TEXT_HEIGHT + dy),colorSelected);
			gameFont.DrawText(menu[i],UINT_MAX,Vector2(15.0f + dx,15.0f - 1 + i * TEXT_HEIGHT + dy),colorSelected);
		}

		gameFont.DrawText(menu[i],UINT_MAX,Vector2(15.0f + dx,15.0f + i * TEXT_HEIGHT + dy),color);
	}

	time += GetElapsedTime();
	
	return true;
}