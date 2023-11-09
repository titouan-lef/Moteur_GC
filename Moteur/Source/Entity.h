#pragma once
#include "Transform.h"

class Component;

class Entity
{
public:
	Entity();
	virtual ~Entity();

	void Born();
	bool IsDead() { return m_isDead; }

	int m_ID;

	virtual void Init() {}			// Initialize entity and all children, called once
	virtual void Update() {}		// Update entity called every frame
	virtual void PostUpdate() {}	// Update entity called every frame after all updates

	void AddChild(Entity* child);	// Add child to entity

	template <typename T = Component>
	T* AddComponent();		// Add component to entity

	template <typename T = Component>
	T* GetComponent();		// Get component from entity

	void RealUpdate();		// Update entity, all children and components
	void Render();			// Render all children and components

	void Destroy();			// Destroy entity and all children

protected:

	Entity* m_Parent = nullptr;

	std::vector<Entity*> m_Children;
	std::vector<Component*> m_Components;

	void SetParent(Entity* parent) { m_Parent = parent; }
private:
	// Check if entity is destroyed
	bool m_isDead = false;
	// Check if entity is initialized
	bool m_isBorn = false;
	// Prevent copying parent/child or nullptr
	bool CheckAddChild(Entity* child);
	// Prevent copying components
	bool CheckAddComponent(Component* component);
};

template <typename T>
T* Entity::AddComponent()
{
	T* component = new T();
	if (!CheckAddComponent(component)) return nullptr;
	dynamic_cast<Component*>(component)->SetOwner(this);
	dynamic_cast<Component*>(component)->Initialize();
	m_Components.push_back(dynamic_cast<Component*>(component));
	dynamic_cast<Component*>(component)->SetOwner(this);
	return component;
}

template <typename T>
T* Entity::GetComponent()
{
	for (auto it = m_Components.begin(); it != m_Components.end(); ++it)
	{
		if (dynamic_cast<T*>(*it) != nullptr)
			return dynamic_cast<T*>(*it);
	}
	return nullptr;
}