#include "pch.h"
#include <Windows.h>
#include <unordered_map>
#include <stdexcept>
#include <fileapi.h>
#include "include/detours_ext.h"
#include "CdRom.h"
#include "file.util.h"
#include "hooker_fileapi.h"

std::unordered_map<intptr_t, LPCSTR> handleMap;

static char executePathFmtA[MAX_PATH] = { 0 };

static DWORD (WINAPI* TrueGetLogicalDrives) (VOID) = GetLogicalDrives;
static UINT (WINAPI* TrueGetDriveTypeA) (_In_opt_ LPCSTR lpRootPathName) = GetDriveTypeA;
static HANDLE (WINAPI* TrueCreateFileW) (
    _In_ LPCWSTR lpFileName,
    _In_ DWORD dwDesiredAccess,
    _In_ DWORD dwShareMode,
    _In_opt_ LPSECURITY_ATTRIBUTES lpSecurityAttributes,
    _In_ DWORD dwCreationDisposition,
    _In_ DWORD dwFlagsAndAttributes,
    _In_opt_ HANDLE hTemplateFile
) = CreateFileW;


static BOOL
(WINAPI* TrueGetVolumeInformationA)
(   _In_opt_ LPCSTR lpRootPathName,
    _Out_writes_opt_(nVolumeNameSize) LPSTR lpVolumeNameBuffer,
    _In_ DWORD nVolumeNameSize,
    _Out_opt_ LPDWORD lpVolumeSerialNumber,
    _Out_opt_ LPDWORD lpMaximumComponentLength,
    _Out_opt_ LPDWORD lpFileSystemFlags,
    _Out_writes_opt_(nFileSystemNameSize) LPSTR lpFileSystemNameBuffer,
    _In_ DWORD nFileSystemNameSize
) = GetVolumeInformationA;

static HANDLE (WINAPI* TrueCreateFileA)(_In_ LPCSTR lpFileName,
    _In_ DWORD dwDesiredAccess,
    _In_ DWORD dwShareMode,
    _In_opt_ LPSECURITY_ATTRIBUTES lpSecurityAttributes,
    _In_ DWORD dwCreationDisposition,
    _In_ DWORD dwFlagsAndAttributes,
    _In_opt_ HANDLE hTemplateFile) = CreateFileA;

BOOL (WINAPI* TrueCloseHandle) (_In_ _Post_ptr_invalid_ HANDLE hObject) = CloseHandle;

/// <summary>
/// DX로 로드 하는 파일들... 때문에.. 동영상이 여기 해당
/// </summary>
/// <param name="lpFileName"></param>
/// <param name="dwDesiredAccess"></param>
/// <param name="dwShareMode"></param>
/// <param name="lpSecurityAttributes"></param>
/// <param name="dwCreationDisposition"></param>
/// <param name="dwFlagsAndAttributes"></param>
/// <param name="hTemplateFile"></param>
/// <returns></returns>
DECLSPEC_EXPORT HANDLE WINAPI CreateFileWHookProc(
    _In_ LPCWSTR lpFileName,
    _In_ DWORD dwDesiredAccess,
    _In_ DWORD dwShareMode,
    _In_opt_ LPSECURITY_ATTRIBUTES lpSecurityAttributes,
    _In_ DWORD dwCreationDisposition,
    _In_ DWORD dwFlagsAndAttributes,
    _In_opt_ HANDLE hTemplateFile
) {
    WCHAR buff[MAX_PATH];
    if (CCdRom::GetIntance()->ResolveFileW(lpFileName, buff)) {
        return TrueCreateFileW(buff,
            dwDesiredAccess,
            dwShareMode,
            lpSecurityAttributes,
            dwCreationDisposition,
            dwFlagsAndAttributes,
            hTemplateFile);
    }

    return TrueCreateFileW(lpFileName,
        dwDesiredAccess,
        dwShareMode,
        lpSecurityAttributes,
        dwCreationDisposition,
        dwFlagsAndAttributes,
        hTemplateFile);
}

