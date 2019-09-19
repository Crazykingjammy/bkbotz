#pragma once
//////////////////////////////////////////////////////////////////////////
// Standard Includes
#include <vector>
#include <string>
#include <MsXml.h>
#include <MsXml2.h>
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// Custom Includes
#include "./../Render/RenderInterface.h"
#include "./../Input/InputMgr.h"
#include "./../Game/GameProfiler/GameProfiler.h"
#include "./../Sound/SoundSystem.h"
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// Defines
#ifndef MAX_STRING_LENGTH
#define MAX_STRING_LENGTH 256
#endif
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// Enumerations
enum MenuIndex
{
	Main_Menu = 0,
	Audio_options,
	Controller_config,
	NumMenus
};
enum ButtonActionCodes
{
	ActivateSubMenu = 0,				//	00
	StartSinglePlayerGame,			//	01
	StartMultiplayerGame,				//	02
	DisplayCredits,						//	03
	QuickStartSinglePlayer,				//	04
	QuickStartMultiplayer,				//	05
	ActivateAudioMenu,					//	06
	ActivateControlsMenu,				//	07
	ActivateCredits,						//	08
	ActivateMainMenu,					//	09
	ActivateCreditsScreen,				//	10
	ActivateInGameMenu,				//	11
	ExitGame,								//	12
	StartSoccer,
	StartBattle,
	NumActions
};

enum ReturnCodes
{
	Begin_New_Single_Player_Game = 0,
	Begin_New_Multiplayer_Game,
	Exit_To_MainMenu,
	Exit_Game,
	ChangeToAudioMenu,
	ChangeToControlsMenu,
	ChangeToMainMenu,
	NumReturnCodes
};
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// Data Types
struct Animation
{
	int ID;
	int FrameCount;
	int CurrentFrame;
	char BackgroundPath[MAX_STRING_LENGTH];
	CLabel *Background;
	vector<char *> ImagePaths;
	vector<CLabel *> Frames;
	CLabel *CurFrame;
	float FrameTimer;
	bool Loop;
	bool IsFinished;
	bool IsPlaying;
	void Play(CRenderInterface *r,int x, int y)
	{
		if(CurrentFrame >= FrameCount || CurrentFrame < 0)
		{
			CurrentFrame = 0;
			this->Reset();
		}
		else
		{
			if(Background != NULL)
			{
				Background->m_MyBody.centerPosition.x = (float)x;
				Background->m_MyBody.centerPosition.y = (float)y;
				Background->m_Visible = true;
			}
			CurFrame = Frames[CurrentFrame];
			CurFrame->m_MyBody.centerPosition.x = (float)x;
			CurFrame->m_MyBody.centerPosition.y = (float)y;
			CurFrame->m_Visible = true;
			CurrentFrame++;
			if(CurrentFrame == FrameCount)
			{
				if(Loop)
				{
					CurrentFrame = 0;
					this->Reset();
					IsFinished = false;
					IsPlaying = true;
				}
				else
				{
					this->Reset();
					IsFinished = true;
					IsPlaying = false;
				}
			}
			else
				IsPlaying = true;
		}
	}
	void Reset()
	{
		for(unsigned int i = 0; i < Frames.size(); i++)
		{
			Frames[i]->m_Visible = false;
		}
		CurFrame = Frames[0];
	}
};

struct MenuButton
{
	int								ID;
	int								CP[2];
	int								SP[2];
	bool							HasASubmenu;
	bool							IsInASubmenu;
	bool							PlayAnimationWhenSelected;
	int								AnimationID;
	int								SubmenuID;
	int								ActionCode;
	char							SelectedImage[MAX_STRING_LENGTH];
	char							UnselectedImage[MAX_STRING_LENGTH];
	CLabel						*Selected;
	CLabel						*Unselected;
};

struct Submenu
{
	int								ID;
	bool							Active;
	bool							Animating;
	vector<int>				ButtonIDs;
	int								Openspeed;
	int								FrameCount;
	int								OpenedByButtonID;
	int								StartingPosition[2];
};

struct MainMenuData
{
	Submenu					*CurrentSubmenu;
	MenuButton				*CurrentMenuButton;

	int								ID;
	int								Type;

	vector<MenuButton>	ButtonMasterIndex;
	vector<Submenu>		SubmenuIndex;
	vector<int>				MainMenuButtonIDs;
	vector<Animation>		AnimationIndex;

