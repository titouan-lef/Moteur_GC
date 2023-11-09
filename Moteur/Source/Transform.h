#pragma once

#include "Component.h"

using namespace DirectX;

class Transform : public Component
{
public:
	Transform();
	virtual ~Transform() {}

	virtual void Update() override;

#pragma region Getters/Setters
	XMFLOAT3* GetPosition() { return &Position; }										// Retourne la position
	XMFLOAT3* GetScale() { return &Scale; }											// Retourne l'echelle
	bool IsDirty() { return m_isDirty; }												// Retourne si l'entite est en mouvement
	void SetPosition(float x, float y, float z) { Position = XMFLOAT3(x, y, z); }	// Change la position

	// Change l'echelle
	void inline SetScale(float x, float y, float z) { Scale = XMFLOAT3(x, y, z); }
	void inline SetScale(float mod) { Scale = XMFLOAT3(Scale.x * mod, Scale.y * mod, Scale.z * mod); }

	// Retourne la matrice de transformation
	XMMATRIX inline GetMatrix() { return XMLoadFloat4x4(&Matrix); }
	XMFLOAT4X4 inline GetRotMatrix() { return RotationMatrix; }
	XMMATRIX inline GetMatrixTranspose() { return XMMatrixTranspose(XMLoadFloat4x4(&Matrix)); }
	bool inline IsOnScreen() { return m_isOnScreen; }
#pragma endregion

	void MoveByVector(XMFLOAT3 vec, float elapsed);	// D�place la position en fonction d'un vecteur
	void FromMatrix(XMMATRIX* matrix);				// Initialise la transformation � partir d'une matrice de transformation
	void UpdateMatrix();							// Met � jour la matrice de transformation en fonction de la position, de l'�chelle et de la rotation

#pragma region Rotation

	void Rotate(float yaw, float pitch, float roll);	// Effectue une rotation en utilisant des angles yaw, pitch et roll
	void RotateRoll(float angle);						// Effectue une rotation autour de l'axe X (roll)
	void RotatePitch(float angle);						// Effectue une rotation autour de l'axe Y (pitch)
	void RotateYaw(float angle);	
	// Effectue une rotation autour de l'axe Z (yaw)
	static void ChangeDirection(Transform* transform);

	XMFLOAT3 GetDirection() { return m_Direction; }
	void SetDirection(float x, float y, float z) {
		m_Direction = XMFLOAT3(x, y, z);
	};
#pragma endregion

private:
	bool m_isOnScreen = true;
	void CheckIfOnScreen();

	// Direction de l'entit�
	XMFLOAT3 m_Direction;
	// Position de l'entit�
	XMFLOAT3 Position;

	// �chelle de l'entit�
	XMFLOAT3 Scale;

	// Rotation de l'entit�
	XMFLOAT3 Dir;				// Vecteur de direction
	XMFLOAT3 Right;				// Vecteur de droite
	XMFLOAT3 Up;				// Vecteur vers le haut
	XMFLOAT4 RotationQuat;		// Quaternion de rotation
	XMFLOAT4X4 RotationMatrix;	// Matrice de rotation

	void UpdateRotMatrix();		// Met � jour la matrice de rotation

	XMFLOAT4X4 Matrix;           // Matrice de transformation

};