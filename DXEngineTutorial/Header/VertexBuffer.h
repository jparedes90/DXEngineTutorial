#ifndef _VERTEX_BUFFER_H
#define _VERTEX_BUFFER_H

#include "Shader.h"

#include <d3d11_1.h>
#include <directxmath.h>

class VertexBuffer
{
public:
	struct VertexType
	{
		XMFLOAT3 position;
		XMFLOAT2 uv;
	};

	VertexBuffer(void);
	~VertexBuffer(void);

	bool Initialize(ID3D11Device* device, Shader* shader, float size, bool writeable = false);
	void Render(ID3D11DeviceContext* deviceContext);

	VertexType* GetVertices();
	ID3D11Buffer* GetVertexBuffer();
	int GetIndexCount();
	int GetVertexCount();

private:
	ID3D11Buffer* m_vertexBuffer;
	ID3D11Buffer* m_indexBuffer;
	int m_vertexCount;
	int m_indexCount;
	VertexType* m_vertices;
	Shader* m_shader;
};

#endif