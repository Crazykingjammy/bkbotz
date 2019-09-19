///////////////////////////////////////////////////////////
// File: "CSoundSystem.cpp"
// Author: Francesco Rinaldi (FR)
// Creation Date: 8/3/2004 
// Modified Date: 1/29/06
// Purpose: this class initializes the the interface for 
//			loading waves *and now mp3s* it is a singlton and a "wrapper" 
//          also stores all necessary *audio/ SFX and music* files
////////////////////////////////////////////////////////////

#include ".\SoundSystem.h"
// messageBox macro
#define MSBOX(a)	MessageBox(m_hWnd, a, "Error", MB_OK);
// macro to safely release a COM object
#ifndef  SAFE_RELEASE
#define SAFE_RELEASE(p) if (p) {p->Release(); p = NULL; }
#endif 

CSoundSystem* CSoundSystem::pInstance = 0;


void CSoundSystem::InitSoundSystem(HWND winHandle)
{
	if (winHandle)
		m_hWnd = winHandle;
	DirectSoundCreate8(NULL, &m_dsObj, NULL);
	m_dsObj->SetCooperativeLevel(m_hWnd, DSSCL_NORMAL);

}

int CSoundSystem::AddSFX(char * filename)
{
	// we want to not load the same file more than once
 	for (unsigned int i = 0; i < m_ListOfSFX.size(); i++)
	{
		if (!strcmp(filename, m_ListOfSFX[i].sFileName))
		{
			return (int)i;			
		}
	}
	LoadWave(filename);
	int count = (int)m_ListOfSFX.size();
	return (count-1);
}

int CSoundSystem::AddMusic(char * filename)
{
	// we want to not load the same file more than once
	unsigned int size = (unsigned int)m_ListOfMusicFiles.size();
	char *pCompareFileName;
	for (unsigned int i = 0; i < size; i++)
	{
		pCompareFileName = m_ListOfMusicFiles[i].sFileName;
		if (!strcmp(filename, pCompareFileName))
		{
			return (int)i;
		}
	}
	tMP3 newMusic;
	strcpy(newMusic.sFileName, filename);
	m_ListOfMusicFiles.push_back(newMusic);


	return ((int)m_ListOfMusicFiles.size()-1);
}

void CSoundSystem::ClearSFX()
{
	for (unsigned int i = 0; i < m_ListOfSFX.size(); i++)
	{
		if (m_ListOfSFX[i].m_dsBuffer)
		{
			m_ListOfSFX[i].m_dsBuffer->Stop();
			SAFE_RELEASE(m_ListOfSFX[i].m_dsBuffer);
			memset(m_ListOfSFX[i].sFileName, 0, 
					sizeof(m_ListOfSFX[i].sFileName));
		}
	}
	m_ListOfSFX.clear();
}
void CSoundSystem::ChangingState()
{
	m_CurrentFrameSFXs.clear();

	for (int i = 0; i < (int)m_ListOfSFX.size(); i++)
	{
		StopSFX(i);
	}
}
void CSoundSystem::ClearMusic()
{
	for (unsigned int i = 0; i < m_ListOfMusicFiles.size(); i++)
	{
		memset(m_ListOfMusicFiles[i].sFileName, 0, 
			sizeof(m_ListOfMusicFiles[i].sFileName));
	} 
	m_ListOfMusicFiles.clear();
}

void CSoundSystem::ClearAll()
{
	ClearSFX();
	ClearMusic();
}

void CSoundSystem::SetVolumeSFX(float vol)
{
	m_CurrentVolumeSFX = (LONG)vol;
	// -9600 to 0		0 is max volume
	if (m_CurrentVolumeSFX > 0)
		m_CurrentVolumeSFX = 0;
	if (m_CurrentVolumeSFX < m_MinVolumeSFX)
		m_CurrentVolumeSFX = m_MinVolumeSFX;
	
	for (int i = 0; i < (int)m_ListOfSFX.size(); i++)
	{
		m_ListOfSFX[i].m_dsBuffer->SetVolume(m_CurrentVolumeSFX);
	}
}

