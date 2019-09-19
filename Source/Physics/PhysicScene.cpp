#include "PhysicScene.h"
#include <time.h>


CPhysicsScene::CPhysicsScene(int size)
{
	if (size < 0)
		size = 0;

	listOfBodies.clear();
	listOfBodies.reserve(size);

	listOfWallCollisions.reserve(size);

	listOfCollision.reserve(size*size);

	numOfBodies = 0;
	X_AXIS = CVector3f(1.0f, 0.0f, 0.0f);
	Y_AXIS = CVector3f(0.0f, 1.0f, 0.0f);
	Z_AXIS = CVector3f(0.0f, 0.0f, 1.0f);

	physicTimer.Reset();
	currTime = 0.0f;
	lastTime = 0.0f;
	deltaTime= 0.0f;
	sceneGravity.y = -9.8f;
	UpdatePerSec  = 1.0f/60.0f;				// we want 60 fps for processing physics
	is3D		  = true;
	isDoPenetration = true;					// do we want to handle penetration
}

CPhysicsScene::~CPhysicsScene(void)
{
	ClearScene();
}

CPhysicsScene::CPhysicsScene(const CPhysicsScene& scene)
{
}

void CPhysicsScene::SetDefaultWorld()
{
	theWorld.SetDefaultWorld();

	is3D = true;
}

void CPhysicsScene::CreateWorld(float width, float height, float length)
{
	AddWallToWorld(CVector3f(0.0f, 0.0f, 0.0f), Y_AXIS, true, "Floor", 0.9999f);
	AddWallToWorld(CVector3f(0.0f, height, 0.0f), -Y_AXIS, true, "Ceiling", 0.9999f);
	AddWallToWorld(CVector3f(-width/2.0f, 0.0f, 0.0f), X_AXIS, true, "LeftWall", 0.9999f);
	AddWallToWorld(CVector3f(width/2.0f, 0.0f, 0.0f), -X_AXIS, true, "RightWall", 0.9999f);
	AddWallToWorld(CVector3f(0.0f, 0.0f, length/2.0f), -Z_AXIS, true, "BackWall", 0.9999f);
	AddWallToWorld(CVector3f(0.0f, 0.0f,-length/2.0f), Z_AXIS, true, "FrontWall", 0.9999f);

}

bool CPhysicsScene::BindBodyToWorld(int bodyIndex)
{
	bool isColl = false;

	//CTriangle ground(CVector3f(100.0f, 0.0f, -100.0f), CVector3f(-100.0f, 0.0f, -100.0f), CVector3f(0.0f, 0.0f, 100.0f));
	//ground.n = CVector3f(0.0f, 1.0f, 0.0f);

	//CVector3f newPos;
	//if (CollSphereToTriangle(ground, body->centerPosition, body->radius, &newPos))
	//{
	//	body->centerPosition += newPos;
	//	body->velocityVec.y *= -1;
	//}

	// set up our variable we are going to need

	// the diststance point returned by collSphereToPlane
	float dP;
	CVector3f n;				// store the normal for the plane
	CVector3f normP;			// the normal projection from the plane
	CVector3f vecProj;			// the vector projection from the plane
//    float nP;					// n' used for vector reflection
	CVector3f velF;				// the new velocity for the current body
	tCollisionData wallColl;	// the collision data needed to store if we find a collision
    
	int numplanes = (int)theWorld.m_worldPlanes.size();

	//tWorldPlane *currPlane;
	
	for (int i = 0; i < numplanes; i++)
	{
		if (CollSphereToPlane(listOfBodies[bodyIndex]->centerPosition, listOfBodies[bodyIndex]->radius, theWorld.m_worldPlanes[i].point, theWorld.m_worldPlanes[i].normal, &dP))
		{
			if (theWorld.m_worldPlanes[i].handleCollisionResponse)
			{
				//CollResponseSphereToPlane(body, theWorld.planes[i].normal, dP);
				// find the projected normal to find the new position of the body
				listOfBodies[bodyIndex]->centerPosition += (theWorld.m_worldPlanes[i].normal *(listOfBodies[bodyIndex]->radius - dP));
				/************************************************************************/
				/************************************************************************/
				/* Lets find the vector projection this will be the new vector for the body
				ViP = Vi - ( N(N dot Vi)                                            */
				/************************************************************************/

				// find n'
				// now we have our vec projection calculate the new velocity
				// V' = V - 2(N DOT V)N
				listOfBodies[bodyIndex]->velocityVec = (listOfBodies[bodyIndex]->velocityVec - ((theWorld.m_worldPlanes[i].normal*(theWorld.m_worldPlanes[i].normal.DotProduct(listOfBodies[bodyIndex]->velocityVec)))*2))*theWorld.m_worldPlanes[i].staticCoeFric;//body->velocityVec - ((n*nP)*2);
	
			}
			wallColl.body1Inx = bodyIndex;
			wallColl.body2Inx = theWorld.m_worldPlanes[i].planeIndx;
			wallColl.collisionNormal = theWorld.m_worldPlanes[i].normal;
			listOfWallCollisions.push_back(wallColl);
			isColl = true;
		}
	}
	return isColl;
}

