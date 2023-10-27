#pragma once

#include "framwork.h"

class Component;

class Entity
{
public:
	Entity();
	virtual ~Entity();

	int m_ID;

	std::vector<Entity*> m_Children;
	std::vector<Component*> m_Components;

	virtual void Init();
	virtual void Update();
	virtual void PostUpdate();
	virtual void Render();
	virtual void OnDestroy();

	void AddChild(Entity* child);
	void AddComponent(Component* component);

	void RealUpdate();
protected:
	Entity* m_Parent;

	void SetParent(Entity* parent) { m_Parent = parent; }
private:
	// Prevent copying parent/child or nullptr
	bool CheckAddChild(Entity* child);
	// Prevent copying components
	bool CheckAddComponent(Component* component);
};