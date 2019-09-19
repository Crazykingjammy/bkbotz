#include "../GameState.h"

#define START_RUMBLE 1
#define STOP_RUMBLE 0

void CBattleGameState::CollisionDetection()
{
	for (int i = 0; i < numOfBalls; i++)
	{
			//Player 1 Collision
			if (m_Scene.HaveTheTwoBodiesCollided(balls[i].m_MyBodyIndex, Player1->m_MyBodyIndex))
			{
				Player1->PickUpBall(&balls[i]);
				break;
			}

			if( (m_Scene.HaveTheTwoBodiesCollided(balls[i].m_MyBodyIndex,ShieldPlayer1->m_MyBodyIndex)) && !ShieldPlayer1->m_MyBody.isNapping )
			{
				// play SFX
				pSound->PlaySFX(m_SFXBallHitShield);
				//Create the forward direction. because we are jus giving it the players forward direction.

				//Decrease Heat
				Player2->AddHeat(-Player1->m_KE);

				//Set the ID
				balls[i].m_PlayerID = ShieldPlayer1->m_MyBodyIndex;
				//Reflect by players foward direciton for now...
				balls[i].m_MyBody.Add2Force( (player1DirectionVec) *  ShieldPlayer1->m_ReflectingForce);
			
			}


			//Player 2 Collision
			if (m_Scene.HaveTheTwoBodiesCollided(balls[i].m_MyBodyIndex, Player2->m_MyBodyIndex))
			{
				Player2->PickUpBall(&balls[i]);
				break;
			}

			if( (m_Scene.HaveTheTwoBodiesCollided(balls[i].m_MyBodyIndex,ShieldPlayer2->m_MyBodyIndex)) && !ShieldPlayer2->m_MyBody.isNapping )
			{
				// play SFX
				pSound->PlaySFX(m_SFXBallHitShield);

				//Decrease Heat
				Player2->AddHeat(-Player2->m_KE);

				//Set the ID
				balls[i].m_PlayerID = ShieldPlayer2->m_MyBodyIndex;
				//Reflect by players foward direciton for now...
				balls[i].m_MyBody.Add2Force( (player2DirectionVec) *  ShieldPlayer2->m_ReflectingForce);
			}

			//Target stuff
			if(balls[i].m_Active)
			{
				for(int j = 0; j < NumTargets; j++)
				{

					

					if (m_Scene.HaveTheTwoBodiesCollided(balls[i].m_MyBodyIndex, Targets[j].m_MyBodyIndex))
					{
						if( (balls[i].m_PlayerID == Player1->m_MyBodyIndex)    && ( (j % 2) == 1) )
						{
							Player1Score += 1;
							balls[i].m_Active = false;

							//No more ID
							balls[i].m_PlayerID = -1;

							pSound->PlaySFX(this->m_SFXBallHitTarget);
						}

						if( (balls[i].m_PlayerID == Player2->m_MyBodyIndex)   && ( (j % 2) == 0)  )
						{
							Player2Score += 1;
							balls[i].m_Active = false;

							//No more ID
							balls[i].m_PlayerID = -1;
							pSound->PlaySFX(this->m_SFXBallHitTarget);
						}
					
						if( (balls[i].m_PlayerID == ShieldPlayer1->m_MyBodyIndex)    && ( (j % 2) == 1) )
						{
							Player1Score += 5;
							balls[i].m_Active = false;

							//No more ID
							balls[i].m_PlayerID = -1;
						}

						if( (balls[i].m_PlayerID == ShieldPlayer2->m_MyBodyIndex)   && ( (j % 2) == 0)  )
						{
							Player2Score += 5;
							balls[i].m_Active = false;

							//No more ID
							balls[i].m_PlayerID = -1;
						}



						if(pRenderInterface->Light[0].m_Diffuse.a <= 9.0f)
						{
							pRenderInterface->Light[2].m_Diffuse += 2.0f;
						}
						else
						{
							pRenderInterface->Light[2].m_Diffuse = 9.0f;
						}

						pRenderInterface->Light[2].m_MyBody.centerPosition = Targets[j].m_MyBody.centerPosition;



					}
				
				
				
				}

			}

	}

	
	//Pickup only your cannon bitches.....
	if(m_Scene.HaveTheTwoBodiesCollided(Player1->m_MyBodyIndex, XCannon->m_MyBodyIndex))
	{
		Player1->AttachCannon(XCannon);
	}

	if(m_Scene.HaveTheTwoBodiesCollided(Player2->m_MyBodyIndex, YCannon->m_MyBodyIndex))
	{
		Player2->AttachCannon(YCannon);
	}



}

