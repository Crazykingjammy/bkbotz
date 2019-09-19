#include "DXInput.h"
#include <stdio.h>

#define SAFE_RELEASE(p)     { if(p) { (p)->Release(); (p) = NULL; } }

Joystick::Joystick(unsigned int id)
{
	
	this->id = id;
	device_counter = 0;
	memset(ControllerName, 0, sizeof(ControllerName));

	di = NULL;
	joystick = NULL;
}

Joystick::~Joystick()
{
	Shutdown();
}

HRESULT
Joystick::deviceName(char* name)
{
	HRESULT hr;
	DIDEVICEINSTANCE device;

	ZeroMemory(&device, sizeof(device));
	device.dwSize = sizeof(device);

 	if (!di || !joystick) {
		return E_INVALIDARG;
	}

	if (FAILED(hr = joystick->GetDeviceInfo(&device))) {
		return hr;
	}

	strcpy(name, device.tszProductName);

	return hr;
}

HRESULT
Joystick::Init(HINSTANCE hInstance, HWND hWnd)
{
	m_hHInstance = hInstance;
	m_hHwnd = hWnd;

	HRESULT hr;

	// Create a DirectInput device
	if (FAILED(hr = DirectInput8Create(m_hHInstance,
		DIRECTINPUT_VERSION, 
		IID_IDirectInput8,
		(VOID**)&di, NULL))) {
			return hr;
		}

		// Look for the first simple joystick we can find.
		if (FAILED(hr = di->EnumDevices(DI8DEVCLASS_GAMECTRL, ::enumCallback,
			(LPVOID)this, DIEDFL_ATTACHEDONLY))) {
				return hr;
			}

			// Make sure we got a joystick
			if (joystick == NULL) {
				return E_FAIL;
			}

			// Set the data format to "simple joystick" - a predefined data format 
			//
			// A data format specifies which controls on a device we are interested in,
			// and how they should be reported. This tells DInput that we will be
			// passing a DIJOYSTATE2 structure to IDirectInputDevice::GetDeviceState().
			if (FAILED(hr = joystick->SetDataFormat(&c_dfDIJoystick2))) {
				return hr;
			}

			// Set the cooperative level to let DInput know how this device should
			// interact with the system and with other DInput applications.
			if (FAILED(hr = joystick->SetCooperativeLevel(m_hHwnd, DISCL_EXCLUSIVE | DISCL_FOREGROUND))) {
				return hr;
			}

			// Additional Joystick Setup.
				// Set the Axis Ranges for the Joystick.
				DIPROPRANGE diPropRange;

				// Set the size of the structures.
				diPropRange.diph.dwSize = sizeof(DIPROPRANGE);
				diPropRange.diph.dwHeaderSize = sizeof(DIPROPHEADER);

				// Set the minimum and maximum values.
				diPropRange.lMin = -1000;
				diPropRange.lMax = 1000;

				// How to Set them in memory.
				diPropRange.diph.dwHow = DIPH_BYOFFSET;

				// Which axis to change.
				diPropRange.diph.dwObj = DIJOFS_X;

				// Set the X-Axis Property.
				joystick->SetProperty(DIPROP_RANGE, &diPropRange.diph);

				// Set the Y-Axis Property.
				diPropRange.diph.dwObj = DIJOFS_Y;
				joystick->SetProperty(DIPROP_RANGE, &diPropRange.diph);


			return S_OK;
}

HRESULT
Joystick::Shutdown()
{
	if (joystick) { 
		joystick->Unacquire();
	}

	SAFE_RELEASE(joystick);
	SAFE_RELEASE(di);

	return S_OK;
}

HRESULT
Joystick::JoystickGetState()
{
	HRESULT hr;

	if (joystick == NULL) {
		return S_OK;
	}

	// Poll the device to read the current state
	hr = joystick->Poll(); 
	if (FAILED(hr)) {

		// DirectInput is telling us that the input stream has been
		// interrupted.  We aren't tracking any state between polls, so we
		// don't have any special reset that needs to be done.  We just
		// re-acquire and try again.
		hr = joystick->Acquire();

		while (hr == DIERR_INPUTLOST) {
			hr = joystick->Acquire();
		}

		// If we encounter a fatal error, return failure.
		if ((hr == DIERR_INVALIDPARAM) || (hr == DIERR_NOTINITIALIZED)) {
			return E_FAIL;
		}

		// If another application has control of this device, return success.
		// We'll just have to wait our turn to use the joystick.
		if (hr == DIERR_OTHERAPPHASPRIO) {
			return S_OK;
		}
	}

	// Get the input's device state
	if (FAILED(hr = joystick->GetDeviceState(sizeof(DIJOYSTATE2), &js))) {
		return hr;
	}

	return S_OK;
}

BOOL CALLBACK
Joystick::enumCallback(const DIDEVICEINSTANCE* instance, VOID* context)
{
	  BOOL                 bEnumForce ;
	
	DeviceGuid = instance->guidInstance;
	
	// Are we enuming force feedback joysticks?
	bEnumForce =  (BOOL)context ;

	// If this is the requested device ID ...
	if (device_counter == this->id) {

		// Obtain an interface to the enumerated joystick.  Stop the enumeration
		// if the requested device was created successfully.
		if (SUCCEEDED(di->CreateDevice(DeviceGuid, &joystick, NULL))) {
			return DIENUM_STOP;
		}  
	}

	// Otherwise, increment the device counter and continue with
	// the device enumeration.
	device_counter++;

	return DIENUM_CONTINUE;
}

