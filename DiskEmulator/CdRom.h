#pragma once
#include "CdAudioPlayer.h"
#include "CdFs.h"

class CCdRom {
private:
	CCdAudioPlayer* m_pPlayer;
	CCdFs* m_pFs;

	int m_diskNo;
	bool m_initialized = false;

public:
	static const char virtualDriveA = 'E'; // 대문자로 날아올거다.. 아마도..
	static const WCHAR virtualDriveW = L'E';
public:
	CCdRom();
	void InititalizeRootPath(LPCSTR lpFileName);
	bool IsRootPathInitialized() {
		return m_initialized;
	}

	void SetAudioHandle(HWND hwnd);
	void DecisionDisk(LPCSTR lpFileName);

	bool IsVirutalDiskA(LPCSTR lpPathName);
	bool IsVirutalDiskW(LPCWSTR lpPathName);

	void PlayAudio();
	void SeekAudioTrack(DWORD toTrackNo);
	void StopAudio();
	/// <summary>
	/// dwParam2->dwReturn = diskTrackCount[requireDisk - 1];
	/// </summary>
	/// <returns></returns>
	DWORD GetTotalTrack();
	/// <summary>
	/// diskTrackLength[requireDisk - 1][dwParam2->dwTrack - 1];
	/// </summary>
	/// <returns></returns>
	DWORD GetTrackLength(DWORD track);

	bool GetLabel(LPCSTR lpRootPathName, char buff[], DWORD size);
	bool ResolveFileA(LPCSTR lpSourceFileName, LPCSTR lpDestinatonFileName);
	bool ResolveFileW(LPCWSTR lpSourceFileName, LPCWSTR lpDestinatonFileName);
public:
	static CCdRom* GetIntance() {
		static CCdRom* rom = new CCdRom();
		return rom;
	}
};