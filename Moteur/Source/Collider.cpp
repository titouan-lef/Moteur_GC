#include "Collider.h"
#include "Entity.h"
#include "Transform.h"
#include "CollisionManagrer.h"

Collider::Collider()
{
	m_isDirty = true;
	m_isActive = true;
	m_name = "Collider";
	m_owner = nullptr;
}

Collider::~Collider()
{
	CollisionManager::RemoveEntity(GetOwner());
	delete m_Collider;
}


void Collider::Initialize()
{
	m_Collider = GetOwner()->GetComponent<Transform>();
	CollisionManager::AddEntity(GetOwner());
}

void Collider::Update()
{
	// Check si l'entity est dans une zone autour du joueur
	Transform t = GetOwner()->GetComponent<Transform>();
	if (m_Collider->GetPosition().x > 100.0f || m_Collider->GetPosition().x < -100.0f ||
		m_Collider->GetPosition().y > 100.0f || m_Collider->GetPosition().y < -100.0f ||
		m_Collider->GetPosition().z > 100.0f || m_Collider->GetPosition().z < -100.0f)
	{
		m_isActive = false;
	}
	else
	{
		m_isActive = true;
	}
}

Transform* Collider::GetCollider()
{
	return m_Collider;
}

bool Collider::CheckCollision(Collider& collider1, Collider& collider2) {
	XMFLOAT3 cube1Scale = collider1.GetCollider()->GetScale();
	XMFLOAT3 cube1Min = {
		collider1.GetCollider()->GetPosition().x - cube1Scale.x / 2.0f,
		collider1.GetCollider()->GetPosition().y - cube1Scale.y / 2.0f,
		collider1.GetCollider()->GetPosition().z - cube1Scale.z / 2.0f
	};
	XMFLOAT3 cube1Max = {
		collider1.GetCollider()->GetPosition().x + cube1Scale.x / 2.0f,
		collider1.GetCollider()->GetPosition().y + cube1Scale.y / 2.0f,
		collider1.GetCollider()->GetPosition().z + cube1Scale.z / 2.0f
	};

	XMFLOAT3 cube2Scale = collider2.GetCollider()->GetScale();
	XMFLOAT3 cube2Min = {
		collider2.GetCollider()->GetPosition().x - cube2Scale.x / 2.0f,
		collider2.GetCollider()->GetPosition().y - cube2Scale.y / 2.0f,
		collider2.GetCollider()->GetPosition().z - cube2Scale.z / 2.0f
	};
	XMFLOAT3 cube2Max = {
		collider2.GetCollider()->GetPosition().x + cube2Scale.x / 2.0f,
		collider2.GetCollider()->GetPosition().y + cube2Scale.y / 2.0f,
		collider2.GetCollider()->GetPosition().z + cube2Scale.z / 2.0f
	};

	// Check sur l'axe X
	bool overlapX = (cube1Min.x <= cube2Max.x && cube1Max.x >= cube2Min.x);

	// Check sur l'axe Y
	bool overlapY = (cube1Min.y <= cube2Max.y && cube1Max.y >= cube2Min.y);

	// Check sur l'axe Z
	bool overlapZ = (cube1Min.z <= cube2Max.z && cube1Max.z >= cube2Min.z);

	return overlapX && overlapY && overlapZ;
}