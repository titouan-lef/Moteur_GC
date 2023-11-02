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
	XMFLOAT3 inline GetPosition() { return Position; }										// Retourne la position
	XMFLOAT3 inline GetScale() { return Scale; }											// Retourne l'echelle
	bool inline IsDirty() { return m_isDirty; }												// Retourne si l'entite est en mouvement
	void inline SetPosition(float x, float y, float z) { Position = XMFLOAT3(x, y, z); }	// Change la position

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

	void MoveByVector(XMFLOAT3 vec);	// Déplace la position en fonction d'un vecteur
	void Identity();					// Réinitialise la transformation à l'identité
	void FromMatrix(XMMATRIX* matrix);	// Initialise la transformation à partir d'une matrice de transformation
	void UpdateMatrix();				// Met à jour la matrice de transformation en fonction de la position, de l'échelle et de la rotation

#pragma region Rotation
	void Rotate(float yaw, float pitch, float roll);	// Effectue une rotation en utilisant des angles yaw, pitch et roll
	void RotateRoll(float angle);						// Effectue une rotation autour de l'axe X (roll)
	void RotatePitch(float angle);						// Effectue une rotation autour de l'axe Y (pitch)
	void RotateYaw(float angle);						// Effectue une rotation autour de l'axe Z (yaw)
#pragma endregion

private:
	// Position de l'entité
	XMFLOAT3 Position;

	// Échelle de l'entité
	XMFLOAT3 Scale;

	// Rotation de l'entité
	XMFLOAT3 Dir;              // Vecteur de direction
	XMFLOAT3 Right;            // Vecteur de droite
	XMFLOAT3 Up;               // Vecteur vers le haut
	XMFLOAT4 RotationQuat;     // Quaternion de rotation

	XMFLOAT4X4 Matrix;           // Matrice de transformation
	XMFLOAT3 m_Direction;
};