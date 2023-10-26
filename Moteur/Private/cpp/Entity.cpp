#include "../header/Entity.h"

#pragma region TRANSFORM
TRANSFORM::TRANSFORM()
{
    Scale = XMFLOAT3(1.0f, 1.0f, 1.0f);
	Dir = XMFLOAT3(0.0f, 0.0f, 1.0f);
	Right = XMFLOAT3(1.0f, 0.0f, 0.0f);
	Up = XMFLOAT3(0.0f, 1.0f, 0.0f);
    RotationQuat = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
    Position = XMFLOAT3(0.0f, 0.0f, 0.0f);
    Matrix = XMFLOAT4X4();
}

void TRANSFORM::MoveByVector(XMFLOAT3 vec)
{
	Position.x += vec.x;
	Position.y += vec.y;
	Position.z += vec.z;
}

void TRANSFORM::Identity()
{
	XMStoreFloat4x4(&Matrix, XMMatrixIdentity());
}

void TRANSFORM::FromMatrix(XMMATRIX* matrix)
{
    // Met à jour la matrice de transformation à partir d'une matrice de transformation
    XMStoreFloat4x4(&Matrix, *matrix);
}

void TRANSFORM::UpdateMatrix()
{
	XMStoreFloat4x4(&Matrix, XMMatrixAffineTransformation(XMLoadFloat3(&Scale), XMVectorZero(), XMLoadFloat4(&RotationQuat), XMLoadFloat3(&Position)));
}

void TRANSFORM::Rotate(float pitch, float yaw, float roll)
{
	// Effectue une rotation à partir des angles yaw, pitch et roll
	XMStoreFloat4(&RotationQuat, XMQuaternionRotationRollPitchYaw(pitch, yaw, roll));
}

void TRANSFORM::RotateYaw(float angle)
{
	// Effectue une rotation autour de l'axe Z
	XMStoreFloat4(&RotationQuat, XMQuaternionRotationAxis(XMLoadFloat3(&Dir), angle));
}

void TRANSFORM::RotatePitch(float angle)
{
	// Effectue une rotation autour de l'axe Y
	XMStoreFloat4(&RotationQuat, XMQuaternionRotationAxis(XMLoadFloat3(&Right), angle));
}

void TRANSFORM::RotateRoll(float angle)
{
	// Effectue une rotation autour de l'axe X
	XMStoreFloat4(&RotationQuat, XMQuaternionRotationAxis(XMLoadFloat3(&Up), angle));
}
#pragma endregion

#pragma region Entity
Entity::Entity()
{
	//ctor
}

Entity::~Entity()
{
	/*for (auto it = m_Components.begin(); it != m_Components.end(); ++it)
	{
		delete *it;
	}
	m_Components.clear();*/
}

//void Entity::AddComponent(Entity* entity, Component* component)
//{
//	entity->m_Components.push_back(component);
//	component->SetEntity(entity);
//}
#pragma endregion
