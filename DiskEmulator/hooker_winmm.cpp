#pragma once
#include "pch.h"
#include "hooker_winmm.h"
#include <mmsystem.h>
#include <Windows.h>
#include <stdio.h>
#include "include/detours_ext.h"
#include "CdRom.h"
#include "fileapi.h"

#pragma comment(lib, "winmm.lib")

// uMsg = https://docs.microsoft.com/ko-kr/windows/win32/multimedia/multimedia-commands?redirectedfrom=MSDN
// https://docs.microsoft.com/en-us/previous-versions/dd757160(v=vs.85)
// https://petra.tistory.com/865
// https://docs.microsoft.com/en-us/windows/win32/multimedia/retrieving-compact-disc-track-specific-information
// https://202psj.tistory.com/84
// https://www.codeproject.com/Articles/8396/Using-DirectSound-to-Play-Audio-Stream-Data

static MCIERROR (WINAPI* TrueMciSendCommandW)(
    _In_ MCIDEVICEID mciId,
    _In_ UINT uMsg,
    _In_opt_ DWORD_PTR dwParam1,
    _In_opt_ DWORD_PTR dwParam2
) = mciSendCommandW;

static MCIERROR (WINAPI*TrueMciSendCommandA) (
    _In_ MCIDEVICEID mciId,
    _In_ UINT uMsg,
    _In_opt_ DWORD_PTR dwParam1,
    _In_opt_ DWORD_PTR dwParam2
) = mciSendCommandA;

static MCIERROR (WINAPI* TrueMciSendStringA)(
    _In_ LPCSTR lpstrCommand,
    _Out_writes_opt_(uReturnLength) LPSTR lpstrReturnString,
    _In_ UINT uReturnLength,
    _In_opt_ HWND hwndCallback
) = mciSendStringA;


MCIERROR _MciOpen(MCIDEVICEID mciId, UINT uMsg, DWORD dwParam1, MCI_OPEN_PARMS* dwParam2);
MCIERROR _MciStatus(MCIDEVICEID mciId, UINT uMsg, DWORD dwParam1, MCI_STATUS_PARMS* dwParam2);
MCIERROR _MciClose(MCIDEVICEID mciId, UINT uMsg, DWORD dwParam1, MCI_GENERIC_PARMS* dwParam2);
MCIERROR _MciSet(MCIDEVICEID mciId, UINT uMsg, DWORD dwParam1, MCI_SET_PARMS* dwParam2);
MCIERROR _MciPlay(MCIDEVICEID mciId, UINT uMsg, DWORD dwParam1, MCI_PLAY_PARMS* dwParam2);
MCIERROR _MciSeek(MCIDEVICEID mciId, UINT uMsg, DWORD dwParam1, MCI_SEEK_PARMS* dwParam2);
MCIERROR _MciStop(MCIDEVICEID mciId, UINT uMsg, DWORD dwParam1, MCI_GENERIC_PARMS* dwParam2);

MCIERROR WINAPI MciSendCommandWHookProc(
    _In_ MCIDEVICEID mciId,
    _In_ UINT uMsg,
    _In_opt_ DWORD_PTR dwParam1,
    _In_opt_ DWORD_PTR dwParam2
    ) {
    
    TrueMciSendCommandW(mciId, uMsg, dwParam1, dwParam2);

    return 0;
}

MCIERROR WINAPI MciSendCommandAHookProc(
    _In_ MCIDEVICEID mciId,
    _In_ UINT uMsg,
    _In_opt_ DWORD_PTR dwParam1,
    _In_opt_ DWORD_PTR dwParam2
    ) {

    switch (uMsg) {
        case MCI_OPEN: // ???
            return _MciOpen(mciId, uMsg, dwParam1, (MCI_OPEN_PARMS*)dwParam2);
            // 0x465a9f
            // dwParam1 MCI_OPEN_ELEMENT | MCI_OPEN_TYPE
        case MCI_SET:
            return _MciSet(mciId, uMsg, dwParam1, (MCI_SET_PARMS*)dwParam2);
        case MCI_STATUS: // 0x465667
            return _MciStatus(mciId, uMsg, dwParam1, (MCI_STATUS_PARMS*)dwParam2); 
        case MCI_CLOSE: // 0x465765
            return _MciClose(mciId, uMsg, dwParam1, (MCI_GENERIC_PARMS*)dwParam2);
            // dwParam1 MCI_WAIT
            //DebugBreak();
            break;
        case MCI_PLAY:
            return _MciPlay(mciId, uMsg, dwParam1, (MCI_PLAY_PARMS*)dwParam2);
        case MCI_SEEK:
            return _MciSeek(mciId, uMsg, dwParam1, (MCI_SEEK_PARMS*)dwParam2);
        case MCI_STOP:
            return _MciStop(mciId, uMsg, dwParam1, (MCI_GENERIC_PARMS*)dwParam2);
        default:
            DebugBreak();
    }
    
    
    return 0; // MCIERR_INVALID_DEVICE_ID 256 + 1 
}

