#ifndef __Target_h__
#define __Target_h__

#include "../Object.h"
#include "../2d/Bar/Bar.h"

class CTarget : public CBaseObject
{
public:

	//Members

	CMeter*	m_LifeMeter;

	float m_Life;
	float m_MaxLife;
	bool m_Active;
	int m_TeamID;



public:

	CTarget()
	{
		m_MaxLife = 100.0f;
		m_Life = m_MaxLife;
		m_Active = true;

		//Default Life Bar
		m_LifeMeter = new CMeter("textures/health2.bmp");
		m_LifeMeter->m_MaxCapacity = m_MaxLife;

	}

	//Methods
	bool Hit(float points = 1.0f)
	{
		//Take up some life.
		m_Life -= points;
		m_LifeMeter->Add(-points);

		//If we are dead or not.
		if(m_Life < 0.0f)
		{
			//m_Active = false;
			//m_Visible = false;
			m_Life = 0.0f;
			m_LifeMeter->Empty();

			return false;
		}
		else
		{
			return true;
		}
	}

	//Yes, Another Fucking Update;

	void Activate()
	{
		m_Active = true;
		m_Visible = true;
		m_Life = 5;
	}

	void PushBack()
	{
		//Update the forces
		m_MyBody.Add2Force(m_MyBody.vOrientation * m_MyBody.GetMass() * 10.0f);
		m_MyBody.Add2Force(CVector3f(0.0f,-1.0f,0.0f) * m_MyBody.GetMass() );
	}

};

#endif