void CBattleGameState::Controls()
{

	//See if we can get away with this.
	pInput->SetAllStates();


	if(pRenderInterface->Light[2].m_Diffuse.a > 0.5f )
	{
		pRenderInterface->Light[2].m_Diffuse -= 0.7f;
	}
	else
	{
		pRenderInterface->Light[2].m_Diffuse = 0.0f;
	}

	if(pRenderInterface->Light[2].m_Diffuse.a > 50.5f )
	{
		pRenderInterface->Light[2].m_Diffuse = 9.0f;
	}

	//////////////////////////////////////////////////////////////////////////
	// Player 1 Controls
	//////////////////////////////////////////////////////////////////////////
	player1DirectionVec.MakeZero();

	if( (pInput->Controller1JoyY() < -200) ||( pInput->KeyboardKeyPress(DIK_W) ) )
	{
		player1DirectionVec.x += pRenderInterface->m_ActiveCamera->m_Look.x;
		player1DirectionVec.z += pRenderInterface->m_ActiveCamera->m_Look.z;
	}
	if(pInput->Controller1JoyY() > 200 ||( pInput->KeyboardKeyPress(DIK_S) ))
	{	
		player1DirectionVec.x += -pRenderInterface->m_ActiveCamera->m_Look.x;
		player1DirectionVec.z += -pRenderInterface->m_ActiveCamera->m_Look.z;
	}
	if(pInput->Controller1JoyX() < -200 ||( pInput->KeyboardKeyPress(DIK_A) ))
	{
		player1DirectionVec.x += -pRenderInterface->m_ActiveCamera->m_Right.x;
		player1DirectionVec.z += -pRenderInterface->m_ActiveCamera->m_Right.z;
	}
	if(pInput->Controller1JoyX() > 200 ||( pInput->KeyboardKeyPress(DIK_D) ))
	{
		player1DirectionVec.x += pRenderInterface->m_ActiveCamera->m_Right.x;
		player1DirectionVec.z += pRenderInterface->m_ActiveCamera->m_Right.z;
	} 

	float cosAng = Player1->m_MyBody.vOrientation.DotProduct(player1DirectionVec);
	float ang = acos(cosAng);
	if (ang <= 0.001f)
	{
		ang = 0.001f;
	}
	//Player1->m_RenderingOrientation.y = ang;


	if (pInput->Controller1JoyPress(SPRINT) ||( pInput->KeyboardKeyPress(DIK_C) ))
	{
		Player1->MoveByDirection(player1DirectionVec, true);
	}
	else
	{
		Player1->MoveByDirection(player1DirectionVec, false);
	}

	//Rotating the cannon
	if(pInput->Controller1JoyPress(ROTATE_LEFT) ||( pInput->KeyboardKeyPress(DIK_Z) ))
		Player1->RotateCannonLeft();
	if(pInput->Controller1JoyPress(ROTATE_RIGHT) ||( pInput->KeyboardKeyPress(DIK_X) ))
		Player1->RotateCannonRight();
	if( (pInput->Controller1JoyPress(ROTATE_LEFT) && pInput->Controller1JoyPress(ROTATE_RIGHT) ) || ( pInput->KeyboardKeyPress(DIK_Z) && pInput->KeyboardKeyPress(DIK_X)  ) )
		Player1->CenterCannon();

	if (pInput->Controller1JoyPress(SHOOT) ||( pInput->KeyboardKeyPress(DIK_SPACE) ) )
	{
		pInput->ControllerForceFeedback(1, START_RUMBLE) ;	
		if(Player1->ChargeShot())
		{	
			//pSound->PlaySFX(this->m_SFXCharging);
		}
	}
	else
	{
		if(Player1->ReleaseChargetShot())
		{
			pSound->PlaySFX(this->m_SFXShooting);
		}
	}


	if (pInput->Controller1JoyPress(BLOCK) ||( pInput->KeyboardKeyPress(DIK_V) ) )
	{

		pInput->ControllerForceFeedback(1, START_RUMBLE) ;	
		Player1->ActivateShield();
	}
	else
	{
		Player1->DisableShield();
	}
	//////////////////////////////////////////////////////////////////////////
	// Player 1 Controls
	//////////////////////////////////////////////////////////////////////////




	//////////////////////////////////////////////////////////////////////////
	// Player 2 Controls
	//////////////////////////////////////////////////////////////////////////
	player2DirectionVec.MakeZero();
	if( (pInput->Controller2JoyY() < -200) || ( pInput->KeyboardKeyPress(DIK_UP) ))
	{
		player2DirectionVec.x += pRenderInterface->m_ActiveCamera->m_Look.x;
		player2DirectionVec.z += pRenderInterface->m_ActiveCamera->m_Look.z;
	}
	if( (pInput->Controller2JoyY() > 200) || ( pInput->KeyboardKeyPress(DIK_DOWN)) )
	{	
		player2DirectionVec.x += -pRenderInterface->m_ActiveCamera->m_Look.x;
		player2DirectionVec.z += -pRenderInterface->m_ActiveCamera->m_Look.z;
	}
	if( ( pInput->Controller2JoyX() < -200) || ( pInput->KeyboardKeyPress(DIK_LEFT)) )
	{
		player2DirectionVec.x += -pRenderInterface->m_ActiveCamera->m_Right.x;
		player2DirectionVec.z += -pRenderInterface->m_ActiveCamera->m_Right.z;
	}
	if( (pInput->Controller2JoyX() > 200) || ( pInput->KeyboardKeyPress(DIK_RIGHT)) )
	{
		player2DirectionVec.x += pRenderInterface->m_ActiveCamera->m_Right.x;
		player2DirectionVec.z += pRenderInterface->m_ActiveCamera->m_Right.z;
	} 

	if (pInput->Controller2JoyPress(SPRINT) || ( pInput->KeyboardKeyPress(DIK_1) ) )
	{
		Player2->MoveByDirection(player2DirectionVec, true);
	}
	else
	{
		Player2->MoveByDirection(player2DirectionVec, false);
	}


	//Rotating the cannon
	if(pInput->Controller2JoyPress(ROTATE_LEFT) || ( pInput->KeyboardKeyPress(DIK_4)) )
		Player2->RotateCannonLeft();
	if(pInput->Controller2JoyPress(ROTATE_RIGHT) || ( pInput->KeyboardKeyPress(DIK_5)) )
		Player2->RotateCannonRight();
	if( ( pInput->Controller2JoyPress(ROTATE_LEFT) && pInput->Controller2JoyPress(ROTATE_RIGHT)) || (pInput->KeyboardKeyPress(DIK_4) && pInput->KeyboardKeyPress(DIK_5))   )
		Player2->CenterCannon();

	if (pInput->Controller2JoyPress(SHOOT) || ( pInput->KeyboardKeyPress(DIK_RETURN) ) )
	{
		pInput->ControllerForceFeedback(2, START_RUMBLE) ;	
		if(Player2->ChargeShot())
		{
			//pSound->PlaySFX(this->m_SFXCharging);
		}
	}
	else
	{
		if(Player2->ReleaseChargetShot())
		{
			pSound->PlaySFX(this->m_SFXShooting);
		}
	}



	if (pInput->Controller2JoyPress(BLOCK) || ( pInput->KeyboardKeyPress(DIK_0)))
	{

		pInput->ControllerForceFeedback(2, START_RUMBLE) ;	
		Player2->ActivateShield();
	}
	else
	{
		Player2->DisableShield();
	}
	//////////////////////////////////////////////////////////////////////////
	// Player 1 Controls
	//////////////////////////////////////////////////////////////////////////



	// timer >= inpout time
	// do this
	if(GetAsyncKeyState('I'))
		pRenderInterface->m_ActiveCamera->MoveUp();
	if(GetAsyncKeyState('K'))
		pRenderInterface->m_ActiveCamera->MoveDown();
	if(GetAsyncKeyState('J'))
		pRenderInterface->m_ActiveCamera->MoveLeft();
	if(GetAsyncKeyState('L'))
		pRenderInterface->m_ActiveCamera->MoveRight();
	if(GetAsyncKeyState('U'))
		pRenderInterface->m_ActiveCamera->MoveFoward();
	if(GetAsyncKeyState('O'))
		pRenderInterface->m_ActiveCamera->MoveBackward();


	
	if(GetAsyncKeyState('M'))
	{
		pRenderInterface->SetActiveCamera(Camera0);
		Camera0->m_Visible = false;
	}
	if(GetAsyncKeyState('N'))
	{
		pRenderInterface->SetActiveCamera(Camera1);
		Camera0->m_Visible = true;
	}


	if(GetAsyncKeyState('F'))
		pRenderInterface->m_ActiveCamera->LookLeft();
	if(GetAsyncKeyState('H'))
		pRenderInterface->m_ActiveCamera->LookRight();
	if(GetAsyncKeyState('T'))
		pRenderInterface->m_ActiveCamera->LookUp();
	if(GetAsyncKeyState('G'))
		pRenderInterface->m_ActiveCamera->LookDown();


	m_Scene.SetPenetrationFlag(true);
	if (GetAsyncKeyState('Z'))
	{
		for(int i = 0; i < numOfBalls; i++)
		{
				balls[i].m_MyBody.Add2Force((Player1->m_MyBody.centerPosition - balls[i].m_MyBody.centerPosition)*100);
		}
	}

}


