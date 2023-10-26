#include "Component.h"

Component::Component()
{
	m_isDirty = false;
	m_isActive = true;
	m_entity = nullptr;
}

Component::~Component()
{
	delete m_entity;
}