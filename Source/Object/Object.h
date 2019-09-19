#ifndef __Object_h__
#define __Object_h__

#include "../main.h"
#include "../Physics/RigidBody.h"


class CBaseObject
{
public:

	CRigidBody m_MyBody;

	//Orientation for Rendering
	CVector3f m_RenderingOrientation;


	char m_ModelName[256];
	int ModelIndex;
	int m_MyBodyIndex;
	bool m_Visible;
	bool m_DrawShadow;
	bool m_DrawBoundingSphere;


public:

	CBaseObject()
	{
		//Visible as default.
		m_Visible = true;

		//VisibleShadow  at create
		m_DrawShadow = true;

		//Dont draw the bounding sphere.
		m_DrawBoundingSphere = false;

		//Default index.
		int ModelIndex = 0;

		//Face Forward 
		m_MyBody.vOrientation.z = 1.0f;

	}

	~CBaseObject() {}

	//Accessors
	float GetX() {return m_MyBody.centerPosition.x;}
	float GetY() {return m_MyBody.centerPosition.y;}
	float GetZ() {return m_MyBody.centerPosition.z;}

	void RotateX(float angle)
	{
		m_MyBody.vOrientation.RotateX(angle);
		m_RenderingOrientation.x += angle;
	}

	void RotateY(float angle)
	{
		m_MyBody.vOrientation.RotateY(angle);
		m_RenderingOrientation.y += angle;
	}

	void RotateZ(float angle)
	{
		m_MyBody.vOrientation.RotateZ(angle);
		m_RenderingOrientation.z += angle;
	}

};


class CWorld : public CBaseObject
{

public:

	//List of triangles.
	vector<CTriangle> m_Triangles;

	//Constructor & Destructor

	//This will take in filenames for skybox as well.
	CWorld(char* filename) {strcpy(m_ModelName,filename);}
	
	~CWorld() {}

public:


};



#endif