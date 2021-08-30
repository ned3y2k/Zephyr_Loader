#include "pch.h"
#include <mmsystem.h>
#include <Windows.h>
#include "hooker_WinMMEx.h"
#include "include/detours_ext.h"

#pragma comment(lib, "winmm.lib")

static MMRESULT(WINAPI* TrueWaveOutOpen)(
    _Out_opt_ LPHWAVEOUT phwo,
    _In_ UINT uDeviceID,
    _In_ LPCWAVEFORMATEX pwfx,
    _In_opt_ DWORD_PTR dwCallback,
    _In_opt_ DWORD_PTR dwInstance,
    _In_ DWORD fdwOpen
    ) = waveOutOpen;

static MMRESULT(WINAPI* TrueMmioClose)(_In_ HMMIO hmmio, _In_ UINT fuClose) = mmioClose;

static UINT(WINAPI* TrueMixerGetNumDevs)(void) = mixerGetNumDevs;

static MMRESULT(WINAPI* TrueMixerGetDevCapsA)(
    _In_ UINT_PTR uMxId,
    _Out_writes_bytes_(cbmxcaps) LPMIXERCAPSA pmxcaps,
    _In_ UINT cbmxcaps
    ) = mixerGetDevCapsA;

static MMRESULT(WINAPI* TrueMixerOpen)(
    _Out_opt_ LPHMIXER phmx,
    _In_ UINT uMxId,
    _In_opt_ DWORD_PTR dwCallback,
    _In_opt_ DWORD_PTR dwInstance,
    _In_ DWORD fdwOpen
    ) = mixerOpen;


static MMRESULT(WINAPI* TrueMixerClose)(
    _In_ HMIXER hmx
    ) = mixerClose;

static MMRESULT (WINAPI* TrueMixerGetControlDetailsA)(
    _In_opt_ HMIXEROBJ hmxobj,
    _Inout_ LPMIXERCONTROLDETAILS pmxcd,
    _In_ DWORD fdwDetails
) = mixerGetControlDetailsA;

static MMRESULT (WINAPI* TrueMixerGetLineInfoA)(
    _In_opt_ HMIXEROBJ hmxobj,
    _Inout_ LPMIXERLINEA pmxl,
    _In_ DWORD fdwInfo
) = mixerGetLineInfoA;

static MMRESULT (WINAPI* TrueMixerGetLineControlsA)(
    _In_opt_ HMIXEROBJ hmxobj,
    _Inout_ LPMIXERLINECONTROLSA pmxlc,
    _In_ DWORD fdwControls
) = mixerGetLineControlsA;

static MMRESULT (WINAPI* TrueMixerSetControlDetails)(
    _In_opt_ HMIXEROBJ hmxobj,
    _In_ LPMIXERCONTROLDETAILS pmxcd,
    _In_ DWORD fdwDetails
) = mixerSetControlDetails;

static MMRESULT (WINAPI* TrueMmioGetInfo)(
    _In_ HMMIO hmmio,
    _Out_ LPMMIOINFO pmmioinfo,
    _In_ UINT fuInfo
) = mmioGetInfo;

static MMRESULT (WINAPI* TrueMmioAdvance)(
    _In_ HMMIO hmmio,
    _In_opt_ LPMMIOINFO pmmioinfo,
    _In_ UINT fuAdvance
) = mmioAdvance;

static  MMRESULT (WINAPI* TrueMmioSetInfo)(
    _In_ HMMIO hmmio,
    _In_ LPCMMIOINFO pmmioinfo,
    _In_ UINT fuInfo
) = mmioSetInfo;

static LONG(WINAPI* TrueMmioSeek)(
    _In_ HMMIO hmmio,
    _In_ LONG lOffset,
    _In_ int iOrigin
) = mmioSeek;

static HMMIO(WINAPI* TrueMmioOpenA)(
    _Inout_updates_bytes_opt_(128) LPSTR pszFileName,
    _Inout_opt_ LPMMIOINFO pmmioinfo,
    _In_ DWORD fdwOpen
) = mmioOpenA;

static MMRESULT(WINAPI* TrueMmioDescend)(
    _In_ HMMIO hmmio,
    _Inout_ LPMMCKINFO pmmcki,
    _In_opt_ const MMCKINFO  FAR* pmmckiParent,
    _In_ UINT fuDescend
) = mmioDescend;

static LONG(WINAPI* TrueMmioRead)(
    _In_ HMMIO hmmio,
    _Out_writes_bytes_(cch) HPSTR pch,
    _In_ LONG cch
) = mmioRead;