void CSoundSystem::SetVolumeForASFX(int indx, float vol)
{
	if (indx < 0 || indx < (int)m_ListOfSFX.size())
		return;
	// -9600 to 0		0 is max volume
	if (vol > 0)
		vol = 0;
	if (vol < (float)m_MinVolumeSFX)
		vol = (float)m_MinVolumeSFX;

	m_ListOfSFX[indx].m_dsBuffer->SetVolume((LONG)vol);
}


void CSoundSystem::PlayQSFX(int nIndex, bool interupt, bool bLoop /* = false */)
{
	if (nIndex < 0 || nIndex > (int)m_ListOfSFX.size())
		return;

	tSFX newSFX;
	newSFX.looping = bLoop;
	newSFX.SFXindex = nIndex;
	newSFX.interupt = interupt;

	m_CurrentFrameSFXs.enqueue(newSFX);

}

bool CSoundSystem::IsSFXPlaying(int indx)
{
	if (indx < 0 || indx > (int)m_ListOfSFX.size())
		return false;

	DWORD status;
	m_ListOfSFX[indx].m_dsBuffer->GetStatus(&status);
	if (status & DSBSTATUS_PLAYING || (status & DSBSTATUS_LOOPING) )
		return true;
	
	
	return false;
}

void CSoundSystem::Update()
{
	tSFX SFXToPlay;
	while(!m_CurrentFrameSFXs.isEmpty())
	{
		if (m_CurrentFrameSFXs.dequeue(SFXToPlay))
		{
			if (IsSFXPlaying(SFXToPlay.SFXindex))
			{
				if (SFXToPlay.interupt)
				{		
					m_ListOfSFX[SFXToPlay.SFXindex].m_dsBuffer->SetCurrentPosition(0);
				}
			}
			else
			{
				PlaySFX(SFXToPlay.SFXindex, SFXToPlay.looping);		
			}
		}
	}

	if (m_DoWeWantToPlayMusic)
	{
		PlayMusic();
	}
	else
	{
		StopMusic();
	}

	//while(!m_CurrentFrameStopSFXs.isEmpty())
	//{
	//	if(m_CurrentFrameStopSFXs.dequeue(SFXToPlay))
	//	{
	//		StopSFX(SFXToPlay.SFXindex);
	//	}
	//}
}

void CSoundSystem::PlaySFX(int nIndex, bool bLoop)
{
	unsigned int size = (unsigned int)m_ListOfSFX.size();
	if ((unsigned int)nIndex > size || nIndex < 0)
	{
		return;
	}
	LPDIRECTSOUNDBUFFER	*soundBuffer;
	soundBuffer = &m_ListOfSFX[nIndex].m_dsBuffer;

	if (soundBuffer)
	{
		if (bLoop)
			m_ListOfSFX[nIndex].m_dsBuffer->Play(0, 0, DSBPLAY_LOOPING);
		else
		{
			m_ListOfSFX[nIndex].m_dsBuffer->Play(0, 0, 0);
		}
	}
}

void CSoundSystem::StopQSFX(int nIndex)
{
	if (nIndex < 0 || nIndex > (int)m_ListOfSFX.size())
		return;
	tSFX stop;
	stop.SFXindex = nIndex;
	m_CurrentFrameStopSFXs.enqueue(stop);
}

void CSoundSystem::StopSFX(int nIndex)
{
	if ((unsigned int)nIndex > m_ListOfSFX.size() || nIndex < 0)
	{
		return;
	}
	m_ListOfSFX[nIndex].m_dsBuffer->Stop();
	m_ListOfSFX[nIndex].m_dsBuffer->SetCurrentPosition(0);
}

void CSoundSystem::PlayMusic()
{
	if (!m_IsMusicPlaying)
	{
		if (m_CurrMusicTrack.IsReady())
		{
			m_CurrMusicTrack.Play();
			m_IsMusicPlaying = true;
		}
	}
}

