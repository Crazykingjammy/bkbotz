#include "../Gamestate.h"

CBattleGameState* CBattleGameState::pInstance = 0;

CBattleGameState* CBattleGameState::GetInstance()
{
	//If there is no instance
	if(pInstance == 0)
		pInstance = new CBattleGameState;		//We Create One

	return pInstance;							//Return the Instance.
}

CLabel tempLabel;

void CBattleGameState::Initalize(Game* game)
{

	//Make sure to assign the game.
	pGame = game;

	//Set the number of players
	m_NumberPlayers = 2;

	// default the 
	m_PhysicsSimSpeed = 1.0f;
	//////////////////////////////////////////////////////////////////////////
	// Camera Creation
	//////////////////////////////////////////////////////////////////////////

	m_ArenaCamera = new CFreeCamera();
	m_ArenaCamera2 = new CFreeCamera();
	m_CustomCamera = new CFreeCamera();
	m_WinCamera	= new CFreeCamera();
	m_LookArroundCamera = new CFreeCamera();

	pRenderInterface->SetActiveCamera(m_CustomCamera);

	//Set position.
	m_ArenaCamera->m_MyBody.centerPosition = CVector3f(-1.159f,75.91f,-100.924f);
	m_ArenaCamera->m_DrawShadow = false;


	//For good play.
	m_ArenaCamera2->m_MyBody.centerPosition = CVector3f(-1.159f,35.91f,-125.924f);


	//Eh for intro.
	m_CustomCamera->m_MyBody.centerPosition = CVector3f(100.0f,10.0f,0.0f);

	//Yeah now we can slow it down
	m_WinCamera->m_LinearSpeed = 0.035f;

	//Yeah now we can slow it down
	m_CustomCamera->m_LinearSpeed = 0.894205f;


	/************************************************************************/
	/* Camera Creation                                                      */
	/************************************************************************/


	//////////////////////////////////////////////////////////////////////////
	// Lights
	//////////////////////////////////////////////////////////////////////////
	
	//Set up Lights here
	pRenderInterface->Light[0].m_Active = true;
	pRenderInterface->Light[0].m_MyBody.centerPosition.y = 45.0f;
	pRenderInterface->Light[0].m_Specular = 0.3f;
	pRenderInterface->Light[0].m_Diffuse = EXColor(0.9f,0.75f,0.85f,1.0f);
	pRenderInterface->Light[0].m_Ambient = EXColor(0.0f,0.0f,0.0f,1.0f);
	pRenderInterface->Light[0].m_Attenuation1 = 0.00435f;
	pRenderInterface->Light[0].m_Attenuation2 = 0.0f;

	//pRenderInterface->Light[0].m_MyBodyIndex = m_Scene.AddBody(&pRenderInterface->Light[0].m_MyBody);
	//pRenderInterface->AddObject(&pRenderInterface->Light[0]);
	pRenderInterface->Light[0].m_MyBody.radius = 5.0f;


	/************************************************************************/
	/* Lights                                                               */
	/************************************************************************/


	//////////////////////////////////////////////////////////////////////////
	// World
	//////////////////////////////////////////////////////////////////////////
	
	//Create the arena.
	m_ArenaBuilding = new CArena;


	switch (pGameInfo->IndexOfLevel)
	{

	case pGameInfo->EGYPT:
		{		
			strcpy(m_ArenaBuilding->m_ModelName,"Models/EgyptBigArena.X");
			pRenderInterface->SetWorld("Models/EgyptBigGround.X");
			m_MUSICBackGround = pSound->AddMusic("Audio/Music/EgyptLevelBMG.mp3");

			break;
		}


	case pGameInfo->HITECH:
		{		
			strcpy(m_ArenaBuilding->m_ModelName,"Models/World/hightechArena.x");
			pRenderInterface->SetWorld("Models/World/hightechGround.x");

			if(  (!strcmp(pGameInfo->Players[0]._characterName,"Tbot")) || (!strcmp(pGameInfo->Players[1]._characterName,"Tbot")) )
			{
				m_MUSICBackGround = pSound->AddMusic("Audio/Music/Envious.mp3");
			}
			else
			{
				m_MUSICBackGround = pSound->AddMusic("Audio/Music/Eighty Five Riddim.mp3");
			}


			break;
		}

	case pGameInfo->HELL:
		{		
			strcpy(m_ArenaBuilding->m_ModelName,"Models/World/hellArena.x");
			pRenderInterface->SetWorld("Models/World/hellGround.x");
			m_MUSICBackGround = pSound->AddMusic("Audio/Music/hell.mp3");
			
			break;
		}

	case pGameInfo->BEACH:
		{		
			strcpy(m_ArenaBuilding->m_ModelName,"Models/World/BeachArena.x");
			pRenderInterface->SetWorld("Models/World/BeachGround.x");
			m_MUSICBackGround = pSound->AddMusic("Audio/Music/Follow The Leader.mp3");

			//Set up Lights here
			pRenderInterface->Light[0].m_Active = true;
			pRenderInterface->Light[0].m_MyBody.centerPosition.y = 35.0f;
			pRenderInterface->Light[0].m_MyBody.centerPosition.z = 45.0f;
			pRenderInterface->Light[0].m_Specular = 0.1f;
			pRenderInterface->Light[0].m_Diffuse = EXColor(0.6f,0.65f,0.55f,1.0f);
			pRenderInterface->Light[0].m_Ambient = EXColor(0.7f,0.7f,0.6f,1.0f);
			pRenderInterface->Light[0].m_Attenuation1 = 0.00335f;
			pRenderInterface->Light[0].m_Attenuation2 = 0.0f;

			//pRenderInterface->Light[0].m_MyBodyIndex = m_Scene.AddBody(&pRenderInterface->Light[0].m_MyBody);
			//pRenderInterface->AddObject(&pRenderInterface->Light[0]);
			//pRenderInterface->Light[0].m_MyBody.radius = 5.;

			break;
		}


	default:
		{
			strcpy(m_ArenaBuilding->m_ModelName,"Models/World/hellArena.x");
			pRenderInterface->SetWorld("Models/World/hellGround.x");
			m_MUSICBackGround = pSound->AddMusic("Audio/Music/hell.mp3");

		}
		break;
	}

	


	pRenderInterface->AddObject(m_ArenaBuilding);
	m_ArenaBuilding->m_DrawShadow = false;

	/************************************************************************/
	/* World                                                                */
	/************************************************************************/



	//////////////////////////////////////////////////////////////////////////
	// Objects
	//////////////////////////////////////////////////////////////////////////

	//Create our shit here.
	InitalizeObjects();

	//Create 2d stuff
	InitalizeMenu();
	InitalizeHud();



	/************************************************************************/
	/* Objects                                                              */
	/************************************************************************/


	//////////////////////////////////////////////////////////////////////////
	// Particles
	//////////////////////////////////////////////////////////////////////////

	//Gimmie a texture.
	strcpy(m_Explosion.m_ModelName,"Textures/flare.bmp");
	m_Explosion.m_ParticleSize = 0.8f;
	m_Explosion.SetNumParticles(500);
	pRenderInterface->AddParticle(&m_Explosion);

	//Gimmie a texture.
	strcpy(m_Explosion2.m_ModelName,"Textures/oop_cloud.bmp");
	m_Explosion2.m_ParticleSize = 0.8f;
	m_Explosion2.SetNumParticles(500);
	pRenderInterface->AddParticle(&m_Explosion2);
	m_Explosion2.RotateY(DegreesToRadians(180.0f));






	/************************************************************************/
	/* Particles                                                            */
	/************************************************************************/

	//////////////////////////////////////////////////////////////////////////
	// Numbers
	//////////////////////////////////////////////////////////////////////////
	
	m_CurrentBattleState = COUNTDOWN;
	m_InputFreq = 0.05f;
	m_WinningPlayerIndex = 0;

	//Jus default the fucking thing
	m_WhoPushedStart = 0;

	//Set the rounds with teh book keeper.
	m_RoundCounter.SetMaxPoints(pGameInfo->Points4Win);

	/************************************************************************/
	/* Numbers                                                              */
	/************************************************************************/


	pRenderInterface->CreateScene();


	//////////////////////////////////////////////////////////////////////////
	// Load Models and Textures
	//////////////////////////////////////////////////////////////////////////
	


	int ready = pRenderInterface->LoadModel("models/ready.x");
	int notready = pRenderInterface->LoadModel("models/shield.x");

	for(int i = 0; i < m_NumberPlayers; i++)
	{
		m_Cannons[i].m_BallModelIndex = pRenderInterface->LoadModel(pGameInfo->Players[i]._cannon._ballModelName);

		//Assign the index dem
		m_Players[i].m_NotReadyIndex = notready;
		m_Players[i].m_ReadyIndex = ready;

	}

	//Load it here cos were not adding it to the scene ;-)
	m_SafeBall.ModelIndex = pRenderInterface->LoadModel("models/ball.x");

	m_CountDownIndices[5] = m_CountDownNumber->ModelIndex;
	
	m_CountDownIndices[0] = pRenderInterface->LoadTexture("textures/0.bmp");
	m_CountDownIndices[1] = pRenderInterface->LoadTexture("textures/1.bmp");
	m_CountDownIndices[2] = pRenderInterface->LoadTexture("textures/2.bmp");
	m_CountDownIndices[3] = pRenderInterface->LoadTexture("textures/GreenWins.bmp");
	m_CountDownIndices[4] = pRenderInterface->LoadTexture("textures/SphinxWin.bmp");
	
	/************************************************************************/
	/* Load Models and Textures                                             */
	/************************************************************************/

	/************************************************************************/
	/* Lets load some SFX                                                   */
	/************************************************************************/
	//m_Player1Shoot = pSound->AddSFX("Songs&SFX/Sprint.wav");

	m_SFXShoot = pSound->AddSFX("Audio/SFX/shot_regular.wav");
	m_SFXFullChargeShoot = pSound->AddSFX("Audio/SFX/shot_fully_charged.wav");
	m_SFXCharging		 = pSound->AddSFX("Audio/SFX/charging.wav");
	m_SFXShieldOn = pSound->AddSFX("Audio/SFX/shield_on.wav");
	m_SFXShieldOff = pSound->AddSFX("Audio/SFX/shield_on.wav");

	m_SFXBlock = pSound->AddSFX("Audio/SFX/shield_block.wav");
	m_SFXBlockNotDangerous = pSound->AddSFX("Audio/SFX/shield_block1.wav");

	m_SFXBlockCounter = pSound->AddSFX("Audio/SFX/Counter_Attack.wav");
	m_SFXSprint = pSound->AddSFX("Audio/SFX/Sprint.wav");

	m_SFXOverHeat = pSound->AddSFX("Audio/SFX/heat_meter.wav");
	m_SFXPickUpBall = pSound->AddSFX("Audio/SFX/pickup_ball1.wav");
	m_SFXPickUpCannon = pSound->AddSFX("Audio/SFX/pickup_barrell.wav");
	m_SFXCountDown = pSound->AddSFX("Audio/SFX/countdown_woman_vox.wav");
	m_SFXWinner = pSound->AddSFX("Audio/SFX/Sprint.wav");
	m_SFXBallHitPlayer = pSound->AddSFX("Audio/SFX/ball_hit_player.wav");
	m_SFXPlayerHitTarget = pSound->AddSFX("Audio/SFX/Sprint.wav");
	m_SFXTargetHit = pSound->AddSFX("Audio/SFX/TargetHit.wav");
	m_SFXBallHitHead = pSound->AddSFX("Audio/SFX/BallHitHead.wav");
	m_SFXOutOfAmmo   = pSound->AddSFX("Audio/SFX/fart.wav");
	m_SFXPauseGame   = pSound->AddSFX("Audio/SFX/exported/pause1.wav");
	m_SFXPauseSelect  = pSound->AddSFX("Audio/SFX/menu_scrollover.wav");

	char buf[128];

	m_CurrInactiveBallSFX = 0;
	for (int i = 0; i < NUM_OF_INACTIVE_BALL_SFX; i++)
	{
		sprintf(buf, "Audio/SFX/InactivateBall%d.wav", i);
		m_SFXBallBackToSafeMode[i] = pSound->AddSFX(buf);		
	}
	m_TimerRestoreDefaultInactiveBallSFX.Reset();


	//////////////////////////////////////////////////////////////////////////
	// create special wall
	m_BackWall  = m_Scene.AddWallToWorld(CVector3f(0.0f, 0.0f, -49.0f),CVector3f(0.0f, 0.0f, 1.0f), false, "BackWallKiller");
	m_FrontWall = m_Scene.AddWallToWorld(CVector3f(0.0f, 0.0f, 49.0f),CVector3f(0.0f, 0.0f, -1.0f), false, "FrontWallKiller");
	m_MidLinePlayer[0] = m_Scene.AddWallToWorld(CVector3f(0.0f, 0.0f, 0.0f), CVector3f(0.0f, 0.0f, -1.0f), false, "Player 1 MidLine");
	m_MidLinePlayer[1] = m_Scene.AddWallToWorld(CVector3f(0.0f, 0.0f, 0.0f), CVector3f(0.0f, 0.0f, 1.0f), false, "Player 2 MidLine");
	m_Scene.CreateWorld(75.0f, 30.0f, 100.0f);

	m_IntroTimer.Reset();
	Reset();
}

