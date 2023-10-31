#pragma once 
#include "Entity.h"

struct Vertex;

class MyRectangle : public Entity
{
public:
	MyRectangle();
	~MyRectangle();

	void Init() override;
	void Update() override;
	void PostUpdate() override;
	void Render() override;
	void OnDestroy() override;

private:
	static std::vector<Vertex> m_vertices;
	static std::vector<UINT16> m_indices;
};