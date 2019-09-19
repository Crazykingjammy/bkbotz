#include "../GameState.h"

CCharacterSelectState * CCharacterSelectState::pInstance = NULL;

CCharacterSelectState * CCharacterSelectState::GetInstance()
{
	if (!pInstance)
		pInstance = new CCharacterSelectState;
	return pInstance;
}


CCharacterSelectState::CCharacterSelectState()
{
	m_NumPlayers = 2;
	m_NumRows = 4;
	m_NumCols = 3;
	m_NumOfSelections = m_NumRows*m_NumCols;

	m_GridStartPos    = CVector3f(-30.0f, -10.0f, -30.0f);
	m_SpaceingX		  = 30.0f;
	m_SpaceingZ		  = 20.0f;
	m_SpaceingY		  = 0.0f;
	m_ModeLabel		  = NULL;
	m_LevelLabel	  = NULL;
	m_iCurrentModeDisplayed = 0;
	m_iCurrentState		= SELECTCHAR;
	m_bInitialized		= false;
	m_iYouAreIT			= 0;
	m_bShowHelp			= false;
	m_DisplayHelpTimer.Reset();
	m_iCurrentlySelecting = SELECTING_MODE;
	m_bAnimateArrows	= false;
	m_PickYourCharacterTimer.Reset();
	m_MaxTime = 30;		// max time is 30 seconds to choose char
	m_CurTime = m_MaxTime;		// count down from max to 0
	m_iArrowLabelIndex = 0;
	m_iCheckArrowLabelIndex = 0;
	m_iNoCharacterIconIndex = 0;
}

CCharacterSelectState::~CCharacterSelectState()
{

}

