#include "../Gamestate.h"

void CBattleGameState::Controls()
{
	pInput->SetAllStates();



	//Control all the players.
	for(int i = 0 ; i < m_NumberPlayers; i++)
	{
		PlayerControls(i);
	}

	//BS.

	if (GetAsyncKeyState('1'))
	{
		m_PhysicsSimSpeed += 0.1f;
		if (m_PhysicsSimSpeed > 1.0f) m_PhysicsSimSpeed = 1.0f;
	}

	if (GetAsyncKeyState('2'))
	{
		m_PhysicsSimSpeed -= 0.1f;
		if (m_PhysicsSimSpeed < 0.0f) m_PhysicsSimSpeed = 0.0f;
	}

}

void CBattleGameState::CollisionDetection()
{
	//For collisions with any balls...
	for(int currBall = 0; currBall < m_NumberBalls; currBall++)
	{
		if (m_Balls[currBall].m_Dangerous && m_Scene.HasTheBodyCollidedWithWall(m_Balls[currBall].m_MyBodyIndex, m_FrontWall))
		{
			InactiveBall(currBall);
		}
		if (m_Balls[currBall].m_Dangerous && m_Scene.HasTheBodyCollidedWithWall(m_Balls[currBall].m_MyBodyIndex, m_BackWall))
		{
			InactiveBall(currBall);
		}
		if (!m_Balls[currBall].m_Dangerous)
		{
			CVector3f gravPoint;
			gravPoint.MakeRandom(135.0f, -135.0f, 0.0f, 0.0f, 0.0f, 0.0f);
			m_Balls[currBall].m_MyBody.PushTowardAPosi
				(gravPoint, 2.50f);
		}

		//Player collision with balls.
		for(int curPlayer = 0; curPlayer < m_NumberPlayers; curPlayer++)
		{

			//Make shield add force.
			if(m_Players[curPlayer].m_ShieldActive)
			{
				//m_Balls[currBall].m_MyBody.Add2Force( (  ( m_Balls[currBall].m_MyBody.centerPosition) - (m_Players[curPlayer].m_MyBody.centerPosition) ) * (m_Players[curPlayer].m_Shield->m_ReflectingForce/4.0f) ); 
				m_Balls[currBall].m_MyBody.PushTowardABody(&m_Players[curPlayer].m_MyBody,(m_Players[curPlayer].m_HeatFromBlocking * m_Players[curPlayer].m_Damage ) );
			}

			if (m_Scene.HaveTheTwoBodiesCollided(m_Balls[currBall].m_MyBodyIndex, m_Players[curPlayer].m_MyBodyIndex))
			{
				if(m_Balls[currBall].m_Dangerous && (m_Balls[currBall].m_PlayerID != m_Players[curPlayer].m_MyBodyIndex) && (m_Balls[currBall].m_PlayerID != m_Shields[curPlayer].m_MyBodyIndex) )
				{
					if(m_Players[curPlayer].m_Barrell)
					{
						pSound->PlayQSFX(m_SFXBallHitPlayer, true);
						InactiveBall(currBall);
						m_Players[curPlayer].AddHeat( (m_Balls[currBall].m_DamageAmount * 7.0f)  - m_Players[curPlayer].m_Damage );

					}
				}
				else
				{
					//If everything is good we atempt to pick it up.
					if (m_Players[curPlayer].PickUpBall(&m_Balls[currBall]))
					{
						pSound->PlayQSFX(m_SFXPickUpBall, true);
					}
				}


			}		

			//Players cannon against the balls

			//if there was a previous barell
			if(m_Players[curPlayer].m_PreviousBarrell)
			{
				//Check if the players previous cannon has been hit.
				if( m_Scene.HaveTheTwoBodiesCollided(m_Balls[currBall].m_MyBodyIndex, m_Players[curPlayer].m_PreviousBarrell->m_MyBodyIndex) )
				{
					//If its yours or an inactive ball.
					if(m_Balls[currBall].m_Dangerous && (m_Balls[currBall].m_PlayerID != m_Players[curPlayer].m_MyBodyIndex) && (m_Balls[currBall].m_PlayerID != m_Shields[curPlayer].m_MyBodyIndex) )
					{

						//Do the casual deactivation shit.
						pSound->PlayQSFX(m_SFXBallHitHead, true);
						InactiveBall(currBall);
						m_Players[curPlayer].AddHeat( (m_Balls[currBall].m_DamageAmount) * 4.0f);					
					}

				}
			}


			//Shields!
			if (m_Scene.HaveTheTwoBodiesCollided(m_Balls[currBall].m_MyBodyIndex, m_Shields[curPlayer].m_MyBodyIndex)   )
			{
				//Play sound and add force :-)
				m_Balls[currBall].m_MyBody.Add2Force((m_Players[curPlayer].m_MyBody.vOrientation * m_Shields[curPlayer].m_ReflectingForce) * 0.5f );
				pSound->PlayQSFX(m_SFXBlockNotDangerous, true);

				//Set thee balls id, and stats after you deflect dammit!!
				if(m_Balls[currBall].m_Dangerous)
				{
					//Play sound and add force :-)
					m_Balls[currBall].m_MyBody.Add2Force((m_Players[curPlayer].m_MyBody.vOrientation * m_Shields[curPlayer].m_ReflectingForce) * m_Balls[currBall].m_MyBody.GetMass() );

					pSound->PlayQSFX(m_SFXBlock, true);
					//m_Players[curPlayer].AddHeat(-(m_Players[curPlayer].m_Damage + m_Balls[currBall].m_DamageAmount) );
					m_Players[curPlayer].m_KE = 0.0f;
					m_Players[curPlayer].m_HeatMeter->Empty();

					//Shrink the shield when you get hit.
					m_Shields[curPlayer].m_MyBody.radius -= m_Shields[curPlayer].m_ShrinkHit;

					m_Balls[currBall].m_PlayerID = m_Shields[curPlayer].m_MyBodyIndex;
					//m_Balls[currBall].m_DamageAmount = m_Shields[curPlayer].m_ReflectingDamage;
				}

			}
		}

		//Target collision with balls.
		for(int curTarget = 0; curTarget < m_NumberTargets; curTarget++)
		{

			//if (m_Targets[curTarget].m_Life - m_Balls[currBall].m_DamageAmount <= 0.0f)
			//{
			//	if (CheckForWinningShot(m_Balls[currBall].m_MyBody.centerPosition, m_Balls[currBall].m_MyBody.velocityVec, m_Targets[curTarget].m_MyBody.centerPosition))
			//	{
			//		m_PhysicsSimSpeed = 0.01f;
			//		m_ArenaCamera->LookAt(m_Targets[curTarget].m_MyBody.centerPosition);
			//	}
			//	else
			//	{
			//		m_PhysicsSimSpeed = 1.0f;
			//		m_ArenaCamera->LookAt(pRenderInterface->m_World);
			//	}
			//}

			//if (m_Targets[curTarget].m_Life - m_Balls[currBall].m_DamageAmount <= 0.0f)
			//{
			//	if (AreWeClose(&m_Balls[currBall],&m_Targets[curTarget]))
			//	{
			//		m_PhysicsSimSpeed = 0.01f;
			//		m_ArenaCamera->LookAt(m_Targets[curTarget].m_MyBody.centerPosition);
			//	}
			//	else
			//	{
			//		m_PhysicsSimSpeed = 1.0f;
			//		m_ArenaCamera->LookAt(pRenderInterface->m_World);
			//	}
			//}

			// lets add the devasating bullet time with a camera look at

			if(m_Scene.HaveTheTwoBodiesCollided(m_Balls[currBall].m_MyBodyIndex,m_Targets[curTarget].m_MyBodyIndex))
			{
				if( (m_Balls[currBall].m_Dangerous) )
				{
					//for(int curPlayer = 0; curPlayer < m_NumberPlayers; curPlayer++)
					//{
						//TODO:: Fix it so we cant destroy our own targets. and scale the hit by velocity.
						if(true)
						{
							pSound->PlayQSFX(m_SFXTargetHit, true);

							//Hit the target.
							m_Targets[curTarget].Hit(m_Balls[currBall].m_DamageAmount);

							//Inactive the ball of course
							InactiveBall(currBall);

							if( (curTarget % 2) == 0 )
							{
								m_Explosion2.m_MyBody.centerPosition = m_Targets[curTarget].m_MyBody.centerPosition;
								m_Explosion2.Kill();
								m_Explosion2.Trigger();
							}
							else
							{
								m_Explosion.m_MyBody.centerPosition = m_Targets[curTarget].m_MyBody.centerPosition;
								m_Explosion.Kill();
								m_Explosion.Trigger();
							}



						}


					//}


				}

			}
		}

		//Balls update i hope i get rid of this.
		//float mag = m_Balls[currBall].m_MyBody.velocityVec.Magnitude();

		if( (m_Balls[currBall].m_ActiveTime.GetElapsedSeconds() > m_Balls[currBall].m_ActiveLength) &&  m_Balls[currBall].m_Dangerous ) 
		{
			InactiveBall(currBall);
		}



	}


	//Go trouch the cannons.
	for(int curCannon = 0; curCannon < m_NumberPlayers; curCannon++)
	{

		//See if thay collided with each player.
		for(int curPlayer = 0; curPlayer < m_NumberPlayers; curPlayer++)
		{
			//Collect my cannon.
			if(m_Scene.HaveTheTwoBodiesCollided(m_Cannons[curCannon].m_MyBodyIndex,m_Players[curPlayer].m_MyBodyIndex))
			{
				m_Players[curPlayer].AttachCannon(&m_Cannons[curCannon]);
				pSound->PlayQSFX(m_SFXPickUpCannon);
			}
		}

	}

}