static MMRESULT(WINAPI* TrueMmioAscend)(
    _In_ HMMIO hmmio,
    _In_ LPMMCKINFO pmmcki,
    _In_ UINT fuAscend
) = mmioAscend;

static  MMRESULT(WINAPI* TrueWaveOutReset)(
    _In_ HWAVEOUT hwo
) = waveOutReset;

static BOOL(WINAPI* TrueSndPlaySoundA)(
    _In_opt_ LPCSTR pszSound,
    _In_ UINT fuSound
) = sndPlaySoundA;

static MMRESULT(WINAPI* TrueWaveOutPrepareHeader)(
    _In_ HWAVEOUT hwo,
    _Inout_updates_bytes_(cbwh) LPWAVEHDR pwh,
    _In_ UINT cbwh
) = waveOutPrepareHeader;


static MMRESULT(WINAPI* TrueWaveOutWrite)(
    _In_ HWAVEOUT hwo,
    _Inout_updates_bytes_(cbwh) LPWAVEHDR pwh,
    _In_ UINT cbwh
) = waveOutWrite;

static MMRESULT(WINAPI* TrueWaveOutUnprepareHeader)(
    _In_ HWAVEOUT hwo,
    _Inout_updates_bytes_(cbwh) LPWAVEHDR pwh,
    _In_ UINT cbwh
) = waveOutUnprepareHeader;

MMRESULT WINAPI WaveOutOpenHookProc (
    _Out_opt_ LPHWAVEOUT phwo,
    _In_ UINT uDeviceID,
    _In_ LPCWAVEFORMATEX pwfx,
    _In_opt_ DWORD_PTR dwCallback,
    _In_opt_ DWORD_PTR dwInstance,
    _In_ DWORD fdwOpen
    ) {
    return TrueWaveOutOpen(phwo, uDeviceID, pwfx, dwCallback, dwInstance, fdwOpen);
}

MMRESULT WINAPI MmioCloseHookProc(_In_ HMMIO hmmio, _In_ UINT fuClose) {
    return TrueMmioClose(hmmio, fuClose);
}

UINT WINAPI MixerGetNumDevsHookProc(void) {
    return TrueMixerGetNumDevs();
}

MMRESULT WINAPI MixerGetDevCapsAHookProc(
    _In_ UINT_PTR uMxId,
    _Out_writes_bytes_(cbmxcaps) LPMIXERCAPSA pmxcaps,
    _In_ UINT cbmxcaps
) {
    return TrueMixerGetDevCapsA(uMxId, pmxcaps, cbmxcaps);
}

MMRESULT WINAPI MixerOpenHookProc(
    _Out_opt_ LPHMIXER phmx,
    _In_ UINT uMxId,
    _In_opt_ DWORD_PTR dwCallback,
    _In_opt_ DWORD_PTR dwInstance,
    _In_ DWORD fdwOpen
) {
    return TrueMixerOpen(phmx, uMxId, dwCallback, dwInstance, fdwOpen);
}

MMRESULT WINAPI MixerCloseHookProc(_In_ HMIXER hmx) { return TrueMixerClose(hmx); }

MMRESULT WINAPI MixerGetControlDetailsAHookProc(
    _In_opt_ HMIXEROBJ hmxobj,
    _Inout_ LPMIXERCONTROLDETAILS pmxcd,
    _In_ DWORD fdwDetails
) {
    return TrueMixerGetControlDetailsA(hmxobj, pmxcd, fdwDetails);
}

MMRESULT WINAPI MixerGetLineInfoAHookProc(
    _In_opt_ HMIXEROBJ hmxobj,
    _Inout_ LPMIXERLINEA pmxl,
    _In_ DWORD fdwInfo
) {
    return TrueMixerGetLineInfoA(hmxobj, pmxl, fdwInfo);
}

MMRESULT WINAPI MixerGetLineControlsAHookProc(
    _In_opt_ HMIXEROBJ hmxobj,
    _Inout_ LPMIXERLINECONTROLSA pmxlc,
    _In_ DWORD fdwControls
) {
    return TrueMixerGetLineControlsA(hmxobj, pmxlc, fdwControls);
}

MMRESULT WINAPI MixerSetControlDetailsHookProc(
    _In_opt_ HMIXEROBJ hmxobj,
    _In_ LPMIXERCONTROLDETAILS pmxcd,
    _In_ DWORD fdwDetails
) {
    return TrueMixerSetControlDetails(hmxobj, pmxcd, fdwDetails);
}