void CCharacterSelectState::Initalize(Game *game)
{
	//////////////////////////////////////////////////////////////////////////
	// well lets do standard game state stuff
	pGame = game;

	m_ModeSelectPosition.x = pGame->Window.WindowWidth/2.0f;
	m_ModeSelectPosition.y = (pGame->Window.WindowHeight/2.0f)-100.0f;

	m_CSCamera = new CFreeCamera;
	m_CSCamera->m_LinearSpeed = 1.50f;
	m_CSCamera->m_MyBody.centerPosition = CVector3f(0.0f, 100.0f, -100.0f);
	pRenderInterface->SetActiveCamera(m_CSCamera);

	//Set up Lights here
	pRenderInterface->Light[0].m_Active = true;
	pRenderInterface->Light[0].m_MyBody.centerPosition.y = 45.0f;
	pRenderInterface->Light[0].m_Specular = 0.7f;
	pRenderInterface->Light[0].m_Diffuse = EXColor(0.75179f,0.751575f,0.751685f,1.0f);
	pRenderInterface->Light[0].m_Ambient = EXColor(0.3f,0.3f,0.3f,1.0f);
	pRenderInterface->Light[0].m_Attenuation1 = 0.0435f;
	pRenderInterface->Light[0].m_Attenuation2 = 0.0f;

	pRenderInterface->Light[1].m_Active = true;
	pRenderInterface->Light[1].m_CastShadow = false;
	pRenderInterface->Light[1].m_MyBody.centerPosition.y = 45.0f;
	pRenderInterface->Light[1].m_Specular = 0.1217f;
	pRenderInterface->Light[1].m_Diffuse = EXColor(0.0f,0.0f,0.95f,1.0f);
	pRenderInterface->Light[1].m_Ambient = EXColor(0.0f,0.0f,0.0f,1.0f);
	pRenderInterface->Light[1].m_Attenuation1 = 0.0835f;
	pRenderInterface->Light[1].m_Attenuation2 = 0.0f;
	//pRenderInterface->Light[1].m_Type = SpotLight;

	pRenderInterface->Light[2].m_Active = true;
	pRenderInterface->Light[2].m_CastShadow = false;
	pRenderInterface->Light[2].m_MyBody.centerPosition.y = 45.0f;
	pRenderInterface->Light[2].m_Specular = 0.1217f;
	pRenderInterface->Light[2].m_Diffuse = EXColor(0.0f,0.95f,0.0f,1.0f);
	pRenderInterface->Light[2].m_Ambient = EXColor(0.0f,0.0f,0.0f,1.0f);
	pRenderInterface->Light[2].m_Attenuation1 = 0.0835f;
	pRenderInterface->Light[2].m_Attenuation2 = 0.0f;
	//pRenderInterface->Light[2].m_Type = SpotLight;

	pRenderInterface->Light[0].m_MyBody.radius = 5.0f;

	m_CSArena = new CArena("Models/CharacterSelectModels/CharSelworld.x");
	pRenderInterface->AddObject(m_CSArena);
	m_CSArena->m_DrawShadow = false;

	pRenderInterface->SetWorld("Models/CharacterSelectModels/CharSelGround.x");

	char buffer[128] = {0};

	// Create the characters "Stands" this will display a selectable or not selectable character
	m_pSelections = new CSelectOption*[m_NumCols];
	for (int i = 0; i < m_NumCols; i++)
	{
		m_pSelections[i] = new CSelectOption[m_NumRows];
	}
	// the player iterators
	m_Iterators = new CSelectIterator[m_NumPlayers];
	//////////////////////////////////////////////////////////////////////////
	
	//////////////////////////////////////////////////////////////////////////
	// Create All players display info
	m_IteratorIcons			= new CLabel[m_NumPlayers];				// icon for the players iterator
	m_SelectedLabel			= new CLabel[m_NumPlayers];				// for the name of the selection
	m_CharacterIconLabels	= new CLabel[m_NumPlayers];		// display character icon for each player
	//////////////////////////////////////////////////////////////////////////
	

	//////////////////////////////////////////////////////////////////////////
	// The Title Barhere we want a base behind what we want it to say
	m_Title		= new CLabel("Textures/CharacterSelect2D/Title.bmp");
	m_WhatTitleSay = new CLabel("Textures/font.tga");
	m_WhatTitleSay->m_FontWidth = 32;
	//////////////////////////////////////////////////////////////////////////
	
	// allocate the "IT" Icon
	m_ITIcon	= new CLabel("Textures/CharacterSelect2D/ITIcon.bmp");
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	// HELP STUFF
	// displays a marquee for which button to display the help
	m_HelpMarquee	= new CLabel("Textures/font.tga");		
	m_HelpMarquee->m_FontWidth = 32;
	m_HelpMarquee->m_Scale = 0.5f;
	// displays the help screen a big screen displaying help info
	m_HelpScreen	= new CLabel("Textures/CharacterSelect2D/HelpScreen.bmp");		
	m_HelpAcceptLabel = new CLabel("Textures/CharacterSelect2D/HelpAccept.bmp");
	m_HelpGoBackLabel = new CLabel("Textures/CharacterSelect2D/HelpGoBack.bmp");
	//////////////////////////////////////////////////////////////////////////
	

	//////////////////////////////////////////////////////////////////////////
	// Mode Select
	//////////////////////////////////////////////////////////////////////////
	// Initialize the Mode Select
	// Set up the labels that will display the current mode and level
	m_ModeLabel	= new CLabel("Textures/CharacterSelect2D/DefaultMode.bmp");
	m_LevelLabel = new CLabel("Textures/CharacterSelect2D/DefaultLevel.bmp");
	m_DoneLabel = new CLabel("Textures/CharacterSelect2D/Done.bmp");
	// arrows as itereators through mode or level select
	m_ArrowLabel = new CLabel("Textures/CharacterSelect2D/Arrows.bmp");
	//////////////////////////////////////////////////////////////////////////
	
	// display the time Label
	m_TimeLabel	= new CLabel("Textures/font.tga");
	m_TimeLabel->m_FontWidth = 32;
	m_TimeLabel->m_Scale = 2.0f;

	//////////////////////////////////////////////////////////////////////////
	// lets load the VS Label
	//m_VSLabel = new CLabel("Textures/CharacterSelect2D/VSLabel.bmp");
	m_VSLabel = new CLabel("Textures/IntroStuff/BKBLogo.tga");

	//////////////////////////////////////////////////////////////////////////
	// load the textures needed for each level and mode
	//////////////////////////////////////////////////////////////////////////
	// Set up the Mode Select
	m_Modes			= new CModeOption[NUMOFMODES];

	//////////////////////////////////////////////////////////////////////////
	// lets load the VS Label
	m_VSLabel->m_Visible = false;
	m_VSLabel->m_MyBody.centerPosition.x = pGame->Window.WindowWidth/2.0f;
	m_VSLabel->m_MyBody.centerPosition.y = (pGame->Window.WindowHeight/2.0f)-100.0f;
	pRenderInterface->AddLabel(m_VSLabel);


	for (int i = 0; i < m_NumPlayers; i++)
	{
		//////////////////////////////////////////////////////////////////////////
		// the name of the character
		strcpy(m_SelectedLabel[i].m_ModelName, "Textures/keysTexture32.bmp");
		m_SelectedLabel[i].m_MyBody.centerPosition.x = i * 300.0f;
		m_SelectedLabel[i].m_Scale *= 0.5f;
		pRenderInterface->AddLabel(&m_SelectedLabel[i]);
		//////////////////////////////////////////////////////////////////////////
		// set the label for the icon of the selected icon
		strcpy(m_CharacterIconLabels[i].m_ModelName, "Textures/CharacterSelect2D/CharacterSelectIcons/NoCharacterIcon.bmp");
		m_CharacterIconLabels[i].m_Visible = true;

		pRenderInterface->AddLabel(&m_CharacterIconLabels[i]);
		
		//////////////////////////////////////////////////////////////////////////
		// highlight the character icon with the color of the player blue or green
		sprintf(buffer, "Textures/CharacterSelect2D/CharacterSelectIcons/CharacterHighlight%d.bmp", i);
		// lets set up the icons :
		//		its texture and its position
		strcpy(m_IteratorIcons[i].m_ModelName, buffer);

		m_IteratorIcons[i].m_MyBody.centerPosition.x += 75.0f;
		m_IteratorIcons[i].m_MyBody.centerPosition.y = pGame->Window.WindowHeight/2.0f;
		m_IteratorIcons[i].m_MyBody.centerPosition.y += i * 100.0f;
		if ((i % 2))
		{
			m_IteratorIcons[i].m_MyBody.centerPosition.x = pGame->Window.WindowWidth-75.0f;
			m_IteratorIcons[i].m_MyBody.centerPosition.y = pGame->Window.WindowHeight/2.0f;
			m_IteratorIcons[i].m_MyBody.centerPosition.y += (i-1) * 100.0f;
		}

		pRenderInterface->AddLabel(&m_IteratorIcons[i]);
		
		m_CharacterIconLabels[i].m_MyBody.centerPosition = m_IteratorIcons[i].m_MyBody.centerPosition;
	}

	//////////////////////////////////////////////////////////////////////////
	// here we want a base behind what we want it to say

	m_Title->m_MyBody.centerPosition.x = pGame->Window.WindowWidth / 2.0f;
	m_Title->m_MyBody.centerPosition.y = 25.0f;
	pRenderInterface->AddLabel(m_Title);



	strcpy(m_WhatTitleSay->m_Text, "SELECT CHARACTER");
	m_WhatTitleSay->m_MyBody.centerPosition.x = (pGame->Window.WindowWidth / 2.0f)-((strlen(m_WhatTitleSay->m_Text)*m_WhatTitleSay->m_FontWidth)/2.0f);
	m_WhatTitleSay->m_MyBody.centerPosition.y += 10.0f;
	pRenderInterface->AddLabel(m_WhatTitleSay);

	// end title stuff
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	
	// Add the "IT" Icon
	// add it to the scene, must be after the iterator icon
	pRenderInterface->AddLabel(m_ITIcon);



	m_iCurrentModeDisplayed = 0;

	//////////////////////////////////////////////////////////////////////////

	// displays a marquee for which button to display the help
	m_HelpMarquee->m_Visible = true;
	m_HelpMarquee->m_MyBody.centerPosition.x = pGame->Window.WindowWidth/2.0f;
	m_HelpMarquee->m_MyBody.centerPosition.y = 100.0f;
	strcpy(m_HelpMarquee->m_Text, "Sprint to View Help");
	pRenderInterface->AddLabel(m_HelpMarquee);

	// displays the help screen a big screen displaying help info
	m_HelpScreen->m_MyBody.centerPosition.x = pGame->Window.WindowWidth/2.0f;
	m_HelpScreen->m_MyBody.centerPosition.y = pGame->Window.WindowHeight/2.0f;
	m_HelpScreen->m_Visible = m_bShowHelp;

	pRenderInterface->AddLabel(m_HelpScreen);

	//////////////////////////////////////////////////////////////////////////
	// Initialize the Mode Select
	// Set up the labels that will display the current mode and level
	m_ModeLabel->m_Visible = false;
	pRenderInterface->AddLabel(m_ModeLabel);

	m_LevelLabel->m_Visible = false;
	pRenderInterface->AddLabel(m_LevelLabel);

	m_DoneLabel->m_Visible = false;
	pRenderInterface->AddLabel(m_DoneLabel);

	// arrows as itereators through mode or level select
	m_ArrowLabel->m_Visible = false;
	pRenderInterface->AddLabel(m_ArrowLabel);

	// Time
	m_TimeLabel->m_Visible = true;
	m_TimeLabel->m_MyBody.centerPosition.x = pGame->Window.WindowWidth/2.0f;
	m_TimeLabel->m_MyBody.centerPosition.y = pGame->Window.WindowHeight - 100.0f;
	pRenderInterface->AddLabel(m_TimeLabel);


	// more Help Stuff

	m_HelpAcceptLabel->m_Visible = true;
	m_HelpAcceptLabel->m_Scale *= 0.5f;
	m_HelpAcceptLabel->m_MyBody.centerPosition.y = (float)pGame->Window.WindowHeight;
	//m_HelpAcceptLabel->m_MyBody.centerPosition.x = 100.0f;
	pRenderInterface->AddLabel(m_HelpAcceptLabel);
	m_HelpGoBackLabel->m_Visible = true;
	m_HelpGoBackLabel->m_MyBody.centerPosition.y = (float)pGame->Window.WindowHeight;
	//m_HelpAcceptLabel->m_MyBody.centerPosition.x = pGame->Window.WindowWidth-100.0f;
	m_HelpGoBackLabel->m_Scale *= 0.5f;
	pRenderInterface->AddLabel(m_HelpGoBackLabel);



	// testing
	//////////////////////////////////////////////////////////////////////////
	// 
	// Create the characters "Stands" this will display a selectable or not selectable character
	int iD = 0;
	for (int i = 0; i < m_NumCols; i++)
	{
		for (int j = 0; j < m_NumRows; j++)
		{
			// set the stands
			strcpy(m_pSelections[i][j].m_ModelName, "Models/CharacterSelectModels/CharSelStand.x");
			sprintf(buffer, "Stand %d", i);
			strcpy(m_pSelections[i][j].m_MyBody.myName, buffer);
			m_pSelections[i][j].m_MyBody.SetMass(180.0f);
			m_pSelections[i][j].m_MyBody.radius = 2.0f;
			m_pSelections[i][j].m_MyBody.dragCoefficient = m_pSelections[i][j].m_MyBody.GetMass();
			m_pSelections[i][j].m_MyBody.isMovableByCollision = false;
			m_pSelections[i][j].SetSelectionID(iD++);
			m_pSelections[i][j].m_isSelectable = false;
			m_pSelections[i][j].m_MyBodyIndex = m_Physics.AddBody(&m_pSelections[i][j].m_MyBody);
			pRenderInterface->AddObject(&m_pSelections[i][j]);	
		}
	}

	InitCharSelect();
	// 
	//////////////////////////////////////////////////////////////////////////





	//////////////////////////////////////////////////////////////////////////
	// create the scene needed for the renderer

	pRenderInterface->CreateScene();

	//////////////////////////////////////////////////////////////////////////


    
	//////////////////////////////////////////////////////////////////////////
	// Set the position of the 2 help labels

	m_HelpAcceptLabel->m_MyBody.centerPosition.x += m_HelpAcceptLabel->m_Rect.right/2.0f;
	m_HelpAcceptLabel->m_MyBody.centerPosition.y -= m_HelpAcceptLabel->m_Rect.bottom/2.0f;

	m_HelpGoBackLabel->m_MyBody.centerPosition.x = (float) (pGame->Window.WindowWidth - m_HelpGoBackLabel->m_Rect.right/2.0f);
	m_HelpGoBackLabel->m_MyBody.centerPosition.y -= (float)(m_HelpGoBackLabel->m_Rect.bottom/2);

	m_TimeLabel->m_MyBody.centerPosition.x -= 64.0f;
	//////////////////////////////////////////////////////////////////////////
	
	for (int i = 0; i < m_NumPlayers; i++)
	{
		m_iNoCharacterIconIndex = m_CharacterIconLabels[i].ModelIndex;
	}
	
	// indexes for the arrows, proper indecies are found only after a create scene
	m_iArrowLabelIndex = m_ArrowLabel->ModelIndex;
	m_iCheckArrowLabelIndex = pRenderInterface->LoadTexture("Textures/CharacterSelect2D/CheckArrows.bmp");

	m_iVSBackground = pRenderInterface->LoadTexture("Textures/CharacterSelect2D/VSLabel.bmp");
	
	//////////////////////////////////////////////////////////////////////////
	// okay here is where we position the labes for mode, level, and play
	// setting the position is based on the rect of the bmp
	// this rect only get set after createScene()
	m_ModeLabel->m_MyBody.centerPosition = m_ModeSelectPosition;
	//m_ModeLabel->m_MyBody.centerPosition.y -= 100.0f;		// the original starts in the middle, lets raise it a bit
	m_LevelLabel->m_MyBody.centerPosition = m_ModeLabel->m_MyBody.centerPosition;
	m_LevelLabel->m_MyBody.centerPosition.y += m_ModeLabel->m_Rect.bottom;	// have to add by the height of the bitmap
	m_DoneLabel->m_MyBody.centerPosition = m_LevelLabel->m_MyBody.centerPosition;
	m_DoneLabel->m_MyBody.centerPosition.y += m_LevelLabel->m_Rect.bottom;

	//////////////////////////////////////////////////////////////////////////
	// load the textures needed for each level and mode
	//////////////////////////////////////////////////////////////////////////
	// Set up the Mode Select

	m_Modes[0].ModelIndex = pRenderInterface->LoadTexture("Textures/CharacterSelect2D/Mode0.bmp");
	// lets name each mode
	m_Modes[0].SetModeName("BATTLE");
	m_Modes[0].AddLevelToMode("EGYTP")->ModelIndex = pRenderInterface->LoadTexture("Textures/CharacterSelect2D/Mode0Level0.bmp");
	m_Modes[0].AddLevelToMode("HITECH")->ModelIndex = pRenderInterface->LoadTexture("Textures/CharacterSelect2D/Mode0Level1.bmp");
	m_Modes[0].AddLevelToMode("HELL")->ModelIndex = pRenderInterface->LoadTexture("Textures/CharacterSelect2D/Mode0Level2.bmp");
	m_Modes[0].AddLevelToMode("BEACH")->ModelIndex = pRenderInterface->LoadTexture("Textures/CharacterSelect2D/Mode0Level3.bmp");
	
	m_Modes[1].ModelIndex = pRenderInterface->LoadTexture("Textures/CharacterSelect2D/Mode1.bmp");
	// lets name each mode
	m_Modes[1].SetModeName("SOCCER");
	m_Modes[1].AddLevelToMode("STADIUM")->ModelIndex = pRenderInterface->LoadTexture("Textures/CharacterSelect2D/Mode1Level0.bmp");
	m_Modes[2].ModelIndex = pRenderInterface->LoadTexture("Textures/CharacterSelect2D/Mode2.bmp");
	
	for (int i = 0; i < m_NumCols; i++)
	{
		for (int j = 0; j < m_NumRows; j++)
		{
			switch(m_pSelections[i][j].GetSelectionID())
			{
			case pGameInfo->SFINKS:
				m_pSelections[i][j].m_isSelectable = true;
				//m_pSelections[i][j].m_MyBody.radius = 1.0f;
				strcpy(m_pSelections[i][j].m_MyBody.myName, "Sfinx Bot");
				m_pSelections[i][j].ModelIndex = pRenderInterface->LoadModel("Models/balls/jewl.x");
	
				m_pSelections[i][j].m_IconLabelIndex = pRenderInterface->LoadTexture("Textures/CharacterSelect2D/CharacterSelectIcons/SphinxIcon.bmp");
				break;
			case pGameInfo->GREENBOT:
				m_pSelections[i][j].m_isSelectable = true;
				//m_pSelections[i][j].m_MyBody.radius = 1.0f;
				strcpy(m_pSelections[i][j].m_MyBody.myName, "Green Bot");
				m_pSelections[i][j].ModelIndex = pRenderInterface->LoadModel("Models/balls/ruby.x");
				m_pSelections[i][j].m_IconLabelIndex = pRenderInterface->LoadTexture("Textures/CharacterSelect2D/CharacterSelectIcons/GoobIcon.bmp");
				break;
			case pGameInfo->BLUEBOT:
				m_pSelections[i][j].m_isSelectable = true;
				//m_pSelections[i][j].m_MyBody.radius = 1.0f;
				strcpy(m_pSelections[i][j].m_MyBody.myName, "Blue Bot");
				m_pSelections[i][j].ModelIndex = pRenderInterface->LoadModel("Models/balls/rubyblue.x");
				m_pSelections[i][j].m_IconLabelIndex = pRenderInterface->LoadTexture("Textures/CharacterSelect2D/CharacterSelectIcons/BlueIcon.bmp");
				break;
			case pGameInfo->TBOT:
				m_pSelections[i][j].m_isSelectable = true;
				//m_pSelections[i][j].m_MyBody.radius = 1.0f;
				strcpy(m_pSelections[i][j].m_MyBody.myName, "T Bot");
				m_pSelections[i][j].ModelIndex = pRenderInterface->LoadModel("Models/tball.x");
				m_pSelections[i][j].m_IconLabelIndex = pRenderInterface->LoadTexture("Textures/CharacterSelect2D/CharacterSelectIcons/TBotIcon.bmp");
				break;
			case pGameInfo->DEATHBOT:
				m_pSelections[i][j].m_isSelectable = true;
				//m_pSelections[i][j].m_MyBody.radius = 7.0f;
				strcpy(m_pSelections[i][j].m_MyBody.myName, "Death Bot");
				m_pSelections[i][j].ModelIndex = pRenderInterface->LoadModel("Models/balls/skull.x");
				m_pSelections[i][j].m_IconLabelIndex = pRenderInterface->LoadTexture("Textures/CharacterSelect2D/CharacterSelectIcons/Deathbot.bmp");
				break;

			case pGameInfo->PFROST:
				m_pSelections[i][j].m_isSelectable = true;
				//m_pSelections[i][j].m_MyBody.radius = 7.0f;
				strcpy(m_pSelections[i][j].m_MyBody.myName, "Pfrost");
				m_pSelections[i][j].ModelIndex = pRenderInterface->LoadModel("Models/balls/snowball.x");
				m_pSelections[i][j].m_IconLabelIndex = pRenderInterface->LoadTexture("Textures/CharacterSelect2D/CharacterSelectIcons/PissedMrFrosty.bmp");
				break;

			case pGameInfo->KENNY:
				m_pSelections[i][j].m_isSelectable = true;
				//m_pSelections[i][j].m_MyBody.radius = 7.0f;
				strcpy(m_pSelections[i][j].m_MyBody.myName, "Kenny");
				m_pSelections[i][j].ModelIndex = pRenderInterface->LoadModel("Models/botz/kennyhead.x");
				m_pSelections[i][j].m_IconLabelIndex = pRenderInterface->LoadTexture("Textures/CharacterSelect2D/CharacterSelectIcons/KennyIcon.bmp");
				break;


			default:
				break;
			}
		}
	}

	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	// load sounds
	SFXCollidedAndNotSelectable = pSound->AddSFX("Audio/SFX/Character_Select_Denied.wav");
	SFXCollidedAndSelectable    = pSound->AddSFX("Audio/SFX/menu_select.wav");
	SFXPlayerReady				= pSound->AddSFX("Audio/SFX/Pause.wav");
	SFXPlayersCollided			= pSound->AddSFX("Audio/SFX/ball_hit_player.wav");
	SFXScrollMode				= pSound->AddSFX("Audio/SFX/menu_scrollover.wav");
	SFXSelectedAndDone			= pSound->AddSFX("Audio/SFX/menu_select.wav");

	SFXVSClash					= pSound->AddSFX("Audio/SFX/VSClash.wav");
	BGMusic						= pSound->AddMusic("Audio/Music/CharacterSelectMusic.mp3");
	BGVSMusic					= pSound->AddMusic("Audio/Music/VSMusic.mp3");

	pSound->ChangeMusicTrack(BGMusic);

	// end loading sounds
	//////////////////////////////////////////////////////////////////////////
	
	m_Physics.CreateWorld(75.0f, 100.0f, 75.0f);
	m_bInitialized = true;
}

