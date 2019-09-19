#include "Player.h"


void CPlayer::MoveByDirection()
{
	m_MovementBuffer.Normalize();
	m_MyBody.vOrientation = m_MovementBuffer;
	
	if (m_Moveing)
	{
		if (m_Sprinting && m_Barrell)
		{
			m_MyBody.Add2Force(m_MovementBuffer * m_fSprint);
			AddHeat(m_HeatFromSprint);

		}
		else
		{
			//If we have a barrell move fast.
			if(m_Barrell)
			{
				m_MyBody.Add2Force(m_MovementBuffer * m_fRegular);
			}
			else
			{
				//if not we have to move a lil but slower.
				m_MyBody.Add2Force(m_MovementBuffer * (m_fRegular * 0.8f));
			}
		}

	}

	m_Sprinting = false;
	m_Moveing   = false;
}

void CPlayer::RepositionBarrel()
{

	strcpy(m_BallCountString,"Flame");

	if(m_PreviousBarrell)
	{
		//Set the index of the barells shield.
		m_PreviousBarrell->m_OverHeatShield.ModelIndex = m_NotReadyIndex;
		m_PreviousBarrell->m_OverHeatShield.m_MyBody.centerPosition = m_PreviousBarrell->m_MyBody.centerPosition;
	}
	

	//Tell us if we can pick up our head already.
	if(m_KE <= m_PickupKE)
	{
		strcpy(m_BallCountString,"Ready");

		if(m_PreviousBarrell)
		{
			//Set the index of the barells shield.
			m_PreviousBarrell->m_OverHeatShield.ModelIndex = m_ReadyIndex;
		}
	}

	

	if (m_Barrell)
	{
		sprintf(m_BallCountString,"%d|%d",m_Barrell->m_NumBalls,m_Barrell->m_MaxBalls);


		//Hehehhehehehe we alwayse wanna keep the cannon on the players body.
		m_Barrell->m_MyBody.centerPosition = m_MyBody.centerPosition;
		m_Barrell->m_MyBody.centerPosition.y += m_MyBody.radius;



		if(m_Shield)
		{
			m_Shield->m_MyBody.centerPosition = m_MyBody.centerPosition;
		}

		//If we are holding on dont reset the orientation.
		if(m_HoldOn && !m_LockOn)
			return;

		if( /*!(m_MyBody.vOrientation.Magnitude() < 0.01f)  &&*/ (!m_LockOn) )
		{
			m_Barrell->m_MyBody.vOrientation = m_MyBody.vOrientation;
		}

		

	}
}


bool CPlayer::PickUpBall(CBall* ball)
{
	if(!m_Barrell)
		return false;

	if(ball->m_PickupAble == false)
		return false;
	
	// cant pick up a dangerous one!!
	if(ball->m_Dangerous)
		return false;

	if(m_Barrell->m_NumBalls >= m_Barrell->m_MaxBalls)
		return false;

	//Kill the ball
	ball->m_Dangerous = false;
	ball->m_MyBody.isNapping = true;
	ball->m_PlayerID = m_MyBodyIndex;
	ball->m_Visible  = false;

	//Add ball to the stock.
	m_Barrell->m_BallStock.push_back(ball);

	//inc the num of balls
	m_Barrell->m_NumBalls++;

	return true;
}

void CPlayer::ActivateShield()
{
	if(!m_Barrell)
	{
		DisableShield();
		return;
	}
		

	if(!m_Shield)
		return;

	//Decrease the shield radius over time.
	if(m_ShieldTimer.GetElapsedSeconds() > 0.05f)
	{
		m_Shield->m_MyBody.radius -= m_Shield->m_ShrinkRate;
		m_ShieldTimer.Reset();
	}


	//Stop the player.
	m_MyBody.ZeroLinearForces();

	m_Shield->m_Visible = true;

	//Activate it in physics
	m_Shield->m_MyBody.isNapping = false;

	//Inactive the player
	m_MyBody.isNapping = true;

	m_ShieldActive = true;

	//Heat up
	AddHeat(m_HeatFromBlocking);

}

void CPlayer::DisableShield()
{
	if(!m_Shield)
		return;

	//Make me my max size;
	m_Shield->m_MyBody.radius = m_Shield->m_MaxRadius;

	m_Shield->m_Visible = false;

	//Inactivate it in physics
	m_Shield->m_MyBody.isNapping = true;

	//Active the player
	m_MyBody.isNapping = false;

	m_ShieldActive = false;
}

bool CPlayer::GetHit(float factor)
{
	////Add to the KE.
	//m_KE += (m_Damage * factor);
	//
	////Check see if we have overheated
	//if(m_KE > m_MaxKE)
	//{
	//	OverHeat();

	//	//Return true cos we overheated.
	//	return false;
	//}

	////We havent overheated yet
	//return true;

	AddHeat(factor);
	return true;

}


bool CPlayer::ChargeShot()
{
	bool charging = false;
	//If we have no cannon cant charge!
	if(!m_Barrell)
		return charging;

	if (m_Barrell->Charge())
	{
		m_bReadyToFire = true;
		if (!m_Barrell->m_bReachedMax)
		{
			AddHeat(m_HeatFromShooting);
		}
		else if (m_PlayerUpdateTimer.GetElapsedSeconds() > m_fTimeDelta)
		{
			AddHeat(m_CoolDownRate);
			m_PlayerUpdateTimer.Reset();
		}
		charging = true;
	}
	return charging;

}

