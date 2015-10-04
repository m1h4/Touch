#pragma once

#include "Vector.h"

#ifdef _DEBUG
#define ASSERT(expression) if(EvaluateAssert((BOOL)expression,#expression,__FUNCTION__,__FILE__,__LINE__)) DebugBreak();
#define TRACE Trace
#else
#define ASSERT __noop
#define TRACE __noop
#endif

VOID Trace(LPCTSTR format,...);
BOOL EvaluateAssert(BOOL expression,LPCSTR expressionString,LPCSTR function,LPCSTR file,LONG line);
LONG WINAPI DumpException(LPEXCEPTION_POINTERS exceptionInfo);