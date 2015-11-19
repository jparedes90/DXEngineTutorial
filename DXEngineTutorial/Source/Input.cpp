#include "Input.h"

Input::Input(void)
{
	m_directInput = 0;
	m_keyboard = 0;
	m_mouse = 0;
	memset(m_keys, 0, sizeof(bool)*s_NumKeys);
}

Input::~Input(void)
{
	// Release the mouse.
	if(m_mouse)
	{
		m_mouse->Unacquire();
		m_mouse->Release();
		m_mouse = 0;
	}

	// Release the keyboard.
	if(m_keyboard)
	{
		m_keyboard->Unacquire();
		m_keyboard->Release();
		m_keyboard = 0;
	}

	// Release the main interface to direct input.
	if(m_directInput)
	{
		m_directInput->Release();
		m_directInput = 0;
	}
}

bool Input::Initialize(HINSTANCE hInstance, HWND hwnd, int screenWidth, int screenHeight)
{
	HRESULT result;

	m_screenWidth = screenWidth;
	m_screenHeight = screenHeight;

	m_mouseX = 0;
	m_mouseY = 0;

	//initialize the direct input interface
	result = DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_directInput, NULL);
	if(FAILED(result))
	{
		return false;
	}

	//initialize keyboard interface
	result = m_directInput->CreateDevice(GUID_SysKeyboard, &m_keyboard, NULL);
	if(FAILED(result))
	{
		return false;
	}

	//set data format for keyboard
	result = m_keyboard->SetDataFormat(&c_dfDIKeyboard);
	if(FAILED(result))
	{
		return false;
	}

	////set cooperative level of the keyboard so it doesnt share with other programs
	result = m_keyboard->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_EXCLUSIVE);
	if(FAILED(result))
	{
		return false;
	}

	result = m_keyboard->Acquire();
	if(FAILED(result))
	{
		return false;
	}

	//initialize mouse interface
	result = m_directInput->CreateDevice(GUID_SysMouse, &m_mouse, NULL);
	if(FAILED(result))
	{
		return false;
	}

	//set data format for mouse
	result = m_mouse->SetDataFormat(&c_dfDIMouse);
	if(FAILED(result))
	{
		return false;
	}

	//set cooperative level of the mouse to not share with other programs
	result = m_mouse->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	if(FAILED(result))
	{
		return false;
	}

	result = m_mouse->Acquire();
	if(FAILED(result))
	{
		return false;
	}

	return true;
}

bool Input::Update()
{
	memcpy(m_prevKeys, m_keys, sizeof(bool) * s_NumKeys);

	bool result;

	//read keyboard
	result = ReadKeyboard();
	if(!result)
	{
		return false;
	}

	//read mouse
	result = ReadMouse();
	if(!result)
	{
		return false;
	}

	//update mouse position
	ProcessInput();

	return true;
}

bool Input::IsKeyDown(unsigned int key)
{
	return m_keys[key];
}

bool Input::IsKeyHit(unsigned int key)
{
	return m_keys[key] && !m_prevKeys[key];
}

void Input::GetMousePosition(int& x, int& y)
{
	x = m_mouseX;
	y = m_mouseY;
}

bool Input::ReadKeyboard()
{
	HRESULT result;

	// Read the keyboard device.
	result = m_keyboard->GetDeviceState(sizeof(m_keys), (LPVOID)&m_keys);
	if(FAILED(result))
	{
		// If the keyboard lost focus or was not acquired then try to get control back.
		if((result == DIERR_INPUTLOST) || (result == DIERR_NOTACQUIRED))
		{
			m_keyboard->Acquire();
		}
		else
		{
			return false;
		}
	}

	return true;
}

bool Input::ReadMouse()
{
	//sometimes mouse doesnt get initialized when putting a breakpoint before it reaches its update
	if(m_mouse == NULL)
	{
		m_directInput->CreateDevice(GUID_SysMouse, &m_mouse, NULL);
		return false;
	}

	HRESULT result;

	//read mouse device
	result = m_mouse->GetDeviceState(sizeof(DIMOUSESTATE), (LPVOID)&m_mouseState);
	if(FAILED(result))
	{
		if((result == DIERR_INPUTLOST) || (result == DIERR_NOTACQUIRED))
		{
			m_mouse->Acquire();
		}
		else
		{
			return false;
		}
	}

	return false;
}

void Input::ProcessInput()
{
	m_mouseX += m_mouseState.lX;
	m_mouseX += m_mouseState.lY;

	if(m_mouseX < 0) m_mouseX = 0;
	if(m_mouseY < 0) m_mouseY = 0;
	if(m_mouseX > m_screenWidth) m_mouseX = m_screenWidth;
	if(m_mouseY > m_screenHeight) m_mouseY = m_screenHeight;
}
