// Timer class for physics
#ifndef _PHYSICS_TIMER_H_
#define _PHYSICS_TIMER_H_

#include <Windows.h>

class CPhysicsTimer
{
protected:
	LARGE_INTEGER counterFrequency;
	LARGE_INTEGER lastCount;
public:
	
	CPhysicsTimer(void)
	{
		QueryPerformanceFrequency(&counterFrequency);
		QueryPerformanceCounter(&lastCount);
	}
	~CPhysicsTimer(void){}

	// resets timer to zero
	void Reset() 
	{ 
		QueryPerformanceCounter(&lastCount);	
	}
	float GetElapsedSeconds(void)
	{
		// Get the current count
		LARGE_INTEGER currCount;
		QueryPerformanceCounter(&currCount);
		float seconds = float((currCount.QuadPart - lastCount.QuadPart) / float(counterFrequency.QuadPart));
		return seconds;
	}
};

#endif