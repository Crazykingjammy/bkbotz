#ifndef __Bar_h__
#define __Bar_h__

#include "../2D.h"

class CMeter : public CLabel
{
public:
	//Members
	float m_MaxCapacity;
	float m_CurrentCapacity;


public:

	CMeter(char* filename) : CLabel(filename)
	{
		m_MaxCapacity		= 100.0f;
		m_CurrentCapacity	= 100.0f;
	}
	~CMeter(){}

	//Methods
	void Subtract(float amount)
	{
		if(m_CurrentCapacity <= 0.0f)
		{
			m_CurrentCapacity = 0.0f;
			m_Rect.right = 0;
			return;
		}

		//Update capacity.
		m_CurrentCapacity -= amount;

		//Resize the rect.
		m_Rect.right = (long) (m_ImageWidth * (m_CurrentCapacity/m_MaxCapacity) );

	}

	void Add(float amount)
	{
		if(m_CurrentCapacity > m_MaxCapacity)
		{
			m_CurrentCapacity = m_MaxCapacity;
			m_Rect.right = this->m_ImageWidth;
			return;
		}

		//Update capacity.
		m_CurrentCapacity += amount;

		//Resize the rect.
		m_Rect.right = (long) (m_ImageWidth * (m_CurrentCapacity/m_MaxCapacity) );



	}

	void Fill() {m_CurrentCapacity = m_MaxCapacity; Add(0.0f); } 
	void Empty() {m_CurrentCapacity = 0;Add(0.0f); }
};


#endif
