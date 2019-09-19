#include ".\menusystem.h"
CMenuSystem *CMenuSystem::pInst = 0;
CMenuSystem::CMenuSystem(void)
{
	firstinit = true;
	incredits = false;
}

CMenuSystem::~CMenuSystem(void)
{
}

bool	CMenuSystem::Initialize()
{
	//this->xml = NULL;
	//CoInitialize(NULL);
	//CoCreateInstance(__uuidof(SAXXMLReader40), NULL, CLSCTX_ALL, __uuidof(ISAXXMLReader), (void **)&xml);
	//xml->putContentHandler(&Script);
	//wchar_t tmp[MAX_STRING_LENGTH];
	//mbstowcs(tmp, "./source/menu/menu.xml", MAX_STRING_LENGTH);
	//xml->parseURL(tmp);

	//CurrentMenu = 0;

	InitializeMainMenu();
	InitializeAudioMenu();
	Creditsbackground = new CLabel("./content/2d/menu/creditsbkg.tga");
	Creditsbackground->m_Visible = false;
	Creditsbackground->m_MyBody.centerPosition = CVector3f(400,300,0);
	CRenderInterface::GetInstance()->AddLabel(Creditsbackground);
	CreditsInitialize();

	RunOnceDuringUpdate = true;
	this->Status = true;
	pInput = InputManager::GetInstance();
	menuCamera = new CFreeCamera;
	menuCamera->m_LinearSpeed = 1.0f;
	menuCamera->m_MyBody.centerPosition.z -= 100.0f;
	menuCamera->m_MyBody.centerPosition.y += 23.0f;
	CRenderInterface::GetInstance()->Light[0].m_Active = true;
	CRenderInterface::GetInstance()->Light[0].m_MyBody.centerPosition = CVector3f(70.0f,100.0f,-20.0f);;
	CRenderInterface::GetInstance()->Light[0].m_Specular = 0.8f;
	CRenderInterface::GetInstance()->Light[0].m_Diffuse = EXColor(1.96f,1.865f,0.965f,1.0f);
	CRenderInterface::GetInstance()->Light[0].m_Ambient = EXColor(0.6f,0.65f,0.65f,1.0f);
	CRenderInterface::GetInstance()->Light[0].m_Attenuation1 = 0.008543f;
	CRenderInterface::GetInstance()->Light[0].m_Attenuation2 = 0.0f;
	CRenderInterface::GetInstance()->SetActiveCamera(menuCamera);
	CRenderInterface::GetInstance()->SetWorld("./models/tree.x");
	CRenderInterface::GetInstance()->CreateScene();
	pSound = CSoundSystem::GetInstance();
	SFXcycleButtons = pSound->AddSFX("Audio/SFX/menu_scrollover.wav");
	SFXactiveButton = pSound->AddSFX("Audio/SFX/menu_select.wav");
	//MUSIC			= pSound->AddMusic();
	SKIPTHEMOTHERFUCKINGINTRO = true;
	
	if(firstinit == false)
	{	
		Catagory5logo->m_Visible = false;
		LockingKeylogo->m_Visible = false;
	}
	else
	{
		firstinit = false;
	}

	ResetAllSubmenus();

		pSound->ChangeMusicTrack(pSound->AddMusic("Audio/music/HighTech.mp3"));

	return true;
}
int		CMenuSystem::Update(int MenuToUse)
{
	if(RunOnceDuringUpdate)
	{
		AnimationTimer.Reset();
		InputTimer.Reset();
		DrawTimer.Reset();
		RunOnceDuringUpdate = false;
		
		MainMenu.CurrentMenuButton = &MainMenu.ButtonMasterIndex[0];
		Draw(MenuToUse);
		MainMenuUpdate();
		Cat5Timer.Reset();
	}
	int ReturnValue = 0;

	if(incredits == false)
	{
		switch(MenuToUse)
		{
		case 0:
			ShutOffAudioMenu();
			ReturnValue = MainMenuUpdate();
			break;
		case 1:
			ShutOffMainMenu();
			ReturnValue = AudioMenuUpdate();
			break;
		}
		Draw(MenuToUse);
	}
	else
		CreditsSequence();
	if(ReturnValue == -1)
		return 0;
	else
		return ReturnValue;
}
bool	CMenuSystem::Shutdown()
{
	return true;
}

