#ifndef __Barrell_h__
#define __Barrell_h__

#include "../Object.h"
#include "../Ball/Ball.h"
#include "../../Physics/PhysicsTimer.h"
#include "../Shield/Shield.h"

class CBarrell : public CBaseObject
{
public:

	//List of balls, not a sack of balls.
	list<CBall*> m_BallStock;

	//Timer
	CPhysicsTimer m_ChargeTimer;
	CPhysicsTimer m_PickUpTimer;	// we want to have a certain time to pick up our loose cannon
	CPhysicsTimer m_ShootRateTimer; //we can only shoot so fast can we.

	CShield		m_OverHeatShield;

	float m_ChargeFreq;				// yes i am HAHAHHHAhahaehaehaehahah... ehem.
	float m_LengthTilPickUp;		// this is the amount of time needed before we can pick the barrell up again

	//Members
	float m_SpeedModifier;
	float m_RotationSpeed;
	float m_MaxRotation;
	float m_RotationAngle;
	float m_DamageAmount;
	float m_ShootingHeat, m_MaxChargeheat;
	float m_RegPower, m_MaxPower, m_CurrentPower, m_ChargeRate, m_FireRate;
	float m_BallMass, m_BallRadius;

	int m_NumBalls, m_MaxBalls, m_BallModelIndex;
	
	bool m_bReachedMax;
	bool m_bPickUpAble;				// can we pick up this barrell

public:

	CBarrell();
	CBarrell(char *modelname);
	~CBarrell();

	bool Shoot(bool &bFullCharge);
	bool Charge(void);
	bool PopOff(void);
	bool CanPickUp(void);
};



#endif
