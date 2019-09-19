#include "Explosions.h"


void CExplosion::CreateParticles()
{
	//Allocate the particles;
	m_Particles = new ParticleObject[m_NumberParticles];

	for(int i = 0; i < m_NumberParticles; i++)
	{
		m_Particles[i].active = false;
		m_Particles[i].velocity = 0.0f;
		m_Particles[i].position = 0.0f;
		m_Particles[i].color.r = 1.0f;
		m_Particles[i].color.g = 1.0f;
	}

}
void CExplosion::Simulate()
{
//	if(m_Time.GetElapsedSeconds() >= 0.005f)
//	{
		UpdateParticles();
//		m_Time.Reset();
//	}
}


void CExplosion::Shutdown()
{
	//Clean up that array.
	delete [] m_Particles;
}

void CExplosion::Kill()
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

void CExplosion::Trigger()
{
	m_Active = true;

	//Start life counting.
	m_LifeTimer.Reset();

	CVector3f dir;

	for(int i = 0; i < m_NumberParticles; i++)
	{


		//Randomize explosion force
		m_ExplosionForce.MakeRandom(0.093f,-0.093f,0.196f,0.0f,0.74f,-0.0f);

		//dir = m_MyBody.vOrientation;
		//dir.Normalize();
		//m_ExplosionForce += dir;


		//Set the velocity.
		m_Particles[i].velocity.x += m_ExplosionForce.x;
		m_Particles[i].velocity.y += m_ExplosionForce.y;
		m_Particles[i].velocity.z += m_ExplosionForce.z;
	}
}


void CExplosion::UpdateParticles()
{
	//Update positions accoring to velocity.
	for(int i = 0; i < m_NumberParticles; i++)
	{
		m_Particles[i].position.x += m_Particles[i].velocity.x;
		m_Particles[i].position.y += m_Particles[i].velocity.y;
		m_Particles[i].position.z += m_Particles[i].velocity.z;
	
	
		//Gravity
		m_Particles[i].velocity.x -= m_Gravity.x;
		m_Particles[i].velocity.y -= m_Gravity.y;
		m_Particles[i].velocity.z -= m_Gravity.z;

		////If we hit the ground.
		//if(m_Particles[i].position.y < m_Ground)
		//{
		//	//Reflection.
		//	m_Particles[i].velocity.y *= -1.0f;

		//	//Friction.
		//	m_Particles[i].velocity *= m_FrictionFactor;
		//}

		//m_Particles[i].color.r += 0.0000072f;
	//m_Particles[i].color.g += 0.000002f;
		//m_Particles[i].color.b += 0.000002f;
	
	}


	if(m_LifeTimer.GetElapsedSeconds() > 1.0f)
	{
		Kill();
	}

}