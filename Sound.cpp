#include "Globals.h"
#include "Sound.h"

extern IDirectSound* audioSound;

bool Sound::LoadFromFile(LPCTSTR path,unsigned long buffers)
{
	if(!audioSound || mBuffers)
		return false;

	HMMIO handle = mmioOpen((LPTSTR)path,NULL,MMIO_ALLOCBUF|MMIO_READ);
	if(!handle)
		return false;

	MMCKINFO head;
	if(mmioDescend(handle,&head,NULL,NULL) != MMSYSERR_NOERROR)
	{
		mmioClose(handle,NULL);
		return false;
	}

	// Check to make sure this is a valid wave file
	if(head.ckid != FOURCC_RIFF || head.fccType != mmioFOURCC('W','A','V','E'))
	{
		mmioClose(handle,NULL);
		return false;
	}

    // Search the input file for for the 'fmt ' chunk
	MMCKINFO chunk = {mmioFOURCC('f','m','t',' ')};
	if(mmioDescend(handle,&chunk,&head,MMIO_FINDCHUNK) != MMSYSERR_NOERROR)
	{
		mmioClose(handle,NULL);
		return false;
	}

	// Expect the 'fmt' chunk to be at least as large as <PCMWAVEFORMAT>;
	// if there are extra parameters at the end, we'll ignore them
	if(chunk.cksize < (LONG)sizeof(PCMWAVEFORMAT))
	{
		mmioClose(handle,NULL);
		return false;
	}

	// Read the 'fmt ' chunk into <pcmWaveFormat>
	PCMWAVEFORMAT format;
	if(mmioRead(handle,(HPSTR)&format,sizeof(format)) != sizeof(format) )
	{
		mmioClose(handle,NULL);
		return false;
	}

	// Allocate the waveformatex, but if its not pcm format, read the next
	// word, and thats how many extra bytes to allocate
	if(format.wf.wFormatTag != WAVE_FORMAT_PCM)
	{
		mmioClose(handle,NULL);
		return false;
	}

    // Ascend the input file out of the 'fmt ' chunk.
    if(mmioAscend(handle,&chunk,NULL) != MMSYSERR_NOERROR)
	{
		mmioClose(handle,NULL);
		return false;
	}

	// Seek to the data
	if(mmioSeek(handle,head.dwDataOffset + sizeof(FOURCC),SEEK_SET) == -1)
		return false;

	// Search the input file for the 'data' chunk.
	chunk.ckid = mmioFOURCC('d', 'a', 't', 'a');
	if(mmioDescend(handle,&chunk,&head,MMIO_FINDCHUNK) != MMSYSERR_NOERROR)
	{
		mmioClose(handle,NULL);
		return false;
	}

	unsigned char* buffer = new unsigned char[chunk.cksize];
	if(!buffer)
	{
		mmioClose(handle,NULL);
		return false;
	}

	MMIOINFO info;
	if(mmioGetInfo(handle,&info,NULL) != MMSYSERR_NOERROR)
	{
		mmioClose(handle,NULL);
		return false;
	}

	for(unsigned int ct = 0; ct < chunk.cksize; ++ct)
	{
		// Copy the bytes from the io to the buffer
		if(info.pchNext == info.pchEndRead)
		{
			if(mmioAdvance(handle,&info,MMIO_READ) != MMSYSERR_NOERROR)
			{
				mmioClose(handle,NULL);
				return false;
			}

			if(info.pchNext == info.pchEndRead)
			{
				mmioClose(handle,NULL);
				return false;
			}
		}

		// Actual copy
		buffer[ct] = *(LPBYTE)info.pchNext;

		info.pchNext++;
    }

	if(mmioSetInfo(handle,&info,NULL)  != MMSYSERR_NOERROR)
	{
		mmioClose(handle,NULL);
		return false;
	}

	mmioClose(handle,NULL);

	WAVEFORMATEX fmt;
	fmt.cbSize = sizeof(WAVEFORMATEX);
	fmt.nAvgBytesPerSec = format.wf.nAvgBytesPerSec;
	fmt.nBlockAlign = format.wf.nBlockAlign;
	fmt.nChannels = format.wf.nChannels;
	fmt.nSamplesPerSec = format.wf.nSamplesPerSec;
	fmt.wBitsPerSample = (WORD)((format.wf.nAvgBytesPerSec << 3) / format.wf.nSamplesPerSec);
	fmt.wFormatTag = format.wf.wFormatTag;

	if(!LoadFromMemory(&fmt,buffer,chunk.cksize,buffers))
	{
		delete[] buffer;
		return false;
	}

	delete[] buffer;
	return true;
}

