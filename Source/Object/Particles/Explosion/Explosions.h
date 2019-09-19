#ifndef __Explosions_h__
#define __Explosions_h__

#include "../Particle.h"


class CExplosion : public CBaseParticleEmitter
{
	//Members

private:


public:

	CVector3f m_Gravity;
	CVector3f m_ExplosionForce;

	CPhysicsTimer m_LifeTimer;

	//Ground 
	float m_Ground;

	//Friction Factor
	float m_FrictionFactor;

	//Methods
public:

	CExplosion(int numParticles,char* bitmap)
	{
		// Get the texture file.
		strcpy(m_ModelName,bitmap);

		m_NumberParticles = numParticles;
		m_TimeStep = 0.05f;

		m_Gravity.y = -5.0f;

		m_ExplosionForce.x = 5.0f;
		m_ExplosionForce.y = 15.0f;
		m_ExplosionForce.z = -5.0f;
		
		
		m_ParticleSize = 1.0f;

		m_World = false;
		m_Ground = 0.0f;
		m_FrictionFactor = 0.3f;
		
	}

	CExplosion()
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

		m_World = false;
		m_Active = false;
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