#pragma once
#include "SoundFile.h"
#include "include/vorbis/codec.h"
#include "include/vorbis/vorbisfile.h"

#pragma comment(lib, "lib/libogg")
#pragma comment(lib, "lib/libvorbis_static")
#pragma comment(lib, "lib/libvorbisfile_static")

class COggFile : public CSoundFile {
private:
    FILE*           file;
    OggVorbis_File* m_pOggVirbisFile;			// OggVorbis 파일 구조체
    vorbis_info*    m_pVorbisInfo;
    ogg_int64_t pcmTotal;
    DWORD position;

    HRESULT ReadInfo();
public:
    HRESULT SetBeginSamples(DWORD dwBeginSamples);
    COggFile();
    ~COggFile();

    HRESULT Open(LPSTR strFileName, WAVEFORMATEX* pwfx, SOUND_FILE_MODE mode);
    HRESULT Close();
    HRESULT Read(BYTE* pBuffer, DWORD dwSizeToRead, DWORD* pdwSizeRead);

    DWORD   GetTotalSamples();
    HRESULT ResetFile();
};