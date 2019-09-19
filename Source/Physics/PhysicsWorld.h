#ifndef _PHYSICS_WORLD_H_
#define _PHYSICS_WORLD_H_

#include <vector>
using namespace std;
#include <time.h>
#include "Math.h"

#define NAME_LENGTH 128

// a plane is defined as a point and a normal
struct tWorldPlane
{
	CVector3f point;
	CVector3f normal;
	float	  staticCoeFric;
	float	  kineticCoeFric;
	int		  planeIndx;
	char	  planeName[NAME_LENGTH];
	bool	  handleCollisionResponse;
	tWorldPlane()
	{
		staticCoeFric = kineticCoeFric = 0.0f;
		memset(planeName, 0, NAME_LENGTH);
		handleCollisionResponse		=   true;
	}
};

class CPhysicsWorld
{
private:

public:
	// this will store all the planes loaded in the world
	vector <tWorldPlane> m_worldPlanes;
	int m_numberOfPlanes;
public:
	CPhysicsWorld();
	~CPhysicsWorld();
	void SetDefaultWorld();
	int  AddPlaneToWorld(CVector3f pt, CVector3f n, 
		bool doResponse = true, char *name = NULL, 
		float sCF = 0.0f, float kCF = 0.0f);

};
















#endif