#pragma once

#include "framwork.h"
#include "Entity.h"

class Component
{
public:
	Component();
	virtual ~Component();

	virtual void Initialize() = 0;

	bool inline IsActive() { return m_isActive; }

	void inline Activate() { m_isActive = true; OnActivate(); }
	void inline Deactivate() { m_isActive = false; OnDeactivate(); }

	virtual void Update() = 0;

	void inline SetOwner(Entity* entity) { m_owner = entity; }
	Entity* GetOwner() { return m_owner; }
protected:
	bool m_isDirty;
	bool m_isActive;
	Entity* m_owner;

	virtual void OnActivate() = 0;
	virtual void OnDeactivate() = 0;
};