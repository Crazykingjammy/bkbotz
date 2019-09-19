#ifndef __JOYSTICK_H__
#define __JOYSTICK_H__

#include <windows.h>
#include <basetsd.h>
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>

class Joystick
{

public:
	char ControllerName[128];


private:
	
	GUID DeviceGuid;
	unsigned int            id;
	unsigned int            device_counter;

	LPDIRECTINPUT8          di;
	LPDIRECTINPUTDEVICE8    joystick;

	DIJOYSTATE2 js;

	HINSTANCE m_hHInstance;
	HWND m_hHwnd;

	/***********************************************/
	
public:
	Joystick(unsigned int id);
	~Joystick();

	HRESULT deviceName(char* name);

	HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	HRESULT Shutdown();

	HRESULT JoystickGetState();

	bool DXInputJoyPress(DWORD button);
	float DXInputGetJoyX(void);
	float DXInputGetJoyY(void);

	BOOL CALLBACK enumCallback(const DIDEVICEINSTANCE* instance, VOID* context);
	

	// Device Querying
	static unsigned int deviceCount();
};


BOOL CALLBACK enumCallback(const DIDEVICEINSTANCE* instance, VOID* context);
BOOL CALLBACK countCallback(const DIDEVICEINSTANCE* instance, VOID* counter);

#endif /* __JOYSTICK_H__ */

#ifndef __KEYBOARD_H__
#define __KEYBOARD_H__

class Keyboard
{

public:

	Keyboard();
	~Keyboard();

	HINSTANCE m_hHInstance;
	HWND m_hHwnd;
	LPDIRECTINPUT8 m_diobj;

	char m_szKeyboard[256];
	/****************************************************************************/
	LPDIRECTINPUTDEVICE8 m_diKeyboard;
	/****************************************************************************/
	void ObjectInit(HINSTANCE hInstance, HWND hWnd);
	void DXInputKeyboardInit(DWORD one = DISCL_BACKGROUND, DWORD two = DISCL_NONEXCLUSIVE);
	void DXInputGetKeyboardState();
	bool DXInputKeyboardPress(DWORD button);
	HRESULT Shutdown();

};

#endif

#ifndef __MOUSE_H__
#define __MOUSE_H__

class Mouse
{
public:
	Mouse();
	~Mouse();

	HINSTANCE m_hHInstance;
	HWND m_hHwnd;
	LPDIRECTINPUT8 m_diobj;

LPDIRECTINPUTDEVICE8 m_diMouse;
/****************************************************************************/
DIMOUSESTATE m_diMouseState;
/****************************************************************************/

	void ObjectInit(HINSTANCE hInstance, HWND hWnd);
	void DXInputMouseInit(DWORD one = DISCL_BACKGROUND, DWORD two = DISCL_NONEXCLUSIVE);
	void DXInputGetMouseState();
	bool DXInputMousePress(DWORD button);
	float DXInputGetMouseX(void);
	float DXInputGetMouseY(void);
	HRESULT Shutdown();
};

#endif