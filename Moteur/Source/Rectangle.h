#pragma once 
#include "Entity.h"

class Mesh;
class Camera;

class MyRectangle : public Entity
{
public:
	MyRectangle();
	~MyRectangle();

	void Update();
	/*void Init() override;
	void Update() override;
	void PostUpdate() override;
	void Render() override;
	void OnDestroy() override;*/

private:
	static Mesh* m_mesh;

	Camera* camera = nullptr;
};