#ifndef __Math_h__
#define __Math_h__

class CTEMPMatrix
{
public:

	float X,	 Y,	 Z;
	float RX,	RY,	RZ;
	float SX,	SY,	SZ;
	float EX,	EY,	EZ;

public:
	CTEMPMatrix()
	{
		X	= Y	= Z	=	0.0f;
		RX	= RY =	RZ = 0.0f;
		SX = SY = SZ = 0.0f;
		EX = EY = EZ = 0.0f;
	}
};


#endif