#include "InputMgr.h"

#define CONTROLLER_ONE 1
#define CONTROLLER_TWO 2
#define CONTROLLER_THREE 3
#define CONTROLLER_FOUR 4


InputManager *InputManager::pInstance = 0;
void InputManager::DeleteInstance(void)
{
	if(pInstance)
	{
		delete pInstance;
		pInstance = 0;
	}
}

InputManager *InputManager::GetInstance(void)
{
	if(pInstance == 0)
	{
		pInstance = new InputManager;
	}

	return pInstance;
}

unsigned int InputManager::Initialize(HINSTANCE hInstance, HWND hWnd)
{

DeviceCount = Joystick::deviceCount();

Kboard = new Keyboard();

Kboard->ObjectInit(hInstance, hWnd);
Kboard->DXInputKeyboardInit(DISCL_BACKGROUND, DISCL_NONEXCLUSIVE);

Mice = new Mouse();

Mice->ObjectInit(hInstance, hWnd);
Mice->DXInputMouseInit(DISCL_BACKGROUND, DISCL_NONEXCLUSIVE);

		for(unsigned int i = 0; i < DeviceCount; i++)
		{
			
			Jstick = new Joystick(i);

			Jstick->Init(hInstance, hWnd);

			//Add to the list.
			Joy.push_back(Jstick);
		}	

		//Return the Index
return (unsigned int)(Joy.size() - 1);

}

void InputManager::ControllerDeviceName(int Controller, char * devicename)
{
	switch( Controller )
	{
		// The Start function of the IDirectInputEffect interface is used to
		// start an effect
	case CONTROLLER_ONE:
		if(Joy.size() >= 1)
			Joy[0]->deviceName(devicename);
		break ;
	case CONTROLLER_TWO:
		if(Joy.size() >= 2)
			Joy[1]->deviceName(devicename);
		break ;
	case CONTROLLER_THREE:
		if(Joy.size() >= 3)	
			Joy[2]->deviceName(devicename);
		break ;
	case CONTROLLER_FOUR:
		if(Joy.size() >= 4)
			Joy[3]->deviceName(devicename);
		break ;
	}
}

void InputManager::SetAllStates()
{
	for(unsigned int i = 0; i < Joy.size(); i++)
	{
			Joy[i]->JoystickGetState();	
	}
	
	Kboard->DXInputGetKeyboardState();
	Mice->DXInputGetMouseState();

}

bool InputManager::KeyboardKeyPress(DWORD button)
{
	if(Kboard->DXInputKeyboardPress(button))
		return true;

return false;
}

bool InputManager::MouseButtonPress(DWORD button)
{
	if(Mice->DXInputMousePress(button))
		return true;

return false;
}

float InputManager::MouseGetX(void)
{
	return Mice->DXInputGetMouseX();
}
float InputManager::MouseGetY(void)
{
	return Mice->DXInputGetMouseY();
}

void InputManager::Shutdown()
{
	//Go trough all an delete bitch.
	for(unsigned int i = 0; i < Joy.size(); i++)
	{
		//Clean up xfile
		Joy[i]->Shutdown();
		delete Joy[i];
		Joy.clear();
	}

	Mice->Shutdown();
	Kboard->Shutdown();
	this->DeleteInstance();

}

