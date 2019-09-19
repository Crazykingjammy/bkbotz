#ifndef __Lights_h__
#define __Lights_h__

#include "../../Object/Object.h"


//Enums for types of lights... yes.. i had to say it.
enum CLightType {PointLight = 0, DirectionalLight = 2, SpotLight = 4};

class CLight : public CBaseObject
{
public:

	//Members
	EXColor m_Ambient,m_Diffuse,m_Specular;
	CVector3f m_Direction;
	float m_Range, m_Falloff, m_Attenuation0, m_Attenuation1, m_Attenuation2, m_Theta, m_Phi;

	//Light type.
	bool m_Active, m_CastShadow;

	//0 = point, 2 = dir, 4 = spot
	char m_Type;

	//Redundent extra 2 bytes to make the class have that 4 byte allignment 
	char scrach1,scrach2,scrach3;


public:
	
	//Functions

	CLight(bool active = false);
	~CLight();

	//Reset lights to defualt values.
	void Reset();


};



#endif