	char							MenuBackgroundImage[MAX_STRING_LENGTH];
	char							UnselectedBackgroundImage[MAX_STRING_LENGTH];
	char							SelectedBackgroundImage[MAX_STRING_LENGTH];
	char							MenuBGMPath[MAX_STRING_LENGTH];
	char							SelectionChangedSoundPath[MAX_STRING_LENGTH];

	CLabel						*MenuBackground;
	CLabel						*UnselectedButtonBackground;
	CLabel						*SelectedButtonBackground;
};
struct SliderOption
{
	int								Data;
	int								Position[2];
	CLabel						*Image;
};
struct SliderMenu
{
	bool							LoopOptions;
	bool							ShowArrows;

	int								ID;
	int								LeftArrowPos[2];
	int								RightArrowPos[2];

	SliderOption				*CurrentOption;
	vector<SliderOption>	Options;
	vector<Animation>		AnimationIndex;
	CLabel						*Highlight;
	CLabel						*LeftArrow;
	CLabel						*RightArrow;

	void SelectPrevious()
	{
		for(unsigned int i = 0; i < Options.size(); i++)
		{
			if(CurrentOption == &Options[i])
			{
				if(i == 0)
				{
					if(LoopOptions)
					{
						CurrentOption = &Options[Options.size() - 1];
					}
					else
					{
						break;
					}
				}
				else
				{
					CurrentOption = &Options[i - 1];
					break;
				}
			}
		}
	}
	void SelectNext()
	{
		for(unsigned int i = 0; i < Options.size(); i++)
		{
			if(CurrentOption == &Options[i])
			{
				if(i == (Options.size() - 1))
				{
					if(LoopOptions)
					{
						CurrentOption = &Options[0];
					}
					else
					{
						break;
					}
				}
				else
				{
					CurrentOption = &Options[i+1];
					break;
				}
			}
		}
	}
	SliderOption *GetCurrentSelection()
	{
		return CurrentOption;
	}
};
struct GameSelectionMenu
{
	int								ID;
	int								Type;
	int								ObjectSpacing;
	bool							IsHorizontal;

	SliderMenu					*CurrentSlider;
	MenuButton				*CurrentButton;

	vector<MenuButton>	ButtonMasterIndex;
	vector<int>				SelectionOrder;

	CLabel						*Background;
	CLabel						*UnselectedButtonBackground;
	CLabel						*SelectedButtonBackground;
};
struct AudioMenu
{
	int								SelectionTracker;
	SliderMenu					SFXVolumeControl;
	SliderMenu					BGMVolumeControl;
	SliderMenu					SFXToggle;
	SliderMenu					BGMToggle;
	MenuButton				BackButton;
	bool							SFXStatus;
	bool							BGMStatus;
	int								SFXVolume;
	int								BGMVolume;

	CLabel						*Background;
	CLabel						*SoundEffects;
	CLabel						*Music;
	CLabel						*SoundVolumeImage;
	CLabel						*MusicVolumeImage;
	CLabel						*ToggleHighlight;
	CLabel						*VolumeHighlight;
	CLabel						*Audio;
	CLabel						*GameOptions;
	CLabel						*Rounds;
	float							SFXToggleSelectionCoords[2];
	float							SFXVolumeHighlightCoords[2];
	float							BGMToggleSelectionCoords[2];
	float							BGMVolumeHighlightCoords[2];
};
struct ControlsMenu
{
	int								p1SelectionTracker;
	int								p2SelectionTracker;
	MenuButton				p1Shoot;
	MenuButton				p1Block;
	MenuButton				p1Sprint;
	MenuButton				p1TurnLeft;
	MenuButton				p1TurnRight;
	MenuButton				p1Start;

	MenuButton				p2Shoot;
	MenuButton				p2Block;
	MenuButton				p2Sprint;
	MenuButton				p2TurnLeft;
	MenuButton				p2TurnRight;
	MenuButton				p2Start;
};
//////////////////////////////////////////////////////////////////////////

class CMenuSystem
{
	static CMenuSystem		*pInst;

	MainMenuData			MainMenu;
	
	ControlsMenu				ControllerConfigMenu;

