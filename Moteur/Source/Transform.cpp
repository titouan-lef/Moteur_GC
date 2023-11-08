#include "Transform.h"
#include "Camera.h"
#include "Engine.h"

Transform::Transform()
{
	Scale = XMFLOAT3(1.0f, 1.0f, 1.0f);
	Dir = XMFLOAT3(0.0f, 0.0f, 1.0f);
	Right = XMFLOAT3(1.0f, 0.0f, 0.0f);
	Up = XMFLOAT3(0.0f, 1.0f, 0.0f);
	RotationQuat = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	Position = XMFLOAT3(0.0f, 0.0f, 0.0f);
	Matrix = XMFLOAT4X4();
	m_isDirty = true;
	m_isActive = true;
	m_name = "Transform";
	m_owner = nullptr;
	this->UpdateMatrix();
}

Transform::~Transform()
{
	delete m_owner;
}

void Transform::Update()
{
	if (m_isDirty)
	{
		UpdateMatrix();
		UpdateRotMatrix();
		m_isDirty = false;
	}

	// vector Direction
	//_31 = Position.x;
	//_32 = Position.y;
	//_33 = Position.z;
}

void Transform::MoveByVector(XMFLOAT3 vec, float elapsed)
{
	Position.x += vec.x * elapsed;
	Position.y += vec.y * elapsed;
	Position.z += vec.z * elapsed;
	m_isDirty = true;
}
void Transform::Identity()
{
	XMStoreFloat4x4(&Matrix, XMMatrixIdentity());
}

void Transform::FromMatrix(XMMATRIX* matrix)
{
	// Met à jour la matrice de Transformation à partir d'une matrice de Transformation
	XMStoreFloat4x4(&Matrix, *matrix);
}

void Transform::UpdateMatrix()
{
	XMStoreFloat4x4(&Matrix, XMMatrixAffineTransformation(XMLoadFloat3(&Scale), XMVectorZero(), XMLoadFloat4(&RotationQuat), XMLoadFloat3(&Position)));
}

void Transform::Rotate(float pitch, float yaw, float roll)
{
	// Effectue une rotation à partir des angles yaw, pitch et roll à partir du quaternion actuelle
	XMMATRIX rotation = XMMatrixRotationRollPitchYaw(pitch, yaw, roll);
	XMStoreFloat4(&RotationQuat, XMQuaternionMultiply(XMLoadFloat4(&RotationQuat), XMQuaternionRotationMatrix(rotation)));
	m_isDirty = true;
}

void Transform::RotateYaw(float angle)
{
	// Effectue une rotation autour de l'axe Z
	XMStoreFloat4(&RotationQuat, XMQuaternionRotationAxis(XMLoadFloat3(&Dir), angle));
	m_isDirty = true;
}


void Transform::ChangeDirection(Transform* transform) {
	XMFLOAT3 direction = transform->GetDirection();
	direction.x = -direction.x;
	direction.y = -direction.y;
	direction.z = -direction.z;
	transform->SetDirection(direction.x, direction.y, direction.z);
}

void Transform::CheckIfOnScreen()
{
	// Supposons que gWorld, gView et gProjection sont les matrices World, View et Projection
	XMMATRIX worldViewProjection = XMMatrixMultiply(GetMatrix(), XMMatrixMultiply(Camera::GetInstance()->GetViewMatrix(), Camera::GetInstance()->GetProjMatrix()));

	for (int i = 0; i < 8; i++)
	{
		// Coordonnées du point en espace local (objet)
		XMFLOAT3 pointInLocalSpace;
		switch (i)
		{
			case 0:
				pointInLocalSpace = XMFLOAT3(-1.0f, -1.0f, -1.0f);
				break;
			case 1:
				pointInLocalSpace = XMFLOAT3(1.0f, 1.0f, -1.0f);
				break;
			case 2:
				pointInLocalSpace = XMFLOAT3(1.0f, -1.0f, -1.0f);
				break;
			case 3:
				pointInLocalSpace = XMFLOAT3(-1.0f, 1.0f, -1.0f);
				break;
			case 4:
				pointInLocalSpace = XMFLOAT3(-1.0f, -1.0f, 1.0f);
				break;
			case 5:
				pointInLocalSpace = XMFLOAT3(1.0f, 1.0f, 1.0f);
				break;
			case 6:
				pointInLocalSpace = XMFLOAT3(1.0f, -1.0f, 1.0f);
				break;
			case 7:
				pointInLocalSpace = XMFLOAT3(-1.0f, 1.0f, 1.0f);
				break;
			default:
				break;
		}

		// Transformez les coordonnées du point en espace écran
		XMVECTOR pointInScreenSpace = XMVector2TransformCoord(XMLoadFloat3(&pointInLocalSpace), worldViewProjection);

		// Convertissez les coordonnées normalisées en coordonnées écran
		float xScreen = (pointInScreenSpace.m128_f32[0] + 1.0f) * 0.5f * Engine::GetInstance()->GetWindowSize().x;  // screenWidth est la largeur de la fenêtre
		float yScreen = (1.0f - pointInScreenSpace.m128_f32[1]) * 0.5f * Engine::GetInstance()->GetWindowSize().y; // screenHeight est la hauteur de la fenêtre

		// Vérifiez si le point est dans la fenêtre d'affichage
		if (xScreen >= -1 && xScreen < 1 && yScreen >= -1 && yScreen < 1)
		{
			m_isOnScreen = true;
			return;
		}
		else
		{
			m_isOnScreen = false;
		}
	}
}

void Transform::UpdateRotMatrix()
{
	XMStoreFloat4x4(&RotationMatrix, XMMatrixRotationQuaternion(XMLoadFloat4(&RotationQuat)));
}

void Transform::RotatePitch(float angle)
{
	// Effectue une rotation autour de l'axe Y
	XMStoreFloat4(&RotationQuat, XMQuaternionRotationAxis(XMLoadFloat3(&Right), angle));
	m_isDirty = true;
}

void Transform::RotateRoll(float angle)
{
	// Effectue une rotation autour de l'axe X
	XMStoreFloat4(&RotationQuat, XMQuaternionRotationAxis(XMLoadFloat3(&Up), angle));
	m_isDirty = true;
}