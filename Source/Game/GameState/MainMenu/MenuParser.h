#pragma once
#include <vector>
#include <string>
#include <MsXml.h>
#include <MsXml2.h>
#include <d3d9.h>
#include <d3dx9.h>

#include "./2DOverlay.h"

#pragma comment(lib, "d3d9")
#pragma comment(lib, "d3dx9")
using namespace std;
#ifndef MAX_STRING_LENGTH
#define MAX_STRING_LENGTH 256
#endif

#ifndef BUTTON_TAGS
#define BUTTON_TAGS 10
#endif

#ifndef ANIMATION_TAGS
#define ANIMATION_TAGS 4 /* + The number of frames*/
#endif

#ifndef MAINMENU_TAGS
#define MAINMENU_TAGS 7 /* + The number of buttons + the number of submenus*/
#endif

#ifndef SUBMENU_TAGS
#define SUBMENU_TAGS 5 /* + The number of buttons */
#endif
enum DataTypes
{
	integer = 0,
	floatingpoint,
	NumDataTypes
};
struct ModelData
{
	int ID;
	vector<D3DMATERIAL9>			m_Materials;
	vector<IDirect3DTexture9*>		m_Textures;
	ID3DXMesh*						m_Mesh;
};

struct Animation
{
	int ID;
	int FrameCount;
	char BackgroundPath[MAX_STRING_LENGTH];
	CBitmap2D *Background;
	vector<char *> ImagePaths;
	vector<CBitmap2D*> Frames;
	float FrameTimer;
};

struct MenuButton
{
	int								ID;
	int								CP[2];
	int								SP[2];
	bool							HasASubmenu;
	bool							IsInASubmenu;
	bool							PlayAnimationWhenSelected;
	int								AnimationID;
	int								SubmenuID;
	int								ActionCode;
	char							SelectedImage[MAX_STRING_LENGTH];
	char							UnselectedImage[MAX_STRING_LENGTH];
	CBitmap2D					*Selected;
	CBitmap2D					*Unselected;
};

struct Submenu
{
	int								ID;
	bool							Active;
	bool							Animating;
	vector<int>				ButtonIDs;
	int								Openspeed;
	int								FrameCount;
	int								OpenedByButtonID;
	int								StartingPosition[2];
};

struct MainMenuData
{
	Submenu					*CurrentSubmenu;
	MenuButton				*CurrentMenuButton;
	
	int								ID;
	int								Type;

	vector<MenuButton>	ButtonMasterIndex;
	vector<Submenu>		SubmenuIndex;
	vector<int>				MainMenuButtonIDs;
	vector<Animation>		AnimationIndex;

	char							MenuBackgroundImage[MAX_STRING_LENGTH];
	char							UnselectedBackgroundImage[MAX_STRING_LENGTH];
	char							SelectedBackgroundImage[MAX_STRING_LENGTH];
	char							MenuBGMPath[MAX_STRING_LENGTH];
	char							SelectionChangedSoundPath[MAX_STRING_LENGTH];

	CBitmap2D					*MenuBackground;
	CBitmap2D					*UnselectedButtonBackground;
	CBitmap2D					*SelectedButtonBackground;
};
struct SliderOption
{
	int								DataType;
	int								Position[2];
	CBitmap2D					*Image;
	void							*Data;
};
struct SliderMenu
{
	bool							LoopOptions;
	bool							ShowArrows;
	
	int								ID;
	int								LeftArrowPos[2];
	int								RightArrowPos[2];

	SliderOption				*CurrentOption;
	vector<SliderOption>	Options;
	vector<Animation>		AnimationIndex;
	CBitmap2D					*Highlight;
	CBitmap2D					*LeftArrow;
	CBitmap2D					*RightArrow;
	
	void SelectPrevious()
	{
		for(unsigned int i = 0; i < Options.size(); i++)
		{
			if(CurrentOption == &Options[i])
			{
				if(i == 0)
				{
					if(LoopOptions)
					{
						CurrentOption = &Options[Options.size() - 1];
					}
					else
					{
						break;
					}
				}
			}
		}
	}
	void SelectNext()
	{
		for(unsigned int i = 0; i < Options.size(); i++)
		{
			if(CurrentOption == &Options[i])
			{
				if(i == (Options.size() - 1))
				{
					if(LoopOptions)
					{
						CurrentOption = &Options[0];
					}
					else
					{
						break;
					}
				}
			}
		}
	}
	SliderOption *GetCurrentSelection()
	{
		return CurrentOption;
	}
};
struct GameSelectionMenu
{
	int								ID;
	int								Type;
	int								ObjectSpacing;
	bool							IsHorizontal;
	