void CBattleGameState::GameRun()
{
	//Check the score
	if( (Player1Score >= WinningScore) ||  (Player2Score >= WinningScore) )
	{
		Score->m_Visible = true;


		CurrentState = WIN;
		m_WinningTime.Reset();
	}

	if ( pInput->PressStart() || pInput->KeyboardKeyPress(DIK_ESCAPE))
	{
		Resume->m_MyBody.isNapping = false;
		Resume->m_Visible = true;

		Restart->m_MyBody.isNapping = false;
		Restart->m_Visible = true;

		Exit->m_MyBody.isNapping = false;
		Exit->m_Visible = true;

		Me->m_MyBody.isNapping = false;
		Me->m_Visible  = true;

		CurrentState = PAUSE;
		return;
	}

	//Check for controls.
	if (m_InputTimer.GetElapsedSeconds() >= m_InputFreq)
	{
		Controls();
		m_InputTimer.Reset();
	}

	// Update the physics
	m_Scene.Simulate();

	Player1->Update();
	Player2->Update();

	CollisionDetection();

	//If player is beyong boundes.. set him to the broundry.
	if(Player1->m_MyBody.centerPosition.z >= 0.0f)
	{
		Player1->m_MyBody.centerPosition.z = 0.0f;
	}

	if(Player2->m_MyBody.centerPosition.z <= 0.0f)
	{
		Player2->m_MyBody.centerPosition.z = 0.0f;
	}


	if(XCannon->m_MyBody.centerPosition.z >= 10.0f)
	{
		XCannon->m_MyBody.velocityVec.z *= -1.5f;
	}

	if(YCannon->m_MyBody.centerPosition.z <= -10.0f)
	{
		YCannon->m_MyBody.velocityVec.z *= -1.5f;
	}

	//keep targets on the wall bitches.
	for(int i = 0; i < NumTargets; i++)
	{
		Targets[i].m_MyBody.Add2Force( (Targets[i].m_MyBody.vOrientation * TargetPullbackForce) );
		Targets[i].m_MyBody.Add2Force( (CVector3f(0.0f,-1.0f,0.0f) * TargetPulldownForce)		);
	}

	//See if a ball is active or not
	for(int i = 0; i < numOfBalls; i++)
	{
		//Object is basicaly at rest.
		if(  (balls[i].m_MyBody.velocityVec.Magnitude()  < 0.7f )   )
		{
			//No Player ID;
			balls[i].m_PlayerID = -1;
			balls[i].m_Active =  false;
		}
	}

	//Camera Update.
	Camera0->LookAt(Player1);
	Camera1->LookAt(pRenderInterface->m_World);


	char test[128];
	sprintf(test, "%d Score %d",Player1Score,Player2Score);
	strcpy(Score->m_Text,test);


	cout << "Balls : " << XCannon->m_NumBalls << endl;

	//State change params Last thing to be called.
	if(GetAsyncKeyState('Q'))
	{
		ChangeState(pGame,CIntroGameState::GetInstance());
		return;
	}
}

