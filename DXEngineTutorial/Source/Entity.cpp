#include "Entity.h"
#include "Timer.h"

Entity::Entity(void)
{
	m_position = XMVectorSet(0, 0, 0, 0);
	m_velocity = XMVectorSet(0, 0, 0, 0);
	XMStoreFloat4x4(&m_worldMatrix, XMMatrixIdentity());
	m_sprite = NULL;
}

Entity::~Entity(void)
{
	if(m_sprite != NULL)
	{
		delete m_sprite;
		m_sprite = NULL;
	}
}

void Entity::InitializeSprite(ID3D11Device* device, Shader* shader, LPCSTR textureFileName, float spriteSize)
{
	m_sprite = new Sprite(spriteSize);
	m_sprite->Initialize(device, shader, textureFileName);
}

void Entity::InitializeAnimatedSprite(ID3D11Device* device, ID3D11DeviceContext* deviceContext, Shader* shader, LPCSTR textureFileName, 
									float spriteSize, float framesPerSecond, float animationSpeed, bool isLooping)
{
	m_sprite = new AnimatedSprite(spriteSize, framesPerSecond, animationSpeed, isLooping);
	((AnimatedSprite*)m_sprite)->Initialize(device, deviceContext, shader, textureFileName);
}

void Entity::Update()
{
	float dt = Timer::GetDeltaTime();

	m_position += m_velocity * dt;
	m_velocity *= 0.96f;

	XMStoreFloat4x4(&m_worldMatrix, XMMatrixTranslation(m_position.m128_f32[0], m_position.m128_f32[1], m_position.m128_f32[2]));

	if(m_sprite)
	{
		m_sprite->Update();
	}
}

void Entity::Render(ID3D11DeviceContext* deviceContext, XMFLOAT4X4 viewMatrix, XMFLOAT4X4 projectionMatrix)
{
	if(m_sprite)
	{
		m_sprite->Render(deviceContext, m_worldMatrix, viewMatrix, projectionMatrix);
	}
}

void Entity::SetPosition(float x, float y)
{
	m_position = XMVectorSet(x, y, 0, 0);
}

void Entity::SetVelocity(float x, float y)
{
	m_velocity = XMVectorSet(x, y, 0, 0);
}