void Sound::Unload(void)
{
	if(mBuffers)
	{
		for(unsigned long i = 0; i < mBufferCount; ++i)
			mBuffers[i]->Release();

		delete[] mBuffers;
		mBuffers = NULL;
		mBufferCount = 0;
	}
}

bool Sound::LoadFromMemory(LPWAVEFORMATEX format,unsigned char* data,unsigned long size,unsigned long buffers)
{
	if(!audioSound || mBuffers || !buffers)
		return false;

	mBufferCount = buffers;
	mBuffers = new IDirectSoundBuffer*[mBufferCount];
	if(!mBuffers)
		return false;

	DSBUFFERDESC dsbd;
	ZeroMemory(&dsbd,sizeof(DSBUFFERDESC));
	dsbd.dwSize          = sizeof(DSBUFFERDESC);
	dsbd.dwFlags         = DSBCAPS_GLOBALFOCUS|DSBCAPS_CTRLVOLUME;
	dsbd.dwBufferBytes   = size;
	dsbd.guid3DAlgorithm = DS3DALG_DEFAULT;
	dsbd.lpwfxFormat     = format;

	if(FAILED(audioSound->CreateSoundBuffer(&dsbd,&mBuffers[0],NULL)))
		return false;

	LPVOID ptr = NULL;
	ULONG sz;
	if(FAILED(mBuffers[0]->Lock(NULL,NULL,&ptr,&sz,NULL,NULL,DSBLOCK_ENTIREBUFFER)))
	{
		mBuffers[0]->Release();
		mBuffers[0] = NULL;
		return false;
	}

	CopyMemory(ptr,data,size);

	mBuffers[0]->Unlock(ptr,sz,NULL,NULL);

	for(unsigned long i = 1; i < mBufferCount; ++i)
		audioSound->DuplicateSoundBuffer(mBuffers[0],&mBuffers[i]);

	return true;
}

bool Sound::Play(bool loop)
{
	if(!mBuffers)
		return false;

	for(unsigned int i = 0; i < mBufferCount; ++i)
	{
		unsigned long status = NULL;
		mBuffers[i]->GetStatus(&status);

		if(!(status & DSBSTATUS_PLAYING))
		{
			mBuffers[i]->Play(NULL,NULL,loop ? DSBPLAY_LOOPING : NULL);
			return true;
		}
	}

	return false;
}

bool Sound::GetPlaying(void) const
{
	if(!mBuffers)
		return false;

	for(unsigned int i = 0; i < mBufferCount; ++i)
	{
		unsigned long status = NULL;
		mBuffers[i]->GetStatus(&status);

		if(status & DSBSTATUS_PLAYING)
			return true;
	}

	return false;
}

bool Sound::GetLooping(void) const
{
	if(!mBuffers)
		return false;

	for(unsigned int i = 0; i < mBufferCount; ++i)
	{
		unsigned long status = NULL;
		mBuffers[i]->GetStatus(&status);

		if(status & DSBSTATUS_PLAYING && status & DSBSTATUS_LOOPING)
			return true;
	}

	return false;
}

void Sound::Stop(void)
{
	if(!mBuffers)
		return;

	for(unsigned int i = 0; i < mBufferCount; ++i)
		mBuffers[i]->Stop();
}

void Sound::Reset(void)
{
	if(!mBuffers)
		return;

	for(unsigned int i = 0; i < mBufferCount; ++i)
		mBuffers[i]->SetCurrentPosition(0);
}

void Sound::SetVolume(float volume)
{
	if(!mBuffers)
		return;

	for(unsigned int i = 0; i < mBufferCount; ++i)
		mBuffers[i]->SetVolume((LONG)(DSBVOLUME_MIN * (1.0f - max(min(volume,1.0f),0.0f))));
}

float Sound::GetVolume(void) const
{
	if(!mBuffers)
		return 0;

	long volume;
	mBuffers[0]->GetVolume(&volume);

	return 1.0f - volume / (float)DSBVOLUME_MIN;
}