bool CPhysicsScene::HandleSpecialPlaneCollision(int bodyIndex, int planeIndex)
{
	if (bodyIndex < 0 || bodyIndex >= (int)listOfBodies.size())
		return false;
	if (planeIndex < 0 || planeIndex >= (int)theWorld.m_worldPlanes.size())
		return false;
	// okay we're good lets handle this

	CRigidBody *body = NULL;

	tWorldPlane *currPlane;

	float dP = 0.0f;
	bool collided = false;
	body	  = listOfBodies[bodyIndex];
	currPlane = &theWorld.m_worldPlanes[planeIndex];

	if (CollSphereToPlane(body->centerPosition, body->radius, currPlane->point, currPlane->normal, &dP))	
	{
		//CollResponseSphereToPlane(body, theWorld.planes[i].normal, dP);
		// find the projected normal to find the new position of the body
		body->centerPosition += (currPlane->normal *(body->radius - dP));
		/************************************************************************/
		/************************************************************************/
		/* Lets find the vector projection this will be the new vector for the body
			ViP = Vi - ( N(N dot Vi)                                            */
		/************************************************************************/

		// find n'
		// now we have our vec projection calculate the new velocity
		// V' = V - 2(N DOT V)N
		body->velocityVec = (body->velocityVec - ((currPlane->normal*(currPlane->normal.DotProduct(body->velocityVec)))*2))*currPlane->staticCoeFric;//body->velocityVec - ((n*nP)*2);
	
		collided = true;
	}	
	body = NULL;
	return collided;
}

int CPhysicsScene::AddBody(CRigidBody *body)
{
	if (body)
	{
		int index = this->numOfBodies;
		numOfBodies++;
		//body->gravityForce = sceneGravity;
		this->listOfBodies.push_back(body);
		return index;
	}
	return numOfBodies;
}

int  CPhysicsScene::AddWallToWorld(CVector3f point, CVector3f normal, bool doResponse, char *wallName, float sFriction)
{
	return theWorld.AddPlaneToWorld(point, normal, doResponse, wallName, sFriction);
}

bool CPhysicsScene::ClearScene()
{
	if (listOfBodies.empty())
		return false;

	listOfBodies.clear();
	numOfBodies = 0;
	return true;
}

void CPhysicsScene::Simulate(float simSpeed)
{
	lastTime = currTime;
	currTime = physicTimer.GetElapsedSeconds();
	
	int numBodies = (int)listOfBodies.size();
	deltaTime = simSpeed* (currTime - lastTime);
	//deltaTime = physicTimer.GetElapsedSeconds() * simSpeed;
	CRigidBody *currBody = NULL;
	// clear all the collisions from the previous frame
	listOfCollision.clear();
	listOfWallCollisions.clear();
	// delta time 0.0017
	// time to update all the stored bodies in the this scene
	for (int currBodyIndx = 0; currBodyIndx < numBodies; currBodyIndx++)
	{
		//// for clarity
		//currBody = ;
		if (!listOfBodies[currBodyIndx]->isNapping)	// do the update only if the body is not napping
		{
			listOfBodies[currBodyIndx]->Update(deltaTime);
			ConstrainForces(listOfBodies[currBodyIndx]);
			// do collision detection
			SceneCollisionDetect(currBodyIndx);
		}
	}
	// do collision response
	SceneCollisionResponse();
	currBody = NULL;
	//physicTimer.Reset();
}

