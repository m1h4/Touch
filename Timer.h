#pragma once

#define TIMER_FRAME_RATE 60

bool InitializeTimer(void);
void UninitializeTimer(void);

bool UpdateTimer(void);

float GetElapsedTime(void);
float GetAbsoluteTime(void);