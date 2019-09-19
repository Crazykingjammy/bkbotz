#include "../GameState.h"

CIntroState * CIntroState::pInstance = 0;

CIntroState * CIntroState::GetInstance()
{
	if (!pInstance)
		pInstance = new CIntroState;
	return pInstance;
}

void CIntroState::Initalize(Game* game)
{
	pGame = game;

	m_Camera	= new CFreeCamera;
	pRenderInterface->SetActiveCamera(m_Camera);
	pRenderInterface->SetWorld("Models/CharacterSelectModels/CharSelGround.x");
	m_TimerIntro.Reset();

	m_SplashLabel	= new CLabel("Textures/IntroStuff/catagory5.bmp");
	m_SplashLabel->m_MyBody.centerPosition.x = pGame->Window.WindowWidth/2.0f;
	m_SplashLabel->m_MyBody.centerPosition.y = pGame->Window.WindowHeight/2.0f;
	
	pRenderInterface->AddLabel(m_SplashLabel);

	m_PressStartLabel = new CLabel("Textures/IntroStuff/pressenter2.bmp");
	m_PressStartLabel->m_MyBody.centerPosition.x = pGame->Window.WindowWidth/2.0f;
	m_PressStartLabel->m_MyBody.centerPosition.y = pGame->Window.WindowHeight/2.0f;
	m_PressStartLabel->m_Visible = false;
	pRenderInterface->AddLabel(m_PressStartLabel);

	m_TLKLabel		  = new CLabel("Textures/IntroStuff/logo.bmp");
	m_TLKLabel->m_Visible = false;
	pRenderInterface->AddLabel(m_TLKLabel);




	pRenderInterface->CreateScene();
	m_TLKLabel->m_MyBody.centerPosition.x += m_TLKLabel->m_Rect.right;
	m_TLKLabel->m_MyBody.centerPosition.y = (float)pGame->Window.WindowHeight;

	m_Cat5Logo = m_SplashLabel->ModelIndex;
	m_TLKLogo = pRenderInterface->LoadTexture("Textures/IntroStuff/TheLockingKeyLogo.bmp");
	m_BKBLogo = pRenderInterface->LoadTexture("Textures/IntroStuff/BKBLogo.tga");

	m_State = SHOWLOGOS;

}

void CIntroState::Update(float timeDelta)
{
	pInput->SetAllStates();
	pSound->ChangeMusicTrack(pSound->AddMusic("Audio/music/GameIntro.mp3"));
	if (pInput->KeyboardKeyPress(DIK_RETURN) ||( pInput->ControllerJoyPress(0, CGameProfile::GetInstance()->ButtonLayout[0]._shoot) || pInput->ControllerJoyPress(1, CGameProfile::GetInstance()->ButtonLayout[1]._shoot) ) )
	{
		m_State = GOTOMAINMENU;
	}

	float elapsed = m_TimerIntro.GetElapsedSeconds();

	switch(m_State)
	{
	case SHOWLOGOS:
		{	
			if (elapsed > 7.0 && elapsed < 12.2f)
			{
				m_SplashLabel->ModelIndex = m_TLKLogo;
			}
			else if (elapsed > 10.0)
			{
				m_PressStartLabel->m_Visible = true;
				m_SplashLabel->ModelIndex = m_BKBLogo;
				m_State = STILLWAITING;
			}
		}
		break;
	case STILLWAITING:
		{
			MoveStartLabel();
		}
		break;
	case GOTOMAINMENU:
		{
			ChangeState(pGame, CCharacterSelectState::GetInstance());
			return;
		}
		break;
	}
}

void CIntroState::MoveStartLabel()
{
	m_NewPosition.MakeRandom((float)pGame->Window.WindowWidth, 0.0f, (float)pGame->Window.WindowHeight, pGame->Window.WindowHeight/2.0f, 0.0f, 0.0f);
	
	CVector3f vel = m_NewPosition - m_PressStartLabel->m_MyBody.centerPosition;
	vel.Normalize();
	m_PressStartLabel->m_MyBody.centerPosition += vel;

}

void CIntroState::Shutdown()
{
	delete m_Camera;
	delete m_SplashLabel;
	delete m_TLKLabel;
	delete m_PressStartLabel;


	pRenderInterface->DestroyScene();

	if (pInstance)
		delete pInstance;
	pInstance = 0;
}