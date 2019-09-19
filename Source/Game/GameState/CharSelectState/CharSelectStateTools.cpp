#include "../gamestate.h"



void CCharacterSelectState::CheckControls()
{

	for (int curIter = 0; curIter < m_NumPlayers; curIter++)
	{
		CheckIteratorControls(curIter);
		m_Iterators[curIter].Update(m_fTimeDelta);
		m_Iterators[curIter].m_Barrell->m_RenderingOrientation.z += 0.01f;
	}

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
}


void CCharacterSelectState::Collision()
{
	for (int curIter = 0; curIter < m_NumPlayers; curIter++)
	{
		for (int i = 0; i < m_NumCols; i++)
		{
			for (int j = 0; j < m_NumRows; j++)
			{
				if (m_pSelections[i][j].GetSelectionID() == m_Iterators[curIter].m_SelectedID)
				{
					m_pSelections[i][j].m_RenderingOrientation.y += 0.01f;
				}
				if (m_Physics.HaveTheTwoBodiesCollided(m_Iterators[curIter].m_MyBodyIndex, m_pSelections[i][j].m_MyBodyIndex))
				{
					if (m_pSelections[i][j].m_isSelectable)
					{
						pSound->PlayQSFX(SFXCollidedAndSelectable);
						if (!m_Iterators[curIter].m_isReady)
							m_Iterators[curIter].SetCurrentSelectionOption(&m_pSelections[i][j]);
					}
					else
					{
						pSound->PlayQSFX(SFXCollidedAndNotSelectable);
					}

					if( (curIter%2) == 0 )
					{
						pRenderInterface->Light[1].m_Diffuse.b = 80.0f; 
						pRenderInterface->Light[1].m_CastShadow = true;
					}
					else
					{
						pRenderInterface->Light[2].m_Diffuse.g = 80.0f;
						pRenderInterface->Light[2].m_CastShadow = true;
					}
					//Reset the timer.
					m_Iterators[curIter].m_LightStablizeTimer.Reset();
				}
			}				
		}

		//////////////////////////////////////////////////////////////////////////
		// Lights for The Iterators
		pRenderInterface->Light[curIter+1].m_MyBody.centerPosition = m_Iterators[curIter].m_MyBody.centerPosition;
		pRenderInterface->Light[curIter+1].m_MyBody.centerPosition.y += 7.0f;

		// END LIGHTING
		//////////////////////////////////////////////////////////////////////////
	}

	//////////////////////////////////////////////////////////////////////////
	// check collision against other players
	for (int curIter = 0; curIter < m_NumPlayers; curIter++)
	{
		for (int secondIter = 0; secondIter < m_NumPlayers; secondIter++)
		{
			if (secondIter == curIter)
				continue;
			if (m_Physics.HaveTheTwoBodiesCollided(m_Iterators[curIter].m_MyBodyIndex, m_Iterators[secondIter].m_MyBodyIndex))
			{
				pSound->PlayQSFX(SFXPlayersCollided);

				if (m_TimerIT.GetElapsedSeconds() > 0.5f)
				{
					m_Iterators[curIter].m_isIT		= !m_Iterators[curIter].m_isIT;
					m_Iterators[secondIter].m_isIT	= !m_Iterators[secondIter].m_isIT;
					m_TimerIT.Reset();
					return;
				}
			}

		}
	}
}


void CCharacterSelectState::CheckIteratorControls(int iterIndx)
{

	//Movement.
	if (pInput->ControllerJoystickUp(iterIndx))
	{
		m_Iterators[iterIndx].m_MovementBuffer += CVector3f(0.0f, 0.0f, 1.0f);
		//m_Iterators[iterIndx].m_MovementBuffer.x += pRenderInterface->m_ActiveCamera->m_Look.x;
		//m_Iterators[iterIndx].m_MovementBuffer.z += pRenderInterface->m_ActiveCamera->m_Look.z;
		m_Iterators[iterIndx].m_Moveing = true;
	}
	if (pInput->ControllerJoystickDown(iterIndx))
	{
		m_Iterators[iterIndx].m_MovementBuffer += CVector3f(0.0f, 0.0f, -1.0f);
		//m_Iterators[iterIndx].m_MovementBuffer.x += -pRenderInterface->m_ActiveCamera->m_Look.x;
		//m_Iterators[iterIndx].m_MovementBuffer.z += -pRenderInterface->m_ActiveCamera->m_Look.z;
		m_Iterators[iterIndx].m_Moveing = true;
	}
	if (pInput->ControllerJoystickLeft(iterIndx))
	{
		m_Iterators[iterIndx].m_MovementBuffer += CVector3f(-1.0f, 0.0f, 0.0f);
		//m_Iterators[iterIndx].m_MovementBuffer.x += -pRenderInterface->m_ActiveCamera->m_Right.x;
		//m_Iterators[iterIndx].m_MovementBuffer.z += -pRenderInterface->m_ActiveCamera->m_Right.z;
		m_Iterators[iterIndx].m_Moveing = true;
	}
	if (pInput->ControllerJoystickRight(iterIndx))
	{
		m_Iterators[iterIndx].m_MovementBuffer += CVector3f(1.0f, 0.0f, 0.0f);
		//m_Iterators[iterIndx].m_MovementBuffer.x += pRenderInterface->m_ActiveCamera->m_Right.x;
		//m_Iterators[iterIndx].m_MovementBuffer.z += pRenderInterface->m_ActiveCamera->m_Right.z;
		m_Iterators[iterIndx].m_Moveing = true;
	}

	//Make selection
	if (pInput->ControllerJoyPress(iterIndx, pGameInfo->ButtonLayout[iterIndx]._shoot))
	{
		// we are satisfied with our selection we are ready
		if (m_Iterators[iterIndx].m_SelectedID >= 0)
		{
			pSound->PlayQSFX(SFXPlayerReady);
			m_Iterators[iterIndx].m_isReady = true;

			pRenderInterface->Light[iterIndx+1].m_Diffuse = EXColor(15.0f,0.0f,0.0f,1.0f);
		}
	}
	
	if (pInput->ControllerJoyPress(iterIndx, pGameInfo->ButtonLayout[iterIndx]._block) || pInput->KeyboardKeyPress(DIK_ESCAPE))
	{
		m_iCurrentState = BACK_TO_MAIN_MENU;
	}

	if (m_DisplayHelpTimer.GetElapsedSeconds() > 0.12f)
	{
		if (pInput->ControllerJoyPress(iterIndx, pGameInfo->ButtonLayout[iterIndx]._sprint))
		{
			m_HelpScreen->m_Visible = !m_HelpScreen->m_Visible;
		}
		m_DisplayHelpTimer.Reset();
	}
}