void CBattleGameState::InactiveBall(int index)
{
	if (m_TimerRestoreDefaultInactiveBallSFX.GetElapsedSeconds() > 1.0f)
	{
		m_CurrInactiveBallSFX = 0;
	}

	PlaySFXInactiveBall();
	//Balls update i hope i get rid of this.
	float mag = m_Balls[index].m_MyBody.velocityVec.Magnitude();

	//kool.
	m_Balls[index].m_Dangerous = false;
	m_Balls[index].ModelIndex = m_SafeBall.ModelIndex;
	m_Balls[index].m_MyBody.SetMass(m_SafeBall.m_MyBody.GetMass());
	m_Balls[index].m_MyBody.radius = m_SafeBall.m_MyBody.radius;
	m_Balls[index].m_ActiveTime.Reset();
	m_Balls[index].m_PlayerID = -1;						//Inactive ball ID;
	m_Balls[index].m_MyBody.ZeroLinearForces();
	m_Balls[index].m_MyBody.Add2Force(m_Scene.Y_AXIS*5000);

}

void CBattleGameState::PlaySFXInactiveBall()
{
	pSound->PlayQSFX(m_SFXBallBackToSafeMode[m_CurrInactiveBallSFX], true);
	m_CurrInactiveBallSFX++;
	if (m_CurrInactiveBallSFX > (NUM_OF_INACTIVE_BALL_SFX-1))
	{
		m_CurrInactiveBallSFX = (NUM_OF_INACTIVE_BALL_SFX-1);
	}
	m_TimerRestoreDefaultInactiveBallSFX.Reset();
}

