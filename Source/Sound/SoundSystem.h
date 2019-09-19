#ifndef __SoundSystem_h__
#define __SoundSystem_h__

///////////////////////////////////////////////////////////
// File: CSoundSystem.h 
// Author: Francesco Rinaldi (FR)
// Creation Date: 8/3/2004            
// Purpose: Direct sound Wrapper
//	  
////////////////////////////////////////////////////////////

#include <stdio.h>			// for FILE and sprintf
#include <dsound.h>			// needed for dsound apps
#include <vector>
#include <string>
using namespace std;

#pragma comment (lib, "dsound.lib")  // contains dsound func defs
#pragma comment (lib, "dxguid.lib")	 // contains DX GUID's

#include "mp3.h"
#include "../Utilities/Queue.h"

class CSoundSystem
{
private:
	static CSoundSystem *pInstance;

	LPDIRECTSOUND8		m_dsObj;	    // main dsound object
	WAVEFORMATEX		m_waveFormatEx; // stores wave info
	DSBUFFERDESC		m_dsbd;			// describes a buffer
	HWND				m_hWnd;			// window handler

	struct tWAV
	{
		LPDIRECTSOUNDBUFFER	m_dsBuffer;			// stores the wave data		
		char sFileName[128];					// fileName for wave
	};

	struct tMP3
	{
		char   sFileName[128];
	};

	struct tChunkHeader
	{
		char cType[4];		// name of the chunk.
		long lSize;			// Size of chunk, in bytes
	};
	
	struct tSFX
	{
		int SFXindex;
		bool looping;
		bool interupt;
	};

	// Sound FX are loaded as .wavs
	// WAVs can stay loaded in memory as much as we need
	vector<tWAV> m_ListOfSFX;

	// Storage for a collection of mp3 songs for BGM or jukebox
	vector<tMP3> m_ListOfMusicFiles;
	
	Queue <tSFX> m_CurrentFrameSFXs;
	Queue <tSFX> m_CurrentFrameStopSFXs;

	Mp3		m_CurrMusicTrack;
	unsigned int m_currMusicFileIndex;

	LONG	m_CurrentVolumeSFX;
	LONG	m_MinVolumeSFX;

	bool m_IsMusicPlaying;		// check to see if the mp3 is playing
	bool m_DoWeWantToPlayMusic;

private:
	CSoundSystem(void);

	//////////////////////////////////////
	// function : playSFX
	// input	: int (index ID), bool (loop?)
	// return	: none
	// purpose	: to play sound
	////////////////////////////////////
	void PlaySFX(int nIndex, bool bLoop = false);
	//////////////////////////////////////
	// function : PlayMusic
	// input	: void
	// return	: void
	// purpose	: to play the current music track
	////////////////////////////////////
	void PlayMusic();


public:

#define MAX_VOLUME 0
#define MIN_VOLUME -9600

	// sets the volume for all SFXs
	void SetVolumeSFX(float vol);
	void SetVolumeForASFX(int indx, float vol);
	void WantToPlayMusic(bool bplay)
	{
		m_DoWeWantToPlayMusic = bplay;
	}

	// update will go through the accumulated SFX and play them
	void Update();

	// In: the indx of the SFX
	bool IsSFXPlaying(int indx);

	// clears the queue of the current frames SFXs
	void ChangingState();

	// accessors /////////
	HWND getHwnd(void) { return m_hWnd; }
	////////////////////////

	// modifiers //////////
	void setHwnd(HWND hwnd) { m_hWnd = hwnd; }
	///////////////////////

	//////////////////////////////////////
	// function : PlayQSFX
	// input	: int (index ID), bool (loop?)
	// return	: none
	// purpose	: add a SFX to the queue to play sound
	////////////////////////////////////
	void PlayQSFX(int nIndex, bool interupt = false, bool bLoop = false);

	///////////////////////////////////
	// function : stopQSFX
	// input		: int (index ID)
	// return		: none
	// purpose	: to stop sound in the queue
	///////////////////////////////////
	void StopQSFX(int nIndex);
	///////////////////////////////////
	// function : stopSFX
	// input		: int (index ID)
	// return		: none
	// purpose	: to stop sound
	///////////////////////////////////
	void StopSFX(int nIndex);
	//////////////////////////////////////
	// function : StopMusic
	// input	: void
	// return	: none
	// purpose	: to stop current music track
	////////////////////////////////////
	void StopMusic();


	//////////////////////////////////////
	// function : initDirectSound(void)
	// input	: none
	// return	: none
	// purpose	: to initialize Direct Sound
	///////////////////////////////////////
	void InitSoundSystem(HWND winHandle);

	//////////////////////////////////////
	// function : loadWave
	// input	: string
	// return	: int/ the id of the wav
	// purpose	: to be able to load a wave file
	////////////////////////////////////////
	int LoadWave(char *fileName);

	//////////////////////////////////////////////////////////////////////////
	// function	: AddSFX
	// input	: char *
	// return	: int the id for the SFX
	// purpose	: so the game can create sfx
	//////////////////////////////////////////////////////////////////////////
	int AddSFX(char * filename);

	//////////////////////////////////////////////////////////////////////////
	// function	: AddMusic
	// input	: char *
	// return	: int the id for the music Track
	// purpose	: so the game can store music tracks
	//////////////////////////////////////////////////////////////////////////
	int AddMusic(char * filename);

	//////////////////////////////////////////////////////////////////////////
	// function	: ChangeMusicTrack
	// input	: int newTrack index
	// return	: void
	// purpose	: change a track in the music list
	//////////////////////////////////////////////////////////////////////////
	void ChangeMusicTrack(int newTrack);

	//////////////////////////////////////////////////////////////////////////
	// function	: NextMusicTrack
	// input	: void
	// return	: void
	// purpose	: play the next track on list
	//////////////////////////////////////////////////////////////////////////
	void NextMusicTrack();

	//////////////////////////////////////////////////////////////////////////
	// function	: ClearSFX
	// input	: void
	// return	: void
	// purpose	: to Free up memory from the SFX list
	//////////////////////////////////////////////////////////////////////////
	void ClearSFX(void);

	//////////////////////////////////////////////////////////////////////////
	// function	: ClearMusic
	// input	: void
	// return	: void
	// purpose	: to Free up memory from the Music list
	//////////////////////////////////////////////////////////////////////////
	void ClearMusic(void);

	//////////////////////////////////////////////////////////////////////////
	// function	: ClearAll
	// input	: void
	// return	: void
	// purpose	: to Free up memory from both list
	//////////////////////////////////////////////////////////////////////////
	void ClearAll(void);

	///////////////////////////////////////
	// function : shutdownDirectSound
	// input	: none
	// return	: none
	// purpose	: to stop and release the buffer
	/////////////////////////////////////////
	void ShutdownSoundSystem(void);

	//////////////////////////////////////
	// Function : getInstance()
	// Input	: none
	// return	: Address of the instance of the class
	// purpose	: makes the class the first time it is called
	//				and then returns the address of the class.
	//////////////////////////////////////////////////////
	static CSoundSystem* GetInstance(void);
	// copy constructor
	CSoundSystem(const CSoundSystem&);
	// ass operator
	CSoundSystem& operator=(const CSoundSystem&);

	virtual ~CSoundSystem(void);

};

#endif