#include "Entity.h"
#include <EventSys.h>

Entity::Entity()
{
	//ctor
}

Entity::~Entity()
{
	for (auto it = m_Components.begin(); it != m_Components.end(); ++it)
	{
		delete *it;
	}
	m_Components.clear();
}

void Entity::AddComponent(Entity* entity, Component* component)
{
	entity->m_Components.push_back(component);
	component->SetEntity(entity);
}

void Entity::RealUpdate()
{
	Update();
	PostUpdate();
	for (auto it = m_Components.begin(); it != m_Components.end(); ++it)
	{
		if (!(*it)->IsActive()) continue;
		if (!(*it)->IsDirty()) continue;
		(*it)->Update();
	}
}
