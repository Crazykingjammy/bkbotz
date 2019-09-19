#ifndef __Arena_h__
#define __Arena_h__


#include "../Object.h"

class CArena : public CBaseObject
{
public:


public:

	//Constructor/Destructor
	CArena(char* modelname)
	{
		//Copy the name.
		strcpy(m_ModelName,modelname);
	}
	CArena()
	{

	}
	~CArena(){}
};


#endif