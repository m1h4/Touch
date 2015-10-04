#pragma once

bool InitializeEditor(HWND hWnd);
void UninitializeEditor(void);

bool DrawEditor(HWND hWnd);
bool UpdateEditor(HWND hWnd);

bool KeyboardEditor(HWND hWnd,LPARAM lParam,WPARAM wParam);
bool MouseEditor(HWND hWnd,LPARAM lParam,WPARAM wParam);