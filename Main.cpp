#include "Globals.h"
#include "Timer.h"
#include "Game.h"
#include "Editor.h"
#include "Debug.h"
#include "Audio.h"

//#define EXPERIMENTAL_FAST_LOOP

LPCTSTR mainClassName = TEXT("Touch Class");
LPCTSTR mainWindowName = TEXT("Touch");

HDC		mainDC = NULL;
HGLRC	mainRC = NULL;
HWND	mainWnd = NULL;
BOOL	mainEditor = FALSE;
BOOL	mainSound = TRUE;

void ReSizeScene(ULONG width,ULONG height)
{
	if(!height)
		height = 1;

	glViewport(0,0,width,height);
}

bool InitializeGraphics(void)
{
	//glShadeModel(GL_SMOOTH);
	//glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
	//glPolygonMode(GL_FRONT_AND_BACK,GL_POINT);

	glClearColor(0.0f,0.0f,0.0f,1.0f);
	glClearDepth(1.0f);

	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_BACK);

	glDisable(GL_DEPTH_TEST);
	//glDepthFunc(GL_LEQUAL);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

	//glEnable(GL_POLYGON_SMOOTH);
	//glHint(GL_POLYGON_SMOOTH_HINT,GL_NICEST);

	glEnable(GL_LINE_SMOOTH);
	glHint(GL_LINE_SMOOTH_HINT,GL_NICEST);

	//glHint(GL_PERSPECTIVE_CORRECTION_HINT,GL_NICEST);
	
	return true;
}

bool Draw(HWND hWnd)
{
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	if(mainEditor)
	{
		if(!DrawEditor(hWnd))
			return false;
	}
	else
	{
		if(!DrawGame(hWnd))
			return false;
	}

	SwapBuffers(mainDC);

	return true;
}

bool Update(HWND hWnd)
{
	if(!UpdateTimer())
		return false;

	if(mainEditor)
	{
		if(!UpdateEditor(hWnd))
			return false;
	}
	else
	{
		if(!UpdateGame(hWnd))
			return false;
	}

	return true;
}

void UninitializeWindow(void)
{
	if(mainRC)
	{
		if(!wglMakeCurrent(NULL,NULL))
			MessageBox(NULL,TEXT("Could not release the device context."),TEXT("Error"),MB_OK|MB_ICONINFORMATION);

		if(!wglDeleteContext(mainRC))
			MessageBox(NULL,TEXT("Could not release the rendering context."),TEXT("Error"),MB_OK|MB_ICONINFORMATION);

		mainRC = NULL;
	}

	if(mainDC && !ReleaseDC(mainWnd,mainDC))
	{
		MessageBox(NULL,TEXT("Could not release the device context."),TEXT("Error"),MB_OK|MB_ICONINFORMATION);

		mainDC = NULL;
	}

	if(mainWnd && !DestroyWindow(mainWnd))
	{
		MessageBox(NULL,TEXT("Could not release the window handle."),TEXT("Error"),MB_OK|MB_ICONINFORMATION);

		mainWnd = NULL;
	}

	if(!UnregisterClass(mainClassName,GetModuleHandle(NULL)))
		MessageBox(NULL,TEXT("Could not unregister the window class."),TEXT("Error"),MB_OK|MB_ICONINFORMATION);
}

LRESULT WINAPI WinProcedure(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	switch(uMsg)
	{
	//case WM_SYSCOMMAND:
	//	switch(wParam)
	//	{
	//	case SC_SCREENSAVE:
	//	case SC_MONITORPOWER:
	//		return FALSE;
	//	}
	//	break;

	case WM_CLOSE:
		PostQuitMessage(0);
		return 0;

	case WM_SIZE:
		ReSizeScene(LOWORD(lParam),HIWORD(lParam));
		return 0;

	case WM_KEYDOWN:
		if(mainEditor)
		{
			if(!KeyboardEditor(hWnd,lParam,wParam))
			{
				//#ifdef _DEBUG
				if(wParam == VK_ESCAPE)
					PostQuitMessage(0);
				//#endif
			}
		}
		else
		{
			if(!KeyboardGame(hWnd,lParam,wParam))
			{
				//#ifdef _DEBUG
				if(wParam == VK_ESCAPE)
					PostQuitMessage(0);
				//#endif
			}
		}
		return 0;

	case WM_LBUTTONDOWN:
		if(mainEditor)
			MouseEditor(hWnd,lParam,wParam);
		else
			MouseGame(hWnd,lParam,wParam);
		return 0;

	//case WM_SETCURSOR:
	//	if(LOWORD(lParam) == HTCLIENT)
	//	{
	//		SetCursor(NULL);
	//		return TRUE;
	//	}
	//	break;
	}

	return DefWindowProc(hWnd,uMsg,wParam,lParam);
}

