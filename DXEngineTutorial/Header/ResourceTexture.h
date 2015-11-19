#ifndef _RESOURCE_TEXTURE_H
#define _RESOURCE_TEXTURE_H

#include "Texture.h"

class ResourceTexture
{
public:
	ResourceTexture(void);
	~ResourceTexture(void);

	bool Load(ID3D11Device* device, WCHAR* textureFileName);

	Texture* GetTexture();
	string GetName();

private:
	Texture* m_texture;
};

#endif