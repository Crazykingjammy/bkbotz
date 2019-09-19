#ifndef __RIGIDBODY_H_
#define __RIGIDBODY_H_

#include "Math.h"
#include "string.h"
//////////////////////////////////////////////////////////////////////////
// CRigidBody class defines an Abstract base class for any derived
//		rigid bodies.  contains the basics members and methods defined for
//		every different kind of body.  any derived bodies would implement
//		additional members and methods to further describe that certain body
//////////////////////////////////////////////////////////////////////////
enum {BASE_BODY = 0, SPHERE_BODY, BOX_BODY, MAX_BODIES};

#define NUMPOINTS 8
#define MAX_CHAR 128

class CRigidBody
{
private:
	Scalar    m_Mass;						// mass of the body
	Scalar	  m_InvMass;
public:
	
	// used for Linear equations
	CVector3f centerPosition;			// the center point of the body
	CVector3f velocityVec;				// velocity vector on earth
	CVector3f velocityVecBody;			// velocity in body coordinates
	CVector3f forceVec;					// a force vec applied to the body
	//////////////////////////////////////////////////////////////////////////
	
	// angular equations 
	CVector3f acceleration;				// the acceleration of the body
	CVector3f angularAcceleration;		// what the acceleration of the rotation
	
	CVector3f moments;					// this is the amount of torque acted on the body
	
	CVector3f EulerAngles;				// the euler Angle in body coords.

	CMatrix3x3 inertia;					// the mass moment of inertia on earth coords, used for resistance to rotation
	CMatrix3x3 inverseInertia;			// the inverse of the inertia Matrix
	CMatrix3x3 inverseInertiaBody;		// the inverse of the moment of inertia in Body coords.  
	
	CQuaternion orientation;			// the orientation of the body in earth
	CVector3f   vOrientation;			// Orientation for rendering.

	CVector3f anglularVelocityGlobal;	     // angular velocity in terms of earth fixed coords
	CVector3f angularAccelerationGlobal;	 // angular acceleration in terms of earth fixed coords
	//////////////////////////////////////////////////////////////////////////
	
	// additional body properties
	CVector3f hardPoints[NUMPOINTS];	// the hardpoints of the body used for collision
	CVector3f gravityForce;				// the force and gravity direction

	float	  elasticity;				// the elasticity of the body, when collision occurs
	float	  dragCoefficient;			// the drag coefficient for the body as it moves through the air.

	int		  rigidBodyType;			// a type of rigid body : defined by the enumeraton


	Scalar	  radius;					// the bounding radius

	bool	  isEffectedByGravity;		// is this body effected by gravity?
	bool	  isEffectedByDrag;			// is this body effected by linear drag?
	bool	  isMovableByCollision;		// flag stating if the body moves on collisions
	bool	  isNapping;				// is the body in a napping state.  this means that any type of physic will be applied to the body
	//////////////////////////////////////////////////////////////////////////
	
	//////////////////////////////////////////////////////////////////////////
	// An ID for this body
	char	  myName[MAX_CHAR];
public:
	CRigidBody(void);

	virtual ~CRigidBody(void)
 	{
	}

	CRigidBody(const CRigidBody& body)
	{
		centerPosition		 = body.centerPosition;
		velocityVec			 = body.velocityVec;
		forceVec			 = body.forceVec;
		acceleration		 = body.acceleration;
		velocityVecBody		 = body.velocityVecBody;
		isNapping			 = body.isNapping;
		m_Mass			     = body.m_Mass;
		m_InvMass			 = body.m_InvMass;
	}

	CRigidBody & operator= (const CRigidBody& body)
	{
		centerPosition = body.centerPosition;
		velocityVec			 = body.velocityVec;
		forceVec			 = body.forceVec;
		acceleration		 = body.acceleration;
		velocityVecBody		 = body.velocityVecBody;
		isNapping			 = body.isNapping;
		m_Mass			     = body.m_Mass;
		m_InvMass			 = body.m_InvMass;

		return *this;
	}

	virtual void Update(Scalar deltaTime);
	virtual void ComputeForces();
	virtual void ZeroLinearForces();
	virtual void Add2Force(CVector3f force);
	virtual void Integrate(Scalar deltaTime);
	virtual void SetMass(Scalar massKg);
	virtual Scalar GetMass(){	return m_Mass; }
	virtual void PushTowardABody(CRigidBody *body, float speed);
    virtual void PushTowardAPosi(CVector3f pos, float speed);
};







#endif