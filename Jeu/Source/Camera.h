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
	inline XMMATRIX GetProjMatrix() { return XMLoadFloat4x4(&m_projMatrix); }
	inline XMMATRIX GetViewProjMatrix() { return XMMatrixMultiply(XMLoadFloat4x4(&m_viewMatrix), XMLoadFloat4x4(&m_projMatrix)); }
#pragma endregion

	static XMFLOAT4X4 m_projMatrix;
protected:
	float m_fov = -1;
	float m_aspectRatio = -1;
	float m_nearPlane = -1;
	float m_farPlane = -1;

	//Transform* m_transform = nullptr;

	static XMFLOAT4X4 m_viewMatrix;

private:

};