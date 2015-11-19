#ifndef _TEXTURE_SHADER_H
#define _TEXTURE_SHADER_H

#include "Shader.h"

class TextureShader : public Shader
{
public:
	TextureShader(ID3D11Device* device, HWND hwnd, WCHAR* shaderFileName);
	virtual ~TextureShader(void);

	void Begin(ID3D11DeviceContext* deviceContext, int indexCount);
	void End(ID3D11DeviceContext* deviceContext);

protected:
	bool Initialize(ID3D11Device* device, HWND hwnd, WCHAR* shaderFileName);
	bool InitializeSamplerState(ID3D11Device* device);

private:
	ID3D11SamplerState* m_samplerState;

};

#endif