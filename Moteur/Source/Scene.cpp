#include "framwork.h"
#include "Entity.h"
#include "Scene.h"

Scene::~Scene()
{
	for (auto& entity : m_entities)
	{
		delete entity;
	}
}

void Scene::RealUpdate()
{
	for (auto& entity : m_entities)
	{
		entity->RealUpdate();
		entity->Render();
	}
}

void Scene::AddEntity(Entity* entity)
{
	m_entities.push_back(entity);
}

void Scene::RemoveEntity(Entity* entity)
{
	for (auto it = m_entities.begin(); it != m_entities.end(); ++it)
	{
		if (*it == entity)
		{
			m_entities.erase(it);
			return;
		}
	}
}
