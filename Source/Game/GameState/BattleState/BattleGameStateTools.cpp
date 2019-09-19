#include "../Gamestate.h"


void CBattleGameState::InitalizeObjects()
{
	char buffer[128];

	//////////////////////////////////////////////////////////////////////////
	// Setup Balls\
	//////////////////////////////////////////////////////////////////////////
	

	m_NumberBalls = 30;
	m_Balls = new CBall[m_NumberBalls];
	
	for(int i = 0; i < m_NumberBalls; i++)
	{
		strcpy(m_Balls[i].m_ModelName,"Models/ball.x");
		sprintf(buffer,"Ball %d",i);
		strcpy(m_Balls[i].m_MyBody.myName,buffer);
		pRenderInterface->AddObject(&m_Balls[i]);
		m_Balls[i].m_MyBodyIndex = m_Scene.AddBody(&m_Balls[i].m_MyBody);

	}

	
	SetBalls();


	/************************************************************************/
	/* Setup Balls                                                          */
	/************************************************************************/


	//////////////////////////////////////////////////////////////////////////
	// Targets
	//////////////////////////////////////////////////////////////////////////
	

	/************************************************************************/
	/* Targets                                                              */
	/************************************************************************/




	//////////////////////////////////////////////////////////////////////////
	// Player Setup
	//////////////////////////////////////////////////////////////////////////

	SetAllPlayers();

	/************************************************************************/
	/* Player Setup                                                         */
	/************************************************************************/


}

void CBattleGameState::InitalizeMenu()
{

	strcpy(m_MenuIcons[RESUME].m_ModelName,"Textures/Resume.bmp");
	strcpy(m_MenuIcons[RESTART].m_ModelName,"Textures/Restart.bmp");
	strcpy(m_MenuIcons[QUIT].m_ModelName,"Textures/Quit.bmp");
	strcpy(m_MenuIcons[CAMERASELECT].m_ModelName,"Textures/CameraSelect.bmp");
	strcpy(m_MenuIcons[CAMERA1].m_ModelName,"Textures/Resume.bmp");
	strcpy(m_MenuIcons[CAMERA2].m_ModelName,"Textures/Resume.bmp");
	strcpy(m_MenuIcons[CAMERA3].m_ModelName,"Textures/Resume.bmp");

	//Set the background images.
	strcpy(m_BackgroundIcon.m_ModelName,"Textures/ButtonBackground.tga");
	pRenderInterface->AddLabel(&m_BackgroundIcon);

	for(int i = 0; i < NUMOPTIONS; i++)
	{

		//Add the shit to the fucking scene.
		pRenderInterface->AddLabel(&m_MenuIcons[i]);
		m_MenuIcons[i].m_MyBody.centerPosition.x  = -MENUICON_WIDTH_F / 2.0f;
		m_MenuIcons[i].m_MyBody.centerPosition.y  += (float) ( (i * MENUICON_HEIGHT_F/ 2.0f) + (MENUICON_HEIGHT_F / 2.0f) );

		//Hide the sub stuff.
		if(i > CAMERA1)
		{
			//Hide them for now.
			//m_MenuIcons[i].m_Visible			= false;

		}



	}

	m_BackgroundIcon.m_MyBody.centerPosition = m_MenuIcons[0].m_MyBody.centerPosition;
}


void CBattleGameState::InitalizeHud()
{
	//Add the count down.
	m_CountDownNumber = new CLabel;
	strcpy( m_CountDownNumber->m_ModelName,"textures/3.bmp");
	pRenderInterface->AddLabel(m_CountDownNumber);

	m_CountDownNumber->m_MyBody.centerPosition = CVector3f( (float)(pGame->Window.WindowWidth/2),(float)(pGame->Window.WindowHeight/2),0.0f );
	m_CountDownNumber->m_Visible = false;

	
	//yeah yeah.
	m_TimeDisplay.m_MyBody.centerPosition.x = (float)(pGame->Window.WindowWidth/2);
	m_TimeDisplay.m_MyBody.centerPosition.x -= 80.0f;
	m_TimeDisplay.m_MyBody.centerPosition.y = 20.0f;
	strcpy(m_TimeDisplay.m_ModelName,"textures/font.tga");
	strcpy(m_TimeDisplay.m_Text,"Hello Bitches");
	m_TimeDisplay.m_FontWidth = 32;
	pRenderInterface->AddLabel(&m_TimeDisplay);
}


