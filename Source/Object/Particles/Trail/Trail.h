#ifndef __Trail_h__
#define __Trail_h__

#include "../Particle.h"


class CTrail : public CBaseParticleEmitter
{
	//Members

private:


public:

	CVector3f m_Gravity;
	CVector3f m_ExplosionForce;
	CPhysicsTimer m_LifeTimer;

	CPhysicsTimer m_EmitTime;
	int m_CurrentParticle;

	//Ground 
	float m_Ground;

	//Trail Length
	float m_TrailTailFactor;

	//Friction Factor
	float m_FrictionFactor;

	//Hopefuly this wont hit me to hard.
	CPhysicsTimer *m_ParticleTimers;

	//Methods
public:

	CTrail(int numParticles,char* bitmap)
	{
		////
		///	  Dontuse this really.........
		////	
		//////////////////////////////////////////////////////////////////////////
		

		// Get the texture file.
		strcpy(m_ModelName,bitmap);

		m_NumberParticles = numParticles;
		m_TimeStep = 0.05f;

		m_Gravity.y = -5.0f;

		m_ExplosionForce.x = 5.0f;
		m_ExplosionForce.y = 15.0f;
		m_ExplosionForce.z = -5.0f;
		
		
		m_ParticleSize = 1.0f;

		m_Ground = 0.0f;
		m_FrictionFactor = 0.3f;
		m_CurrentParticle = 0;
		m_TrailTailFactor = 0.001f;
		m_World = false;
		
	}

	CTrail()
	{
		// Get the texture file.
		strcpy(m_ModelName,"NoName.bmp");

		m_NumberParticles = 100;

		m_TimeStep = 0.01f;

		m_Gravity.y = 0.005f;

		//m_ExplosionForce.x = 5.0f;
		//m_ExplosionForce.y = 15.0f;
		//m_ExplosionForce.z = -5.0f;

		m_Ground = 0.0f;
		m_FrictionFactor = 0.8f;
		m_ParticleSize = 1.0f;

		m_Active = true;
		m_CurrentParticle = 0;
		m_TrailTailFactor = 0.5f;
		m_World	= true;
	}
	//Virtual functions taht need to be filled up.
	void CreateParticles();
	void Simulate();
	void Shutdown();
	void Trigger();
	void Kill();

	//Set some stuff
	void SetNumParticles(int count) {m_NumberParticles = count;}

	


private:
	//Put our update code in here for simulate to call
	void UpdateParticles();

};


#endif