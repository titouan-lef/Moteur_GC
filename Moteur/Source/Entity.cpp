#include "framwork.h"
#include "Component.h"
#include "Entity.h"

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

void Entity::AddChild(Entity* child)
{
	if (!CheckAddChild(child)) return;
	this->m_Children.push_back(child);
	child->SetParent(this);
}

void Entity::AddComponent(Component* component)
{
	if (CheckAddComponent(component)) return;
	this->m_Components.push_back(component);
	component->SetOwner(this);
}

void Entity::RealUpdate()
{
	Update();
	PostUpdate();
	for (auto it = m_Components.begin(); it != m_Components.end(); ++it)
	{
		if ((*it)->IsActive())
			(*it)->Update();
	}
}

bool Entity::CheckAddChild(Entity* child)
{
	if (child == nullptr)
	{
		MessageBox(NULL, L"Child is nullptr", L"Error", MB_OK);
		return false;
	}
	if (child->m_Parent != nullptr)
	{
		MessageBox(NULL, L"Child already has a parent", L"Error", MB_OK);
		return false;
	}
	return true;
}

bool Entity::CheckAddComponent(Component* component)
{
	if (component == nullptr)
	{
		MessageBox(NULL, L"Component is nullptr", L"Error", MB_OK);
		return false;
	}
	if (component->GetOwner() != nullptr)
	{
		MessageBox(NULL, L"Component already has an owner", L"Error", MB_OK);
		return false;
	}
	return true;
}
