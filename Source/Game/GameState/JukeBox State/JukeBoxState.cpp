#include "../gamestate.h"

CJukeBoxState * CJukeBoxState::pInstance = 0;

CJukeBoxState::CJukeBoxState(void)
{

}
CJukeBoxState::~CJukeBoxState(void)
{
}

void CJukeBoxState::Initalize(Game* game)
{
	pGame = game;

	virtuaKeyboard.Initalize();

	pRenderInterface->SetWorld("Models/SoccerField.x");

	pRenderInterface->Light[0].m_Active = true;
	pRenderInterface->Light[0].m_MyBody.centerPosition.y = 10.0f;
	pRenderInterface->Light[0].m_Specular = 0.0f;
	pRenderInterface->Light[0].m_Diffuse = EXColor(0.6f,0.65f,0.65f,1.0f);
	pRenderInterface->Light[0].m_Ambient = EXColor(0.6f,0.65f,0.65f,1.0f);
	pRenderInterface->Light[0].m_Attenuation2 = 0.00000002f;

	jCamera0 = new CFreeCamera;

	this->pSound = CSoundSystem::GetInstance();

	sfx = pSound->AddSFX("Songs&SFX/bounce.wav");
	targetHit = pSound->AddSFX("Songs&SFX/TargetHit.wav");

	song = pSound->AddMusic("Songs&SFX/EgyptMusic.mp3");
	devilSong = pSound->AddMusic("Songs&SFX/Devil.mp3");
	sizzla	  = pSound->AddMusic("Songs&SFX/sizzla.mp3");
	pSound->ChangeMusicTrack(sizzla);
	pRenderInterface->CreateScene();
}

void CJukeBoxState::Update(float timeDelta)
{
	pInput->SetAllStates();


	virtuaKeyboard.Update();

	if (pInput->Controller1JoyPress(2))
	{
		ChangeState(pGame, CIntroGameState::GetInstance());
		return;
	}

	if (pInput->Controller1JoyPress(0))
		pSound->PlaySFX(sfx);

	if (pInput->Controller1JoyPress(1))
	{
		pSound->PlaySFX(targetHit);
	}
 	pSound->PlayMusic();

	static CPhysicsTimer time; 
	//if (time.GetElapsedSeconds() > 60.0f)
	//{
	//	pSoundInstance->NextMusicTrack();
	//	time.Reset();
	//}
	if (pInput->KeyboardKeyPress(DIK_RETURN))
	{
		pSound->ChangeMusicTrack(sizzla);
	}
}

void CJukeBoxState::Shutdown()
{
	pRenderInterface->DestroyScene();
	jPhysicsScene.ClearScene();

	//pSoundInstance->ClearAll();

	if (pInstance)
		delete pInstance;
	pInstance = 0;
}

CJukeBoxState *CJukeBoxState::GetInstance()
{
	if (!pInstance)
		return 	pInstance = new CJukeBoxState;

	return pInstance;
}