#include "../GameState.h"




CBattleGameState* CBattleGameState::pInstance = 0;

void CBattleGameState::Initalize(Game* game)
{

	pGame = game;

	pSound = CSoundSystem::GetInstance();

	m_SFXBallBounce = pSound->AddSFX("Songs&SFX/bounce.wav"); 
	m_SFXShooting = pSound->AddSFX("Songs&SFX/shoot.wav");
	m_SFXCharging = pSound->AddSFX("Songs&SFX/Charge.wav");
	m_SFXBallHitTarget = pSound->AddSFX("Songs&SFX/TargetHit.wav");
	m_SFXPlayer1Scored = pSound->AddSFX("Songs&SFX/Player1 Win.wav");
	m_SFXPlayer2Scored = pSound->AddSFX("Songs&SFX/Player2 win.wav");
	m_SFXBallHitShield = pSound->AddSFX("Songs&SFX/Deflect.wav");

	//m_MUSICEgypt = pSound->AddMusic("Songs&SFX/sizzla.mp3");
	m_MUSICEgypt = pSound->AddMusic("Songs&SFX/EgyptMusic 1.mp3");
	pSound->ChangeMusicTrack(m_MUSICEgypt);

	//////////////////////////////////////////////////////////////////////////
	// Lights Setup
	//////////////////////////////////////////////////////////////////////////

	//Set up Lights here
	pRenderInterface->Light[0].m_Active = true;
	pRenderInterface->Light[0].m_MyBody.centerPosition.y = 30.0f;
	pRenderInterface->Light[0].m_Specular = 0.7f;
	pRenderInterface->Light[0].m_Diffuse = EXColor(0.5f,0.35f,0.25f,1.0f);
	pRenderInterface->Light[0].m_Ambient = EXColor(0.0f,0.0f,0.0f,1.0f);

//	pRenderInterface->Light[0].m_MyBodyIndex = m_Scene.AddBody(&pRenderInterface->Light[0].m_MyBody);
//	pRenderInterface->AddObject(&pRenderInterface->Light[0]);
	pRenderInterface->Light[0].m_MyBody.radius = 10.0f;


	//Set up Lights here
	pRenderInterface->Light[1].m_Active = true;
	pRenderInterface->Light[1].m_MyBody.centerPosition.y = 20.0f;
	pRenderInterface->Light[1].m_MyBody.centerPosition.x = 3.0f;
	pRenderInterface->Light[1].m_MyBody.centerPosition.z = 20.0f;
	pRenderInterface->Light[1].m_Specular = 0.7f;
	pRenderInterface->Light[1].m_Diffuse = EXColor(3.2f,2.92f,2.85f,1.0f);
	pRenderInterface->Light[1].m_Ambient = EXColor(1.0f,1.0f,1.0f,1.0f);
	pRenderInterface->Light[1].m_Type = 2;


	pRenderInterface->Light[2].m_Active = true;
	pRenderInterface->Light[2].m_MyBody.centerPosition.y = 10.0f;
	pRenderInterface->Light[2].m_Specular = 0.7f;
	pRenderInterface->Light[2].m_Diffuse = EXColor(0.0f,0.0f,0.0f,1.0f);
	pRenderInterface->Light[2].m_Ambient = EXColor(0.0f,0.0f,0.0f,1.0f);


	//////////////////////////////////////////////////////////////////////////
	// Lights Setup
	//////////////////////////////////////////////////////////////////////////
	

	//////////////////////////////////////////////////////////////////////////
	// Camera Setup
	//////////////////////////////////////////////////////////////////////////
	


	Camera0 = new CFreeCamera(0.25f, 4/3, .001f, 50);
	Camera1 = new CFreeCamera(0.25f, 4/3, .001f, 5000);

	Camera1->m_MyBody.centerPosition.y += 5.0f;

	Camera0->m_MyBody.centerPosition.y += 5.0f;
	Camera0->m_MyBody.centerPosition.x -= 15.0f;
	Camera0->m_MyBody.centerPosition.z -= 10.0f;

	Camera0->m_LinearSpeed = 5.0f;

	Camera1->m_LinearSpeed = 5.0f;
	Camera1->m_MyBody.centerPosition.z -= 116.4f;
	Camera1->m_MyBody.centerPosition.y += 29.3f;
	Camera1->m_MyBody.centerPosition.x += 23.86f;
	pRenderInterface->SetActiveCamera(Camera1);

	Camera0->m_MyBody.radius = 4.0f;
	Camera0->m_MyBody.dragCoefficient = 1.0f;
	m_Scene.AddBody(&Camera0->m_MyBody);


	strcpy(Camera0->m_ModelName,"Models/Camera.x");
	pRenderInterface->AddObject(Camera0);
	Camera0->m_Visible = false;



	//////////////////////////////////////////////////////////////////////////
	// Camera Setup
	//////////////////////////////////////////////////////////////////////////




	//////////////////////////////////////////////////////////////////////////
	// Object Setup
	//////////////////////////////////////////////////////////////////////////
	

	
	pRenderInterface->SetWorld("Models/EgyptGround.X");
	//pRenderInterface->SetWorld("Models/Egyptfull.X");
	//pRenderInterface->SetWorld("Models/hitech.X");
	//pRenderInterface->SetWorld("Models/WorldGround.X");

	//Arena = new CArena("Models/hitecharena.X");
	//Arena = new CArena("Models/BattleArena.X");
	Arena = new CArena("Models/Egypt.X");
	pRenderInterface->AddObject(Arena);
	Arena->m_DrawShadow = false;

	InitalizeObjects();

	//////////////////////////////////////////////////////////////////////////
	// Object Setup
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	// 2D Setup
	//////////////////////////////////////////////////////////////////////////

	Initalize2D();
	
	//////////////////////////////////////////////////////////////////////////
	// 2D Setup
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	// Game Number Stuff
	//////////////////////////S///////////////////////////////////////////////


		//Nobody Scores Yet!
		Player1Score = 0;
		Player2Score = 0;
		WinningScore = 20;


		//Controls
		SHOOT = 0;
		BLOCK = 1;
		SPRINT = 2;
		ROTATE_LEFT = 4;
		ROTATE_RIGHT = 5;
		START = 7;

		m_InputFreq = 0.05f;
		m_WinningWait = 3.0f;


		//States
		CurrentState = COUNT_DOWN;

		m_WinningTime.Reset();

		CurrentSelection = RESUME;

	//////////////////////////////////////////////////////////////////////////
	// Game Number Stuff
	//////////////////////////////////////////////////////////////////////////

		Reset();

	//Create the Scene
		pRenderInterface->CreateScene();
		
}

