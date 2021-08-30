#include "pch.h"
#include "CppUnitTest.h"
#include <Windows.h>
#include <CommCtrl.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

#pragma comment(lib, "winmm")

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

// 후킹을 해서 WindowProc 을 처리 하는 방법이 있다... speed.h 참조
// 또는 WNDCLASSA.lpfnWndProc에 함수 포인터를 할당한다.;
HWND GetConsoleHwnd(void);

namespace PracticeDiskEmulator
{
	TEST_CLASS(PracticeMci)
	{
    public:
        BEGIN_TEST_METHOD_ATTRIBUTE(PracticeMci_Process)
            TEST_IGNORE()
        END_TEST_METHOD_ATTRIBUTE()
        TEST_METHOD(PracticeMci_Process) {
            WNDCLASSA wc = { };

            wc.lpfnWndProc = WindowProc;
            wc.hInstance = 0;
            wc.lpszClassName = "Sample Window Class";
            RegisterClassA(&wc);

            HANDLE hnd = CreateWindowA(wc.lpszClassName, "test", CS_HREDRAW | CS_VREDRAW, 0, 0, 300, 300, 0, 0, 0, 0);
            DWORD err = GetLastError();

            MCI_OPEN_PARMS openParam;
            DWORD_PTR cb = 0; // (DWORD_PTR)hnd

            openParam.wDeviceID = 1008;
            openParam.dwCallback = cb;
            WCHAR deviceType[] = L"cdaudio";
            openParam.lpstrDeviceType = deviceType;
            WCHAR elementName[] = L"E:\\";
            openParam.lpstrElementName = elementName;

            MCIERROR r;
            DWORD flag = MCI_OPEN_ELEMENT | MCI_OPEN_TYPE;
            r = mciSendCommandW(0, MCI_OPEN, flag, (DWORD)&openParam);            
            err = GetLastError();
            if (r || err)
                return;


            MCI_SET_PARMS setParam;
            setParam.dwCallback = cb;
            setParam.dwAudio = 0;
            setParam.dwTimeFormat = 0xA;
            // 0x400 MCI_SET_TIME_FORMAT  Set the time format to minute/second/frame (MSF) format
            r = mciSendCommandW(openParam.wDeviceID, MCI_SET, MCI_SET_TIME_FORMAT, (DWORD)&setParam);
            err = GetLastError();
            if (r || err)
                return;

            MCI_STATUS_PARMS statusParam;
            statusParam.dwCallback = cb;
            statusParam.dwItem = MCI_STATUS_NUMBER_OF_TRACKS;
            statusParam.dwReturn = 0x20d;
            statusParam.dwTrack = 0x400;

            if (r = mciSendCommand(openParam.wDeviceID, MCI_STATUS,
                MCI_STATUS_ITEM, (DWORD)&statusParam))
            {
                r = mciSendCommand(openParam.wDeviceID, MCI_CLOSE, 0, NULL);
                err = GetLastError();
                if (r || err)
                    return;
            }
            err = GetLastError();
            if (r || err)
                return;

            int iNumTracks;

            iNumTracks = (int)statusParam.dwReturn;

            statusParam.dwItem = MCI_STATUS_LENGTH;

            for (int i = 1; i <= iNumTracks; i++) {
                statusParam.dwTrack = i;
                statusParam.dwReturn = 0xfff0000;
                r = mciSendCommandA(openParam.wDeviceID, MCI_STATUS, MCI_STATUS_ITEM | MCI_TRACK, (DWORD)&statusParam);
                if (r)
                    DebugBreak();
                //printf("track: %d, %08x\n", i, statusParam.dwReturn);
            }
		}
	};
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    return 0;
}