MCIERROR _MciOpen(MCIDEVICEID mciId, UINT uMsg, DWORD dwParam1, MCI_OPEN_PARMS* dwParam2) {
    char* deviceType = (char*)dwParam2->lpstrDeviceType;
    char* elementName = (char*)dwParam2->lpstrElementName;
    
    //TrueMciSendCommandA(mciId, uMsg, dwParam1, (DWORD_PTR)dwParam2);

    dwParam2->wDeviceID = 1;
    //DebugBreak();
    return 0;
}

MCIERROR _MciStatus(MCIDEVICEID mciId, UINT uMsg, DWORD dwParam1, MCI_STATUS_PARMS* dwParam2) {
    if (dwParam1 == MCI_STATUS_ITEM) {
        dwParam2->dwReturn = CCdRom::GetIntance()->GetTotalTrack();
    } else if(dwParam1 == (MCI_STATUS_ITEM | MCI_TRACK) && dwParam2->dwItem == MCI_STATUS_LENGTH) {
        dwParam2->dwReturn = CCdRom::GetIntance()->GetTrackLength(dwParam2->dwTrack);
    }
    else {
        //DebugBreak();
    }
    return 0;
}

MCIERROR _MciClose(MCIDEVICEID mciId, UINT uMsg, DWORD dwParam1, MCI_GENERIC_PARMS* dwParam2) {
    //DebugBreak();
    return 0;
}

MCIERROR _MciSet(MCIDEVICEID mciId, UINT uMsg, DWORD dwParam1, MCI_SET_PARMS* dwParam2) {
    if (dwParam1 == MCI_SET_TIME_FORMAT) { }
    else {
        //DebugBreak();
    }
    return 0;
}

MCIERROR _MciPlay(MCIDEVICEID mciId, UINT uMsg, DWORD dwParam1, MCI_PLAY_PARMS* dwParam2) {   
    //MCI_NOTIFY_SUCCESSFUL    명령을 성공적으로 수행
        //MCI_NOTIFY_FAILURE          명령 수행 실패
        //MCI_NOTIFY_SUPERSEDED    새 통지 메시지를 받은 경우
        //MCI_NOTIFY_ABORTED        새 명령을 받은 경우

    if (dwParam1 == (MCI_FROM | MCI_TO)) {
        SendMessageA((HWND)dwParam2->dwCallback, MM_MCINOTIFY, MCI_NOTIFY_SUCCESSFUL, 0);      
        CCdRom::GetIntance()->PlayAudio();
    }    
    
    //DebugBreak();
    return 0;
}

MCIERROR _MciSeek(MCIDEVICEID mciId, UINT uMsg, DWORD dwParam1, MCI_SEEK_PARMS* dwParam2) {
    if (dwParam1 == MCI_TO) {
        // 아마 이걸로 트랙 변경됨
        CCdRom::GetIntance()->SeekAudioTrack(dwParam2->dwTo);
        //DebugBreak();
    }
    else {
        //DebugBreak();
    }
    
    return 0;
}

MCIERROR _MciStop(MCIDEVICEID mciId, UINT uMsg, DWORD dwParam1, MCI_GENERIC_PARMS* dwParam2) {
    //DebugBreak();
    CCdRom::GetIntance()->StopAudio();
    return 0;
}

MCIERROR WINAPI MciSendStringAHookProc(_In_ LPCSTR lpstrCommand,
    _Out_writes_opt_(uReturnLength) LPSTR lpstrReturnString,
    _In_ UINT uReturnLength,
    _In_opt_ HWND hwndCallback) {
    DebugBreak();
    return 0;
}

DWORD AttachWinMM() {
    DWORD error = NO_ERROR;
    error = DetourAttachAndCommit(&TrueMciSendStringA, MciSendStringAHookProc);
    if (error) return error;

    error = DetourAttachAndCommit(&TrueMciSendCommandA, MciSendCommandAHookProc);
    if (error) return error;

    error = DetourAttachAndCommit(&TrueMciSendCommandW, MciSendCommandWHookProc);
    if (error) return error;

    return 0;
}

DWORD DetachWinMM() {
    DetourUpdateThread(GetCurrentThread());
    DetourDetach(&TrueMciSendStringA, MciSendStringAHookProc);
    DetourDetach(&TrueMciSendCommandA, MciSendCommandAHookProc);
    DetourDetach(&TrueMciSendCommandW, MciSendCommandWHookProc);
    return DetourTransactionCommit();
}