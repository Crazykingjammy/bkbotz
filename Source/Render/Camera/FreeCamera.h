#ifndef __Free_Camera_h__
#define __Free_Camera_h__


#include "Camera.h"


class CFreeCamera : public CBaseCamera
{
public:
	//Nothing to much here.

public:

	CFreeCamera::CFreeCamera(float ffov = 0.25f, float faspect = 4/3, float fnear = 0.5f, float ffar = 50000.0f)
	{
		m_Fov = _PI_ * ffov;
		m_Aspect = faspect;
		m_Near = fnear;
		m_Far = ffar;
		m_LinearSpeed = 1.0f;
		m_Look = 0.0f;
		m_Look.z = 1.0f;
		m_Up = 0.0f;
		m_Up.y = 1.0f;
		m_Right = 0.0f;
		m_Right.x = 1.0f;
	}




};



#endif