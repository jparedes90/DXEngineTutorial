#include "AnimatedSprite.h"
#include "Timer.h"
#include "Engine.h"

AnimatedSprite::AnimatedSprite(float size, float framesPerSecond, float animationSpeed, bool isLooping) :
	Sprite(size)
{
	m_framesPerSecond = (1000.0f/framesPerSecond)/1000.0f;
	m_animationSpeed = animationSpeed;
	m_isLooping = isLooping;
	m_currentSpeed = 0;
}

AnimatedSprite::~AnimatedSprite(void)
{
}

void AnimatedSprite::Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, Shader* shader, LPCTSTR textureFileName)
{
	Sprite::Initialize(device, shader, textureFileName, true);

	m_deviceContext = deviceContext;
	m_currentFrame = 0;
	m_previousFrame = -1.0f;
	m_maxFrames = (float)m_texture->GetWidth()/(float)m_texture->GetHeight();
}

void AnimatedSprite::Update()
{
	if(m_maxFrames == 1.0f) return;

	if(m_currentFrame < m_maxFrames)
	{
		float dt = Timer::GetDeltaTime();

		m_currentSpeed += m_animationSpeed * dt;

		if(m_currentSpeed > m_framesPerSecond)
		{
			m_currentFrame++;
			m_currentSpeed = 0.0f;

			if(m_currentFrame >= m_maxFrames)
			{
				if(m_isLooping)
				{
					m_currentFrame = 0.0f;
				}
				else
				{
					m_currentFrame = m_maxFrames;
				}
			}
		}
	}

	//no reason to lock/update the buffer if its still the same frame
	if(m_currentFrame == m_previousFrame) return;

	D3D11_MAPPED_SUBRESOURCE mappedResource;
	VertexBuffer::VertexType* vertices = m_vertexBuffer->GetVertices();

	vertices[0].uv.x = m_currentFrame/m_maxFrames;
	vertices[0].uv.y = 1.0f;

	vertices[1].uv.x = m_currentFrame/m_maxFrames;
	vertices[1].uv.y = 0.0f;

	vertices[2].uv.x = (m_currentFrame + 1.0f)/m_maxFrames;
	vertices[2].uv.y = 0.0f;

	vertices[3].uv.x = (m_currentFrame + 1.0f)/m_maxFrames;
	vertices[3].uv.y = 1.0f;

	//lock the vertex buffer so it written to
	HRESULT result = m_deviceContext->Map(m_vertexBuffer->GetVertexBuffer(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if(FAILED(result))
	{
		return;
	}

	VertexBuffer::VertexType* verticesPtr = (VertexBuffer::VertexType*)mappedResource.pData;
	memcpy(verticesPtr, (void*)vertices, sizeof(VertexBuffer::VertexType) * m_vertexBuffer->GetVertexCount());

	m_deviceContext->Unmap(m_vertexBuffer->GetVertexBuffer(), 0);

	//update previous frame
	m_previousFrame = m_currentFrame;
}

void AnimatedSprite::Render(ID3D11DeviceContext* deviceContext, XMFLOAT4X4 worldMatrix, XMFLOAT4X4 viewMatrix, XMFLOAT4X4 projectionMatrix)
{
	Engine::GetEngine()->GetGraphics()->EnableAlphaBlending(true);
	Sprite::Render(deviceContext, worldMatrix, viewMatrix, projectionMatrix);
	Engine::GetEngine()->GetGraphics()->EnableAlphaBlending(false);
}
