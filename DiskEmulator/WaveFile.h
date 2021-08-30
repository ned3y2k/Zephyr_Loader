// WaveFile.h: interface for the CWaveFile class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WAVEFILE_H__E4AAE5DC_8AA1_4270_9BB5_43C1EEE72711__INCLUDED_)
#define AFX_WAVEFILE_H__E4AAE5DC_8AA1_4270_9BB5_43C1EEE72711__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SoundFile.h"


class CWaveFile : public CSoundFile
{
private:
    HMMIO         m_hmmio;       // MM I/O handle for the WAVE
    MMCKINFO      m_ck;          // Multimedia RIFF chunk
    MMCKINFO      m_ckRiff;      // Use in opening a WAVE file
    DWORD         m_dwSize;      // The size of the wave file
    MMIOINFO      m_mmioinfoOut;
    SOUND_FILE_MODE m_mode;
    BOOL          m_bIsReadingFromMemory;
    BYTE*         m_pbData;
    BYTE*         m_pbDataCur;
    ULONG         m_ulDataSize;
    HRESULT ReadMMIO();
    HRESULT WriteMMIO(WAVEFORMATEX* pwfxDest);
    HRESULT OpenFromMemory(BYTE* pbData, ULONG ulDataSize, WAVEFORMATEX* pwfx, SOUND_FILE_MODE mode);
    HRESULT Write(UINT nSizeToWrite, BYTE* pbData, UINT* pnSizeWrote);

public:
	HRESULT SetBeginSamples(DWORD dwBeginSamples);
    CWaveFile();
    ~CWaveFile();

    HRESULT Open( LPSTR strFileName, WAVEFORMATEX* pwfx, SOUND_FILE_MODE mode );
    HRESULT Close();
    HRESULT Read( BYTE* pBuffer, DWORD dwSizeToRead, DWORD* pdwSizeRead );

    DWORD   GetTotalSamples();
    HRESULT ResetFile();
};

#endif // !defined(AFX_WAVEFILE_H__E4AAE5DC_8AA1_4270_9BB5_43C1EEE72711__INCLUDED_)