void CPhysicsScene::EndSimulate()
{

}

void CPhysicsScene::SceneCollisionDetect(int body1Index)
{
	unsigned int numBodies = (unsigned int)listOfBodies.size();

	CRigidBody *Body1, *Body2;

	int checkCollision = NOCOLLISON;
	bool isCollOrPen = false;
	tCollisionData newCollData;

	//////////////////////////////////////////////////////////////////////////
	// needed for collision list
	CVector3f rV;			// returned by twoBodyCollision defines it and we store it in the collison data
	CVector3f cN;			//
	//////////////////////////////////////////////////////////////////////////

	Body1 = listOfBodies[body1Index];

	// keep all the bodies in the world no matter what
	BindBodyToWorld(body1Index);

	int collWallSize = (int)listOfWallCollisions.size();
	for (int collWallIndex = 0; collWallIndex < collWallSize; collWallIndex++)
	{
		tCollisionData collData = listOfWallCollisions[collWallIndex];
	}

	for (unsigned int body2Index = 0; body2Index < numBodies; body2Index++)
	{
		Body2 = listOfBodies[body2Index];
		if (Body1 == Body2 || Body2->isNapping)
			continue;
		// lets do ball to ball collision
		checkCollision = TwoBodyCollision(Body1, Body2, rV, cN); 
		switch(checkCollision)
		{
		case COLLISION:
			newCollData.collType = COLLISION;
			isCollOrPen = true;
			break;
		case PENETRATING:
			newCollData.collType = PENETRATING;
			isCollOrPen = true;
			break;
		case NOCOLLISON:
			isCollOrPen = false;
			break;
		default:
			isCollOrPen = false;
			break;
		};

		if (isCollOrPen)
		{
			newCollData.body1 = Body1;
			newCollData.body1Inx = body1Index;
			newCollData.body2 = Body2;
			newCollData.body2Inx = body2Index;
			newCollData.collisionNormal = cN;
			newCollData.relativeVelocity = rV;
			if (!CheckForDupCollisions(&newCollData))
			{
				listOfCollision.push_back(newCollData);
			}
		}
	}
}

bool CPhysicsScene::CheckForDupCollisions(tCollisionData *collData)
{
	unsigned int numOfColls = (unsigned int)listOfCollision.size();

	for (unsigned int curCollIndx = 0; curCollIndx < numOfColls; ++curCollIndx)
	{
		if ((collData->body1 == listOfCollision[curCollIndx].body1 && 
			collData->body2 == listOfCollision[curCollIndx].body2 || 
			collData->body2 == listOfCollision[curCollIndx].body1 && 
			collData->body1 == listOfCollision[curCollIndx].body2) && 
			collData->collType == listOfCollision[curCollIndx].collType			
			)
		{
			return true;
		}
	}
	return false;
}

void CPhysicsScene::SceneCollisionResponse()
{
	unsigned int numCollisions = (unsigned int)listOfCollision.size();
	if (numCollisions <= 0)
		return;
	unsigned int currCollIndx;

	tCollisionData *curColl;
	for (currCollIndx = 0; currCollIndx < numCollisions; currCollIndx++)
	{
		curColl = &listOfCollision[currCollIndx];
		switch(curColl->collType) 
		{
		case COLLISION:
				TwoBodyResponse(curColl->body1, curColl->body2, curColl->relativeVelocity, curColl->collisionNormal);
			break;
		case PENETRATING:
			
				HandlePenetration(curColl->body1, curColl->body2, curColl);

				if (curColl->collType == COLLISION)
				{
					TwoBodyResponse(curColl->body1, curColl->body2, curColl->relativeVelocity, curColl->collisionNormal);
				}
			break;
		default:
			break;
		};
	}
}

