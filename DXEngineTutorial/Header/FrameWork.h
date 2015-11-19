#ifndef _FRAME_WORK_H
#define _FRAME_WORK_H

#define WIN32_LEAN_AND_MEAN

#include "Engine.h"

class FrameWork
{
public:
	FrameWork(void);
	~FrameWork(void);

	bool Initialize(GameComponent* gameComponent);
	void Run();

private:
	bool CreateDXWindow(char* windowTitle, int x, int y, int width, int height);
	
	char* m_applicationName;
	HINSTANCE m_hInstance;
};

#endif