DECLSPEC_EXPORT HANDLE WINAPI CreateFileAHookProc(
    _In_ LPCSTR lpFileName,
    _In_ DWORD dwDesiredAccess,
    _In_ DWORD dwShareMode,
    _In_opt_ LPSECURITY_ATTRIBUTES lpSecurityAttributes,
    _In_ DWORD dwCreationDisposition,
    _In_ DWORD dwFlagsAndAttributes,
    _In_opt_ HANDLE hTemplateFile
) {
    if (CompareExtensionName(lpFileName, "exe") && !CCdRom::GetIntance()->IsRootPathInitialized()) {
        Dirname(lpFileName, executePathFmtA, MAX_PATH);
        CCdRom::GetIntance()->InititalizeRootPath(executePathFmtA);
        strcat_s(executePathFmtA, MAX_PATH, "\\%s");
    }

    char buff[MAX_PATH];
    if (lpFileName[1] != ':') {
        CCdRom::GetIntance()->DecisionDisk(lpFileName);
        
        sprintf_s(buff, MAX_PATH, executePathFmtA, lpFileName);
        return TrueCreateFileA(buff,
            dwDesiredAccess, dwShareMode,
            lpSecurityAttributes,
            dwCreationDisposition, dwFlagsAndAttributes,
            hTemplateFile);
    }

    if (CCdRom::GetIntance()->ResolveFileA(lpFileName, buff)) {
        return TrueCreateFileA(buff,
            dwDesiredAccess, dwShareMode,
            lpSecurityAttributes,
            dwCreationDisposition, dwFlagsAndAttributes,
            hTemplateFile);   
    }       

    return TrueCreateFileA(lpFileName,
        dwDesiredAccess, dwShareMode,
        lpSecurityAttributes, 
        dwCreationDisposition, dwFlagsAndAttributes, 
        hTemplateFile);
}


DECLSPEC_EXPORT BOOL WINAPI GetVolumeInformationAHookProc(
    _In_opt_ LPCSTR lpRootPathName,
    _Out_writes_opt_(nVolumeNameSize) LPSTR lpVolumeNameBuffer,
    _In_ DWORD nVolumeNameSize,
    _Out_opt_ LPDWORD lpVolumeSerialNumber,
    _Out_opt_ LPDWORD lpMaximumComponentLength,
    _Out_opt_ LPDWORD lpFileSystemFlags,
    _Out_writes_opt_(nFileSystemNameSize) LPSTR lpFileSystemNameBuffer,
    _In_ DWORD nFileSystemNameSize
) {
    if (CCdRom::GetIntance()->GetLabel(lpRootPathName, lpVolumeNameBuffer, nVolumeNameSize)) {
       return true;
    }

    return TrueGetVolumeInformationA(lpRootPathName,
        lpVolumeNameBuffer,
        nVolumeNameSize,
        lpVolumeSerialNumber,
        lpMaximumComponentLength,
        lpFileSystemFlags,
        lpFileSystemNameBuffer,
        nFileSystemNameSize);
}

DECLSPEC_EXPORT UINT WINAPI GetDriveTypeAHookProc(_In_opt_ LPCSTR lpRootPathName) {
    if(CCdRom::GetIntance()->IsVirutalDiskA(lpRootPathName))
        return DRIVE_CDROM;

    return TrueGetDriveTypeA(lpRootPathName);
}
DECLSPEC_EXPORT DWORD WINAPI GetLogicalDrivesHookProc(VOID) {
    DWORD r = TrueGetLogicalDrives();
    return 0x65535;
}

DECLSPEC_EXPORT BOOL WINAPI CloseHandleHookProc(_In_ _Post_ptr_invalid_ HANDLE hObject) {
    return TrueCloseHandle(hObject);
}


DWORD AttachFileApi()
{
    DWORD error = NO_ERROR;

    error = DetourAttachAndCommit(&TrueGetLogicalDrives, GetLogicalDrivesHookProc);
    if (error) return error;

    error = DetourAttachAndCommit(&TrueGetDriveTypeA, GetDriveTypeAHookProc);
    if (error) return error;

    error = DetourAttachAndCommit(&TrueGetVolumeInformationA, GetVolumeInformationAHookProc);
    if (error) return error;

    error = DetourAttachAndCommit(&TrueCreateFileW, CreateFileWHookProc);
    if (error) return error;

    error = DetourAttachAndCommit(&TrueCreateFileA, CreateFileAHookProc);
    if (error) return error;

    error = DetourAttachAndCommit(&TrueCloseHandle, CloseHandleHookProc);
    if (error) return error;
}

DWORD DetachFileApi() {
    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());

    DetourDetach(&TrueGetLogicalDrives, GetLogicalDrivesHookProc);
    DetourDetach(&TrueGetDriveTypeA, GetDriveTypeAHookProc);
    DetourDetach(&TrueGetVolumeInformationA, GetVolumeInformationAHookProc);
    DetourDetach(&TrueCreateFileW, CreateFileWHookProc);
    DetourDetach(&TrueCreateFileA, CreateFileAHookProc);
    DetourDetach(&TrueCloseHandle, CloseHandleHookProc);

    return DetourTransactionCommit();
}