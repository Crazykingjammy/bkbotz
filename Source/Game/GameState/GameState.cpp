#include "gamestate.h"

void CBaseGameState::ChangeState(Game* game,CBaseGameState* newstate)
{
	//Change the games State.
	game->ChangeState(newstate);

}
