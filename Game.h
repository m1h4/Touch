#pragma once

bool InitializeGame(HWND hWnd);
void UninitializeGame(void);

bool DrawGame(HWND hWnd);
bool UpdateGame(HWND hWnd);

bool KeyboardGame(HWND hWnd,LPARAM lParam,WPARAM wParam);
bool MouseGame(HWND hWnd,LPARAM lParam,WPARAM wParam);