bool InitializeWindow(ULONG width,ULONG height)
{
	PIXELFORMATDESCRIPTOR pfd =
	{
		sizeof(PIXELFORMATDESCRIPTOR),
		1,
		PFD_DRAW_TO_WINDOW|PFD_SUPPORT_OPENGL|PFD_DOUBLEBUFFER,
		PFD_TYPE_RGBA,
		32,	// Color Buffer Size
		0,0,0,0,0,0,
		0,
		0,
		0,
		0,0,0,0,
		24,	// Depth Buffer Size
		0,
		0,
		PFD_MAIN_PLANE,
		0,
		0,0,0
	};

	UINT		pixelFormat;
	WNDCLASS	wc = {0};
	DWORD		dwExStyle = 0;
	DWORD		dwStyle = WS_POPUP|WS_CAPTION|WS_SYSMENU|WS_MINIMIZEBOX;
	RECT		windowRect = {0,0,width,height};

	wc.style			= CS_CLASSDC;
	wc.lpfnWndProc		= WinProcedure;
	wc.hInstance		= GetModuleHandle(NULL);
	wc.hIcon			= LoadIcon(NULL,IDI_WINLOGO);
	wc.hCursor			= LoadCursor(NULL,IDC_ARROW);
	wc.lpszClassName	= mainClassName;

	if(!RegisterClass(&wc))
	{
		MessageBox(NULL,TEXT("Could not register the window class."),TEXT("Error"),MB_OK|MB_ICONEXCLAMATION);

		return false;
	}

	AdjustWindowRectEx(&windowRect,dwStyle,FALSE,dwExStyle);

	mainWnd = CreateWindowEx(dwExStyle,
								mainClassName,
								mainWindowName,
								dwStyle,
								CW_USEDEFAULT,CW_USEDEFAULT,
								windowRect.right-windowRect.left,
								windowRect.bottom-windowRect.top,
								NULL,
								NULL,
								GetModuleHandle(NULL),
								NULL);
	if(!mainWnd)
	{
		UninitializeWindow();
		MessageBox(NULL,TEXT("Window creation error."),TEXT("Error"),MB_OK|MB_ICONEXCLAMATION);
		return false;
	}
	
	mainDC = GetDC(mainWnd);
	if(!mainDC)
	{
		UninitializeWindow();
		MessageBox(NULL,TEXT("Could not create a device context."),TEXT("Error"),MB_OK|MB_ICONEXCLAMATION);
		return false;
	}

	pixelFormat = ChoosePixelFormat(mainDC,&pfd);
	if(!pixelFormat)
	{
		UninitializeWindow();
		MessageBox(NULL,TEXT("Could not find a suitable pixel format."),TEXT("Error"),MB_OK|MB_ICONEXCLAMATION);
		return false;
	}

	if(!SetPixelFormat(mainDC,pixelFormat,&pfd))
	{
		UninitializeWindow();
		MessageBox(NULL,TEXT("Could not set the pixel format."),TEXT("Error"),MB_OK|MB_ICONEXCLAMATION);
		return false;
	}

	mainRC = wglCreateContext(mainDC);
	if(!mainRC)
	{
		UninitializeWindow();
		MessageBox(NULL,TEXT("Could not create a rendering context."),TEXT("Error"),MB_OK|MB_ICONEXCLAMATION);
		return false;
	}

	if(!wglMakeCurrent(mainDC,mainRC))
	{
		UninitializeWindow();
		MessageBox(NULL,TEXT("Could not activate the rendering context."),TEXT("Error"),MB_OK|MB_ICONEXCLAMATION);
		return false;
	}

	RECT rect;
	GetClientRect(mainWnd,&rect);

	ReSizeScene(rect.right-rect.left,rect.bottom-rect.top);

	return true;
}

