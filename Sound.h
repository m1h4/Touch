#pragma once

class Sound
{
public:
	Sound(void) : mBuffers(NULL), mBufferCount(0) {}
	~Sound(void) { Unload(); }

	bool LoadFromFile(LPCTSTR path,unsigned long buffers = 8);
	bool LoadFromMemory(LPWAVEFORMATEX format,unsigned char* data,unsigned long size,unsigned long buffers);
	void Unload(void);

	bool Play(bool loop = false);
	bool GetPlaying(void) const;
	bool GetLooping(void) const;
	void Stop(void);
	void Reset(void);
	void SetVolume(float volume);
	float GetVolume(void) const;

public:
	unsigned long			mBufferCount;
	IDirectSoundBuffer**	mBuffers;
};