void CBattleGameState::PlayerControls(int index)
{
	if(pInput->ControllerJoyPress(index,pGameInfo->ButtonLayout[index]._start) || pInput->KeyboardKeyPress(DIK_ESCAPE))
	{
		m_CurrentMenuSelection = 0;
		m_CurrentBattleState = PAUSE;
		pSound->PlayQSFX(m_SFXPauseGame);

		m_WhoPushedStart = index;
	}

	//Movement.
	if (pInput->ControllerJoystickUp(index))
	{
		m_Players[index].m_MovementBuffer.x += pRenderInterface->m_ActiveCamera->m_Look.x;
		m_Players[index].m_MovementBuffer.z += pRenderInterface->m_ActiveCamera->m_Look.z;
		m_Players[index].m_Moveing = true;
	}
	if (pInput->ControllerJoystickDown(index))
	{
		m_Players[index].m_MovementBuffer.x += -pRenderInterface->m_ActiveCamera->m_Look.x;
		m_Players[index].m_MovementBuffer.z += -pRenderInterface->m_ActiveCamera->m_Look.z;
		m_Players[index].m_Moveing = true;
	}
	if (pInput->ControllerJoystickLeft(index))
	{
		m_Players[index].m_MovementBuffer.x += -pRenderInterface->m_ActiveCamera->m_Right.x;
		m_Players[index].m_MovementBuffer.z += -pRenderInterface->m_ActiveCamera->m_Right.z;
		m_Players[index].m_Moveing = true;
	}
	if (pInput->ControllerJoystickRight(index))
	{
		m_Players[index].m_MovementBuffer.x += pRenderInterface->m_ActiveCamera->m_Right.x;
		m_Players[index].m_MovementBuffer.z += pRenderInterface->m_ActiveCamera->m_Right.z;
		m_Players[index].m_Moveing = true;
	}

	//Sprinting
	if (pInput->ControllerJoyPress(index,pGameInfo->ButtonLayout[index]._sprint))
	{
		m_Players[index].m_Sprinting = true;
		pSound->PlayQSFX(m_SFXSprint);
	}

	//Shooting
	if (pInput->ControllerJoyPress(index,pGameInfo->ButtonLayout[index]._shoot))
	{
		if (m_Players[index].ChargeShot())
		{
			pSound->PlayQSFX(m_SFXCharging);
		}
		else
		{
			pSound->PlayQSFX(m_SFXOutOfAmmo, true);
		}
	}
	else
	{
		if (m_Players[index].ReleaseChargetShot())
		{
			if (m_Players[index].HasReachedMaxCharge())
			{
				pSound->PlayQSFX(m_SFXFullChargeShoot, true);
				pSound->StopSFX(m_SFXCharging);
			}
			else
			{
				pSound->PlayQSFX(m_SFXShoot, true);
				pSound->StopSFX(m_SFXCharging);
			}
		}
	}

	//Shield Time.
	if(pInput->ControllerJoyPress(index,pGameInfo->ButtonLayout[index]._block) )
	{
		m_Players[index].ActivateShield();
	}
	else
	{
		m_Players[index].DisableShield();
	}

	if (m_Players[index].m_ShieldActive)
	{
		pSound->PlayQSFX(m_SFXShieldOn);
	}
	else
	{

	}

	//if we are on lock on mode.
	if(!m_Players[index].m_LockOn)
	{
		//If we push any of the rotate buttions do a lock on.
		if(pInput->ControllerJoyPress(index,pGameInfo->ButtonLayout[index]._rotateright) || pInput->ControllerJoyPress(index, pGameInfo->ButtonLayout[index]._rotateleft) )
		{
			//Locks the reposition
			m_Players[index].m_HoldOn = true;
		}
		else
		{
			//Else we are not locked on bitches.
			m_Players[index].m_HoldOn = false;
		}
	}
	else
	{
		//Center barrell.
		m_Players[index].CenterCannon();

		//else do rotation on the barrell.
		if(pInput->ControllerJoyPress(index,pGameInfo->ButtonLayout[index]._rotateleft))
		{
			m_Players[index].RotateCannonLeft45();
		}

		if(pInput->ControllerJoyPress(index,pGameInfo->ButtonLayout[index]._rotateright))
		{
			m_Players[index].RotateCannonRight45();
		}

		//Else we are not locked on bitches.
		m_Players[index].m_HoldOn = false;
	}

	////Setting Lock on.
	//if( pInput->ControllerJoyPress(index,pGameInfo->ButtonLayout[index]._rotateleft) && pInput->ControllerJoyPress(index,pGameInfo->ButtonLayout[index]._rotateright) )
	//{
	//	//Switch the lock on mode.
	//	m_Players[index].m_LockOn = !m_Players[index].m_LockOn;
	//}

}



