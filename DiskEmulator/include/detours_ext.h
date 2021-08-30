#pragma once
#include "detours.h"

template<typename T, typename std::enable_if<DetoursIsFunctionPointer<T>::value, int>::type = 0>
LONG DetourAttachAndCommit(_Inout_ T* ppPointer, _In_ T pDetour) noexcept {
    DWORD error;
    error = DetourTransactionBegin();
    if (error) return error;

    error = DetourUpdateThread(GetCurrentThread());
    if (error) return error;

    error = DetourAttach(
        reinterpret_cast<void**>(ppPointer),
        reinterpret_cast<void*>(pDetour));
    if (error) return error;

    return DetourTransactionCommit();
}