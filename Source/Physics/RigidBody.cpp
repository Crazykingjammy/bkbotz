#include "RigidBody.h"

CRigidBody::CRigidBody()
{
	centerPosition.MakeZero();
	velocityVec.MakeZero();
	velocityVecBody.MakeZero();
	acceleration.MakeZero();
	angularAcceleration.MakeZero();
	moments.MakeZero();
	forceVec.MakeZero();
	gravityForce			= CVector3f(0.0f, -9.8f, 0.0f);
	m_Mass                  = 10.0f;
	m_InvMass				= 1/m_Mass;
	radius				 = 1.0f;
	rigidBodyType		 = BASE_BODY;
	isEffectedByGravity  = true;
	isEffectedByDrag	 = true;
	isMovableByCollision = true;
	isNapping			 = false;
	vOrientation.z		 = 1.0f;
	elasticity			 = 0.5f;
	dragCoefficient      = 0.0f;
	strcpy(myName, "DefaultBodyName");
}

void CRigidBody::Add2Force(CVector3f force)
{
	this->forceVec += force;
}

void CRigidBody::Integrate(Scalar deltaTime)
{
	// Newtons second law
	// F = ma;
	// we have force and mass find acceleration
	acceleration = forceVec * m_InvMass;

	// using the improved Euler method we can integrate 
	// with respect to time.  this retains the 3rd term in the taylor 
	// series.  supposedly not the most stable but we'll use it for now
	// here's the differential equations:
	// v(t) = v(t-1) + a*dt
	velocityVec += acceleration * deltaTime;

	// p(t) = p(t-1) + v(t)*dt + a * dt * dt / 2;
	//centerPosition += (velocityVec * deltaTime) + (acceleration * deltaTime * deltaTime / 2);
	centerPosition += velocityVec * deltaTime;
}

void CRigidBody::ComputeForces()
{
	// have body effected by gravity if need be
	if (isEffectedByGravity)
	{
		CVector3f gravforce = gravityForce * m_Mass;
		Add2Force(gravforce);
	}
	if (isEffectedByDrag)
	{
		// compute for drag coefficiantcy (hehe if thats a word)
		CVector3f dragForce = -velocityVec;
		dragForce.Normalize();

		// 18008350618 ab lounge

		float mag = velocityVec.Magnitude();
		//* dragCoefficient;
		Add2Force((dragForce * (mag))* dragCoefficient);
	}
}

void CRigidBody::Update(Scalar deltaTime)
{
	ComputeForces();
	Integrate(deltaTime);
}

void CRigidBody::ZeroLinearForces()
{
	forceVec.MakeZero();
	velocityVec.MakeZero();
}

void CRigidBody::SetMass(Scalar massKg)
{
	m_Mass = massKg;
	m_InvMass = 1/m_Mass;
}

void CRigidBody::PushTowardABody(CRigidBody *body, float speed)
{
	// find the direction
	CVector3f newForceDir = body->centerPosition - centerPosition;
	newForceDir.Normalize();
	Add2Force(newForceDir*speed);
}
void CRigidBody::PushTowardAPosi(CVector3f pos, float speed)
{
	// find the direction
	CVector3f newForceDir = pos - centerPosition;
	newForceDir.Normalize();
	Add2Force(newForceDir* speed);
}