	SliderMenu					*CurrentSlider;
	MenuButton				*CurrentButton;
	
	vector<MenuButton>	ButtonMasterIndex;
	vector<int>				SelectionOrder;

	CBitmap2D					*Background;
	CBitmap2D					*UnselectedButtonBackground;
	CBitmap2D					*SelectedButtonBackground;
};
class MenuParser : public ISAXContentHandler
{
private:
	// tags used multiple places
	bool NumberOfButtons;
	bool ID;

	// Opening tag for menu index
	bool MenuStart;

	// Opening tag for an animation
	bool AnimationStart;
		bool Background;			// background animation image
		bool FrameCount;		// # of frames in the animation
		bool FrameTimer;		// time between frames
		bool FrameImage;		//	each frame's image

	// Opening of the main menu
	bool MainMenu;
		bool MenuBGM;						// BGM for menu
		bool SelectionChangeSound;		// sound effect to play on selection change
		bool NumberOfSubmenus;			// how many submenus are contained in this menu?
		bool UnselectedBackground;		// What to use as the default background
		bool SelectedBackground;			// What to use as a selected background on the main menu
		bool MainMenuBKG;
	
	// the start of a button
	bool ButtonStart;
		// does this button open a submenu?
		bool HasASubmenu;
		
		// Submenu's ID
		bool SubmenuID;

		// what does this button do when it's pressed?
		bool ActionCode;

		// center position
		bool CenterPointX;
		bool CenterPointY;
	
		// image to be displayed when the button is selected
		bool SelectedImage;

		// the unselected or "normal" image
		bool UnselectedImage;

		// does this button have an animation linked to it
		bool PlayAnimationWhenSelected;
	
		// if yes, it's animation, if not, it's -1
		bool AnimationID;

	// start of a submenu
	bool SubmenuStart;
		// what button opens this submenu?
		bool OpenedByButton;
	
		// how fast does this menu open?
		bool OpenSpeed;

		// which buttons are present in this submenu?
		bool ButtonIDs;
public:
//	Menu TempMenu;
private:	
	Submenu TempSubmenu;
	MenuButton TempButton;
	Animation TempAnimation;
	public:
//	vector<Menu> LoadedMenus;
	MenuParser(void);
	~MenuParser(void);

	// xml event callbacks
	HRESULT STDMETHODCALLTYPE startDocument( void ) { return S_OK; }
	HRESULT STDMETHODCALLTYPE endDocument( void ) { return S_OK; }
	HRESULT STDMETHODCALLTYPE putDocumentLocator(ISAXLocator *pLocator) { return S_OK;}
	HRESULT STDMETHODCALLTYPE startPrefixMapping(const wchar_t *pwchPrefix, int cchPrefix, const wchar_t *pwchUri, int cchUri) { return S_OK;}
	HRESULT STDMETHODCALLTYPE endPrefixMapping(const wchar_t *pwchPrefix, int cchPrefix) { return S_OK;}
	HRESULT STDMETHODCALLTYPE endElement(const wchar_t *pwchNamespaceUri, int cchNamespaceUri, const wchar_t * pwchLocalName, int cchLocalName, const wchar_t * pwchQName, int cchQName) { return S_OK; }
	HRESULT STDMETHODCALLTYPE characters(const wchar_t *pwchChars, int cchChars);
	HRESULT STDMETHODCALLTYPE ignorableWhitespace(const wchar_t *pwchChars, int cchChars) { return S_OK;}
	HRESULT STDMETHODCALLTYPE processingInstruction(const wchar_t *pwchTarget, int cchTarget, const wchar_t *pwchData, int cchData) {return S_OK;}
	HRESULT STDMETHODCALLTYPE skippedEntity(const wchar_t *pwchName, int cchName) {return S_OK; };

