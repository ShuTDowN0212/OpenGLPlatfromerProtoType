//	Wave & mp3 load and play (Header)
//	Dept. Software Convergence, Kyung Hee University
//	Prof. Daeho Lee, nize@khu.ac.kr
#pragma once

class CWave {
public:
	WAVEFORMATEX m_WaveFormatEx; 
	int m_nSampleCnt;
	short *m_pSoundData;

	HWAVEOUT m_hPlay;
	WAVEHDR m_WaveHdr;

	CWave() {
		m_pSoundData = NULL;
		m_nSampleCnt = 0;
	}

	~CWave() {
		if(m_pSoundData) delete [] m_pSoundData;
	}

	void Init(int nSampleCnt)
	{
		m_nSampleCnt = nSampleCnt;

		m_pSoundData = new short[m_nSampleCnt];
	}
};

void PlayWave(CWave *pWave);
bool LoadWave(char *FileName, CWave *pWave);
bool LoadMp3(char *FileName, CWave *pWave);
