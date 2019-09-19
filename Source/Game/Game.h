#ifndef __Game_h__
#define __Game_h__

//Header Include Files.
#include "..//main.h"
#include "..//Render/Render.h"
#include "..//Render/Direct3D/D3dMain.h"
#include "../Render/RenderInterface.h"
#include "../Object/Player/Player.h"
#include "../Object/Barrell/Barrell.h"
#include "../Object/Ball/Ball.h"
#include "../Object/2D/2D.h"
#include "../Object/2D/Hud/Hud.h"
#include "../Object/Arena/Arena.h"
#include "../Input/InputMgr.h"
#include "../render/Camera/FreeCamera.h"
#include "../Sound/SoundSystem.h"
#include "../Physics/PhysicScene.h"
#include "../Object/2D/Bar/Bar.h"
#include "../Object/Shield/Shield.h"
#include "../Object/Target/Target.h"
#include "../Object/Particles/Explosion/Explosions.h"
#include "../Object/VirtualKeyboard/VirtualKeyboard.h"
#include "GameProfiler/GameProfiler.h"
#include "BookKeeper/BookKeeper.h"
#include "../MenuSystem/MenuSystem.h"
#include "../Object/SelectOption/SelectOption.h"
#include "../Object/Particles/Explosion/Explosions.h"
#include "../Object/Particles/Trail/Trail.h"

class CBaseGameState;

struct WinStruct
{	
	//Windows Data types.

	HINSTANCE hInstance;
	WNDCLASS WndClass;
	HWND hWnd;
	HDC hDC;
	MSG Msg;

	//Parameters
	 int		WindowWidth;
	 int		WindowHeight;
	 int		WindowX;
	 int		WindowY;
	 DWORD		WindowStyle;
	 DWORD		WindowFlags;
	 char*		WindowName;
	 char*		WindowClassName;

};


class Game
{
private:

	//Render Manager
	CRenderInterface *pRenderInterface;

	//Current State
	CBaseGameState *m_CurrentState;
	
	

	CGameProfile	*m_GameProfile;
	// input instance
	InputManager *pInput;

	CSoundSystem *pSound;

public:
	
	//Window data.
	WinStruct Window;

	// timer
	float timeDelta;
	// test to see if the app is running
	bool	  isGameRunning;
	//Constructor and Destructor.
	Game();
	~Game();

	//Initalize 
	void Initalize();

	//Execute
	void Execute();

	//Shutdown
	void Shutdown();

	//Procedure to run window.
	static LRESULT CALLBACK WindowProcedure(HWND hWnd, UINT uMessage, WPARAM wParam, LPARAM lParam);

	void ChangeState(CBaseGameState* NewState);


private:
	//Private Functions.

	//Create Window Here.
    void InitWindow();

	//Run the Window.
	void GameRun();




	friend CBaseGameState;

};



#endif