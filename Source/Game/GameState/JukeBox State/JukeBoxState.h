#ifndef __JukeBox_State_h__
#define __JukeBox_State_h__

#include "../GameState.h"


class CJukeBoxState : public CBaseGameState
{
private:

#define ROW 3
#define COL 10

	struct tCell
	{
		float x, y;			// screen coords for cell
		int   indxToObj;	// hold an index to the representing object/ label
		tCell(void)
		{
			x = y = 0.0f;
			indxToObj = 0;
		}
	};

	static CJukeBoxState *pInstance;

	CFreeCamera *jCamera0;

	// this will represent every letter in the 
	// alphabet A-Z a-z
	CLabel *jKeys;
	CLabel *jKeyIter;
	int		jIterXpos, jIterYPos;

	int		jNumOfKeys;

	CLabel *jOutput;

	CPhysicsScene	jPhysicsScene;

	float jWinWidth, jWinHeight;

	tCell jCell[ROW][COL];


public:
	static CJukeBoxState* GetInstance();

	CJukeBoxState(void);
	~CJukeBoxState(void);

	void Initalize(Game* game);
	void Update(float timeDelta);
	void Shutdown();

	// here we will position the keys
	void CreateKeyboardLayout();




};


#endif