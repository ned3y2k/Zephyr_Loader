#pragma once
#include "MyDirectSound.h"
#include "SoundFile.h"
#include "include/macro.h"

class CCdAudioPlayer {
private:
	CMyDirectSound* m_pDSound = nullptr;
	CSoundFile* m_pSoundFile = nullptr;
	PSTR* m_pFileList = nullptr;
	PSTR m_pPathPattern;

	WAVEFORMATEX m_WaveformatEx;
	bool m_Play = false;
	int m_fileCnt = 0;

	void InitFileList(char* rootPath, size_t rootPathLen);

public:
	DEBUG_EXPORT void Initialize(HWND hWnd);
	DEBUG_EXPORT void SetRootPath(char* pFindPattern);

	DEBUG_EXPORT void Release();

	DEBUG_EXPORT CCdAudioPlayer() {
		m_Play = false;
		ZeroMemory(&m_WaveformatEx, sizeof(WAVEFORMATEX));
	};
	DEBUG_EXPORT ~CCdAudioPlayer() {
		Release();
	};

	DEBUG_EXPORT void SetTrack(int disk, int trak);
	DEBUG_EXPORT void Play();
	DEBUG_EXPORT void Stop();

	DEBUG_EXPORT bool GetFileName(int disk, int track, char destination[]);
	DEBUG_EXPORT bool GetFilePath(int disk, int track, char destination[MAX_PATH]);
	DEBUG_EXPORT DWORD GetTotalTrack(DWORD disk);
	DEBUG_EXPORT DWORD GetTrackLength(DWORD disk, DWORD track);

	DEBUG_EXPORT void Copy(char dest[][MAX_PATH]);

	DEBUG_EXPORT HRESULT GetAudioSamples(LPBYTE lpDesBuf, const DWORD dwRequiredBytes, DWORD& dwRetBytes);
};