void	CMenuSystem::Draw(int MenuToDraw)
{
	if(DrawTimer.GetElapsedSeconds() >= 0.005)
	{
		DrawTimer.Reset();
		if(Cat5Timer.GetElapsedSeconds() <= 3.0 && Catagory5logo->m_Visible == true)
		{
			if(SKIPTHEMOTHERFUCKINGINTRO == true )
			{
				Catagory5logo->m_Visible = false;
			}
			LogoTimer.Reset();
		}
		else if(Cat5Timer.GetElapsedSeconds() >= 3.0 && Catagory5logo->m_Visible == true)
		{
			Catagory5logo->m_Visible = false;
			LockingKeylogo->m_Visible = true;
			LogoTimer.Reset();
		}
		else if(LogoTimer.GetElapsedSeconds() <= 3.0  && LockingKeylogo->m_Visible == true)
		{
			if(SKIPTHEMOTHERFUCKINGINTRO == true)
				LockingKeylogo->m_Visible = false;
			
			if(firstinit == false)
			{
				Catagory5logo->m_Visible = false;
				LockingKeylogo->m_Visible = true;
			}
		}
		else
		{
			LockingKeylogo->m_Visible = false;
			switch(MenuToDraw)
			{
			case 0:
				MainMenuDraw();
				break;
			case 1:
				DrawAudioMenu();
				break;
			case 2:
				DrawControlsMenu();
				break;
			}
		}
	}
}
void	CMenuSystem::ResetSubmenu(Submenu *s)
{
	for(unsigned int i = 0; i < s->ButtonIDs.size(); i++)
	{
		MainMenu.ButtonMasterIndex[s->ButtonIDs[i]].SP[1] = 575;
	}
}
void	CMenuSystem::ResetAllSubmenus()
{
	for(unsigned int i = 0; i < MainMenu.SubmenuIndex.size(); ++i)
	{
		MainMenu.SubmenuIndex[i].Active = false;
		for(unsigned int j = 0; j < MainMenu.SubmenuIndex[i].ButtonIDs.size(); j++)
		{
			MainMenu.ButtonMasterIndex[MainMenu.SubmenuIndex[i].ButtonIDs[j]].SP[1] = 575;
			MainMenu.ButtonMasterIndex[MainMenu.SubmenuIndex[i].ButtonIDs[j]].Selected->m_Visible = false;
			MainMenu.ButtonMasterIndex[MainMenu.SubmenuIndex[i].ButtonIDs[j]].Unselected->m_Visible = false;
		}
	}
	ResetAllAnimations();
	if(firstinit == true)
	{
		firstinit = false;
		Catagory5logo->m_Visible = false;
		LockingKeylogo->m_Visible = false;
	}
}
void	CMenuSystem::InitializeMainMenu()
{
	MainMenu.CurrentMenuButton = NULL;
	MainMenu.CurrentSubmenu = NULL;

	strcpy(MainMenu.MenuBackgroundImage, "./Content/2D/Menu/menubkg.tga");
	strcpy(MainMenu.UnselectedBackgroundImage, "./Content/2D/Menu/InactiveBackground.bmp");
	strcpy(MainMenu.SelectedBackgroundImage, "./Content/2D/Menu/ActiveBackground.bmp");
	strcpy(MainMenu.MenuBGMPath, "./Content/Audio/MenuMusic.wav");

	MainMenu.MenuBackground = new CLabel(MainMenu.MenuBackgroundImage);
	MainMenu.MenuBackground->m_MyBody.centerPosition = CVector3f(400,300,0);
	MainMenu.UnselectedButtonBackground = new CLabel(MainMenu.UnselectedBackgroundImage);
	MainMenu.UnselectedButtonBackground->m_Visible = false;
	MainMenu.SelectedButtonBackground = new CLabel(MainMenu.SelectedBackgroundImage);
	MainMenu.SelectedButtonBackground->m_Visible = false;

	SaturationBombingLogo = new CLabel("./Content/2D/Menu/SatuartionBombing.tga");
	SaturationBombingLogo->m_MyBody.centerPosition = CVector3f(400,300, 0);
	KeyTest = new CLabel("./Content/2D/Menu/Test.bmp");
	SaturationBombingLogo->m_Visible = false;

	MenuButton m;
	m.ID = 0;
	m.HasASubmenu = true;
	m.IsInASubmenu = false;
	m.ActionCode = ActivateSubMenu;
	m.SubmenuID = 0;
	m.CP[0] = 100;
	m.CP[1] = 575;
	m.SP[1] = 575;
	strcpy(m.SelectedImage, "./Content/2D/Menu/Battle.tga");
	strcpy(m.UnselectedImage, "./Content/2D/Menu/Battle.tga");
	
	m.PlayAnimationWhenSelected = true;
	m.AnimationID = 0;
	MainMenu.ButtonMasterIndex.push_back(m);

	m.ID = 1;
	m.HasASubmenu = true;
	m.IsInASubmenu = false;
	m.ActionCode = ActivateAudioMenu;
	m.SubmenuID = 1;
	m.CP[0] = 300;
	m.CP[1] = 575;
	strcpy(m.SelectedImage, "./Content/2D/Menu/OptionsSelectedText.tga");
	strcpy(m.UnselectedImage, "./Content/2D/Menu/Options.tga");
	m.PlayAnimationWhenSelected = true;
	m.AnimationID = 0;
	MainMenu.ButtonMasterIndex.push_back(m);

	m.ID = 2;
	m.HasASubmenu = false;
	m.IsInASubmenu = false;
	m.ActionCode = DisplayCredits;
	m.SubmenuID = -1;
	m.CP[0] = 500;
	m.CP[1] = 575;
	strcpy(m.SelectedImage, "./Content/2D/Menu/Creditsselected.tga");
	strcpy(m.UnselectedImage, "./Content/2D/Menu/credits.tga");
	m.PlayAnimationWhenSelected = true;
	m.AnimationID = 0;
	MainMenu.ButtonMasterIndex.push_back(m);

	m.ID = 3;
	m.HasASubmenu =false;
	m.IsInASubmenu = false;
	m.ActionCode = ExitGame;
	m.SubmenuID = -1;
	m.CP[0] = 700;
	m.CP[1] = 575;
	strcpy(m.SelectedImage, "./Content/2D/Menu/ExitSelectedText.tga");
	strcpy(m.UnselectedImage, "./Content/2D/Menu/Exit.tga");
	m.PlayAnimationWhenSelected = true;
	m.AnimationID = 0;
	MainMenu.ButtonMasterIndex.push_back(m);

	/*m.ID = 4;
	m.HasASubmenu =false;
	m.IsInASubmenu = true;
	m.ActionCode = StartSinglePlayerGame;
	m.SubmenuID = 0;
	m.CP[0] = 100;
	m.CP[1] = 525;
	strcpy(m.SelectedImage, "./Content/2D/Menu/QuickStart.tga");
	strcpy(m.UnselectedImage, "./Content/2D/Menu/QuickStart.tga");
	m.PlayAnimationWhenSelected = true;
	m.AnimationID = 0;
	MainMenu.ButtonMasterIndex.push_back(m);

	m.ID = 5;
	m.HasASubmenu =false;
	m.IsInASubmenu = true;
	m.ActionCode = StartSinglePlayerGame;
	m.SubmenuID = 0;
	m.CP[0] = 100;
	m.CP[1] = 475;
	strcpy(m.SelectedImage, "./Content/2D/Menu/SelectGame.tga");
	strcpy(m.UnselectedImage, "./Content/2D/Menu/SelectGame.tga");
	m.PlayAnimationWhenSelected = true;
	m.AnimationID = 0;
	MainMenu.ButtonMasterIndex.push_back(m);*/

	m.ID = 4;
	m.HasASubmenu =false;
	m.IsInASubmenu = true;
	m.ActionCode = StartBattle;
	m.SubmenuID = 0;
	m.CP[0] = 100;
	m.CP[1] = 525;
	strcpy(m.SelectedImage, "./Content/2D/Menu/MultiplayerSelectedText.tga");
	strcpy(m.UnselectedImage, "./Content/2D/Menu/MultiplayerSelectedText.tga");
	m.PlayAnimationWhenSelected = true;
	m.AnimationID = 0;
	MainMenu.ButtonMasterIndex.push_back(m);

	m.ID = 5;
	m.HasASubmenu =false;
	m.IsInASubmenu = true;
	m.ActionCode = StartSoccer;
	m.SubmenuID = 0;
	m.CP[0] = 100;
	m.CP[1] = 475;
	strcpy(m.SelectedImage, "./Content/2D/Menu/Soccer.tga");
	strcpy(m.UnselectedImage, "./Content/2D/Menu/Soccer.tga");
	m.PlayAnimationWhenSelected = true;
	m.AnimationID = 0;
	MainMenu.ButtonMasterIndex.push_back(m);

	m.ID = 6;
	m.HasASubmenu =false;
	m.IsInASubmenu = true;
	m.ActionCode = ActivateAudioMenu;
	m.SubmenuID = 1;
	m.CP[0] = 300;
	m.CP[1] = 525;
	strcpy(m.SelectedImage, "./Content/2D/Menu/Audio.tga");
	strcpy(m.UnselectedImage, "./Content/2D/Menu/Audio.tga");
	m.PlayAnimationWhenSelected = true;
	m.AnimationID = 0;
	MainMenu.ButtonMasterIndex.push_back(m);

	m.ID = 7;
	m.HasASubmenu =false;
	m.IsInASubmenu = true;
	m.ActionCode = ActivateControlsMenu;
	m.SubmenuID = 1;
	m.CP[0] = 300;
	m.CP[1] = 475;
	strcpy(m.SelectedImage, "./Content/2D/Menu/controlsselected.tga");
	strcpy(m.UnselectedImage, "./Content/2D/Menu/controlsselected.tga");
	m.PlayAnimationWhenSelected = true;
	m.AnimationID = 0;
	MainMenu.ButtonMasterIndex.push_back(m);

	for(unsigned int i = 0; i < MainMenu.ButtonMasterIndex.size(); i++)
	{
		MainMenu.ButtonMasterIndex[i].Unselected = new CLabel(MainMenu.ButtonMasterIndex[i].UnselectedImage);
		MainMenu.ButtonMasterIndex[i].Unselected->m_MyBody.centerPosition = CVector3f((float)MainMenu.ButtonMasterIndex[i].CP[0], (float)MainMenu.ButtonMasterIndex[i].CP[1], 0);

		MainMenu.ButtonMasterIndex[i].Selected = new CLabel(MainMenu.ButtonMasterIndex[i].SelectedImage);
		MainMenu.ButtonMasterIndex[i].Selected->m_MyBody.centerPosition = CVector3f((float)MainMenu.ButtonMasterIndex[i].CP[0], (float)MainMenu.ButtonMasterIndex[i].CP[1], 0);
	}

	Submenu s;
	s.ID = 0;
	s.Active = false;
	s.Openspeed = 2;
	s.OpenedByButtonID = 0;
	s.ButtonIDs.push_back(4);
	s.StartingPosition[0] = 100;
	s.StartingPosition[1] = 550;
	s.FrameCount = (int)(s.ButtonIDs.size() * 4);
	MainMenu.SubmenuIndex.push_back(s);

	s.ButtonIDs.clear();
	s.ID = 1;
	s.Active = false;
	s.Openspeed = 2;
	s.OpenedByButtonID = 1;
	s.ButtonIDs.push_back(6);
	s.ButtonIDs.push_back(7);
	s.StartingPosition[0] = 500;
	s.StartingPosition[1] = 550;
	MainMenu.SubmenuIndex.push_back(s);

	Animation a;
	a.ID = 0;
	strcpy(a.BackgroundPath, "./Content/2D/Menu/NewButtonBackground.tga");
	a.FrameCount = 0;
	a.ImagePaths.push_back("./Content/2D/Menu/SelectedAnimationFrame1.bmp");
	a.ImagePaths.push_back("./Content/2D/Menu/SelectedAnimationFrame2.bmp");
	a.ImagePaths.push_back("./Content/2D/Menu/SelectedAnimationFrame3.bmp");
	a.ImagePaths.push_back("./Content/2D/Menu/SelectedAnimationFrame4.bmp");
	a.FrameTimer = 0.25f;

	MainMenu.AnimationIndex.push_back(a);

	for(unsigned int i = 0; i < MainMenu.AnimationIndex.size(); i++)
	{
		MainMenu.AnimationIndex[i].Background = new CLabel(MainMenu.AnimationIndex[i].BackgroundPath);
		for(unsigned int j = 0; j < MainMenu.AnimationIndex[i].ImagePaths.size(); j++)
		{
			CLabel *tmp = new CLabel(MainMenu.AnimationIndex[i].ImagePaths[j]);
			MainMenu.AnimationIndex[i].Frames.push_back(tmp);
		}
	}
	MainMenu.CurrentMenuButton = &MainMenu.ButtonMasterIndex[0];
	MainMenu.MainMenuButtonIDs.push_back(0);
	MainMenu.MainMenuButtonIDs.push_back(1);
	MainMenu.MainMenuButtonIDs.push_back(2);
	MainMenu.MainMenuButtonIDs.push_back(3);

	Catagory5logo = new CLabel("./Content/2D/Menu/catagory5.bmp");
	Catagory5logo->m_MyBody.centerPosition = CVector3f(400,300, 0);
	LockingKeylogo = new CLabel("./Content/2D/Menu/TheLockingKeyLogo.bmp");
	LockingKeylogo->m_MyBody.centerPosition = CVector3f(400,300, 0);

	//	CRenderInterface::GetInstance()->DestroyScene();
	// CRenderInterface::GetInstance()->SetWorld("Models/pool.x");
	CRenderInterface::GetInstance()->AddLabel(Catagory5logo);
	Catagory5logo->m_Visible = true;
	CRenderInterface::GetInstance()->AddLabel(LockingKeylogo);
	LockingKeylogo->m_Visible = false;
	//CRenderInterface::GetInstance()->AddLabel(MainMenu.MenuBackground);
	//MainMenu.MenuBackground->m_Visible = false;
	CRenderInterface::GetInstance()->AddLabel(MainMenu.UnselectedButtonBackground);
	MainMenu.MenuBackground->m_Visible = false;
	CRenderInterface::GetInstance()->AddLabel(MainMenu.SelectedButtonBackground);
	MainMenu.MenuBackground->m_Visible = false;
	for(unsigned int i = 0; i < MainMenu.AnimationIndex.size(); i++)
	{
		CRenderInterface::GetInstance()->AddLabel(MainMenu.AnimationIndex[i].Background);
		MainMenu.AnimationIndex[i].Background->m_Visible = false;
		MainMenu.AnimationIndex[i].Reset();
		CRenderInterface::GetInstance()->AddLabel(MainMenu.AnimationIndex[i].CurFrame);
	}
	for(unsigned int i = 0; i < MainMenu.ButtonMasterIndex.size(); i++)
	{
		CRenderInterface::GetInstance()->AddLabel(MainMenu.ButtonMasterIndex[i].Selected);
		MainMenu.ButtonMasterIndex[i].Selected->m_Visible = false;
		CRenderInterface::GetInstance()->AddLabel(MainMenu.ButtonMasterIndex[i].Unselected);
		MainMenu.ButtonMasterIndex[i].Unselected->m_Visible = false;
	}
}
void	CMenuSystem::MainMenuDraw()
{
	// main menu background images
	MainMenu.MenuBackground->m_Visible = true;

	// Draw Submenus
	for(unsigned int i = 0; i < MainMenu.SubmenuIndex.size(); i++)
	{
		if(MainMenu.SubmenuIndex[i].Active)
		{
			for(unsigned int j = 0; j < MainMenu.SubmenuIndex[i].ButtonIDs.size(); j++)
			{
				MenuButton *A = &MainMenu.ButtonMasterIndex[MainMenu.SubmenuIndex[i].ButtonIDs[j]];
				int a = MainMenu.ButtonMasterIndex[MainMenu.SubmenuIndex[i].ButtonIDs[j]].SP[1];
				int b = MainMenu.ButtonMasterIndex[MainMenu.SubmenuIndex[i].ButtonIDs[j]].CP[1];
				if(a >= b && MainMenu.ButtonMasterIndex[MainMenu.SubmenuIndex[i].ButtonIDs[j]].SP[1] != MainMenu.ButtonMasterIndex[MainMenu.SubmenuIndex[i].ButtonIDs[j]].CP[1])
				{
					MainMenu.ButtonMasterIndex[MainMenu.SubmenuIndex[i].ButtonIDs[j]].SP[1] -= 2;
					MainMenu.ButtonMasterIndex[MainMenu.SubmenuIndex[i].ButtonIDs[j]].Selected->m_MyBody.centerPosition.y = (float)MainMenu.ButtonMasterIndex[MainMenu.SubmenuIndex[i].ButtonIDs[j]].SP[1];
					MainMenu.ButtonMasterIndex[MainMenu.SubmenuIndex[i].ButtonIDs[j]].Unselected->m_MyBody.centerPosition.y = (float)MainMenu.ButtonMasterIndex[MainMenu.SubmenuIndex[i].ButtonIDs[j]].SP[1];
				}
				else
				{
					MainMenu.ButtonMasterIndex[MainMenu.SubmenuIndex[i].ButtonIDs[j]].SP[1] = MainMenu.ButtonMasterIndex[MainMenu.SubmenuIndex[i].ButtonIDs[j]].CP[1];
					MainMenu.ButtonMasterIndex[MainMenu.SubmenuIndex[i].ButtonIDs[j]].Selected->m_MyBody.centerPosition.y = (float)MainMenu.ButtonMasterIndex[MainMenu.SubmenuIndex[i].ButtonIDs[j]].CP[1];
					MainMenu.ButtonMasterIndex[MainMenu.SubmenuIndex[i].ButtonIDs[j]].Unselected->m_MyBody.centerPosition.y = (float)MainMenu.ButtonMasterIndex[MainMenu.SubmenuIndex[i].ButtonIDs[j]].CP[1];
				}
//				MainMenu.SelectedButtonBackground->Draw((float)MainMenu.SubmenuIndex[i].StartingPosition[0], (float)MainMenu.ButtonMasterIndex[MainMenu.SubmenuIndex[i].ButtonIDs[j]].SP[1]);
				MainMenu.ButtonMasterIndex[MainMenu.SubmenuIndex[i].ButtonIDs[j]].Unselected->m_Visible = true;
			}
			break;
		}
		else
			continue;
	}

	// Draw Current menu button
	if(MainMenu.CurrentMenuButton->PlayAnimationWhenSelected == false)
	{
//		MainMenu.SelectedButtonBackground->Draw((float)MainMenu.CurrentMenuButton->CP[0], (float)MainMenu.CurrentMenuButton->CP[1]);
		MainMenu.CurrentMenuButton->Selected->m_Visible = true;
		MainMenu.CurrentMenuButton->Selected->m_MyBody.centerPosition.x = (float)MainMenu.CurrentMenuButton->CP[0];
		MainMenu.CurrentMenuButton->Selected->m_MyBody.centerPosition.y = (float)MainMenu.CurrentMenuButton->CP[1];
	}
	else
	{
		Animation *a = &MainMenu.AnimationIndex[MainMenu.CurrentMenuButton->AnimationID];
		static int AnimationFrame = 0;
		a->Background->m_MyBody.centerPosition.x = (float)MainMenu.CurrentMenuButton->CP[0];
		a->Background->m_MyBody.centerPosition.y = (float)MainMenu.CurrentMenuButton->CP[1];
		a->Background->m_Visible = true;
		a->Frames[AnimationFrame]->m_MyBody.centerPosition.x = (float)MainMenu.CurrentMenuButton->CP[0];
		a->Frames[AnimationFrame]->m_MyBody.centerPosition.y = (float)MainMenu.CurrentMenuButton->CP[1];
		MainMenu.CurrentMenuButton->Selected->m_MyBody.centerPosition.x = (float)MainMenu.CurrentMenuButton->CP[0];
		MainMenu.CurrentMenuButton->Selected->m_MyBody.centerPosition.y = (float)MainMenu.CurrentMenuButton->CP[1];
		MainMenu.CurrentMenuButton->Selected->m_Visible = true;
		if(AnimationTimer.GetElapsedSeconds() >= 0.15)
		{
			AnimationTimer.Reset();
			a->Play(CRenderInterface::GetInstance(), MainMenu.CurrentMenuButton->CP[0], MainMenu.CurrentMenuButton->CP[1]);
		}
	}

	// draw main menu buttons
	for(unsigned int i =0 ;i < MainMenu.MainMenuButtonIDs.size(); i++)
	{
		// dont draw the unselected image if it's the current button
		if(MainMenu.CurrentMenuButton->ID == MainMenu.MainMenuButtonIDs[i])
			continue;
		if(MainMenu.ButtonMasterIndex[MainMenu.MainMenuButtonIDs[i]].HasASubmenu
			&& MainMenu.SubmenuIndex[MainMenu.MainMenuButtonIDs[i]].Active)
		{
//			MainMenu.SelectedButtonBackground->Draw((float)MainMenu.ButtonMasterIndex[MainMenu.MainMenuButtonIDs[i]].CP[0], (float)MainMenu.ButtonMasterIndex[MainMenu.MainMenuButtonIDs[i]].CP[1]);
			MainMenu.ButtonMasterIndex[MainMenu.MainMenuButtonIDs[i]].Selected->m_MyBody.centerPosition.x = (float)MainMenu.ButtonMasterIndex[MainMenu.MainMenuButtonIDs[i]].CP[0];
			MainMenu.ButtonMasterIndex[MainMenu.MainMenuButtonIDs[i]].Selected->m_MyBody.centerPosition.y = (float)MainMenu.ButtonMasterIndex[MainMenu.MainMenuButtonIDs[i]].CP[1];
			MainMenu.ButtonMasterIndex[MainMenu.MainMenuButtonIDs[i]].Selected->m_Visible = true;
		}
		else
		{
//			MainMenu.UnselectedButtonBackground->Draw((float)MainMenu.ButtonMasterIndex[MainMenu.MainMenuButtonIDs[i]].CP[0], (float)MainMenu.ButtonMasterIndex[MainMenu.MainMenuButtonIDs[i]].CP[1]);
			MainMenu.ButtonMasterIndex[MainMenu.MainMenuButtonIDs[i]].Unselected->m_MyBody.centerPosition.x = (float)MainMenu.ButtonMasterIndex[MainMenu.MainMenuButtonIDs[i]].CP[0];
			MainMenu.ButtonMasterIndex[MainMenu.MainMenuButtonIDs[i]].Unselected->m_MyBody.centerPosition.y = (float)MainMenu.ButtonMasterIndex[MainMenu.MainMenuButtonIDs[i]].CP[1];
			MainMenu.ButtonMasterIndex[MainMenu.MainMenuButtonIDs[i]].Unselected->m_Visible = true;
		}
	}
}
int		CMenuSystem::MainMenuUpdate()
{
	pInput->SetAllStates();
	int ActionResult = 0;
	// just check on the first player controller
	if (pInput->ControllerJoyPress(0, CGameProfile::GetInstance()->ButtonLayout[0]._shoot) || pInput->KeyboardKeyPress(DIK_RETURN)&& InputTimer.GetElapsedSeconds() > 0.15)
	{
		InputTimer.Reset();
		SKIPTHEMOTHERFUCKINGINTRO = true;
		Catagory5logo->m_Visible = false;
		LockingKeylogo->m_Visible = false;
	}
	if(LockingKeylogo->m_Visible == false && Catagory5logo->m_Visible == false)
	{
		if(pInput->ControllerJoystickLeft(-1) && InputTimer.GetElapsedSeconds() > 0.15)
		{
			pSound->PlayQSFX(SFXcycleButtons);
			InputTimer.Reset();
			ResetAllSubmenus();
			if(MainMenu.CurrentMenuButton->IsInASubmenu)
			{
				if(MainMenu.SubmenuIndex[MainMenu.CurrentMenuButton->SubmenuID].Active)
				{
					MainMenu.SubmenuIndex[MainMenu.CurrentMenuButton->SubmenuID].Active = false;
				}
				int newButtonID = MainMenu.SubmenuIndex[MainMenu.CurrentMenuButton->SubmenuID].OpenedByButtonID;
				MainMenu.ButtonMasterIndex[newButtonID].Selected->m_Visible = false;
				if(newButtonID == 0)
				{
					MainMenu.CurrentMenuButton->Selected->m_Visible = false;
					MainMenu.CurrentMenuButton = &MainMenu.ButtonMasterIndex[3];
					MainMenu.CurrentMenuButton->Selected->m_Visible = true;
				}
				else if(newButtonID == 1)
				{
					MainMenu.CurrentMenuButton->Selected->m_Visible = false;
					MainMenu.CurrentMenuButton = &MainMenu.ButtonMasterIndex[0];
					MainMenu.CurrentMenuButton->Selected->m_Visible = true;
				}
				else if(newButtonID == 2)
				{
					MainMenu.CurrentMenuButton->Selected->m_Visible = false;
					MainMenu.CurrentMenuButton = &MainMenu.ButtonMasterIndex[1];
					MainMenu.CurrentMenuButton->Selected->m_Visible = true;
				}
				else if(newButtonID == 3)
				{
					MainMenu.CurrentMenuButton->Selected->m_Visible = false;
					MainMenu.CurrentMenuButton = &MainMenu.ButtonMasterIndex[2];
					MainMenu.CurrentMenuButton->Selected->m_Visible = true;
				}

				if(MainMenu.CurrentMenuButton->HasASubmenu)
				{
					if(MainMenu.SubmenuIndex[MainMenu.CurrentMenuButton->SubmenuID].Active)
						MainMenu.SubmenuIndex[MainMenu.CurrentMenuButton->SubmenuID].Active = false;
				}
				MainMenu.CurrentMenuButton->Unselected->m_Visible = false;
			}
			else
			{
				if(MainMenu.CurrentMenuButton->ID == 0)
				{
					MainMenu.CurrentMenuButton->Selected->m_Visible = false;
					MainMenu.CurrentMenuButton = &MainMenu.ButtonMasterIndex[3];
				}
				else if(MainMenu.CurrentMenuButton->ID == 1)
				{
					MainMenu.CurrentMenuButton->Selected->m_Visible = false;
					MainMenu.CurrentMenuButton = &MainMenu.ButtonMasterIndex[0];
				}
				else if(MainMenu.CurrentMenuButton->ID == 2)
				{
					MainMenu.CurrentMenuButton->Selected->m_Visible = false;
					MainMenu.CurrentMenuButton = &MainMenu.ButtonMasterIndex[1];
				}
				else if(MainMenu.CurrentMenuButton->ID == 3)
				{
					MainMenu.CurrentMenuButton->Selected->m_Visible = false;
					MainMenu.CurrentMenuButton = &MainMenu.ButtonMasterIndex[2];
				}

				if(MainMenu.CurrentMenuButton->HasASubmenu)
				{
					if(MainMenu.SubmenuIndex[MainMenu.CurrentMenuButton->SubmenuID].Active)
						MainMenu.SubmenuIndex[MainMenu.CurrentMenuButton->SubmenuID].Active = false;
				}
				MainMenu.CurrentMenuButton->Selected->m_Visible = true;
				MainMenu.CurrentMenuButton->Unselected->m_Visible = false;
			}
		}

		if((pInput->ControllerJoystickRight(-1)) && InputTimer.GetElapsedSeconds() > 0.15)
		{
			pSound->PlayQSFX(SFXcycleButtons);
			InputTimer.Reset();
			ResetAllSubmenus();
			if(MainMenu.CurrentMenuButton->IsInASubmenu)
			{
				MainMenu.SubmenuIndex[MainMenu.CurrentMenuButton->SubmenuID].Active = false;

				int newButtonID = MainMenu.SubmenuIndex[MainMenu.CurrentMenuButton->SubmenuID].OpenedByButtonID;
				MainMenu.ButtonMasterIndex[newButtonID].Selected->m_Visible = false;
				if(newButtonID == 0)
				{
					MainMenu.CurrentMenuButton->Selected->m_Visible = false;
					MainMenu.CurrentMenuButton = &MainMenu.ButtonMasterIndex[1];
				}
				else if(newButtonID == 1)
				{
					MainMenu.CurrentMenuButton->Selected->m_Visible = false;
					MainMenu.CurrentMenuButton = &MainMenu.ButtonMasterIndex[2];
				}
				else if(newButtonID == 2)
				{
					MainMenu.CurrentMenuButton->Selected->m_Visible = false;
					MainMenu.CurrentMenuButton = &MainMenu.ButtonMasterIndex[3];
				}
				else if(newButtonID == 3)
				{
					MainMenu.CurrentMenuButton->Selected->m_Visible = false;
					MainMenu.CurrentMenuButton = &MainMenu.ButtonMasterIndex[0];
				}
				MainMenu.CurrentMenuButton->Selected->m_Visible = true;
				MainMenu.CurrentMenuButton->Unselected->m_Visible = false;
			}
			else
			{
				if(MainMenu.CurrentMenuButton->ID == 0)
				{
					MainMenu.CurrentMenuButton->Selected->m_Visible = false;
					MainMenu.CurrentMenuButton = &MainMenu.ButtonMasterIndex[1];
				}
				else if(MainMenu.CurrentMenuButton->ID == 1)
				{
					MainMenu.CurrentMenuButton->Selected->m_Visible = false;
					MainMenu.CurrentMenuButton = &MainMenu.ButtonMasterIndex[2];
				}
				else if(MainMenu.CurrentMenuButton->ID == 2)
				{
					MainMenu.CurrentMenuButton->Selected->m_Visible = false;
					MainMenu.CurrentMenuButton = &MainMenu.ButtonMasterIndex[3];
				}
				else if(MainMenu.CurrentMenuButton->ID == 3)
				{
					MainMenu.CurrentMenuButton->Selected->m_Visible = false;
					MainMenu.CurrentMenuButton = &MainMenu.ButtonMasterIndex[0];
				}
				if(MainMenu.CurrentMenuButton->HasASubmenu)
				{
					if(MainMenu.SubmenuIndex[MainMenu.CurrentMenuButton->SubmenuID].Active)
						MainMenu.SubmenuIndex[MainMenu.CurrentMenuButton->SubmenuID].Active = false;
				}
				MainMenu.CurrentMenuButton->Selected->m_Visible = true;
				MainMenu.CurrentMenuButton->Unselected->m_Visible = false;
			}
		}
		if(pInput->ControllerJoystickDown(-1) && InputTimer.GetElapsedSeconds() > 0.15)
		{
			pSound->PlayQSFX(SFXcycleButtons);
			InputTimer.Reset();
			if(MainMenu.CurrentMenuButton->HasASubmenu && MainMenu.SubmenuIndex[MainMenu.CurrentMenuButton->SubmenuID].Active)
			{
				bool NextButtonFound = false;
				int NextButtonID = -1;
				// update selection to the button in the current submenu

				// if the submenu only has one button, return
				if(MainMenu.SubmenuIndex[MainMenu.CurrentMenuButton->SubmenuID].ButtonIDs.size() == 1)
				{
					MainMenu.CurrentMenuButton->Selected->m_Visible = false;
					MainMenu.CurrentMenuButton->Unselected->m_Visible = false;
					MainMenu.CurrentMenuButton = &MainMenu.ButtonMasterIndex[MainMenu.SubmenuIndex[MainMenu.CurrentMenuButton->SubmenuID].ButtonIDs[0]];
					return -1;
				}

				// find the index in the submenus button list of the current selected button
				for(unsigned int i = 0; i < MainMenu.SubmenuIndex[MainMenu.CurrentMenuButton->SubmenuID].ButtonIDs.size(); i++)
				{
					if(MainMenu.CurrentMenuButton->ID == MainMenu.SubmenuIndex[MainMenu.CurrentMenuButton->SubmenuID].ButtonIDs[i])
					{
						// found it
						NextButtonFound = true;
						if(i == MainMenu.SubmenuIndex[MainMenu.CurrentMenuButton->SubmenuID].ButtonIDs.size())
						{
							NextButtonID = MainMenu.SubmenuIndex[MainMenu.CurrentMenuButton->SubmenuID].ButtonIDs[0];
						}
						else
							NextButtonID = MainMenu.SubmenuIndex[MainMenu.CurrentMenuButton->SubmenuID].ButtonIDs[i];
						break;
					}
					else
						continue; // keep looking
				}
				// set the new current button
				if(NextButtonID == -1)
				{
					// entering the submenu
					NextButtonID = MainMenu.SubmenuIndex[MainMenu.CurrentMenuButton->SubmenuID].ButtonIDs[0];
				}
				for(unsigned int i = 0; i < MainMenu.ButtonMasterIndex.size(); i++)
				{
					if(NextButtonID == MainMenu.ButtonMasterIndex[i].ID)
					{
						MainMenu.CurrentMenuButton = &MainMenu.ButtonMasterIndex[i];
						break;
					}
				}
			}
			else if(MainMenu.CurrentMenuButton->IsInASubmenu)
			{
				bool NextButtonFound = false;
				int NextButtonID = -1;
				// update selection to the button in the current submenu

				// find the index in the submenus button list of the current selected button
				for(unsigned int i = 0; i < MainMenu.SubmenuIndex[MainMenu.CurrentMenuButton->SubmenuID].ButtonIDs.size(); i++)
				{
					if(MainMenu.CurrentMenuButton->ID == MainMenu.SubmenuIndex[MainMenu.CurrentMenuButton->SubmenuID].ButtonIDs[i])
					{
						// found it
						NextButtonFound = true;
						int tmp = (int)MainMenu.SubmenuIndex[MainMenu.CurrentMenuButton->SubmenuID].ButtonIDs.size() - 1;
						if(i == 0)
						{
							NextButtonID = MainMenu.SubmenuIndex[MainMenu.CurrentMenuButton->SubmenuID].ButtonIDs[MainMenu.SubmenuIndex[MainMenu.CurrentMenuButton->SubmenuID].ButtonIDs.size() -1];
						}
						else
							NextButtonID = MainMenu.SubmenuIndex[MainMenu.CurrentMenuButton->SubmenuID].ButtonIDs[i-1];
						break;
					}
					else
						continue; // keep looking
				}
				// set the new current button
				if(NextButtonID == -1)
				{
					// entering the submenu
					NextButtonID = MainMenu.SubmenuIndex[MainMenu.CurrentMenuButton->SubmenuID].ButtonIDs[0];
				}
				for(unsigned int i = 0; i < MainMenu.ButtonMasterIndex.size(); i++)
				{
					if(NextButtonID == MainMenu.ButtonMasterIndex[i].ID)
					{
						MainMenu.CurrentMenuButton = &MainMenu.ButtonMasterIndex[i];
						break;
					}
				}
			}
		}
		if(pInput->ControllerJoystickUp(-1)	&& InputTimer.GetElapsedSeconds() > 0.15)
		{
			pSound->PlayQSFX(SFXcycleButtons);
			InputTimer.Reset();
			if(MainMenu.CurrentMenuButton->HasASubmenu && MainMenu.SubmenuIndex[MainMenu.CurrentMenuButton->SubmenuID].Active)
			{
				bool NextButtonFound = false;
				int NextButtonID = -1;
				// update selection to the button in the current submenu

				// if the submenu only has one button, return
				if(MainMenu.SubmenuIndex[MainMenu.CurrentMenuButton->SubmenuID].ButtonIDs.size() == 1)
				{
					MainMenu.CurrentMenuButton->Selected->m_Visible = false;
					MainMenu.CurrentMenuButton->Unselected->m_Visible = false;
					MainMenu.CurrentMenuButton = &MainMenu.ButtonMasterIndex[MainMenu.SubmenuIndex[MainMenu.CurrentMenuButton->SubmenuID].ButtonIDs[0]];
					return -1;
				}

				// find the index in the submenus button list of the current selected button
				for(unsigned int i = 0; i < MainMenu.SubmenuIndex[MainMenu.CurrentMenuButton->SubmenuID].ButtonIDs.size(); i++)
				{
					if(MainMenu.CurrentMenuButton->ID == MainMenu.SubmenuIndex[MainMenu.CurrentMenuButton->SubmenuID].ButtonIDs[i])
					{
						// found it
						NextButtonFound = true;
						if(i == MainMenu.SubmenuIndex[MainMenu.CurrentMenuButton->SubmenuID].ButtonIDs.size())
						{
							NextButtonID = MainMenu.SubmenuIndex[MainMenu.CurrentMenuButton->SubmenuID].ButtonIDs[0];
						}
						else
							NextButtonID = MainMenu.SubmenuIndex[MainMenu.CurrentMenuButton->SubmenuID].ButtonIDs[i];
						break;
					}
					else
						continue; // keep looking
				}
				// set the new current button
				if(NextButtonID == -1)
				{
					// entering the submenu
					NextButtonID = MainMenu.SubmenuIndex[MainMenu.CurrentMenuButton->SubmenuID].ButtonIDs[0];
				}
				for(unsigned int i = 0; i < MainMenu.ButtonMasterIndex.size(); i++)
				{
					if(NextButtonID == MainMenu.ButtonMasterIndex[i].ID)
					{
						MainMenu.CurrentMenuButton = &MainMenu.ButtonMasterIndex[i];
						break;
					}
				}
			}
			else if(MainMenu.CurrentMenuButton->IsInASubmenu)
			{
				bool NextButtonFound = false;
				int NextButtonID = -1;
				// update selection to the button in the current submenu

				// find the index in the submenus button list of the current selected button
				for(unsigned int i = 0; i < MainMenu.SubmenuIndex[MainMenu.CurrentMenuButton->SubmenuID].ButtonIDs.size(); i++)
				{
					if(MainMenu.CurrentMenuButton->ID == MainMenu.SubmenuIndex[MainMenu.CurrentMenuButton->SubmenuID].ButtonIDs[i])
					{
						// found it
						NextButtonFound = true;
						int tmp = (int)MainMenu.SubmenuIndex[MainMenu.CurrentMenuButton->SubmenuID].ButtonIDs.size() - 1;
						if(i == tmp)
						{
							NextButtonID = MainMenu.SubmenuIndex[MainMenu.CurrentMenuButton->SubmenuID].ButtonIDs[0];
						}
						else
							NextButtonID = MainMenu.SubmenuIndex[MainMenu.CurrentMenuButton->SubmenuID].ButtonIDs[i+1];
						break;
					}
					else
						continue; // keep looking
				}
				// set the new current button
				if(NextButtonID == -1)
				{
					// entering the submenu
					NextButtonID = MainMenu.SubmenuIndex[MainMenu.CurrentMenuButton->SubmenuID].ButtonIDs[0];
				}
				for(unsigned int i = 0; i < MainMenu.ButtonMasterIndex.size(); i++)
				{
					if(NextButtonID == MainMenu.ButtonMasterIndex[i].ID)
					{
						MainMenu.CurrentMenuButton = &MainMenu.ButtonMasterIndex[i];
						break;
					}
				}
			}
		}

		if(pInput->ControllerJoyPress(0, CGameProfile::GetInstance()->ButtonLayout[0]._shoot) || pInput->KeyboardKeyPress(DIK_RETURN))
		{
			pSound->PlayQSFX(SFXactiveButton);
			InputTimer.Reset();
			switch(MainMenu.CurrentMenuButton->ActionCode)
			{
			case ActivateSubMenu:
				{
					if(MainMenu.CurrentMenuButton->HasASubmenu)
					{
						if(MainMenu.SubmenuIndex[MainMenu.CurrentMenuButton->SubmenuID].Active == false)
						{
							MainMenu.SubmenuIndex[MainMenu.CurrentMenuButton->SubmenuID].Active = true;
							MainMenu.SubmenuIndex[MainMenu.CurrentMenuButton->SubmenuID].Animating = true;
						}
					}
					ActionResult = -1;
				}
				break;
			case ExitGame:
				{
					ActionResult = Exit_Game;
				}
				break;
			case StartSinglePlayerGame:
				{
					ActionResult = Begin_New_Multiplayer_Game;
				}
				break;
			case StartMultiplayerGame:
				{
					ActionResult = Begin_New_Multiplayer_Game;
				}
				break;
			case StartSoccer:
				{
					ActionResult = StartSoccer;
				}
				break;
			case StartBattle:
				{
					ActionResult = StartBattle;
				}
				break;
			case DisplayCredits:
				{
					incredits = true;
					break;
				}
			case ActivateAudioMenu:
				{
					ShutOffMainMenu();
					ActionResult  = ChangeToAudioMenu;
				}
				break;
			default:
				break;
			}
		}
	}
	return ActionResult;
}
void	CMenuSystem::ResetAllAnimations()
{
	for(unsigned int i = 0; i < MainMenu.AnimationIndex.size(); i++)
	{
		MainMenu.AnimationIndex[i].Background->m_Visible = false;
		for(unsigned int j = 0; j < MainMenu.AnimationIndex[i].Frames.size(); j++)
		{
			MainMenu.AnimationIndex[i].Frames[j]->m_Visible = false;
		}
	}
}
void	CMenuSystem::InitializeAudioMenu()
{
	SoundMenu.SelectionTracker  = 0;
	SoundMenu.SFXStatus = true;
	SoundMenu.BGMStatus = true;
	SoundMenu.SFXVolume = 70;
	SoundMenu.BGMVolume = 70;

	SoundMenu.SFXToggleSelectionCoords[0] = 128;
	SoundMenu.SFXToggleSelectionCoords[1] = 116;

	SoundMenu.SFXVolumeHighlightCoords[0] = 176;
	SoundMenu.SFXVolumeHighlightCoords[1] = 190;

	SoundMenu.BGMToggleSelectionCoords[0] = 128;
	SoundMenu.BGMToggleSelectionCoords[1] = 273;

	SoundMenu.BGMVolumeHighlightCoords[0] = 176;
	SoundMenu.BGMVolumeHighlightCoords[1] = 435;

	SoundMenu.SFXToggle.ShowArrows	= false;
	SoundMenu.SFXToggle.LoopOptions	= false;

	SoundMenu.BGMToggle.ShowArrows	= false;
	SoundMenu.BGMToggle.LoopOptions	= false;

	SoundMenu.SFXVolumeControl.LoopOptions = false;
	SoundMenu.SFXVolumeControl.ShowArrows = false;
	SoundMenu.SFXVolumeControl.ID = 0;
	SoundMenu.SFXVolumeControl.Highlight = new CLabel("./Content/2D/Menu/highlight.tga");
	SoundMenu.SFXVolumeControl.LeftArrow = NULL;
	SoundMenu.SFXVolumeControl.RightArrow = NULL;

	SoundMenu.BGMVolumeControl.LoopOptions = false;
	SoundMenu.BGMVolumeControl.ShowArrows = false;
	SoundMenu.BGMVolumeControl.ID = 0;
	SoundMenu.BGMVolumeControl.Highlight = new CLabel("./Content/2D/Menu/highlight.tga");
	SoundMenu.BGMVolumeControl.LeftArrow = NULL;
	SoundMenu.BGMVolumeControl.RightArrow = NULL;

	SoundMenu.Background = new CLabel("./content/2d/menu/audiomenubackground.tga");
	SoundMenu.Background->m_Visible = false;
	SoundMenu.Background->m_MyBody.centerPosition = CVector3f(400.0f, 300.0f, 0);
	CRenderInterface::GetInstance()->AddLabel(SoundMenu.Background);

	SoundMenu.ToggleHighlight = new CLabel("./content/2d/menu/toggleselected.tga");
	SoundMenu.ToggleHighlight->m_Visible = false;
	CRenderInterface::GetInstance()->AddLabel(SoundMenu.ToggleHighlight);

	SoundMenu.VolumeHighlight = new CLabel("./content/2d/menu/volumebarselected.tga");
	SoundMenu.VolumeHighlight->m_Visible = false;
	CRenderInterface::GetInstance()->AddLabel(SoundMenu.VolumeHighlight);

	SoundMenu.SoundEffects = new CLabel("./Content/2d/menu/soundeffects.tga");
	SoundMenu.SoundEffects->m_Visible = false;
	SoundMenu.SoundEffects->m_MyBody.centerPosition = CVector3f(100.0f,100.0f,0);
	CRenderInterface::GetInstance()->AddLabel(SoundMenu.SoundEffects);

	SoundMenu.SoundVolumeImage = new CLabel("./Content/2d/menu/volume.tga");
	SoundMenu.SoundVolumeImage->m_Visible = false;
	SoundMenu.SoundVolumeImage->m_MyBody.centerPosition = CVector3f(70.0f,175.0f,0);
	CRenderInterface::GetInstance()->AddLabel(SoundMenu.SoundVolumeImage);

	SoundMenu.Music = new CLabel("./Content/2d/menu/music.tga");
	SoundMenu.Music->m_Visible = false;
	SoundMenu.Music->m_MyBody.centerPosition = CVector3f(50.0f,250.0f,0);
	CRenderInterface::GetInstance()->AddLabel(SoundMenu.Music);

	//SoundMenu.MusicVolumeImage = new CLabel("./Content/2d/menu/volume.tga");
	//SoundMenu.MusicVolumeImage->m_Visible = false;
	//SoundMenu.MusicVolumeImage->m_MyBody.centerPosition = CVector3f(70.0f,375.0f,0);
	//CRenderInterface::GetInstance()->AddLabel(SoundMenu.MusicVolumeImage);

	SoundMenu.BackButton.Unselected = new CLabel("./content/2d/menu/back.tga");
	SoundMenu.BackButton.Unselected->m_Visible = false;
	SoundMenu.BackButton.Unselected->m_MyBody.centerPosition = CVector3f(100, 550, 0);
	SoundMenu.BackButton.Selected = new CLabel("./content/2d/menu/backmouseover.tga");
	SoundMenu.BackButton.Selected->m_Visible = false;
	SoundMenu.BackButton.Selected->m_MyBody.centerPosition = CVector3f(100, 550, 0);
	CRenderInterface::GetInstance()->AddLabel(SoundMenu.BackButton.Unselected);
	CRenderInterface::GetInstance()->AddLabel(SoundMenu.BackButton.Selected);

	SoundMenu.Audio = new CLabel("./Content/2D/menu/audiooptions.tga");
	SoundMenu.Audio->m_Visible = false;
	SoundMenu.Audio->m_MyBody.centerPosition = CVector3f(100, 50, 0);
	CRenderInterface::GetInstance()->AddLabel(SoundMenu.Audio);

	SoundMenu.GameOptions = new CLabel("./Content/2d/menu/gameoptions.tga");
	SoundMenu.GameOptions->m_Visible = false;
	SoundMenu.GameOptions->m_MyBody.centerPosition = CVector3f(100,375,0);
	CRenderInterface::GetInstance()->AddLabel(SoundMenu.GameOptions);

	SoundMenu.Rounds = new CLabel("./content/2d/menu/rounds.tga");
	SoundMenu.Rounds->m_Visible = false;
	SoundMenu.Rounds->m_MyBody.centerPosition = CVector3f(100, 416, 0);
	CRenderInterface::GetInstance()->AddLabel(SoundMenu.Rounds);
	// SFX
	SliderOption o;
	o.Data = 10;
	o.Position[0] = 32;
	o.Position[1] = 200;
	o.Image = new CLabel("./Content/2D/Menu/10.tga");
	o.Image->m_Visible = false;
	SoundMenu.SFXVolumeControl.Options.push_back(o);

	o.Data = 20;
	o.Position[0] += 32;
	o.Image = new CLabel("./Content/2D/Menu/20.tga");
	o.Image->m_Visible = false;
	SoundMenu.SFXVolumeControl.Options.push_back(o);

	o.Data = 30;
	o.Position[0] += 32;
	o.Image = new CLabel("./Content/2D/Menu/30.tga");
	o.Image->m_Visible = false;
	SoundMenu.SFXVolumeControl.Options.push_back(o);

	o.Data = 40;
	o.Position[0] += 32;
	o.Image = new CLabel("./Content/2D/Menu/40.tga");
	o.Image->m_Visible = false;
	SoundMenu.SFXVolumeControl.Options.push_back(o);

	o.Data = 50;
	o.Position[0] += 32;
	o.Image = new CLabel("./Content/2D/Menu/50.tga");
	o.Image->m_Visible = false;
	SoundMenu.SFXVolumeControl.Options.push_back(o);

	o.Data = 60;
	o.Position[0] += 32;
	o.Image = new CLabel("./Content/2D/Menu/60.tga");
	o.Image->m_Visible = false;
	SoundMenu.SFXVolumeControl.Options.push_back(o);

	o.Data = 70;
	o.Position[0] += 32;
	o.Image = new CLabel("./Content/2D/Menu/70.tga");
	o.Image->m_Visible = false;
	SoundMenu.SFXVolumeControl.Options.push_back(o);

	o.Data = 80;
	o.Position[0] += 32;
	o.Image = new CLabel("./Content/2D/Menu/80.tga");
	o.Image->m_Visible = false;
	SoundMenu.SFXVolumeControl.Options.push_back(o);

	o.Data = 90;
	o.Position[0] += 32;
	o.Image = new CLabel("./Content/2D/Menu/90.tga");
	o.Image->m_Visible = false;
	SoundMenu.SFXVolumeControl.Options.push_back(o);

	o.Data = 100;
	o.Position[0] += 32;
	o.Image = new CLabel("./Content/2D/Menu/100.tga");
	o.Image->m_Visible = false;
	SoundMenu.SFXVolumeControl.Options.push_back(o);
	SoundMenu.BGMVolumeControl.CurrentOption = &SoundMenu.BGMVolumeControl.Options[0];

	SoundMenu.SFXVolumeControl.CurrentOption = &SoundMenu.SFXVolumeControl.Options[7];

	// Rounds

	o.Data = 1;
	o.Position[0] = 32;
	o.Position[1] = 450;
	o.Image = new CLabel("./Content/2D/Menu/1.tga");
	o.Image->m_Visible = false;
	SoundMenu.BGMVolumeControl.Options.push_back(o);

	o.Data = 2;
	o.Position[0] += 32;
	o.Image = new CLabel("./Content/2D/Menu/2.tga");
	o.Image->m_Visible = false;
	SoundMenu.BGMVolumeControl.Options.push_back(o);

	o.Data = 3;
	o.Position[0] += 32;
	o.Image = new CLabel("./Content/2D/Menu/3.tga");
	o.Image->m_Visible = false;
	SoundMenu.BGMVolumeControl.Options.push_back(o);

	o.Data = 4;
	o.Position[0] += 32;
	o.Image = new CLabel("./Content/2D/Menu/4.tga");
	o.Image->m_Visible = false;
	SoundMenu.BGMVolumeControl.Options.push_back(o);

	o.Data = 5;
	o.Position[0] += 32;
	o.Image = new CLabel("./Content/2D/Menu/5.tga");
	o.Image->m_Visible = false;
	SoundMenu.BGMVolumeControl.Options.push_back(o);

	o.Data = 1;
	o.Position[0] = 32;
	o.Position[1] = 130;
	o.Image = new CLabel("./Content/2d/Menu/on.tga");
	o.Image->m_Visible = false;
	SoundMenu.SFXToggle.Options.push_back(o);

	o.Data = 0;
	o.Position[0] += 32;
	o.Image = new CLabel("./Content/2d/Menu/off.tga");
	o.Image->m_Visible = false;
	SoundMenu.SFXToggle.Options.push_back(o);

	// music toggle
	o.Data = 1;
	o.Position[0] = 32;
	o.Position[1] = 290;
	o.Image = new CLabel("./Content/2d/Menu/on.tga");
	o.Image->m_Visible = false;
	SoundMenu.BGMToggle.Options.push_back(o);

	o.Data = 0;
	o.Position[0] += 32;
	o.Image = new CLabel("./Content/2d/Menu/off.tga");
	o.Image->m_Visible = false;
	SoundMenu.BGMToggle.Options.push_back(o);

	SoundMenu.SFXToggle.Highlight	= new CLabel("./Content/2d/menu/highlight.tga");
	SoundMenu.SFXToggle.Highlight->m_Visible = false;
	CRenderInterface::GetInstance()->AddLabel(SoundMenu.SFXToggle.Highlight);
	CRenderInterface::GetInstance()->AddLabel(SoundMenu.SFXToggle.Options[0].Image);
	CRenderInterface::GetInstance()->AddLabel(SoundMenu.SFXToggle.Options[1].Image);
	CRenderInterface::GetInstance()->AddLabel(SoundMenu.BGMToggle.Options[0].Image);
	CRenderInterface::GetInstance()->AddLabel(SoundMenu.BGMToggle.Options[1].Image);

	SoundMenu.SFXToggle.Options[0].Image->m_MyBody.centerPosition = CVector3f((float)SoundMenu.SFXToggle.Options[0].Position[0],(float)SoundMenu.SFXToggle.Options[0].Position[1], 0);
	SoundMenu.SFXToggle.Options[1].Image->m_MyBody.centerPosition = CVector3f((float)SoundMenu.SFXToggle.Options[1].Position[0],(float)SoundMenu.SFXToggle.Options[1].Position[1], 0);

	SoundMenu.BGMToggle.Options[0].Image->m_MyBody.centerPosition = CVector3f((float)SoundMenu.BGMToggle.Options[0].Position[0],(float)SoundMenu.BGMToggle.Options[0].Position[1], 0);
	SoundMenu.BGMToggle.Options[1].Image->m_MyBody.centerPosition = CVector3f((float)SoundMenu.BGMToggle.Options[1].Position[0],(float)SoundMenu.BGMToggle.Options[1].Position[1], 0);

	SoundMenu.SFXToggle.CurrentOption = &SoundMenu.SFXToggle.Options[0];
	SoundMenu.BGMToggle.CurrentOption = &SoundMenu.BGMToggle.Options[0];

	SoundMenu.BGMToggle.Highlight	= new CLabel("./Content/2d/menu/highlight.tga");
	SoundMenu.BGMToggle.Highlight->m_Visible = false;
	CRenderInterface::GetInstance()->AddLabel(SoundMenu.BGMToggle.Highlight);
	SoundMenu.BGMVolumeControl.CurrentOption = &SoundMenu.BGMVolumeControl.Options[1];

	CRenderInterface::GetInstance()->AddLabel(SoundMenu.SFXVolumeControl.Highlight);
	SoundMenu.SFXVolumeControl.Highlight->m_Visible = false;
	CRenderInterface::GetInstance()->AddLabel(SoundMenu.BGMVolumeControl.Highlight);
	SoundMenu.BGMVolumeControl.Highlight->m_Visible = false;

	for(unsigned int i = 0 ; i < SoundMenu.SFXVolumeControl.Options.size(); i++)
	{
		SoundMenu.SFXVolumeControl.Options[i].Image->m_MyBody.centerPosition = CVector3f((float)SoundMenu.SFXVolumeControl.Options[i].Position[0], (float)SoundMenu.SFXVolumeControl.Options[i].Position[1], 0);
		CRenderInterface::GetInstance()->AddLabel(SoundMenu.SFXVolumeControl.Options[i].Image);

	}
	for(unsigned int i = 0; i < SoundMenu.BGMVolumeControl.Options.size(); i++)
	{
		SoundMenu.BGMVolumeControl.Options[i].Image->m_MyBody.centerPosition = CVector3f((float)SoundMenu.BGMVolumeControl.Options[i].Position[0], (float)SoundMenu.BGMVolumeControl.Options[i].Position[1], 0);
		CRenderInterface::GetInstance()->AddLabel(SoundMenu.BGMVolumeControl.Options[i].Image);
	}
}
void	CMenuSystem::DrawAudioMenu()
{
	SoundMenu.Background->m_Visible = true;
	SoundMenu.SoundEffects->m_Visible = true;
	SoundMenu.Music->m_Visible = true;
	SoundMenu.SoundVolumeImage->m_Visible = true;
	SoundMenu.Rounds->m_Visible = true;
	SoundMenu.Audio->m_Visible = true;
	SoundMenu.GameOptions->m_Visible = true;
	SoundMenu.Audio->m_Visible = true;
	SoundMenu.SFXToggle.Highlight->m_Visible = true;
	SoundMenu.SFXToggle.Highlight->m_MyBody.centerPosition = CVector3f((float)SoundMenu.SFXToggle.CurrentOption->Position[0], (float)SoundMenu.SFXToggle.CurrentOption->Position[1], 0);
	SoundMenu.SFXToggle.Options[0].Image->m_Visible = true;
	SoundMenu.SFXToggle.Options[1].Image->m_Visible = true;

	SoundMenu.BGMToggle.Highlight->m_Visible = true;
	SoundMenu.BGMToggle.Highlight->m_MyBody.centerPosition = CVector3f((float)SoundMenu.BGMToggle.CurrentOption->Position[0], (float)SoundMenu.BGMToggle.CurrentOption->Position[1], 0);
	SoundMenu.BGMToggle.Options[0].Image->m_Visible = true;
	SoundMenu.BGMToggle.Options[1].Image->m_Visible = true;

	SoundMenu.SFXVolumeControl.Highlight->m_Visible = true;
	SoundMenu.SFXVolumeControl.Highlight->m_MyBody.centerPosition = CVector3f((float)SoundMenu.SFXVolumeControl.CurrentOption->Position[0], (float)SoundMenu.SFXVolumeControl.CurrentOption->Position[1], 0);

	SoundMenu.BGMVolumeControl.Highlight->m_Visible = true;
	SoundMenu.BGMVolumeControl.Highlight->m_MyBody.centerPosition = CVector3f((float)SoundMenu.BGMVolumeControl.CurrentOption->Position[0], (float)SoundMenu.BGMVolumeControl.CurrentOption->Position[1], 0);

	for(unsigned int i =0; i < SoundMenu.SFXVolumeControl.Options.size(); i++)
	{
		SoundMenu.SFXVolumeControl.Options[i].Image->m_Visible = true;
	}

	for(unsigned int i = 0; i < SoundMenu.BGMVolumeControl.Options.size(); i++)
	{
		SoundMenu.BGMVolumeControl.Options[i].Image->m_Visible = true;
	}

	switch(SoundMenu.SelectionTracker)
	{
	case 0:
		SoundMenu.ToggleHighlight->m_Visible = true;
		SoundMenu.ToggleHighlight->m_MyBody.centerPosition = CVector3f(SoundMenu.SFXToggleSelectionCoords[0], SoundMenu.SFXToggleSelectionCoords[1], 0);
		SoundMenu.VolumeHighlight->m_Visible = false;
		SoundMenu.BackButton.Selected->m_Visible = false;
		SoundMenu.BackButton.Unselected->m_Visible = true;
		break;
	case 1:
		SoundMenu.VolumeHighlight->m_Visible = true;
		SoundMenu.VolumeHighlight->m_MyBody.centerPosition = CVector3f(SoundMenu.SFXVolumeHighlightCoords[0], SoundMenu.SFXVolumeHighlightCoords[1], 0);
		SoundMenu.ToggleHighlight->m_Visible = false;
		SoundMenu.BackButton.Selected->m_Visible = false;
		SoundMenu.BackButton.Unselected->m_Visible = true;
		break;
	case 2:
		SoundMenu.ToggleHighlight->m_Visible = true;
		SoundMenu.ToggleHighlight->m_MyBody.centerPosition = CVector3f(SoundMenu.BGMToggleSelectionCoords[0], SoundMenu.BGMToggleSelectionCoords[1], 0);
		SoundMenu.VolumeHighlight->m_Visible = false;
		SoundMenu.BackButton.Selected->m_Visible = false;
		SoundMenu.BackButton.Unselected->m_Visible = true;
		break;
	case 3:
		SoundMenu.VolumeHighlight->m_Visible = true;
		SoundMenu.VolumeHighlight->m_MyBody.centerPosition = CVector3f(SoundMenu.BGMVolumeHighlightCoords[0], SoundMenu.BGMVolumeHighlightCoords[1], 0);
		SoundMenu.ToggleHighlight->m_Visible = false;
		SoundMenu.BackButton.Selected->m_Visible = false;
		SoundMenu.BackButton.Unselected->m_Visible = true;
		break;
	case 4:
		SoundMenu.VolumeHighlight->m_Visible = false;
		SoundMenu.ToggleHighlight->m_Visible = false;
		SoundMenu.BackButton.Selected->m_Visible = true;
		SoundMenu.BackButton.Unselected->m_Visible = false;
		break;
	}
}
int		CMenuSystem::AudioMenuUpdate()
{
	if(pInput->ControllerJoystickLeft(-1) && InputTimer.GetElapsedSeconds() > 0.15)
	{
		InputTimer.Reset();
		switch(SoundMenu.SelectionTracker)
		{
		case 0:
			// SFX toggle
			SoundMenu.SFXToggle.SelectPrevious();
			break;
		case 1:
			SoundMenu.SFXVolumeControl.SelectPrevious();
			// SFX Volume
			break;
		case 2:
			SoundMenu.BGMToggle.SelectPrevious();
			// BGM toggle
			break;
		case 3:
			SoundMenu.BGMVolumeControl.SelectPrevious();
			// BGM Volume
			break;
			// 4 would be the back button, but it doesnt need left or right, so break
		default:
			break;
		}
	}
	if(pInput->ControllerJoystickRight(-1) && InputTimer.GetElapsedSeconds() > 0.15)
	{
		InputTimer.Reset();
		switch(SoundMenu.SelectionTracker)
		{
		case 0:
			// SFX toggle
			SoundMenu.SFXToggle.SelectNext();
			break;
		case 1:
			// SFX Volume
			SoundMenu.SFXVolumeControl.SelectNext();
			break;
		case 2:
			// BGM toggle
			SoundMenu.BGMToggle.SelectNext();
			break;
		case 3:
			// BGM Volume
			SoundMenu.BGMVolumeControl.SelectNext();
			break;
			// 4 would be the back button, but it doesnt need left or right, so break
		default:
			break;
		}
	}
	if(pInput->ControllerJoystickUp(-1) && InputTimer.GetElapsedSeconds() > 0.15)
	{
		InputTimer.Reset();
		if(SoundMenu.SelectionTracker > 0)
		{
			SoundMenu.SelectionTracker--;
		}
		else
			SoundMenu.SelectionTracker = 4;
	}
	if(pInput->ControllerJoystickDown(-1) && InputTimer.GetElapsedSeconds() > 0.15)
	{
		InputTimer.Reset();
		if(SoundMenu.SelectionTracker < 4)
		{
			SoundMenu.SelectionTracker++;
		}
		else
			SoundMenu.SelectionTracker = 0;
	}
	if(pInput->ControllerJoyPress(0, CGameProfile::GetInstance()->ButtonLayout[0]._shoot) || pInput->KeyboardKeyPress(DIK_RETURN))
	{
		InputTimer.Reset();
		if(SoundMenu.SelectionTracker == 4)
		{
			ShutOffAudioMenu();
			// reset the selection tracker to 0
			// turn main menu back on
			SaturationBombingLogo->m_Visible = true;
			for(unsigned int i = 0; i < MainMenu.MainMenuButtonIDs.size(); i++)
			{
				MainMenu.ButtonMasterIndex[MainMenu.MainMenuButtonIDs[i]].Unselected->m_Visible = true;
			}
			MainMenu.CurrentMenuButton = &MainMenu.ButtonMasterIndex[MainMenu.MainMenuButtonIDs[0]];
			MainMenu.CurrentMenuButton->Selected->m_Visible = true;
			MainMenu.CurrentMenuButton->Unselected->m_Visible = false;
			SoundMenu.SelectionTracker = 0;
			return ChangeToMainMenu;
		}
	}
	return -1;
}
void	CMenuSystem::InitializeControlsMenu()
{

}
void	CMenuSystem::DrawControlsMenu()
{

}
int		CMenuSystem::ControlsMenuUpdate()
{
	return -1;
}
void	CMenuSystem::CreditsSequence()
{
	Creditsbackground->m_Visible = true;
	if(Creditstimer.GetElapsedSeconds() >= 0.03333)
	{
		Creditstimer.Reset();
		Credits[creditscounter]->m_Visible = false;
		creditscounter++;
		if(creditscounter > 55)
			creditscounter = 0;
		Credits[creditscounter]->m_Visible = true;

	}

	// Do credits stuff here.
	if(pInput->ControllerJoystickLeft(-1) && InputTimer.GetElapsedSeconds() > 0.15)
		incredits = false;
	if(pInput->ControllerJoystickRight(-1) && InputTimer.GetElapsedSeconds() > 0.15)
		incredits = false;
	if(pInput->ControllerJoystickUp(-1) && InputTimer.GetElapsedSeconds() > 0.15)
		incredits = false;
	if(pInput->ControllerJoystickDown(-1) && InputTimer.GetElapsedSeconds() > 0.15)
		incredits = false;
//	if(pInput->ControllerJoyPress(0, CGameProfile::GetInstance()->ButtonLayout[0]._shoot) || pInput->KeyboardKeyPress(DIK_RETURN))
//		incredits = false;

	if(incredits == false)
	{
		HideCredits();
	}
}
void CMenuSystem::CreditsCleanup()
{
	// DO any necessary cleanup from credits here
	return;
}
void CMenuSystem::CreditsInitialize()
{
	for(int i = 0; i < 56; i++)
	{
		char	filepath[MAX_STRING_LENGTH];
		char	imagenum[3];
		itoa(i+1, imagenum, 10);
		strcpy(filepath, "./content/2d/menu/credits/frame0");
		if(i < 9)
			strcat(filepath, "0");

		strcat(filepath, imagenum);
		strcat(filepath, ".jpg");
		Credits[i] = new CLabel(filepath);
		Credits[i]->m_Visible = false;
		Credits[i]->m_MyBody.centerPosition = CVector3f(720.0f, 545.0f, 0);
		CRenderInterface::GetInstance()->AddLabel(Credits[i]);
	}
	creditscounter = 0;
}
