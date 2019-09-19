#include "Barrell.h"


CBarrell::CBarrell()
{
	strcpy(m_ModelName,"Models/DefualtCannon.x");
	m_SpeedModifier = 1.0f;
	// the speed that the barrel will rotate
	m_RotationSpeed = 0.005f;
	m_MaxRotation = 0.8295f;
	m_MyBody.vOrientation = 0.0f;
	// the increment angle for the rotation
	m_RotationAngle = 0.05f;
	//Looking Straight Forward
	m_MyBody.vOrientation.z = 1.0f;
	m_NumBalls = 0;
	m_MaxBalls = 10;
	m_DamageAmount = 5.0f;

	// firing power for shooting
	m_RegPower = 50000.0f;
	m_ChargeRate = 10000.0f;
	m_CurrentPower = 0.0f;
	m_MaxPower = 70000.0f;
	m_ChargeFreq = 0.005f;

	// the firing rate for the player
	m_FireRate = 0.25f;

	m_BallMass = 1.0f;

	//Yes this will make the ball draw the world for each ball..... yeah.. fuck off.!
	m_BallModelIndex = 0;

	m_bReachedMax	 = false;

	m_ChargeTimer.Reset();

//	m_PickUpTimer.Reset();
	m_LengthTilPickUp = 5.0f;
	m_bPickUpAble = true;

	//Shield stuff
	strcpy(m_OverHeatShield.m_ModelName,"models/notready.x");
	m_OverHeatShield.m_Visible = false;
}
CBarrell::CBarrell(char *modelname)
{
	strcpy(m_ModelName,modelname);
	m_SpeedModifier = 1.0f;
	// the speed that the barrel will rotate
	m_RotationSpeed = 0.005f;
	m_MaxRotation = 0.8295f;
	m_MyBody.vOrientation = 0.0f;
	// the increment angle for the rotation
	m_RotationAngle = 0.05f;
	//Looking Straight Forward
	m_MyBody.vOrientation.z = 1.0f;
	m_NumBalls = 0;
	m_MaxBalls = 10;
	m_DamageAmount = 5.0f;

	// firing power for shooting
	m_RegPower = 50000.0f;
	m_ChargeRate = 10000.0f;
	m_CurrentPower = 0.0f;
	m_MaxPower = 70000.0f;
	m_ChargeFreq = 0.005f;

	m_BallMass = 1.0f;

	// the firing rate for the player
	m_FireRate = 0.25f;

	//Yes this will make the ball draw the world for each ball..... yeah.. fuck off.!
	m_BallModelIndex = 0;
	m_bReachedMax	 = false;
	m_ChargeTimer.Reset();
//	m_PickUpTimer.Reset();
	m_LengthTilPickUp = 5.0f;
	m_bPickUpAble = true;

	//Hide it of course.
	strcpy(m_OverHeatShield.m_ModelName,"models/notready.x");
	m_OverHeatShield.m_Visible = false;
}
CBarrell::~CBarrell(){}

bool CBarrell::Shoot(bool &bFullCharge)
{
	if(m_BallStock.empty())
		return false;

	if(m_ShootRateTimer.GetElapsedSeconds() < m_FireRate)
		return false;

	//Reset the timer.
	m_ShootRateTimer.Reset();

	CBall *ballToShoot = m_BallStock.front();
	//Attach the ball to the center of the barell.
	ballToShoot->m_MyBody.centerPosition = m_MyBody.centerPosition;

	//Set the ball index.
	ballToShoot->ModelIndex = m_BallModelIndex;     //Hehehwehhehehehehehhehehehehehehehe

	//Activate the ball
	ballToShoot->m_Dangerous = true;
	ballToShoot->m_Visible = true;
	ballToShoot->m_MyBody.isNapping = false;
	ballToShoot->m_ActiveTime.Reset();
	ballToShoot->m_MyBody.SetMass(m_BallMass);
	ballToShoot->m_MyBody.radius	= m_BallRadius;
	ballToShoot->m_DamageAmount		= m_DamageAmount;

	//Orentation shit.
	ballToShoot->m_MyBody.vOrientation	= m_MyBody.vOrientation;
	ballToShoot->m_RenderingOrientation = m_RenderingOrientation;

	// forget changing velocity Add force to the ball
	ballToShoot->m_MyBody.ZeroLinearForces();
	CVector3f newForce;

	//Calculate the force.
	newForce = ((m_MyBody.vOrientation * m_CurrentPower));

	//Apply that force
 	ballToShoot->m_MyBody.Add2Force(newForce);
	
	//Release ball from inventory.
    m_BallStock.pop_front();

	//Decerement
	m_NumBalls--;

	m_CurrentPower = m_RegPower;

	bFullCharge = m_bReachedMax;
	
	m_bReachedMax = false;

	//Everything is good return true.
	return true;
}

bool CBarrell::Charge()
{
	//If we are empty return false.
	bool charging = false;
	if(m_BallStock.empty())
		return charging;

	if(m_ChargeTimer.GetElapsedSeconds() > m_ChargeFreq)
	{
		m_CurrentPower += m_ChargeRate;
		//Test against these forces.
		if(m_CurrentPower >= m_MaxPower)
		{
			m_CurrentPower = m_MaxPower;
			m_bReachedMax = true;
		}
		//Reset the timer;
		m_ChargeTimer.Reset();
		charging = true;
	}
	return charging;
}

bool CBarrell::PopOff()
{
	CVector3f newForce;
	for(int i = 0; i < m_NumBalls; i++)
	{
		CBall* CurBall = m_BallStock.front();

		CurBall->m_MyBody.centerPosition = m_MyBody.centerPosition;

		//Activate the ball
		CurBall->m_Dangerous = false;
		CurBall->m_Visible = true;
		CurBall->m_MyBody.isNapping = false;
		CurBall->m_MyBody.ZeroLinearForces();
		CurBall->m_PlayerID = -1;
		newForce.MakeRandom(5000, -5000, 5000, 0, 5000, -5000);
		CurBall->m_MyBody.Add2Force(newForce);

		m_BallStock.pop_front();
	}

	//Jus to make sure.. even though i think this line wil fuck shit up... o well .. happy debugging.
	m_BallStock.clear();

	//We no longer have a cannon.
	m_MyBody.ZeroLinearForces();

	//Blow away the barell.
	float insanity = 500000.0f;

	newForce.MakeRandom(insanity, -insanity, insanity, 
							0.0f, insanity, -insanity);
	//newForce *= insanity;
	m_MyBody.Add2Force(newForce);
	m_MyBody.isNapping = false;
	m_NumBalls = 0;
	m_CurrentPower = 0.0f;
	
	m_bPickUpAble = false;
	m_PickUpTimer.Reset();


	return true;
}

bool CBarrell::CanPickUp()
{
	if (m_bPickUpAble)
		return true;
	if (m_PickUpTimer.GetElapsedSeconds() > m_LengthTilPickUp)
	{	
		m_bPickUpAble = true;	
		return true;
	}

	return false;
}