bool CPlayer::ReleaseChargetShot()
{

	//If we have no cannon cant pickup bitches.....
	bool fired = false;
	if(!m_Barrell)
		return fired;

	//If we are empty return false.
	//if(m_Barrell->m_BallStock.empty())
	//	return false;

	//Reset the timer.
	//m_ChargeTimer.Reset();

	//Everything is fine.. enough balls

	if (m_bReadyToFire)
	{
		fired = m_Barrell->Shoot(m_bFullyCharged);
		m_bReadyToFire = false;
		
	}
	return fired;
}

void CPlayer::OverHeat()
{

	if (!m_Barrell)
		return;
	if (m_Barrell->PopOff())
	{
		//Refix the mass
		m_MyBody.SetMass( m_MyBody.GetMass() - m_Barrell->m_MyBody.GetMass() );

		//Show the shield
		m_Barrell->m_OverHeatShield.m_MyBodyIndex = m_NotReadyIndex;
		m_Barrell->m_OverHeatShield.m_Visible = true;

		m_Barrell = NULL;	
		m_bOverHeated = true;
	}
}

bool CPlayer::AttachCannon(CBarrell* barrell, bool dontcarebouttimer)
{
	//If we already have a barrell, cant do that then can we.
	if(m_Barrell)
	{
		return false;
	}

	if ( (m_KE < m_PickupKE) || dontcarebouttimer)
	{
		//Everytime we get a cannon we are cooled down ;-)
		m_KE = 0.0f;
		m_HeatMeter->Empty();

		//Assign the barrell
		m_Barrell			= barrell;
		m_PreviousBarrell	= barrell; 

		m_MyBody.SetMass(m_Barrell->m_MyBody.GetMass() + m_MyBody.GetMass());

		//And out lil values
		m_HeatFromShooting = m_Barrell->m_ShootingHeat;

		//Inactive the barrel with the physics.
		m_Barrell->m_MyBody.isNapping = true;
		m_Barrell->m_MyBody.ZeroLinearForces();

		//Reset the orientation.
		m_Barrell->m_RenderingOrientation = m_RenderingOrientation;
		m_Barrell->m_MyBody.vOrientation  = m_MyBody.vOrientation;

		//Assign the  barrel to the players position
		m_Barrell->m_MyBody.centerPosition = m_MyBody.centerPosition;
		m_Barrell->m_MyBody.centerPosition.y += m_MyBody.radius;

		//Hide the barrels shield if its visible..
		m_Barrell->m_OverHeatShield.m_Visible = false;

		m_bOverHeated = false;

		//Everything is good.
		return true;
	}

	return false;
}

bool CPlayer::AttachShield(CShield* shield)
{
	//If we have a shield cant pick up another one.
	if(m_Shield)
	{
		return false;
	}

	m_Shield = shield;

	//Kill is physics and make invisible.
	m_Shield->m_MyBody.isNapping = true;
	m_Shield->m_Visible	= false;

	//Everything is good
	return true;
}

void CPlayer::Update(float timeDelta)
{
	m_MyBody.Add2Force(CVector3f(0.0f, -(300.8f*m_MyBody.GetMass()), 0.0f));

	if (m_PlayerUpdateTimer.GetElapsedSeconds() > m_fTimeDelta)
	{
		MoveByDirection();
		//Time the cool offing
		CoolOff();

		m_PlayerUpdateTimer.Reset();
	}

//	m_MyBody.Add2Force(CVector3f(0.0f,-(m_MyBody.GetMass() * 400),0.0f));

	RepositionBarrel();

}

void CPlayer::RotateCannon( float angle)
{
	//Update the angle.
	//m_Barrell->m_MyBody.vOrientation.y += angle;
}

void CPlayer::RotateCannonLeft()
{
	if (!m_Barrell)
		return;

		//Rotate along the y to rotate the barell.
		m_Barrell->RotateY(-m_Barrell->m_RotationAngle);

}

void CPlayer::RotateCannonRight45()
{
	if (!m_Barrell)
		return;

		//We really shouldent rotate to much should we
		m_Barrell->RotateY(DegreesToRadians(45.0f));
}

void CPlayer::RotateCannonLeft45()
{
	if (!m_Barrell)
		return;
	
		//Rotate along the y to rotate the barell.
		m_Barrell->RotateY(DegreesToRadians(-45.0f));
}

void CPlayer::RotateCannonRight()
{
	if (!m_Barrell)
		return;

	//We really shouldent rotate to much should we
	m_Barrell->RotateY(m_Barrell->m_RotationAngle);

}
void CPlayer::AddHeat(float heat)
{
	//Cant really gain heat if we dont have a barrelll do we?
	//if(!m_PreviousBarrell)
	//	return;

	m_KE += heat;
	m_HeatMeter->Add(heat);
	if (m_KE >= m_MaxKE)
	{
		//Make sure we are at 100.
		m_KE = m_MaxKE;
		m_HeatMeter->Fill();
		OverHeat();
	}
}

void CPlayer::CoolOff()
{
	m_KE -= m_CoolDownRate;
	m_HeatMeter->Subtract(m_CoolDownRate);
	if (m_KE <= 0.0f)
		m_KE = 0.0f;
}