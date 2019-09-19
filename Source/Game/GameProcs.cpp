#include "game.h"

CPhysicsTimer gameTimer;


void Game::InitWindow()
{
	//////////////////////////////////////////////////////////////////////////
	//	Create Window
	//////////////////////////////////////////////////////////////////////////


	//Init some Window Shit.
	Window.WindowWidth			= 800;
	Window.WindowHeight			= 600;
	Window.WindowX				= 0;
	Window.WindowY				= 0;
	Window.WindowStyle			= WS_OVERLAPPEDWINDOW;
	Window.WindowFlags			= SW_NORMAL;
	Window.WindowName			= "BlitzkriegBotz : Saturation Bombing";
	Window.WindowClassName		= "preperationh";



	//Get an instance.
	Window.hInstance = GetModuleHandle(0);

	//Set up the window Class.
	memset(&Window.WndClass,0,sizeof(Window.WndClass));
	Window.WndClass.lpszClassName			= Window.WindowClassName;
	Window.WndClass.hInstance				= Window.hInstance;
	Window.WndClass.style					= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	Window.WndClass.lpfnWndProc				= WindowProcedure;
	RegisterClass(&Window.WndClass);

	//Create Our Window Here.
	Window.hWnd = CreateWindow(Window.WindowClassName, Window.WindowName,Window.WindowStyle,
		Window.WindowX, Window.WindowY, Window.WindowWidth, Window.WindowHeight, 0, 0, Window.hInstance, 0);

	//Show the window... yuh
	ShowWindow(Window.hWnd,Window.WindowFlags);
	Window.hDC = GetDC(Window.hWnd);

	ShowCursor(false);


	//////////////////////////////////////////////////////////////////////////
	//	Create Window
	//////////////////////////////////////////////////////////////////////////
}



LRESULT Game::WindowProcedure(HWND hWnd, UINT uMessage, WPARAM wParam, LPARAM lParam)
{

	switch(uMessage)
	{
	case WM_CREATE:
		{

		}
		break;

	case WM_SIZE:
		{

		}
		break;


	case WM_DESTROY:
		{
			PostQuitMessage(0);
		}
		break;

	default:
		{
			return DefWindowProc(hWnd, uMessage, wParam, lParam);
		}
	}

	return 0;

}

//Execute
void Game::Execute()
{
	Window.Msg.message = ~WM_QUIT;

	static float lastTime = (float)timeGetTime(); 

	float lastFrameTime = 0.0f;
	float currFrameTime = 0.0f;

	while(isGameRunning || Window.Msg.message != WM_QUIT)
	{
		if(PeekMessage(&Window.Msg, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&Window.Msg);
			DispatchMessage(&Window.Msg);
		}
		else
		{
			lastFrameTime  = currFrameTime;
			currFrameTime  = gameTimer.GetElapsedSeconds();
			timeDelta = currFrameTime-lastFrameTime;		
			if (timeDelta < 1/60.0f)
			{
				//Game run, render and probable physics update is all thats going to get called here.


				//Check For Game State.
				GameRun();

				//Draw
				gameTimer.Reset();
			}
			pRenderInterface->Update(timeDelta);
			pSound->Update();
		}
	}
}
