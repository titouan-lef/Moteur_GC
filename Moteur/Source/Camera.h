#pragma once

class Entity;

class Camera : public Entity
{
public:
	Camera();
	virtual ~Camera();

	static Camera* m_Instance;

	virtual void Init() override;
	virtual void Update() override;	

#pragma region Gets/Sets
	static inline XMMATRIX GetViewMatrix() { return XMLoadFloat4x4(&m_viewMatrix); }
	static inline XMMATRIX GetProjMatrix() { return XMMatrixTranspose(XMLoadFloat4x4(&m_projMatrix)); }
	static inline XMMATRIX GetViewProjMatrix() { return XMMatrixMultiply(XMLoadFloat4x4(&m_viewMatrix), XMLoadFloat4x4(&m_projMatrix)); }
#pragma endregion

protected:
	float m_fov = -1;
	float m_aspectRatio = -1;
	float m_nearPlane = -1;
	float m_farPlane = -1;

	static XMFLOAT4X4 m_viewMatrix;
	static XMFLOAT4X4 m_projMatrix;

private:

};