void CBattleGameState::SetAllPlayers()
{
	m_Players				 = new CPlayer[m_NumberPlayers];
	m_Cannons				 = new CBarrell[m_NumberPlayers];
	m_Shields				 = new CShield[m_NumberPlayers];
	m_PlayerHuds			 = new CPlayerHud[m_NumberPlayers];
	m_Targets				 = new CTarget[m_NumberPlayers];
	m_SFXPlayerWinningChants = new int[m_NumberPlayers];

	//Maybe...
	m_NumberTargets = m_NumberPlayers;
	

	for(int i = 0; i < m_NumberPlayers; i++)
	{
		//Set the player
		m_Players[i].m_fRegular			= pGameInfo->Players[i]._SpeedForce;
		m_Players[i].m_fSprint			= pGameInfo->Players[i]._SprintSpeedForce;
		m_Players[i].m_Damage			= pGameInfo->Players[i]._HeatResistance;
		m_Players[i].m_MaxKE			= pGameInfo->Players[i]._maxKE;
		m_Players[i].m_PickupKE			= pGameInfo->Players[i]._pickupKE;
		m_Players[i].m_MyBody.radius	= pGameInfo->Players[i]._radius;
		m_Players[i].m_HeatFromBlocking = pGameInfo->Players[i]._blockHeat;
		m_Players[i].m_HeatFromSprint	= pGameInfo->Players[i]._sprintHeat;
		m_Players[i].m_CoolDownRate		= pGameInfo->Players[i]._cooldownHeat;

		m_Players[i].m_MyBody.SetMass(pGameInfo->Players[i]._mass);
		m_Players[i].m_MyBody.dragCoefficient = pGameInfo->Players[i]._drag;
		strcpy(m_Players[i].m_ModelName, pGameInfo->Players[i]._playerModelName);
		strcpy(m_Players[i].m_HeatMeter->m_ModelName, pGameInfo->Players[i]._heatMeterTexture);
		m_SFXPlayerWinningChants[i]        = pSound->AddSFX(pGameInfo->Players[i]._playerChant);

		//Set the cannon
		m_Cannons[i].m_MaxBalls			= pGameInfo->Players[i]._cannon._ballCount;
		m_Cannons[i].m_ChargeRate		= pGameInfo->Players[i]._cannon._chargeRate;
		m_Cannons[i].m_MaxChargeheat	= pGameInfo->Players[i]._cannon._chargeHeat;
		m_Cannons[i].m_ShootingHeat		= pGameInfo->Players[i]._cannon._shootHeat;
		m_Cannons[i].m_RegPower			= pGameInfo->Players[i]._cannon._shootForce;
		// cannons current power is always minimum the reg power of the cannon
		m_Cannons[i].m_CurrentPower	    = m_Cannons[i].m_RegPower;
		m_Cannons[i].m_MaxPower			= pGameInfo->Players[i]._cannon._maxForce;
		m_Cannons[i].m_RegPower			= pGameInfo->Players[i]._cannon._shootForce;
		m_Cannons[i].m_DamageAmount		= pGameInfo->Players[i]._cannon._shootDamage;
		m_Cannons[i].m_BallMass			= pGameInfo->Players[i]._cannon._ballMass;
		m_Cannons[i].m_MyBody.radius	= m_Players[i].m_MyBody.radius;
		m_Cannons[i].m_BallRadius		= pGameInfo->Players[i]._cannon._ballRadius;
		m_Cannons[i].m_FireRate			= pGameInfo->Players[i]._cannon._shootRate;

		strcpy(m_Cannons[i].m_ModelName, pGameInfo->Players[i]._cannon._cannonModelName);
		m_Cannons[i].m_MyBody.dragCoefficient = pGameInfo->Players[i]._cannon._mass;
		m_Cannons[i].m_MyBody.SetMass(pGameInfo->Players[i]._cannon._mass);

		//Set the shield
		m_Shields[i].m_ReflectingDamage	= pGameInfo->Players[i]._shieldReflectDamage;
		m_Shields[i].m_ReflectingForce	= pGameInfo->Players[i]._shieldReflectForce;
		m_Shields[i].m_MyBody.radius	= pGameInfo->Players[i]._blockingRadius;
		m_Shields[i].m_MaxRadius		= pGameInfo->Players[i]._blockingRadius;
		m_Shields[i].m_ShrinkRate		= pGameInfo->Players[i]._shieldShrinkRate;
		m_Shields[i].m_ShrinkHit		= pGameInfo->Players[i]._shieldShrinkHit;

		m_Shields[i].m_MyBody.SetMass(pGameInfo->Players[i]._shieldMass);
		strcpy(m_Shields[i].m_ModelName, pGameInfo->Players[i]._shieldModelName);


		//if even...
		if((i % 2) == 0)
		{
			//Odd players
			m_Players[i].m_MyBody.centerPosition.z -= 45.0f;

			//Position Hud.
			m_PlayerHuds[i].m_Frame.m_MyBody.centerPosition.x += 128.0f;
			m_PlayerHuds[i].m_Frame.m_MyBody.centerPosition.y += 53.0f;

		}
		else
		{
			//Even Players
			m_Players[i].m_MyBody.centerPosition.z += 45.0f;
			m_Players[i].m_DefaultFoward.z = -1.0f;
			m_Players[i].RotateY(DegreesToRadians(180.0f));	


			//Position.
			m_PlayerHuds[i].m_Frame.m_MyBody.centerPosition.x += (float)(pGame->Window.WindowWidth - 128);
			m_PlayerHuds[i].m_Frame.m_MyBody.centerPosition.y += 53.0f;

		}

		//Setup the player
		m_Players[i].AttachCannon(&m_Cannons[i]);
		m_Players[i].AttachShield(&m_Shields[i]);



		//Targets
		strcpy(m_Targets[i].m_ModelName, pGameInfo->Players[i]._targetModelName);
		pRenderInterface->AddObject(&m_Targets[i]);
		m_Targets[i].m_MyBodyIndex = m_Scene.AddBody(&m_Targets[i].m_MyBody);


		//Properties that maybe will get scripted later.
		m_Targets[i].m_MyBody.radius = 4.0f;
		m_Targets[i].m_MyBody.SetMass(200.0f);
		m_Targets[i].m_MyBody.dragCoefficient = m_Targets[i].m_MyBody.GetMass();

		if( (i%2) == 0 )
		{
			//if even, player 2's targets 
			m_Targets[i].m_TeamID = 0;

			//Turn arround.
			m_Targets[i].RotateY(DegreesToRadians(180.0f));

			//Place it
			m_Targets[i].m_MyBody.centerPosition.z -= 30.0f;

		}
		else
		{
			//if odd.
			m_Targets[i].m_TeamID = 1;


			//Place it
			m_Targets[i].m_MyBody.centerPosition.z += 30.0f;
		}


		//How many rounds will there be.
		m_PlayerHuds[i].m_Points4Win = pGameInfo->Points4Win;

		strcpy(m_PlayerHuds[i].m_PlayerIcon.m_ModelName, pGameInfo->Players[i]._playerIconTexture);		

		//Setup the Hud
		m_PlayerHuds[i].Initalize(pGame->Window.WindowWidth,pGame->Window.WindowHeight,m_Players[i].m_HeatMeter,m_Targets[i].m_LifeMeter);

		//Some flippin goodness
		if( ! ((i%2) == 0)  )
		{
			m_PlayerHuds[i].Flip();
		}

		//Add to the rendering
		pRenderInterface->AddObject(&m_Players[i]);
		pRenderInterface->AddObject(&m_Cannons[i]);
		pRenderInterface->AddObject(&m_Shields[i]);

		//Add and set the cannons shields as well.
		pRenderInterface->AddObject(&m_Cannons[i].m_OverHeatShield);
		m_Cannons[i].m_OverHeatShield.m_MyBody.radius = m_Cannons[i].m_MyBody.radius;



		//Add to the physics
		m_Players[i].m_MyBodyIndex = m_Scene.AddBody(&m_Players[i].m_MyBody);
		m_Cannons[i].m_MyBodyIndex = m_Scene.AddBody(&m_Cannons[i].m_MyBody);
		m_Shields[i].m_MyBodyIndex = m_Scene.AddBody(&m_Shields[i].m_MyBody);

	}
}

