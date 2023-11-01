#pragma once

#include "Component.h"

using namespace DirectX;

class Transform : public Component
{
public:
	Transform();
	virtual ~Transform();

	virtual void Update();

#pragma region Getters/Setters
	// Retourne la position
	XMFLOAT3 inline GetPosition() { return Position; }
	// Retourne l'echelle
	XMFLOAT3 inline GetScale() { return Scale; }
	// Retourne si l'entite est en mouvement
	bool inline IsDirty() { return m_isDirty; }
	// Change la position
	void inline SetPosition(float x, float y, float z) { Position = XMFLOAT3(x, y, z); }
	// Change l'echelle
	void inline SetScale(float x, float y, float z) { Scale = XMFLOAT3(x, y, z); }
	void inline SetScale(float mod) { Scale = XMFLOAT3(Scale.x * mod, Scale.y * mod, Scale.z * mod); }
	// Retourne la matrice de transformation
	XMMATRIX inline GetMatrix() { return XMLoadFloat4x4(&Matrix); }
	XMMATRIX inline GetMatrixTranspose() { return XMMatrixTranspose(XMLoadFloat4x4(&Matrix)); }
	XMFLOAT3 GetDirection() { return m_Direction; }
	void SetDirection(float x, float y, float z) {
		m_Direction = XMFLOAT3(x, y, z);
	};
#pragma endregion

	// Déplace la position en fonction d'un vecteur
	void MoveByVector(XMFLOAT3 vec);

	// Réinitialise la transformation à l'identité
	void Identity();

	// Initialise la transformation à partir d'une matrice de transformation
	void FromMatrix(XMMATRIX* matrix);

	// Met à jour la matrice de transformation en fonction de la position, de l'échelle et de la rotation
	void UpdateMatrix();

#pragma region Rotation
	// Effectue une rotation en utilisant des angles yaw, pitch et roll
	void Rotate(float yaw, float pitch, float roll);

	// Effectue une rotation autour de l'axe X (roll)
	void RotateRoll(float angle);

	// Effectue une rotation autour de l'axe Y (pitch)
	void RotatePitch(float angle);

	// Effectue une rotation autour de l'axe Z (yaw)
	void RotateYaw(float angle);
#pragma endregion

private:
	XMFLOAT3 Scale;            // Échelle de l'entité

	XMFLOAT3 Dir;              // Vecteur de direction
	XMFLOAT3 Right;            // Vecteur de droite
	XMFLOAT3 Up;               // Vecteur vers le haut
	XMFLOAT4 RotationQuat;     // Quaternion de rotation

	XMFLOAT3 Position;         // Position de l'entité

	XMFLOAT4X4 Matrix;           // Matrice de transformation
	XMFLOAT3 m_Direction;
};