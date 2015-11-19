#include "EntityManager.h"

EntityManager* EntityManager::m_instance = NULL;

EntityManager::EntityManager(void)
{
}

EntityManager::~EntityManager(void)
{
	RemoveAllEntities();
}

void EntityManager::Update()
{
	int size = (int)m_entities.size();
	for(int i = 0; i < size; ++i)
	{
		m_entities[i]->Update();
	}
}

void EntityManager::Render(ID3D11DeviceContext* deviceContex, XMFLOAT4X4 viewMatrix, XMFLOAT4X4 projectionMatrix)
{
	int size = (int)m_entities.size();
	for(int i = 0; i < size; ++i)
	{
		m_entities[i]->Render(deviceContex, viewMatrix, projectionMatrix);
	}
}

Entity* EntityManager::AddEntity()
{
	Entity* entity = new Entity();
	m_entities.push_back(entity);

	return entity;
}

void EntityManager::RemoveEntity(Entity* entity)
{
	if(entity == NULL) return;

	int index = -1;

	if(ContainsEntity(entity, index))
	{
		m_entities.erase(m_entities.begin() + index);

		delete entity;
	}
}

void EntityManager::RemoveAllEntities()
{
	while(!m_entities.empty())
	{
		Entity* entity = m_entities[0];
		m_entities.erase(m_entities.begin());

		delete entity;
	}
}

bool EntityManager::ContainsEntity(Entity* entity)
{
	int size = (int)m_entities.size();
	for(int i = 0; i < size; ++i)
	{
		if(m_entities[i] == entity)
		{
			return true;
		}
	}

	return false;
}

bool EntityManager::ContainsEntity(Entity* entity, int& index)
{
	index = -1;

	int size = (int)m_entities.size();
	for(int i = 0; i < size; ++i)
	{
		if(m_entities[i] == entity)
		{
			index = i;
			return true;
		}
	}

	return false;
}

EntityManager* EntityManager::GetInstance()
{
	if(m_instance == NULL)
	{
		m_instance = new EntityManager();
	}

	return m_instance;
}
