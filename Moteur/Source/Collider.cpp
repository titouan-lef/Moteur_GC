#include "Entity.h"
#include "Transform.h"
#include "CollisionManager.h"
#include "Collider.h"

Collider::Collider()
{
	m_isDirty = true;
	m_isActive = true;
	m_name = "Collider";
	m_owner = nullptr;
	m_tag = "Default";
}

Collider::~Collider()
{
	CollisionManager::RemoveEntity(GetOwner());
	
}


void Collider::Initialize()
{
	m_Collider = GetOwner()->GetComponent<Transform>();
	CollisionManager::AddEntity(GetOwner());
}

void Collider::Update()
{
	m_isActive = GetOwner()->GetComponent<Transform>()->IsOnScreen();
}

Transform* Collider::GetCollider()
{
	if (m_Collider != nullptr) 	return m_Collider;
	else return nullptr;
}

bool Collider::CheckCollision(Collider* collider) {
	XMFLOAT3 pos1 = *GetOwner()->GetComponent<Transform>()->GetPosition();
	float radius1 = GetOwner()->GetComponent<Transform>()->GetScale()->x;
	XMFLOAT3 pos2 = *collider->GetOwner()->GetComponent<Transform>()->GetPosition();
	float radius2 = collider->GetOwner()->GetComponent<Transform>()->GetScale()->x;

	// Création de sphères avec les paramètres fournis
	BoundingSphere sphere1;
	sphere1.Center = pos1;
	sphere1.Radius = radius1;
	BoundingSphere sphere2;
	sphere2.Center = pos2;
	sphere2.Radius = radius2;

	// Utilisation de la fonction Intersects de DirectXCollision pour détecter la collision
	return sphere1.Intersects(sphere2);
}

void Collider::addListener(EventCallback callback)
{
	m_callbacks.push_back(callback);
}

void Collider::triggerEvent(Entity* e)
{
	for (auto callback : m_callbacks)
	{
		callback(e);
	}
}