void CCharacterSelectState::InitIterators()
{
	srand(0);
	char buffer[128] = {0};
	int whoIsIt = rand() % (m_NumPlayers-1);

	for (int i = 0; i < m_NumPlayers; i++)
	{
		sprintf(buffer, "Models/CharacterSelectModels/Iterator%d.x", i);
		strcpy(m_Iterators[i].m_ModelName, buffer);
		sprintf(buffer, "Player %d", i);
		strcpy(m_Iterators[i].m_MyBody.myName, buffer);
		sprintf(buffer, "Models/CharacterSelectModels/Iterator%dIcon.x", i);
		strcpy(m_Iterators[i].m_Barrell->m_ModelName, buffer);

		m_Iterators[i].m_MyBody.centerPosition.x = (i * 20.0f)-10;
		m_Iterators[i].m_MyBody.SetMass(100.0f);
		m_Iterators[i].m_fRegular = 30000.0f;
		m_Iterators[i].m_MyBody.dragCoefficient = 240.0f;
		m_Iterators[i].m_MyBody.radius = 3.0f;
		m_Iterators[i].m_Barrell->m_MyBody.radius = 4.0f;
		m_Iterators[i].m_Barrell->m_Visible = false;
		m_Iterators[i].m_MyBodyIndex = m_Physics.AddBody(&m_Iterators[i].m_MyBody);
		pRenderInterface->AddObject(&m_Iterators[i]);
		pRenderInterface->AddObject(m_Iterators[i].m_Barrell);
	}
	m_Iterators[whoIsIt].m_isIT = true;
}

