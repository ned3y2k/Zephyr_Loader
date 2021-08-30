#include "pch.h"
#include "SoundFile.h"

HRESULT DXTrace(const char* file, DWORD line, HRESULT hr, const WCHAR* msg, BOOL bPopMsgBox) {
    if (bPopMsgBox) {
        MessageBoxW(nullptr, msg, L"dxerror", MB_OK | MB_ICONERROR);
    }
    return hr;
}