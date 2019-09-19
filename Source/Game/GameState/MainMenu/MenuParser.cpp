//#include "stdafx.h"
#include ".\menuparser.h"

MenuParser::MenuParser(void)
{
	// tags used multiple places
	  NumberOfButtons = false;
	  ID = false;

	// Opening tag for menu index
	  MenuStart = false;

	// Opening tag for an animation
	  AnimationStart = false;
	  Background = false;			// background animation image
	  FrameCount = false;		// # of frames in the animation
	  FrameTimer = false;		// time between frames
	  FrameImage = false;		//	each frame's image

	// Opening of the main menu
	  MainMenu = false;
	  MenuBGM = false;						// BGM for menu
	  MainMenuBKG = false;
	  SelectionChangeSound = false;		// sound effect to play on selection change
	  NumberOfSubmenus = false;		// how many submenus are present?
	  UnselectedBackground = false;
	  SelectedBackground = false;
	// the start of a button
	  ButtonStart = false;
	// does this button open a submenu?
	  HasASubmenu = false;

	  SubmenuID = false;

	// what does this button do when it's pressed?
	  ActionCode = false;

	// center position
	  CenterPointX = false;
	  CenterPointY = false;

	// image to be displayed when the button is selected
	  SelectedImage = false;

	// the unselected or "normal" image
	  UnselectedImage = false;

	// does this button have an animation linked to it
	  PlayAnimationWhenSelected = false;

	// if yes, it's animation, if not, it's -1
	  AnimationID = false;

	// start of a submenu
	  SubmenuStart = false;
	// what button opens this submenu?
	  OpenedByButton = false;

	// how fast does this menu open?
	  OpenSpeed = false;

	  ButtonIDs = false;
}

MenuParser::~MenuParser(void)
{
}

