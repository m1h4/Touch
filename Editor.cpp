#include "Globals.h"
#include "Editor.h"
#include "Timer.h"
#include "Debug.h"
#include "Graphics.h"

#ifdef _DEBUG
static bool debugDraw = true;
#else
static bool debugDraw = false;
#endif

bool InitializeEditor(HWND /*hWnd*/)
{
	return true;
}

void UninitializeEditor(void)
{
}

bool DrawEditor(HWND hWnd)
{
	RECT rect;
	GetClientRect(hWnd,&rect);
	glOrtho(0,rect.right-rect.left,rect.bottom-rect.top,0,1.0f,0.0f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	return true;
}

bool UpdateEditor(HWND hWnd)
{
	return true;
}

bool KeyboardEditor(HWND hWnd,LPARAM /*lParam*/,WPARAM wParam)
{
	switch(wParam)
	{
	case VK_F1:
		debugDraw = !debugDraw;
		break;
	}

	return false;
}

bool MouseEditor(HWND /*hWnd*/,LPARAM lParam,WPARAM /*wParam*/)
{
	return true;
}