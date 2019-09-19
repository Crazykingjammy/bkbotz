#include "VirtualKeyboard.h"

CVirtualKeyboard::CVirtualKeyboard(float winWidth, float winHeight)
{
	pRenderInstance = CRenderInterface::GetInstance();
	pInputInstance  = InputManager::GetInstance();

	m_WinWidth  = winWidth;
	m_WinHeight = winHeight;

	Initalize();
}
CVirtualKeyboard::~CVirtualKeyboard(void)
{
	Shutdown();
}

void CVirtualKeyboard::Initalize()
{

	// the total number of chars for caps 
	//	and lower case letters
	m_NumOfKeys = 52;	
	m_Keys = new CLabel[m_NumOfKeys];



	int AtoZ = m_NumOfKeys/2;

	// create all of the labels we will need for each char
	for (int i = 0; i < AtoZ; i++)
	{
		strcpy(m_Keys[i].m_ModelName, "Textures/keysTexture32.bmp");
		m_Keys[i].m_FontWidth = 30;

		char alphabit = (char)i+65;
		sprintf(m_Keys[i].m_Text, "%c", alphabit);
		sprintf(m_Keys[i].m_MyBody.myName, "%c", alphabit);		

		m_Keys[i].m_Visible = true;
		//m_Keys[i].m_MyBodyIndex = m_PhysicsScene.AddBody(&m_Keys[i].m_MyBody);
		pRenderInstance->AddLabel(&m_Keys[i]);
	}
	m_KeyIter = new CLabel("Textures/keyiter32.bmp");
	m_IterXpos = m_IterYPos = 0;
	pRenderInstance->AddLabel(m_KeyIter);

	m_Output = new CLabel("Textures/font.tga");

	m_Output->m_MyBody.centerPosition.x = 0.0f;
	m_Output->m_MyBody.centerPosition.y = m_WinHeight-100;
	strcpy(m_Output->m_Text, "");
	pRenderInstance->AddLabel(m_Output);

	//for (int i = AtoZ; i < jNumOfKeys; i++)
	//{
	//	jKeys[i].m_ModelName = "Textures/.bmp";
	//	char alphabit = (char)i+65;
	//	memset(jKeys[i].m_Text, 0, sizeof(jKeys[i].m_Text));
	//	memcpy(jKeys[i].m_Text, &alphabit, sizeof(char));

	//	jKeys[i].m_MyBodyIndex = jPhysicsScene.AddBody(&jKeys[i].m_MyBody);
	//	pRenderInterface->AddLabel(&jKeys[i]);
	//}
	////////////////////////////////////////////////////////////////////////////

	// format the cells to represent a key board
	// for Cells positions

	for (int row = 0; row < ROW; row++)
	{
		for (int col = 0; col < COL; col++)
		{
			m_Cell[row][col].x = 100.0f;
			m_Cell[row][col].y = 100.0f;

			m_Cell[row][col].x += (col) * 30;// * ((jWinWidth/COL)-30);
			// the height of the key 
			m_Cell[row][col].y += (row) * 30;// * ((jWinHeight/ROW)-30);
		}
	}


	// to set key object index
	CreateKeyboardLayout();

}
void CVirtualKeyboard::Update()
{
	//jKeyIter->m_MyBody.centerPosition.x += 50.0f;
	//jKeyIter->m_MyBody.centerPosition.y += 50.0f;

	m_KeyIter->m_Rotation += 0.01f;
	// now position the keys to cells
	for (int row = 0; row < 3; row++)
	{
		for (int col = 0; col < 10; col++)
		{
			if (m_Cell[row][col].indxToObj >= 0)
			{
				CLabel *curLabel = &m_Keys[m_Cell[row][col].indxToObj];
				curLabel->m_MyBody.centerPosition.x = m_Cell[row][col].x;
				curLabel->m_MyBody.centerPosition.y = m_Cell[row][col].y;
				//curLabel->m_Rotation += 0.0010f;
			}
		}
	}


	m_KeyIter->m_MyBody.centerPosition.x = m_Cell[m_IterYPos][m_IterXpos].x;
	m_KeyIter->m_MyBody.centerPosition.y = m_Cell[m_IterYPos][m_IterXpos].y;

	static CPhysicsTimer inputTimer;
	if (inputTimer.GetElapsedSeconds() > 0.1f)
	{
		if (pInputInstance->ControllerJoystickUp(-1))
		{
			m_IterYPos++;
			if (m_IterYPos > ROW-1)
				m_IterYPos = 0;
		}
		if (pInputInstance->ControllerJoystickDown(-1))
		{
			m_IterYPos--;
			if (m_IterYPos < 0)
				m_IterYPos = ROW-1;
		}

		if (pInputInstance->ControllerJoystickLeft(-1))
		{
			m_IterXpos++;
			if (m_IterXpos > COL-1)
				m_IterXpos = 0;
		}
		if (pInputInstance->ControllerJoystickRight(-1))
		{
			m_IterXpos--;
			if (m_IterXpos < 0)
				m_IterXpos = COL-1;
		}

		inputTimer.Reset();
	}

	static CPhysicsTimer buttonTimer;
	if (buttonTimer.GetElapsedSeconds()> 0.1f)
	{
		if (pInputInstance->ControllerJoyPress(0, 0))
		{
			if (strlen(m_Output->m_Text) <= 20 && m_Cell[m_IterYPos][m_IterXpos].indxToObj >= 0)
				strcat(m_Output->m_Text, m_Keys[m_Cell[m_IterYPos][m_IterXpos].indxToObj].m_Text);
		}
		if (pInputInstance->ControllerJoyPress(1, 0))
		{
			strcpy(m_Output->m_Text, "");
		}

		buttonTimer.Reset();
	}
}
void CVirtualKeyboard::Shutdown()
{
	delete [] m_Keys;
	delete m_KeyIter;
	delete m_Output;


}