void CSoundSystem::StopMusic()
{
	if (m_IsMusicPlaying)
	{
		m_CurrMusicTrack.Stop();
		m_IsMusicPlaying = false;
	}
}

void CSoundSystem::ChangeMusicTrack(int newTrack)
{
	if (m_IsMusicPlaying)
	{
		if (newTrack == m_currMusicFileIndex)
			return;
	}
	if ((unsigned int)newTrack > m_ListOfMusicFiles.size() || newTrack < 0)
		return;
	
	StopMusic();
	//m_CurrMusicTrack.Cleanup();
	m_CurrMusicTrack.Load(m_ListOfMusicFiles[newTrack].sFileName);
	m_currMusicFileIndex = newTrack;
}

void CSoundSystem::NextMusicTrack()
{
	unsigned int size = (unsigned int)m_ListOfMusicFiles.size();
	if (size <= 0)
		return;

	int nextTrackIndx = (m_currMusicFileIndex+1);
	// if we have music and the next index is greater than the size
	// and we want to loop play the first track in the list
	if (nextTrackIndx >= (int)size)
	{ 
		ChangeMusicTrack(0);
		return;
	}
	ChangeMusicTrack(nextTrackIndx);
}

void CSoundSystem::ShutdownSoundSystem(void)
{
	// clean the sfx list
	ClearSFX();
	// clear the music list
	ClearMusic();
	// release dx obj
	SAFE_RELEASE(m_dsObj);
}

CSoundSystem *CSoundSystem::GetInstance(void)
{
	if(pInstance == 0)
		pInstance = new CSoundSystem;
	return pInstance;
}