bool CCharacterSelectState::InitCharSelect()
{
	InitIterators();

	for (int i = 0; i < m_NumCols; i++)
	{
		for (int j = 0; j < m_NumRows; j++)
		{
			m_pSelections[i][j].m_MyBody.centerPosition.x = m_GridStartPos.x;
			m_pSelections[i][j].m_MyBody.centerPosition.y = m_GridStartPos.y;
			m_pSelections[i][j].m_MyBody.centerPosition.z = m_GridStartPos.z;

			m_pSelections[i][j].m_MyBody.centerPosition.x += i * m_SpaceingX;
			//m_pSelections[i][j].m_MyBody.centerPosition.y += i * m_SpaceingY;
			m_pSelections[i][j].m_MyBody.centerPosition.z += j * m_SpaceingZ;
		}
	}

	return true;
}

void CCharacterSelectState::Shutdown()
{
	delete m_CSCamera;
	// destroy the arena
	delete m_CSArena;
	// all the possible selections
	for (int i = 0; i < m_NumCols; i++)
	{
		delete [] m_pSelections[i];
	}
	delete [] m_pSelections;

	// the labels for the selected character for each player
	delete [] m_IteratorIcons;
	delete [] m_SelectedLabel;
	delete [] m_CharacterIconLabels;

	delete m_Title;
	delete m_ITIcon;


	// the player iterators
	delete [] m_Iterators;
	
	delete m_HelpMarquee;

	delete m_HelpScreen;


	delete m_ModeLabel;
	delete m_LevelLabel;
	delete m_DoneLabel;
	delete m_ArrowLabel;
	delete m_TimeLabel;

	delete m_WhatTitleSay;

	delete m_HelpAcceptLabel;
	delete m_HelpGoBackLabel;
	delete m_VSLabel;

	for (int i = 0; i < NUMOFMODES; i++)
	{
		m_Modes[i].ClearLevelList();
	}
	delete [] m_Modes;

	// clear the modules
	m_Physics.ClearScene();
	pRenderInterface->DestroyScene();

	DeleteInstance();
}

