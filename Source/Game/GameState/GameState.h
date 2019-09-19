#ifndef __Game_State_h__
#define __Game_State_h__

#include "../game.h"


class CBaseGameState
{
private:

public:

	//Systems
	CRenderInterface* pRenderInterface;
	InputManager * pInput;
	CSoundSystem * pSound;


	//Profile stuff
	CGameProfile *pGameInfo;

	//Game
	Game* pGame;

	float m_fTimeDelta;

	//Create Objects
	virtual void Initalize(Game* game) = 0;

	//Game Logic
	virtual void Update(float timeDelta) = 0;

	//Clean up
	virtual void Shutdown() = 0;

	//Change the game State.
	void ChangeState(Game* game,CBaseGameState* newstate);

	CBaseGameState()
	{
		//Get the instance of the Render
		pRenderInterface	= CRenderInterface::GetInstance();
		// Get the instance of the Input
		pInput				= InputManager::GetInstance();
		// get the instance of the sound
		pSound				= CSoundSystem::GetInstance();
		// Get the instance of game data
		pGameInfo			= CGameProfile::GetInstance();
	}
};

class CBattleGameState : public CBaseGameState
{
private:

	//Instance
	static CBattleGameState* pInstance;


	//Objects

	CArena*		m_ArenaBuilding;


	//////////////////////////////////////////////////////////////////////////
	// Cameras
	//////////////////////////////////////////////////////////////////////////

	CFreeCamera* m_ArenaCamera;
	CFreeCamera* m_ArenaCamera2;
	CFreeCamera* m_CustomCamera;
	CFreeCamera* m_WinCamera;
	CFreeCamera* m_LookArroundCamera;

	//////////////////////////////////////////////////////////////////////////
	// Cameras
	//////////////////////////////////////////////////////////////////////////


	//////////////////////////////////////////////////////////////////////////
	// Balls
	//////////////////////////////////////////////////////////////////////////

	CBall* m_Balls;
	int m_NumberBalls;

	CBall m_SafeBall;

	//////////////////////////////////////////////////////////////////////////
	// Balls
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	// Targets
	//////////////////////////////////////////////////////////////////////////

	CTarget* m_Targets;
	int m_NumberTargets;

	//////////////////////////////////////////////////////////////////////////
	// Targets
	//////////////////////////////////////////////////////////////////////////
	





	//////////////////////////////////////////////////////////////////////////
	// Player stuff
	//////////////////////////////////////////////////////////////////////////

	CPlayer*	m_Players;
	CBarrell*	m_Cannons;
	CShield*	m_Shields;
	CPlayerHud* m_PlayerHuds;
	int*        m_SFXPlayerWinningChants;
	


	//////////////////////////////////////////////////////////////////////////
	// Player stuff
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	// Particles
	//////////////////////////////////////////////////////////////////////////
	
	CExplosion m_Explosion, m_Explosion2;
	CTrail	   *m_BallTrail;

	/************************************************************************/
	/* Particles                                                            */
	/************************************************************************/


	//////////////////////////////////////////////////////////////////////////
	// 2D things
	//////////////////////////////////////////////////////////////////////////


#define MENUICON_WIDTH_F 256.0f
#define MENUICON_HEIGHT_F 256.0f
#define MOVEMENT_SPEED_F 10.0f

	enum InGameOptions {RESUME = 0,RESTART,CAMERASELECT,QUIT,CAMERA1,CAMERA2,CAMERA3, NUMOPTIONS};

	CLabel*		m_CountDownNumber;
	int			m_CountDownIndices[6];
	CLabel		m_TimeDisplay;
	CLabel		m_MenuIcons[NUMOPTIONS];
	CLabel		m_BackgroundIcon;
	int m_ActiveBackground,m_InactiveBackground;
	int m_CurrentMenuSelection;
	CPhysicsTimer	m_MenuInputTimer;



	/************************************************************************/
	/* 2d things                                                            */
	/************************************************************************/


	//Physics Scene
	CPhysicsScene m_Scene;



	//////////////////////////////////////////////////////////////////////////
	// Numbers
	//////////////////////////////////////////////////////////////////////////

	// Time the controls
	CPhysicsTimer m_ControlTimer;

	//For intro countdown
	CPhysicsTimer m_IntroTimer;
	
	float m_InputFreq;
	float m_PhysicsSimSpeed;

	//State stuff
	enum BattleStates {PLAY = 0, PAUSE, COUNTDOWN, WIN, INTRO, LOOKARROUND};
	int m_CurrentBattleState;
	int m_NumberPlayers;

	//Position to look at.
	CVector3f m_LookPosition;
	int		m_WinningPlayerIndex;

	//Round 1... Fight!
	CBookKeeper m_RoundCounter;

