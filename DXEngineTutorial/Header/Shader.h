#ifndef _SHADER_H
#define _SHADER_H

#include <d3d11_1.h>
#include <directxmath.h>
#include <fstream>
#include <string>
using namespace std;

using namespace DirectX;

class Shader
{
private:
	struct MatrixBufferType
	{
		XMMATRIX worldMatrix;
		XMMATRIX viewMatrix;
		XMMATRIX projectionMatrix;
	};
public:
	Shader(ID3D11Device* device, HWND hwnd, WCHAR* shaderFileName);
	virtual ~Shader(void);

	virtual void Begin(ID3D11DeviceContext* deviceContext, int indexCount);
	virtual void End(ID3D11DeviceContext* deviceContext);

	bool SetShaderParamters(ID3D11DeviceContext* deviceContext, ID3D11ShaderResourceView* texture);
	bool SetShaderParamters(ID3D11DeviceContext* deviceContext, XMFLOAT4X4 worldMatrix, XMFLOAT4X4 viewMatrix, XMFLOAT4X4 projectionMatrix);

	string GetName();
	bool IsInitialized();

protected:
	virtual bool Initialize(ID3D11Device* device, HWND hwnd, WCHAR* shaderFileName);

	bool m_initialized;

private:
	bool InitializeShader(ID3D11Device* device, HWND hwnd, WCHAR* shaderFileName);
	void OutputShadeErrorMessage(ID3D10Blob* errorMessage, HWND hwnd, WCHAR* shaderFileName);

	ID3D11VertexShader* m_vertexShader;
	ID3D11PixelShader* m_pixelShader;
	ID3D11InputLayout* m_layout;
	ID3D11Buffer* m_matrixBuffer;
	string m_name;
};

#endif