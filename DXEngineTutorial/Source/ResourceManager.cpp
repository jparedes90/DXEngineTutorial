#include "ResourceManager.h"

ResourceManager* ResourceManager::m_instance = NULL;

ResourceManager::ResourceManager(void)
{
}

ResourceManager::~ResourceManager(void)
{
	while(!m_textures.empty())
	{
		delete m_textures[0];

		m_textures.erase(m_textures.begin());
	}

	while(!m_shaders.empty())
	{
		delete m_shaders[0];

		m_shaders.erase(m_shaders.begin());
	}
}

void ResourceManager::LoadShaderResource(ID3D11Device* device, HWND hwnd, WCHAR* shaderFileName)
{
	ResourceShader* resourceShader = new ResourceShader();
	if(!resourceShader->Load(device, hwnd, shaderFileName))
	{
		delete resourceShader;

		cout << "Unable To Load " << shaderFileName << endl;

		return;
	}

	m_shaders.push_back(resourceShader);

	cout << "Successfully Loaded " << shaderFileName << endl;
}

void ResourceManager::LoadShaderResource(Shader* shader)
{
	if(shader == NULL) return;

	ResourceShader* resourceShader = new ResourceShader();
	if(!resourceShader->Load(shader))
	{
		delete resourceShader;

		cout << "Unable To Load Shader" << endl;
		
		return;
	}

	m_shaders.push_back(resourceShader);

	cout << "Successfully Loaded Shader" << resourceShader->GetName() << endl;
}

void ResourceManager::LoadTextureResource(ID3D11Device* device, WCHAR* textureFileName)
{
	ResourceTexture* resourceTexture = new ResourceTexture();
	if(!resourceTexture->Load(device, textureFileName))
	{
		delete resourceTexture;

		cout << "Unable To Load " << textureFileName << endl;

		return;
	}

	m_textures.push_back(resourceTexture);

	cout << "Successfully Loaded " << textureFileName << endl;
}

Shader* ResourceManager::GetShaderByName(char* shaderName)
{
	for(int i = 0; i < (int)m_shaders.size(); ++i)
	{
		ResourceShader* resourceShader = m_shaders[i];
		string resourceShaderName = resourceShader->GetName();

		if(!strcmp(shaderName, resourceShaderName.c_str()))
		{
			return resourceShader->GetShader();
		}
	}

	return NULL;
}

Texture* ResourceManager::GetTextureByName(char* textureName)
{
	for(int i = 0; i < (int)m_textures.size(); ++i)
	{
		ResourceTexture* resourceTexture = m_textures[i];
		string resourceTextureName = resourceTexture->GetName();

		if(!strcmp(textureName, resourceTextureName.c_str()))
		{
			return resourceTexture->GetTexture();
		}
	}

	return NULL;
}

ResourceManager* ResourceManager::GetInstance()
{
	if(m_instance == NULL)
	{
		m_instance = new ResourceManager();
	}

	return m_instance;
}
