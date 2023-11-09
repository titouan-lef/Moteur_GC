#include "framwork.h"
#include "Entity.h"
#include "Scene.h"
#include "CollisionManager.h"
#include "Collider.h"

Scene::~Scene()
{
	for (auto& entity : m_entities)
	{
		delete entity;
	}
}

void Scene::RealUpdate()
{
	if (!m_Initialized)
	{
		this->Init();
		m_Initialized = true;
	}

	for (auto& entity : m_entities)
	{
		entity->RealUpdate();
	}
	Update();
}

void Scene::Render()
{
	for (auto& entity : m_entities)
	{
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

void Scene::KillThemAll()
{
	for (auto it = m_entities.begin(); it != m_entities.end(); ++it)
	{
		if ((*it)->IsDead())
		{
			m_entities.erase(m_entities.begin());
			if ((*it)->GetComponent<Collider>() != nullptr)
				CollisionManager::GetInstance()->RemoveEntity((*it));
			delete (*it);
		}
	}
}
