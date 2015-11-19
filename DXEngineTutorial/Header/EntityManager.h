#ifndef _ENTITY_MANAGER_H
#define _ENTITY_MANAGER_H

#include "Entity.h"

#include <vector>
using namespace std;

class EntityManager
{
public:
	~EntityManager(void);

	void Update();
	void Render(ID3D11DeviceContext* deviceContex, XMFLOAT4X4 viewMatrix, XMFLOAT4X4 projectionMatrix);

	Entity* AddEntity();
	void RemoveEntity(Entity* entity);
	void RemoveAllEntities();

	bool ContainsEntity(Entity* entity);
	bool ContainsEntity(Entity* entity, int& index);

	static EntityManager* GetInstance();

private:
	EntityManager(void);

	vector<Entity*> m_entities;
	static EntityManager* m_instance;
};

#endif