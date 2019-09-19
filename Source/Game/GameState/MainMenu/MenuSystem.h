#pragma once
#include "MenuParser.h"
#include "../../../Physics/PhysicsTimer.h"


#ifndef SUBMENU_STEPPING
#define SUBMENU_STEPPING 200
#endif

enum ButtonActionCodes
{
	ActivateSubMenu = 0,				//	00
	StartSinglePlayerGame,				//	01
	StartMultiplayerGame,				//	02
	DisplayCredits,						//	03
	QuickStartSinglePlayer,				//	04
	QuickStartMultiplayer,				//	05
	ActivateAudioMenu,					//	06
	ActivateVideoMenu,					//	07
	ActivateCredits,					//	08
	ActivateMainMenu,					//	09
	ActivateCreditsScreen,				//	10
	ActivateInGameMenu,					//	11
	ExitGame,							//	12
	NumActions
};

enum ReturnCodes
{
	Begin_New_Single_Player_Game = 0,
	Begin_New_Multiplayer_Game,
	Exit_To_MainMenu,
	Exit_Game,
	NumReturnCodes
};

struct GameData
{
	int Players;
	int Balls;
	int ThemeID;
	float HeatFactor;
};
class CMenuSystem
{
	static CMenuSystem		*pInst;

	MainMenuData			MainMenu;
	GameSelectionMenu		SinglePlayer;
	GameSelectionMenu		Multiplayer;

	bool							Status;
	bool							RunOnceDuringUpdate;
	CBitmap2D					*SaturationBombingLogo;
	CBitmap2D					*KeyTest;
	ModelData					PyramidModel;
	MenuParser					Script;
	ISAXXMLReader			*xml;

	// input instance

	// sound instance

	// Rendering stuff
	IDirect3DDevice9			*Direct3DDevice;
	ID3DXSprite				*Sprite;

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

	void Draw(int MenuToDraw);
	int ProcessInput(MenuButton *m);
	void ResetSubmenu(Submenu *s);
	void ResetAllSubmenus();
	
	void InitializeMainMenu();
	void MainMenuDraw();
	int MainMenuUpdate();
	void DrawTheGodDamnPyramid();

	void InitializeSinglePlayerGameMenu();
	void SinglePlayerGameMenuDraw();
	void SinglePlayerGameMenuUpdate();

	void InitializeMultiplayerGameMenu();
	void MultiplayerGameMenuDraw();
	void MultiplayerGameMenuUpdate();

public:
	~CMenuSystem(void);
	static CMenuSystem *GetInstance()
	{
		if(!pInst)
		{
			pInst = new CMenuSystem;
		}
		return pInst;
	}

	bool Initialize(IDirect3DDevice9 *d3ddev);
	int Update(int MenuToUse);
	bool Shutdown();
	bool IsMenuActive()		{	return Status;		}
	void TurnOffMenus()		{	Status = false;	}
	void TurnOnMenus()		{	Status = true;		}
};