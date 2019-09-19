#include "../GameState.h"

CIntroGameState* CIntroGameState::pInstance = 0;

void CIntroGameState::Initalize(Game* game)
{
	//Get the Pointer to the game.
	pGame = game;
	
	//////////////////////////////////////////////////////////////////////////
	// Lights Setup
	//////////////////////////////////////////////////////////////////////////


	//Set up Lights here
	pRenderInterface->Light[0].m_Active = true;
	pRenderInterface->Light[0].m_MyBody.centerPosition.y = 10.0f;
	pRenderInterface->Light[0].m_Specular = 0.0f;
	pRenderInterface->Light[0].m_Diffuse = EXColor(0.6f,0.65f,0.65f,1.0f);
	pRenderInterface->Light[0].m_Ambient = EXColor(0.1f,0.1f,0.1f,1.0f);
	pRenderInterface->Light[0].m_Attenuation2 = 0.0000002f;


	//////////////////////////////////////////////////////////////////////////
	// Lights Setup
	//////////////////////////////////////////////////////////////////////////


	//////////////////////////////////////////////////////////////////////////
	//	Camera Setup
	//////////////////////////////////////////////////////////////////////////

	Camera0 = new CFreeCamera;
	Camera1 = new CFreeCamera;

	Camera0->m_LinearSpeed = 5.0f;
	Camera0->m_MyBody.centerPosition.y += 24.035156f;
	Camera0->m_MyBody.centerPosition.x -= 35.638084f;
	Camera0->m_MyBody.centerPosition.z -= 26.376434f;

	Camera1->m_MyBody.centerPosition.z = -10.0f;


	pRenderInterface->SetActiveCamera(Camera0);
	//pRenderInterface->

	//////////////////////////////////////////////////////////////////////////
	//	Camera Setup
	//////////////////////////////////////////////////////////////////////////
	m_2DScene.Set2DWorld((float)pGame->Window.WindowWidth,(float)pGame->Window.WindowHeight);
	m_2DScene.sceneGravity.MakeZero();

	//////////////////////////////////////////////////////////////////////////
	// Object Setup
	//////////////////////////////////////////////////////////////////////////

	pRenderInterface->SetWorld("Models/MenuWorld.X");

	Ship = new CBall;
	strcpy(Ship->m_ModelName,"Models/MenuShip.X");
	Ship->m_MyBody.centerPosition.y += 7.0f;
	//pRenderInterface->AddObject(Ship);
	//Ship->m_MyBodyIndex = m_Scene.AddBody(&Ship->m_MyBody);


	Battle = new CLabel("Textures/MenuIcon_1.bmp");
	pRenderInterface->AddLabel(Battle);
	Battle->m_MyBody.centerPosition.x += (float)(pGame->Window.WindowWidth / 3);
	Battle->m_MyBody.centerPosition.y += (float) (pGame->Window.WindowHeight / 3);
	Battle->m_MyBody.radius = 10.0f;
	Battle->m_MyBody.mass = 1.0f;
	Battle->m_MyBodyIndex = m_2DScene.AddBody(&Battle->m_MyBody);


	Credits = new CLabel("Textures/MenuIcon_3.bmp");
	pRenderInterface->AddLabel(Credits);
	Credits->m_MyBody.centerPosition.x += 300.0f;
	Credits->m_MyBody.centerPosition.y += 450.0f;
	Credits->m_MyBody.radius = 10.0f;
	Credits->m_MyBody.mass = 1.0f;
	Credits->m_MyBodyIndex = m_2DScene.AddBody(&Credits->m_MyBody);


	Exit = new CLabel("Textures/MenuIcon_4.bmp");
	pRenderInterface->AddLabel(Exit);
	Exit->m_MyBody.centerPosition.x += 600.0f;
	Exit->m_MyBody.centerPosition.y += 450.0f;
	Exit->m_MyBody.radius = 10.0f;;
	Exit->m_MyBody.mass = 1.0f;
	Exit->m_MyBodyIndex = m_2DScene.AddBody(&Exit->m_MyBody);

	Soccer = new CLabel("Textures/MenuIcon_2.bmp");
	pRenderInterface->AddLabel(Soccer);
	Soccer->m_MyBody.centerPosition.x += 600.0f;
	Soccer->m_MyBody.centerPosition.y += 150.0f;
	Soccer->m_MyBody.radius = 10.0f;;
	Soccer->m_MyBodyIndex = m_2DScene.AddBody(&Soccer->m_MyBody);
	Soccer->m_MyBody.mass = 1.0f;



	Me = new CLabel("Textures/MenuIcon_0.bmp");

	pRenderInterface->AddLabel(Me);
	Me->m_MyBody.isNapping = false;
	Me->m_MyBody.centerPosition.x += (float)(pGame->Window.WindowWidth/2);
	Me->m_MyBody.centerPosition.y += (float)(pGame->Window.WindowHeight/2);
	Me->m_MyBody.mass = 10.0f;
	Me->m_MyBody.radius = 64.0f;;
	Me->m_MyBodyIndex = m_2DScene.AddBody(&Me->m_MyBody);

	//strcpy(Me->m_Text, "B");

	GameText = new CLabel("Textures/jamtext.bmp");
	pRenderInterface->AddLabel(GameText);
	GameText->m_MyBody.centerPosition.x = 280.0f;
	GameText->m_MyBody.centerPosition.y = 30.0f;

	//////////////////////////////////////////////////////////////////////////
	// Object Setup
	//////////////////////////////////////////////////////////////////////////




	//Numbers
	MeForce = 27000.0f * 4.0f;
	m_InputFreq = 0.05f;

	CurrentSelection = 0;

	//Create the Scene
	pRenderInterface->CreateScene();

}

