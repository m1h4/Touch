#include "Globals.h"
#include "Timer.h"
#include "Debug.h"

ULONGLONG	timerFrequency;
ULONGLONG	timerLast;
DOUBLE		timerElapsed;
DOUBLE		timerAbsolute;

bool InitializeTimer(void)
{
	if(!QueryPerformanceFrequency((PLARGE_INTEGER)&timerFrequency))
		return false;

	TRACE(TEXT("Info: Timer frequency is %ull Hz\n"),timerFrequency);

	if(!QueryPerformanceCounter((PLARGE_INTEGER)&timerLast))
		return false;

	timerElapsed = NULL;
	timerAbsolute = NULL;

	return true;
}

void UninitializeTimer(void)
{
	// Does not do anything at this time
}

bool UpdateTimer(void)
{
	ULONGLONG time;

	if(!QueryPerformanceCounter((PLARGE_INTEGER)&time))
		return false;

	timerElapsed = DOUBLE(time - timerLast) / DOUBLE(timerFrequency) * TIMER_FRAME_RATE;
	timerAbsolute += timerElapsed;
	timerLast = time;

	if(timerElapsed > 2.0f)
	{
		//TRACE(TEXT("Hich warning %u\n"),(unsigned long)(timerElapsed * 1000));
		timerElapsed = 2.0f;
	}

	return true;
}

float GetElapsedTime(void)
{
	return (float)timerElapsed;
}

float GetAbsoluteTime(void)
{
	return (float)timerAbsolute;
}