MMRESULT WINAPI MmioGetInfoHookProc(
    _In_ HMMIO hmmio,
    _Out_ LPMMIOINFO pmmioinfo,
    _In_ UINT fuInfo
) {
    return TrueMmioGetInfo(hmmio, pmmioinfo, fuInfo);
}

MMRESULT WINAPI MmioAdvanceHookProc(
    _In_ HMMIO hmmio,
    _In_opt_ LPMMIOINFO pmmioinfo,
    _In_ UINT fuAdvance
) {
    return TrueMmioAdvance(hmmio, pmmioinfo, fuAdvance);
}

 MMRESULT WINAPI MmioSetInfoHookProc(
    _In_ HMMIO hmmio,
    _In_ LPCMMIOINFO pmmioinfo,
    _In_ UINT fuInfo
) {
    return TrueMmioSetInfo(hmmio, pmmioinfo, fuInfo);
}

LONG WINAPI MmioSeekHookProc(
    _In_ HMMIO hmmio,
    _In_ LONG lOffset,
    _In_ int iOrigin
) {
    return TrueMmioSeek(hmmio, lOffset, iOrigin);
}

HMMIO WINAPI MmioOpenAHookProc(
    _Inout_updates_bytes_opt_(128) LPSTR pszFileName,
    _Inout_opt_ LPMMIOINFO pmmioinfo,
    _In_ DWORD fdwOpen
) {
    return TrueMmioOpenA(pszFileName, pmmioinfo, fdwOpen);
}

MMRESULT WINAPI MmioDescendHookProc(
    _In_ HMMIO hmmio,
    _Inout_ LPMMCKINFO pmmcki,
    _In_opt_ const MMCKINFO  FAR* pmmckiParent,
    _In_ UINT fuDescend
) {
    return TrueMmioDescend(hmmio, pmmcki, pmmckiParent, fuDescend);
}

LONG WINAPI MmioReadHookProc(
    _In_ HMMIO hmmio,
    _Out_writes_bytes_(cch) HPSTR pch,
    _In_ LONG cch
) {
    return TrueMmioRead(hmmio, pch, cch);
}

MMRESULT WINAPI MmioAscendHookProc(
    _In_ HMMIO hmmio,
    _In_ LPMMCKINFO pmmcki,
    _In_ UINT fuAscend
) {
    return TrueMmioAscend(hmmio, pmmcki, fuAscend);
}

 MMRESULT WINAPI WaveOutResetHookProc(
    _In_ HWAVEOUT hwo
) {
    return TrueWaveOutReset(hwo);
}

BOOL WINAPI SndPlaySoundAHookProc(
    _In_opt_ LPCSTR pszSound,
    _In_ UINT fuSound
) {
    return TrueSndPlaySoundA(pszSound, fuSound);
}

MMRESULT WINAPI WaveOutPrepareHeaderHookProc(
    _In_ HWAVEOUT hwo,
    _Inout_updates_bytes_(cbwh) LPWAVEHDR pwh,
    _In_ UINT cbwh
) {
    return TrueWaveOutPrepareHeader(hwo, pwh, cbwh);
}


MMRESULT WINAPI WaveOutWriteHookProc(
    _In_ HWAVEOUT hwo,
    _Inout_updates_bytes_(cbwh) LPWAVEHDR pwh,
    _In_ UINT cbwh
) {
    return TrueWaveOutWrite(hwo, pwh, cbwh);
}

MMRESULT WINAPI WaveOutUnprepareHeaderHookProc(
    _In_ HWAVEOUT hwo,
    _Inout_updates_bytes_(cbwh) LPWAVEHDR pwh,
    _In_ UINT cbwh
) {
    return TrueWaveOutUnprepareHeader(hwo, pwh, cbwh);
}

