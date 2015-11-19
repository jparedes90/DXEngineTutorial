#ifndef _RESOURCE_SHADER_H
#define _RESOURCE_SHADER_H

#include "Shader.h"

class ResourceShader
{
public:
	ResourceShader(void);
	~ResourceShader(void);

	bool Load(ID3D11Device* device, HWND hwnd, WCHAR* shaderFileName);
	bool Load(Shader* shader);

	Shader* GetShader();
	string GetName();

private:
	Shader* m_shader;
};

#endif