// here we will position the keys
void CVirtualKeyboard::CreateKeyboardLayout()
{

	/*
	A:A   65     1
	B:B   66     2
	C:C   67     3
	D:D   68      4 
	E:E   69       5     
	F:F   70        6    
	G:G   71         7    
	H:H   72          8  
	I:I   73           9 
	J:J   74            10   
	K:K   75              11   
	L:L   76             12
	M:M   77               13
	N:N   78               14
	O:O   79                15
	P:P   80               16
	Q:Q   81            17
	R:R   82             18
	S:S   83               19  
	T:T   84                 20 
	U:U   85                21
	V:V   86              22
	W:W   87            23
	X:X   88            24
	Y:Y   89              25 
	Z:Z   90                26  
	*/

	/************************************************************************/
	// top row of keys Q - P
	m_Cell[0][0].indxToObj = 16;		// Q
	m_Cell[0][1].indxToObj = 22;		// W
	m_Cell[0][2].indxToObj = 4;		// E
	m_Cell[0][3].indxToObj = 17;		// R
	m_Cell[0][4].indxToObj = 19;		// T
	m_Cell[0][5].indxToObj = 24;		// Y
	m_Cell[0][6].indxToObj = 20;		// U
	m_Cell[0][7].indxToObj = 8;		// I
	m_Cell[0][8].indxToObj = 14;		// O
	m_Cell[0][9].indxToObj = 15;		// P

	/************************************************************************/
	// middle row of keys A - L
	m_Cell[1][0].indxToObj = 0;		// A
	m_Cell[1][1].indxToObj = 18;		// S
	m_Cell[1][2].indxToObj = 3;		// D
	m_Cell[1][3].indxToObj = 5;		// F
	m_Cell[1][4].indxToObj = 6;		// G
	m_Cell[1][5].indxToObj = 7;		// H
	m_Cell[1][6].indxToObj = 9;		// J
	m_Cell[1][7].indxToObj = 10;		// K
	m_Cell[1][8].indxToObj = 11;		// L
	m_Cell[1][9].indxToObj = -1;		// empty Cell;

	/************************************************************************/
	// bottom row of keys A - L
	m_Cell[2][0].indxToObj = 25;		// Z
	m_Cell[2][1].indxToObj = 23;		// X
	m_Cell[2][2].indxToObj = 2;		// C
	m_Cell[2][3].indxToObj = 21;		// V
	m_Cell[2][4].indxToObj = 1;		// B
	m_Cell[2][5].indxToObj = 13;		// N
	m_Cell[2][6].indxToObj = 12;		// M
	m_Cell[2][7].indxToObj = -1;		// empty
	m_Cell[2][8].indxToObj = -1;		// empty
	m_Cell[2][9].indxToObj = -1;		// empty Cell	


	//////////////////////////////////////////////////////////////////////////
	/************************************************************************/



}