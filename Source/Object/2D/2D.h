#ifndef __2D_h__
#define __2D_h__

#include "../Object.h"

struct sRectLabel
{
	long left, top, right, bottom;
};


class CLabel : public CBaseObject
{
public:
	//Store the param of the Image to draw.
	sRectLabel m_Rect;

	//Text if we are using this label to display text.
	char m_Text[64];

	//Storing the images full size.
	long m_ImageHeight, m_ImageWidth;
	long m_FontWidth, m_FontHeight;

	CVector3f m_Scale;
	float	  m_Rotation;

	

	
public:
	//Methods
	CLabel()
	{
		//Assign the image to laod.
		//this->m_ModelName = filename;
		strcpy(m_ModelName,"NoFileName");

		m_Rect.left			= 0;
		m_Rect.top			= 0;
		m_Rect.right		= 0;
		m_Rect.bottom		= 0;

		m_FontHeight		= 30;
		m_FontWidth         = 30;

		m_MyBody.centerPosition.MakeZero();

		//m_MyBody.isNapping = true;
		m_MyBody.isEffectedByDrag = true;

		strcpy(m_Text,"><><><");
		m_Scale = 1.0f;
		m_Rotation = 0.0f;

	}
	CLabel(char* filename)
	{
		//Assign the image to laod.
		//this->m_ModelName = filename;
		strcpy(m_ModelName,filename);

		m_Rect.left			= 0;
		m_Rect.top			= 0;
		m_Rect.right		= 0;
		m_Rect.bottom		= 0;

		m_MyBody.centerPosition.MakeZero();

		//m_MyBody.isNapping = true;
		m_MyBody.isEffectedByDrag = true;

		m_FontHeight		= 30;
		m_FontWidth         = 30;

		strcpy(m_Text,"><><><");
		m_Scale = 1.0f;
		m_Rotation = 0.0f;
	}
	~CLabel(){}
};




#endif