void CBattleGameState::Update(float timeDelta)
{
	m_fTimeDelta = timeDelta;

	pInput->SetAllStates();

	//Update the physics scene.
	m_Scene.Simulate(m_PhysicsSimSpeed);


	switch (m_CurrentBattleState)
	{

	case PLAY:
		{
			//Run the game
			GameRun();
			break;
		}

	case PAUSE:
		{
			m_PhysicsSimSpeed = 0.001f;
			Pause();
			break;
		}

	case COUNTDOWN:
		{
			m_PhysicsSimSpeed = 0.1f;
			CountDown();
			break;
		}

	case WIN:
		{
			GameWin();
			break;
		}

	case INTRO:
		{
			break;
		}

	case LOOKARROUND:
		{
			FreeLookArround();
			break;
		}

	}
}

void CBattleGameState::GameRun()
{

	Controls();

	// update the camera
	//m_ArenaCamera->LookAt(pRenderInterface->m_World);		// lets save this for the bullet time cam
	m_ArenaCamera2->LookAt(pRenderInterface->m_World);

	//Check for collision.
	CollisionDetection();

	//Update the damn player.
	//Update the players.

	for(int i = 0; i < m_NumberPlayers; i++)
	{
		//Update the player.
		m_Players[i].Update(m_fTimeDelta);
		
		if (m_Players[i].m_bOverHeated)
		{
			pSound->PlayQSFX(m_SFXOverHeat);
		}
		//Update the num balls for the huds.
		strcpy(m_PlayerHuds[i].m_BallCount.m_Text,m_Players[i].m_BallCountString);
		
		//Middle arena collision checking!

		if((i%2) == 0 )
		{
			//if odd player
			// constrain the player and its barell
			m_Scene.HandleSpecialPlaneCollision(m_Players[i].m_MyBodyIndex, m_MidLinePlayer[0]);
			m_Scene.HandleSpecialPlaneCollision(m_Cannons[i].m_MyBodyIndex, m_MidLinePlayer[0]);

			//Check if the target has been killed.
			if(m_Targets[i].m_Life <= 0.0f)
			{
				//If odd player.. player 1
				m_CurrentBattleState = WIN;
				m_CountDownNumber->ModelIndex = m_CountDownIndices[3];
				m_CountDownNumber->ModelIndex = m_PlayerHuds[1].m_PlayerIcon.ModelIndex;
				//m_CountDownNumber->m_Visible = true;
				m_IntroTimer.Reset();

				//Player 2 wins.
				m_RoundCounter.AddPoint(1);
				m_PlayerHuds[1].AddPoint();

				//Look at player 2.
				m_LookPosition = m_Players[1].m_MyBody.centerPosition;

				//Why not
				pRenderInterface->SetActiveCamera(m_WinCamera);
				m_WinCamera->m_MyBody.centerPosition = m_LookPosition;
				m_WinCamera->m_MyBody.centerPosition.z -= ( m_Players[1].m_MyBody.radius * 3.5f);
				m_WinCamera->m_MyBody.centerPosition.y += 10.0f;
				m_Players[i].m_MyBody.ZeroLinearForces();
				m_WinningPlayerIndex  = 1;

				//Hide the huds;
				m_PlayerHuds[0].Hide(false);
				m_PlayerHuds[1].Hide(false);

				//DIsplay winning name;
				char buffer[256];
				sprintf(buffer,"%Player %d Wins",2);
				strcpy(m_TimeDisplay.m_Text,buffer);
				m_TimeDisplay.m_MyBody.centerPosition.x = (float)(pGame->Window.WindowWidth/2);
				m_TimeDisplay.m_MyBody.centerPosition.x -= 220.0f;
				m_TimeDisplay.m_MyBody.centerPosition.y = (float)(pGame->Window.WindowHeight);
				m_TimeDisplay.m_MyBody.centerPosition.y -= 50.0f;

				pSound->ChangingState();
				pSound->PlayQSFX(m_SFXPlayerWinningChants[1]);

			}

		}
		else
		{
			//if even.

			m_Scene.HandleSpecialPlaneCollision(m_Players[i].m_MyBodyIndex, m_MidLinePlayer[1]);
			m_Scene.HandleSpecialPlaneCollision(m_Cannons[i].m_MyBodyIndex, m_MidLinePlayer[1]);

			//Check if the target is dead.
			if(m_Targets[i].m_Life <= 0.0f)
			{
				m_CurrentBattleState = WIN;
				m_CountDownNumber->ModelIndex = m_PlayerHuds[0].m_PlayerIcon.ModelIndex;
				//m_CountDownNumber->m_Visible = true;
				m_IntroTimer.Reset();

				//Player 1 wins.h
				m_RoundCounter.AddPoint(0);
				m_PlayerHuds[0].AddPoint();

				//Look at player 1.
				m_LookPosition = m_Players[0].m_MyBody.centerPosition;

				//Why not
				pRenderInterface->SetActiveCamera(m_WinCamera);
				m_WinCamera->m_MyBody.centerPosition = m_LookPosition;
				m_WinCamera->m_MyBody.centerPosition.z += ( m_Players[0].m_MyBody.radius * 3.50f);
				m_WinCamera->m_MyBody.centerPosition.y += 10.0f; 
				m_Players[i].m_MyBody.ZeroLinearForces();
				m_WinningPlayerIndex  = 0;

				//Hide the huds.
				m_PlayerHuds[0].Hide(false);
				m_PlayerHuds[1].Hide(false);

				//DIsplay winning name;
				char buffer[256];
				sprintf(buffer,"%Player %d Wins",1);
				strcpy(m_TimeDisplay.m_Text,buffer);
				m_TimeDisplay.m_MyBody.centerPosition.x = (float)(pGame->Window.WindowWidth/2);
				m_TimeDisplay.m_MyBody.centerPosition.x -= 220.0f;
				m_TimeDisplay.m_MyBody.centerPosition.y = (float)(pGame->Window.WindowHeight);
				m_TimeDisplay.m_MyBody.centerPosition.y -= 50.0f;
				pSound->ChangingState();
				pSound->PlayQSFX(m_SFXPlayerWinningChants[0]);

			}

		}


		//Push back the targets.
		m_Targets[i].PushBack();

	}

}

void CBattleGameState::Shutdown()
{
	
	pSound->StopMusic();
	delete m_ArenaCamera;
	delete m_ArenaCamera2;
	delete m_CustomCamera;

	delete [] m_Balls;

	delete [] m_Players;
	delete [] m_Cannons;
	delete [] m_Shields;
	delete [] m_PlayerHuds;
	delete [] m_Targets;

	m_Scene.ClearScene();

	pRenderInterface->DestroyScene();


	//Clean Me Up.
	if(pInstance)
		delete pInstance;
	pInstance = 0;

}