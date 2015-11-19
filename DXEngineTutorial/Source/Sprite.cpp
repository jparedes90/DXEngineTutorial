#include "Sprite.h"
#include "ResourceManager.h"

Sprite::Sprite(float size)
{
	m_vertexBuffer = NULL;
	m_texture = NULL;

	m_size = size;
}

Sprite::~Sprite(void)
{
	if(m_vertexBuffer)
	{
		delete m_vertexBuffer;
		m_vertexBuffer = NULL;
	}
}

void Sprite::Initialize(ID3D11Device* device, Shader* shader, LPCTSTR textureFileName, bool isWriteAble)
{
	m_shader = shader;

	m_vertexBuffer = new VertexBuffer();
	m_vertexBuffer->Initialize(device, shader, m_size, isWriteAble);

	if(textureFileName != NULL)
	{
		m_texture = ResourceManager::GetInstance()->GetTextureByName((char*)textureFileName);
	}
}

void Sprite::Render(ID3D11DeviceContext* deviceContext, XMFLOAT4X4 worldMatrix, XMFLOAT4X4 viewMatrix, XMFLOAT4X4 projectionMatrix)
{
	if(m_texture)
	{
		m_shader->SetShaderParamters(deviceContext, m_texture->GetTexture());
		m_shader->SetShaderParamters(deviceContext, worldMatrix, viewMatrix, projectionMatrix);

		m_vertexBuffer->Render(deviceContext);
	}
}
