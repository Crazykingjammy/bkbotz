#ifndef __Camera_h__
#define __Camera_h__

#include "../../Object/Object.h"

class CBaseCamera : public CBaseObject
{
public:
	CVector3f m_Right, m_Up, m_Look;
	float m_Fov, m_Aspect, m_Near, m_Far;
	float m_LinearSpeed, m_RotationXSpeed, m_RotationYSpeed, m_SpeedFactor;
	bool m_Active;
public:


	CBaseCamera()
	{
		m_LinearSpeed		= 1.0f;
		m_RotationXSpeed	= 0.005f;
		m_RotationYSpeed	= 0.005f;
		m_SpeedFactor		= 1000.0f;
		m_Active			= false;

	}

	void SetLens(float fov, float aspect, float fnear, float ffar);
	void LookAt(CBaseObject* obj);
	void FollowLookAt(CBaseObject * obj);
	void LookAt(CVector3f pos);
	
	//Rotation of camera
	void SetViewFromMouse(float deltax, float deltay);

	//Rotate the angles
	void RotateView(float angle, CVector3f look);
	
	void LookLeft();
	void LookUp();
	void LookRight();
	void LookDown();

	//Movement of camera.
	void MoveFoward()		{m_MyBody.centerPosition += m_Look  * (m_LinearSpeed);}
	void MoveBackward()		{m_MyBody.centerPosition -= m_Look  * (m_LinearSpeed);}
	void MoveLeft()			{m_MyBody.centerPosition -= m_Right * (m_LinearSpeed);}
	void MoveRight()		{m_MyBody.centerPosition += m_Right * (m_LinearSpeed);}
	void MoveUp()			{m_MyBody.centerPosition += m_Up	* (m_LinearSpeed);}
	void MoveDown()			{m_MyBody.centerPosition -= m_Up	* (m_LinearSpeed);}

};



#endif