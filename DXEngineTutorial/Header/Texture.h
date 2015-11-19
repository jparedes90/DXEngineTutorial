#ifndef _TEXTURE_H
#define _TEXTURE_H

#include <d3d11_1.h>

#include "WICTextureLoader.h"

#include <string>
using namespace std;

class Texture
{
public:
	Texture(void);
	~Texture(void);

	bool Initialize(ID3D11Device* device, WCHAR* fileName);

	ID3D11ShaderResourceView* GetTexture();
	string GetName();

	int GetWidth();
	int GetHeight();

private:
	ID3D11ShaderResourceView* m_texture;
	string m_name;

	int m_width;
	int m_height;
};

#endif