#include "pch.h"
#include <stdio.h>
#include <stdexcept>
#include <Shlwapi.h>
#include "file.util.h"
#include "CdFs.h"

const char diskPathFmtA[] = "%d\\%s";
const WCHAR diskPathFmtW[] = L"%d\\%s";

const char commonPathFmtA[] = "common\\%s";
const WCHAR commonPathFmtW[] = L"common\\%s";

void CCdFs::Initialize(LPCSTR lpExePath)
{
    size_t exePathLen = strlen(lpExePath);
    m_pDiskPathFmtA = (char*)calloc(MAX_PATH, sizeof(char));
    strcpy_s(m_pDiskPathFmtA, MAX_PATH, lpExePath);
    strcat_s(m_pDiskPathFmtA, MAX_PATH, "\\disk\\%s");

    size_t diskFmtSize = strlen(m_pDiskPathFmtA) + 1;
    m_pDiskPathFmtW = (LPWSTR)calloc(diskFmtSize, sizeof(WCHAR));
    mbstowcs_s(nullptr, m_pDiskPathFmtW, diskFmtSize, m_pDiskPathFmtA, strlen(m_pDiskPathFmtA));
}

bool CCdFs::ResolveFileA(int requireDisk, LPCSTR lpSourceFileName, LPCSTR lpDestinatonFileName) {
    LPCSTR relativePath = lpSourceFileName + 3;

    char buff[MAX_PATH] = { 0 };
    sprintf_s(buff, MAX_PATH, m_pDiskPathFmtA, diskPathFmtA, lpSourceFileName + 3);
    sprintf_s(m_lastFileA, buff, requireDisk, relativePath);
    if (PathFileExistsA(m_lastFileA)) {
        strcpy_s((char*)lpDestinatonFileName, strlen(m_lastFileA) + 1, m_lastFileA);
        return true;
    }

    sprintf_s(buff, MAX_PATH, m_pDiskPathFmtA, commonPathFmtA, lpSourceFileName + 3);
    sprintf_s(m_lastFileA, buff, relativePath);
    if (PathFileExistsA(m_lastFileA)) {
        strcpy_s((char*)lpDestinatonFileName, strlen(m_lastFileA) + 1, m_lastFileA);
        return true;
    }

    return false;
}

bool CCdFs::ResolveFileW(int requireDisk, LPCWSTR lpSourceFileName, LPCWSTR lpDestinatonFileName) {
    LPCWSTR relativePath = lpSourceFileName + 3;
    WCHAR buff[MAX_PATH] = { 0 };

    swprintf_s(buff, MAX_PATH, m_pDiskPathFmtW, diskPathFmtW, lpSourceFileName + 3);
    swprintf_s(m_lastFileW, buff, requireDisk, relativePath);
    if (PathFileExistsW(m_lastFileW)) {
        wcscpy_s((wchar_t*)lpDestinatonFileName, wcslen(m_lastFileW) + 1, m_lastFileW);
        return true;
    }

    swprintf_s(buff, MAX_PATH, m_pDiskPathFmtW, commonPathFmtW, lpSourceFileName + 3);
    swprintf_s(m_lastFileW, buff, relativePath);
    if (PathFileExistsW(m_lastFileW)) {
        wcscpy_s((wchar_t*)lpDestinatonFileName, wcslen(m_lastFileW) + 1, m_lastFileW);
        return true;
    }

    return 0;
}