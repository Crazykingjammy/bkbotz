#include ".\menusystem.h"
CMenuSystem *CMenuSystem::pInst = 0;
CMenuSystem::CMenuSystem(void)
{
}

CMenuSystem::~CMenuSystem(void)
{
}

bool	CMenuSystem::Initialize(IDirect3DDevice9 *d3ddev)
{
	this->xml = NULL;
	//CoInitialize(NULL);
	//CoCreateInstance(__uuidof(SAXXMLReader40), NULL, CLSCTX_ALL, __uuidof(ISAXXMLReader), (void **)&xml);
	//xml->putContentHandler(&Script);
	//wchar_t tmp[MAX_STRING_LENGTH];
	//mbstowcs(tmp, "./source/menu/menu.xml", MAX_STRING_LENGTH);
	//xml->parseURL(tmp);

	this->Direct3DDevice = d3ddev;
	//CurrentMenu = 0;
	InitializeMainMenu();
	RunOnceDuringUpdate = true;
	return true;
}
int		CMenuSystem::Update(int MenuToUse)
{
	if(RunOnceDuringUpdate)
	{
		Direct3DDevice->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0xaaaaaaaa, 1.0f, 0);
		AnimationTimer.Reset();
		InputTimer.Reset();
		DrawTimer.Reset();
		RunOnceDuringUpdate = false;
		MainMenu.CurrentMenuButton = &MainMenu.ButtonMasterIndex[0];
		Draw(0);
		MainMenuUpdate();
	}
	int ReturnValue = 0;

	switch(MenuToUse)
	{
	case 0:
		ReturnValue = MainMenuUpdate();
		break;
	}
	Draw(MenuToUse);

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
		Direct3DDevice->BeginScene();
		Direct3DDevice->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0xaaaaaaaa, 1.0f, 0);
		DrawTimer.Reset();
		switch(MenuToDraw)
		{
		case 0:
			MainMenuDraw();
			break;
		}
		Direct3DDevice->EndScene();
		Direct3DDevice->Present(0, 0, 0, 0);
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
		}
	}
}
void	CMenuSystem::InitializeMainMenu()
{
	MainMenu.CurrentMenuButton = NULL;
	MainMenu.CurrentSubmenu = NULL;

	strcpy(MainMenu.MenuBackgroundImage, "./Content/2D/Menu/MainMenuBackground.bmp");
	strcpy(MainMenu.UnselectedBackgroundImage, "./Content/2D/Menu/InactiveBackground.bmp");
	strcpy(MainMenu.SelectedBackgroundImage, "./Content/2D/Menu/ActiveBackground.bmp");
	strcpy(MainMenu.MenuBGMPath, "./Content/Audio/MenuMusic.wav");

	MainMenu.MenuBackground = new CBitmap2D(this->Direct3DDevice, MainMenu.MenuBackgroundImage);
	MainMenu.UnselectedButtonBackground = new CBitmap2D(this->Direct3DDevice, MainMenu.UnselectedBackgroundImage);
	MainMenu.SelectedButtonBackground = new CBitmap2D(this->Direct3DDevice, MainMenu.SelectedBackgroundImage);

	SaturationBombingLogo = new CBitmap2D(this->Direct3DDevice, "./Content/2D/Menu/SatuartionBombing.tga");
	KeyTest = new CBitmap2D(this->Direct3DDevice, "./Content/2D/Menu/Test.bmp");

	MenuButton m;
	m.ID = 0;
	m.HasASubmenu = true;
	m.IsInASubmenu = false;
	m.ActionCode = ActivateSubMenu;
	m.SubmenuID = 0;
	m.CP[0] = 100;
	m.CP[1] = 575;
	m.SP[1] = 575;
	strcpy(m.SelectedImage, "./Content/2D/Menu/SinglePlayerSelectedText.tga");
	strcpy(m.UnselectedImage, "./Content/2D/Menu/SinglePlayer.tga");
	m.PlayAnimationWhenSelected = false;
	m.AnimationID = -1;
	MainMenu.ButtonMasterIndex.push_back(m);

	m.ID = 1;
	m.HasASubmenu = true;
	m.IsInASubmenu = false;
	m.ActionCode = ActivateSubMenu;
	m.SubmenuID = 1;
	m.CP[0] = 300;
	m.CP[1] = 575;
	strcpy(m.SelectedImage, "./Content/2D/Menu/MultiplayerSelectedText.tga");
	strcpy(m.UnselectedImage, "./Content/2D/Menu/Multiplayer.tga");
	m.PlayAnimationWhenSelected = false;
	m.AnimationID = -1;
	MainMenu.ButtonMasterIndex.push_back(m);

	m.ID = 2;
	m.HasASubmenu = true;
	m.IsInASubmenu = false;
	m.ActionCode = ActivateSubMenu;
	m.SubmenuID = 2;
	m.CP[0] = 500;
	m.CP[1] = 575;
	strcpy(m.SelectedImage, "./Content/2D/Menu/OptionsSelectedText.tga");
	strcpy(m.UnselectedImage, "./Content/2D/Menu/Options.tga");
	m.PlayAnimationWhenSelected = false;
	m.AnimationID = -1;
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

	m.ID = 4;
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
	MainMenu.ButtonMasterIndex.push_back(m);

	m.ID = 6;
	m.HasASubmenu =false;
	m.IsInASubmenu = true;
	m.ActionCode = StartMultiplayerGame;
	m.SubmenuID = 1;
	m.CP[0] = 300;
	m.CP[1] = 525;
	strcpy(m.SelectedImage, "./Content/2D/Menu/QuickStart.tga");
	strcpy(m.UnselectedImage, "./Content/2D/Menu/QuickStart.tga");
	m.PlayAnimationWhenSelected = true;
	m.AnimationID = 0;
	MainMenu.ButtonMasterIndex.push_back(m);

	m.ID = 7;
	m.HasASubmenu =false;
	m.IsInASubmenu = true;
	m.ActionCode = StartMultiplayerGame;
	m.SubmenuID = 1;
	m.CP[0] = 300;
	m.CP[1] = 475;
	strcpy(m.SelectedImage, "./Content/2D/Menu/SelectGame.tga");
	strcpy(m.UnselectedImage, "./Content/2D/Menu/SelectGame.tga");
	m.PlayAnimationWhenSelected = true;
	m.AnimationID = 0;
	MainMenu.ButtonMasterIndex.push_back(m);

	m.ID = 8;
	m.HasASubmenu =false;
	m.IsInASubmenu = true;
	m.ActionCode = ActivateAudioMenu;
	m.SubmenuID = 2;
	m.CP[0] = 500;
	m.CP[1] = 525;
	strcpy(m.SelectedImage, "./Content/2D/Menu/Audio.tga");
	strcpy(m.UnselectedImage, "./Content/2D/Menu/Audio.tga");
	m.PlayAnimationWhenSelected = true;
	m.AnimationID = 0;
	MainMenu.ButtonMasterIndex.push_back(m);

	m.ID = 9;
	m.HasASubmenu =false;
	m.IsInASubmenu = true;
	m.ActionCode = ActivateVideoMenu;
	m.SubmenuID = 2;
	m.CP[0] = 500;
	m.CP[1] = 475;
	strcpy(m.SelectedImage, "./Content/2D/Menu/Video.tga");
	strcpy(m.UnselectedImage, "./Content/2D/Menu/Video.tga");
	m.PlayAnimationWhenSelected = true;
	m.AnimationID = 0;
	MainMenu.ButtonMasterIndex.push_back(m);

	m.ID = 10;
	m.HasASubmenu =false;
	m.IsInASubmenu = true;
	m.ActionCode = ActivateCredits;
	m.SubmenuID = 2;
	m.CP[0] = 500;
	m.CP[1] = 425;
	strcpy(m.SelectedImage, "./Content/2D/Menu/Credits.tga");
	strcpy(m.UnselectedImage, "./Content/2D/Menu/Credits.tga");
	m.PlayAnimationWhenSelected = true;
	m.AnimationID = 0;
	MainMenu.ButtonMasterIndex.push_back(m);

	for(unsigned int i = 0; i < MainMenu.ButtonMasterIndex.size(); i++)
	{
		MainMenu.ButtonMasterIndex[i].Selected = new CBitmap2D(this->Direct3DDevice, MainMenu.ButtonMasterIndex[i].SelectedImage);
		MainMenu.ButtonMasterIndex[i].Unselected = new CBitmap2D(this->Direct3DDevice, MainMenu.ButtonMasterIndex[i].UnselectedImage);
	}

	Submenu s;
	s.ID = 0;
	s.Active = false;
	s.Openspeed = 2;
	s.OpenedByButtonID = 0;
	s.ButtonIDs.push_back(4);
	s.ButtonIDs.push_back(5);
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
	s.StartingPosition[0] = 300;
	s.StartingPosition[1] = 550;
	s.FrameCount = (int)(s.ButtonIDs.size() * 4);
	MainMenu.SubmenuIndex.push_back(s);

	s.ButtonIDs.clear();
	s.ID = 2;
	s.Active = false;
	s.Openspeed = 2;
	s.OpenedByButtonID = 2;
	s.ButtonIDs.push_back(8);
	s.ButtonIDs.push_back(9);
	s.ButtonIDs.push_back(10);
	s.StartingPosition[0] = 500;
	s.StartingPosition[1] = 550;
	MainMenu.SubmenuIndex.push_back(s);

	Animation a;
	a.ID = 0;
	strcpy(a.BackgroundPath, "./Content/2D/Menu/SelectedBackground.bmp");
	a.FrameCount = 4;
	a.ImagePaths.push_back("./Content/2D/Menu/SelectedAnimationFrame1.bmp");
	a.ImagePaths.push_back("./Content/2D/Menu/SelectedAnimationFrame2.bmp");
	a.ImagePaths.push_back("./Content/2D/Menu/SelectedAnimationFrame3.bmp");
	a.ImagePaths.push_back("./Content/2D/Menu/SelectedAnimationFrame4.bmp");
	a.FrameTimer = 0.25f;

	MainMenu.AnimationIndex.push_back(a);

	for(unsigned int i = 0; i < MainMenu.AnimationIndex.size(); i++)
	{
		MainMenu.AnimationIndex[i].Background = new CBitmap2D(Direct3DDevice, MainMenu.AnimationIndex[i].BackgroundPath);
		for(unsigned int j = 0; j < MainMenu.AnimationIndex[i].ImagePaths.size(); j++)
		{
			CBitmap2D *tmp = new CBitmap2D(Direct3DDevice, MainMenu.AnimationIndex[i].ImagePaths[j]);
			MainMenu.AnimationIndex[i].Frames.push_back(tmp);
		}
	}
	MainMenu.CurrentMenuButton = &MainMenu.ButtonMasterIndex[0];
	MainMenu.MainMenuButtonIDs.push_back(0);
	MainMenu.MainMenuButtonIDs.push_back(1);
	MainMenu.MainMenuButtonIDs.push_back(2);
	MainMenu.MainMenuButtonIDs.push_back(3);

	HRESULT hr = 0;
	ID3DXBuffer* adjBuffer	= 0;
	ID3DXBuffer* mtrlBuffer = 0;
	DWORD        numMtrls	= 0;

	//Load the xfile here bitch.
	hr = D3DXLoadMeshFromX("./Content/2D/Menu/Pyramid.x",D3DXMESH_MANAGED,Direct3DDevice,
		&adjBuffer,&mtrlBuffer,0,&numMtrls,&PyramidModel.m_Mesh);

	if(FAILED(hr))
	{
		bool Bad = true;
	}

	if( mtrlBuffer != 0 && numMtrls != 0 )
	{
		D3DXMATERIAL* mtrls = (D3DXMATERIAL*)mtrlBuffer->GetBufferPointer();
		//D3DMATERIAL9

		for(int i = 0; i < (signed)numMtrls; i++)
		{
			// the MatD3D property doesn't have an ambient value set
			// when its loaded, so set it now:
			mtrls[i].MatD3D.Ambient = mtrls[i].MatD3D.Diffuse;


			// save the ith material
			PyramidModel.m_Materials.push_back( mtrls[i].MatD3D );

			// check if the ith material has an associative texture
			if( mtrls[i].pTextureFilename != 0 )
			{
				// yes, load the texture for the ith subset
				IDirect3DTexture9* tex = 0;
				D3DXCreateTextureFromFile(
					Direct3DDevice,
					mtrls[i].pTextureFilename,
					&tex);

				// save the loaded texture
				PyramidModel.m_Textures.push_back( tex );
			}
			else
			{
				// no texture for the ith subset
				PyramidModel.m_Textures.push_back( 0 );
			}
		}
	}
}
void	CMenuSystem::MainMenuDraw()
{
	// alpha checking bitmap
	KeyTest->Draw(400.0f, 300.0f, 1.0f);

	DrawTheGodDamnPyramid();
	// main menu background images
	MainMenu.MenuBackground->Draw(400.0f, 300.0f);
	SaturationBombingLogo->Draw(400.0f, 285.0f);

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
				}
				else
				{
					MainMenu.ButtonMasterIndex[MainMenu.SubmenuIndex[i].ButtonIDs[j]].SP[1] = MainMenu.ButtonMasterIndex[MainMenu.SubmenuIndex[i].ButtonIDs[j]].CP[1];
				}
				MainMenu.SelectedButtonBackground->Draw((float)MainMenu.SubmenuIndex[i].StartingPosition[0], (float)MainMenu.ButtonMasterIndex[MainMenu.SubmenuIndex[i].ButtonIDs[j]].SP[1]);
				MainMenu.ButtonMasterIndex[MainMenu.SubmenuIndex[i].ButtonIDs[j]].Unselected->Draw((float)MainMenu.SubmenuIndex[i].StartingPosition[0], (float)MainMenu.ButtonMasterIndex[MainMenu.SubmenuIndex[i].ButtonIDs[j]].SP[1]);
			}
			break;
		}
		else
			continue;
	}


	// Draw Current menu button
	if(MainMenu.CurrentMenuButton->PlayAnimationWhenSelected == false)
	{
		MainMenu.SelectedButtonBackground->Draw((float)MainMenu.CurrentMenuButton->CP[0], (float)MainMenu.CurrentMenuButton->CP[1]);
		MainMenu.CurrentMenuButton->Selected->Draw((float)MainMenu.CurrentMenuButton->CP[0], (float)MainMenu.CurrentMenuButton->CP[1]);
	}
	else
	{
			Animation *a = &MainMenu.AnimationIndex[MainMenu.CurrentMenuButton->AnimationID];
			static int AnimationFrame = 0;
			a->Background->Draw((float)MainMenu.CurrentMenuButton->CP[0], (float)MainMenu.CurrentMenuButton->CP[1]);
			a->Frames[AnimationFrame]->Draw((float)MainMenu.CurrentMenuButton->CP[0], (float)MainMenu.CurrentMenuButton->CP[1]);
			MainMenu.CurrentMenuButton->Selected->Draw((float)MainMenu.CurrentMenuButton->CP[0], (float)MainMenu.CurrentMenuButton->CP[1]);
			if(AnimationTimer.GetElapsedSeconds() >= 0.15)
			{
				AnimationTimer.Reset();
				AnimationFrame++;
				if(AnimationFrame >= a->FrameCount)
				{
					AnimationFrame = 0;
				}
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
			MainMenu.SelectedButtonBackground->Draw((float)MainMenu.ButtonMasterIndex[MainMenu.MainMenuButtonIDs[i]].CP[0], (float)MainMenu.ButtonMasterIndex[MainMenu.MainMenuButtonIDs[i]].CP[1]);
			MainMenu.ButtonMasterIndex[MainMenu.MainMenuButtonIDs[i]].Selected->Draw((float)MainMenu.ButtonMasterIndex[MainMenu.MainMenuButtonIDs[i]].CP[0], (float)MainMenu.ButtonMasterIndex[MainMenu.MainMenuButtonIDs[i]].CP[1]);
		}
		else
		{
			MainMenu.UnselectedButtonBackground->Draw((float)MainMenu.ButtonMasterIndex[MainMenu.MainMenuButtonIDs[i]].CP[0], (float)MainMenu.ButtonMasterIndex[MainMenu.MainMenuButtonIDs[i]].CP[1]);
			MainMenu.ButtonMasterIndex[MainMenu.MainMenuButtonIDs[i]].Unselected->Draw((float)MainMenu.ButtonMasterIndex[MainMenu.MainMenuButtonIDs[i]].CP[0], (float)MainMenu.ButtonMasterIndex[MainMenu.MainMenuButtonIDs[i]].CP[1]);
		}
	}
}
int		CMenuSystem::MainMenuUpdate()
{
	int ActionResult = 0;
	if(GetAsyncKeyState(VK_LEFT)		&& InputTimer.GetElapsedSeconds() > 0.15)
	{
		InputTimer.Reset();
		ResetAllSubmenus();
		if(MainMenu.CurrentMenuButton->IsInASubmenu)
		{
			if(MainMenu.SubmenuIndex[MainMenu.CurrentMenuButton->SubmenuID].Active)
			{
				MainMenu.SubmenuIndex[MainMenu.CurrentMenuButton->SubmenuID].Active = false;
			}
			int newButtonID = MainMenu.SubmenuIndex[MainMenu.CurrentMenuButton->SubmenuID].OpenedByButtonID;

			if(newButtonID == 0)
			{
				MainMenu.CurrentMenuButton = &MainMenu.ButtonMasterIndex[3];
			}
			else if(newButtonID == 1)
			{
				MainMenu.CurrentMenuButton = &MainMenu.ButtonMasterIndex[0];
			}
			else if(newButtonID == 2)
			{
				MainMenu.CurrentMenuButton = &MainMenu.ButtonMasterIndex[1];
			}
			else if(newButtonID == 3)
			{
				MainMenu.CurrentMenuButton = &MainMenu.ButtonMasterIndex[2];
			}

			if(MainMenu.CurrentMenuButton->HasASubmenu)
			{
				if(MainMenu.SubmenuIndex[MainMenu.CurrentMenuButton->SubmenuID].Active)
					MainMenu.SubmenuIndex[MainMenu.CurrentMenuButton->SubmenuID].Active = false;
			}
		}
		else
		{
			if(MainMenu.CurrentMenuButton->ID == 0)
			{
				MainMenu.CurrentMenuButton = &MainMenu.ButtonMasterIndex[3];
			}
			else if(MainMenu.CurrentMenuButton->ID == 1)
			{
				MainMenu.CurrentMenuButton = &MainMenu.ButtonMasterIndex[0];
			}
			else if(MainMenu.CurrentMenuButton->ID == 2)
			{
				MainMenu.CurrentMenuButton = &MainMenu.ButtonMasterIndex[1];
			}
			else if(MainMenu.CurrentMenuButton->ID == 3)
			{
				MainMenu.CurrentMenuButton = &MainMenu.ButtonMasterIndex[2];
			}

			if(MainMenu.CurrentMenuButton->HasASubmenu)
			{
				if(MainMenu.SubmenuIndex[MainMenu.CurrentMenuButton->SubmenuID].Active)
					MainMenu.SubmenuIndex[MainMenu.CurrentMenuButton->SubmenuID].Active = false;
			}
		}
	}

	if(GetAsyncKeyState(VK_RIGHT)	&& InputTimer.GetElapsedSeconds() > 0.15)
	{
		InputTimer.Reset();
		ResetAllSubmenus();
		if(MainMenu.CurrentMenuButton->IsInASubmenu)
		{
			MainMenu.SubmenuIndex[MainMenu.CurrentMenuButton->SubmenuID].Active = false;

			int newButtonID = MainMenu.SubmenuIndex[MainMenu.CurrentMenuButton->SubmenuID].OpenedByButtonID;

			if(newButtonID == 0)
			{
				MainMenu.CurrentMenuButton = &MainMenu.ButtonMasterIndex[1];
			}
			else if(newButtonID == 1)
			{
				MainMenu.CurrentMenuButton = &MainMenu.ButtonMasterIndex[2];
			}
			else if(newButtonID == 2)
			{
				MainMenu.CurrentMenuButton = &MainMenu.ButtonMasterIndex[3];
			}
			else if(newButtonID == 3)
			{
				MainMenu.CurrentMenuButton = &MainMenu.ButtonMasterIndex[0];
			}
		}
		else
		{
			if(MainMenu.CurrentMenuButton->ID == 0)
			{
				MainMenu.CurrentMenuButton = &MainMenu.ButtonMasterIndex[1];
			}
			else if(MainMenu.CurrentMenuButton->ID == 1)
			{
				MainMenu.CurrentMenuButton = &MainMenu.ButtonMasterIndex[2];
			}
			else if(MainMenu.CurrentMenuButton->ID == 2)
			{
				MainMenu.CurrentMenuButton = &MainMenu.ButtonMasterIndex[3];
			}
			else if(MainMenu.CurrentMenuButton->ID == 3)
			{
				MainMenu.CurrentMenuButton = &MainMenu.ButtonMasterIndex[0];
			}

			if(MainMenu.CurrentMenuButton->HasASubmenu)
			{
				if(MainMenu.SubmenuIndex[MainMenu.CurrentMenuButton->SubmenuID].Active)
					MainMenu.SubmenuIndex[MainMenu.CurrentMenuButton->SubmenuID].Active = false;
			}
		}
	}
	if(GetAsyncKeyState(VK_DOWN)	&& InputTimer.GetElapsedSeconds() > 0.15)
	{
		InputTimer.Reset();
		if(MainMenu.CurrentMenuButton->HasASubmenu && MainMenu.SubmenuIndex[MainMenu.CurrentMenuButton->SubmenuID].Active)
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
	if(GetAsyncKeyState(VK_UP)			&& InputTimer.GetElapsedSeconds() > 0.15)
	{
		InputTimer.Reset();
		if(MainMenu.CurrentMenuButton->HasASubmenu && MainMenu.SubmenuIndex[MainMenu.CurrentMenuButton->SubmenuID].Active)
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
	if(GetAsyncKeyState(VK_RETURN)	&& InputTimer.GetElapsedSeconds() > 0.15)
	{
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
		default:
			break;
		}
	}
	return ActionResult;
}
void	CMenuSystem::DrawTheGodDamnPyramid()
{
	// draw the mesh
	D3DXMATRIX Position;
	D3DXMATRIX Orientation;
	D3DXMATRIX OrientationX;
	D3DXMATRIX OrientationZ;
	D3DXMATRIX World;

	static float Yrot = 0;

	Yrot+= 0.025f;

	//Write Transformation
	D3DXMatrixTranslation(&Position,0,-2.5,10);

	//Write Orientation
	D3DXMatrixRotationY(&Orientation,Yrot);
	D3DXMatrixRotationX(&OrientationX,0);
	D3DXMatrixRotationZ(&OrientationZ,0);

	World = Orientation * OrientationX * OrientationZ * Position;

	//Transform
	Direct3DDevice->SetTransform(D3DTS_WORLD,&World);

	for(int i = 0; i < (signed)PyramidModel.m_Materials.size(); i++)
	{
		Direct3DDevice->SetMaterial( &PyramidModel.m_Materials[i] );
		Direct3DDevice->SetTexture(0, PyramidModel.m_Textures[i]);
		PyramidModel.m_Mesh->DrawSubset(i);
	}	
}
void	CMenuSystem::InitializeSinglePlayerGameMenu()
{
	
}
void	CMenuSystem::SinglePlayerGameMenuDraw()
{

}
void	CMenuSystem::SinglePlayerGameMenuUpdate()
{

}
void	CMenuSystem::InitializeMultiplayerGameMenu()
{

}
void	CMenuSystem::MultiplayerGameMenuDraw()
{

}
void	CMenuSystem::MultiplayerGameMenuUpdate()
{

}