void CCharacterSelectState::Update(float timeDelta)
{
	pInput->SetAllStates();

	if (!m_bInitialized)
		return;
	
	if (pInput->KeyboardKeyPress(DIK_ESCAPE))
	{
		m_iCurrentState = BACK_TO_MAIN_MENU;
	}
	// ahh this is for every player
	for (int i = 0; i < 2; i++)
	{
		if (pInput->ControllerJoyPress(i, pGameInfo->ButtonLayout[i]._block))
		{
			m_iCurrentState = BACK_TO_MAIN_MENU;
		}
	}

	switch(m_iCurrentState)
	{
	case SELECTCHAR:
		Run();
		break;
	case SELECTMODE:
		DoModeSelect();
		break;
	case SELECTEDMODE:
		ModeSelected();
		break;
	case BACK_TO_MAIN_MENU:
		{
			ChangeState(pGame, CMenuState::GetInstance());
			return;
		}
		break;
	default:
		break;
	}
}


void CCharacterSelectState::Run()
{
	m_Physics.Simulate();

	CheckControls();

	Collision();

	m_CSCamera->LookAt(pRenderInterface->m_World);

	Display2DInfo();

	//Stabelize lights
	StabelizeLights();

	if (IsEveryOneReady())
	{
		GoToSelectModeAndLevel();
	}
	UpdateTime();
}

