#include "Graphics.h"

Graphics::Graphics(void)
{
	m_dxManager = NULL;
}

Graphics::~Graphics(void)
{
	if(m_dxManager)
	{
		delete m_dxManager;
		m_dxManager = NULL;
	}
}

bool Graphics::InitializeDX(HWND hwnd)
{
	m_dxManager = new DXManager();
	if(!m_dxManager->Initialize((int)SCREEN_WIDTH, (int)SCREEN_HEIGHT, VSYNC_ENABLED, hwnd, FULL_SCREEN))
	{
		return false;
	}

	return true;
}

void Graphics::Initialize()
{

}

void Graphics::BeginScene(float r, float g, float b, float a)
{
	m_dxManager->BeginScene(r, g, b, a);
}

void Graphics::EndScene()
{
	m_dxManager->EndScene();
}

void Graphics::EnableAlphaBlending(bool enable)
{
	m_dxManager->EnableAlphaBlending(enable);
}

void Graphics::EnableZBuffer(bool enable)
{
	m_dxManager->EnableZBuffer(enable);
}

DXManager* Graphics::GetDXManager()
{
	return m_dxManager;
}

HWND Graphics::GetHwnd()
{
	return m_hwnd;
}

ID3D11Device* Graphics::GetDevice()
{
	return m_dxManager->GetDevice();
}

ID3D11DeviceContext* Graphics::GetDeviceContext()
{
	return m_dxManager->GetDeviceContext();
}

void Graphics::SetHwnd(HWND hwnd)
{
	m_hwnd = hwnd;
}
