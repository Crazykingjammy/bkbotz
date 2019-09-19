#include "game.h"
#include "GameState/GameState.h"


//Constructor
Game::Game()
{

	//Pre Init stuff here.
	isGameRunning = true;

}

Game::~Game()
{
}

//Initalize
void Game::Initalize()
{
	//Get object Manager instance
	pRenderInterface = CRenderInterface::GetInstance();

	//Create a Window to Use.
	InitWindow();

	//Get instance of input
	pInput = InputManager::GetInstance();
	pInput->Initialize(Window.hInstance, Window.hWnd);

	pSound = CSoundSystem::GetInstance();
	pSound->InitSoundSystem(Window.hWnd);
	
	m_GameProfile = CGameProfile::GetInstance();

	//Set State to Intro.
	m_CurrentState = CIntroState::GetInstance();

	//Initalize the Render System.
	pRenderInterface->Initialize(Window.hInstance,Window.WindowWidth,Window.WindowHeight,!true,Window.hWnd);

	//Initalize the current state.
	m_CurrentState->Initalize(this);

}



void Game::Shutdown()
{

	//Shutdown Current State.
	m_CurrentState->Shutdown();

	pInput->Shutdown(); 
		
	//Destroy scene and shutdown manager.
	pRenderInterface->Shutdown();

	pSound->ShutdownSoundSystem();

	// yes PLease do delete me.
	pRenderInterface->DeleteInstance();

	m_GameProfile->DeleteInstance();


}

void Game::GameRun()
{
	m_CurrentState->Update(this->timeDelta);
	
}



void Game::ChangeState(CBaseGameState* NewState)
{
	//Shutdown Current State.
	m_CurrentState->Shutdown();

	//Assign the current state
	m_CurrentState = NewState;

	//Initalize the new state.
	m_CurrentState->Initalize(this);

}