bool InputManager::ControllerJoystickUp(int Player)
{	
	// Player One
	/************************************************************************/
	if(Player == 0)
	{	
		if(Joy.size() >= 1)
		{
			if(Joy[0]->DXInputGetJoyY() < -200 || Kboard->DXInputKeyboardPress(DIK_W))
				return true;
		}
		else 
		{
			if(Kboard->DXInputKeyboardPress(DIK_W))
				return true;
		}
	}
	// Player Two
	/************************************************************************/
	else if(Player == 1)
	{
		if(Joy.size() >= 2)
		{
			if(Joy[1]->DXInputGetJoyY() < -200 || Kboard->DXInputKeyboardPress(DIK_UP))
				return true;
		}
		else 
		{
			if(Kboard->DXInputKeyboardPress(DIK_UP))
				return true;
		}
	}
	// Player Three
	/************************************************************************/
	else if(Player == 2)
	{
		if(Joy.size() == 3)
			if(Joy[2]->DXInputGetJoyY() < -200)
				return true;
	}
	// Player Four
	/************************************************************************/
	else if(Player == 3)
	{
		if(Joy.size() == 4)
			if(Joy[3]->DXInputGetJoyY() < -200)
				return true;
	}
	/************************************************************************/
	else if(Player == -1)
	{
		for (int i = 0; i < (int)Joy.size(); i++)
		{
			if (Joy[i]->DXInputGetJoyY() < -200 || Kboard->DXInputKeyboardPress(DIK_UP))
				return true;
		}
		if (Kboard->DXInputKeyboardPress(DIK_UP))
			return true;
	}
	else
		return false;

	
	return false;
}

bool InputManager::ControllerJoystickDown(int Player)
{
	// Player One
	/************************************************************************/
	if(Player == 0)
	{	
		if(Joy.size() >= 1)
		{
			if(Joy[0]->DXInputGetJoyY() > 200 || Kboard->DXInputKeyboardPress(DIK_S))
				return true;
		}
		else 
		{
			if(Kboard->DXInputKeyboardPress(DIK_S))
				return true;
		}
	}
	// Player Two
	/************************************************************************/
	else if(Player == 1)
	{
		if(Joy.size() >= 2)
		{
			if(Joy[1]->DXInputGetJoyY() > 200 || Kboard->DXInputKeyboardPress(DIK_DOWN))
				return true;
		}
		else 
		{
			if(Kboard->DXInputKeyboardPress(DIK_DOWN))
				return true;
		}
	}
	// Player Three
	/************************************************************************/
	else if(Player == 2)
	{
		if(Joy.size() == 3)
			if(Joy[2]->DXInputGetJoyY() > 200)
				return true;
	}
	// Player Four
	/************************************************************************/
	else if(Player == 3)
	{
		if(Joy.size() == 4)
			if(Joy[3]->DXInputGetJoyY() > 200)
				return true;
	}
	/************************************************************************/
	else if(Player == -1)
	{
		for (int i = 0; i < (int)Joy.size(); i++)
		{
			if (Joy[i]->DXInputGetJoyY() > 200 || Kboard->DXInputKeyboardPress(DIK_DOWN))
				return true;
		}
		if (Kboard->DXInputKeyboardPress(DIK_DOWN))
			return true;

	}
	else
		return false;


	return false;
}

bool InputManager::ControllerJoystickLeft(int Player)
{
	// Player One
	/************************************************************************/
	if(Player == 0)
	{	
		if(Joy.size() >= 1)
		{
			if(Joy[0]->DXInputGetJoyX() < -200 || Kboard->DXInputKeyboardPress(DIK_A))
				return true;
		}
		else 
		{
			if(Kboard->DXInputKeyboardPress(DIK_A))
				return true;
		}
	}
	// Player Two
	/************************************************************************/
	else if(Player == 1)
	{
		if(Joy.size() >= 2)
		{
			if(Joy[1]->DXInputGetJoyX() < -200 || Kboard->DXInputKeyboardPress(DIK_LEFT))
				return true;
		}
		else 
		{
			if(Kboard->DXInputKeyboardPress(DIK_LEFT))
				return true;
		}
	}
	// Player Three
	/************************************************************************/
	else if(Player == 2)
	{
		if(Joy.size() == 3)
			if(Joy[2]->DXInputGetJoyX() < -200)
				return true;
	}
	// Player Four
	/************************************************************************/
	else if(Player == 3)
	{
		if(Joy.size() == 4)
			if(Joy[3]->DXInputGetJoyX() < -200)
				return true;
	}
	/************************************************************************/
	else if(Player == -1)
	{
		for (int i = 0; i < (int)Joy.size(); i++)
		{
			if (Joy[i]->DXInputGetJoyX() < -200 || Kboard->DXInputKeyboardPress(DIK_LEFT))
				return true;
		}
		if (Kboard->DXInputKeyboardPress(DIK_LEFT))
			return true;
	}
	else
		return false;


	return false;
}

