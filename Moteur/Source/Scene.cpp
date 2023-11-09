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

	CollisionManager::GetInstance()->Update();

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
	///faire avec int i = 0; i < m_entities.size(); i++ et delete m_entities[i]
	if (m_entities.empty()) return;
	
	for (int i = m_entities.size() - 1; i >= 0; i--)
	{
		if (m_entities[i] == nullptr) continue;
		if (!m_entities[i]->IsDead()) continue;
		if (m_entities[i]->GetComponent<Collider>() != nullptr)
			CollisionManager::GetInstance()->RemoveEntity(m_entities[i]);
		delete m_entities[i];
		m_entities.erase(m_entities.begin() + i);
	}
}