void CPhysicsScene::HandlePenetration(CRigidBody *body1, CRigidBody *body2, tCollisionData *collData)
{
	float dt = UpdatePerSec;
	bool tryAgain = true;
	int check = PENETRATING;
	bool didPen = false;
	int count = 0;

	CRigidBody copyB1, copyB2;
	int size = sizeof(CRigidBody);


	CVector3f relVel, collNorm;

	float fT = _FloatTolerance_;
	while (tryAgain && dt > fT)
	{
		tryAgain = false;
		didPen	 = false;

		memcpy(&copyB1, body1, sizeof(CRigidBody));
		memcpy(&copyB2, body2, sizeof(CRigidBody));

		copyB1.Update(dt);
		copyB2.Update(dt);
		
		check = TwoBodyCollision(&copyB1, &copyB2, relVel, collNorm);

		if (check == PENETRATING)
		{
			dt = dt/2;
			tryAgain = true;
			didPen = true;
		}
		else if (check == COLLISION)
		{
			memcpy(body1, &copyB1, sizeof(CRigidBody));
			memcpy(body2, &copyB2, sizeof(CRigidBody));

			collData->collType = COLLISION;
			collData->body1 = body1;
			collData->body2 = body2;
			collData->relativeVelocity = relVel;
			collData->collisionNormal  = collNorm;
		}
	}

	// okay, if the time step gets too small then we will never act on the collision
	// so lets leave the function and try again next frame

	// so lets try to automatically apply collision
	// NOPE This doesnt work
	//if (dt < _FloatTolerance_)
	//{
	//	memcpy(body1, &copyB1, sizeof(CRigidBody));
	//	memcpy(body2, &copyB2, sizeof(CRigidBody));

	//	collData->collType = COLLISION;
	//	collData->body1 = body1;
	//	collData->body2 = body2;
	//	collData->relativeVelocity = relVel;
	//	collData->collisionNormal  = collNorm;
	//}

}
void CPhysicsScene::TwoBodyResponse(CRigidBody *body1, CRigidBody *body2, CVector3f &relV, CVector3f &collN)
{
	float j;
	float fCr = COEFFICIENTOFRESTITUTION;
	// find the impulse for the bodies
	j = (-(1+fCr) * (relV.DotProduct(collN)) / 
		( collN.DotProduct(collN)  *
		(1/body1->GetMass() + 1/body2->GetMass()) ) );

	if (body1->isMovableByCollision)
		body1->velocityVec += (collN * j) / body1->GetMass() * body1->elasticity;
	if (body2->isMovableByCollision)
		body2->velocityVec -= (collN * j) / body2->GetMass() * body2->elasticity;
}

int CPhysicsScene::TwoBodyCollision(CRigidBody *body1, CRigidBody *body2, CVector3f &relV, CVector3f &collN)
{
	if (!body1 || !body2)
		return false;

	int collided = 0;
	CVector3f collNorm;
	CVector3f relVelocity;

	CVector3f distVec;
	float	  distActual;

	float	combinedRadius;
	
	CVector3f vel1;
	CVector3f vel2;

	distVec = body1->centerPosition - body2->centerPosition;
	distActual = distVec.Magnitude();

	combinedRadius = body1->radius + body2->radius;

	if (distActual > combinedRadius)
		return NOCOLLISON;

	// this is the difference of the actual amount of space separating
	float s = distActual - combinedRadius;

	distVec.Normalize();

	collNorm = distVec;

	vel1 = body1->velocityVec;
	vel2 = body2->velocityVec;
	relVelocity = vel1 - vel2;

	// return the results
	relV = relVelocity;
	collN = collNorm;
	
	float Vrn = relVelocity.DotProduct(collNorm);

	if ((fabs(s) <= COLLISIONTOLLERENCE) && (Vrn < 0.0))
	{
		// we have collision
		collided = COLLISION;
	}
	else if (s < -COLLISIONTOLLERENCE)
	{
		collided = PENETRATING;
	}
	else
	{
		collided = NOCOLLISON;
	}

	return collided;
}

void CPhysicsScene::SetWorld(tWorld *world)
{
	//if (world)
	//	theWorld = *world;
	//else
	//	return;
}

void CPhysicsScene::ConstrainForces(CRigidBody *body)
{
	if (body->forceVec.Magnitude() >= 100.0f)
	{
		body->forceVec.Normalize();
		body->forceVec *= 100.0f;
	}
}

