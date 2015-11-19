#include "FrameWork.h"
#include "GameScene.h"

void main()
{
	FrameWork* frameWork = new FrameWork();

	if(frameWork->Initialize(new GameScene()))
	{
		frameWork->Run();
	}

	delete frameWork;

	return;
}