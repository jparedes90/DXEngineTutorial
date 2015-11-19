#include "DXManager.h"

DXManager::DXManager(void)
{
	m_swapChain = 0;
	m_device = 0;
	m_deviceContext = 0;
	m_renderTargetView = 0;
	m_depthStencilBuffer = 0;
	m_depthStencilState = 0;
	m_depthStencilView = 0;
	m_rasterState = 0;
	m_alphaEnableBlendingState = 0;
	m_alphaDisableBlendingState = 0;
	m_depthDisabledStencilState = 0;
}

DXManager::~DXManager(void)
{
	// Before shutting down set to windowed mode or when you release the swap chain it will throw an exception.
	if(m_swapChain)
	{
		m_swapChain->SetFullscreenState(false, NULL);
	}

	if(m_alphaEnableBlendingState)
	{
		m_alphaEnableBlendingState->Release();
		m_alphaEnableBlendingState = 0;
	}

	if(m_alphaDisableBlendingState)
	{
		m_alphaDisableBlendingState->Release();
		m_alphaDisableBlendingState = 0;
	}

	if(m_depthDisabledStencilState)
	{
		m_depthDisabledStencilState->Release();
		m_depthDisabledStencilState = 0;
	}

	if(m_rasterState)
	{
		m_rasterState->Release();
		m_rasterState = 0;
	}

	if(m_depthStencilView)
	{
		m_depthStencilView->Release();
		m_depthStencilView = 0;
	}

	if(m_depthStencilState)
	{
		m_depthStencilState->Release();
		m_depthStencilState = 0;
	}

	if(m_depthStencilBuffer)
	{
		m_depthStencilBuffer->Release();
		m_depthStencilBuffer = 0;
	}

	if(m_renderTargetView)
	{
		m_renderTargetView->Release();
		m_renderTargetView = 0;
	}

	if(m_deviceContext)
	{
		m_deviceContext->Release();
		m_deviceContext = 0;
	}

	if(m_device)
	{
		m_device->Release();
		m_device = 0;
	}

	if(m_swapChain)
	{
		m_swapChain->Release();
		m_swapChain = 0;
	}
}

bool DXManager::Initialize(int screenWidth, int screenHeight, bool vsync, HWND hwnd, bool fullscreen)
{
	HRESULT result;
	IDXGIFactory* factory;
	IDXGIAdapter* adapter;
	IDXGIOutput* adapterOutput;
	unsigned int numModes, numerator = 0, denominator = 0, stringLength;
	DXGI_MODE_DESC* displayModeList;
	DXGI_ADAPTER_DESC adapterDesc;
	int error;
	ID3D11Texture2D* backBufferPtr;

	//store vsync setting
	m_vsync_enabled = vsync;

	//Create a DirectX graphic interface factory
	result = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory);
	if(FAILED(result))
	{
		return false;
	}

	//Use the factory to create an adapter for the primary graphics interface
	result = factory->EnumAdapters(0, &adapter);
	if(FAILED(result))
	{
		return false;
	}

	//Enumerate the primary adapter output
	result = adapter->EnumOutputs(0, &adapterOutput);
	if(FAILED(result))
	{
		return false;
	}

	//Get the number of modes that fit the DXGI_FORMAT_R8G8B8A8_UNORM display format for the adapter output
	result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, NULL);
	if(FAILED(result))
	{
		return false;
	}

	//Create a list to hold all the possible modes for this monitor/video card combination
	displayModeList = new DXGI_MODE_DESC[numModes];

	//Fill the display mode list
	result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, displayModeList);
	if(FAILED(result))
	{
		return false;
	}

	for(int i = 0; i < (int)numModes; ++i)
	{
		if(displayModeList[i].Width == (unsigned int)screenWidth)
		{
			if(displayModeList[i].Height == (unsigned int)screenHeight)
			{
				numerator = displayModeList[i].RefreshRate.Numerator;
				denominator = displayModeList[i].RefreshRate.Denominator;
			}
		}
	}

	if(numerator == 0 && denominator == 0)
	{
		return false;
	}

	//Get the adapter description
	result = adapter->GetDesc(&adapterDesc);
	if(FAILED(result))
	{
		return false;
	}

	//Store the video card memory in megabytes
	m_videoCardMemory = (int)(adapterDesc.DedicatedVideoMemory / 1024 / 1024);

	//Convert the name of the video card to a character array
	error = wcstombs_s(&stringLength, m_videoCardDescription, 128, adapterDesc.Description, 128);
	if(error != 0)
	{
		return false;
	}

	//release memory
	delete [] displayModeList;
	displayModeList = 0;

	adapterOutput->Release();
	adapterOutput = 0;

	adapter->Release();
	adapter = 0;

	factory->Release();
	factory = 0;

	if(!InitializeSwapChain(hwnd, fullscreen, screenWidth, screenHeight, numerator, denominator))
	{
		return false;
	}

	//Get the pointer to the back buffer
	result = m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBufferPtr);
	if(FAILED(result))
	{
		return false;
	}

	//create the render target view
	result = m_device->CreateRenderTargetView(backBufferPtr, NULL, &m_renderTargetView);
	if(FAILED(result))
	{
		return false;
	}

	//release back buffer
	backBufferPtr->Release();
	backBufferPtr = 0;

	if(!InitializeDepthBuffer(screenWidth, screenHeight))
	{
		return false;
	}

	if(!InitializeDepthStencilBuffer())
	{
		return false;
	}

	if(!InitializeStencilView())
	{
		return false;
	}

	//bind the render target view and depth stencil buffer to the output render pipeline
	m_deviceContext->OMSetRenderTargets(1, &m_renderTargetView, m_depthStencilView);

	if(!InitializeRasterizerState())
	{
		return false;
	}

	InitializeViewport(screenWidth, screenHeight);

	if(!InitializeAlphaBlending())
	{
		return false;
	}

	if(!InitializeZBuffer())
	{
		return false;
	}

	return true;
}