BOOL CALLBACK
enumCallback(const DIDEVICEINSTANCE* instance, VOID* context)
{
	
	if (context != NULL) {
		return ((Joystick *)context)->enumCallback(instance, context);
	} else {
		return DIENUM_STOP;
	}
}

unsigned int
Joystick::deviceCount()
{
	unsigned int counter = 0;
	LPDIRECTINPUT8 di = NULL;
	HRESULT hr;

	if (SUCCEEDED(hr = DirectInput8Create(GetModuleHandle(NULL),
		DIRECTINPUT_VERSION, 
		IID_IDirectInput8,
		(VOID**)&di, NULL))) {
			di->EnumDevices(DI8DEVCLASS_GAMECTRL, ::countCallback,
				&counter, DIEDFL_ATTACHEDONLY);
		}

		return counter;
}

BOOL CALLBACK
countCallback(const DIDEVICEINSTANCE* instance, VOID* counter)
{
//	DeviceGuid = instance->guidInstance;

	if (counter != NULL) {
		unsigned int *tmpCounter = (unsigned int *)counter;
		(*tmpCounter)++;
		counter = tmpCounter;
	}

	return DIENUM_CONTINUE;
}

bool Joystick::DXInputJoyPress(DWORD button)
{

	if (joystick)
	{ 
		if (js.rgbButtons[button] & 0x80)
			return true;
	}

	return false;

}

float Joystick::DXInputGetJoyX(void)
{
	if (!joystick)
		return 0;

	return (float) js.lX;

}
float Joystick::DXInputGetJoyY(void)
{
	if (!joystick)
		return 0;

	return  (float)js.lY;
}

/***************************************************************************************/

Keyboard::Keyboard()
{
	m_diobj = NULL;
}
Keyboard::~Keyboard()
{

}

void Keyboard::ObjectInit(HINSTANCE hInstance, HWND hWnd)
{
	m_hHInstance = hInstance;
	m_hHwnd = hWnd;

	// Create the object for input.
	DirectInput8Create(m_hHInstance, DIRECTINPUT_VERSION,
		IID_IDirectInput8, (void **)&m_diobj, NULL);

}


void Keyboard::DXInputKeyboardInit(DWORD one, DWORD two)
{
	// Create the device for the Keyboard.
	m_diobj->CreateDevice(GUID_SysKeyboard, &m_diKeyboard, NULL);

	// Set the Data Format for the keyboard.
	m_diKeyboard->SetDataFormat(&c_dfDIKeyboard);

	// Set the Cooperative level for the Keyboard.
	m_diKeyboard->SetCooperativeLevel(m_hHwnd, one | two);

	// Acquire the Keyboard.
	m_diKeyboard->Acquire();
}


void Keyboard::DXInputGetKeyboardState()
{
	// 6. Get the information from the device for the Keyboard.
	memset(m_szKeyboard, 0, sizeof(m_szKeyboard));
	HRESULT hr = 0;
	if (FAILED(hr = m_diKeyboard->GetDeviceState(sizeof(m_szKeyboard), (LPVOID)&m_szKeyboard)))
	{
		// Make sure the keyboard did not lose acquistion.
		if (hr = DIERR_INPUTLOST)
			m_diKeyboard->Acquire();
	}

}

bool Keyboard::DXInputKeyboardPress(DWORD button)
{
	if (button < 0 || button > 255)
		return false;

	if (m_szKeyboard[button] & 0x80)
		return true;
	else
		return false;
}

HRESULT
Keyboard::Shutdown()
{
	if (m_diKeyboard) { 
		m_diKeyboard->Unacquire();
	}

	SAFE_RELEASE(m_diKeyboard);

	return S_OK;
}

/***********************************************************/

Mouse::Mouse()
{
	m_diobj = NULL;
}

Mouse::~Mouse()
{
}


void Mouse::ObjectInit(HINSTANCE hInstance, HWND hWnd)
{
	m_hHInstance = hInstance;
	m_hHwnd = hWnd;

	// Create the object for input.
	DirectInput8Create(m_hHInstance, DIRECTINPUT_VERSION,
		IID_IDirectInput8, (void **)&m_diobj, NULL);

}

void Mouse::DXInputMouseInit(DWORD one, DWORD two)
{
	// Create the device for the Mouse.
	m_diobj->CreateDevice(GUID_SysMouse, &m_diMouse, NULL);

	// Set the Data Format fo the Mouse.
	m_diMouse->SetDataFormat(&c_dfDIMouse);

	// Set the Cooperative level for the mouse.
	m_diMouse->SetCooperativeLevel(m_hHwnd, one | two);

	// Acquire the Mouse.
	m_diMouse->Acquire();


}

void Mouse::DXInputGetMouseState()
{
	// for the mouse.
	memset(&m_diMouseState, 0, sizeof(m_diMouseState));
	HRESULT hr = 0;
	if (FAILED(hr = m_diMouse->GetDeviceState(sizeof(m_diMouseState), (LPVOID)&m_diMouseState)))
	{
		// Make sure the keyboard did not lose acquistion.
		if (hr = DIERR_INPUTLOST)
			m_diMouse->Acquire();
	}
}

bool Mouse::DXInputMousePress(DWORD button)
{
	if (button < 0 || button > 7)
		return false;

	if (m_diMouseState.rgbButtons[button] & 0x80)
		return true;
	else
		return false;
}

float Mouse::DXInputGetMouseX(void)
{
	return (float)m_diMouseState.lX;
}
float Mouse::DXInputGetMouseY(void)
{
	return (float)m_diMouseState.lY;
}

HRESULT
Mouse::Shutdown()
{
	if (m_diMouse) { 
		m_diMouse->Unacquire();
	}

	SAFE_RELEASE(m_diMouse);

	return S_OK;
}