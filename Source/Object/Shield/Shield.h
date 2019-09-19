#ifndef __Shield_h__
#define __Shield_h__


#include "../object.h"

class CShield : public CBaseObject
{
public:
	//Members
	float m_ReflectingForce;
	float m_ReflectingDamage;
	float m_MaxRadius;
	float m_ShrinkRate, m_ShrinkHit;



public:
	//Methods
	CShield(char* modelname)
	{
		strcpy(m_ModelName,modelname);
		m_ReflectingForce = 26000.0f;
		m_ReflectingDamage = 10.0f;

		//Shield shouldent move from collision.
		m_MyBody.isMovableByCollision = false;
		m_MyBody.isNapping = true;
		m_MyBody.SetMass(50.0f);

	}

	CShield()
	{
		strcpy(m_ModelName,"Models/DefaultShield.x");
		m_ReflectingForce = 26000.0f;
		m_ReflectingDamage = 10.0f;

		//Shield shouldent move from collision.
		m_MyBody.isMovableByCollision = false;
		m_MyBody.isNapping = true;
		m_MyBody.SetMass(50.0f);
	}

};



#endif
