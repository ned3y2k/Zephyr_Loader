#include "pch.h"
#include <stdexcept>
#include "CdRom.h"
#include "file.util.h"

const char diskImage[3][15] = {
	{"data\\disk1.bmp"},
	{"data\\disk2.bmp"},
	{"data\\disk3.bmp"}
};
const char diskCount = 3;

CCdRom::CCdRom() {
	m_diskNo = 1;
	m_pPlayer = new CCdAudioPlayer();
	m_pFs = new CCdFs();
}

void CCdRom::SetAudioHandle(HWND hwnd) {
	m_pPlayer->Initialize(hwnd);
}

void CCdRom::InititalizeRootPath(LPCSTR lpRootPath) {
	m_initialized = true;

	if (strlen(lpRootPath) > 240) {
		MessageBoxA(nullptr, "설치된 경로가 너무 길어서 실행이 불가합니다.", "에러", MB_ICONQUESTION);
		throw std::invalid_argument("설치된 경로가 너무 길어서 실행이 불가합니다.");
	}

	try {
		m_pPlayer->SetRootPath((char*)lpRootPath);
	}
	catch (std::invalid_argument ex) {
		MessageBoxA(nullptr, "BGM 폴더를 찾을수 없습니다.\ndisk\\bgm 폴더를 만들어주세요.", "에러", MB_OK | MB_ICONEXCLAMATION);
#ifdef DEBUG
		throw ex;
#endif // DEBUG

	}

	m_pFs->Initialize(lpRootPath);
}

void CCdRom::DecisionDisk(LPCSTR lpFileName)
{
	for (int i = 0; i < 3; i++) {
		if (strcmp(lpFileName, diskImage[i]) == 0) {
			m_diskNo = i + 1;
		}
	}
}

bool CCdRom::IsVirutalDiskA(LPCSTR lpPathName) {
	return virtualDriveA == lpPathName[0];
}

bool CCdRom::IsVirutalDiskW(LPCWSTR lpPathName) {
	return virtualDriveW == lpPathName[0];
}

void CCdRom::PlayAudio() {
	m_pPlayer->Play();
}

void CCdRom::SeekAudioTrack(DWORD toTrackNo) {
	m_pPlayer->SetTrack(m_diskNo, toTrackNo);
}

void CCdRom::StopAudio() {
	m_pPlayer->Stop();
}

DWORD CCdRom::GetTotalTrack() {
	return m_pPlayer->GetTotalTrack(m_diskNo);
}

DWORD CCdRom::GetTrackLength(DWORD track) {
	return m_pPlayer->GetTrackLength(m_diskNo, track);
}

bool CCdRom::GetLabel(LPCSTR lpRootPathName, char buff[], DWORD size) {
	if (!IsVirutalDiskA(lpRootPathName))
		return false;

	sprintf_s(buff, size, "ZEPHYR%d", m_diskNo);
}

bool CCdRom::ResolveFileA(LPCSTR lpSourceFileName, LPCSTR lpDestinatonFileName) {
	if (lpSourceFileName[0] == virtualDriveA) {
		return m_pFs->ResolveFileA(m_diskNo, lpSourceFileName, lpDestinatonFileName);
	}

	return false;
}
bool CCdRom::ResolveFileW(LPCWSTR lpSourceFileName, LPCWSTR lpDestinatonFileName) {
	if (lpSourceFileName[0] == virtualDriveW) {
		return m_pFs->ResolveFileW(m_diskNo, lpSourceFileName, lpDestinatonFileName);
	}

	return false;
}