void CBattleGameState::Update(float timeDelta)
{
	char test[128];

	//Might as well get input here.
	pInput->SetAllStates();

	// update the sound well really just tell it to play
	

	switch(CurrentState)
	{

	case COUNT_DOWN:
		{
			Score->m_Visible = false;
			GameText->m_Visible = true;

			if(m_WinningTime.GetElapsedSeconds() > 1.0f)
			{
				sprintf(test, "2");
				strcpy(GameText->m_Text,test);

				if(m_WinningTime.GetElapsedSeconds() > 2.0f)
				{
					sprintf(test, "1");
					strcpy(GameText->m_Text,test);


					if(m_WinningTime.GetElapsedSeconds() > 3.0f)
					{
						sprintf(test, "GO");
						strcpy(GameText->m_Text,test);

						if(m_WinningTime.GetElapsedSeconds() > 4.0f)
						{
							sprintf(test, "3");
							strcpy(GameText->m_Text,test);
							GameText->m_Visible = false;
							Score->m_Visible  = true;
							pSound->PlayMusic();
							CurrentState = PLAY;
						}




					}


				}

			}


			break;
		}
	
	case WIN:
		{
		
				if(Player1Score >= WinningScore)
				{
					sprintf(test, "%Player 1 Wins");
					strcpy(Score->m_Text,test);
					pSound->PlaySFX(m_SFXPlayer1Scored);
				}

				if(Player2Score >= WinningScore)
				{
					sprintf(test, "%Player 2 Wins");
					strcpy(Score->m_Text,test);
					pSound->PlaySFX(m_SFXPlayer2Scored);

				}

				

				if(m_WinningTime.GetElapsedSeconds() > m_WinningWait) 
				{
					Reset();
					GameText->m_Visible = true;
					CurrentState = COUNT_DOWN;
					m_WinningTime.Reset();
				}


			break;
		}

	case PLAY:
		{
			GameRun();
			break;
		}


	case PAUSE:
		{
			Pause();
				break;
		}


	}



}

void CBattleGameState::Shutdown()
{
	pRenderInterface->DestroyScene();

	delete Camera0;
	delete Camera1;
	//delete TestPlayer;


	delete XCannon;
	delete Player1;
	delete ShieldPlayer1;

	delete YCannon;
	delete Player2;
	delete ShieldPlayer2;

	delete [] balls;

	delete [] Targets;

	delete Exit;
	delete Resume;
	delete Restart;
	delete Me;

	delete GameText;

	pSound->StopMusic();

	m_Scene.ClearScene();
	m_2DScene.ClearScene();

	//Clean Me Up.
	if(pInstance)
		delete pInstance;
	pInstance = 0;

}





 CBattleGameState* CBattleGameState::GetInstance()
{
	//If there is no instance
	if(pInstance == 0)
		pInstance = new CBattleGameState;		//We Create One

	return pInstance;							//Return the Instance.
}