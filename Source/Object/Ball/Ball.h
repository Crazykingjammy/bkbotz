#ifndef __Ball_h__
#define __Ball_h__


#include "../Object.h"
#include "../../Physics/PhysicsTimer.h"



class CBall : public CBaseObject
{
public:

	//Members.
	int  m_PlayerID;
	bool m_Dangerous;
	bool m_PickupAble;

	//Damage stuff
	float m_DamageAmount;

	//How long are we active for
	float m_ActiveLength;

	//Activation Timer.
	CPhysicsTimer m_ActiveTime;


public:

	//Methods.
	CBall()
	{
		m_PlayerID = -1;
		m_Dangerous = false;
		m_PickupAble = true;
		m_DamageAmount = 5.0f;
		m_ActiveLength = 5.0f;
	}
	
	//Rest
	void Rest();

	//Activate
	void Activate();


};





#endif