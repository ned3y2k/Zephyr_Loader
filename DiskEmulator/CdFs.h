#pragma once
#include "include/macro.h"

class CCdFs
{
private:
	char* m_pDiskPathFmtA = nullptr; // ?\\disk\\ZEPHYR%d\\%s
	WCHAR* m_pDiskPathFmtW = nullptr; // ?\\disk\\ZEPHYR%d\\%s

	WCHAR m_lastFileW[MAX_PATH];
	char m_lastFileA[MAX_PATH];
public:
	CCdFs() {
	}
	~CCdFs() {
		free(m_pDiskPathFmtA);
		free(m_pDiskPathFmtW);
	}
	void Initialize(LPCSTR lpExePath);

	bool ResolveFileA(int requireDisk, LPCSTR lpSourceFileName, LPCSTR lpDestinatonFileName);
	bool ResolveFileW(int requireDisk, LPCWSTR lpSourceFileName, LPCWSTR lpDestinatonFileName);
};