void CIntroGameState::Update(float timeDelta)
{
	//m_Scene.Simulate();
	m_2DScene.Simulate();


	
	Camera0->LookAt(pRenderInterface->m_World);

	cout << Me->m_MyBody.centerPosition.x << ' ' << Me->m_MyBody.centerPosition.y << ' ' << Me->m_MyBody.centerPosition.z<< endl;
 

	
	//Pull toward the middle.
	Battle->m_MyBody.gravityForce = CVector3f(400.0f, 300.0f, 0.0f) - Battle->m_MyBody.centerPosition; 
	Battle->m_MyBody.gravityForce.Normalize();
	Battle->m_MyBody.gravityForce *= 5.8f;
	Credits->m_MyBody.gravityForce = CVector3f(400.0f, 300.0f, 0.0f) - Credits->m_MyBody.centerPosition;
	Credits->m_MyBody.gravityForce.Normalize();
	Credits->m_MyBody.gravityForce *= 5.8f;
	Soccer->m_MyBody.gravityForce = CVector3f(400.0f, 300.0f, 0.0f) - Soccer->m_MyBody.centerPosition;
	Soccer->m_MyBody.gravityForce.Normalize();
	Soccer->m_MyBody.gravityForce *= 5.8f;
	Exit->m_MyBody.gravityForce = CVector3f(400.0f, 300.0f, 0.0f) - Exit->m_MyBody.centerPosition;
	Exit->m_MyBody.gravityForce.Normalize();
	Exit->m_MyBody.gravityForce *= 5.8f;

	 

	//Check for controls.
	if (m_InputTimer.GetElapsedSeconds() >= m_InputFreq)
	{
		Controls();

		Me->m_Rotation += 0.01f; // Me->m_MyBody.velocityVec.Magnitude();

		Me->m_MyBody.Add2Force(  (CVector3f((float)(pGame->Window.WindowWidth/2), (float)(pGame->Window.WindowHeight/2), 0.0f) - Me->m_MyBody.centerPosition) * 750.0f );



		m_InputTimer.Reset();
	}

	CollisionDetection();
	//attach cam to the player.
	Camera1->m_MyBody.centerPosition = Ship->m_MyBody.centerPosition;

	//Move camera
	Camera1->m_MyBody.centerPosition.y += 4.51f;
	Camera1->m_MyBody.centerPosition.x -= 0.51f;
	Camera1->m_MyBody.centerPosition.z -= 3.3f;
	



	switch(CurrentSelection)
	{
		char test[128];

	case BATTLE:
		{

			sprintf(test, "Battle Game");
			strcpy(GameText->m_Text,test);

			if (pInput->Controller1JoyPress(0))
			{
				ChangeState(pGame,CBattleGameState::GetInstance());
				return;
			}


			break;
		}

	case SOCCER:
		{

			sprintf(test, "Soccer Game");
			strcpy(GameText->m_Text,test);

			if (pInput->Controller1JoyPress(0))
			{
				ChangeState(pGame, CSoccerGameState::GetInstance());
				return;
			}

			break;
		}

	case EXIT:
		{

			sprintf(test, "Exit Game");
			strcpy(GameText->m_Text,test);

			if (pInput->Controller1JoyPress(0))
			{
				//Only place this should be getting called :_D
				PostQuitMessage(0);
			}

			break;
		}

	case CREDITS:
		{

			sprintf(test, "Game Credits");
			strcpy(GameText->m_Text,test);

			if (pInput->Controller1JoyPress(0))
			{
				ChangeState(pGame, CJukeBoxState::GetInstance());
				return;
			}


			break;
		}
	}



}

