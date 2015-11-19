#ifndef _RESOURCE_MANAGER_H
#define _RESOURCE_MANAGER_H

#include "ResourceShader.h"
#include "ResourceTexture.h"

#include <iostream>
#include <vector>
using namespace std;

class ResourceManager
{
public:
	~ResourceManager(void);

	void LoadShaderResource(ID3D11Device* device, HWND hwnd, WCHAR* shaderFileName);
	void LoadShaderResource(Shader* shader);
	void LoadTextureResource(ID3D11Device* device, WCHAR* textureFileName);

	Shader* GetShaderByName(char* shaderName);
	Texture* GetTextureByName(char* textureName);

	static ResourceManager* GetInstance();

private:
	ResourceManager(void);

	vector<ResourceTexture*> m_textures;
	vector<ResourceShader*> m_shaders;

	static ResourceManager* m_instance;
};

#endif