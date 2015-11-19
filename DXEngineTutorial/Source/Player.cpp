#include "Player.h"

Player::Player(void)
{
	m_entity = NULL;
}

Player::~Player(void)
{
}

void Player::Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, Shader* shader)
{
	m_entity = EntityManager::GetInstance()->AddEntity();
	m_entity->InitializeAnimatedSprite(device, deviceContext, shader, "pikachu-walking", 130, 24);
}

void Player::Update()
{
	UpdateInput();
}

void Player::UpdateInput()
{
	m_entity->SetVelocity(0, 0);

	Input* input = Engine::GetEngine()->GetInput();
	if(input == NULL) return;

	if(input->IsKeyDown(DIK_D))
	{
		m_entity->SetVelocity(150.0f, 0.0f);
	}

	if(input->IsKeyDown(DIK_A))
	{
		m_entity->SetVelocity(-150.0f, 0.0f);
	}
}