void CBattleGameState::Reset()
{
	Player1Score = 0;
	Player2Score = 0;

	Score->m_Visible = true;
	pSound->StopMusic();


	XCannon->m_BallStock.clear();
	XCannon->m_NumBalls = 0;
	YCannon->m_BallStock.clear();
	YCannon->m_NumBalls = 0;

	//Clear the cannons
	Player1->m_Barrell = NULL;
	Player2->m_Barrell = NULL;

	for(int i = 0; i < numOfBalls; i++)
	{
		balls[i].m_Active = false;
		balls[i].m_MyBody.isNapping = false;
		balls[i].m_PlayerID = -1;
		balls[i].m_MyBody.centerPosition = CVector3f(float(rand()%10-5), float(rand()%20 - 2), float(rand()%10-5));

	}


	Player1->m_MyBody.centerPosition.MakeZero();
	Player1->m_MyBody.centerPosition.y += Player1->m_MyBody.radius;
	Player1->m_MyBody.centerPosition.z -= 45.0f;

	Player2->m_MyBody.centerPosition.MakeZero();
	Player2->m_MyBody.centerPosition.y += Player2->m_MyBody.radius;
	Player2->m_MyBody.centerPosition.z += 45.0f;

	Player1->AttachCannon(XCannon);
	Player2->AttachCannon(YCannon);

	for (int i = 0; i < NumTargets; i++)
	{
		Targets[i].m_MyBody.forceVec.MakeZero();
		Targets[i].m_MyBody.velocityVec.MakeZero();
		Targets[i].m_MyBody.centerPosition.z = 35.0f;
		Targets[i].m_MyBody.centerPosition.x = ( (float) (i * (Targets->m_MyBody.radius + 1.0f) ) ) ; 

		//if even.
		if( (i % 2)  == 0)
		{
			Targets[i].m_MyBody.centerPosition.z = -35.0f;
		}
	}

}


