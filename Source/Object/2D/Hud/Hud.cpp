#include "hud.h"
#include "../../../Render/RenderInterface.h"

CPlayerHud::CPlayerHud()
{
	
	//Defualt path file names.
	strcpy(m_Frame.m_ModelName,				"Textures/hud/hudframe.tga");
	strcpy(m_PlayerIcon.m_ModelName,		"Textures/hud/Robot_Icons/SphinxIcon.bmp");
	strcpy(m_BallCount.m_ModelName,				"Textures/font.tga");
	strcpy(m_WinIcon.m_ModelName,				"Textures/hud/round_win.bmp");

	//Set the number of rounds.
	m_Points4Win = 2;
	m_CurrentPoints = 0;


}

CPlayerHud::~CPlayerHud()
{

}

void CPlayerHud::Initalize(int screenwidth, int screenheight,CMeter* heatmeter, CMeter* lifemeter)
{
	//Assign the meters;
	m_HeatMeter		= heatmeter;
	m_TargetLife	= lifemeter;

	//Assign screen size
	m_ScreenHeight	= screenheight;
	m_ScreenWidth	= screenwidth;

	//Allocate and set the texture.
	m_EmptyPoints = new CLabel[m_Points4Win];
	for(int i = 0; i < m_Points4Win; i++)
	{
		strcpy(m_EmptyPoints[i].m_ModelName,"Textures/hud/round.bmp");
	}


	AddHudToRender();

	m_WinIcon.m_Visible = false;

	//Scale the images
	m_PlayerIcon.m_Scale		= 0.125f;
	m_HeatMeter->m_Scale.y		= 0.25f;
	m_TargetLife->m_Scale.x		= 1.2867f;
	m_BallCount.m_Scale			= 0.5f;


	//Star with the center position of frame.
	m_PlayerIcon.m_MyBody.centerPosition	= m_Frame.m_MyBody.centerPosition;
	m_PlayerIcon.m_MyBody.centerPosition.x	-= 72.0f; 
	m_PlayerIcon.m_MyBody.centerPosition.y	-= 13.0f; 

	//Place the heat meter
	m_TargetLife->m_MyBody.centerPosition	= m_Frame.m_MyBody.centerPosition;
	m_TargetLife->m_MyBody.centerPosition.y	-= 25.0f;
	m_TargetLife->m_MyBody.centerPosition.x	+= 27.0f;

	//Place the target life meter
	m_HeatMeter->m_MyBody.centerPosition	= m_Frame.m_MyBody.centerPosition;
	m_HeatMeter->m_MyBody.centerPosition.y	-= 10.0f;
	m_HeatMeter->m_MyBody.centerPosition.x	+= 22.0f;

	//Place the score
	m_BallCount.m_MyBody.centerPosition			= m_Frame.m_MyBody.centerPosition;
	m_BallCount.m_MyBody.centerPosition.x		-= 115.0f;
	m_BallCount.m_MyBody.centerPosition.y		+= 15.0f;
	m_BallCount.m_FontWidth						= 32;
	strcpy(m_BallCount.m_Text, "XX|XX");

	//Place the ball icon.
	m_WinIcon.m_MyBody.centerPosition			= m_Frame.m_MyBody.centerPosition;

	//set the position;
	for(int i = 0; i < m_Points4Win; i++)
	{
		
		//Scale properly.
		m_EmptyPoints[i].m_Scale = 0.5f;

		//Position properly.
		m_EmptyPoints[i].m_MyBody.centerPosition			= m_Frame.m_MyBody.centerPosition;
		m_EmptyPoints[i].m_MyBody.centerPosition.x			-= 30.0f;
		m_EmptyPoints[i].m_MyBody.centerPosition.y			+= 10.0f;

		m_EmptyPoints[i].m_MyBody.centerPosition.x			+= (float)(i * 25);

	}

	// X += 20
	// Y -= 35

	// X += 20
	// Y -= 5

	//m_Frame.m_Scale = -m_Frame.m_Scale;

}

void CPlayerHud::Flip()
{
	m_Frame.m_Scale.x				= -m_Frame.m_Scale.x;
	//m_PlayerIcon.m_Scale.x			= - m_PlayerIcon.m_Scale.x;
	m_WinIcon.m_Scale.x				= - m_WinIcon.m_Scale.x;
	m_Score.m_Scale.x				= - m_Score.m_Scale.x;
	//m_BallCount.m_Scale.x			= - m_BallCount.m_Scale.x;
	m_HeatMeter->m_Scale.x			= - m_HeatMeter->m_Scale.x;
	m_TargetLife->m_Scale.x			= - m_TargetLife->m_Scale.x;

	m_HeatMeter->m_MyBody.centerPosition.x -= 45.0f;
	m_TargetLife->m_MyBody.centerPosition.x -= 55.0f;
	m_PlayerIcon.m_MyBody.centerPosition.x  += 145.0f;

	m_BallCount.m_MyBody.centerPosition			= m_Frame.m_MyBody.centerPosition;
	m_BallCount.m_MyBody.centerPosition.x		+= 40.0f;
	m_BallCount.m_MyBody.centerPosition.y		+= 15.0f;

	for(int i = 0; i < m_Points4Win; i++)
	{
		m_EmptyPoints[i].m_MyBody.centerPosition			= m_Frame.m_MyBody.centerPosition;
		m_EmptyPoints[i].m_MyBody.centerPosition.x			+= 30.0f;
		m_EmptyPoints[i].m_MyBody.centerPosition.y			+= 10.0f;
		m_EmptyPoints[i].m_MyBody.centerPosition.x			-= (float)(i * 25);
	}

}

void CPlayerHud::Hide(bool makevisible)
{
	m_Frame.m_Visible			= makevisible;
	m_PlayerIcon.m_Visible		= makevisible;
	//m_WinIcon.m_Visible		    = makevisible;
	//m_Score.m_Visible			= makevisible;
	m_BallCount.m_Visible		= makevisible;
	

	//set the position;
	for(int i = 0; i < m_Points4Win; i++)
	{
		//Hide or not to hide.
		m_EmptyPoints[i].m_Visible = makevisible;
	}


	//if(m_HeatMeter)
		m_HeatMeter->m_Visible  = makevisible;

	//if(m_TargetLife->m_Visible)
		m_TargetLife->m_Visible = makevisible;
}

void CPlayerHud::Update(int numballs, int maxballs )
{

}

void CPlayerHud::AddHudToRender()
{

	//Add him last, or first.
	CRenderInterface::GetInstance()->AddLabel(&m_Frame);
	CRenderInterface::GetInstance()->AddLabel(&m_PlayerIcon);
	CRenderInterface::GetInstance()->AddLabel(&m_BallCount);
	CRenderInterface::GetInstance()->AddLabel(&m_WinIcon);
	CRenderInterface::GetInstance()->AddLabel(m_HeatMeter);
	CRenderInterface::GetInstance()->AddLabel(m_TargetLife);

	//set the position;
	for(int i = 0; i < m_Points4Win; i++)
	{
		CRenderInterface::GetInstance()->AddLabel(&m_EmptyPoints[i]);
	}
	

}