void CBattleGameState::CountDown()
{

	//HEY!

	//Display the round.
	char buffer[256];
	sprintf(buffer,"Round %d",m_RoundCounter.WhichRound());
	strcpy(m_TimeDisplay.m_Text,buffer);
	m_TimeDisplay.m_MyBody.centerPosition.x = (float)(pGame->Window.WindowWidth/2);
	m_TimeDisplay.m_MyBody.centerPosition.x -= 95.0f;
	m_TimeDisplay.m_MyBody.centerPosition.y = 20.0f;

	m_CountDownNumber->m_Visible = true;
	m_CountDownNumber->ModelIndex = m_CountDownIndices[5];
	m_CustomCamera->LookAt(pRenderInterface->m_World);

	if(m_IntroTimer.GetElapsedSeconds() > 1.0f)
	{
		m_CountDownNumber->ModelIndex = m_CountDownIndices[2];
		//m_CustomCamera->LookAt(&m_Players[0]);

		if(m_IntroTimer.GetElapsedSeconds() > 2.0f)
		{
			m_CountDownNumber->ModelIndex = m_CountDownIndices[1];
			//	m_CustomCamera->LookAt(&m_Players[1]);

			if(m_IntroTimer.GetElapsedSeconds() > 3.0f)
			{
				m_ArenaCamera->LookAt(pRenderInterface->m_World);
				pRenderInterface->SetActiveCamera(m_ArenaCamera);
				m_CountDownNumber->ModelIndex = m_CountDownIndices[0];
				m_CustomCamera->m_MyBody.centerPosition = CVector3f(100.0f,10.0f,0.0f);

				if(m_IntroTimer.GetElapsedSeconds() > 4.0f)
				{
					m_CountDownNumber->m_Visible = false;
					m_PhysicsSimSpeed = 1.0f;
					m_CurrentBattleState = PLAY;
					pRenderInterface->SetActiveCamera(m_ArenaCamera);

				}

			}

		}
	}


	//Scale by the fucking time.
	m_CustomCamera->m_MyBody.centerPosition.y += 0.4f;// * pGame->timeDelta;
	m_CustomCamera->MoveLeft();

}

