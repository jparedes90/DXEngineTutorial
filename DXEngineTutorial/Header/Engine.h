#ifndef _ENGINE_H
#define _ENGINE_H

#include "Graphics.h"
#include "Camera.h"
#include "Input.h"
#include "GameComponent.h"
#include "ResourceManager.h"
#include "EntityManager.h"
#include "TextureShader.h"

class Engine
{
public:
	~Engine(void);

	bool InitializeGraphics(HWND hwnd);
	bool Initialize(HINSTANCE hinstance, HWND hwnd);
	void Run();

	void Release();

	void SetGameComponent(GameComponent* gameComponent);

	Graphics* GetGraphics();
	Camera* GetCamera();
	Input* GetInput();

	static Engine* GetEngine();

private:
	Engine(void);

	void Update();
	void Render();

	Graphics* m_graphics;
	Camera* m_camera;
	Input* m_input;
	ResourceManager* m_resourceManager;
	GameComponent* m_gameComponent;

	EntityManager* m_entityManager;

	static Engine* m_instance;
};

#endif