	GameSelectionMenu		SinglePlayer;
	GameSelectionMenu		Multiplayer;
	CFreeCamera				*menuCamera;
	bool							Status;
	bool							RunOnceDuringUpdate;
	bool							firstinit;
	bool							SKIPTHEMOTHERFUCKINGINTRO;
	CLabel						*SaturationBombingLogo;
	CLabel						*KeyTest;
	CLabel						*Catagory5logo;
	CLabel						*LockingKeylogo;
	CLabel						*Creditsbackground;
	CPhysicsTimer				Cat5Timer;
	CPhysicsTimer				LogoTimer;

	CLabel						*Credits[56];
	bool							incredits;
	int								creditscounter;
	CPhysicsTimer				Creditstimer;
	// input instance
	InputManager				*pInput;

	// sound instance
	CSoundSystem			*pSound;
	int								SFXcycleButtons;
	int								SFXactiveButton;
	// Physics timer
	CPhysicsTimer				DrawTimer;
	CPhysicsTimer				AnimationTimer;
	CPhysicsTimer				InputTimer;

	CMenuSystem(void);
	static void DeleteInstance()
	{
		if(pInst)
		{
			delete pInst;
		}
	}

	void	Draw(int MenuToDraw);
	int		ProcessInput(MenuButton *m);
	void	ResetSubmenu(Submenu *s);


	void	InitializeMainMenu();
	void	MainMenuDraw();
	int		MainMenuUpdate();

	void	InitializeAudioMenu();
	void	DrawAudioMenu();
	int		AudioMenuUpdate();

	void	InitializeControlsMenu();
	void	DrawControlsMenu();
	int		ControlsMenuUpdate();

	void ResetAllAnimations();
	void ShutOffAudioMenu()
	{
		// turn off all visibility flags for the sound menu
		SoundMenu.Background->m_Visible = false;
		SoundMenu.SoundEffects->m_Visible = false;
		SoundMenu.Music->m_Visible = false;
		SoundMenu.SoundVolumeImage->m_Visible = false;
		SoundMenu.Audio->m_Visible = false;
		SoundMenu.GameOptions->m_Visible = false;
		SoundMenu.Rounds->m_Visible = false;
		SoundMenu.BGMVolumeControl.Highlight->m_Visible = false;
		SoundMenu.SFXVolumeControl.Highlight->m_Visible = false;
		SoundMenu.ToggleHighlight->m_Visible = false;
		SoundMenu.BackButton.Selected->m_Visible = false;
		SoundMenu.BackButton.Unselected->m_Visible = false;
		for(unsigned int i = 0; i < SoundMenu.SFXVolumeControl.Options.size(); i++)
		{
			SoundMenu.SFXVolumeControl.Options[i].Image->m_Visible = false;
		}
		for(unsigned int i = 0; i < SoundMenu.BGMVolumeControl.Options.size(); i++)
		{
			SoundMenu.BGMVolumeControl.Options[i].Image->m_Visible = false;
		}
		SoundMenu.SFXToggle.Highlight->m_Visible = false;
		SoundMenu.SFXToggle.Options[0].Image->m_Visible = false;
		SoundMenu.SFXToggle.Options[1].Image->m_Visible = false;

		SoundMenu.BGMToggle.Highlight->m_Visible = false;
		SoundMenu.BGMToggle.Options[0].Image->m_Visible = false;
		SoundMenu.BGMToggle.Options[1].Image->m_Visible = false;
	}
	void ShutOffMainMenu()
	{
		MainMenu.MenuBackground->m_Visible = false;
		SaturationBombingLogo->m_Visible = false;
		for(unsigned int i = 0; i < MainMenu.ButtonMasterIndex.size(); i++)
		{
			MainMenu.ButtonMasterIndex[i].Selected->m_Visible = false;
			MainMenu.ButtonMasterIndex[i].Unselected->m_Visible = false;
		}
		MainMenu.CurrentMenuButton = &MainMenu.ButtonMasterIndex[MainMenu.MainMenuButtonIDs[0]];
		ResetAllSubmenus();
	}
	void CreditsInitialize();
	void CreditsSequence();
	void HideCredits()
	{
		for(int i = 0; i < 56; i++)
		{
			Credits[i]->m_Visible = false;
		}
		Creditsbackground->m_Visible = false;
	}

public:
	AudioMenu					SoundMenu;
	void CreditsCleanup();
	void	ResetAllSubmenus();
	~CMenuSystem(void);
	static CMenuSystem *GetInstance()
	{
		if(!pInst)
		{
			pInst = new CMenuSystem;
		}
		return pInst;
	}

	bool Initialize();
	int Update(int MenuToUse);
	bool Shutdown();
};