bool CPhysicsScene::CollSphereToRay(CVector3f spherePos, float sphereRadius, CVector3f rayPoint, CVector3f rayNormal)
{
	//•	Define of a sphere: center point, radius.
	//	•	Define of a ray: starting point, normal vector (length of one).
	//	•	Create a vector called vecToSphere and make it point from the start of the ray to the center of the sphere.
	//	•	If the dot product of the ray’s normal and vecToSphere is smaller than zero than the ray and sphere are not touching. STOP TESTING
	//	•	Scale ray’s normal by the dot product of ray’s normal and vecToSphere.
	//	•	Add the ray’s normal to the start of the ray.
	//	•	Get the distance between the ray’s normal and the center of the sphere.
	//	•	If the distance is smaller than the sphere’s radius, then they are touching.

	bool isColl = false;
	CVector3f vecToSphere =  spherePos - rayPoint;
	float rDv = vecToSphere.DotProduct(rayNormal);//  rayNormal.DotProduct(vecToSphere);
	if (rDv < 0.0f)
	{
		isColl = false;
	}
	else
	{
		rayNormal *= rDv;
		rayPoint += rayNormal;
		CVector3f distV = rayNormal - spherePos;
		float dist = distV.Magnitude();
		if (dist < sphereRadius)
		{
			// they are touching
			isColl = true;
		}
	}

	return isColl;
}

int CPhysicsScene::CollSphereToPlane(CVector3f spherePos, float radius, CVector3f planePoint, CVector3f planeNormal, float *distPt)
{
	bool isColl = false;
	CVector3f distVec = spherePos - planePoint;
	float d = planeNormal.DotProduct(distVec);
	//if (is3D)
	//{
	if (d > radius)
		isColl = false;
	else if(d < radius)
	{
		*distPt = d;
		isColl = true;
	}
	//}
	//else
	//{
	//	if (d < radius)
	//		isColl = false;
	//	else if(d > radius)
	//	{
	//		*distPt = d;
	//		isColl = true;
	//	}
	//}

	return isColl;
}

void CPhysicsScene::CollResponseSphereToPlane(CRigidBody *body, CVector3f planeNormal, float distPt)
{
	// pointer check
	if (!body)
		return;
	// calculate the new position for the body
	// find the projected normal to find the new position of the body
 	CVector3f normP = planeNormal *(body->radius-distPt);
	body->centerPosition += normP;

	/************************************************************************/
	/* Lets find the vector projection this will be the new vector for the body
	ViP = Vi - ( N(N dot Vi)                                             */
	/************************************************************************/

	// find n'
	float nP = planeNormal.DotProduct(body->velocityVec);
	// now we have our vec projection calculate the new velocity
	// V' = V - 2(N DOT V)N
	CVector3f velF = body->velocityVec - ((nP*nP)*2);

	body->velocityVec = velF;
}

void		 CPhysicsScene::PushABodyUp(int index, float magnitude)
{
	if (index < 0 || listOfBodies.size() < 0)
		return;
	listOfBodies[index]->Add2Force(Y_AXIS*magnitude);
}
void		 CPhysicsScene::PushABodyDown(int index, float magnitude)
{
	if (index < 0 || listOfBodies.size() < 0)
		return;
	listOfBodies[index]->Add2Force(-Y_AXIS*magnitude);
}
void		 CPhysicsScene::PushABodyLeft(int index, float magnitude)
{
	if (index < 0 || listOfBodies.size() < 0)
		return;
	listOfBodies[index]->Add2Force(-X_AXIS*magnitude);
}
void		 CPhysicsScene::PushABodyRight(int index, float magnitude)
{
	if (index < 0 || listOfBodies.size() < 0)
		return;
	listOfBodies[index]->Add2Force(X_AXIS*magnitude);
}
void		 CPhysicsScene::PushABodyForward(int index, float magnitude)
{
	if (index < 0 || listOfBodies.size() < 0)
		return;
	listOfBodies[index]->Add2Force(Z_AXIS*magnitude);
}
void		 CPhysicsScene::PushABodyBackward(int index, float magnitude)
{	
	if (index < 0 || listOfBodies.size() < 0)
		return;
	CRigidBody *bod = listOfBodies[index];
	bod->Add2Force(-Z_AXIS*magnitude);
}
void		 CPhysicsScene::PushABodyAnyDirectAndMag(int index, CVector3f dir)
{
	if (index < 0 || listOfBodies.size() < 0)
		return;
	listOfBodies[index]->Add2Force(dir);
}
void	CPhysicsScene::SleepABody(int index)
{
	if (index < 0 || listOfBodies.size() < 0)
		return;
	listOfBodies[index]->isNapping = true;

}
void	CPhysicsScene::WakeUpABody(int index)
{
	if (index < 0 || listOfBodies.size() < 0)
		return;
	listOfBodies[index]->isNapping = false;
}
void CPhysicsScene::SleepAllBodies()
{
	unsigned int listSize = (unsigned int)listOfBodies.size();
	for (unsigned int i = 0; i < listSize; i++)
	{
		SleepABody(i);
	}
}
void CPhysicsScene::WakeUpAllBodies()
{
	unsigned int listSize = (unsigned int)listOfBodies.size();
	for (unsigned int i = 0; i < listSize; i++)
	{
		WakeUpABody(i);
	}
}

