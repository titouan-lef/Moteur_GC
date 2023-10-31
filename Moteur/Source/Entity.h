#pragma once

#include "framwork.h"
//#include "Component.h"

using namespace DirectX;

struct TRANSFORM
{
	XMFLOAT3 Scale;            // Échelle de l'entité

	XMFLOAT3 Dir;              // Vecteur de direction
	XMFLOAT3 Right;            // Vecteur de droite
	XMFLOAT3 Up;               // Vecteur vers le haut
	XMFLOAT4 RotationQuat;     // Quaternion de rotation
	XMMATRIX RotationMatrix;   // Matrice de rotation

	XMFLOAT3 Position;         // Position de l'entité

	XMFLOAT4X4 Matrix;           // Matrice de transformation

	TRANSFORM();

#pragma region Getters/Setters
	// Retourne la position
	XMFLOAT3 GetPosition() { return Position; }
	// Retourne l'echelle
	XMFLOAT3 GetScale() { return Scale; }
	// Change la position
	void SetPosition(float x, float y, float z) { Position = XMFLOAT3(x, y, z); }
	// Change l'echelle
	void SetScale(float x, float y, float z) { Scale = XMFLOAT3(x, y, z); }
	void SetScale(float mul) { Scale = XMFLOAT3(Scale.x*mul, Scale.y*mul, Scale.z*mul); }
	// Retourne la matrice de transformation
	XMMATRIX GetMatrix() { return XMLoadFloat4x4(&Matrix); }
	XMFLOAT4X4 GetMat() { return Matrix;  }// TO DO A supprimer
	XMMATRIX inline GetMatrixTranspose() { return XMMatrixTranspose(XMLoadFloat4x4(&Matrix)); }
	// Retourne la matrice de rotation
	XMMATRIX* GetRotationMatrix() { return &RotationMatrix; }
#pragma endregion

	// Déplace la position en fonction d'un vecteur
	void MoveByVector(XMFLOAT3 vec);

	// Réinitialise la transformation à l'identité
	void Identity();

	// Initialise la transformation à partir d'une matrice de transformation
	void FromMatrix(XMMATRIX* matrix);

	// Met à jour la rotation à partir des vecteurs Direction, Right, et Up
	void UpdateRotationFromVectors();

	// Met à jour la rotation à partir du quaternion RotationQuat
	void UpdateRotationFromQuaternion();

	// Met à jour la matrice de rotation à partir des vecteurs Direction, Right, et Up
	void UpdateRotationFromMatrix();

	// Met à jour la matrice de transformation en fonction de la position, de l'échelle et de la rotation
	void UpdateMatrix();

	// Effectue une rotation en utilisant des angles yaw, pitch et roll
	void Rotate(float yaw, float pitch, float roll);

	// Effectue une rotation autour de l'axe Z (yaw)
	void RotateYaw(float angle);

	// Effectue une rotation autour de l'axe Y (pitch)
	void RotatePitch(float angle);

	// Effectue une rotation autour de l'axe X (roll)
	void RotateRoll(float angle);

	// Effectue une rotation dans l'espace mondial en fonction d'une matrice de rotation
	void RotateWorld(XMMATRIX* matrix);

	// Effectue une rotation autour de l'axe X dans l'espace mondial
	void RotateWorldX(float angle);

	// Effectue une rotation autour de l'axe Y dans l'espace mondial
	void RotateWorldY(float angle);

	// Effectue une rotation autour de l'axe Z dans l'espace mondial
	void RotateWorldZ(float angle);
};


class Entity
{
public:
	Entity();
	virtual ~Entity();
	bool IsInWindow();
	TRANSFORM m_Transform;
	XMFLOAT3 m_Direction;
	//std::vector<Component*> m_Components;

	/*virtual void Init() = 0;
	virtual void Update() = 0;
	virtual void PostUpdate() = 0;
	virtual void Render() = 0;
	virtual void OnDestroy() = 0;*/

	//static void AddComponent(Entity* entity, Component* component);
protected:

private:
};