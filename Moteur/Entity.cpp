#include "Entity.h"

#pragma region TRANSFORM
TRANSFORM::TRANSFORM()
{
    Scale = XMFLOAT3(1.0f, 1.0f, 1.0f);
	Dir = XMFLOAT3(0.0f, 0.0f, 1.0f);
	Right = XMFLOAT3(1.0f, 0.0f, 0.0f);
	Up = XMFLOAT3(0.0f, 1.0f, 0.0f);
    RotationQuat = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
    RotationMatrix = XMMATRIX();
    Position = XMFLOAT3(0.0f, 0.0f, 0.0f);
    Matrix = XMMATRIX();
}

void TRANSFORM::MoveByVector(XMFLOAT3 vec)
{
	Position.x += vec.x;
	Position.y += vec.y;
	Position.z += vec.z;
}

void TRANSFORM::Identity()
{
	Matrix = XMMatrixIdentity();
}

void TRANSFORM::FromMatrix(XMMATRIX* matrix)
{
    // Met � jour la matrice de transformation � partir d'une matrice de transformation
    Matrix = *matrix;
}

void TRANSFORM::UpdateRotationFromVectors()
{
    // Met � jour la matrice de rotation � partir des vecteurs de direction, de droite et de haut
	RotationMatrix.r[0] = XMVectorSet(Dir.x, Dir.y, Dir.z, 0.0f);
	RotationMatrix.r[1] = XMVectorSet(Up.x, Up.y, Up.z, 0.0f);
	RotationMatrix.r[2] = XMVectorSet(Right.x, Right.y, Right.z, 0.0f);
	RotationMatrix.r[3] = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
}

void TRANSFORM::UpdateRotationFromQuaternion()
{
    // Met � jour la matrice de rotation � partir du quaternion de rotation
	RotationMatrix = XMMatrixRotationQuaternion(XMLoadFloat4(&RotationQuat));
}

void TRANSFORM::UpdateRotationFromMatrix()
{
	// Met � jour la matrice de rotation � partir de la matrice de transformation
	RotationMatrix.r[0] = Matrix.r[0];
	RotationMatrix.r[1] = Matrix.r[1];
	RotationMatrix.r[2] = Matrix.r[2];
	RotationMatrix.r[3] = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
}

void TRANSFORM::UpdateMatrix()
{
	// Met � jour la matrice de transformation � partir des vecteurs de direction, de droite et de haut
	Matrix.r[0] = XMVectorSet(Dir.x, Dir.y, Dir.z, 0.0f);
	Matrix.r[1] = XMVectorSet(Up.x, Up.y, Up.z, 0.0f);
	Matrix.r[2] = XMVectorSet(Right.x, Right.y, Right.z, 0.0f);
	Matrix.r[3] = XMVectorSet(Position.x, Position.y, Position.z, 1.0f);
}

void TRANSFORM::Rotate(float yaw, float pitch, float roll)
{
	// Effectue une rotation � partir des angles yaw, pitch et roll
	XMMATRIX rotation = XMMatrixRotationRollPitchYaw(pitch, yaw, roll);
	RotationMatrix = XMMatrixMultiply(rotation, RotationMatrix);
}

void TRANSFORM::RotateYaw(float angle)
{
	// Effectue une rotation autour de l'axe Z
	XMMATRIX rotation = XMMatrixRotationZ(angle);
	RotationMatrix = XMMatrixMultiply(rotation, RotationMatrix);
}

void TRANSFORM::RotatePitch(float angle)
{
	// Effectue une rotation autour de l'axe Y
	XMMATRIX rotation = XMMatrixRotationY(angle);
	RotationMatrix = XMMatrixMultiply(rotation, RotationMatrix);
}

void TRANSFORM::RotateRoll(float angle)
{
	// Effectue une rotation autour de l'axe X
	XMMATRIX rotation = XMMatrixRotationX(angle);
	RotationMatrix = XMMatrixMultiply(rotation, RotationMatrix);
}

void TRANSFORM::RotateWorld(XMMATRIX* matrix)
{
	// Effectue une rotation dans le rep�re monde
	RotationMatrix = XMMatrixMultiply(*matrix, RotationMatrix);
}

void TRANSFORM::RotateWorldX(float angle)
{
	// Effectue une rotation autour de l'axe X dans le rep�re monde
	XMMATRIX rotation = XMMatrixRotationX(angle);
	RotationMatrix = XMMatrixMultiply(rotation, RotationMatrix);
}

void TRANSFORM::RotateWorldY(float angle)
{
	// Effectue une rotation autour de l'axe Y dans le rep�re monde
	XMMATRIX rotation = XMMatrixRotationY(angle);
	RotationMatrix = XMMatrixMultiply(rotation, RotationMatrix);
}

void TRANSFORM::RotateWorldZ(float angle)
{
	// Effectue une rotation autour de l'axe Z dans le rep�re monde
	XMMATRIX rotation = XMMatrixRotationZ(angle);
	RotationMatrix = XMMatrixMultiply(rotation, RotationMatrix);
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
