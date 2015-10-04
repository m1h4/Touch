#pragma once

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

#define _USE_MATH_DEFINES
#include <math.h>

#include <windows.h>
#include <commctrl.h>

#include <gl\gl.h>
#include <gl\glu.h>
//#include <gl\glaux.h>

#include <dsound.h>

#include "Debug.h"
#include "Vector.h"
#include "Matrix.h"
#include "Array.h"
#include "Plane.h"
#include "Color.h"
#include "AABB.h"

#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")