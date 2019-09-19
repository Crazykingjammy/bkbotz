
#ifndef __Intro_State_h__
#define __Intro_State_h__

#include "../Gamestate.h"


class CIntroGameState : public CBaseGameState
{
private:

	//Instance
	static CIntroGameState* pInstance;

	//Objects
	CFreeCamera* Camera0;
	CFreeCamera* Camera1;

	CBall*		Ship;

	CLabel*		Exit;
	CLabel*		Battle;
	CLabel*		Soccer;
	CLabel*		Credits;
	CLabel*		Me;



	CLabel*		GameText;

	CPhysicsScene m_Scene;
	CPhysicsScene m_2DScene;

	CWorld* m_World;


	CPhysicsTimer m_InputTimer;
	float m_InputFreq;


	//Numbers
	float MeForce;


	int CurrentSelection;
	enum {BATTLE = 0,SOCCER,CREDITS,EXIT};

public:

	//Access
	static CIntroGameState* GetInstance();

	CIntroGameState(void) {}

	//Methods
	void Initalize(Game* game);
	void Update(float timeDelta);
	void Shutdown();

public:
	//Extra functions for this state.
	void Controls();
	void CollisionDetection();

};


#endif