bool CPhysicsScene::HaveTheTwoBodiesCollided(int b1Indx, int b2Indx)
{
	unsigned int numColls = (unsigned int)listOfCollision.size();
	if (numColls <= 0)
		return false;
	tCollisionData currColl;
	for (unsigned int currCollIndx = 0; currCollIndx < numColls; currCollIndx++)
	{
		currColl = listOfCollision[currCollIndx];
		if (currColl.collType == COLLISION)
		{
			if ((b1Indx == currColl.body1Inx) && (b2Indx == currColl.body2Inx) 
				|| 
				(b1Indx == currColl.body2Inx) && (b2Indx == currColl.body1Inx))
				return true;
		}
	}

	return false;
}

bool CPhysicsScene::HasTheBodyCollidedWithWall(int bodyIndx, int wallIndx)
{
	for (unsigned int i = 0; i < listOfWallCollisions.size(); i++)
	{
		if (listOfWallCollisions[i].body1Inx == bodyIndx 
			&& 
			listOfWallCollisions[i].body2Inx == wallIndx)
			return true;
	}
	return false;
}
bool CPhysicsScene::HasTheBodyCollidedWithWall(int bodyIndx)
{
	for (int i = 0; i < (int)listOfWallCollisions.size(); i++)
	{
		if (listOfWallCollisions[i].body1Inx == bodyIndx)
			return true;
	}
	return false;
}
bool CPhysicsScene::isCollisionListEmpty()
{
	unsigned int numColls = (unsigned int)listOfCollision.size();
	if (numColls <= 0)
		return true;
	else 
		return false;
}

CRigidBody* CPhysicsScene::GetBody(int index)
{
	for (unsigned int indx = 0; indx <= listOfBodies.size(); indx++)
	{
		if (indx == index)
			return listOfBodies[index];
	}
	return NULL;
}

int CPhysicsScene::GetSize()
{
	return (int)listOfBodies.size();
}

bool CPhysicsScene::RemoveBody(int index)
{
	return true;
}