bool CBattleGameState::CheckForWinningShot(CVector3f curBallPos, CVector3f curBallVel, CVector3f curTargetPos)
{
	// for sic bullet time look at
	CVector3f disVecTweenDBallAndTarget;
	CVector3f PosOfBall = curBallPos;
	CVector3f normal = curBallVel;
	normal.Normalize();


	if (m_Scene.CollSphereToRay(curTargetPos, 10.0f, PosOfBall, normal))
	{
		return true;
	}

	return false;
}

bool CBattleGameState::AreWeClose(CBall * ball, CTarget* target)
{
	CVector3f distance = ball->m_MyBody.centerPosition - target->m_MyBody.centerPosition;

	float ballradius = ball->m_MyBody.radius;
	float targetradius = target->m_MyBody.radius * 2.0f;

	if( distance.Magnitude() < (ballradius + targetradius) )
	{
		return true;
	}

	return false;
}

void CBattleGameState::FreeLookArround()
{
	m_LookArroundCamera->LookAt(pRenderInterface->m_World);

	if(GetAsyncKeyState('I') )
		m_LookArroundCamera->MoveUp();
	if(GetAsyncKeyState('K'))
		m_LookArroundCamera->MoveDown();
	if(GetAsyncKeyState('J'))
		m_LookArroundCamera->MoveLeft();
	if(GetAsyncKeyState('L'))
		m_LookArroundCamera->MoveRight();
	if(GetAsyncKeyState('U'))
		m_LookArroundCamera->MoveFoward();
	if(GetAsyncKeyState('O'))
		m_LookArroundCamera->MoveBackward();



	//Controller press;
	if(pInput->ControllerJoystickUp(m_WhoPushedStart))
		m_LookArroundCamera->MoveUp();

	if(pInput->ControllerJoystickDown(m_WhoPushedStart))
		m_LookArroundCamera->MoveDown();

	if(pInput->ControllerJoystickLeft(m_WhoPushedStart))
		m_LookArroundCamera->MoveLeft();

	if(pInput->ControllerJoystickRight(m_WhoPushedStart))
		m_LookArroundCamera->MoveRight();


	if(GetAsyncKeyState(VK_ESCAPE) || ( pInput->ControllerJoyPress(m_WhoPushedStart,pGameInfo->ButtonLayout[m_WhoPushedStart]._start) ) )
	{
		//Show the hud.
		for(int i = 0 ; i < m_NumberPlayers; i++)
		{
			m_PlayerHuds[i].Hide(true);
		}

		//Play sound bitches!
		pSound->PlayQSFX(m_SFXPauseGame);

		//Display back the round bitches

		//Display the round.
		char buffer[256];
		sprintf(buffer,"Round %d",m_RoundCounter.WhichRound());
		strcpy(m_TimeDisplay.m_Text,buffer);
		m_TimeDisplay.m_MyBody.centerPosition.x = (float)(pGame->Window.WindowWidth/2);
		m_TimeDisplay.m_MyBody.centerPosition.x -= 80.0f;
		m_TimeDisplay.m_MyBody.centerPosition.y = 20.0f;


		pRenderInterface->SetActiveCamera(m_ArenaCamera);
		m_CurrentMenuSelection = 0;
		m_CurrentBattleState = PAUSE;
	}

}

