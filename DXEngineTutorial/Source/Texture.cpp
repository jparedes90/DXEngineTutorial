#include "Texture.h"

using namespace DirectX;

Texture::Texture(void)
{
	m_texture = 0;
	m_width = 0;
	m_height = 0;
}

Texture::~Texture(void)
{
	if(m_texture)
	{
		m_texture->Release();
		m_texture = NULL;
	}

	m_name.clear();
}

bool Texture::Initialize(ID3D11Device* device, WCHAR* fileName)
{
	HRESULT result;

	wstring tempName(fileName);

	m_name = string(tempName.begin(), tempName.end());

	int pos = m_name.find_last_of("/");
	if(pos >= 0)
	{
		m_name = m_name.substr(pos + 1, m_name.length());
	}
	m_name = m_name.substr(0, m_name.find_last_of("."));

	//load the texture
	result = CreateWICTextureFromFile(device, fileName, NULL, &m_texture, NULL);
	if(FAILED(result))
	{
		return false;
	}

	//get width and height
	ID3D11Resource* resource = 0;
	m_texture->GetResource(&resource);

	ID3D11Texture2D* texture2D = 0;
	result = resource->QueryInterface(&texture2D);

	if(SUCCEEDED(result))
	{
		D3D11_TEXTURE2D_DESC desc;
		texture2D->GetDesc(&desc);

		m_width = desc.Width;
		m_height = desc.Height;
	}

	texture2D->Release();
	resource->Release();

	return true;
}

ID3D11ShaderResourceView* Texture::GetTexture()
{
	return m_texture;
}

string Texture::GetName()
{
	return m_name;
}

int Texture::GetWidth()
{
	return m_width;
}

int Texture::GetHeight()
{
	return m_height;
}
