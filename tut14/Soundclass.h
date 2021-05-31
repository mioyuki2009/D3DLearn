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

	IXAudio2* XAudioEngine;                                 // IXAduio2��Ƶ����
	IXAudio2MasteringVoice* pmaster;                        // ����������
	IXAudio2SourceVoice* pSource;                           // ��Դ
	BYTE* pDataBuffer;
	WAVEFORMATEXTENSIBLE wfx = { 0 };
	XAUDIO2_BUFFER buffer = { 0 };
};