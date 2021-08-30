#include "pch.h"
#include "hooker_time.h"
#include "include/detours_ext.h"
#include "KeyModer.h"
#include <timeapi.h>


static long orgWndPrc;

static DWORD(WINAPI* TrueTimeGetTime)(void) = timeGetTime;

DWORD WINAPI TimeGetTimeHookProc(void) {
	auto moder = CKeyModer::GetInstance();

	moder->ProcKeyEvent();
    return TrueTimeGetTime() * moder->GetSpeed();
}

DWORD AttachTime() {
    DWORD err;

    err = DetourAttachAndCommit(&TrueTimeGetTime, TimeGetTimeHookProc);
    if (err) return err;

    return 0;
}

DWORD DetachTime() {
    //UnhookWindowsHookEx(hhk);

    DetourUpdateThread(GetCurrentThread());

    DetourDetach(&TrueTimeGetTime, TimeGetTimeHookProc);
    return DetourTransactionCommit();
}