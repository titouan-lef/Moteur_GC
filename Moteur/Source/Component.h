#pragma once

#include "framwork.h"
#include "Entity.h"

class Component
{
public:
	Component();
	virtual ~Component();

	virtual void Initialize();

	bool inline IsActive() { return m_isActive; }

	void inline Activate() { m_isActive = true; OnActivate(); }
	void inline Deactivate() { m_isActive = false; OnDeactivate(); }

	virtual void Update();

	void inline SetOwner(Entity* entity) { m_owner = entity; }
	Entity* GetOwner() { return m_owner; }
protected:
	bool m_isDirty;
	bool m_isActive;
	Entity* m_owner;

	virtual void OnActivate();
	virtual void OnDeactivate();
};