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
	m_isActive = GetOwner()->GetComponent<Transform>()->IsOnScreen();
}

Transform* Collider::GetCollider()
{
	if (m_Collider != nullptr) 	return m_Collider;
	else return nullptr;
}

bool Collider::CheckCollision(Collider* collider) {
	XMFLOAT3 cube1Scale = GetOwner()->GetComponent<Transform>()->GetScale();
	XMFLOAT3 cube1Min = {
		GetOwner()->GetComponent<Transform>()->GetPosition().x - cube1Scale.x / 2.0f,
		GetOwner()->GetComponent<Transform>()->GetPosition().y - cube1Scale.y / 2.0f,
		GetOwner()->GetComponent<Transform>()->GetPosition().z - cube1Scale.z / 2.0f
	};
	XMFLOAT3 cube1Max = {
		GetOwner()->GetComponent<Transform>()->GetPosition().x + cube1Scale.x / 2.0f,
		GetOwner()->GetComponent<Transform>()->GetPosition().y + cube1Scale.y / 2.0f,
		GetOwner()->GetComponent<Transform>()->GetPosition().z + cube1Scale.z / 2.0f
	};

	XMFLOAT3 cube2Scale = collider->GetOwner()->GetComponent<Transform>()->GetScale();
	XMFLOAT3 cube2Min = {
		collider->GetOwner()->GetComponent<Transform>()->GetPosition().x - cube2Scale.x / 2.0f,
		collider->GetOwner()->GetComponent<Transform>()->GetPosition().y - cube2Scale.y / 2.0f,
		collider->GetOwner()->GetComponent<Transform>()->GetPosition().z - cube2Scale.z / 2.0f
	};
	XMFLOAT3 cube2Max = {
		collider->GetOwner()->GetComponent<Transform>()->GetPosition().x + cube2Scale.x / 2.0f,
		collider->GetOwner()->GetComponent<Transform>()->GetPosition().y + cube2Scale.y / 2.0f,
		collider->GetOwner()->GetComponent<Transform>()->GetPosition().z + cube2Scale.z / 2.0f
	};

	// Check sur l'axe X
	bool overlapX = (cube1Min.x <= cube2Max.x && cube1Max.x >= cube2Min.x);

	// Check sur l'axe Y
	bool overlapY = (cube1Min.y <= cube2Max.y && cube1Max.y >= cube2Min.y);

	// Check sur l'axe Z
	bool overlapZ = (cube1Min.z <= cube2Max.z && cube1Max.z >= cube2Min.z);

	return overlapX && overlapY && overlapZ;
}

void Collider::OnCollision(Entity* e)
{
	std::cout << "Collision" << std::endl;
}
