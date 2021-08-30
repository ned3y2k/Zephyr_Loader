#include "pch.h"
#include <Windows.h>
#include "hooker_user32.h"
#include "include/detours_ext.h"
#include "CdRom.h"
#include "KeyModer.h"

static HWND
(WINAPI* TrueCreateWindowExA)(
    _In_ DWORD dwExStyle,
    _In_opt_ LPCSTR lpClassName,
    _In_opt_ LPCSTR lpWindowName,
    _In_ DWORD dwStyle,
    _In_ int X,
    _In_ int Y,
    _In_ int nWidth,
    _In_ int nHeight,
    _In_opt_ HWND hWndParent,
    _In_opt_ HMENU hMenu,
    _In_opt_ HINSTANCE hInstance,
    _In_opt_ LPVOID lpParam) = CreateWindowExA;

static LONG (WINAPI* TrueChangeDisplaySettingsExA)(
    _In_opt_ LPCSTR lpszDeviceName,
    _In_opt_ DEVMODEA* lpDevMode,
    _Reserved_ HWND hwnd,
    _In_ DWORD dwflags,
    _In_opt_ LPVOID lParam) = ChangeDisplaySettingsExA;

LONG WINAPI ChangeDisplaySettingsExAHookProc (
    _In_opt_ LPCSTR lpszDeviceName,
    _In_opt_ DEVMODEA* lpDevMode,
    _Reserved_ HWND hwnd,
    _In_ DWORD dwflags,
    _In_opt_ LPVOID lParam) {

    if (lpDevMode == nullptr) {
        lpDevMode->dmPelsWidth = 1366;
        lpDevMode->dmPelsHeight = 768;
    }

    return TrueChangeDisplaySettingsExA(lpszDeviceName, lpDevMode, hwnd, dwflags, lParam);
}

HWND WINAPI CreateWindowExAHookProc(
    _In_ DWORD dwExStyle,
    _In_opt_ LPCSTR lpClassName,
    _In_opt_ LPCSTR lpWindowName,
    _In_ DWORD dwStyle,
    _In_ int X,
    _In_ int Y,
    _In_ int nWidth,
    _In_ int nHeight,
    _In_opt_ HWND hWndParent,
    _In_opt_ HMENU hMenu,
    _In_opt_ HINSTANCE hInstance,
    _In_opt_ LPVOID lpParam) {
    
/*
    CCdRom::GetIntance()->SetAudioHandle(
        TrueCreateWindowExA(dwExStyle, lpClassName, "ZephyrHook", dwStyle, X, Y, nWidth, nHeight, nullptr, nullptr, nullptr, nullptr)
    );
    */
    HWND gameHandle = TrueCreateWindowExA(dwExStyle, lpClassName, lpWindowName, dwStyle, X, Y, nWidth, nHeight, hWndParent, hMenu, hInstance, lpParam);;
    CCdRom::GetIntance()->SetAudioHandle(gameHandle);
    CKeyModer::GetInstance()->Initialize(gameHandle);

    return gameHandle;
}

DWORD AttachUser32() {
    DWORD error = NO_ERROR;
    //error = DetourAttachAndCommit(&TrueChangeDisplaySettingsExA, ChangeDisplaySettingsExAHookProc);

    error = DetourAttachAndCommit(&TrueCreateWindowExA, CreateWindowExAHookProc);
    if (error) return error;

    return error;
}

DWORD DetachUser32() {
    DWORD error = NO_ERROR;

    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());

    //DetourDetach(&TrueChangeDisplaySettingsExA, ChangeDisplaySettingsExAHookProc);
    DetourDetach(&TrueCreateWindowExA, CreateWindowExAHookProc);

    return DetourTransactionCommit();
}