bool CPhysicsScene::CollPointToTriangle(CTriangle triangle, CVector3f testPoint)
{
	bool collided = false;

	/*
		•	Define of a triangle: V0 first point, V1 second point, V2 last point, normal.
		•	Define of the test point: test P.
		•	Create a vector called edge0 and make it point from V0 to V1.
					* Remember, from V0 to V1 is ( V1 – V0 ) *
		•	Create a vector called edge1 and make it point from V1 to V2.
		•	Create a vector called edge2 and make it point from V2 to V0.
		•	Create a vector called normal0 (which is the normal to the edge) and set it to the edge0 cross normal.
		•	Set normal0 to the length of one.
		•	Create a vector called normal1 (which is the normal to the edge) and set it to the edge1 cross normal.
		•	Set normal1 to the length of one.
		•	Create a vector called normal2 (which is the normal to the edge) and set it to the edge2 cross normal.
		•	Set normal2 to the length of one.
		•	Create a vector called testVec0 and make it point from V0 to testP.
		•	Create a vector called testVec1 and make it point from V1 to testP.
		•	Create a vector called testVec2 and make it point from V2 to testP.
		•	If the dot product of testVec0 and normal0 is greater than zero stop testing because it is not inside the triangle.
		•	If the dot product of testVec1 and normal1 is greater than zero stop testing because it is not inside the triangle.
		•	If the dot product of testVec2 and normal2 is greater than zero stop testing because it is not inside the triangle.
		•	If you have made it this far the test point is inside the triangle.
	*/
	CVector3f edge0, edge1, edge2;
	edge0 = triangle.pt[1] - triangle.pt[0];
	edge1 = triangle.pt[2] - triangle.pt[1];
	edge2 = triangle.pt[0] - triangle.pt[2];

	CVector3f normal0 = /*triangle.n.CrossProduct(edge0);*/ edge0.CrossProduct(triangle.n);
	CVector3f normal1 = /*triangle.n.CrossProduct(edge1);*/ edge1.CrossProduct(triangle.n);
	CVector3f normal2 = /*triangle.n.CrossProduct(edge2);*/ edge2.CrossProduct(triangle.n);

	normal0.Normalize();
	normal1.Normalize();
	normal2.Normalize();


	CVector3f testVec0 = testPoint - triangle.pt[0];
	CVector3f testVec1 = testPoint - triangle.pt[1];
	CVector3f testVec2 = testPoint - triangle.pt[2];

	float dot0, dot1, dot2;
	dot0 = normal0.DotProduct(testVec0);//testVec0.DotProduct(normal0);
	dot1 = normal1.DotProduct(testVec1);//testVec1.DotProduct(normal1);
	dot2 = normal2.DotProduct(testVec2);//testVec2.DotProduct(normal2);

	if (dot0 > 0.0f)
		return false;
	if (dot1 > 0.0f)
		return false;
	if (dot2 > 0.0f)
		return false;

	return true;
}

bool CPhysicsScene::CollSphereToTriangle(CTriangle triangle, CVector3f spherePos, float radius, CVector3f *newSpherePos)
{
	/*
	•	Define of a sphere: center, radius.
	•	Define of a triangle: three vertices, normal.
	•	Create a vector called vecToCenter and make it point from one of the triangle vertices to the sphere’s center.
	•	Take the dot product of vecToCenter and the triangle’s normal and store it in dis2plane.
	•	Create a vector called projVector and set it to the triangle’s normal scaled by the negative dis2plane.
	•	Add projVector to the sphere’s center to get the projected point on the triangle’s plane.
	•	Use the point in triangle collision test to see if the projected point is in the triangle.
	•	If the projected point is inside the triangle, create a vector called move and set it to the triangle’s normal 
		scaled by the sphere’s radius minus the dis2plane.  Then add move to the sphere’s center to get the new sphere’s center.
	*/

	CVector3f vecToCenter = spherePos - triangle.pt[0];
	float dis2Plane = vecToCenter.DotProduct(triangle.n);
	CVector3f projVector = triangle.n * (-dis2Plane);
	CVector3f projPoint  = spherePos + projVector;
	if (CollPointToTriangle(triangle, projPoint))
	{
		//CVector3f move = triangle.n * (radius - dis2Plane);
		*newSpherePos = triangle.n * (radius - dis2Plane);
		return true;
	}
	return false;
}

bool CPhysicsScene::CollLineSegToTriangle(CTriangle triangle, CVector3f pStart, CVector3f pEnd)
{
	/*
	•	Define of a triangle: three vertices, normal.
	•	Define of a line: start point, end point.
	•	Create a vector called lineNormal and make it point from the start of the line to the end of the line.
	•	Set lineNormal to the length of one.
	•	Take the dot product of the line’s start point and the triangle’s normal and store it in startOffSet.
	•	Take the dot product of lineNormal and the triangle’s normal and store it in U.
	•	Take the dot product of triangle’s normal and one of the triangle’s vertices and store it in planeOffSet.
	•	Subtract startOffSet from planeOffset, then divide by U and store that in distance.
	•	If the distance is less than zero or greater than the length of the line stop testing.
	•	Scale lineNormal by distance.
	•	Add lineNormal to the start of the line to get the project point to the triangle’s plane.
	•	Use the point in triangle collision test to see if the projected point is in the triangle.
	*/
 	bool collided = false;
	CVector3f lineNormal = pEnd - pStart;
	float lineLength = lineNormal.Magnitude();
	lineNormal.Normalize();

	float startOffset = pStart.DotProduct(triangle.n);
	float U = lineNormal.DotProduct(triangle.n);
	float planeOffset = triangle.n.DotProduct(triangle.pt[0]);
	float dist = (planeOffset - startOffset) / U;
	if (dist < 0.0f || dist > lineLength)
	{
		collided = false;
	}
	else
	{
		lineNormal *= dist;
		CVector3f projPoint = pStart + lineNormal;
		if (CollPointToTriangle(triangle, projPoint))
		{
			collided = true;
		}
	}
	return collided;
}