	int m_WhoPushedStart;
	//////////////////////////////////////////////////////////////////////////
	// Numbers
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	/************************************************************************/
	/* Sound Indices                                                        */
	/************************************************************************/
	//////////////////////////////////////////////////////////////////////////

	int m_SFXShoot;
	int m_SFXFullChargeShoot;
	int m_SFXCharging;
	int m_SFXShieldOn;
	int m_SFXShieldOff;
	int m_SFXBlock;
	int m_SFXBlockNotDangerous;
	int m_SFXBlockCounter;
	int m_SFXSprint;
	int m_SFXOverHeat;
	int m_SFXPickUpBall;
	int m_SFXPauseGame;
	int m_SFXPauseSelect;
	int m_SFXPickUpCannon;

	// play a scale when the ball inactivates
#define NUM_OF_INACTIVE_BALL_SFX 8
	int m_SFXBallBackToSafeMode[NUM_OF_INACTIVE_BALL_SFX];
	int m_CurrInactiveBallSFX;
	CPhysicsTimer m_TimerRestoreDefaultInactiveBallSFX;


	int m_SFXCountDown;
	int m_SFXWinner;
	int m_SFXBallHitPlayer;
	int m_SFXPlayerHitTarget;
	int m_SFXTargetHit;
	int m_SFXBallHitHead;

	int m_SFXOutOfAmmo;
	//////////////////////////////////////////////////////////////////////////
	// Music
	int m_MUSICBackGround;

	//////////////////////////////////////////////////////////////////////////
	// special indices needed for wall reaction
	int m_BackWall;
	int m_FrontWall;
	int m_MidLinePlayer[2];

public:

	//Access
	static CBattleGameState* GetInstance();


	//Methods
	void Initalize(Game* game);
	void Update(float timeDelta);
	void Shutdown();

private:


	//Extra functions for this state.

	//Check controls here
	void Controls();

	//Yes we love to keep things clean
	void PlayerControls(int index);

	//Check for any collisions
	void CollisionDetection();

	//Run the game.
	void GameRun();

	//Pause menu upate.
	void Pause();

	//Free cam to check the scene out bitches
	void FreeLookArround();

	//Intro CountDown
	void CountDown();

	//Create the 2d stuff... menu mostly
	void InitalizeMenu();

	//Create hud stuff.
	void InitalizeHud();

	//Create Objects for the mode.
	void InitalizeObjects();

	//Reset the game.
	void Reset();

	//Set the balls up
	void SetBalls();

	//Inactive the ball.
	void InactiveBall(int index);
	// Play through the scale for SFX of inactivating the ball
	void PlaySFXInactiveBall();

	//Set the players up
	void SetAllPlayers();

	//Win stuff.
	void GameWin();

	// lets try bullet time on the last shot
	bool CheckForWinningShot(CVector3f curBallPos, CVector3f curBallVel, CVector3f curTargetPos);

	// lets try bullet time on the last shot
	bool AreWeClose(CBall * ball, CTarget* target);

};

class CMenuState : public CBaseGameState
{
	//Members
private:
	static CMenuState* pInstance;
	CFreeCamera *menuCamera;

	CPhysicsScene	menuPhysicsScene;

	CPlayer * menuPlayer;

	CLabel *pickGameButton;
	CLabel *otherButton;
	CLabel *menuIterator;
	CLabel *menuBackground;
	CLabel *pickGameText;

	CLabel m_Image;

	CLabel m_Logo;

	//Methods
	CMenuState(void){}
public:
	CMenuSystem *menus;
	int MenuToUse;
	//Instance
	static CMenuState* GetInstance();

	void Initalize(Game* game);
	void Update(float timeDelta);
	void Shutdown();
	void SetCurrentMenu(int NewMenu);
};






class CCharacterSelectState : public CBaseGameState
{
private:
	enum eModes {BATTLEMODE = 0, SOCCERMODE, NUMOFMODES};
	enum eState {SELECTCHAR = 420, SELECTMODE, SELECTEDMODE, BACK_TO_MAIN_MENU, NUMSTATES};
	enum {SELECTING_MODE = 0, SELECTING_LEVEL, SELECTING_DONE, NUMBER_OF_SELECTIONS};

	static CCharacterSelectState *pInstance;

	//////////////////////////////////////////////////////////////////////////
	// Set up the character Select 
	CFreeCamera *m_CSCamera;

	CArena		  *m_CSArena;	// the arena for the character select

	CLabel			*m_Title;				// the texture for the title
	CLabel			*m_WhatTitleSay;		// display the current text on the title

	CLabel			*m_ITIcon;				// the icon for "IT"

	CSelectOption   **m_pSelections;
	CSelectOption	*m_CharactersModels;	// show the character model on the stand

	CSelectIterator	*m_Iterators;		// the iterators for each character selects
	CLabel			*m_IteratorIcons;	// displays the icon for each player in the select screen
	CLabel			*m_SelectedLabel;	// display the name of the selected character

