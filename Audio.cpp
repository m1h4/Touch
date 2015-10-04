#include "Globals.h"
#include "Audio.h"

IDirectSound* audioSound = NULL;

bool InitializeAudio(HWND hWnd)
{
	ASSERT(audioSound == NULL);

	if(FAILED(CoCreateInstance(CLSID_DirectSound8,NULL,CLSCTX_INPROC_SERVER,IID_IDirectSound8,(void**)&audioSound)))
		return false;

	if(FAILED(audioSound->Initialize(NULL)))
		return false;

	if(FAILED(audioSound->SetCooperativeLevel(hWnd,DSSCL_PRIORITY)))
		return false;

	return true;
}

void UninitializeAudio(void)
{
	if(audioSound)
	{
		audioSound->Release();
		audioSound = NULL;
	}
}