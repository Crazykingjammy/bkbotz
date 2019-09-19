#include "PhysicsWorld.h"

CPhysicsWorld::CPhysicsWorld()
{
	m_numberOfPlanes = 0;
	m_worldPlanes.reserve(6);
}

CPhysicsWorld::~CPhysicsWorld()
{
	m_worldPlanes.clear();
}

void CPhysicsWorld::SetDefaultWorld()
{
	// create the right wall
	tWorldPlane plane;
	int i;
	i = AddPlaneToWorld(CVector3f(37.5f, 0.0f, 0.0f), CVector3f(-1.0f, 0.0f, 0.0f), true,
		"Right Wall");
	i = AddPlaneToWorld(CVector3f(-37.5f, 0.0f, 0.0f), CVector3f(1.0f, 0.0f, 0.0f), true,
		"Left Wall");
	i = AddPlaneToWorld(CVector3f(0.0f, 0.0f, 50.0f), CVector3f(0.0f, 0.0f, -1.0f), true,
		"Front Wall");
	i = AddPlaneToWorld(CVector3f(0.0f, 0.0f, -50.0f), CVector3f(0.0f, 0.0f, 1.0f), true,
		"Back Wall");
	i = AddPlaneToWorld(CVector3f(0.0f, 50.0f, 0.0f), CVector3f(0.0f, -1.0f, 0.0f), true,
		"Ceiling Wall");
	i = AddPlaneToWorld(CVector3f(0.0f, 0.0f, 0.0f), CVector3f(0.0f, 1.0f, 0.0f), true,
		"Floor Wall");
}

int  CPhysicsWorld::AddPlaneToWorld(CVector3f pt, CVector3f n, bool doResponse, char *name, float sCF, float kCF)
{
	tWorldPlane newPlane;
	if (!name)
	{
		newPlane.point = pt;
		newPlane.normal = n;
		newPlane.staticCoeFric = sCF;
		newPlane.kineticCoeFric= kCF;
		newPlane.handleCollisionResponse = doResponse;
		newPlane.planeIndx = m_numberOfPlanes;
		strcpy(newPlane.planeName, "DefaultWorldPlane");
		m_worldPlanes.push_back(newPlane);
		m_numberOfPlanes = (int)m_worldPlanes.size();
	}
	else
	{
		newPlane.point = pt;
		newPlane.normal = n;
		newPlane.staticCoeFric = sCF;
		newPlane.kineticCoeFric= kCF;
		newPlane.handleCollisionResponse = doResponse;
		newPlane.planeIndx = m_numberOfPlanes;
		strcpy(newPlane.planeName, name);
		m_worldPlanes.push_back(newPlane);
	}
	m_numberOfPlanes = (int)m_worldPlanes.size();
	return m_numberOfPlanes-1;		// one less from what we have this is the index in the array
}