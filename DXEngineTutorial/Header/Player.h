#ifndef _PLAYER_H
#define _PLAYER_H

#include "Engine.h"
class Entity;

class Player
{
public:
	Player(void);
	~Player(void);

	void Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, Shader* shader);
	void Update();
	void UpdateInput();

private:
	Entity* m_entity;
};

#endif