INT WINAPI WinMain(HINSTANCE /*hInstance*/,HINSTANCE /*hPrevInstance*/,LPSTR lpCmdLine,INT /*nCmdShow*/)
{
	mainEditor = !lstrcmpiA(lpCmdLine,"/editor");
	mainSound = !!lstrcmpiA(lpCmdLine,"/nosound");

	SetUnhandledExceptionFilter(DumpException);

	srand(GetTickCount());

	timeBeginPeriod(1);

	InitCommonControls();

	CoInitializeEx(NULL,NULL);

	if(!InitializeWindow(480,800))
		return 0;

	if(!InitializeGraphics())
	{
		UninitializeWindow();
		MessageBox(NULL,TEXT("Graphics initialization failed."),TEXT("Error"),MB_OK|MB_ICONEXCLAMATION);
		return false;
	}

	if(!InitializeTimer())
	{
		UninitializeWindow();
		MessageBox(NULL,TEXT("Timer initialization failed."),TEXT("Error"),MB_OK|MB_ICONEXCLAMATION);
		return false;
	}

	if(mainSound)
	{
		if(!InitializeAudio(mainWnd))
		{
			UninitializeWindow();
			MessageBox(NULL,TEXT("Audio initialization failed."),TEXT("Error"),MB_OK|MB_ICONEXCLAMATION);
			return false;
		}
	}

	if(mainEditor)
	{
		if(!InitializeEditor(mainWnd))
		{
			UninitializeWindow();
			MessageBox(NULL,TEXT("Editor initialization failed."),TEXT("Error"),MB_OK|MB_ICONEXCLAMATION);
			return false;
		}
	}
	else
	{
		if(!InitializeGame(mainWnd))
		{
			UninitializeWindow();
			MessageBox(NULL,TEXT("Game initialization failed."),TEXT("Error"),MB_OK|MB_ICONEXCLAMATION);
			return false;
		}
	}

	//ShowCursor(FALSE);
//#ifdef _DEBUG
	//ShowWindow(mainWnd,SW_SHOWMAXIMIZED);
//#else
	ShowWindow(mainWnd,SW_SHOW);
//#endif
	//SetForegroundWindow(mainWnd);
	//SetFocus(mainWnd);

#ifdef EXPERIMENTAL_FAST_LOOP
	typedef void (APIENTRY *PFNWGLEXTSWAPCONTROLPROC) (int);

	PFNWGLEXTSWAPCONTROLPROC wglSwapIntervalEXT = (PFNWGLEXTSWAPCONTROLPROC)wglGetProcAddress("wglSwapIntervalEXT");
	if(wglSwapIntervalEXT)
		wglSwapIntervalEXT(0);

	ULONGLONG frameTime;
	ULONGLONG frequency;

	QueryPerformanceFrequency((PLARGE_INTEGER)&frequency);

	frameTime = frequency / TIMER_FRAME_RATE;

	ULONGLONG lastTime;

	QueryPerformanceCounter((PLARGE_INTEGER)&lastTime);
#endif

	MSG	msg;

	for(;;)
	{
		if(PeekMessage(&msg,NULL,0,0,PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		if(msg.message == WM_QUIT)
			break;

#ifdef EXPERIMENTAL_FAST_LOOP
		ULONGLONG currentTime;

		QueryPerformanceCounter((PLARGE_INTEGER)&currentTime);

		if(currentTime - lastTime > frameTime)
		{
			lastTime = currentTime;
#endif

			if(!Update(mainWnd) || !Draw(mainWnd))
				PostQuitMessage(0);

#ifdef EXPERIMENTAL_FAST_LOOP
		}
#endif

	}

	if(mainEditor)
		UninitializeEditor();
	else
		UninitializeGame();

	if(mainSound)
		UninitializeAudio();

	UninitializeTimer();
	UninitializeWindow();

	CoUninitialize();

	timeEndPeriod(1);

  	return msg.wParam;
}