int CPhysicsScene::GetBody(CRigidBody *body)
{
	if (body)
	{
		for (int i = 0; i < numOfBodies; i++)
		{
			CRigidBody *currBody = listOfBodies[i];
			if (currBody == body)
				return i;
		}
	}
	return -1;
}

void CPhysicsScene::SetGravity(CVector3f g)
{
	int size = (int)listOfBodies.size();
	CRigidBody *pBody = NULL;
	for (int currBody = 0; currBody < size; currBody++)
	{
		pBody = listOfBodies[currBody];
		pBody->gravityForce = g;
	}
//	pBody = NULL;
}


















/************************************************************************/
/* This was all to do friction                                          */
/************************************************************************/










//
//
//
//
//		dP = 0.0f;
//		currPlane = &theWorld.m_worldPlanes[i];
//
//		n = currPlane->normal;//theWorld.m_worldPlanes[i].normal;
//		if (CollSphereToPlane(body->centerPosition, body->radius, theWorld.m_worldPlanes[i].point, n, &dP))	
//		{
//			//CollResponseSphereToPlane(body, theWorld.planes[i].normal, dP);
//			// find the projected normal to find the new position of the body
//			if (currPlane->handleCollisionResponse)
//			{
//				normP = n *(body->radius - dP);
//				body->centerPosition += normP;
//				/************************************************************************/
//				/************************************************************************/
//				/* Lets find the vector projection this will be the new vector for the body
//				ViP = Vi - ( N(N dot Vi)                                             */
//				/************************************************************************/
//
//				// find n'
//				nP = n.DotProduct(body->velocityVec);
//				// now we have our vec projection calculate the new velocity
//				// V' = V - 2(N DOT V)N
//				velF = body->velocityVec - ((n*nP)*2);
//
//				body->velocityVec = velF;//body->velocityVec - ((n*nP)*2);
//			}
//
//
//			/************************************************************************/
//			/*  
//					// lets attempt some friction when we find the collision
//					// step 1: we have - the collision point for where the collision happened
//					//				   - the normal for the plane
//					//				   - the current velocity of the body
//					//				   - 
//					// lets see in order to respond as if two bodies collided we will need
//					//	to find the relative velocity and the collision normal
//					//  
//					*/
//					// i need to find the tangential force vector
////					CVector3f tF = n.CrossProduct(body->velocityVec);
////					tF = tF.CrossProduct(n);
////					tF.Normalize();
////					
////					float J;
////					float fCr = COEFFICIENTOFRESTITUTION;
////					float fSf = theWorld.m_worldPlanes[i].staticCoeFric;
////					
////					
////					
////					CVector3f collN = (n*n.DotProduct(body->velocityVec))*2;
////					J = (-(1+fCr) * (body->velocityVec.DotProduct(collN)) /
////							(collN.DotProduct(collN)	*	(1/body->GetMass() + 1/body->GetMass())));
////
////					// find the impulse for the bodies
////					//j = (-(1+fCr) * (relV.DotProduct(collN)) / 
////					//	( collN.DotProduct(collN)  *
////					//	(1/body1->GetMass() + 1/body2->GetMass()) ) );
////
//////					CVector3f tFloat;
//////					tFloat = (tF*(fSf*J));
////					body->velocityVec = body->velocityVec + (((n*J) + (tF*(fSf*J)))/body->GetMass());
////
//
//
//                  /*                                                  */
//
//			// store that we collided with a wall
//
//	}
//