void CBattleGameState::Initalize2D()
{
	m_2DScene.Set2DWorld(800.0f,600.0f);
	m_2DScene.sceneGravity.MakeZero();


	//Add player 1 heat bar
	pRenderInterface->AddLabel(Player1->m_HeatMeter);

	Player1->m_HeatMeter->m_MyBody.centerPosition.x = 0.0f + 128.0f;//(float)(Player1->m_HeatMeter->m_ImageWidth/2);
	Player1->m_HeatMeter->m_MyBody.centerPosition.y = 30.0f;

	//Add player 2 heat bar
	pRenderInterface->AddLabel(Player2->m_HeatMeter);

	Player2->m_HeatMeter->m_MyBody.centerPosition.x = 800.0f - 50.0f;//(float)(Player2->m_HeatMeter->m_ImageWidth/2));
	Player2->m_HeatMeter->m_MyBody.centerPosition.y = 30.0f;

	GameText = new CLabel("Textures/jamtext.bmp");
	GameText->m_MyBody.centerPosition.x = 400.0f;
	GameText->m_MyBody.centerPosition.y = 300.0f;
	strcpy(GameText->m_Text,"3");
	pRenderInterface->AddLabel(GameText);

	Score = new CLabel("Textures/jamtext.bmp");
	Score->m_MyBody.centerPosition.x = 280.0f;
	Score->m_MyBody.centerPosition.y = 30.0f;
	Score->m_Visible = false;
	pRenderInterface->AddLabel(Score);


	Me = new CLabel("Textures/MenuIcon_0.bmp");
	pRenderInterface->AddLabel(Me);
	Me->m_MyBody.centerPosition.x = 450.0f;
	Me->m_MyBody.centerPosition.y = 300.0f;
	Me->m_MyBody.mass = 10.0f;
	Me->m_MyBody.radius = 64.0f;;
	Me->m_MyBodyIndex = m_2DScene.AddBody(&Me->m_MyBody);
	Me->m_MyBody.isNapping = true;
	Me->m_Visible = false;
	MeForce = 1700.0f;

	Resume = new CLabel("Textures/MenuIcon_1.bmp");
	pRenderInterface->AddLabel(Resume);

	Resume->m_MyBody.centerPosition.x += 300.0f;
	Resume->m_MyBody.centerPosition.y += 150.0f;
	Resume->m_MyBody.radius = 10.0f;
	Resume->m_MyBody.mass = 1.0f;
	Resume->m_MyBody.isNapping = true;
	Resume->m_Visible = false;
	Resume->m_MyBodyIndex = m_2DScene.AddBody(&Resume->m_MyBody);
	//Battle->m_MyBody.isEffectedByGravity = false;


	Restart = new CLabel("Textures/MenuIcon_2.bmp");
	pRenderInterface->AddLabel(Restart);

	Restart->m_MyBody.centerPosition.x += 300.0f;
	Restart->m_MyBody.centerPosition.y += 450.0f;
	Restart->m_MyBody.radius = 10.0f;
	Restart->m_MyBody.mass = 1.0f;
	Restart->m_MyBody.isNapping = true;
	Restart->m_Visible = false;
	Restart->m_MyBodyIndex = m_2DScene.AddBody(&Restart->m_MyBody);
	//Credits->m_MyBody.isEffectedByGravity = false;


	Exit = new CLabel("Textures/MenuIcon_4.bmp");
	pRenderInterface->AddLabel(Exit);

	Exit->m_MyBody.centerPosition.x += 600.0f;
	Exit->m_MyBody.centerPosition.y += 450.0f;
	Exit->m_MyBody.radius = 10.0f;
	Exit->m_MyBody.mass = 1.0f;
	Exit->m_MyBody.isNapping = true;
	Exit->m_Visible = false;
	Exit->m_MyBodyIndex = m_2DScene.AddBody(&Exit->m_MyBody);

}

