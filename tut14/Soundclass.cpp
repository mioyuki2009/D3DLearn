#include "Soundclass.h"
#include "SoundTools.hpp"
SoundClass::SoundClass()
{
	XAudioEngine = nullptr;
	pmaster = nullptr;
	pSource = nullptr;
	pDataBuffer = nullptr;
}

SoundClass::~SoundClass()
{
}

bool SoundClass::Initialize()
{
	bool result;
	
	// Initialize direct sound and the primary sound buffer.
	result = InitializeDirectSound();
	if (!result)
	{
		return false;
	}

	// Load a wave audio file onto a secondary buffer.
	result = PopulateWaveSound("sound01.wav");
	if (!result)
	{
		return false;
	}

	// Play the wave file now that it has been loaded.
	result = PlayWaveFile();
	if (!result)
	{
		return false;
	}

	return true;
}

bool SoundClass::PlayWaveFile() {
	if (FAILED(XAudioEngine->CreateSourceVoice(&pSource, (WAVEFORMATEX*)& wfx)))
		return false;
	if (FAILED(pSource->SubmitSourceBuffer(&buffer)))
		return false;
	if (FAILED(pSource->Start(0)))
		return false;
	return true;
}

void SoundClass::Shutdown()
{
	ShutdownDirectSound();
	return;
}

void SoundClass::ShutdownDirectSound() {
	XAudioEngine->Release();
	delete[] pDataBuffer;
	CoUninitialize();
}

bool SoundClass::InitializeDirectSound()
{
	bool Ret = CoInitializeEx(NULL, COINIT_MULTITHREADED) == S_OK;
	if (!Ret)
		return false;

	if (FAILED(XAudio2Create(&XAudioEngine)))return false;
	if (FAILED(XAudioEngine->CreateMasteringVoice(&pmaster)))return false;
	return true;
}

bool SoundClass::PopulateWaveSound(const char* fileName) {

	const char* strFileName = fileName;
	// Open the file
	HANDLE hFile = CreateFile(
		strFileName,
		GENERIC_READ,
		FILE_SHARE_READ,
		NULL,
		OPEN_EXISTING,
		0,
		NULL);

	if (INVALID_HANDLE_VALUE == hFile)
		return HRESULT_FROM_WIN32(GetLastError()) == S_OK;

	if (INVALID_SET_FILE_POINTER == SetFilePointer(hFile, 0, NULL, FILE_BEGIN))
		return HRESULT_FROM_WIN32(GetLastError()) == S_OK;
	DWORD dwChunkSize;
	DWORD dwChunkPosition;
	//check the file type, should be fourccWAVE or 'XWMA'
	FindChunk(hFile, fourccRIFF, dwChunkSize, dwChunkPosition);
	DWORD filetype;
	ReadChunkData(hFile, &filetype, sizeof(DWORD), dwChunkPosition);
	if (filetype != fourccWAVE)
		return false;

	FindChunk(hFile, fourccFMT, dwChunkSize, dwChunkPosition);
	ReadChunkData(hFile, &wfx, dwChunkSize, dwChunkPosition);
	//fill out the audio data buffer with the contents of the fourccDATA chunk
	FindChunk(hFile, fourccDATA, dwChunkSize, dwChunkPosition);
	pDataBuffer = new BYTE[dwChunkSize];
	ReadChunkData(hFile, pDataBuffer, dwChunkSize, dwChunkPosition);

	buffer.AudioBytes = dwChunkSize;  //size of the audio buffer in bytes
	buffer.pAudioData = pDataBuffer;  //buffer containing audio data
	buffer.Flags = XAUDIO2_END_OF_STREAM; // tell the source voice not to expect any data after this buffer
	return true;
}