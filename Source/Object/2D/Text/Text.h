#ifdef	__Text_Label_h__
#define __Text_Label_h__


#include "../../Object.h"


class CText2D : public CBaseObject
{
public:
	// Members
	LPCSTR m_Text;



public:
	//Functions
	CText2D(LPCSTR filename,LPCSTR text)
	{
		m_ModelName = filename;
		m_Text = text;
	}
	~CText2D() {};

	void SetText(LPCSTR text) {m_Text = text}; 

};



#endif