#include "pch.h"
#include <stdexcept>
#include "OggFile.h"
#include "fileapi.h"
#include "CdAudioPlayer.info.h"
#include "CdAudioPlayer.h"

HRESULT CALLBACK GetSamples(LPBYTE lpDesBuf, const DWORD dwRequiredSamples, DWORD& dwRetSamples, LPVOID lpData)
{
	CCdAudioPlayer* player = (CCdAudioPlayer*)lpData;
	player->GetAudioSamples(lpDesBuf, dwRequiredSamples, dwRetSamples);
	return 0;
}

void CCdAudioPlayer::Initialize(HWND hwnd) {
	m_pSoundFile = new COggFile();
	m_pDSound = new CMyDirectSound();
	m_pDSound->SetCallback(&GetSamples, this);
}

void CCdAudioPlayer::SetRootPath(char* rootPath) {
	size_t rootPathLen = strlen(rootPath);
	if (rootPathLen == 0)
		return;

	InitFileList(rootPath, rootPathLen);

	const char fmtAppend[] = "\\disk\\bgm\\%s";
	const int bufferSize = rootPathLen + sizeof(fmtAppend);

	m_pPathPattern = (PSTR)calloc(bufferSize, sizeof(char));
	strcpy_s(m_pPathPattern, bufferSize, rootPath);
	strcat_s(m_pPathPattern, bufferSize, fmtAppend);
}

void CCdAudioPlayer::InitFileList(char* rootPath, size_t rootPathLen) {
	const char pattern[] = "\\disk\\bgm\\*.ogg";
	

	const size_t pFindPatternSize = rootPathLen + sizeof(pattern);
	char* pFindPattern = (char*)calloc(pFindPatternSize, sizeof(char));
	strcpy_s(pFindPattern, pFindPatternSize, rootPath);
	strcat_s(pFindPattern, pFindPatternSize, pattern);

	WIN32_FIND_DATAA findData;
	HANDLE h = FindFirstFileA(pFindPattern, &findData);
	if (h == INVALID_HANDLE_VALUE) {
		free(pFindPattern);
		throw std::invalid_argument("Dir open error");
	}

	do {
		m_fileCnt++;
	} while (FindNextFileA(h, &findData));
	FindClose(h);

	if (m_fileCnt == 0) {
		m_pFileList = nullptr;
		throw std::invalid_argument("file not found");
	}

	m_pFileList = (char**)calloc(m_fileCnt, sizeof(intptr_t));

	h = FindFirstFileA(pFindPattern, &findData);
	if (h == INVALID_HANDLE_VALUE) {
		throw std::invalid_argument("Dir open error");
	}

	int i = 0;
	do {
		int len = strlen(findData.cFileName) + 1;
		m_pFileList[i] = (char*)calloc(len, sizeof(char));
		strcpy_s(m_pFileList[i], len, findData.cFileName);
		i++;
	} while (FindNextFileA(h, &findData));
	FindClose(h);
	free(pFindPattern);
}

void CCdAudioPlayer::Release() {
	if (m_pSoundFile != nullptr) {
		delete m_pSoundFile;
		m_pSoundFile = nullptr;
	}

	if (m_pDSound != nullptr) {
		delete m_pDSound;
		m_pDSound = nullptr;
	}

	if (m_pFileList != nullptr) {
		for (int i = 0; i < m_fileCnt; i++) {
			free(m_pFileList[i]);
		}

		free(m_pFileList);
		m_pFileList = nullptr;
	}
}

void CCdAudioPlayer::SetTrack(int disk, int track) {
	Stop();
	if (disk < 1 || track < 2) return;
	
	char filePath[MAX_PATH];
	ZeroMemory(filePath, MAX_PATH);
	if (!GetFilePath(disk, track, filePath)) {
		return;
	}

	m_pSoundFile->Open(filePath, &m_WaveformatEx, read);
	m_WaveformatEx = *m_pSoundFile->GetFormat();
	m_pDSound->SetFormat(m_WaveformatEx);
}

void CCdAudioPlayer::Play() {
	if (!m_Play) {
		m_pDSound->Play();
		m_Play = true;
	}

	// 지속적으로 Play 명령이 떨어진다.
	if (m_pSoundFile->GetTotalSamples() < m_pDSound->GetSamplesPlayed()) {
		m_pDSound->Stop();
		m_pSoundFile->ResetFile();
		m_pDSound->Play();
	}

}

void CCdAudioPlayer::Stop() {
	if (m_Play) {
		m_pDSound->Stop();
		m_pSoundFile->ResetFile();
	}
	m_Play = false;
}

HRESULT CCdAudioPlayer::GetAudioSamples(LPBYTE lpDesBuf, const DWORD dwRequiredSamples, DWORD& dwRetSamples)
{
	DWORD dwRequiredBytes = 0, dwRetBytes = 0;
	WAVEFORMATEX* pWFE = m_pSoundFile->GetFormat();
	dwRequiredBytes = dwRequiredSamples * pWFE->nBlockAlign;
	m_pSoundFile->Read(lpDesBuf, dwRequiredBytes, &dwRetBytes);
	dwRetSamples = dwRetBytes / pWFE->nBlockAlign;
	dwRetSamples = dwRetBytes;
	return dwRetBytes;
}

void CCdAudioPlayer::Copy(char dest[][MAX_PATH]) {
	for (int i = 0; i < m_fileCnt; i++) {
		strcpy_s(dest[i], strlen(m_pFileList[i]), m_pFileList[i]);
	}
}

bool CCdAudioPlayer::GetFileName(int disk, int track, char destination[]) {
	if (disk < 0 || track < 0 || destination == nullptr) return false;

	char pattern[3] = { 0 };
	sprintf_s(pattern, "%02d", track);

	char* buff;
	disk--;
	disk *= 3;

	char tmp[3] = { 0 };

	size_t len;
	for (int i = 0; i < m_fileCnt; i++) {
		len = strlen(m_pFileList[i]);
		buff = m_pFileList[i];
		//memcpy_s(buff, len, m_pFileList[i], len);
		memcpy_s(tmp, 3, buff + disk, 2);
		if (strcmp(tmp, pattern) == 0) {
			memcpy_s(destination, len + 1, buff, len + 1);
			return true;
		}
	}

	return false;
}

bool CCdAudioPlayer::GetFilePath(int disk, int track, char destination[MAX_PATH]) {
	char fileName[MAX_PATH] = { 0 };
	int r = GetFileName(disk, track, fileName);
	if (!r)
		return r;

	r = sprintf_s(destination, MAX_PATH, m_pPathPattern, fileName);

	return r>0;
}

DWORD CCdAudioPlayer::GetTotalTrack(DWORD disk) {
	DWORD cnt = diskTrackCount[disk - 1];

	return cnt;
}

DWORD CCdAudioPlayer::GetTrackLength(DWORD disk, DWORD track) {
	DWORD length = diskTrackLength[disk - 1][track - 1];
	return length;
}