#include "../gamestate.h"


CBattleGameState* CBattleGameState::GetInstance()
{
	//If there is no instance
	if(pInstance == 0)
		pInstance = new CBattleGameState;		//We Create One

	return pInstance;							//Return the Instance.
}

void CMenuState::Shutdown()
{

}

void CMenuState::Initalize(Game* game)
{
	pGame = game;
}



void CMenuState::Update(float timeDelta)
{

}