void DXManager::BeginScene(float r, float g, float b, float a)
{
	float color[4];

	color[0] = r;
	color[1] = g;
	color[2] = b;
	color[3] = a;

	//clear the back buffer
	m_deviceContext->ClearRenderTargetView(m_renderTargetView, color);

	//Clear the depth buffer
	m_deviceContext->ClearDepthStencilView(m_depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

void DXManager::EndScene()
{
	if(m_vsync_enabled)
	{
		//Lock to screen refresh rate
		m_swapChain->Present(1, 0);
	}
	else
	{
		//Present as fast as possible
		m_swapChain->Present(0, 0);
	}
}

bool DXManager::InitializeSwapChain(HWND hwnd, bool fullscreen, int screenWidth, int screenHeight, unsigned int numerator, unsigned int denominator)
{
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	D3D_FEATURE_LEVEL featureLevel;
	HRESULT result;

	//initialize swap chain
	ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));

	//set to a single back buffer
	swapChainDesc.BufferCount = 1;

	//set the width and height of the back buffer
	swapChainDesc.BufferDesc.Width = screenWidth;
	swapChainDesc.BufferDesc.Height = screenHeight;

	//set regular 32-bit back buffer
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

	//Set the refresh rate of the back buffer
	if(m_vsync_enabled)
	{
		swapChainDesc.BufferDesc.RefreshRate.Numerator = numerator;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = denominator;
	}
	else
	{
		swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	}

	// set the usage of the back buffer
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

	//set the handle for the window
	swapChainDesc.OutputWindow = hwnd;

	//turn multisampling off
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;

	//set to full screen or windowed mode
	swapChainDesc.Windowed = !fullscreen;
	
	//set the scan line ordering and scaling to unspecified
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	//Discard the back buffer contents after presenting
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	swapChainDesc.Flags = 0;
	
	//Set the feature level to DirectX11
	featureLevel = D3D_FEATURE_LEVEL_11_0;

	//create swap chain, device, and device context
	result = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, &featureLevel, 1,
											D3D11_SDK_VERSION, &swapChainDesc, &m_swapChain, &m_device, NULL, &m_deviceContext);

	if(FAILED(result))
	{
		return false;
	}

	return true;
}

bool DXManager::InitializeDepthBuffer(int screendWidth, int screenHeight)
{
	D3D11_TEXTURE2D_DESC depthBufferDesc;
	HRESULT result;

	//Initialize depth buffer desc
	ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));

	//set up depth buffer desc
	depthBufferDesc.Width = screendWidth;
	depthBufferDesc.Height = screenHeight;
	depthBufferDesc.MipLevels = 1;
	depthBufferDesc.ArraySize = 1;
	depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthBufferDesc.SampleDesc.Count = 1;
	depthBufferDesc.SampleDesc.Quality = 0;
	depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthBufferDesc.CPUAccessFlags = 0;
	depthBufferDesc.MiscFlags = 0;

	//create the texture for the depth buffer
	result = m_device->CreateTexture2D(&depthBufferDesc, NULL, &m_depthStencilBuffer);
	if(FAILED(result))
	{
		return false;
	}

	return true;
}

bool DXManager::InitializeDepthStencilBuffer()
{
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	HRESULT result;

	//initialize depth stencil desc;
	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));

	//set up description
	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

	depthStencilDesc.StencilEnable = true;
	depthStencilDesc.StencilReadMask = 0xFF;
	depthStencilDesc.StencilWriteMask = 0xFF;

	//stencil operations if pixel if front-facing
	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	//stencil operations if pixel is back-facing
	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	//create depth stencil state
	result = m_device->CreateDepthStencilState(&depthStencilDesc, &m_depthStencilState);
	if(FAILED(result))
	{
		return false;
	}

	//set depth stencil state
	m_deviceContext->OMSetDepthStencilState(m_depthStencilState, 1);

	return true;
}

