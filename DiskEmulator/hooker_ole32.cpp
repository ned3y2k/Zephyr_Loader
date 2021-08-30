#include "pch.h"
#include <Windows.h>
#include <combaseapi.h>
#include <ddraw.h>
#include "include/detours_ext.h"
#include "ole32.h"

static HRESULT (__stdcall* TrueCoCreateInstance)(
    _In_ REFCLSID rclsid,
    _In_opt_ LPUNKNOWN pUnkOuter,
    _In_ DWORD dwClsContext,
    _In_ REFIID riid,
    _COM_Outptr_ _At_(*ppv, _Post_readable_size_(_Inexpressible_(varies))) LPVOID  FAR* ppv
) = CoCreateInstance;

static HRESULT __stdcall CoCreateInstanceHookPrc(
    _In_ REFCLSID rclsid,
    _In_opt_ LPUNKNOWN pUnkOuter,
    _In_ DWORD dwClsContext,
    _In_ REFIID riid,
    _At_(*ppv, _Post_readable_size_(_Inexpressible_(varies))) LPVOID  FAR* ppv
) {
    return TrueCoCreateInstance(rclsid, pUnkOuter, dwClsContext, riid, ppv);
}

DWORD AttachOle32() {
    DWORD error = NO_ERROR;
    error = DetourAttachAndCommit(&TrueCoCreateInstance, CoCreateInstanceHookPrc);
    return error;
}

DWORD DetachOle32() {
    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());

    DetourDetach(&TrueCoCreateInstance, CoCreateInstanceHookPrc);

    return DetourTransactionCommit();
}
