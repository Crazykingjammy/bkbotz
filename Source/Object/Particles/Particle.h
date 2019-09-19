#ifndef __Particle_h__
#define __Particle_h__

#include "../Object.h"
#include "../../Physics/PhysicsTimer.h"


struct ParticleObject
{
	EXColor			color;
	CVector3f		position,velocity;
	float			life;
	bool			active;
};

class CBaseParticleEmitter : public CBaseObject
{
	//Members

protected:
	//That we cant toucn my friend.
		int m_NumberParticles;

public:

	ParticleObject*	m_Particles;
	CPhysicsTimer	m_Time;

	float m_ParticleLife;
	float m_TimeStep;
	float m_ParticleSize;

	bool m_Active, m_World;

	//Methods

public:

	CBaseParticleEmitter()
	{
		//strcpy(m_ModelName,"NoFileNameBitches");
		//m_NumberParticles = 100;


	}

	//Initalize the particle system.
	virtual void CreateParticles()		= 0;

	//Update the particle system
	virtual void Simulate()		= 0;

	//Cleanup anything we need bitches.
	virtual void Shutdown()		= 0;

	//Start particles
	virtual void Trigger()		= 0;

	//Reset particles
	virtual void Kill()			= 0;

	//We do need this.
	int GetNumberParticles() {return m_NumberParticles;}

	

};



#endif