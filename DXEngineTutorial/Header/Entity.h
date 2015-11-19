#ifndef _ENTITY_H_
#define _ENTITY_H_

#include "AnimatedSprite.h"

using namespace DirectX;

class Entity
{
public:
	friend class EntityManager;
	~Entity(void);

	void InitializeSprite(ID3D11Device* device, Shader* shader, LPCSTR textureFileName, float spriteSize);
	void InitializeAnimatedSprite(ID3D11Device* device, ID3D11DeviceContext* deviceContext, Shader* shader, LPCSTR textureFileName,
									float spriteSize, float framesPerSecond, float animationSpeed = 1.0f, bool isLooping = true);

	void Update();
	void Render(ID3D11DeviceContext* deviceContext, XMFLOAT4X4 viewMatrix, XMFLOAT4X4 projectionMatrix);

	void SetPosition(float x = 0.0f, float y = 0.0f);
	void SetVelocity(float x = 0.0f, float y = 0.0f);

private:
	Entity(void);

	XMFLOAT4X4 m_worldMatrix;
	XMVECTOR m_position;
	XMVECTOR m_velocity;
	Sprite* m_sprite;
};

#endif