HRESULT STDMETHODCALLTYPE MenuParser::characters(const wchar_t *pwchChars, int cchChars)
{
	static int ButtonTags = 0;
	
	static int MenuTags = 0;
	static int MenuButtons = 0;
	static int SubmenuCount = 0;

	static int SubmenuTags = 0;
	static int SubmentButtons = 0;

	static int AnimationTags = 0;
	static int AnimationFrames = 0;

	int temp = cchChars;
	char buffer[MAX_STRING_LENGTH];
	for(int i = 0; i < MAX_STRING_LENGTH; ++i)
	{
		buffer[i] = ' ';
	}
	wcstombs(buffer, pwchChars, (MAX_STRING_LENGTH - 1));
	if(cchChars > 0) buffer[cchChars] = '\0';

	/*if(MenuStart)
	{
		MenuStart = false;
	}
	if(AnimationStart && ID)
	{
		AnimationStart = false;
		ID = false;
		TempAnimation.ID = atoi(buffer);
		AnimationTags++;
	}
	if(Background)
	{
		Background = false;
		AnimationTags++;
		strcpy(TempAnimation.BackgroundPath, buffer);		
	}
	if(FrameCount)
	{
		FrameCount = false;
		AnimationTags++;
		AnimationFrames = atoi(buffer);
	}
	if(FrameTimer)
	{
		FrameTimer = false;
		AnimationTags++;
		TempAnimation.FrameTimer = (float)atof(buffer);
	}
	if(FrameImage)
	{
		FrameImage = false;
		char tmp[MAX_STRING_LENGTH];
		strcpy(tmp, buffer);
		TempAnimation.ImagePaths.push_back(tmp);
	}

	if(AnimationTags == ANIMATION_TAGS && TempAnimation.ImagePaths.size() == AnimationFrames)
	{
		// animation complete
		AnimationTags = 0;
		AnimationFrames = 0;
//		TempMenu.AnimationIndex.push_back(TempAnimation);
	}
	
	if(MainMenu)
	{
		MainMenu = false;
	}
	if(NumberOfButtons)
	{
		NumberOfButtons = false;
		MenuTags++;
		MenuButtons = atoi(buffer);
	}
	if(NumberOfSubmenus)
	{
		NumberOfSubmenus = false;
		MenuTags++;
		SubmenuCount = atoi(buffer);
	}
	if(SelectionChangeSound)
	{
		SelectionChangeSound = false;
		MenuTags++;
		strcpy(TempMenu.SelectionChangedSoundPath, buffer);
	}
	if(MenuBGM)
	{
		MenuBGM = false;
		MenuTags++;
		strcpy(TempMenu.MenuBGMPath, buffer);
	}
	if(MainMenuBKG)
	{
		MainMenuBKG = false;
		MenuTags++;
		strcpy(TempMenu.MenuBackgroundImage, buffer);
	}
	if(UnselectedBackground)
	{
		UnselectedBackground = false;
		MenuTags++;
		strcpy(TempMenu.UnselectedBackgroundImage, buffer);
	}
	if(SelectedBackground)
	{
		SelectedBackground = false;
		MenuTags++;
		strcpy(TempMenu.SelectedBackgroundImage, buffer);
	}
	if(ButtonStart && ID)
	{
		ButtonStart = false;
		ID = false;
		ButtonTags++;
		TempButton.ID = atoi(buffer);
	}
	if(HasASubmenu)
	{
		HasASubmenu = false;
		ButtonTags++;
		TempButton.HasASubmenu = (bool)atoi(buffer);
	}
	if(SubmenuID)
	{
		SubmenuID = false;
		ButtonTags++;
		TempButton.SubmenuID = atoi(buffer);
	}
	if(ActionCode)
	{
		ActionCode = false;
		ButtonTags++;
		TempButton.ActionCode = atoi(buffer);
	}
	if(CenterPointX)
	{
		CenterPointX = false;
		ButtonTags++;
		TempButton.CP[0] = atoi(buffer);
	}
	if(CenterPointY)
	{
		CenterPointY = false;
		ButtonTags++;
		TempButton.CP[1] = atoi(buffer);
	}
	if(SelectedImage)
	{
		SelectedImage = false;
		ButtonTags++;
		strcpy(TempButton.SelectedImage, buffer);
	}
	if(UnselectedImage)
	{
		UnselectedImage = false;
		ButtonTags++;
		strcpy(TempButton.UnselectedImage, buffer);
	}
	if(PlayAnimationWhenSelected)
	{
		PlayAnimationWhenSelected = false;
		ButtonTags++;
		TempButton.PlayAnimationWhenSelected = (bool)atoi(buffer);
	}
	if(AnimationID)
	{
		AnimationID = false;
		ButtonTags++;
		TempButton.AnimationID = atoi(buffer);
	}
	if(SubmenuID)
	{
		SubmenuID = false;
		ButtonTags++;
		TempButton.SubmenuID = atoi(buffer);
	}

	if(ButtonTags == BUTTON_TAGS)
	{
		ButtonTags = 0;
		TempMenu.ButtonMasterIndex.push_back(TempButton);
	}
	if(SubmenuStart && ID)
	{
		SubmenuStart = false;
		ID = false;
		TempSubmenu.ID = atoi(buffer);
	}
	if(OpenedByButton)
	{
		OpenedByButton = false;
		SubmenuTags++;
		TempSubmenu.OpenedByButtonID = atoi(buffer);
	}
	if(OpenSpeed)
	{
		OpenSpeed = false;
		SubmenuTags++;
		TempSubmenu.Openspeed = atoi(buffer);
	}
	if(NumberOfButtons)
	{
		NumberOfButtons = false;
		SubmenuTags++;
		SubmentButtons = atoi(buffer);
	}
	if(ButtonIDs && ID)
	{
		ID= false;
		TempSubmenu.ButtonIDs.push_back(atoi(buffer));
	}
	if(SubmenuTags == SUBMENU_TAGS && TempSubmenu.ButtonIDs.size() == SubmentButtons)
	{
		SubmenuTags = 0;
		SubmentButtons = 0;
		TempMenu.SubmenuIndex.push_back(TempSubmenu);
	}*/
	return S_OK;
}