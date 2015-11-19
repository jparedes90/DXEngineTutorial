#ifndef _GAME_SCENE_H
#define _GAME_SCENE_H

#include "GameComponent.h"
#include "Player.h"

class GameScene : public GameComponent
{
public:
	GameScene(void);
	virtual ~GameScene(void);

	bool Initialize();
	void Update();
	void Render(ID3D11DeviceContext* deviceContext, XMFLOAT4X4 viewMatrix, XMFLOAT4X4 projectionMatrix);

private:
	Player* m_player;

};

#endif