void CBattleGameState::Pause()
{
	m_2DScene.Simulate();

	Me->m_MyBody.Add2Force( CVector3f(0.0f, 700.0f,0.0f));

	if(m_2DScene.HaveTheTwoBodiesCollided(Resume->m_MyBodyIndex,Me->m_MyBodyIndex))
	{
		CurrentSelection = RESUME;
	}

	if(m_2DScene.HaveTheTwoBodiesCollided(Restart->m_MyBodyIndex,Me->m_MyBodyIndex))
	{
		CurrentSelection = RESTART;
	}

	if(m_2DScene.HaveTheTwoBodiesCollided(Exit->m_MyBodyIndex,Me->m_MyBodyIndex))
	{
		CurrentSelection = EXIT;
	}



	//Move me arround niggA
	if ( ( pInput->Controller1JoyPress(BLOCK) )  || ( pInput->Controller2JoyPress(BLOCK) ) || pInput->KeyboardKeyPress(DIK_SPACE) )
	{
		Me->m_MyBody.velocityVec.MakeZero();
	}

 	if(  pInput->ControllerJoystickUp()  )
	{
		Me->m_MyBody.Add2Force( CVector3f(0.0f,-1.0f,0.0f) * (MeForce * 4.0f) );
	}
	if( pInput->ControllerJoystickDown() )
	{	
		Me->m_MyBody.Add2Force( CVector3f(0.0f,1.0f,0.0f) * MeForce );
	}
	if(pInput->ControllerJoystickLeft() )
	{
		Me->m_MyBody.Add2Force( CVector3f(-1.0f,0.0f,0.0f) * MeForce );
	}
	if(pInput->ControllerJoystickRight() )
	{
		Me->m_MyBody.Add2Force( CVector3f(1.0f,0.0f,0.0f) * MeForce );
	} 

	switch(CurrentSelection)
	{
		char test[128];

	case RESUME:
		{

			sprintf(test, "Resume Game");
			strcpy(Score->m_Text,test);
			if ( (pInput->Controller1JoyPress(SHOOT)) || (pInput->Controller2JoyPress(SHOOT)) || (pInput->KeyboardKeyPress(DIK_RETURN)) )
			{
				CurrentState = PLAY;
				Resume->m_MyBody.isNapping = true;
				Resume->m_Visible = false;

				Restart->m_MyBody.isNapping = true;
				Restart->m_Visible = false;

				Exit->m_MyBody.isNapping = true;
				Exit->m_Visible = false;

				Me->m_MyBody.isNapping = true;
				Me->m_Visible = false;


				//Reset positions
				Me->m_MyBody.centerPosition.x = 450.0f;
				Me->m_MyBody.centerPosition.y = 300.0f;

				Resume->m_MyBody.centerPosition.x = 300.0f;
				Resume->m_MyBody.centerPosition.y = 150.0f;

				Restart->m_MyBody.centerPosition.x = 300.0f;
				Restart->m_MyBody.centerPosition.y = 450.0f;

				Exit->m_MyBody.centerPosition.x = 600.0f;
				Exit->m_MyBody.centerPosition.y = 450.0f;

			}


			break;
		}

	case RESTART:
		{

			sprintf(test, "Restart Game");
			strcpy(Score->m_Text,test);

			if ( (pInput->Controller1JoyPress(SHOOT)) || (pInput->Controller2JoyPress(SHOOT)) || (pInput->KeyboardKeyPress(DIK_RETURN)) )
			{
				CurrentState = COUNT_DOWN;
				Resume->m_MyBody.isNapping = true;
				Resume->m_Visible = false;

				Restart->m_MyBody.isNapping = true;
				Restart->m_Visible = false;

				Exit->m_MyBody.isNapping = true;
				Exit->m_Visible = false;

				Me->m_MyBody.isNapping = true;
				Me->m_Visible = false;


				//Reset positions
				Me->m_MyBody.centerPosition.x = 450.0f;
				Me->m_MyBody.centerPosition.y = 300.0f;

				Resume->m_MyBody.centerPosition.x = 300.0f;
				Resume->m_MyBody.centerPosition.y = 150.0f;

				Restart->m_MyBody.centerPosition.x = 300.0f;
				Restart->m_MyBody.centerPosition.y = 450.0f;

				Exit->m_MyBody.centerPosition.x = 600.0f;
				Exit->m_MyBody.centerPosition.y = 450.0f;

				Reset();

				m_WinningTime.Reset();
			}

			break;
		}

	case EXIT:
		{

			sprintf(test, "Exit Game");
			strcpy(Score->m_Text,test);

		if ( (pInput->Controller1JoyPress(SHOOT)) || (pInput->Controller2JoyPress(SHOOT)) || (pInput->KeyboardKeyPress(DIK_RETURN)) )
			{
				ChangeState(pGame,CIntroGameState::GetInstance());
				return;
			}

			break;
		}


	}

}