	// COM essentials
	unsigned long __stdcall AddRef(void)  { return 0; }
	unsigned long __stdcall Release(void) { return 0; }
	long __stdcall QueryInterface(const struct _GUID &,void ** )
	{ return 0; }

	HRESULT STDMETHODCALLTYPE startElement( 
		/* [in] */ const wchar_t *pwchNamespaceUri,
		/* [in] */ int cchNamespaceUri,
		/* [in] */ const wchar_t *pwchLocalName,
		/* [in] */ int cchLocalName,
		/* [in] */ const wchar_t *pwchQName,
		/* [in] */ int cchQName,
		/* [in] */ ISAXAttributes *pAttributes)
	{
		char buffer[MAX_STRING_LENGTH];
		wcstombs(buffer, pwchLocalName, (MAX_STRING_LENGTH - 1));
		if(cchLocalName > 0) buffer[cchLocalName] = '\0';

		if(!strcmp(buffer, "Menu"))
		{
			MenuStart = true;
		}
		else if(!strcmp(buffer, "SelectionAnimation"))
		{
			AnimationStart = true;
		}
		else if(!strcmp(buffer, "BackgroundImage"))
		{
			Background = true;
		}
		else if(!strcmp(buffer, "NumberOfFrames"))
		{
			FrameCount = true;
		}
		else if(!strcmp(buffer, "FrameTimer"))
		{
			FrameTimer = true;
		}
		else if(!strcmp(buffer, "Image"))
		{
			FrameImage = true;
		}
		else if(!strcmp(buffer, "MainMenu"))
		{
			MainMenu = true;
		}
		else if(!strcmp(buffer, "NumberOfButtons"))
		{
			NumberOfButtons = true;
		}
		else if(!strcmp(buffer, "NumberOfSubmenus"))
		{
			NumberOfSubmenus = true;
		}
		else if(!strcmp(buffer, "SelectionChangeSound"))
		{
			SelectionChangeSound = true;
		}
		else if(!strcmp(buffer, "MenuBGM"))
		{
			MenuBGM = true;
		}
		else if(!strcmp(buffer, "MenuBKG"))
		{
			MainMenuBKG = true;
		}
		else if(!strcmp(buffer, "UnselectedImageBackground"))
		{
			UnselectedBackground = true;
		}
		else if(!strcmp(buffer, "SelectedImageBackground"))
		{
			SelectedBackground = true;
		}
		else if(!strcmp(buffer, "MenuButton"))
		{
			ButtonStart = true;
		}
		else if(!strcmp(buffer, "ID"))
		{
			ID = true;
		}
		else if(!strcmp(buffer, "HasASubmenu"))
		{
			HasASubmenu = true;
		}
		else if(!strcmp(buffer, "SubmenuID"))
		{
			SubmenuID = true;
		}
		else if(!strcmp(buffer, "ActionCode"))
		{
			ActionCode = true;
		}
		else if(!strcmp(buffer, "CenterPointX"))
		{
			CenterPointX = true;
		}
		else if(!strcmp(buffer, "CenterPointY"))
		{
			CenterPointY = true;
		}
		else if(!strcmp(buffer, "SelectedImage"))
		{
			SelectedImage = true;
		}
		else if(!strcmp(buffer, "UnselectedImage"))
		{
			UnselectedImage = true;
		}
		else if(!strcmp(buffer, "PlayAnimationWhenSelected"))
		{
			PlayAnimationWhenSelected = true;
		}
		else if(!strcmp(buffer, "AnimationID"))
		{
			AnimationID= true;
		}
		else if(!strcmp(buffer, "Submenu"))
		{
			SubmenuStart = true;
		}
		else if(!strcmp(buffer, "ID"))
		{
			ID = true;
		}
		else if(!strcmp(buffer, "OpenedByButtonID"))
		{
			OpenedByButton = true;
		}
		else if(!strcmp(buffer, "OpenSpeed"))
		{
			OpenSpeed = true;
		}
		else if(!strcmp(buffer, "NumberOfButtons"))
		{
			NumberOfButtons = true;
		}
		else if(!strcmp(buffer, "MenuButtonIDs"))
		{
			ButtonIDs = true;
		}
		else if(!strcmp(buffer, "ID"))
		{
			ID = true;
		}
		return S_OK;
	}
};