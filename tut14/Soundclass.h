#pragma once
#include <Windows.h>
#include <XAudio2.h>

class SoundClass
{
private:

public:
	SoundClass();
	~SoundClass();
	
	bool Initialize();
	void Shutdown();

private:
	bool InitializeDirectSound();
	bool PopulateWaveSound(const char*);
	void ShutdownDirectSound();

	bool PlayWaveFile();

private:

	IXAudio2* XAudioEngine;                                 // IXAduio2音频引擎
	IXAudio2MasteringVoice* pmaster;                        // 声音管理器
	IXAudio2SourceVoice* pSource;                           // 音源
	BYTE* pDataBuffer;
	WAVEFORMATEXTENSIBLE wfx = { 0 };
	XAUDIO2_BUFFER buffer = { 0 };
};