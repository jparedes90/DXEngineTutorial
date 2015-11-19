#include "Engine.h"

Engine* Engine::m_instance = NULL;

Engine::Engine(void)
{
	m_graphics = NULL;
	m_camera = NULL;
	m_resourceManager = NULL;
	m_entityManager = NULL;
	m_input = NULL;
	m_gameComponent = NULL;
}

Engine::~Engine(void)
{
	if(m_graphics)
	{
		delete m_graphics;
		m_graphics = NULL;
	}

	if(m_camera)
	{
		delete m_camera;
		m_camera = NULL;
	}

	if(m_resourceManager)
	{
		delete m_resourceManager;
		m_resourceManager = NULL;
	}

	if(m_entityManager)
	{
		delete m_entityManager;
		m_entityManager = NULL;
	}

	if(m_input)
	{
		delete m_input;
		m_input = NULL;
	}

	if(m_gameComponent)
	{
		delete m_gameComponent;
		m_gameComponent = NULL;
	}
}

bool Engine::InitializeGraphics(HWND hwnd)
{
	m_graphics = new Graphics();

	return m_graphics->InitializeDX(hwnd);
}

bool Engine::Initialize(HINSTANCE hinstance, HWND hwnd)
{
	m_entityManager = EntityManager::GetInstance();

	m_resourceManager = ResourceManager::GetInstance();
	m_resourceManager->LoadTextureResource(m_graphics->GetDevice(), L"Textures/mario-shell-sprite.png");
	m_resourceManager->LoadTextureResource(m_graphics->GetDevice(), L"Textures/pikachu-walking.png");
	m_resourceManager->LoadShaderResource(new TextureShader(m_graphics->GetDevice(), hwnd, L"Shaders/texture.fx"));

	m_camera = new Camera();
	m_camera->InitializeProjectionMatrix((float)XM_PI/4.0f, (float)SCREEN_WIDTH/(float)SCREEN_HEIGHT, SCREEN_NEAR, SCREEN_DEPTH);
	m_camera->InitializeOthoMatrix(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_NEAR, SCREEN_DEPTH);
	m_camera->SetPosition(0.0f, 0.0f, -1.0f);

	m_graphics->Initialize();

	m_input = new Input();
	m_input->Initialize(hinstance, hwnd, SCREEN_WIDTH, SCREEN_HEIGHT);
	//m_input->Update();

	if(m_gameComponent != NULL)
	{
		if(!m_gameComponent->Initialize())
		{
			return false;
		}
	}
	else
	{
		cout << "NO GAME COMPONENT" << endl;
	}

	return true;
}

void Engine::Run()
{
	Update();
	Render();
}

void Engine::Update()
{
	if(m_gameComponent != NULL)
	{
		m_gameComponent->Update();
	}
	m_entityManager->Update();
	m_input->Update();
}

void Engine::Render()
{
	m_camera->Update();

	m_graphics->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	//render stuff goes here

	XMFLOAT4X4 viewMatrix = m_camera->GetViewMatrix();
	XMFLOAT4X4 projMatrix = m_camera->GetOrthoMatrix();

	//XMFLOAT4X4 worldMatrix;
	//D3DXMatrixIdentity(&worldMatrix);

	if(m_gameComponent != NULL)
	{
		m_gameComponent->Render(m_graphics->GetDeviceContext(), viewMatrix, projMatrix);
	}

	m_entityManager->Render(m_graphics->GetDeviceContext(), viewMatrix, projMatrix);

	m_graphics->EndScene();
}

Engine* Engine::GetEngine()
{
	if(m_instance == NULL)
	{
		m_instance = new Engine();
	}

	return m_instance;
}

void Engine::Release()
{
	if(m_instance)
	{
		delete m_instance;
		m_instance = NULL;
	}
}

Graphics* Engine::GetGraphics()
{
	return m_graphics;
}

Input* Engine::GetInput()
{
	return m_input;
}

void Engine::SetGameComponent(GameComponent* gameComponent)
{
	m_gameComponent = gameComponent;
}
