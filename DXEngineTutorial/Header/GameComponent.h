#ifndef _GAME_COMPONENT_H
#define _GAME_COMPONENT_H

#include <d3d11_1.h>
#include <directxmath.h>

using namespace DirectX;

class GameComponent
{
public:
	GameComponent(){}
	virtual ~GameComponent(){}

	virtual bool Initialize() = 0;
	virtual void Update() = 0;
	virtual void Render(ID3D11DeviceContext* deviceContext, XMFLOAT4X4 viewMatrix, XMFLOAT4X4 projectionMatrix) = 0;
};

#endif