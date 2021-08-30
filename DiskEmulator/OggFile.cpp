#include "pch.h"
#include "OggFile.h"

COggFile::COggFile() {
	m_pWaveformatex = NULL;

	//E_NOTIMPL
}

COggFile::~COggFile() {
	Close();
}

HRESULT COggFile::Open(LPSTR strFileName, WAVEFORMATEX* pwfx, SOUND_FILE_MODE mode) {
	if (mode != SOUND_READ)
		return E_NOTIMPL;

	if (strFileName == NULL)
		return E_INVALIDARG;

	SAFE_DELETE_ARRAY(m_pWaveformatex);
	
	HRESULT hr;
	errno_t err = fopen_s(&file, strFileName, "rb");
	
	if (err) {
		return DXTRACE_ERR(TEXT("OGG OPEN FAIl"), E_FAIL);
	}

	if (FAILED(hr = ReadInfo()))
	{
		fclose(file);
		return DXTRACE_ERR_NOMSGBOX(TEXT("ReadInfo"), hr);
	}

	if (FAILED(hr = ResetFile()))
		return DXTRACE_ERR(TEXT("ResetFile"), hr);

	return S_OK;
}

HRESULT COggFile::ReadInfo() {
	m_pOggVirbisFile = new OggVorbis_File();
	ZeroMemory(m_pOggVirbisFile, sizeof(OggVorbis_File));

	ov_open(file, m_pOggVirbisFile, NULL, 0);
	m_pVorbisInfo = ov_info(m_pOggVirbisFile, -1);

	m_pWaveformatex = new WAVEFORMATEX();
	ZeroMemory(m_pWaveformatex, sizeof(WAVEFORMATEX));

	m_pWaveformatex->cbSize = sizeof(WAVEFORMATEX);
	m_pWaveformatex->wBitsPerSample = 16;
	m_pWaveformatex->wFormatTag = WAVE_FORMAT_PCM; // 왜 1이지?
	
	m_pWaveformatex->nChannels = m_pVorbisInfo->channels;
	m_pWaveformatex->nSamplesPerSec = m_pVorbisInfo->rate;
	m_pWaveformatex->nAvgBytesPerSec = m_pWaveformatex->nSamplesPerSec * m_pWaveformatex->nChannels * 2;	// 테이터사이즈 셋팅
	m_pWaveformatex->nBlockAlign = (m_pVorbisInfo->channels * m_pWaveformatex->wBitsPerSample) / 8;

	pcmTotal = ov_pcm_total(m_pOggVirbisFile, -1);


	return S_OK;
}

DWORD COggFile::GetTotalSamples()
{
	return pcmTotal;
}

HRESULT COggFile::ResetFile() {
	position = 0;
	ov_time_seek(m_pOggVirbisFile, 0);
	return S_OK;
}

HRESULT COggFile::Read(BYTE* pBuffer, DWORD dwSizeToRead, DWORD* pdwSizeRead) {

	if (m_pOggVirbisFile == NULL)
		return CO_E_NOTINITIALIZED;
	if (pBuffer == NULL || pdwSizeRead == NULL)
		return E_INVALIDARG;
	if (pdwSizeRead != NULL)
		*pdwSizeRead = 0;

	int sec = 1;
	int ret = 0;
	DWORD soundPosition = position + dwSizeToRead;
	int bufferPosition = 0;
	do {
		ret = ov_read(m_pOggVirbisFile, (char*)(pBuffer + bufferPosition), soundPosition-position, 0, 2, 1, &sec);
		position += ret;
		bufferPosition += ret;
		*pdwSizeRead += ret;
	} while (ret && position < soundPosition);
	//CString strSamplesPlayed;
	//strSamplesPlayed.Format(_T("scrol sample: %d \n"), m_iBeginPlayPos);
	//OutputDebugString(strSamplesPlayed);

	return S_OK;
}

HRESULT COggFile::Close() {
	SAFE_DELETE_ARRAY(m_pWaveformatex);
	errno_t err = ov_clear(m_pOggVirbisFile);
	if (err) return E_FAIL;

	err = fclose(file);
	if (err) return E_FAIL;

	delete m_pOggVirbisFile;
	free(m_pVorbisInfo);

	return S_OK;
}
HRESULT COggFile::SetBeginSamples(DWORD dwBeginSamples) {
	position = 0;

	ov_pcm_seek(m_pOggVirbisFile, position);
	return S_OK;
}