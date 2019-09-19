#include "trail.h"


void CTrail::CreateParticles()
{
	//Allocate the particles;
	m_Particles = new ParticleObject[m_NumberParticles];

	//Yeah... give them timers .
	//m_ParticleTimers = new CPhysicsTimer[m_NumberParticles];

	for(int i = 0; i < m_NumberParticles; i++)
	{
		m_Particles[i].active = false;
		m_Particles[i].velocity = 0.0f;
		m_Particles[i].position = 0.0f;
		m_Particles[i].color.r = 1.0f;
		m_Particles[i].color.g = 1.0f;
	}

}
void CTrail::Simulate()
{
//	if(m_Time.GetElapsedSeconds() >= 0.005f)
//	{
		UpdateParticles();
//		m_Time.Reset();
//	}
}


void CTrail::Shutdown()
{
	//Clean up that array.
	delete [] m_Particles;
	//delete [] m_ParticleTimers;
}

void CTrail::Kill()
{
	m_Active = false;

	//Kill all the particles.
	for(int i = 0; i < m_NumberParticles; i++)
	{
		m_Particles[i].velocity.MakeZero();
		m_Particles[i].position.MakeZero();
		m_Particles[i].color = 0.0f;
		m_Particles[i].color.r = 255.0f;
		//m_Particles[i].color.g = 1.0f;

	}
}

void CTrail::Trigger()
{
	m_Active = true;

	//Start life counting.
	m_LifeTimer.Reset();

	//Start emit time;
	m_EmitTime.Reset();
	m_CurrentParticle = 0;
}


void CTrail::UpdateParticles()
{
	//Update positions accoring to velocity.

	if(m_EmitTime.GetElapsedSeconds() > m_TrailTailFactor)
	{
		//Set the position to the emiter position.
		m_Particles[m_CurrentParticle].position.y = m_MyBody.centerPosition.y;

		//I dont know why.
		m_Particles[m_CurrentParticle].position.x = -m_MyBody.centerPosition.x;
		m_Particles[m_CurrentParticle].position.z = -m_MyBody.centerPosition.z;

		m_CurrentParticle++;

		m_EmitTime.Reset();

		//If we hit the max go back to the begning.
		if(m_CurrentParticle > m_NumberParticles)
			m_CurrentParticle = 0;
	}

	/*for(int i = 0; i < m_NumberParticles; i++)
	{
		if(m_ParticleTimers->GetElapsedSeconds())
	}*/

}