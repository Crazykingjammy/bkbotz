#ifndef _SELECTOPTION_H_
#define _SELECTOPTION_H_

#include "../Object.h"
#include "../Player/Player.h"
#include "../../Physics/physicstimer.h"

#define MAX_CHAR 128

class CSelectOption : public CBaseObject
{
private:
	int  m_selectionID;

public:
	CSelectOption(void);
	~CSelectOption(void);
	int  m_NumSelected;			// how many are currently selecting this selection
	int	 m_IconLabelIndex;		// the index to its Icon
	bool m_isSelectable;		// bool if this is selectable, will the iterator even go to this one
	bool m_isSelected;			// bool if this was selected
	
	int GetSelectionID(){
		return m_selectionID;
	}
	void SetSelectionID(int id)
	{
		m_selectionID = id;
	}

};

class CSelectIteratorIcon : public CBaseObject
{
private:

public:
	CSelectIteratorIcon(void);
	CSelectIteratorIcon(const CSelectIteratorIcon& selIterIcon);
	CSelectIteratorIcon & operator = (const CSelectIteratorIcon &selIterIcon);
	~CSelectIteratorIcon(void);
	
};

class CSelectIterator : public CPlayer
{
private:


public:
	CPhysicsTimer m_LightStablizeTimer;
	float m_LightStablizeFreq;

	char m_CurrentSelectedOption[MAX_CHAR];

	int  m_SelectedID;					// the ID of the current selected Selection
	int	 m_PreviousSelectID;			// the ID of the last selected ID
	int  m_iSelectionIDIconIndex;		// yeah

	bool m_isReady;						// has the player made a selection
	bool m_isIT;						// is the iterator "IT"

	char *GetCurrentSelectionOption(){
		return m_CurrentSelectedOption;
	}
	void		   SetCurrentSelectionOption(CSelectOption *currOption);
	CSelectIterator(void);
	CSelectIterator(const CSelectIterator&sI);
	CSelectIterator &operator =(const CSelectIterator &sI);
	~CSelectIterator(void);
};

class CLevel : public CBaseObject
{
private:
	//CLabel *m_LevelLabel;

public:
	//int		  m_TextureIndex;
	CLevel(void){}
	~CLevel(void){}

};

class CModeOption : public CBaseObject
{
private:
	vector <CLevel*> m_ListOfLevels;
	int				 m_iIndex;
	int				m_iCurrentSelectedLevel;
public:
	// k we want to be able to add levels to this mode
	CLevel *AddLevelToMode(char *levelName);
	void    ClearLevelList();
	// go through the list based on controls 
	// returns the pointer of the current selected level to be able to 
	// display its texture
	bool	IncrementLevelList();
	bool	DecrementLevelList();
	int		GetCurrentLevel();
	int		GetCurrentLevelLabelIndex();
	bool	IsIndexInbounds(int index);
	bool    SetModeName(const char * modeName);

	int		GetNumberOfLevels() 
	{
		return (int)m_ListOfLevels.size();
	}

	CModeOption(void);
	~CModeOption(void);
};


#endif