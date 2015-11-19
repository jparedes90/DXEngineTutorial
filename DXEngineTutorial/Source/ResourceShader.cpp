#include "ResourceShader.h"

ResourceShader::ResourceShader(void)
{
	m_shader = NULL;
}

ResourceShader::~ResourceShader(void)
{
	if(m_shader)
	{
		delete m_shader;
		m_shader = NULL;
	}
}

bool ResourceShader::Load(ID3D11Device* device, HWND hwnd, WCHAR* shaderFileName)
{
	m_shader = new Shader(device, hwnd, shaderFileName);
	if(!m_shader->IsInitialized())
	{
		delete m_shader;
		m_shader = NULL;

		return false;
	}

	return true;
}

bool ResourceShader::Load(Shader* shader)
{
	if(shader == NULL) return false;

	m_shader = shader;
	if(!m_shader->IsInitialized())
	{
		delete m_shader;
		m_shader = NULL;

		return false;
	}

	return true;
}

Shader* ResourceShader::GetShader()
{
	return m_shader;
}

std::string ResourceShader::GetName()
{
	return m_shader->GetName();
}
