#ifndef __BookKeeper_h__
#define __BookKeeper_h__

#include "../GameProfiler/GameProfiler.h"

class CBookKeeper
{

	//Members
private:
	int m_PlayerPoints[PLAYER_COUNT];
	int m_MaxPoints, m_CurrentRound;


	//Methods.
public:
	CBookKeeper()
	{
		NewMatch();
		m_MaxPoints = 2;
	}
	~CBookKeeper()
	{

	}

	// We have won a round!
	void AddPoint(int player){m_PlayerPoints[player] += 1; m_CurrentRound++;}
	void SetMaxPoints(int rounds) {m_MaxPoints = rounds;} 
	int WhichRound() {return m_CurrentRound;}


	//Fresh rounds...of bullets..and pizza.. and sex; and shots.
	void NewMatch() 
	{
		//We dont have anything from the begning.
		for(int i = 0; i < PLAYER_COUNT; i++)
		{
			m_PlayerPoints[i] = 0;
		}

		m_CurrentRound = 1;
	}

	//Returns -1 if nobody has won yet.. or returns player who is the winner.
	int WhoWon()
	{
		//-1 means nobody is winning.
		int Winner = -1;

		//Who the fuck has the most points.
		//for(int i = 0; i < PLAYER_COUNT; i++)
		//{
		//	for(int j = 0; j < PLAYER_COUNT; j++)
		//	{
		//		if(m_PlayerPoints[i] > m_PlayerPoints[j])
		//		{
		//			Winner = i;
		//		}
		//	}
		//}

		if(m_PlayerPoints[0] > m_PlayerPoints[1] )
			Winner = 0;

		if(m_PlayerPoints[1] > m_PlayerPoints[0] )
			Winner = 1;

		if(m_PlayerPoints[0] == m_PlayerPoints[1] )
			Winner = -1;

		//Now check if the points are winning the round.
		if(m_PlayerPoints[Winner] < m_MaxPoints)
			Winner = -1;

		return Winner;
	}


};


#endif