	CLabel			*m_HelpMarquee;		// displays a marquee for which button to display the help
	CLabel			*m_HelpScreen;		// displays the help screen a big screen displaying help info

	CLabel			*m_HelpAcceptLabel;		// display what button to select 
	CLabel			*m_HelpGoBackLabel;		// display what button to go back to main menu

	CLabel			*m_CharacterIconLabels;	// display the icon for the selected character
	CLabel		    *m_TimeLabel;		// display the time

	int				 m_iNoCharacterIconIndex;	// the index for the no selecteable character icon

	CPhysicsScene	 m_Physics;		// for collision and love

	CVector3f	   m_GridStartPos;	// where the grid starts in space
	//CVector3f	   m_CharacterIconPos;	// the starting screen position for the icons
	float		   m_SpaceingX;
	float		   m_SpaceingY;
	float		   m_SpaceingZ;		// how far apart are each selection along x and z
	int			   m_NumPlayers;
	int			   m_NumRows;
	int			   m_NumCols;
	int			   m_NumOfSelections;	// how many do we have selected

	CPhysicsTimer  m_PickYourCharacterTimer;
	int			   m_MaxTime;
	int			   m_CurTime;		// a count down from max to 0

	//////////////////////////////////////////////////////////////////////////
	// lets play a game of tag
	int			  m_iYouAreIT;			// the index for the player that is "IT"
	CPhysicsTimer m_TimerIT;			// the timing or the new IT
	//////////////////////////////////////////////////////////////////////////


	/************************************************************************/
	/* Set up The mode and level select                                               */
	/************************************************************************/
	CModeOption		*m_Modes;			// this is the list of modes

	CLabel			*m_ModeLabel;		// this is the heiarchy of the buttons
	CLabel			*m_LevelLabel;		
	CLabel			*m_DoneLabel;

	CLabel			*m_ArrowLabel;

	CLabel			*m_VSLabel;
	int				 m_iVSBackground;

	CPhysicsTimer    m_ReadyToChangeStateTimer;

	CVector3f        m_ModeSelectPosition;
	int				 m_iArrowLabelIndex;
	int				 m_iCheckArrowLabelIndex;
	int				 m_iCurrentModeDisplayed;
	int				 m_iNumberOfModes;
	int				 m_iCurrentlySelecting;	// is user selecting Mode or Level
	bool			 m_bAnimateArrows;
	/************************************************************************/
	/* End Mode select                                                      */
	/************************************************************************/

	//////////////////////////////////////////////////////////////////////////
	// Sound IDS
	int SFXCollidedAndSelectable;
	int SFXCollidedAndNotSelectable;
	int SFXPlayerReady;
	int SFXPlayersCollided;
	int SFXScrollMode;
	int SFXSelectedAndDone;
	int SFXVSClash;

	int BGMusic;
	int BGVSMusic;



	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////

	int m_iCurrentState;

	CPhysicsTimer m_Timer;

	CPhysicsTimer m_DisplayHelpTimer;

	bool m_bInitialized;
	bool m_bShowHelp;


	// private Default constructer
	CCharacterSelectState(void);

public:
	//Methods
	void Initalize(Game* game);
	void Update(float timeDelta);
	void Shutdown();

	void Run();
	void Collision();

	bool InitCharSelect();
	void InitIterators();
	void CheckControls();
	void CheckIteratorControls(int iterIndx);
	bool IsEveryOneReady();
	void SendDataToProfile();
	void StabelizeLights();
	void Display2DInfo();

	//////////////////////////////////////////////////////////////////////////
	// select Mode Controls
	void DoModeSelect();
	void ModeSelectControls(int iPlayer);
	void ModeSelected();

	void UpdateArrows();
	void UpdateTime();
	void UpdateCharacterIcons();
	void GoToSelectModeAndLevel();
	void UpdateVSBackground();

public:


	static CCharacterSelectState *GetInstance();
	static void DeleteInstance();
	~CCharacterSelectState(void);

};






class CIntroState : public CBaseGameState
{
	enum {STILLWAITING = 0, SHOWLOGOS, GOTOMAINMENU};
private:
	static CIntroState *pInstance;
	CFreeCamera		   *m_Camera;

	CPhysicsTimer		m_TimerIntro;

	CLabel				*m_PressStartLabel;
	CLabel				*m_TLKLabel;
	CLabel				*m_SplashLabel;
	int					 m_Cat5Logo,
		m_TLKLogo,
		m_BKBLogo;
	CVector3f            m_NewPosition;

	int					 m_State;
public:
	//Methods
	void Initalize(Game* game);
	void Update(float timeDelta);
	void Shutdown();

	void MoveStartLabel();

	static CIntroState * GetInstance();

};












#endif