#include "Component.h"

Component::Component()
{
	m_isDirty = false;
	m_isActive = true;
	m_owner = nullptr;
}

Component::~Component()
{
	delete m_owner;
}