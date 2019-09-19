#include "lights.h"


CLight::CLight(bool active)
{
	//Set the type
	m_Type = PointLight;

	m_Active = false;
	m_CastShadow = true;

	//Set positions
	m_Ambient		= 0.1f;
	m_Diffuse		= 0.8f;
	m_Specular		= 1.0f;
	m_Direction.y	= -1.0f;

	m_Range			= 1000.0f;
	m_Falloff		= 0.7f;
	m_Attenuation0	= 0.0f;
	m_Attenuation1	= 0.01834f;
	m_Attenuation2	= 0.0f;
	m_Theta			= 30.5f;
	m_Phi			= 0.2f;

	strcpy(m_ModelName,"models/Light.x");

}

void CLight::Reset()
{
	//Set the type
	m_Type = PointLight;

	m_Active = false;
	m_CastShadow = true;

	//Set positions
	m_Ambient		= 0.1f;
	m_Diffuse		= 0.8f;
	m_Specular		= 1.0f;
	m_MyBody.centerPosition	= 0.0f;
	m_Direction		= 1.0f;

	m_Range			= 1000.0f;
	m_Falloff		= 1.0f;
	m_Attenuation0	= 0.0f;
	m_Attenuation1	= 0.01834f;
	m_Attenuation2	= 0.0f;
	m_Theta			= 0.5f;
	m_Phi			= 0.7f;

}

CLight::~CLight()
{
}