int CSoundSystem::LoadWave(char *fileName)
{
	FILE *pFile = NULL;			// pointer to a file
	char szBuffer[32] = {0};	// store file data

	// variables used for locking the buffer
	unsigned char *pAudio1 = NULL;			// pointer to first write buffer
	unsigned char *pAudio2 = NULL;			// pointer to 2nd write buffer
	unsigned long ulAudioLength1 = 0;		// length of 1st pointer
	unsigned long ulAudioLength2 = 0;		// length of 2nd pointer

	// open the file for reading in binary.
	pFile = fopen(fileName, "rb");
	if (!pFile)
	{
		strcpy(szBuffer, fileName);
		strcat(szBuffer, " Not Found");
		MSBOX(szBuffer);
		return -1;
	}

	// read in the header information
	fread(szBuffer, 12, 1, pFile);

	// make sure the header information is valid.
	if (memcmp(szBuffer, "RIFF", 4) || memcmp(szBuffer+8, "WAVE", 4))
	{
		MSBOX("Invalid header information");
		fclose(pFile);
		return -1;
	}

	// retrieve the format chunk information
	tChunkHeader fmtChunk;
	fread(&fmtChunk, sizeof(fmtChunk), 1, pFile);

	// make sure this is actually the format chunk
	if (memcmp(fmtChunk.cType, "fmt ", 4) || fmtChunk.lSize < 16)
	{
		MSBOX("Invalid format Chunk");
		fclose(pFile);
		return -1;
	}

	// store the WAVEFORMATEX information
	memset(&m_waveFormatEx, 0, sizeof(m_waveFormatEx));
	fread(&m_waveFormatEx.wFormatTag,			sizeof(short), 1, pFile);
	fread(&m_waveFormatEx.nChannels,				sizeof(short), 1, pFile);
	fread(&m_waveFormatEx.nSamplesPerSec,		sizeof(int),		1, pFile);
	fread(&m_waveFormatEx.nAvgBytesPerSec,		sizeof(int),		1, pFile);
	fread(&m_waveFormatEx.nBlockAlign,				sizeof(short),	1, pFile);
	fread(&m_waveFormatEx.wBitsPerSample,		sizeof(short),	1, pFile);

	// make sure the file is encoded in PCM format.
	if (m_waveFormatEx.wFormatTag != WAVE_FORMAT_PCM)
	{
		MSBOX("Invalid PCM format");
		fclose(pFile);
		return -1;
	}

	// make sure the bitpersample is valid
	if ((m_waveFormatEx.wBitsPerSample != 8) &&
		(m_waveFormatEx.wBitsPerSample != 16))
	{
		MSBOX("Invalid Bits per Sample - nust be 8 or 16");
		fclose(pFile);
		return -1;
	}

	// skip the remaining WAVEFORMATEX bytes
	for (int i = fmtChunk.lSize - 16; i > 0; i--)
		fread(szBuffer, 1, 1, pFile);

	// read in the next chunk
	tChunkHeader chunk;
	fread(&chunk, sizeof(chunk), 1, pFile);

	// skip all other chunks except the data
	while(memcmp(chunk.cType, "data", 4))
	{
		// read past unwanted data.
		for (int i = 0; i < chunk.lSize; i++)
			fread(szBuffer, 1, 1, pFile);

		// read in the next chunk.
		fread(&chunk, sizeof(chunk), 1, pFile);
	}

	// make memory for the wave file
	BYTE *pData = new BYTE[chunk.lSize];

	// make sure memory was allocated.
	if (!pData)
	{
		MSBOX("Not enough memory");
		fclose(pFile);
		return -1;
	}

	// read the data from the file
	fread(pData, sizeof(BYTE), chunk.lSize, pFile);

	// done with the file
	fclose(pFile);

	// 1. initialize the DSBUFFERDESC structure before use
	memset(&m_dsbd, 0, sizeof(m_dsbd));
	m_dsbd.dwSize = sizeof(m_dsbd);

	// 2. set the control flags for the buffer
	m_dsbd.dwFlags = DSBCAPS_LOCSOFTWARE | DSBCAPS_CTRLFREQUENCY |
		DSBCAPS_CTRLVOLUME	 | DSBCAPS_CTRLPAN;

	// set the size of the buffer in bytes.
	m_dsbd.dwBufferBytes = chunk.lSize;

	// give the buffer the description of the wave file
	m_dsbd.lpwfxFormat = &m_waveFormatEx;

	// 3. Create the sound buffer
	tWAV tempBuffer;
	tempBuffer.m_dsBuffer = NULL;
	strcpy(tempBuffer.sFileName, fileName);
	//tempBuffer.sFileName = fileName;
	if (FAILED(m_dsObj->CreateSoundBuffer(&m_dsbd, &tempBuffer.m_dsBuffer, NULL)))
	{
		MSBOX("Failed to create soundBuffer");
		delete [] pData;
		return -1;
	}

	// 4. lock the buffer.
	if (FAILED(tempBuffer.m_dsBuffer->Lock(0, chunk.lSize, (void **)&pAudio1, 
		&ulAudioLength1, (void **)&pAudio2, 
		&ulAudioLength2, 
		DSBLOCK_FROMWRITECURSOR)))
	{
		MSBOX("Failed to lock the buffer");
		delete [] pData;
		return -1;	
	}

	// 5. copy the data into the buffer from these pointers
	memcpy(pAudio1, pData, ulAudioLength1);
	memcpy(pAudio2, pData + ulAudioLength1, ulAudioLength2);

	// 6. unlock the buffer
	if (FAILED(tempBuffer.m_dsBuffer->Unlock(pAudio1, ulAudioLength1, 
		pAudio2, ulAudioLength2)))
	{
		MSBOX("Failed to unlock");
		delete [] pData;
		return -1;
	}

	// 7. Delete the Data from system memory
	delete [] pData;
	tempBuffer.m_dsBuffer->SetVolume(m_CurrentVolumeSFX);
	m_ListOfSFX.push_back(tempBuffer);

	return ((int)m_ListOfSFX.size()-1);

}

CSoundSystem::CSoundSystem(void)
{
	m_currMusicFileIndex = 0;
	m_CurrentVolumeSFX	 = MAX_VOLUME;
	m_MinVolumeSFX		 = MIN_VOLUME;
	m_IsMusicPlaying = false;
	m_DoWeWantToPlayMusic = true;
}

CSoundSystem::~CSoundSystem(void)
{
}