void CIntroGameState::Shutdown()
{
	//Destroy teh modles and reset the scene.
	pRenderInterface->DestroyScene();


	m_Scene.ClearScene();

	m_2DScene.ClearScene();

	delete Camera0;
	delete Camera1;

	delete Ship;


	delete Exit;
	delete Soccer;
	delete Battle;
	delete Credits;


	//Clean Me Up.
	if(pInstance)
		delete pInstance;
	pInstance = 0;
}



void CIntroGameState::Controls()
{


	//Get input States.
	pInput->SetAllStates();


	if (pInput->Controller1JoyPress(1))
	{
		Me->m_MyBody.velocityVec.MakeZero();
	}


	// timer >= inpout time
	// do this
	if(GetAsyncKeyState('W'))
		pRenderInterface->m_ActiveCamera->MoveUp();
	if(GetAsyncKeyState('S'))
		pRenderInterface->m_ActiveCamera->MoveDown();
	if(GetAsyncKeyState('A'))
		pRenderInterface->m_ActiveCamera->MoveLeft();
	if(GetAsyncKeyState('D'))
		pRenderInterface->m_ActiveCamera->MoveRight();
	if(GetAsyncKeyState('C'))
		pRenderInterface->m_ActiveCamera->MoveFoward();
	if(GetAsyncKeyState('V'))
		pRenderInterface->m_ActiveCamera->MoveBackward();


	if(GetAsyncKeyState('F'))
		pRenderInterface->m_ActiveCamera->LookLeft();
	if(GetAsyncKeyState('H'))
		pRenderInterface->m_ActiveCamera->LookRight();
	if(GetAsyncKeyState('T'))
		pRenderInterface->m_ActiveCamera->LookUp();
	if(GetAsyncKeyState('G'))
		pRenderInterface->m_ActiveCamera->LookDown();

	if(pInput->Controller1JoyY() < -200)
	{
		Me->m_MyBody.Add2Force( CVector3f(0.0f,-1.0f,0.0f) * (MeForce ) );
	}
	if(pInput->Controller1JoyY() > 200)
	{	
		Me->m_MyBody.Add2Force( CVector3f(0.0f,1.0f,0.0f) * MeForce );
	}
	if(pInput->Controller1JoyX() < -200)
	{
		Me->m_MyBody.Add2Force( CVector3f(-1.0f,0.0f,0.0f) * MeForce );
	}
	if(pInput->Controller1JoyX() > 200)
	{
		Me->m_MyBody.Add2Force( CVector3f(1.0f,0.0f,0.0f) * MeForce );
	} 


	if(GetAsyncKeyState('P'))
		pRenderInterface->SetActiveCamera(Camera1);
	if(GetAsyncKeyState('L'))
		pRenderInterface->SetActiveCamera(Camera0);

}

void CIntroGameState::CollisionDetection()
{
	if(m_2DScene.HaveTheTwoBodiesCollided(Battle->m_MyBodyIndex,Me->m_MyBodyIndex))
	{
		CurrentSelection = BATTLE;
		Camera0->m_MyBody.centerPosition.y = 24.035156f;
		Camera0->m_MyBody.centerPosition.x = -35.638084f;
		Camera0->m_MyBody.centerPosition.z = -26.376434f;
	}

	if(m_2DScene.HaveTheTwoBodiesCollided(Soccer->m_MyBodyIndex,Me->m_MyBodyIndex))
	{
		CurrentSelection = SOCCER;
	}

	if(m_2DScene.HaveTheTwoBodiesCollided(Exit->m_MyBodyIndex,Me->m_MyBodyIndex))
	{
		CurrentSelection = EXIT;

		Camera0->m_MyBody.centerPosition.y = 23.254128f;
		Camera0->m_MyBody.centerPosition.x = -58.199429f;
		Camera0->m_MyBody.centerPosition.z = 19.165016f;
	}
	if(m_2DScene.HaveTheTwoBodiesCollided(Credits->m_MyBodyIndex,Me->m_MyBodyIndex))
	{
		CurrentSelection = CREDITS;
	}

}

 CIntroGameState* CIntroGameState::GetInstance()
{
	//If there is no instance
	if(pInstance == 0)
		pInstance = new CIntroGameState;		//We Create One

	return pInstance;							//Return the Instance.
}