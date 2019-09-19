#ifndef __PHYSICS_SCENE_H_
#define __PHYSICS_SCENE_H_



#include "PhysicsTimer.h"
#include "RigidBody.h"
#include "PhysicsWorld.h"

#define NUMPLANES 6

struct tPlane
{
	CVector3f point;
	CVector3f normal;
};

struct tWorld
{
	tPlane planes[NUMPLANES];
	//float width, height, depth;
	//vector < CTriangle tri > triangles;
	float groundFriction;
};

#define	COEFFICIENTOFRESTITUTION	0.5f
#define COLLISIONTOLLERENCE			2.0f

struct tCollisionData
{
	// store the impacting normal of the collision
	CVector3f collisionNormal;		// will also be used to define the normal of the plane when collided with the world
	CVector3f relativeVelocity;		// of the two bodies

	CRigidBody *body1;				// pointers to the body
	CRigidBody *body2;

	float		  distPoint;		// the distance point used to move the body when colliding with a plane
	int body1Inx, body2Inx;			// why not, store thier index with in this scene
	int collType;					// the time of the collision be it out of the enum declared in the scene
	tCollisionData()
	{
		body1 = NULL; body2 = NULL;
		body1Inx = -1; body2Inx = -1;
		collType = 0;
		distPoint = 0.0f;
	}

};

class CPhysicsScene
{
public:
	enum {NOCOLLISON = 0, COLLISION, PENETRATING, WORLDCOLLISION, NUM_OF_COLLISIONS_TYPES };

	CVector3f X_AXIS;
	CVector3f Y_AXIS;
	CVector3f Z_AXIS;

private:
	
	// the list of all the bodies to simulate
	vector <CRigidBody *>	listOfBodies;
	vector <tCollisionData> listOfCollision;
	vector <tCollisionData> listOfWallCollisions;
	
	// the number of current bodies. used to return the index
	int			numOfBodies;
	//// a pointer to a world.  struct defined above
	//tWorld		theWorld;

	CPhysicsWorld theWorld;

	CPhysicsTimer physicTimer;

	float		  UpdatePerSec;

	float		currTime, lastTime;
	float		deltaTime;

	bool		  is3D;
	bool		  isDoPenetration;

public:
	CPhysicsScene(int size = 20);
	CVector3f	sceneGravity;

	~CPhysicsScene();
	CPhysicsScene(const CPhysicsScene& phScene);
	
	//static CPhysicsScene *GetInstance();
	//void DeleteInstance();

	int	 AddBody(CRigidBody *body);
	int  AddWallToWorld(CVector3f point, CVector3f normal, bool doResponse, char *wallName = NULL, float sFriction = 1.0f);
	bool RemoveBody(int index);
	bool ClearScene();
	CRigidBody * GetBody(int index);
	int			 GetBody(CRigidBody *body);

	int GetSize();


	void		 Simulate(float simSpeed = 1.0f);
	void		 EndSimulate();

	void		 SceneCollisionDetect(int body1Indx);
	void		 SceneCollisionResponse();
	void		 HandlePenetration(CRigidBody *body1, CRigidBody *body2, tCollisionData *collData);

	void		 SetPenetrationFlag(bool isHandlingpentration) { isDoPenetration = isHandlingpentration;}

	void		 CreateWorld(float width, float height, float length);

	void		 SetWorld(tWorld *world);
	void		 SetDefaultWorld();

	void		 SetGravity(CVector3f g);

	bool		 BindBodyToWorld(int bodyIndex);

	void		 SleepABody(int index);
	void		 WakeUpABody(int index);
	void		 SleepAllBodies();
	void		 WakeUpAllBodies();

	// these are all in relation to world coords
	void		 PushABodyUp(int index, float magnitude);
	void		 PushABodyDown(int index, float magnitude);
	void		 PushABodyLeft(int index, float magnitude);
	void		 PushABodyRight(int index, float magnitude);
	void		 PushABodyForward(int index, float magnitude);
	void		 PushABodyBackward(int index, float magnitude);
	void		 PushABodyAnyDirectAndMag(int index, CVector3f dir);
	// Accessors
	CVector3f	 GetPositionOfABody(int index);
	CVector3f	 GetVelocityOfABody(int index);
	CVector3f	 GetAngularVelOfBody(int index);
	CVector3f	 GetForceOfABody(int index);
	CQuaternion  GetOrientationQ(int index);
	CMatrix3x3	 GetOrientationM(int index);

	// modifiers
	void		 SetPositionOfABody(int index, CVector3f pos);
	void		 SetVelocityOfABody(int index, CVector3f vel);
	void		 SetForceOfABody(int index, CVector3f force);

	bool		 HandleSpecialPlaneCollision(int bodyIndex, int planeIndex);

	//////////////////////////////////////////////////////////////////////////
	// added functionality to check if two bodies have collided
	//		this traverse through the collison list
	bool		 HaveTheTwoBodiesCollided(int b1Indx, int b2Indx);
	bool		 HasTheBodyCollidedWithWall(int bodyIndx, int wallIndx);
	bool	     HasTheBodyCollidedWithWall(int bodyIndx);
	// this returns true if the collision list is empty
	bool		 isCollisionListEmpty();
	// returns the size of the collision list
	int			 GetSizeOfCollisionList(){ return (int)listOfCollision.size();}

	//// this is used out side of the scene, if you need to do a quick collision check against two bodies
	//// no need for the extra params
	//int			 TwoBodyCollision(CRigidBody *body1, CRigidBody *body2);
	bool		 CollSphereToRay(CVector3f spherePos, float sphereRadius, CVector3f rayPoint, CVector3f rayNormal);

private:
	void		 ConstrainForces(CRigidBody *body);

	// collision Functions
	// tests collisions against two bodys in this scene and return true if successfull and returns the 
	//		relative velocity vector and the collision normal so that it can be inserted into the collision list
	int			 TwoBodyCollision(CRigidBody *body1, CRigidBody *body2, CVector3f &relV, CVector3f &collN);
	void		 TwoBodyResponse(CRigidBody *body1, CRigidBody *body2, CVector3f &relV, CVector3f &collN);
	//////////////////////////////////////////////////////////////////////////

	int			 CollSphereToPlane(CVector3f spherePos, float radius, CVector3f planePoint, CVector3f planeNormal, float *distPt);
	void		 CollResponseSphereToPlane(CRigidBody *body, CVector3f planeNormal, float distPt);

	bool		 CollPointToTriangle(CTriangle triangle, CVector3f testPoint);
	bool		 CollSphereToTriangle(CTriangle triangle, CVector3f spherePos, float radius, CVector3f *newSpherePos);
	bool		 CollLineSegToTriangle(CTriangle triangle, CVector3f pStart, CVector3f pEnd);

	//////////////////////////////////////////////////////////////////////////
	// check to see if we are calculating the same collision
	bool		CheckForDupCollisions(tCollisionData *collData);



};

#endif