void CBattleGameState::InitalizeObjects()
{

	//////////////////////////////////////////////////////////////////////////
	// Players
	//////////////////////////////////////////////////////////////////////////

	//Player 1

	XCannon = new CBarrell("Models/Cannon.x");
	Player1 = new CPlayer("Models/sbot.x","Textures/HeatMeter.bmp");
	ShieldPlayer1 = new CShield("Models/Shield.x");

	//Set orientation before you attach cannon pleeeaasseee
	//Player1->RotateY(DegreesToRadians(180));



	//Setup Player.
	Player1->m_fRegular = 37.0f;
	Player1->m_fSprint = 100.0f;
	Player1->m_MyBody.centerPosition.z += 45.0f;
	Player1->m_MyBody.mass = 10.0f;
	Player1->m_MyBody.radius = 3.0f;
	Player1->m_MyBody.isMovableByCollision = false;
	Player1->m_MyBodyIndex = m_Scene.AddBody(&Player1->m_MyBody);

	//Setup Cannon.
	XCannon->m_MyBody.radius = 2.0f;
	XCannon->m_MyBody.isNapping = true;
	XCannon->m_RotationSpeed += 0.03f;


	//Setup Shield

	ShieldPlayer1->m_MyBody.radius = 7.0f;
	pRenderInterface->AddObject(ShieldPlayer1);

	//Add to Render
	pRenderInterface->AddObject(XCannon);
	pRenderInterface->AddObject(Player1);

	//Add to physics scene  
	ShieldPlayer1->m_MyBodyIndex = m_Scene.AddBody(&ShieldPlayer1->m_MyBody);
	XCannon->m_MyBodyIndex = m_Scene.AddBody(&XCannon->m_MyBody);


	//Attach to player
	Player1->AttachCannon(XCannon);
	Player1->AttachShield(ShieldPlayer1);



	//Player 2

	YCannon = new CBarrell("Models/CannonPlayer.X");
	Player2 = new CPlayer("Models/player2.x","Textures/HeatMeter.bmp");
	ShieldPlayer2 = new CShield("Models/Shield.x");

	//Set orientation before you attach cannon pleeeaasseee
	Player2->RotateY(DegreesToRadians(180));



	//Setup Player.
	Player2->m_fRegular = 37.0f;
	Player2->m_fSprint = 100.0f;
	Player2->m_MyBody.centerPosition.z -= 45.0f;
	Player2->m_MyBody.mass = 10.0f;
	Player2->m_MyBody.radius = 3.0f;
	Player2->m_MyBody.isMovableByCollision = false;
	Player2->m_MyBodyIndex = m_Scene.AddBody(&Player2->m_MyBody);

	//Setup Cannon.
	YCannon->m_MyBody.radius = 2.0f;
	YCannon->m_MyBody.isNapping = true;
	YCannon->m_RotationSpeed += 0.03f;


	//Setup Shield

	ShieldPlayer2->m_MyBody.radius = 7.0f;
	pRenderInterface->AddObject(ShieldPlayer2);

	//Add to Render
	pRenderInterface->AddObject(YCannon);
	pRenderInterface->AddObject(Player2);

	//Add to physics scene  
	ShieldPlayer2->m_MyBodyIndex = m_Scene.AddBody(&ShieldPlayer2->m_MyBody);
	YCannon->m_MyBodyIndex = m_Scene.AddBody(&YCannon->m_MyBody);


	//Attach to player
	Player2->AttachCannon(YCannon);
	Player2->AttachShield(ShieldPlayer2);

	//////////////////////////////////////////////////////////////////////////
	// Players
	//////////////////////////////////////////////////////////////////////////


	numOfBalls = 20;
	balls = new CBall[numOfBalls];
	char buffer[128];
	for (int i = 0; i < numOfBalls; i++)
	{
		strcpy(balls[i].m_ModelName,"Models/ball.x");
		balls[i].m_MyBody.centerPosition = CVector3f(float(rand()%10-5), float(rand()%20 - 2), float(rand()%10-5));
		balls[i].m_MyBody.mass	  = 1.0f;
		balls[i].m_MyBody.radius  = 2.0f;
		balls[i].m_MyBody.dragCoefficient = 0.20f;
		balls[i].m_MyBodyIndex = m_Scene.AddBody(&balls[i].m_MyBody);
		sprintf(buffer, "Ball %d", i);
		strcpy(balls[i].m_MyBody.myName, buffer);
		pRenderInterface->AddObject(&balls[i]);
	}


	//balls[0].m_PickupAble = false;
	//strcpy(balls[0].m_ModelName,"Models/SoccerBall.x");




	//Targets

	TargetPullbackForce = 250.0f;
	TargetPulldownForce = 50.0f;

	NumTargets = 2;
	Targets = new CTarget[NumTargets];
	for(int i = 0; i < NumTargets; i++)
	{
		strcpy(Targets[i].m_ModelName,"Models/Target1.x");
		Targets[i].m_MyBody.radius = 4.0f;
		Targets[i].m_MyBody.mass = 50.0f;
		Targets[i].m_MyBody.dragCoefficient = 1.0f;
		Targets[i].m_MyBodyIndex = m_Scene.AddBody(&Targets[i].m_MyBody);
		Targets[i].m_MyBody.centerPosition.x = ( (float) (i * (Targets->m_MyBody.radius + 1.0f) ) ) ; 

		//If even
		if( (i % 2) == 0 )
		{
			Targets[i].RotateY(DegreesToRadians(180));
			strcpy(Targets[i].m_ModelName,"Models/Target2.x");
		}
		pRenderInterface->AddObject(&Targets[i]);

	}

	//	Targets[i].m_MyBody.centerPosition.z = 
}