void CCharacterSelectState::StabelizeLights()
{
	for(int i = 0; i < m_NumPlayers; i++)
	{
		if(m_Iterators[i].m_LightStablizeTimer.GetElapsedSeconds() > m_Iterators[i].m_LightStablizeFreq)
		{
			if( (i%2) == 0 )
			{
				pRenderInterface->Light[1].m_Diffuse.b += -0.95f;

				if(pRenderInterface->Light[1].m_Diffuse.b < 4.95f)
				{
					pRenderInterface->Light[1].m_Diffuse.b = 3.95f;
					pRenderInterface->Light[1].m_CastShadow = false;
				}
			}
			else
			{
				pRenderInterface->Light[2].m_Diffuse.g += -0.95f;

				if(pRenderInterface->Light[2].m_Diffuse.g < 4.95f)
				{
					pRenderInterface->Light[2].m_Diffuse.g = 3.95f;
					pRenderInterface->Light[2].m_CastShadow = false;
				}
			}
	
			m_Iterators[i].m_LightStablizeTimer.Reset();
		}
	}
}

void CCharacterSelectState::Display2DInfo()
{
	char buffer[128] = {0};
	for (int i = 0; i < m_NumPlayers; i++)
	{
		strcpy(m_SelectedLabel[i].m_Text, m_Iterators[i].m_CurrentSelectedOption);
		m_SelectedLabel[i].m_MyBody.centerPosition = m_IteratorIcons[i].m_MyBody.centerPosition;
		m_SelectedLabel[i].m_MyBody.centerPosition.x -= m_IteratorIcons[i].m_Rect.right/2;
		m_SelectedLabel[i].m_MyBody.centerPosition.y += m_IteratorIcons[i].m_Rect.bottom/2;

		if (m_Iterators[i].m_isIT)
		{
			m_ITIcon->m_MyBody.centerPosition = m_IteratorIcons[i].m_MyBody.centerPosition;
			m_ITIcon->m_MyBody.centerPosition.x -= m_IteratorIcons[i].m_Rect.right/2;
			m_ITIcon->m_MyBody.centerPosition.y -= m_IteratorIcons[i].m_Rect.bottom/2;
		}
		if (m_Iterators[i].m_iSelectionIDIconIndex == -1)
			m_CharacterIconLabels[i].ModelIndex = m_iNoCharacterIconIndex;
		else
            m_CharacterIconLabels[i].ModelIndex = m_Iterators[i].m_iSelectionIDIconIndex;
	}
	// lets move the marquee and wrap the screen
	m_HelpMarquee->m_MyBody.centerPosition.x -= 1.0f;
	if ((m_HelpMarquee->m_MyBody.centerPosition.x+(strlen(m_HelpMarquee->m_Text)*m_HelpMarquee->m_FontWidth)) <= 0.0f)
		m_HelpMarquee->m_MyBody.centerPosition.x = pGame->Window.WindowWidth+256.0f;
}

void CCharacterSelectState::DoModeSelect()
{
	//m_DisplayedModeLabel->m_Visible = true;
	
	m_TimeLabel->m_Visible = false;
	m_HelpMarquee->m_Visible = false;
	m_ModeLabel->m_Visible = true;
	m_LevelLabel->m_Visible = true;
	m_DoneLabel->m_Visible = true;
	m_ArrowLabel->m_Visible = true;
	
	if (m_Timer.GetElapsedSeconds() > .12f)
	{
		ModeSelectControls(m_iYouAreIT);
		m_Timer.Reset();
	}

	UpdateArrows();
	//m_DisplayedModeLabel->ModelIndex = m_ModeLabels[m_iCurrentModeDisplayed].ModelIndex;
	m_ModeLabel->ModelIndex = m_Modes[m_iCurrentModeDisplayed].ModelIndex;
	m_LevelLabel->ModelIndex= m_Modes[m_iCurrentModeDisplayed].GetCurrentLevelLabelIndex();
}