void CBattleGameState::SetBalls()
{
	for(int i = 0; i < m_NumberBalls; i++)
	{
		//Set Values.
		m_Balls[i].m_MyBody.ZeroLinearForces();
		m_Balls[i].m_MyBody.centerPosition =  CVector3f(float(rand()%10-5), float(rand()%20 - 2), float(rand()%10-5));
		m_Balls[i].m_MyBody.SetMass(1.0f);
		m_Balls[i].m_MyBody.radius = 1.7f;
		m_Balls[i].m_MyBody.dragCoefficient = 0.2f ;
		m_Balls[i].m_Dangerous  = false;
		m_Balls[i].m_Visible = true;
		m_Balls[i].m_MyBody.isNapping   = false;
		m_Balls[i].m_PickupAble = true;
		m_Balls[i].m_PlayerID = -1;
		m_Balls[i].m_DamageAmount = 5.0f;
		//m_Balls[i].m_MyBody.isEffectedByDrag = false;
		m_Balls[i].ModelIndex = m_SafeBall.ModelIndex;

	}

	//The Default ball in teh world an all its values.
	strcpy(m_SafeBall.m_ModelName,"models/ball.x" );
	m_SafeBall.m_MyBody.SetMass(1.0f);
	m_SafeBall.m_MyBody.radius = 1.7f;
}