bool DXManager::InitializeStencilView()
{
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	HRESULT result;

	//Initialize description
	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

	//set up description
	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	//create depth stencil view
	result = m_device->CreateDepthStencilView(m_depthStencilBuffer, &depthStencilViewDesc, &m_depthStencilView);
	if(FAILED(result))
	{
		return false;
	}

	return true;
}

bool DXManager::InitializeRasterizerState()
{
	D3D11_RASTERIZER_DESC rasterDesc;
	HRESULT result;

	//set up description
	rasterDesc.AntialiasedLineEnable = false;
	rasterDesc.CullMode = D3D11_CULL_BACK;
	rasterDesc.DepthBias = 0;
	rasterDesc.DepthBiasClamp = 0.0f;
	rasterDesc.DepthClipEnable = true;
	rasterDesc.FillMode = D3D11_FILL_SOLID;
	rasterDesc.FrontCounterClockwise = false;
	rasterDesc.MultisampleEnable = false;
	rasterDesc.ScissorEnable = false;
	rasterDesc.SlopeScaledDepthBias = 0.0f;

	//create the rasterize state
	result = m_device->CreateRasterizerState(&rasterDesc, &m_rasterState);
	if(FAILED(result))
	{
		return false;
	}

	//set the rasterizer
	m_deviceContext->RSSetState(m_rasterState);

	return true;
}

void DXManager::InitializeViewport(int screenWidth, int screenHeight)
{
	D3D11_VIEWPORT viewport;

	//setup
	viewport.Width = (float)screenWidth;
	viewport.Height = (float)screenHeight;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;

	//create viewport
	m_deviceContext->RSSetViewports(1, &viewport);
}

bool DXManager::InitializeAlphaBlending()
{
	D3D11_BLEND_DESC blendStateDesc;
	HRESULT result;

	// Clear the blend state description.
	ZeroMemory(&blendStateDesc, sizeof(D3D11_BLEND_DESC));

	//initialize/clear description
	blendStateDesc.RenderTarget[0].BlendEnable = TRUE;
	blendStateDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blendStateDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	blendStateDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendStateDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendStateDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendStateDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendStateDesc.RenderTarget[0].RenderTargetWriteMask = 0x0f;

	//create the enabled blend state
	result = m_device->CreateBlendState(&blendStateDesc, &m_alphaEnableBlendingState);
	if(FAILED(result))
	{
		return false;
	}

	//Modify to create disabled alpha blend state
	blendStateDesc.RenderTarget[0].BlendEnable = false;

	//create the disabled blend state
	result = m_device->CreateBlendState(&blendStateDesc, &m_alphaDisableBlendingState);
	if(FAILED(result))
	{
		return false;
	}

	return true;
}

bool DXManager::InitializeZBuffer()
{
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	HRESULT result;

	//clear the description
	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));

	//set up description
	depthStencilDesc.DepthEnable = false;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

	depthStencilDesc.StencilEnable = true;
	depthStencilDesc.StencilReadMask = 0xFF;
	depthStencilDesc.StencilWriteMask = 0xFF;

	//stencil operations if pixel if front-facing
	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	//stencil operations if pixel is back-facing
	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	//create disabled state
	result = m_device->CreateDepthStencilState(&depthStencilDesc, &m_depthDisabledStencilState);
	if(FAILED(result))
	{
		return false;
	}

	return true;
}

void DXManager::EnableAlphaBlending(bool enable)
{
	float blendFactor[4];
	blendFactor[0] = 0.0f;
	blendFactor[0] = 0.0f;
	blendFactor[0] = 0.0f;
	blendFactor[0] = 0.0f;

	if(enable)
	{
		m_deviceContext->OMSetBlendState(m_alphaEnableBlendingState, blendFactor, 0xffffffff);
	}
	else
	{
		m_deviceContext->OMSetBlendState(m_alphaDisableBlendingState, blendFactor, 0xffffffff);
	}
}

void DXManager::EnableZBuffer(bool enable)
{
	if(enable)
	{
		m_deviceContext->OMSetDepthStencilState(m_depthStencilState, 1);
	}
	else
	{
		m_deviceContext->OMSetDepthStencilState(m_depthDisabledStencilState, 1);
	}
}

ID3D11Device* DXManager::GetDevice()
{
	return m_device;
}

ID3D11DeviceContext* DXManager::GetDeviceContext()
{
	return m_deviceContext;
}
