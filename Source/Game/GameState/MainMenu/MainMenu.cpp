#include "../GameState.h"

#include "../gamestate.h"

CMenuState * CMenuState::pInstance = 0;

CMenuState::~CMenuState(void)
{
}

void CMenuState::Initalize(Game* game)
{
	this->pSound = CSoundSystem::GetInstance();

	pGame = game;

	pRenderInterface->SetWorld("Models/pool.x");

	pRenderInterface->Light[0].m_Active = true;
	pRenderInterface->Light[0].m_MyBody.centerPosition.y = 10.0f;
	pRenderInterface->Light[0].m_Specular = 0.0f;
	pRenderInterface->Light[0].m_Diffuse = EXColor(0.6f,0.65f,0.65f,1.0f);
	pRenderInterface->Light[0].m_Ambient = EXColor(0.6f,0.65f,0.65f,1.0f);
	pRenderInterface->Light[0].m_Attenuation2 = 0.00000002f;

	// allocate all need objects 
	//////////////////////////////////////////////////////////////////////////
	// allocate 3D stuff that you want
	menuCamera = new CFreeCamera;
	menuPlayer = new CPlayer("Models/bot.x", "Textures/HeatMeter.bmp");

	//////////////////////////////////////////////////////////////////////////
	// lets do some 2D stuff
	pickGameButton = new CLabel("Textures/MenuButtonBackground.bmp");
	menuIterator	= new CLabel("Textures/MenuIterator.bmp");

	pickGameButton->m_MyBody.centerPosition.x = 800/2;
	pickGameButton->m_MyBody.centerPosition.y = 600/2;

	menuIterator->m_MyBody.centerPosition.x = 300;
	menuIterator->m_MyBody.centerPosition.y = 64;


	pRenderInterface->AddLabel(menuIterator);
	pRenderInterface->AddLabel(pickGameButton);




	menuCamera->m_LinearSpeed = 1.0f;

	pRenderInterface->SetActiveCamera(menuCamera);

	pRenderInterface->CreateScene();






}

void CMenuState::Update(float timeDelta)
{
	// typical input reset
	pInput->SetAllStates();


	// apon hitting space bar we will change to a destined state
	if (pInput->KeyboardKeyPress(DIK_SPACE))
	{
		ChangeState(pGame, CIntroGameState::GetInstance());
		return;
	}

	if(GetAsyncKeyState('Q'))
		pRenderInterface->m_ActiveCamera->MoveUp();
	if(GetAsyncKeyState('E'))
		pRenderInterface->m_ActiveCamera->MoveDown();
	if(GetAsyncKeyState('A'))
		pRenderInterface->m_ActiveCamera->MoveLeft();
	if(GetAsyncKeyState('D'))
		pRenderInterface->m_ActiveCamera->MoveRight();
	if(GetAsyncKeyState('W'))
		pRenderInterface->m_ActiveCamera->MoveFoward();
	if(GetAsyncKeyState('S'))
		pRenderInterface->m_ActiveCamera->MoveBackward();

}

void CMenuState::Shutdown()
{
	pRenderInterface->DestroyScene();
	menuPhysicsScene.ClearScene();

	//pSoundInstance->ClearAll();

	delete menuCamera;

	// delete instance
	if (pInstance)
		delete pInstance;
	pInstance = 0;
}

CMenuState *CMenuState::GetInstance()
{
	if (!pInstance)
		return 	pInstance = new CMenuState;

	return pInstance;
}