void CBattleGameState::Reset()
{
//	pSound->StopMusic();
	m_PhysicsSimSpeed = 1.0f;
	pSound->ChangeMusicTrack(m_MUSICBackGround);
	//pSound->ChangingState();
	SetBalls();
	
	for(int i = 0; i < m_NumberPlayers; i++)
	{

		m_Shields[i].m_MyBody.ZeroLinearForces();
		m_Cannons[i].m_MyBody.ZeroLinearForces();
		m_Players[i].m_MyBody.ZeroLinearForces();

		//Target stuff
		m_Targets[i].m_Life = m_Targets[i].m_MaxLife;
		m_Targets[i].m_LifeMeter->Fill();
		m_Targets[i].m_MyBody.centerPosition.MakeZero();


		if((i % 2) == 0)
		{
			//Odd players
			m_Players[i].m_MyBody.centerPosition.MakeZero();
			m_Players[i].m_MyBody.centerPosition.z -= 30.0f;
			m_Players[i].m_MyBody.centerPosition.y += m_Players[i].m_MyBody.radius;

			m_Targets[i].m_MyBody.centerPosition.z -= 45.0f;;

		}
		else
		{
			//Even Players
			m_Players[i].m_MyBody.centerPosition.MakeZero();
			m_Players[i].m_MyBody.centerPosition.y += m_Players[i].m_MyBody.radius;
			m_Players[i].m_MyBody.centerPosition.z += 30.0f;
		
			m_Targets[i].m_MyBody.centerPosition.z += 45.0f;;
		}

		m_Cannons[i].m_BallStock.clear();
		m_Cannons[i].m_NumBalls = 0;

		//Make sure we have our default shields.
		m_Players[i].m_Barrell = NULL;
		m_Players[i].AttachCannon(&m_Cannons[i],true);
		m_Players[i].DisableShield();

		//Show the huds.
		m_PlayerHuds[i].Hide(true);

	}
	pSound->PlayQSFX(m_SFXCountDown);

}

