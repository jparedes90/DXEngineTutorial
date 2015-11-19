#ifndef _INPUT_H
#define _INPUT_H

//Pre-Processing Directive
#define DIRECTINPUT_VERSION 0x0800//removes annoying message defaulting to version 8

#include <dinput.h>

class Input
{
public:
	Input(void);
	~Input(void);

	bool Initialize(HINSTANCE hInstance, HWND hwnd, int screenWidth, int screenHeight);
	bool Update();

	bool IsKeyDown(unsigned int key);
	bool IsKeyHit(unsigned int key);
	void GetMousePosition(int& x, int& y);

private:
	const static int s_NumKeys = 256;

	bool ReadKeyboard();
	bool ReadMouse();
	void ProcessInput();

	IDirectInput8 * m_directInput;
	IDirectInputDevice8* m_keyboard;
	IDirectInputDevice8* m_mouse;
	DIMOUSESTATE m_mouseState;

	bool m_keys[s_NumKeys];
	bool m_prevKeys[s_NumKeys];

	int m_screenWidth;
	int m_screenHeight;
	int m_mouseX;
	int m_mouseY;
};

#endif