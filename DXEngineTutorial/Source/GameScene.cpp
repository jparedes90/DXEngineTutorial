#include "GameScene.h"
#include "TextureShader.h"

GameScene::GameScene(void)
{
}


GameScene::~GameScene(void)
{
	if(m_player)
	{
		delete m_player;
	}
}

bool GameScene::Initialize()
{
	TextureShader* shader = (TextureShader*)ResourceManager::GetInstance()->GetShaderByName("texture.fx");
	if(shader == NULL)
	{
		return false;
	}

	m_player = new Player();
	m_player->Initialize(Engine::GetEngine()->GetGraphics()->GetDevice(), Engine::GetEngine()->GetGraphics()->GetDeviceContext(), shader);

	return true;
}

void GameScene::Update()
{
	m_player->Update();
}

void GameScene::Render(ID3D11DeviceContext* deviceContext, XMFLOAT4X4 viewMatrix, XMFLOAT4X4 projectionMatrix)
{

}
