// dllmain.cpp : DLL 애플리케이션의 진입점을 정의합니다.
#include "pch.h"
#include <iostream>
#include <tlhelp32.h>
#include "include/detours.h"
#include "hooker.h"
#include "trace.h"

DWORD FindProcessId(const std::wstring& processName);

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    LONG error;
    WCHAR exePath[MAX_PATH + 1];
    DWORD len = GetModuleFileNameW(NULL, exePath, MAX_PATH);

    if (DetourIsHelperProcess()) {
        return TRUE;
    }

    if (ul_reason_for_call == DLL_PROCESS_ATTACH || ul_reason_for_call == DLL_THREAD_ATTACH) {
        DisableThreadLibraryCalls(hModule); // said to increase performance?

        // || ul_reason_for_call == DLL_THREAD_ATTACH
        AttachUser32();
        AttachFileApi();
        AttachWinMM();
        AttachTime();
        //attachWinMMEx();
    }
    else if (ul_reason_for_call == DLL_PROCESS_DETACH ) {
        //  || ul_reason_for_call == DLL_THREAD_DETACH
        DetachUser32();
        DetachFileApi();
        DetachWinMM();
        DetachTime();
        //detachWinmmEx();
    }
    return TRUE;
}

DWORD FindProcessId(const std::wstring& processName)
{
    PROCESSENTRY32 processInfo;
    processInfo.dwSize = sizeof(processInfo);

    HANDLE processesSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
    if (processesSnapshot == INVALID_HANDLE_VALUE)
        return 0;

    Process32First(processesSnapshot, &processInfo);
    if (!processName.compare(processInfo.szExeFile))
    {
        CloseHandle(processesSnapshot);
        return processInfo.th32ProcessID;
    }

    while (Process32Next(processesSnapshot, &processInfo))
    {
        if (!processName.compare(processInfo.szExeFile))
        {
            CloseHandle(processesSnapshot);
            return processInfo.th32ProcessID;
        }
    }

    CloseHandle(processesSnapshot);
    return 0;
}