DWORD attachWinMMEx() {
    DWORD err;

    err = DetourAttachAndCommit(&TrueWaveOutOpen, WaveOutOpenHookProc);
    if (err) return err;

    err = DetourAttachAndCommit(&TrueMmioClose, MmioCloseHookProc);
    if (err) return err;

    err = DetourAttachAndCommit(&TrueMixerGetNumDevs, MixerGetNumDevsHookProc);
    if (err) return err;

    err = DetourAttachAndCommit(&TrueMixerGetDevCapsA, MixerGetDevCapsAHookProc);
    if (err) return err;

    err = DetourAttachAndCommit(&TrueMixerOpen, MixerOpenHookProc);
    if (err) return err;

    err = DetourAttachAndCommit(&TrueMixerClose, MixerCloseHookProc);
    if (err) return err;

    err = DetourAttachAndCommit(&TrueMixerGetControlDetailsA, MixerGetControlDetailsAHookProc);
    if (err) return err;

    err = DetourAttachAndCommit(&TrueMixerGetLineInfoA, MixerGetLineInfoAHookProc);
    if (err) return err;

    err = DetourAttachAndCommit(&TrueMixerGetLineControlsA, MixerGetLineControlsAHookProc);
    if (err) return err;

    err = DetourAttachAndCommit(&TrueMixerSetControlDetails, MixerSetControlDetailsHookProc);
    if (err) return err;

    err = DetourAttachAndCommit(&TrueMmioGetInfo, MmioGetInfoHookProc);
    if (err) return err;

    err = DetourAttachAndCommit(&TrueMmioAdvance, MmioAdvanceHookProc);
    if (err) return err;

    err = DetourAttachAndCommit(&TrueMmioSetInfo, MmioSetInfoHookProc);
    if (err) return err;

    err = DetourAttachAndCommit(&TrueMmioSeek, MmioSeekHookProc);
    if (err) return err;

    err = DetourAttachAndCommit(&TrueMmioOpenA, MmioOpenAHookProc);
    if (err) return err;

    err = DetourAttachAndCommit(&TrueMmioDescend, MmioDescendHookProc);
    if (err) return err;

    err = DetourAttachAndCommit(&TrueMmioRead, MmioReadHookProc);
    if (err) return err;

    err = DetourAttachAndCommit(&TrueMmioAscend, MmioAscendHookProc);
    if (err) return err;

    err = DetourAttachAndCommit(&TrueWaveOutReset, WaveOutResetHookProc);
    if (err) return err;

    err = DetourAttachAndCommit(&TrueSndPlaySoundA, SndPlaySoundAHookProc);
    if (err) return err;

    err = DetourAttachAndCommit(&TrueWaveOutPrepareHeader, WaveOutPrepareHeaderHookProc);
    if (err) return err;

    err = DetourAttachAndCommit(&TrueWaveOutWrite, WaveOutWriteHookProc);
    if (err) return err;

    err = DetourAttachAndCommit(&TrueWaveOutUnprepareHeader, WaveOutUnprepareHeaderHookProc);
    if (err) return err;

    return 0;
}

DWORD detachWinmmEx() {
    DWORD err;

    DetourUpdateThread(GetCurrentThread());

    DetourDetach(&TrueWaveOutOpen, WaveOutOpenHookProc);
    DetourDetach(&TrueMmioClose, MmioCloseHookProc);
    DetourDetach(&TrueMixerGetNumDevs, MixerGetNumDevsHookProc);
    DetourDetach(&TrueMixerGetDevCapsA, MixerGetDevCapsAHookProc);
    DetourDetach(&TrueMixerOpen, MixerOpenHookProc);
    DetourDetach(&TrueMixerClose, MixerCloseHookProc);
    DetourDetach(&TrueMixerGetControlDetailsA, MixerGetControlDetailsAHookProc);
    DetourDetach(&TrueMixerGetLineInfoA, MixerGetLineInfoAHookProc);
    DetourDetach(&TrueMixerGetLineControlsA, MixerGetLineControlsAHookProc);
    DetourDetach(&TrueMixerSetControlDetails, MixerSetControlDetailsHookProc);
    DetourDetach(&TrueMmioGetInfo, MmioGetInfoHookProc);
    DetourDetach(&TrueMmioAdvance, MmioAdvanceHookProc);
    DetourDetach(&TrueMmioSetInfo, MmioSetInfoHookProc);
    DetourDetach(&TrueMmioSeek, MmioSeekHookProc);
    DetourDetach(&TrueMmioOpenA, MmioOpenAHookProc);
    DetourDetach(&TrueMmioDescend, MmioDescendHookProc);
    DetourDetach(&TrueMmioRead, MmioReadHookProc);
    DetourDetach(&TrueMmioAscend, MmioAscendHookProc);
    DetourDetach(&TrueWaveOutReset, WaveOutResetHookProc);
    DetourDetach(&TrueSndPlaySoundA, SndPlaySoundAHookProc);
    DetourDetach(&TrueWaveOutPrepareHeader, WaveOutPrepareHeaderHookProc);
    DetourDetach(&TrueWaveOutWrite, WaveOutWriteHookProc);
    DetourDetach(&TrueWaveOutUnprepareHeader, WaveOutUnprepareHeaderHookProc);

    return DetourTransactionCommit();
}