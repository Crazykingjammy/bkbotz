#ifndef _VIRTUAL_KEYBOARD_H_
#define _VIRTUAL_KEYBOARD_H_

//#include "../../../2D.h"
#include "../../Render/RenderInterface.h"
#include "../../Physics/PhysicsTimer.h"
#include "../../Input/InputMgr.h"




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

class CVirtualKeyboard
{
private:
#define ROW 3
#define COL 10

	CRenderInterface *pRenderInstance;
	InputManager	 *pInputInstance;

	// the rows and colums for the keyboard
	unsigned int m_MaxRows;
	unsigned int m_MaxCols;
	// the top left corner position of keyboard positioned for screen
	CVector3f m_Position;


	// this will represent every letter in the
	// alphabet A-Z a-z
	CLabel *m_Keys;		// the array of keyboard key textures
	CLabel *m_KeyIter;
	int		m_IterXpos, m_IterYPos;

	int		m_NumOfKeys;

	CLabel *m_Output;

	float m_WinWidth, m_WinHeight;

	tCell m_Cell[ROW][COL];

public:

	CVirtualKeyboard(float winWidth = 800.0f, float winHeight = 600.0f);
	~CVirtualKeyboard(void);

	void Initalize();
	void Update();
	void Shutdown();

	// here we will position the keys
	void CreateKeyboardLayout();


};



















#endif