bool InputManager::ControllerJoystickRight(int Player)
{
	// Player One
	/************************************************************************/
	
	if(Player == 0)
	{	
		if(Joy.size() >= 1)
		{
			if(Joy[0]->DXInputGetJoyX() > 200 || Kboard->DXInputKeyboardPress(DIK_D))
				return true;
		}
		else 
		{
			if(Kboard->DXInputKeyboardPress(DIK_D))
				return true;
		}
	}
	// Player Two
	/************************************************************************/
	else if(Player == 1)
	{
		if(Joy.size() >= 2)
		{
			if(Joy[1]->DXInputGetJoyX() > 200 || Kboard->DXInputKeyboardPress(DIK_RIGHT))
				return true;
		}
		else 
		{
			if(Kboard->DXInputKeyboardPress(DIK_RIGHT))
				return true;
		}
	}
	// Player Three
	/************************************************************************/
	else if(Player == 2)
	{
		if(Joy.size() == 3)
			if(Joy[2]->DXInputGetJoyX() > 200)
				return true;
	}
	// Player Four
	/************************************************************************/
	else if(Player == 3)
	{
		if(Joy.size() == 4)
			if(Joy[3]->DXInputGetJoyX() > 200)
				return true;
	}
	/************************************************************************/
	else if(Player == -1)
	{
		for (int i = 0; i < (int)Joy.size(); i++)
		{
			if (Joy[i]->DXInputGetJoyX() > 200)
				return true;
		}

		if (Kboard->DXInputKeyboardPress(DIK_RIGHT))
			return true;
	}
	else
		return false;


	return false;
}

//Any Controller
bool InputManager::ControllerJoyPress(int Player, DWORD button)
{

	// Player One
	/************************************************************************/
	if(Player == 0)
	{	
		// KEYBOARD Player One
		if(button == 0)
			if(Kboard->DXInputKeyboardPress(DIK_G))
				return true;

		if(button == 1)
			if(Kboard->DXInputKeyboardPress(DIK_F))
				return true;

		if(button == 2)
			if(Kboard->DXInputKeyboardPress(DIK_SPACE))
				return true;

		if(button == 4)
			if(Kboard->DXInputKeyboardPress(DIK_T))
				return true;

		if(button == 5)
			if(Kboard->DXInputKeyboardPress(DIK_Y))
				return true;


		if(Joy.size() >= 1)
		{
			if(Joy.size() >= 1)
			{
				if(Joy[0]->DXInputJoyPress(button))
					return true;
			}
		}
	}
	// Player Two
	/************************************************************************/
	else if(Player == 1)
	{
		// KEYBOARD Player One
		if(button == 0)
			if(Kboard->DXInputKeyboardPress(DIK_NUMPAD2))
				return true;

		if(button == 1)
			if(Kboard->DXInputKeyboardPress(DIK_NUMPAD1))
				return true;

		if(button == 2)
			if(Kboard->DXInputKeyboardPress(DIK_NUMPAD0))
				return true;

		if(button == 4)
			if(Kboard->DXInputKeyboardPress(DIK_NUMPAD7))
				return true;

		if(button == 5)
			if(Kboard->DXInputKeyboardPress(DIK_NUMPAD4))
				return true;

		if(Joy.size() >= 2)
		{
			if(Joy[1]->DXInputJoyPress(button))
				return true;
		}
		
	}
	// Player Three
	/************************************************************************/
	else if(Player == 2)
	{
		if(Joy.size() == 3)
			if(Joy[2]->DXInputJoyPress(button))
				return true;
	}
	// Player Four
	/************************************************************************/
	else if(Player == 3)
	{
		if(Joy.size() == 4)
			if(Joy[3]->DXInputJoyPress(button))
				return true;
	}
	/************************************************************************/
	else
		return false;

	return false;
}