void CCharacterSelectState::ModeSelectControls(int iPlayer)
{

	// Up will update the currentSelecting option and the arrows
	if (pInput->ControllerJoystickUp(iPlayer))
	{
		--m_iCurrentlySelecting;
		if (m_iCurrentlySelecting < SELECTING_MODE)
			m_iCurrentlySelecting = SELECTING_MODE;
		pSound->PlayQSFX(SFXScrollMode);
	}
	if (pInput->ControllerJoystickDown(iPlayer))
	{
		++m_iCurrentlySelecting;
		if (m_iCurrentlySelecting > SELECTING_MODE+NUMBER_OF_SELECTIONS-1)
			m_iCurrentlySelecting = SELECTING_MODE+NUMBER_OF_SELECTIONS-1;
	
		pSound->PlayQSFX(SFXScrollMode);
	}

	switch(m_iCurrentlySelecting)
	{
	case SELECTING_MODE:
		{
			strcpy(m_WhatTitleSay->m_Text, "Select Mode");
			m_WhatTitleSay->m_MyBody.centerPosition.x = (pGame->Window.WindowWidth / 2.0f)-((strlen(m_WhatTitleSay->m_Text)*m_WhatTitleSay->m_FontWidth)/2.0f);

			if (pInput->ControllerJoystickRight(iPlayer))
			{
				m_iCurrentModeDisplayed++;
				pSound->PlayQSFX(SFXScrollMode);
			}
			if (pInput->ControllerJoystickLeft(iPlayer))
			{
				m_iCurrentModeDisplayed--;
				pSound->PlayQSFX(SFXScrollMode);
			}

			if (m_iCurrentModeDisplayed >= NUMOFMODES)
				m_iCurrentModeDisplayed = 0;
			if (m_iCurrentModeDisplayed < 0)
				m_iCurrentModeDisplayed = NUMOFMODES-1;
		}
		break;
	case SELECTING_LEVEL:
		{
			strcpy(m_WhatTitleSay->m_Text, "Select Level");
			m_WhatTitleSay->m_MyBody.centerPosition.x = (pGame->Window.WindowWidth / 2.0f)-((strlen(m_WhatTitleSay->m_Text)*m_WhatTitleSay->m_FontWidth)/2.0f);

			if (pInput->ControllerJoystickRight(iPlayer))
			{
				m_Modes[m_iCurrentModeDisplayed].IncrementLevelList();
				pSound->PlayQSFX(SFXScrollMode);	
			}
			if (pInput->ControllerJoystickLeft(iPlayer))
			{
				m_Modes[m_iCurrentModeDisplayed].DecrementLevelList();
				pSound->PlayQSFX(SFXScrollMode);
			}
		}
		break;
	case SELECTING_DONE:
		{
			strcpy(m_WhatTitleSay->m_Text, "Start Playing!");
			m_WhatTitleSay->m_MyBody.centerPosition.x = (pGame->Window.WindowWidth / 2.0f)-((strlen(m_WhatTitleSay->m_Text)*m_WhatTitleSay->m_FontWidth)/2.0f);

			if (pInput->ControllerJoyPress(iPlayer, pGameInfo->ButtonLayout[iPlayer]._shoot))
			{
				pSound->PlayQSFX(SFXSelectedAndDone);
				m_iCurrentState = SELECTEDMODE;
				m_ReadyToChangeStateTimer.Reset();
			}
		}
		break;
	default:
		break;
	};

}

void CCharacterSelectState::ModeSelected()
{
	pSound->ChangeMusicTrack(BGVSMusic);

	SendDataToProfile();
	
	m_ModeLabel->m_Visible = false;
	m_LevelLabel->m_Visible = false;
	m_DoneLabel->m_Visible = false;
	m_ArrowLabel->m_Visible = true;
	m_VSLabel->m_Visible = true;
	m_HelpAcceptLabel->m_Visible = false;
	m_HelpGoBackLabel->m_Visible = false;
	m_Title->m_Visible = false;
	m_WhatTitleSay->m_Visible = false;
	m_ITIcon->m_Visible = false;



	CVector3f centerpoint(pGame->Window.WindowWidth/2.0f, pGame->Window.WindowHeight/2.0f, 0.0f);
	CVector3f movVec;
	
	if (m_ReadyToChangeStateTimer.GetElapsedSeconds() < 3.0f)
	{
		for (int i = 0; i < m_NumPlayers; i++)
		{
			movVec = centerpoint - m_IteratorIcons[i].m_MyBody.centerPosition;
			movVec.Normalize();

			m_IteratorIcons[i].m_MyBody.centerPosition += movVec;
		}
		//UpdateArrows();
		UpdateVSBackground();

		m_ArrowLabel->ModelIndex = m_iVSBackground;
		m_ArrowLabel->m_MyBody.centerPosition.x = pGame->Window.WindowWidth/2.0f;
		m_ArrowLabel->m_MyBody.centerPosition.y = pGame->Window.WindowHeight/2.0f;

		UpdateCharacterIcons();
		pSound->PlayQSFX(SFXVSClash);
	}
	else
	{
		pSound->ChangingState();
		switch (m_iCurrentModeDisplayed)
		{
		case BATTLEMODE:
			ChangeState(pGame, CBattleGameState::GetInstance());
			return;
			break;
		case SOCCERMODE:
			ChangeState(pGame, CBattleGameState::GetInstance());
			return;
			break;
		//case POOLMODE:
		//	ChangeState(pGame, CBattleGameState::GetInstance());
		//	return;	
		//	break;
		default:
			ChangeState(pGame, CBattleGameState::GetInstance());
			return;
			break;
		}
		m_ReadyToChangeStateTimer.Reset();
	}
}



