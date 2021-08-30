#pragma once

#include <mmsystem.h>
#include <dsound.h>

#if defined(DEBUG) || defined(_DEBUG)
HRESULT DXTrace(const char* file, DWORD line, HRESULT hr, const WCHAR* msg, BOOL bPopMsgBox);

#define DXTRACE_MSG(str)              DXTrace( __FILE__, (DWORD)__LINE__, 0, str, false )
#define DXTRACE_ERR(str,hr)           DXTrace( __FILE__, (DWORD)__LINE__, hr, str, false )
#define DXTRACE_ERR_MSGBOX(str,hr)    DXTrace( __FILE__, (DWORD)__LINE__, hr, str, true )
#define DXTRACE_ERR_NOMSGBOX(str,hr)    DXTrace( __FILE__, (DWORD)__LINE__, hr, str, false )
#else
#define DXTRACE_MSG(str)              (0L)
#define DXTRACE_ERR(str,hr)           (hr)
#define DXTRACE_ERR_MSGBOX(str,hr)    (hr)
#define DXTRACE_ERR_NOMSGBOX(str,hr)    (hr)
#endif

enum SOUND_FILE_MODE {
    read = 1,
    write = 2
};
#define SOUND_READ   1
#define SOUND_WRITE  2

#define SAFE_DELETE(p)       { if(p) { delete (p);     (p)=NULL; } }
#define SAFE_DELETE_ARRAY(p) { if(p) { delete[] (p);   (p)=NULL; } }
#define SAFE_RELEASE(p)      { if(p) { (p)->Release(); (p)=NULL; } }
// S_OK
class CSoundFile {
protected:
    PWAVEFORMATEX m_pWaveformatex;        // Pointer to WAVEFORMATEX structure
public:
    PWAVEFORMATEX GetFormat() { return m_pWaveformatex; };

    virtual HRESULT SetBeginSamples(DWORD dwBeginSamples) = 0;

    virtual HRESULT Open(LPSTR strFileName, WAVEFORMATEX* pwfx, SOUND_FILE_MODE mode) = 0;
    virtual HRESULT Close() = 0;
    virtual HRESULT Read(BYTE* pBuffer, DWORD dwSizeToRead, DWORD* pdwSizeRead) = 0;

    virtual DWORD   GetTotalSamples() = 0;
    virtual HRESULT ResetFile() = 0;
};