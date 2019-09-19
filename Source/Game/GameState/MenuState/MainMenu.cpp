#include "../GameState.h"

#include "../gamestate.h"

CMenuState * CMenuState::pInstance = 0;

CMenuState::~CMenuState(void)
{
}

void CMenuState::Initalize(Game* game)
{
	pGame = game;

	this->menus		= CMenuSystem::GetInstance();

	strcpy(m_Logo.m_ModelName,"textures/Introstuff/MenuTitle.tga");
	pRenderInterface->AddLabel(&m_Logo);

	m_Logo.m_MyBody.centerPosition.x = pGame->Window.WindowWidth/2.0f;

	m_Logo.m_MyBody.centerPosition.y = pGame->Window.WindowHeight/2.0f;
	m_Logo.m_MyBody.centerPosition.y += 50.0f;

	menus->Initialize();
	

	//strcpy(m_Image.m_ModelName,"textures/hud.bmp");
	//pRenderInterface->AddLabel(&m_Image);
}

void CMenuState::Update(float timeDelta)
{
	pRenderInterface->m_ActiveCamera->LookAt(pRenderInterface->m_World);
	
	pRenderInterface->m_ActiveCamera->m_LinearSpeed = 0.01f;
	pRenderInterface->m_ActiveCamera->MoveLeft();

	static int menu = 0;
	int command = menus->Update(menu);
	bool blah;
	switch(command)
	{
	case 0:
		break;
	case StartBattle:
		{
			// change to battle state..if you didnt know that, go play in traffic blindfolded
			pSound->ChangingState();
			ChangeState(pGame, CCharacterSelectState::GetInstance());
			return;
			blah = true;
			break;
		}
		break;
	case StartSoccer:
		// change to soccer state...seriously, if you couldnt figure that out, run your face over with a steam roller
		blah = true;
		break;
	case Exit_Game:
		// I think you know what to do here, if not, have a nice tall glass of antifreeze
		blah = true;
		pGame->isGameRunning = false;
		PostQuitMessage(0);
		break;
	case ChangeToAudioMenu:
		menu = 1;
		break;
	case ChangeToControlsMenu:
		menu = 2;
		break;
	case ChangeToMainMenu:
		menu = 0;
		if (menus->SoundMenu.SFXVolumeControl.CurrentOption->Data == 100)
			pSound->SetVolumeSFX(MAX_VOLUME);
		else if (menus->SoundMenu.SFXVolumeControl.CurrentOption->Data < 99)
			pSound->SetVolumeSFX(MAX_VOLUME/4);
		if (!menus->SoundMenu.SFXToggle.CurrentOption->Data)
		{
			pSound->SetVolumeSFX(MIN_VOLUME);
		}

		if (!menus->SoundMenu.BGMToggle.CurrentOption->Data)
		{
			pSound->WantToPlayMusic(false);
		}
		else
		{
			pSound->WantToPlayMusic(true);
		}
		
		CGameProfile::GetInstance()->Points4Win = menus->SoundMenu.BGMVolumeControl.CurrentOption->Data;
		break;
	default:
		break;
	}
	pInput->SetAllStates();
	// apon hitting space bar we will change to a destined state
	if (pInput->KeyboardKeyPress(DIK_SPACE))
	{
//		ChangeState(pGame, CJukeBoxState::GetInstance());
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
//
	//menuCamera->LookAt(menuPlayer);

	//menuIterator->m_MyBody.centerPosition.x += 0.01f; //= pickGameButton->m_MyBody.centerPosition.x;
	//menuIterator->m_MyBody.centerPosition.y += 0.01f; //= pickGameButton->m_MyBody.centerPosition.y;

//	if (pInput->KeyboardKeyPress(DIK_LEFT))
//	{
//		menuIterator->m_MyBody.centerPosition.x = pickGameButton->m_MyBody.centerPosition.x;
//		menuIterator->m_MyBody.centerPosition.y = pickGameButton->m_MyBody.centerPosition.y;
//	}
//	if (pInput->KeyboardKeyPress(DIK_RIGHT))
//	{
//		menuIterator->m_MyBody.centerPosition.x = otherButton->m_MyBody.centerPosition.x;
//		menuIterator->m_MyBody.centerPosition.y = otherButton->m_MyBody.centerPosition.y;
//	}
}

void CMenuState::Shutdown()
{
	pRenderInterface->DestroyScene();
	menuPhysicsScene.ClearScene();

	//pSoundInstance->ClearAll();

//	delete menuCamera;

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