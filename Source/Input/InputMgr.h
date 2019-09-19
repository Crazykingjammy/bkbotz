#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H

#include "../Render/Direct3D/D3DLibs.h"
#include "DXInput.h"
#include <iostream>
using namespace std;
#include <vector>


class InputManager
{
public:

InputManager()
{
}
~InputManager()
{
}

vector<Joystick*> Joy;

unsigned int DeviceCount;

Joystick *Jstick;
Keyboard *Kboard;
Mouse * Mice;

static InputManager *pInstance;

InputManager(const InputManager&)
{
}
InputManager &operator = (const InputManager&)
{
}

void DeleteInstance(void);
static InputManager *GetInstance(void);
/******************************************************/
unsigned int Initialize(HINSTANCE hInstace, HWND hWnd);
void Shutdown();


void SetAllStates();

void ControllerDeviceName(int Controller, char * deviceName);
/******************************************************/
bool ControllerJoystickUp(int Player);
bool ControllerJoystickDown(int Player);
bool ControllerJoystickLeft(int Player);
bool ControllerJoystickRight(int Player);
/******************************************************/
bool ControllerJoyPress(int Player, DWORD button);
/******************************************************/
bool KeyboardKeyPress(DWORD button);
/******************************************************/
bool MouseButtonPress(DWORD button);
float MouseGetX(void);
float MouseGetY(void);

};

#endif