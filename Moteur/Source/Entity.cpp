#include "Component.h"
#include "Engine.h"
#include "Entity.h"
#include "Collider.h"
#include "Scene.h"
#include "CollisionManager.h"

Entity::Entity()
{
	m_ID = rand() % 1000000;
}

Entity::~Entity()
{
	for (auto it : m_Components)
	{
		delete it;
	}
	for (auto it : m_Children)
	{
		delete it;
	}
}

void Entity::Born()
{
	Init();
	for (auto it = m_Components.begin(); it != m_Components.end(); ++it)
	{
		(*it)->Initialize();
	}
	for (auto it = m_Children.begin(); it != m_Children.end(); ++it)
	{
		(*it)->Born();
	}
	m_isBorn = true;
}

void Entity::Render()
{
	for (auto it = m_Children.begin(); it != m_Children.end(); ++it)
	{
		(*it)->Render();
	}
	for (auto it = m_Components.begin(); it != m_Components.end(); ++it)
	{
		if ((*it)->IsActive())
			(*it)->Render();
	}
}

void Entity::Destroy()
{
	for (auto it = m_Children.begin(); it != m_Children.end(); ++it)
	{
		(*it)->Destroy();
	}

	m_isDead = true;
}

void Entity::AddChild(Entity* child)
{
	if (!CheckAddChild(child)) return;
	this->m_Children.push_back(child);
	child->SetParent(this);
}

void Entity::RemoveChild(Entity* child)
{
	for (auto it = m_Children.begin(); it != m_Children.end(); ++it)
	{
		if (*it == child)
		{
			m_Children.erase(it);
			return;
		}
	}
}

void Entity::RealUpdate()
{
	if (!m_isBorn)
	{
		Born();
	}

	Update();
	for (auto it = m_Components.begin(); it != m_Components.end(); ++it)
	{
		if ((*it)->IsActive())
			(*it)->Update();
	}
	for (auto it = m_Children.begin(); it != m_Children.end(); ++it)
	{
		(*it)->RealUpdate();
	}
	PostUpdate();
	MurderChildren();
}

void Entity::MurderChildren()
{
	if (m_Children.empty()) return;
	for (int i = (int)m_Children.size()-1; i > 0; i--)
	{
		if (m_Children[i] == nullptr) continue;
		if (!m_Children[i]->IsDead()) continue;
		if (m_Children[i]->GetComponent<Collider>() != nullptr)
			CollisionManager::GetInstance()->RemoveEntity(m_Children[i]);
		delete m_Children[i];
		m_Children.erase(m_Children.begin() + i);
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
	// Check if component is nullptr
	if (component == nullptr)
	{
		MessageBox(NULL, L"Component is nullptr", L"Error", MB_OK);
		return false;
	}

	//Check if there is already a component of the same type
	for (Component* comp : m_Components)
	{
		if (typeid(*comp) == typeid(*component))
		{
			MessageBox(NULL, L"Component already exists", L"Error", MB_OK);
			return false;
		}
	}
	return true;
}