void CBattleGameState::Pause()
{

	for(int i = 0; i < CAMERA1; i++)
	{
		if(m_MenuIcons[i].m_MyBody.centerPosition.x < MENUICON_WIDTH_F / 2.0f )
		{
			m_MenuIcons[i].m_MyBody.centerPosition.x += MOVEMENT_SPEED_F;
		}

	}


	if(m_MenuInputTimer.GetElapsedSeconds() > 0.1f)
	{


		if(pInput->ControllerJoystickUp(m_WhoPushedStart))
		{
			m_CurrentMenuSelection--;

			if(m_CurrentMenuSelection < 0)
				m_CurrentMenuSelection = 0;

			pSound->PlayQSFX(m_SFXPauseSelect);
		}
		if(pInput->ControllerJoystickDown(m_WhoPushedStart))
		{
			m_CurrentMenuSelection++;


			if(m_CurrentMenuSelection > QUIT)
				m_CurrentMenuSelection = QUIT;

			pSound->PlayQSFX(m_SFXPauseSelect);

		}

		m_MenuInputTimer.Reset();

	}



//	if ( pInput->ControllerJoyPress(0,pGameInfo->ButtonLayout[0]._shoot) || pInput->ControllerJoyPress(1,pGameInfo->ButtonLayout[1]._shoot) )
	if ( pInput->ControllerJoyPress(m_WhoPushedStart,pGameInfo->ButtonLayout[m_WhoPushedStart]._shoot) )
	{

		for(int i = 0; i < NUMOPTIONS; i++)
		{
			m_MenuIcons[i].m_MyBody.centerPosition.x  = -MENUICON_WIDTH_F / 2.0f;
		}

		//Play a sound bitches
		pSound->PlayQSFX(m_SFXPauseGame);


		switch(m_CurrentMenuSelection)
		{
		case RESUME:
			{
				m_CurrentBattleState = PLAY;
				m_PhysicsSimSpeed = 1.0f;

				break;
			}

		case RESTART:
			{
				Reset();
				m_IntroTimer.Reset();
				pRenderInterface->SetActiveCamera(m_CustomCamera);
				m_CurrentBattleState = COUNTDOWN;
				break;
			}

		case CAMERASELECT:
			{

				//Hide the huds.
				for(int i = 0 ; i < m_NumberPlayers; i++)
				{
					m_PlayerHuds[i].Hide(false);
				}


				//Display we are in free cam.
				strcpy(m_TimeDisplay.m_Text,"Free Camera");
				m_TimeDisplay.m_MyBody.centerPosition.x = (float)(pGame->Window.WindowWidth/2);
				m_TimeDisplay.m_MyBody.centerPosition.x -= 135.0f;
				m_TimeDisplay.m_MyBody.centerPosition.y = 20.0f;


				//Assign the camera to the active camera an set it to active camera.
				m_LookArroundCamera->m_MyBody.centerPosition = pRenderInterface->m_ActiveCamera->m_MyBody.centerPosition;
				pRenderInterface->SetActiveCamera(m_LookArroundCamera);
				m_CurrentBattleState = LOOKARROUND;

				break;
			}

		case QUIT:
			{
				ChangeState(pGame, CMenuState::GetInstance());
				return;
				break;
			}


		}




	}



	m_BackgroundIcon.m_MyBody.centerPosition = m_MenuIcons[m_CurrentMenuSelection].m_MyBody.centerPosition;

}






void CBattleGameState::GameWin()
{

	for(int i = 0; i < m_NumberPlayers; i++)
	{
		//Update the player.
		m_Players[i].Update(m_fTimeDelta);
	}



	//Camera stuff;
	m_WinCamera->LookAt(m_Players[m_WinningPlayerIndex].m_MyBody.centerPosition);

	m_WinCamera->MoveLeft();
	m_WinCamera->MoveBackward();
	m_WinCamera->m_MyBody.centerPosition.y -= 0.04f;

	m_PhysicsSimSpeed = 0.1f;
	if(m_IntroTimer.GetElapsedSeconds() > 1.0f)
		if( (m_IntroTimer.GetElapsedSeconds() > 5.0f) || pInput->ControllerJoyPress(0,pGameInfo->ButtonLayout[0]._shoot) )
		{
			switch(m_RoundCounter.WhoWon())
			{
			case -1:
				{
					Reset();
					m_CurrentBattleState = COUNTDOWN;
					pRenderInterface->SetActiveCamera(m_CustomCamera);
					m_IntroTimer.Reset();
					break;
				}

			default:
				{
					ChangeState(pGame,CCharacterSelectState::GetInstance());
					return;
					break;
				}
			}

		}


}