void CCharacterSelectState::SendDataToProfile()
{
	int *ids = new int[m_NumPlayers];
	for(int i = 0; i < m_NumPlayers; i++)
	{
		ids[i] = m_Iterators[i].m_SelectedID;
	}
	pGameInfo->SetPlayerIndices(ids, m_NumPlayers);
	pGameInfo->SetLevelIndexToLoad(m_Modes[m_iCurrentModeDisplayed].GetCurrentLevel());
	delete [] ids;
}

bool CCharacterSelectState::IsEveryOneReady()
{
	int totalReady = 0;
	
	for (int i = 0; i < m_NumPlayers; i++)
	{
		if (m_Iterators[i].m_isReady)
			totalReady++;
	}
	if (totalReady >= m_NumPlayers)
		return true;

	return false;

}

void CCharacterSelectState::DeleteInstance()
{
	if (pInstance)
		delete pInstance;
	pInstance = NULL;
}

void CCharacterSelectState::UpdateArrows()
{
	switch(m_iCurrentlySelecting)
	{
	case SELECTING_MODE:
		{
			m_ArrowLabel->ModelIndex = m_iArrowLabelIndex;
			m_ArrowLabel->m_MyBody.centerPosition = m_ModeLabel->m_MyBody.centerPosition;
		}
		break;
	case SELECTING_LEVEL:
		{
			m_ArrowLabel->ModelIndex = m_iArrowLabelIndex;
			m_ArrowLabel->m_MyBody.centerPosition = m_LevelLabel->m_MyBody.centerPosition;
		}
		break;
	case SELECTING_DONE:
		{
			m_ArrowLabel->ModelIndex = m_iCheckArrowLabelIndex;
			m_ArrowLabel->m_MyBody.centerPosition = m_DoneLabel->m_MyBody.centerPosition;
		}
		break;
	default:
		break;
	};
	if (m_iCurrentlySelecting == SELECTING_MODE || m_Modes[m_iCurrentModeDisplayed].GetNumberOfLevels() > 1)
	{
		m_bAnimateArrows = true;
	}
	else
	{
		m_bAnimateArrows = false;
	}
	// yeah yeah i know, static bool!  ill clean it up later, i just need it working
	// btw its only for animation big deal
	static bool bSmallToBig = true;
	static CPhysicsTimer changeSizeTimer;

	if (m_bAnimateArrows)
	{
		if (bSmallToBig)
			m_ArrowLabel->m_Scale += 0.001f;
		else
			m_ArrowLabel->m_Scale -= 0.001f;

		if (changeSizeTimer.GetElapsedSeconds() > 0.5f)
		{
			bSmallToBig = !bSmallToBig;
			changeSizeTimer.Reset();
		}
	}
}

void CCharacterSelectState::UpdateVSBackground()
{
	static bool bSmallToBig = true;
	static CPhysicsTimer changeSizeTimer;

	if (bSmallToBig)
		m_VSLabel->m_Scale.x += 0.01f;
	else
		m_VSLabel->m_Scale.x -= 0.01f;

	if (changeSizeTimer.GetElapsedSeconds() > 0.5f)
	{
		bSmallToBig = !bSmallToBig;
		changeSizeTimer.Reset();
	}
}
void CCharacterSelectState::UpdateTime()
{
	static char buffer[128];

	if (m_PickYourCharacterTimer.GetElapsedSeconds() > 1.0f)
	{
		m_CurTime--;
		m_PickYourCharacterTimer.Reset();
	}
	if (m_CurTime <= 0)
	{
		m_CurTime = 0;
		for (int i = 0; i < m_NumPlayers; i++)
		{
			if (m_Iterators[i].m_SelectedID == -1)
			{
				m_Iterators[i].m_SelectedID = 0;
			}
		}
		GoToSelectModeAndLevel();
	}
	sprintf(buffer, "%d", m_CurTime);
	strcpy(m_TimeLabel->m_Text, buffer);
}

void CCharacterSelectState::GoToSelectModeAndLevel()
{
	for (int i = 0; i < m_NumPlayers; i++)
	{
		if (m_Iterators[i].m_isIT)
		{
			m_iYouAreIT = i;
			break;
		}
	}
	m_iCurrentState = SELECTMODE;
	m_iCurrentlySelecting = SELECTING_MODE;
}

void CCharacterSelectState::UpdateCharacterIcons()
{

	for (int i = 0; i < m_NumPlayers; i++)
	{
		m_CharacterIconLabels[i].m_MyBody.centerPosition = m_IteratorIcons[i].m_MyBody.centerPosition;
		m_SelectedLabel[i].m_MyBody.centerPosition = m_IteratorIcons[i].m_MyBody.centerPosition;
		m_SelectedLabel[i].m_MyBody.centerPosition.x -= m_IteratorIcons[i].m_Rect.right/2;
		m_SelectedLabel[i].m_MyBody.centerPosition.y += m_IteratorIcons[i].m_Rect.bottom/2;
		if (m_Iterators[i].m_isIT)
		{
			m_ITIcon->m_MyBody.centerPosition = m_IteratorIcons[i].m_MyBody.centerPosition;
			m_ITIcon->m_MyBody.centerPosition.x -= m_IteratorIcons[i].m_Rect.right/2;
			m_ITIcon->m_MyBody.centerPosition.y -= m_IteratorIcons[i].m_Rect.bottom/2;
		}
	}
}