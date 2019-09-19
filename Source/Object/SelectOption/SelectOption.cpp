#include "SelectOption.h"

CSelectOption::CSelectOption(void)
{
	m_NumSelected		= 0;
	m_isSelected		= false;
	m_isSelectable		= true;
	m_selectionID		= -1;
	m_IconLabelIndex	= -1;
}

CSelectOption::~CSelectOption(void)
{
}

CSelectIterator::CSelectIterator(void)
{
	memset(m_CurrentSelectedOption, 0, sizeof(m_CurrentSelectedOption));
	m_LightStablizeFreq			= 0.005f;
	m_SelectedID				= -1;
	m_PreviousSelectID		    = -1;
	m_isReady					= false;
	m_Barrell					= new CBarrell;
	m_Barrell->m_MyBody.radius  = 1.0f;
	m_isIT						= false;
	m_iSelectionIDIconIndex		= -1;
}

CSelectIterator::CSelectIterator(const CSelectIterator&sI)
{
	m_SelectedID = sI.m_SelectedID;
	strcpy(m_CurrentSelectedOption, sI.m_CurrentSelectedOption);
}
CSelectIterator &CSelectIterator::operator =(const CSelectIterator &sI)
{
	m_SelectedID = sI.m_SelectedID;
	strcpy(m_CurrentSelectedOption, sI.m_CurrentSelectedOption);
	return *this;
}
CSelectIterator::~CSelectIterator(void)
{
	if (m_Barrell)
	{
		delete m_Barrell;
		m_Barrell = NULL;
	}
}

void CSelectIterator::SetCurrentSelectionOption(CSelectOption *currOption)
{
	strcpy(m_CurrentSelectedOption, currOption->m_MyBody.myName);
	m_SelectedID				= currOption->GetSelectionID();
	currOption->m_isSelected	= true;
	m_Barrell->ModelIndex		= currOption->ModelIndex;
	m_Barrell->m_Visible		= true;
	m_iSelectionIDIconIndex		= currOption->m_IconLabelIndex;

	//m_Barrell->m_MyBody.radius  = currOption->m_MyBody.radius;
}

CSelectIteratorIcon::CSelectIteratorIcon(void)
{

}
CSelectIteratorIcon::CSelectIteratorIcon(const CSelectIteratorIcon &selIterIcon)
{

}
CSelectIteratorIcon & CSelectIteratorIcon::operator=(const CSelectIteratorIcon &selIterIcon)
{
	return *this;
}
CSelectIteratorIcon::~CSelectIteratorIcon(void)
{

}


CModeOption::CModeOption(void)
{
	m_ListOfLevels.reserve(10);
	m_ListOfLevels.clear();
	m_iIndex				= 0;
	m_iCurrentSelectedLevel = 0;
}

CModeOption::~CModeOption()
{
}

CLevel * CModeOption::AddLevelToMode(char *levelName)
{
	CLevel *newLevel = new CLevel;

	if (newLevel)
	{
		strcpy(newLevel->m_MyBody.myName, levelName);
		m_ListOfLevels.push_back(newLevel);
	}
	return m_ListOfLevels[m_iIndex++];
}

void	 CModeOption::ClearLevelList()
{
	for (int i = 0; i < (int)m_ListOfLevels.size(); i++)
	{
		delete m_ListOfLevels[i];
	}
	m_ListOfLevels.clear();
}

bool CModeOption::IncrementLevelList()
{
	if (IsIndexInbounds(m_iCurrentSelectedLevel+1))
	{
		m_iCurrentSelectedLevel++;
	}
	else
		m_iCurrentSelectedLevel = 0;

	return true;
}

bool CModeOption::DecrementLevelList()
{
	if (IsIndexInbounds(m_iCurrentSelectedLevel-1))
	{
		m_iCurrentSelectedLevel--;
	}
	else
		m_iCurrentSelectedLevel = m_iIndex-1;

	return true;
}

int CModeOption::GetCurrentLevel()
{
	return m_iCurrentSelectedLevel;
}

int CModeOption::GetCurrentLevelLabelIndex()
{
	CLevel *curLevel = NULL;
	if (IsIndexInbounds(m_iCurrentSelectedLevel))
	{
		curLevel = m_ListOfLevels[m_iCurrentSelectedLevel];
		return curLevel->ModelIndex;
	}
	return 0;
}
bool CModeOption::IsIndexInbounds(int index)
{
	if (!m_ListOfLevels.size())
		return false;
	if (index < 0 || index >= (int)m_ListOfLevels.size())
		return false;

	return true;
}
bool CModeOption::SetModeName(const char * modeName)
{
	if (!modeName)
		return false;
	strcpy(m_MyBody.myName, modeName);
	return true;
}



