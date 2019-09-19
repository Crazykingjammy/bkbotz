#ifndef __Hud_h__
#define __Hud_h__

#include "../Bar/Bar.h"


class CPlayerHud
{
public:

	//Members

	int m_ScreenWidth, m_ScreenHeight;

	CLabel m_Frame, m_PlayerIcon, m_WinIcon, m_Score, m_BallCount; 
	CMeter *m_HeatMeter;
	CMeter *m_TargetLife;
	CLabel* m_EmptyPoints;

	int m_Points4Win,m_CurrentPoints;

public:

	//Methods

	CPlayerHud();

	//Initalize the hud. set position before you call this.
	void Initalize(int screenwidth, int screenheight, CMeter * heatmeter, CMeter* lifemeter);

	//Yeah yeah you dont need to use this its internal in the inititalize.
	void AddHudToRender();

	//Update hehehehehehe... yeah... this sets the ball number strings for the huds.
	void Update(int numballs, int maxballs );

	//Shows and Hides the hud
	void Hide(bool makevisible);

	//Update the hud
	void AddPoint()
	{
		if(m_CurrentPoints < m_Points4Win)
		{
			m_EmptyPoints[m_CurrentPoints].ModelIndex = m_WinIcon.ModelIndex;
			m_CurrentPoints++;


		}

	}

	void Flip();

	~CPlayerHud();
};


#endif