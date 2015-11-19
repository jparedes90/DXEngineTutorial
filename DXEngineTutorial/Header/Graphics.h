#ifndef _GRAPHICS_H
#define _GRAPHICS_H

#include "DXManager.h"
#include "SystemDefs.h"

class Graphics
{
public:
	Graphics(void);
	~Graphics(void);

	bool InitializeDX(HWND hwnd);
	void Initialize();
	void BeginScene(float r, float g, float b, float a);
	void EndScene();

	void EnableAlphaBlending(bool enable);
	void EnableZBuffer(bool enable);

	DXManager* GetDXManager();
	HWND GetHwnd();
	ID3D11Device* GetDevice();
	ID3D11DeviceContext* GetDeviceContext();

	void SetHwnd(HWND hwnd);

private:
	DXManager* m_dxManager;
	HWND m_hwnd;
};

#endif